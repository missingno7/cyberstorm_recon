; Lane s46: readable hand-assembly reconstruction, pinned WASM 10.6 candidate.
; Names are working labels; see REPORT.md for encoding blockers and object cuts.
.486
.model flat

QUARTER_TURN EQU 040000000h
ANGLE_INDEX_SHIFT EQU 20
QUADRANT_SAMPLES EQU 1024
HALF_TURN_SAMPLES EQU 2048
FIXED_FRACTION_BITS EQU 16

FixedSin_angle EQU 8
FixedCos_angle EQU 8
FixedTan_angle EQU 8
RotatePoint_point EQU 8
RotatePoint_angle EQU 12

EXTRN fixedCosineTable:WORD ; oracle target 004A4C18; external data, not embedded code

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC FixedSin
PUBLIC FixedCos
PUBLIC FixedTan
PUBLIC RotatePoint

; Existing C-linkage name FixedSin retained; table computation is cosine-shaped.
FixedSin PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + FixedSin_angle]
    sar eax, ANGLE_INDEX_SHIFT
    adc eax, 0
    cdq
    xor eax, edx
    sub eax, edx
    cmp eax, QUADRANT_SAMPLES
    jl sin_positive
    neg eax
    add eax, HALF_TURN_SAMPLES
    movzx eax, word ptr [eax*2 + fixedCosineTable]
    neg eax
    jmp SHORT sin_scale
sin_positive:
    movzx eax, word ptr [eax*2 + fixedCosineTable]
sin_scale:
    shl eax, 2
    leave
    ret
FixedSin ENDP

; Existing name FixedCos retained; phase shifted by one quarter turn.
FixedCos PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + FixedCos_angle]
    sub eax, QUARTER_TURN
    sar eax, ANGLE_INDEX_SHIFT
    adc eax, 0
    cdq
    xor eax, edx
    sub eax, edx
    cmp eax, QUADRANT_SAMPLES
    jl cos_positive
    neg eax
    add eax, HALF_TURN_SAMPLES
    movzx eax, word ptr [eax*2 + fixedCosineTable]
    neg eax
    jmp SHORT cos_scale
cos_positive:
    movzx eax, word ptr [eax*2 + fixedCosineTable]
cos_scale:
    shl eax, 2
    leave
    ret
FixedCos ENDP

; Working name FixedTan: divide shifted sine by cosine using EDX:EAX.
FixedTan PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + FixedTan_angle]
    mov ecx, eax
    sar eax, ANGLE_INDEX_SHIFT
    adc eax, 0
    cdq
    xor eax, edx
    sub eax, edx
    cmp eax, QUADRANT_SAMPLES
    jl tan_cos_positive
    neg eax
    add eax, HALF_TURN_SAMPLES
    movzx eax, word ptr [eax*2 + fixedCosineTable]
    neg eax
    jmp SHORT tan_cos_scale
tan_cos_positive:
    movzx eax, word ptr [eax*2 + fixedCosineTable]
tan_cos_scale:
    shl eax, 2
    xchg ecx, eax
    sub eax, QUARTER_TURN
    sar eax, ANGLE_INDEX_SHIFT
    adc eax, 0
    cdq
    xor eax, edx
    sub eax, edx
    cmp eax, QUADRANT_SAMPLES
    jl tan_sin_positive
    neg eax
    add eax, HALF_TURN_SAMPLES
    movzx eax, word ptr [eax*2 + fixedCosineTable]
    neg eax
    jmp SHORT tan_sin_scale
tan_sin_positive:
    movzx eax, word ptr [eax*2 + fixedCosineTable]
tan_sin_scale:
    shl eax, 2
    mov edx, eax
    xor eax, eax
    idiv ecx
    leave
    ret
FixedTan ENDP

; Rotate a signed 16.16 point using the same rounded angle lookup.
RotatePoint PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov eax, dword ptr [ebp + RotatePoint_angle]
    mov edi, eax
    sar eax, ANGLE_INDEX_SHIFT
    adc eax, 0
    cdq
    xor eax, edx
    sub eax, edx
    cmp eax, QUADRANT_SAMPLES
    jl rotate_cos_positive
    neg eax
    add eax, HALF_TURN_SAMPLES
    movzx eax, word ptr [eax*2 + fixedCosineTable]
    neg eax
    jmp SHORT rotate_cos_scale
rotate_cos_positive:
    movzx eax, word ptr [eax*2 + fixedCosineTable]
rotate_cos_scale:
    shl eax, 2
    xchg edi, eax
    sub eax, QUARTER_TURN
    sar eax, ANGLE_INDEX_SHIFT
    adc eax, 0
    cdq
    xor eax, edx
    sub eax, edx
    cmp eax, QUADRANT_SAMPLES
    jl rotate_sin_positive
    neg eax
    add eax, HALF_TURN_SAMPLES
    movzx eax, word ptr [eax*2 + fixedCosineTable]
    neg eax
    jmp SHORT rotate_sin_scale
rotate_sin_positive:
    movzx eax, word ptr [eax*2 + fixedCosineTable]
rotate_sin_scale:
    shl eax, 2
    mov esi, eax
    ; Preserve the frame pointer while EBP addresses the two-coordinate point.
    push ebp
    mov ebp, dword ptr [ebp + RotatePoint_point]
    neg eax
    imul dword ptr [ebp + 4]
    mov ebx, eax
    mov ecx, edx
    mov eax, edi
    imul dword ptr [ebp]
    add eax, ebx
    adc edx, ecx
    shrd eax, edx, FIXED_FRACTION_BITS
    xchg esi, eax
    imul dword ptr [ebp]
    mov ebx, eax
    mov ecx, edx
    mov eax, edi
    imul dword ptr [ebp + 4]
    add eax, ebx
    adc edx, ecx
    shrd eax, edx, FIXED_FRACTION_BITS
    mov dword ptr [ebp], esi
    mov dword ptr [ebp + 4], eax
    pop ebp
    pop ebx
    pop edi
    pop esi
    leave
    ret
RotatePoint ENDP
_TEXT ENDS
END
