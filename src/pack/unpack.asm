; Lane s46: readable hand-assembly reconstruction, pinned WASM 10.6 candidate.
; Names are working labels; see REPORT.md for encoding blockers and object cuts.
.486
.model flat

RUN_LENGTH_MASK EQU 127
FAST_ZERO_RUN_MIN EQU 12
FAST_LITERAL_MIN EQU 20

UnpackLzw_dst EQU 8
UnpackLzw_src EQU 12


_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC UnpackLzw

; Historical UnpackLzw label retained; this is byte-run/literal decoding.
UnpackLzw PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    cld
    mov edi, dword ptr [ebp + UnpackLzw_dst]
    mov esi, dword ptr [ebp + UnpackLzw_src]
    xor eax, eax
    xor ebx, ebx
    xor edx, edx
    mov dl, RUN_LENGTH_MASK
    jmp SHORT run_command
run_value:
    mov al, byte ptr [esi]
    inc esi
    or al, al
    jne run_remainder
    cmp ecx, FAST_ZERO_RUN_MIN
    jl run_remainder
    ; Long zero runs: align EDI, then use dword stores and a byte remainder.
    mov ebx, ecx
    mov ecx, edi
    neg ecx
    and ecx, 3
    sub ebx, ecx
    rep stosb
    mov ecx, ebx
    and ecx, 0FFFFFFFCh
    sub ebx, ecx
    shr ecx, 2
    rep stosd
    mov ecx, ebx
run_remainder:
    rep stosb
run_command:
    mov al, byte ptr [esi]
    mov ecx, eax
    inc esi
    and ecx, edx
    je run_done
    or al, al
    js run_value
    cmp ecx, FAST_LITERAL_MIN
    jge literal_align
literal_byte:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec ecx
    jne literal_byte
    jmp SHORT run_command
    ; Long literals: align EDI, assemble dwords in BL/BH, then copy the remainder.
literal_align:
    mov ebx, ecx
    mov ecx, edi
    neg ecx
    and ecx, 3
    sub ebx, ecx
    rep movsb
    mov ecx, ebx
literal_dword:
    mov bl, byte ptr [esi]
    mov bh, byte ptr [esi + 1]
    rol ebx, 010h
    mov bl, byte ptr [esi + 2]
    mov bh, byte ptr [esi + 3]
    rol ebx, 010h
    sub ecx, 4
    mov dword ptr [edi], ebx
    add esi, 4
    add edi, 4
    cmp ecx, 4
    jge literal_dword
    rep movsb
    jmp SHORT run_command
run_done:
    mov eax, edi
    sub eax, dword ptr [ebp + UnpackLzw_dst]
    pop ebx
    pop edi
    pop esi
    leave
    ret
UnpackLzw ENDP
_TEXT ENDS
END
