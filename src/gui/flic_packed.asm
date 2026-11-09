; Lane s46: readable hand-assembly reconstruction, pinned WASM 10.6 candidate.
; Names are working labels; see REPORT.md for encoding blockers and object cuts.
.486
.model flat

TOKENS_PER_BLOCK EQU 8
DISTANCE_SHIFT EQU 4
LENGTH_MASK EQU 15
MIN_MATCH_LENGTH EQU 3
LONG_COUNT_MARKER EQU 0FFFFh

UnpackFlicPacked_dst EQU 8
UnpackFlicPacked_src EQU 12

EXTRN packedBlocksLeft:DWORD ; oracle target 004A4BB4; external data, not embedded code
EXTRN packedTailBits:DWORD ; oracle target 004A4BB8; external data, not embedded code

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC UnpackFlicPacked

; Eight tokens per block; set token bits select a sliding-window reference.
UnpackFlicPacked PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    cld
    push ds
    pop es
    mov edi, dword ptr [ebp + UnpackFlicPacked_dst]
    mov esi, dword ptr [ebp + UnpackFlicPacked_src]
    xor eax, eax
    lodsb
    cmp al, 8
    jae packed_small_header
    ; Header below eight gives tail-token count followed by block count.
    mov dword ptr [packedTailBits], eax
    lodsw
    cmp eax, LONG_COUNT_MARKER
    je packed_long_count
    mov dword ptr [packedBlocksLeft], eax
    jmp SHORT packed_next_block
packed_long_count:
    lodsd
    mov dword ptr [packedBlocksLeft], eax
    xor eax, eax
    jmp SHORT packed_next_block
packed_small_header:
    mov ecx, eax
    shr ecx, 3
    and al, 7
    dec ecx
    mov dword ptr [packedTailBits], eax
    mov dword ptr [packedBlocksLeft], ecx
    jmp SHORT packed_next_block
packed_literal_block:
    movsd
    movsd
packed_next_block:
    dec dword ptr [packedBlocksLeft]
    js packed_tail
    lodsb
    or al, al
    je packed_literal_block
    mov bl, al
    mov ecx, TOKENS_PER_BLOCK
packed_token:
    add bl, bl
    jb packed_back_reference
    movsb
    loop packed_token
    jmp SHORT packed_next_block
    ; 16-bit token: distance=(token>>4)+1, length=(token&15)+3.
packed_back_reference:
    lodsw
    push esi
    push ecx
    mov esi, edi
    mov ecx, eax
    shr eax, DISTANCE_SHIFT
    and ecx, LENGTH_MASK
    inc eax
    add ecx, MIN_MATCH_LENGTH
    sub esi, eax
    rep movsb
    pop ecx
    pop esi
    loop packed_token
    jmp SHORT packed_next_block
    ; Decode the partial block after all complete eight-token blocks.
packed_tail:
    mov ecx, dword ptr [packedTailBits]
    or ecx, ecx
    je packed_done
    xor eax, eax
    mov dword ptr [packedTailBits], eax
    lodsb
    mov bl, al
    jmp SHORT packed_token
packed_done:
    mov eax, edi
    sub eax, dword ptr [ebp + UnpackFlicPacked_dst]
    pop ebx
    pop edi
    pop esi
    leave
    ret
UnpackFlicPacked ENDP
_TEXT ENDS
END
