; Lane h3: IMA ADPCM block decoder, hand assembly (TASM). Object 0046cdbc..0046ceec.
; The routine saves ES/GS and copies DS into both, so it is assembly-origin, not compiler output.
; Names are working labels; the state layout is the LinkDecoderState of the C reconstruction.
.486
.model flat

STATE_SOURCE      EQU 0
STATE_DEST        EQU 4
STATE_SAMPLE      EQU 10h
STATE_PREDICTOR   EQU 14h
STATE_DIFFERENCE  EQU 18h
STATE_ENCODED     EQU 1Ch
STATE_NIBBLE      EQU 1Eh
STATE_STEP        EQU 20h
STATE_STEP_INDEX  EQU 22h
STATE_FORMAT      EQU 24h

decode_state EQU 8
decode_count EQU 12

EXTRN decodeIndexDelta:WORD ; oracle target 004A3E10; external data, not embedded code
EXTRN decodeStepSize:WORD ; oracle target 004A3E30; external data, not embedded code
EXTRN decodeRemaining:DWORD ; oracle target 004A3EE2; external data, not embedded code
EXTRN decodeOriginalCount:DWORD ; oracle target 004A3EE6; external data, not embedded code

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC decode_link_samples

; Decode count samples (count/2 for 16-bit output) from state->source to state->destination.
decode_link_samples PROC NEAR
    enter 0, 0
    push es
    push gs
    push esi
    push edi
    push ebx
    push ecx
    push edx
    mov ebx, dword ptr [ebp + decode_state]
    mov eax, dword ptr [ebp + decode_count]
    mov decodeOriginalCount, eax
    cmp word ptr [ebx + STATE_FORMAT], 10h
    jne decode_store_count
    shr eax, 1
decode_store_count:
    mov decodeRemaining, eax
    mov esi, dword ptr [ebx + STATE_SOURCE]
    mov edi, dword ptr [ebx + STATE_DEST]
    push ds
    pop es
    push ds
    pop gs
decode_next:
    test dword ptr [ebx + STATE_SAMPLE], 1
    je decode_low_nibble
    movzx eax, word ptr [ebx + STATE_ENCODED]
    shr eax, 4
    and eax, 0Fh
    mov word ptr [ebx + STATE_NIBBLE], ax
    jmp decode_difference
decode_low_nibble:
    xor eax, eax
    mov al, byte ptr gs:[esi]
    mov word ptr [ebx + STATE_ENCODED], ax
    inc esi
    and eax, 0Fh
    mov word ptr [ebx + STATE_NIBBLE], ax
decode_difference:
    mov dword ptr [ebx + STATE_DIFFERENCE], 0
    movzx ecx, word ptr [ebx + STATE_STEP]
    test eax, 4
    je decode_bit1
    add dword ptr [ebx + STATE_DIFFERENCE], ecx
decode_bit1:
    test eax, 2
    je decode_bit0
    mov edx, ecx
    shr edx, 1
    add dword ptr [ebx + STATE_DIFFERENCE], edx
decode_bit0:
    test eax, 1
    je decode_eighth
    mov edx, ecx
    shr edx, 2
    add dword ptr [ebx + STATE_DIFFERENCE], edx
decode_eighth:
    mov edx, ecx
    shr edx, 3
    add dword ptr [ebx + STATE_DIFFERENCE], edx
    test eax, 8
    je decode_predict
    neg dword ptr [ebx + STATE_DIFFERENCE]
decode_predict:
    mov eax, dword ptr [ebx + STATE_PREDICTOR]
    add eax, dword ptr [ebx + STATE_DIFFERENCE]
    cmp eax, 7FFFh
    jl decode_not_high
    mov eax, 7FFFh
decode_not_high:
    cmp eax, -8000h
    jg decode_not_low
    mov eax, -8000h
decode_not_low:
    mov dword ptr [ebx + STATE_PREDICTOR], eax
    cmp word ptr [ebx + STATE_FORMAT], 10h
    jne decode_store_byte
    stosw
    jmp decode_step
decode_store_byte:
    xor ah, 80h
    mov al, ah
    stosb
decode_step:
    movzx ecx, word ptr [ebx + STATE_NIBBLE]
    movzx eax, word ptr [ecx*2 + decodeIndexDelta]
    add word ptr [ebx + STATE_STEP_INDEX], ax
    cmp word ptr [ebx + STATE_STEP_INDEX], 8000h
    jb decode_index_high
    mov word ptr [ebx + STATE_STEP_INDEX], 0
    jmp decode_new_step
decode_index_high:
    cmp word ptr [ebx + STATE_STEP_INDEX], 58h
    jbe decode_new_step
    mov word ptr [ebx + STATE_STEP_INDEX], 58h
decode_new_step:
    movzx ecx, word ptr [ebx + STATE_STEP_INDEX]
    movzx eax, word ptr [ecx*2 + decodeStepSize]
    add dword ptr [ebx + STATE_SAMPLE], 1
    mov word ptr [ebx + STATE_STEP], ax
    dec decodeRemaining
    jne decode_next
    mov dword ptr [ebx + STATE_SOURCE], esi
    mov dword ptr [ebx + STATE_DEST], edi
    mov eax, decodeOriginalCount
    pop edx
    pop ecx
    pop ebx
    pop edi
    pop esi
    pop gs
    pop es
    leave
    ret
decode_link_samples ENDP
_TEXT ENDS
END
