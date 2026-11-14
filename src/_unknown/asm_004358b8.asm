; Lane a1: assembly-origin raster routines, 004358b8..00435b5a.
; TASM32 5.0, -ml -m5. Names and SpanState field meanings are working labels.
; Two zero linker-fill bytes at 00435b5a..00435b5c are outside this object.
.486
.model flat

SPAN_INTEGER_STEP EQU 0
SPAN_FRACTION_STEP EQU 4
SPAN_SKIP EQU 8
SPAN_DESTINATION EQU 0Ch
SPAN_SOURCE EQU 10h
SPAN_WIDTH EQU 14h
SPAN_STEP EQU 18h

EXTRN W?gMenuBackground$nul:DWORD ; existing binding 005138e0
gMenuBackground EQU W?gMenuBackground$nul
EXTRN W?gShellSurfacePixels$npnv:DWORD ; existing binding 00513890
gShellSurfacePixels EQU W?gShellSurfacePixels$npnv
EXTRN surfaceRowOffsets:DWORD ; existing binding 005138a4
EXTRN surfaceRowStride:DWORD ; existing binding 005138a8
EXTRN W?viewportClipLeft$ni:DWORD ; existing binding 005138fc
viewportClipLeft EQU W?viewportClipLeft$ni
EXTRN W?viewportClipTop$ni:DWORD ; existing binding 00513900
viewportClipTop EQU W?viewportClipTop$ni
EXTRN W?viewportClipRight$ni:DWORD ; existing binding 00513904
viewportClipRight EQU W?viewportClipRight$ni
EXTRN W?viewportClipBottom$ni:DWORD ; existing binding 00513908
viewportClipBottom EQU W?viewportClipBottom$ni

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC DrawScaledSpan
PUBLIC DrawSpan16
PUBLIC FillTerrainRows
PUBLIC FillTerrainRowsClipped

DrawScaledSpan PROC NEAR
    ; Nearest-neighbor span sampling. EDX is the fraction, ESI its step; ADC advances source EBP.
    enter 0, 0
    push esi
    push edi
    push ebx
    push ebp
    mov eax, dword ptr [ebp + 8]
    mov ebx, dword ptr [eax + SPAN_INTEGER_STEP]
    mov esi, dword ptr [eax + SPAN_FRACTION_STEP]
    mov ebp, dword ptr [eax + SPAN_SOURCE]
    mov edi, dword ptr [eax + SPAN_DESTINATION]
    mov edx, dword ptr [eax + SPAN_SKIP]
    mov ecx, dword ptr [eax + SPAN_WIDTH]
    or edx, edx
    je SHORT scaled_center
    ; Skip to the clipped start, rounding the initial fixed-point coordinate by half a pixel.
    mov eax, dword ptr [eax + SPAN_STEP]
    imul edx
    add eax, 08000h
    mov edx, eax
    shr eax, 010h
    shl edx, 010h
    and eax, 0FFh
    add ebp, eax
    jmp SHORT scaled_count
scaled_center:
    mov edx, 080000000h
scaled_count:
    cmp ecx, 0Ch
    jl SHORT scaled_tail
    test edi, 3
    je SHORT scaled_dword
scaled_align:
    mov al, byte ptr [ebp]
    add edx, esi
    mov byte ptr [edi], al
    inc edi
    adc ebp, ebx
    dec ecx
    test edi, 3
    jne SHORT scaled_align
scaled_dword:
    ; Pack four sampled indices into EAX, preserving ADD carry across each sample load.
    add edx, esi
    mov al, byte ptr [ebp]
    adc ebp, ebx
    add edx, esi
    mov ah, byte ptr [ebp]
    adc ebp, ebx
    rol eax, 010h
    add edx, esi
    mov al, byte ptr [ebp]
    adc ebp, ebx
    add edx, esi
    mov ah, byte ptr [ebp]
    adc ebp, ebx
    rol eax, 010h
    mov dword ptr [edi], eax
    sub ecx, 4
    add edi, 4
    cmp ecx, 4
    jge SHORT scaled_dword
scaled_tail:
    or ecx, ecx
    jle SHORT scaled_done
scaled_byte:
    mov al, byte ptr [ebp]
    add edx, esi
    mov byte ptr [edi], al
    adc ebp, ebx
    inc edi
    dec ecx
    jne SHORT scaled_byte
scaled_done:
    pop ebp
    pop ebx
    pop edi
    pop esi
    leave
    ret
DrawScaledSpan ENDP

DrawSpan16 PROC NEAR
    ; Copy an unscaled byte span, using aligned destination dwords for longer spans.
    enter 0, 0
    push esi
    push edi
    mov eax, dword ptr [ebp + 8]
    mov esi, dword ptr [eax + SPAN_SOURCE]
    mov edi, dword ptr [eax + SPAN_DESTINATION]
    mov ecx, dword ptr [eax + SPAN_WIDTH]
    add esi, dword ptr [eax + SPAN_SKIP]
    cmp ecx, 8
    jl SHORT span_tail
    test edi, 3
    je SHORT span_dword
span_align:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec ecx
    test edi, 3
    jne SHORT span_align
span_dword:
    mov eax, dword ptr [esi]
    sub ecx, 4
    mov dword ptr [edi], eax
    add esi, 4
    add edi, 4
    cmp ecx, 4
    jge SHORT span_dword
span_tail:
    or ecx, ecx
    jle SHORT span_done
span_byte:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec ecx
    jne SHORT span_byte
span_done:
    pop edi
    pop esi
    leave
    ret
DrawSpan16 ENDP

FillTerrainRows PROC NEAR
    ; Fill inclusive left/right row spans with the background index; clip vertically.
    enter 0, 0
    push esi
    push edi
    push ebx
    mov esi, dword ptr [ebp + 8]
    mov edx, dword ptr [ebp + 0Ch]
    mov eax, dword ptr [ebp + 014h]
    cmp eax, dword ptr [viewportClipBottom]
    jg fill_done
    mov ebx, eax
    add ebx, dword ptr [ebp + 018h]
    dec ebx
    cmp ebx, dword ptr [viewportClipTop]
    jl fill_done
    sub eax, dword ptr [viewportClipTop]
    jge SHORT fill_clip_bottom
    add dword ptr [ebp + 018h], eax
    neg eax
    add esi, eax
    add edx, eax
    mov eax, dword ptr [viewportClipTop]
    mov dword ptr [ebp + 014h], eax
fill_clip_bottom:
    sub ebx, dword ptr [viewportClipBottom]
    jle SHORT fill_count
    sub dword ptr [ebp + 018h], ebx
fill_count:
    cmp dword ptr [ebp + 018h], 0
    jle SHORT fill_done
    ; String stores use the flat data selector in ES.
    cld
    push ds
    pop es
    mov ebx, dword ptr [ebp + 014h]
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + ebx*4]
    add edi, dword ptr [gShellSurfacePixels]
    ; Replicate the background byte across EAX (the original retains any high word bits).
    mov eax, dword ptr [gMenuBackground]
    mov ah, al
    mov ebx, eax
    rol eax, 010h
    or eax, ebx
fill_row:
    push edi
    xor ecx, ecx
    xor ebx, ebx
    mov cl, byte ptr [esi]
    mov bl, byte ptr [edx]
    add ecx, dword ptr [ebp + 010h]
    add ebx, dword ptr [ebp + 010h]
    add edi, ecx
    inc esi
    inc edx
    sub ebx, ecx
    inc ebx
    jle SHORT fill_next
    cmp ebx, 0Ch
    jl SHORT fill_tail
    ; Align destination, fill whole dwords, then the remaining 0..3 bytes.
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
fill_tail:
    mov ecx, ebx
    rep stosb
fill_next:
    pop edi
    add edi, dword ptr [surfaceRowStride]
    dec dword ptr [ebp + 018h]
    jne SHORT fill_row
fill_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
FillTerrainRows ENDP

FillTerrainRowsClipped PROC NEAR
    ; The same row fill with inclusive horizontal clipping as well.
    enter 0, 0
    push esi
    push edi
    push ebx
    mov esi, dword ptr [ebp + 8]
    mov edx, dword ptr [ebp + 0Ch]
    mov eax, dword ptr [ebp + 014h]
    cmp eax, dword ptr [viewportClipBottom]
    jg clipped_done
    mov ebx, eax
    add ebx, dword ptr [ebp + 018h]
    dec ebx
    cmp ebx, dword ptr [viewportClipTop]
    jl clipped_done
    sub eax, dword ptr [viewportClipTop]
    jge SHORT clipped_bottom
    add dword ptr [ebp + 018h], eax
    neg eax
    add esi, eax
    add edx, eax
    mov eax, dword ptr [viewportClipTop]
    mov dword ptr [ebp + 014h], eax
clipped_bottom:
    sub ebx, dword ptr [viewportClipBottom]
    jle SHORT clipped_count
    sub dword ptr [ebp + 018h], ebx
clipped_count:
    cmp dword ptr [ebp + 018h], 0
    jle clipped_done
    ; String stores use the flat data selector in ES.
    cld
    push ds
    pop es
    mov ebx, dword ptr [ebp + 014h]
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + ebx*4]
    add edi, dword ptr [gShellSurfacePixels]
    mov eax, dword ptr [gMenuBackground]
    mov ah, al
    mov ebx, eax
    rol eax, 010h
    or eax, ebx
clipped_row:
    push edi
    xor ecx, ecx
    xor ebx, ebx
    mov cl, byte ptr [esi]
    add ecx, dword ptr [ebp + 010h]
    cmp ecx, dword ptr [viewportClipLeft]
    jge SHORT clipped_right
    mov ecx, dword ptr [viewportClipLeft]
clipped_right:
    mov bl, byte ptr [edx]
    add ebx, dword ptr [ebp + 010h]
    cmp ebx, dword ptr [viewportClipRight]
    jle SHORT clipped_fill
    mov ebx, dword ptr [viewportClipRight]
clipped_fill:
    add edi, ecx
    inc esi
    inc edx
    sub ebx, ecx
    inc ebx
    jle SHORT clipped_next
    cmp ebx, 0Ch
    jl SHORT clipped_tail
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
clipped_tail:
    mov ecx, ebx
    rep stosb
clipped_next:
    pop edi
    add edi, dword ptr [surfaceRowStride]
    dec dword ptr [ebp + 018h]
    jne SHORT clipped_row
clipped_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
FillTerrainRowsClipped ENDP
_TEXT ENDS
END
