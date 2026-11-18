; Pixel, span, rectangle and line primitives on an indexed surface.
.486
.model flat

RenderPutPixel_x EQU 8
RenderPutPixel_y EQU 12
RenderPutPixel_color EQU 16

RenderGetPixel_x EQU 8
RenderGetPixel_y EQU 12

RenderFill_x EQU 8
RenderFill_y EQU 12
RenderFill_width EQU 16
RenderFill_height EQU 20

RenderCopy_source EQU 8
RenderCopy_destination EQU 12
RenderCopy_x EQU 16
RenderCopy_y EQU 20
RenderCopy_width EQU 24
RenderCopy_height EQU 28

RenderFillSpans_spans EQU 8
RenderFillSpans_rowsLeft EQU -4
RenderFillSpans_localBytes EQU 4

RenderHorizontal_x EQU 8
RenderHorizontal_y EQU 12
RenderHorizontal_length EQU 16

RenderVertical_x EQU 8
RenderVertical_y EQU 12
RenderVertical_length EQU 16

RenderLine_x0 EQU 8
RenderLine_y0 EQU 12
RenderLine_x1 EQU 16
RenderLine_y1 EQU 20
RenderLine_twiceDx EQU -4
RenderLine_twiceDy EQU -8
RenderLine_localBytes EQU 8

MIN_WIDE_FILL EQU 12
DWORD_ALIGN_MASK EQU 3
DWORD_MASK EQU -4
MIN_WIDE_LINE EQU 16

EXTRN surfaceRowOffsets:DWORD
EXTRN W?gShellSurfacePixels$npnv:DWORD
gShellSurfacePixels EQU W?gShellSurfacePixels$npnv
EXTRN W?gMenuBackground$nul:DWORD
gMenuBackground EQU W?gMenuBackground$nul
EXTRN surfaceRowStride:DWORD
EXTRN W?gMenuForeground$nul:DWORD
gMenuForeground EQU W?gMenuForeground$nul

; Convert scanline y to the address of its first pixel.
LocateSurfaceRow MACRO addressReg, rowReg
    mov addressReg, dword ptr [surfaceRowOffsets]
    mov addressReg, dword ptr [addressReg + rowReg*4]
    add addressReg, dword ptr [gShellSurfacePixels]
ENDM

; Fill the alignment prefix and whole dwords at EDI. EBX retains the byte tail.
FillAlignedDwords MACRO
    mov ecx, edi
    neg ecx
    and ecx, DWORD_ALIGN_MASK
    sub ebx, ecx
    rep stosb
    mov ecx, ebx
    and ecx, DWORD_MASK
    sub ebx, ecx
    shr ecx, 2
    rep stosd
ENDM

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC RenderPutPixel
PUBLIC RenderGetPixel
PUBLIC RenderFill
PUBLIC RenderCopy
PUBLIC RenderFillSpans
PUBLIC RenderHorizontal
PUBLIC RenderVertical
PUBLIC RenderLine

; Write the low byte of color at (x,y).
RenderPutPixel PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + RenderPutPixel_y]
    LocateSurfaceRow edx, eax
    add edx, dword ptr [ebp + RenderPutPixel_x]
    mov eax, dword ptr [ebp + RenderPutPixel_color]
    mov byte ptr [edx], al
    leave
    ret
RenderPutPixel ENDP

; Return the indexed pixel at (x,y) in AL.
RenderGetPixel PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + RenderGetPixel_y]
    LocateSurfaceRow edx, eax
    add edx, dword ptr [ebp + RenderGetPixel_x]
    mov al, byte ptr [edx]
    leave
    ret
RenderGetPixel ENDP

; Fill a background rectangle, aligning wide rows for dword stores.
RenderFill PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    cld
    push ds
    pop es
    mov edx, dword ptr [ebp + RenderFill_y]
    mov eax, dword ptr [gMenuBackground]
    LocateSurfaceRow edi, edx
    add edi, dword ptr [ebp + RenderFill_x]
    mov esi, dword ptr [surfaceRowStride]
    mov edx, dword ptr [ebp + RenderFill_width]
    mov ebx, dword ptr [ebp + RenderFill_height]
    or edx, edx
    jle fill_done
    or ebx, ebx
    jle fill_done
    sub esi, edx
    cmp edx, MIN_WIDE_FILL
    jl fill_short_row
    mov ah, al
    mov ecx, eax
    rol eax, 16
    or eax, ecx
fill_aligned_row:
    mov ebx, edx
    FillAlignedDwords
    mov ecx, ebx
    rep stosb
    add edi, esi
    dec dword ptr [ebp + RenderFill_height]
    jne fill_aligned_row
    jmp SHORT fill_done
fill_short_row:
    mov ecx, edx
    rep stosb
    add edi, esi
    dec ebx
    jne fill_short_row
fill_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
RenderFill ENDP

; Copy whole dwords in each row between surfaces with the same pitch.
RenderCopy PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    cld
    push ds
    pop es
    mov edx, dword ptr [ebp + RenderCopy_y]
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + edx*4]
    add edi, dword ptr [ebp + RenderCopy_x]
    mov esi, edi
    add edi, dword ptr [ebp + RenderCopy_destination]
    add esi, dword ptr [ebp + RenderCopy_source]
    mov ecx, dword ptr [ebp + RenderCopy_width]
    shr ecx, 2
    jle copy_done
    mov ebx, dword ptr [ebp + RenderCopy_height]
    or ebx, ebx
    jle copy_done
copy_row:
    push esi
    push edi
    push ecx
    rep movsd
    pop ecx
    pop edi
    pop esi
    add esi, dword ptr [surfaceRowStride]
    add edi, dword ptr [surfaceRowStride]
    dec ebx
    jne copy_row
copy_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
RenderCopy ENDP

; Fill one [left,right) span per row from a table headed by first y and row count.
RenderFillSpans PROC NEAR
    enter RenderFillSpans_localBytes, 0
    push esi
    push edi
    push ebx
    cld
    push ds
    pop es
    xor eax, eax
    mov esi, dword ptr [ebp + RenderFillSpans_spans]
    mov ebx, dword ptr [esi]
    mov edx, dword ptr [esi + 4]
    add esi, 8
    or edx, edx
    jle spans_done
    LocateSurfaceRow edi, ebx
    mov dword ptr [ebp + RenderFillSpans_rowsLeft], edx
    mov eax, dword ptr [gMenuBackground]
    mov ah, al
    mov ebx, eax
    rol eax, 16
    or eax, ebx
span_row:
    push edi
    mov edx, dword ptr [esi]
    mov ebx, dword ptr [esi + 4]
    add edi, edx
    add esi, 8
    sub ebx, edx
    cmp ebx, MIN_WIDE_FILL
    jl span_tail
    FillAlignedDwords
span_tail:
    mov ecx, ebx
    rep stosb
    pop edi
    add edi, dword ptr [surfaceRowStride]
    dec dword ptr [ebp + RenderFillSpans_rowsLeft]
    jne span_row
spans_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
RenderFillSpans ENDP

; Draw a foreground span. Wide spans use aligned dword stores.
RenderHorizontal PROC NEAR
    enter 0, 0
    push edi
    push ebx
    cld
    push ds
    pop es
    mov edx, dword ptr [ebp + RenderHorizontal_y]
    mov ebx, dword ptr [ebp + RenderHorizontal_length]
    mov eax, dword ptr [gMenuForeground]
    LocateSurfaceRow edi, edx
    add edi, dword ptr [ebp + RenderHorizontal_x]
    or ebx, ebx
    jle horizontal_done
    cmp ebx, MIN_WIDE_LINE
    jl horizontal_tail
    mov ah, al
    mov ecx, eax
    rol eax, 16
    or eax, ecx
    FillAlignedDwords
horizontal_tail:
    mov ecx, ebx
    rep stosb
horizontal_done:
    pop ebx
    pop edi
    leave
    ret
RenderHorizontal ENDP

; Draw a vertical foreground line, stepping by the row stride.
RenderVertical PROC NEAR
    enter 0, 0
    push edi
    mov edx, dword ptr [ebp + RenderVertical_y]
    mov ecx, dword ptr [ebp + RenderVertical_length]
    mov eax, dword ptr [gMenuForeground]
    LocateSurfaceRow edi, edx
    add edi, dword ptr [ebp + RenderVertical_x]
    mov edx, dword ptr [surfaceRowStride]
    or ecx, ecx
    jle vertical_done
vertical_pixel:
    mov byte ptr [edi], al
    add edi, edx
    dec ecx
    jne vertical_pixel
vertical_done:
    pop edi
    leave
    ret
RenderVertical ENDP

; Draw both endpoints with Bresenham stepping along the dominant axis.
RenderLine PROC NEAR
    enter RenderLine_localBytes, 0
    push edi
    push esi
    push ebx
    cld
    push ds
    pop es
    mov eax, dword ptr [ebp + RenderLine_x0]
    mov ebx, dword ptr [ebp + RenderLine_x1]
    cmp eax, ebx
    jle line_deltas
    xchg ebx, eax
    mov dword ptr [ebp + RenderLine_x0], eax
    mov dword ptr [ebp + RenderLine_x1], ebx
    mov ecx, dword ptr [ebp + RenderLine_y0]
    mov edx, dword ptr [ebp + RenderLine_y1]
    mov dword ptr [ebp + RenderLine_y0], edx
    mov dword ptr [ebp + RenderLine_y1], ecx
line_deltas:
    sub ebx, eax
    shl ebx, 1
    mov dword ptr [ebp + RenderLine_twiceDx], ebx
    ; A descending line uses a negative row stride.
    mov edx, dword ptr [surfaceRowStride]
    mov eax, dword ptr [ebp + RenderLine_y1]
    sub eax, dword ptr [ebp + RenderLine_y0]
    jge line_row_direction_set
    neg eax
    neg edx
line_row_direction_set:
    shl eax, 1
    mov dword ptr [ebp + RenderLine_twiceDy], eax
    mov eax, dword ptr [ebp + RenderLine_y0]
    LocateSurfaceRow edi, eax
    add edi, dword ptr [ebp + RenderLine_x0]
    mov eax, dword ptr [gMenuForeground]
    ; ESI holds the signed error. ECX includes the final endpoint.
    mov ebx, dword ptr [ebp + RenderLine_twiceDx]
    cmp ebx, dword ptr [ebp + RenderLine_twiceDy]
    jle line_y_major
    mov esi, dword ptr [ebp + RenderLine_twiceDx]
    shr esi, 1
    mov ecx, esi
    inc ecx
    neg esi
    mov ebx, dword ptr [ebp + RenderLine_twiceDy]
line_x_step:
    dec ecx
    jl line_done
    stosb
    add esi, ebx
    jl line_x_step
    add edi, edx
    sub esi, dword ptr [ebp + RenderLine_twiceDx]
    jmp SHORT line_x_step
line_y_major:
    mov esi, dword ptr [ebp + RenderLine_twiceDy]
    shr esi, 1
    mov ecx, esi
    inc ecx
    neg esi
    mov ebx, dword ptr [ebp + RenderLine_twiceDx]
line_y_step:
    dec ecx
    jl line_done
    mov byte ptr [edi], al
    add edi, edx
    add esi, ebx
    jl line_y_step
    inc edi
    sub esi, dword ptr [ebp + RenderLine_twiceDy]
    jmp SHORT line_y_step
line_done:
    pop ebx
    pop esi
    pop edi
    leave
    ret
RenderLine ENDP
_TEXT ENDS
END
