; Lane s46: readable hand-assembly reconstruction, pinned WASM 10.6 candidate.
; Names are working labels; see REPORT.md for encoding blockers and object cuts.
.486
.model flat

OCTANT_RATIO_SHIFT EQU 9
ANGLE_OUTPUT_SHIFT EQU 18
QUARTER_ANGLE_UNITS EQU 4096
HALF_ANGLE_UNITS EQU 8192
FULL_ANGLE_UNITS EQU 16384

VectorAtan_x EQU 8
VectorAtan_y EQU 12

EXTRN atanCorrections:BYTE ; oracle target 004A54F8; external data, not embedded code

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC VectorAtan

; Lookup-based vector angle, returned in a wrapping 32-bit turn.
VectorAtan PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov eax, dword ptr [ebp + VectorAtan_x]
    cdq
    mov esi, edx
    xor eax, edx
    sub eax, edx
    mov ecx, eax
    mov eax, dword ptr [ebp + VectorAtan_y]
    cdq
    mov edi, edx
    xor eax, edx
    sub eax, edx
    je atan_x_sign
    cmp eax, ecx
    jg atan_steep
    ; Round abs(y)*512/abs(x), then apply the byte correction table.
    cdq
    shld edx, eax, OCTANT_RATIO_SHIFT
    shl eax, OCTANT_RATIO_SHIFT
    mov ebx, ecx
    sar ebx, 1
    add eax, ebx
    div ecx
    cmp eax, 0
    je atan_x_sign
    movzx edx, byte ptr [eax + atanCorrections]
    shl eax, 2
    add eax, edx
    jmp SHORT atan_x_sign
    ; Exchange axes in the steep octant; complement the lookup angle.
atan_steep:
    xchg ecx, eax
    cdq
    shld edx, eax, OCTANT_RATIO_SHIFT
    shl eax, OCTANT_RATIO_SHIFT
    mov ebx, ecx
    sar ebx, 1
    add eax, ebx
    div ecx
    cmp eax, 0
    je atan_complement
    movzx edx, byte ptr [eax + atanCorrections]
    shl eax, 2
    add eax, edx
atan_complement:
    sub eax, QUARTER_ANGLE_UNITS
    neg eax
    ; Restore the original signs to select the quadrant.
atan_x_sign:
    cmp esi, 0
    jge atan_y_sign
    sub eax, HALF_ANGLE_UNITS
    neg eax
atan_y_sign:
    cmp edi, 0
    jge atan_scale
    sub eax, FULL_ANGLE_UNITS
    neg eax
atan_scale:
    shl eax, ANGLE_OUTPUT_SHIFT
    pop ebx
    pop edi
    pop esi
    leave
    ret
VectorAtan ENDP
_TEXT ENDS
END
