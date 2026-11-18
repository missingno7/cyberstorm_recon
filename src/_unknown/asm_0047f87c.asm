; Integer vector lengths, square root and signed multiply helpers.
.486
.model flat

VectorLength_x EQU 8
VectorLength_y EQU 12

VectorLength3D_x EQU 8
VectorLength3D_y EQU 12
VectorLength3D_z EQU 16

ISqrt_radicand EQU 8

SignedMultiplyDivide_a EQU 8
SignedMultiplyDivide_b EQU 12
SignedMultiplyDivide_divisor EQU 16

FixedMultiplyShift_a EQU 8
FixedMultiplyShift_b EQU 12
FixedMultiplyShift_shift EQU 16

; Take the two's-complement magnitude of EAX, using EDX as its sign mask.
MagnitudeEAX MACRO
    cdq
    xor eax, edx
    sub eax, edx
ENDM

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC VectorLength
PUBLIC VectorLength3D
PUBLIC ISqrt
PUBLIC SignedMultiplyDivide
PUBLIC FixedMultiplyShift

; Approximate planar distance by the larger magnitude plus half the smaller.
VectorLength PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + VectorLength_x]
    MagnitudeEAX
    mov ecx, eax
    mov eax, dword ptr [ebp + VectorLength_y]
    MagnitudeEAX
    cmp ecx, eax
    jl y_longer
    sar eax, 1
    add eax, ecx
    leave
    ret
y_longer:
    sar ecx, 1
    add eax, ecx
    leave
    ret
VectorLength ENDP

; Sort three magnitudes and weight the smaller two with arithmetic shifts.
VectorLength3D PROC NEAR
    enter 0, 0
    push ebx
    mov eax, dword ptr [ebp + VectorLength3D_x]
    MagnitudeEAX
    mov ecx, eax
    mov eax, dword ptr [ebp + VectorLength3D_y]
    MagnitudeEAX
    mov ebx, eax
    mov eax, dword ptr [ebp + VectorLength3D_z]
    MagnitudeEAX
    cmp ecx, ebx
    jge sort_third
    xchg ecx, ebx
sort_third:
    cmp ebx, eax
    jge weight_components
    xchg ebx, eax
    cmp ecx, ebx
    jge weight_components
    xchg ecx, ebx
weight_components:
    ; ECX >= EBX >= EAX. Add 1/4 of the smallest and successive fractions of the middle.
    sar eax, 2
    add eax, ecx
    sar ebx, 2
    add eax, ebx
    sar ebx, 2
    add eax, ebx
    sar ebx, 1
    add eax, ebx
    pop ebx
    leave
    ret
VectorLength3D ENDP

; Find the unsigned integer square root, testing one root bit at a time.
ISqrt PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov edx, dword ptr [ebp + ISqrt_radicand]
    push ebp
    sub ebx, ebx
    or edx, edx
    je root_done
    bsr ecx, edx
    shr ecx, 1
    inc ebx
    shl ebx, cl
    ; Only CX is tested. The bit index from BSR is already at most 15 after the shift.
    jcxz root_done
    mov eax, ebx
    shl eax, cl
    mov ebp, ebx
    shr ebp, 1
    dec ecx
root_cross_term:
    mov esi, ebx
    shl esi, cl
    shl esi, 1
root_trial_square:
    mov edi, ebp
    shl edi, cl
    add edi, eax
    add edi, esi
    dec ecx
    ; EDI is the trial square and EDX is the radicand.
    cmp edi, edx
    ja root_reject_bit
    add ebx, ebp
    shr ebp, 1
    je root_done
    mov eax, edi
    jmp SHORT root_cross_term
root_reject_bit:
    shr ebp, 1
    je root_done
    shr esi, 1
    jmp SHORT root_trial_square
root_done:
    pop ebp
    mov eax, ebx
    pop ebx
    pop edi
    pop esi
    leave
    ret
ISqrt ENDP

; Divide the full signed EDX:EAX product by the third argument.
SignedMultiplyDivide PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + SignedMultiplyDivide_a]
    imul dword ptr [ebp + SignedMultiplyDivide_b]
    idiv dword ptr [ebp + SignedMultiplyDivide_divisor]
    leave
    ret
SignedMultiplyDivide ENDP

; Shift the low 32 bits of the product arithmetically by CL.
FixedMultiplyShift PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + FixedMultiplyShift_a]
    imul dword ptr [ebp + FixedMultiplyShift_b]
    mov ecx, dword ptr [ebp + FixedMultiplyShift_shift]
    sar eax, cl
    leave
    ret
FixedMultiplyShift ENDP
_TEXT ENDS
END
