; Lane s46: readable hand-assembly reconstruction, pinned WASM 10.6 candidate.
; Names are working labels; see REPORT.md for encoding blockers and object cuts.
.486
.model flat

EXTENDED_COMMAND EQU 080h
SHORT_SKIP_MASK EQU 127
LONG_LENGTH_MASK EQU 03FFFh
LONG_RUN_FLAG EQU 04000h
LONG_COMMAND_MASK EQU 07FFFh

UnpackFlicPixels_dst EQU 8
UnpackFlicPixels_src EQU 12


_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC UnpackFlicPixels

; Decode signed pixel commands: literals, repeated bytes, destination skips.
UnpackFlicPixels PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    cld
    push ds
    pop es
    mov esi, dword ptr [ebp + UnpackFlicPixels_src]
    mov edi, dword ptr [ebp + UnpackFlicPixels_dst]
    mov ebx, LONG_LENGTH_MASK
    mov edx, SHORT_SKIP_MASK
pixels_command:
    xor eax, eax
    mov al, byte ptr [esi]
    inc esi
    or al, al
    jg pixels_literal
    je pixels_short_run
    and eax, edx
    je pixels_long_command
pixels_skip:
    add edi, eax
    jmp SHORT pixels_command
pixels_literal:
    mov ecx, eax
    and ecx, ebx
pixels_copy_byte:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec ecx
    jne pixels_copy_byte
    jmp SHORT pixels_command
pixels_short_run:
    mov al, byte ptr [esi]
    inc esi
pixels_run_length:
    mov ecx, eax
    and ecx, ebx
    mov al, byte ptr [esi]
    inc esi
pixels_repeat_byte:
    mov byte ptr [edi], al
    inc edi
    dec ecx
    jne pixels_repeat_byte
    jmp SHORT pixels_command
    ; Extended 16-bit command. A zero command ends the stream.
pixels_long_command:
    movsx eax, word ptr [esi]
    add esi, 2
    or eax, eax
    jg pixels_skip
    je pixels_done
    and eax, LONG_COMMAND_MASK
    cmp eax, LONG_RUN_FLAG
    jge pixels_run_length
    jmp SHORT pixels_literal
pixels_done:
    mov eax, edi
    sub eax, dword ptr [ebp + UnpackFlicPixels_dst]
    pop ebx
    pop edi
    pop esi
    leave
    ret
UnpackFlicPixels ENDP
_TEXT ENDS
END
