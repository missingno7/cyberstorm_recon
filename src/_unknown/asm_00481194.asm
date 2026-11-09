; Lane s49: readable hand-assembly reconstruction, pinned TASM32 candidate.
; Names are working labels. Operand order preserves the original XCHG encodings.
.486
.model flat

RenderClipLine_x0_ptr EQU 8
RenderClipLine_y0_ptr EQU 12
RenderClipLine_x1_ptr EQU 16
RenderClipLine_y1_ptr EQU 20
RenderClipLine_rect EQU 24


_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC RenderClipLine

; Clip signed endpoints to {left,top,right,bottom}; write back only on success.
RenderClipLine PROC NEAR
    enter 0, 0
    push edi
    push esi
    push ebx
    push ebp
    mov edx, dword ptr [ebp + RenderClipLine_x0_ptr]
    mov ebx, dword ptr [edx]
    mov edx, dword ptr [ebp + RenderClipLine_y0_ptr]
    mov ecx, dword ptr [edx]
    mov edx, dword ptr [ebp + RenderClipLine_x1_ptr]
    mov esi, dword ptr [edx]
    mov edx, dword ptr [ebp + RenderClipLine_y1_ptr]
    mov edi, dword ptr [edx]
    ; EBX,ECX=(x0,y0); ESI,EDI=(x1,y1); saved EBP now holds the rectangle.
    mov ebp, dword ptr [ebp + RenderClipLine_rect]
    mov eax, dword ptr [ebp + 4]
    cmp ecx, eax
    jl clip_top_first
    cmp edi, eax
    jge clip_left
    xchg ebx, esi
    xchg ecx, edi
    jmp SHORT intersect_top
clip_top_first:
    cmp edi, eax
    jl clip_reject
    ; Full signed product and quotient find the intersection with the top edge.
intersect_top:
    mov edx, eax
    sub edx, ecx
    sub ecx, edi
    mov eax, ebx
    sub eax, esi
    imul edx
    idiv ecx
    add ebx, eax
    mov ecx, dword ptr [ebp + 4]
clip_left:
    mov eax, dword ptr [ebp]
    cmp ebx, eax
    jl clip_left_first
    cmp esi, eax
    jge clip_bottom
    xchg ebx, esi
    xchg ecx, edi
    jmp SHORT intersect_left
clip_left_first:
    cmp esi, eax
    jl clip_reject
intersect_left:
    mov edx, eax
    sub edx, ebx
    sub ebx, esi
    mov eax, ecx
    sub eax, edi
    imul edx
    idiv ebx
    add ecx, eax
    mov ebx, dword ptr [ebp]
clip_bottom:
    mov eax, dword ptr [ebp + 0Ch]
    cmp ecx, eax
    jg clip_bottom_first
    cmp edi, eax
    jle clip_right
    xchg ebx, esi
    xchg ecx, edi
    jmp SHORT intersect_bottom
clip_bottom_first:
    cmp edi, eax
    jg clip_reject
intersect_bottom:
    mov edx, eax
    sub edx, ecx
    sub ecx, edi
    mov eax, ebx
    sub eax, esi
    imul edx
    idiv ecx
    add ebx, eax
    mov ecx, dword ptr [ebp + 0Ch]
clip_right:
    mov eax, dword ptr [ebp + 8]
    cmp ebx, eax
    jg clip_right_first
    cmp esi, eax
    jle clip_accept
    xchg ebx, esi
    xchg ecx, edi
    jmp SHORT intersect_right
clip_right_first:
    cmp esi, eax
    jg clip_reject
intersect_right:
    mov edx, eax
    sub edx, ebx
    sub ebx, esi
    mov eax, ecx
    sub eax, edi
    imul edx
    idiv ebx
    add ecx, eax
    mov ebx, dword ptr [ebp + 8]
    ; Restore the argument frame, then commit both clipped endpoints.
clip_accept:
    pop ebp
    mov edx, dword ptr [ebp + RenderClipLine_x0_ptr]
    mov dword ptr [edx], ebx
    mov edx, dword ptr [ebp + RenderClipLine_y0_ptr]
    mov dword ptr [edx], ecx
    mov edx, dword ptr [ebp + RenderClipLine_x1_ptr]
    mov dword ptr [edx], esi
    mov edx, dword ptr [ebp + RenderClipLine_y1_ptr]
    mov dword ptr [edx], edi
    mov eax, 1
    pop ebx
    pop esi
    pop edi
    leave
    ret
clip_reject:
    pop ebp
    xor eax, eax
    pop ebx
    pop esi
    pop edi
    leave
    ret
RenderClipLine ENDP
_TEXT ENDS
END
