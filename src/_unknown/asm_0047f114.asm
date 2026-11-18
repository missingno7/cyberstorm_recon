; Clipped, scaled indexed-image drawing.
.486
.model flat

RenderImage_image EQU 8
RenderImage_x EQU 12
RenderImage_y EQU 16
RenderImage_flags EQU 20
RenderImage_width EQU 24
RenderImage_height EQU 28

FLIP_X EQU 1
FLIP_Y EQU 2
IMAGE_WIDTH EQU 4
FRACTION_BITS EQU 16
HALF_PIXEL EQU 8000h
IMAGE_HEIGHT EQU 6
BLEND_MODE EQU 1

EXTRN W?viewportClipRight$ni:DWORD
viewportClipRight EQU W?viewportClipRight$ni
EXTRN W?viewportClipBottom$ni:DWORD
viewportClipBottom EQU W?viewportClipBottom$ni
EXTRN W?viewportClipLeft$ni:DWORD
viewportClipLeft EQU W?viewportClipLeft$ni
EXTRN W?viewportClipTop$ni:DWORD
viewportClipTop EQU W?viewportClipTop$ni
EXTRN surfaceRowOffsets:DWORD
EXTRN W?gShellSurfacePixels$npnv:DWORD
gShellSurfacePixels EQU W?gShellSurfacePixels$npnv
EXTRN W?gDrawRemap$npnv:DWORD
gDrawRemap EQU W?gDrawRemap$npnv
EXTRN surfaceRowStride:DWORD
EXTRN worldColorsBackup:BYTE

_DATA SEGMENT DWORD PUBLIC USE32 'DATA'
sourcePixels dd 0
sourceWidth dd 0
sourceHeight dd 0
destLeft dd 0
destTop dd 0
destWidth dd 0
destHeight dd 0
; Source coordinates and steps are signed 16.16.
sourceX dd 0
stepX dd 0
sourceY dd 0
stepY dd 0
    ; The three dwords between the coordinate steps and loop scratch have no observed reads.
    dd 3 dup (0)
stepXWhole dd 0
stepXFraction dd 0
startXWhole dd 0
startXFraction dd 0
_DATA ENDS

; ESI = sourcePixels + floor(sourceY) * sourceWidth + floor(sourceX).
LocateSourcePixel MACRO
    xor edx, edx
    movsx eax, word ptr [sourceY + 2]
    mov esi, dword ptr [sourcePixels]
    imul dword ptr [sourceWidth]
    movsx ecx, word ptr [sourceX + 2]
    add esi, eax
    add esi, ecx
ENDM

; Split 16.16 sampling into a signed whole step and a left-justified fraction for ADD/ADC.
PrepareHorizontalStep MACRO
    movsx ebp, word ptr [stepX]
    shl ebp, FRACTION_BITS
    movsx eax, word ptr [stepX + 2]
    mov dword ptr [stepXWhole], eax
    movsx eax, word ptr [sourceX]
    shl eax, FRACTION_BITS
    mov dword ptr [startXFraction], eax
ENDM

; Restore the destination row, advance both coordinates and test the remaining height.
AdvanceScaledRow MACRO
    pop edi
    mov eax, dword ptr [stepY]
    add edi, dword ptr [surfaceRowStride]
    add dword ptr [sourceY], eax
    dec dword ptr [destHeight]
ENDM

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC RenderImage
PUBLIC ScaleTransparentRows
PUBLIC ScaleRemappedRows
PUBLIC ScaleRemappedFractionalRows
PUBLIC ScaleBlendedRows
PUBLIC ScaleTranslatedRows

; Scale an indexed image into the clipped destination rectangle.
RenderImage PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov esi, dword ptr [ebp + RenderImage_image]
    mov ecx, dword ptr [ebp + RenderImage_width]
    mov edx, dword ptr [ebp + RenderImage_height]
    or ecx, ecx
    je render_done
    jg height_sign
    neg ecx
    mov dword ptr [ebp + RenderImage_width], ecx
    xor dword ptr [ebp + RenderImage_flags], FLIP_X
height_sign:
    or edx, edx
    je render_done
    jg check_bounds
    neg edx
    mov dword ptr [ebp + RenderImage_height], edx
    xor dword ptr [ebp + RenderImage_flags], FLIP_Y
check_bounds:
    mov eax, dword ptr [ebp + RenderImage_x]
    mov ebx, dword ptr [ebp + RenderImage_y]
    mov dword ptr [destLeft], eax
    mov dword ptr [destTop], ebx
    mov dword ptr [destWidth], ecx
    mov dword ptr [destHeight], edx
    cmp eax, dword ptr [viewportClipRight]
    jg render_done
    cmp ebx, dword ptr [viewportClipBottom]
    jg render_done
    add eax, ecx
    add ebx, edx
    cmp eax, dword ptr [viewportClipLeft]
    jl render_done
    cmp ebx, dword ptr [viewportClipTop]
    jl render_done
    ; Map the two source endpoints to the first and last destination pixels.
    movzx eax, word ptr [esi + IMAGE_WIDTH]
    mov dword ptr [sourceWidth], eax
    xor ebx, ebx
    dec eax
    test dword ptr [ebp + RenderImage_flags], FLIP_X
    je x_endpoints
    xchg ebx, eax
x_endpoints:
    mov ecx, dword ptr [destWidth]
    shl eax, FRACTION_BITS
    shl ebx, FRACTION_BITS
    sub eax, ebx
    ; A half-pixel bias rounds the 16.16 sample coordinate.
    add ebx, HALF_PIXEL
    mov dword ptr [sourceX], ebx
    dec ecx
    jle set_x_step
    cdq
    idiv ecx
set_x_step:
    mov dword ptr [stepX], eax
    movzx eax, word ptr [esi + IMAGE_HEIGHT]
    mov dword ptr [sourceHeight], eax
    xor ebx, ebx
    dec eax
    test dword ptr [ebp + RenderImage_flags], FLIP_Y
    je y_endpoints
    xchg ebx, eax
y_endpoints:
    mov ecx, dword ptr [destHeight]
    shl eax, FRACTION_BITS
    shl ebx, FRACTION_BITS
    sub eax, ebx
    add ebx, HALF_PIXEL
    mov dword ptr [sourceY], ebx
    dec ecx
    jle set_y_step
    cdq
    idiv ecx
set_y_step:
    mov dword ptr [stepY], eax
    ; Clipping the leading edges also advances the starting sample coordinates.
    mov ecx, dword ptr [viewportClipLeft]
    sub ecx, dword ptr [destLeft]
    jle clip_right
    mov eax, dword ptr [viewportClipLeft]
    mov dword ptr [destLeft], eax
    mov eax, dword ptr [stepX]
    cdq
    imul ecx
    add dword ptr [sourceX], eax
    sub dword ptr [destWidth], ecx
clip_right:
    mov ecx, dword ptr [destLeft]
    add ecx, dword ptr [destWidth]
    dec ecx
    sub ecx, dword ptr [viewportClipRight]
    jle clip_top
    sub dword ptr [destWidth], ecx
clip_top:
    mov ecx, dword ptr [viewportClipTop]
    sub ecx, dword ptr [destTop]
    jle clip_bottom
    mov eax, dword ptr [viewportClipTop]
    mov dword ptr [destTop], eax
    mov eax, dword ptr [stepY]
    cdq
    imul ecx
    add dword ptr [sourceY], eax
    sub dword ptr [destHeight], ecx
clip_bottom:
    mov ecx, dword ptr [destTop]
    add ecx, dword ptr [destHeight]
    dec ecx
    sub ecx, dword ptr [viewportClipBottom]
    jle check_dimensions
    sub dword ptr [destHeight], ecx
check_dimensions:
    cmp dword ptr [destWidth], 0
    jle render_done
    cmp dword ptr [destHeight], 0
    jle render_done
    cld
    push ds
    pop es
    add esi, dword ptr [esi]
    mov dword ptr [sourcePixels], esi
    mov ebx, dword ptr [destTop]
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + ebx*4]
    add edi, dword ptr [gShellSurfacePixels]
    add edi, dword ptr [destLeft]
    ; Select raw, two-color blend, or palette lookup.
    mov ebx, dword ptr [gDrawRemap]
    or ebx, ebx
    je use_plain_rows
    cmp ebx, BLEND_MODE
    jne select_remap_step
    call ScaleBlendedRows
    pop ebx
    pop edi
    pop esi
    leave
    ret
select_remap_step:
    movsx eax, word ptr [stepX + 2]
    or eax, eax
    je use_fractional_remap
    call ScaleRemappedRows
    pop ebx
    pop edi
    pop esi
    leave
    ret
use_fractional_remap:
    call ScaleRemappedFractionalRows
    pop ebx
    pop edi
    pop esi
    leave
    ret
use_plain_rows:
    call ScaleTransparentRows
render_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
RenderImage ENDP

; Copy nonzero source pixels. EDI is the first destination row.
ScaleTransparentRows PROC NEAR
    push ebp
    PrepareHorizontalStep
plain_row:
    LocateSourcePixel
    mov ecx, dword ptr [destWidth]
    mov edx, dword ptr [stepXWhole]
    mov ebx, dword ptr [startXFraction]
    push edi
    dec edi
    inc ecx
plain_skip:
    inc edi
    dec ecx
    jle plain_next_row
plain_sample:
    ; ADD sets the fractional carry for the following ADC on the source pointer.
    mov al, byte ptr [esi]
    add ebx, ebp
    adc esi, edx
    or al, al
    je plain_skip
    mov byte ptr [edi], al
    inc edi
    dec ecx
    jne plain_sample
plain_next_row:
    AdvanceScaledRow
    jne plain_row
    pop ebp
    ret
ScaleTransparentRows ENDP

; Translate nonzero pixels through EBX, stepping by the integer part plus carry.
ScaleRemappedRows PROC NEAR
    push ebp
    PrepareHorizontalStep
remap_row:
    LocateSourcePixel
    push edi
    mov ecx, dword ptr [destWidth]
    mov edx, dword ptr [startXFraction]
    dec edi
    inc ecx
    xor eax, eax
remap_skip:
    inc edi
    dec ecx
    jle remap_next_row
remap_sample:
    mov al, byte ptr [esi]
    add edx, ebp
    adc esi, dword ptr [stepXWhole]
    or al, al
    je remap_skip
    inc edi
    mov al, byte ptr [ebx + eax]
    mov byte ptr [edi - 1], al
    dec ecx
    jne remap_sample
remap_next_row:
    AdvanceScaledRow
    jne remap_row
    pop ebp
    ret
ScaleRemappedRows ENDP

; Translate through EBX when the integer part of the horizontal step is zero.
ScaleRemappedFractionalRows PROC NEAR
    push ebp
    PrepareHorizontalStep
fractional_row:
    LocateSourcePixel
    push edi
    mov ecx, dword ptr [destWidth]
    mov edx, dword ptr [startXFraction]
    dec edi
    inc ecx
    xor eax, eax
fractional_skip:
    inc edi
    dec ecx
    jle fractional_next_row
fractional_sample:
    mov al, byte ptr [esi]
    add edx, ebp
    adc esi, 0
    or al, al
    je fractional_skip
    inc edi
    mov al, byte ptr [ebx + eax]
    mov byte ptr [edi - 1], al
    dec ecx
    jne fractional_sample
fractional_next_row:
    AdvanceScaledRow
    jne fractional_row
    pop ebp
    ret
ScaleRemappedFractionalRows ENDP

; Index the blend table with the source in BH and the destination in BL.
ScaleBlendedRows PROC NEAR
    push ebp
    movsx ebp, word ptr [stepX]
    shl ebp, FRACTION_BITS
    mov dword ptr [stepXFraction], ebp
    movsx eax, word ptr [stepX + 2]
    mov dword ptr [stepXWhole], eax
    movsx eax, word ptr [sourceX]
    shl eax, FRACTION_BITS
    mov dword ptr [startXFraction], eax
    xor ebx, ebx
    movsx eax, word ptr [sourceX + 2]
    mov dword ptr [startXWhole], eax
blend_row:
    xor edx, edx
    movsx eax, word ptr [sourceY + 2]
    mov esi, dword ptr [sourcePixels]
    imul dword ptr [sourceWidth]
    add esi, eax
    add esi, dword ptr [startXWhole]
    push edi
    mov ecx, dword ptr [destWidth]
    dec edi
    inc ecx
    mov eax, dword ptr [startXFraction]
    mov edx, dword ptr [stepXWhole]
    mov ebp, dword ptr [stepXFraction]
blend_skip:
    inc edi
    dec ecx
    jle blend_next_row
blend_sample:
    mov bh, byte ptr [esi]
    add eax, ebp
    adc esi, edx
    or bh, bh
    je blend_skip
    mov bl, byte ptr [edi]
    mov al, byte ptr [ebx + worldColorsBackup]
    mov byte ptr [edi], al
    inc edi
    dec ecx
    jne blend_sample
blend_next_row:
    AdvanceScaledRow
    jne blend_row
    pop ebp
    ret
ScaleBlendedRows ENDP

; Alternate table-lookup loop using XLAT with the palette in EBX.
ScaleTranslatedRows PROC NEAR
    push ebp
    movsx ebp, word ptr [stepX]
    shl ebp, FRACTION_BITS
xlat_row:
    LocateSourcePixel
    mov ecx, dword ptr [destWidth]
    movsx edx, word ptr [stepX + 2]
    movsx eax, word ptr [stepX]
    push edi
    shl eax, FRACTION_BITS
    dec edi
    inc ecx
xlat_skip:
    inc edi
    dec ecx
    jle xlat_next_row
xlat_sample:
    mov al, byte ptr [esi]
    add eax, ebp
    adc esi, edx
    or al, al
    je xlat_skip
    xlat
    mov byte ptr [edi], al
    inc edi
    dec ecx
    jne xlat_sample
xlat_next_row:
    AdvanceScaledRow
    jne xlat_row
    pop ebp
    ret
ScaleTranslatedRows ENDP
_TEXT ENDS
END
