; Indexed blitters for encoded packets, packed transparent pixels, and a two-by-two marker.
.486
.model flat

RenderSpecialImage_image EQU 8
RenderSpecialImage_x EQU 12
RenderSpecialImage_y EQU 16

IMAGE_WIDTH EQU 4
IMAGE_HEIGHT EQU 6
PACKET_COUNT_MASK EQU 7Fh
DWORD_ALIGN_MASK EQU 3
DWORD_MASK EQU -4

DrawTransparentBitmap_x EQU 8
DrawTransparentBitmap_y EQU 12
DrawTransparentBitmap_width EQU 16
DrawTransparentBitmap_height EQU 20
DrawTransparentBitmap_pixels EQU 24
DrawTransparentBitmap_palette EQU 28

SimMarkPosition_x EQU 8
SimMarkPosition_y EQU 12
SimMarkPosition_color EQU 20

EXTRN W?viewportClipLeft$ni:DWORD
viewportClipLeft EQU W?viewportClipLeft$ni
EXTRN W?viewportClipTop$ni:DWORD
viewportClipTop EQU W?viewportClipTop$ni
EXTRN W?viewportClipRight$ni:DWORD
viewportClipRight EQU W?viewportClipRight$ni
EXTRN W?viewportClipBottom$ni:DWORD
viewportClipBottom EQU W?viewportClipBottom$ni
EXTRN surfaceRowOffsets:DWORD
EXTRN W?gShellSurfacePixels$npnv:DWORD
gShellSurfacePixels EQU W?gShellSurfacePixels$npnv
EXTRN W?gDrawRemap$npnv:DWORD
gDrawRemap EQU W?gDrawRemap$npnv
EXTRN surfaceRowStride:DWORD

_DATA SEGMENT DWORD PUBLIC USE32 'DATA'
rowDestination dd 0
rowsRemaining dd 0
rleLeft dd 0
rleTop dd 0
_DATA ENDS

; Consume ECX bytes at EDI: byte prefix, aligned dwords, byte tail.
AlignedPacket MACRO byteOp, dwordOp
    mov ebx, ecx
    mov ecx, edi
    neg ecx
    and ecx, DWORD_ALIGN_MASK
    sub ebx, ecx
    rep byteOp
    mov ecx, ebx
    and ecx, DWORD_MASK
    sub ebx, ecx
    shr ecx, 2
    rep dwordOp
    mov ecx, ebx
    rep byteOp
ENDM

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC RenderSpecialImage
PUBLIC RleAlignedFill
PUBLIC RleAlignedCopy
PUBLIC RleMappedAlignedFill
PUBLIC RleMappedAlignedCopy
PUBLIC DrawTransparentBitmap
PUBLIC SimMarkPosition

; Decode a run-length image only when its whole rectangle fits in the viewport.
RenderSpecialImage PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov esi, dword ptr [ebp + RenderSpecialImage_image]
    mov eax, dword ptr [ebp + RenderSpecialImage_x]
    mov ebx, dword ptr [ebp + RenderSpecialImage_y]
    mov dword ptr [rleLeft], eax
    mov dword ptr [rleTop], ebx
    cmp eax, dword ptr [viewportClipLeft]
    jl rle_done
    cmp ebx, dword ptr [viewportClipTop]
    jl rle_done
    movzx ecx, word ptr [esi + IMAGE_WIDTH]
    movzx edx, word ptr [esi + IMAGE_HEIGHT]
    mov dword ptr [rowsRemaining], edx
    add ecx, eax
    add edx, ebx
    dec ecx
    dec edx
    cmp ecx, dword ptr [viewportClipRight]
    jg rle_done
    cmp edx, dword ptr [viewportClipBottom]
    jg rle_done
    cld
    push ds
    pop es
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + ebx*4]
    add edi, dword ptr [gShellSurfacePixels]
    add edi, dword ptr [rleLeft]
    mov dword ptr [rowDestination], edi
    add esi, dword ptr [esi]
    xor eax, eax
    mov edx, PACKET_COUNT_MASK
    mov ebx, dword ptr [gDrawRemap]
    or ebx, ebx
    jne mapped_begin
    jmp SHORT plain_packet
plain_next_row:
    dec dword ptr [rowsRemaining]
    je rle_done
    mov edi, dword ptr [surfaceRowStride]
    add dword ptr [rowDestination], edi
    mov edi, dword ptr [rowDestination]
plain_packet:
    ; A zero count ends the row. The high bit selects a repeat; color zero skips it.
    mov al, byte ptr [esi]
    inc esi
    mov ecx, eax
    and ecx, edx
    je plain_next_row
    or al, al
    jns plain_literal
    mov al, byte ptr [esi]
    inc esi
    or al, al
    jne plain_repeat
    add edi, ecx
    jmp SHORT plain_packet
plain_repeat:
    mov byte ptr [edi], al
    inc edi
    dec ecx
    jne plain_repeat
    jmp SHORT plain_packet

; Aligned repeated-color path. Resume the plain packet decoder with EAX cleared.
RleAlignedFill LABEL NEAR
    mov ah, al
    mov ebx, eax
    rol eax, 16
    or eax, ebx
    AlignedPacket stosb, stosd
    xor eax, eax
    jmp SHORT plain_packet
plain_literal:
    ; Positive packet: copy literal indices, including zero.
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec ecx
    jne plain_literal
    jmp SHORT plain_packet

; Aligned literal path. The ordinary literal loop above copies one byte at a time.
RleAlignedCopy LABEL NEAR
    AlignedPacket movsb, movsd
    xor eax, eax
    jmp SHORT plain_packet
rle_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
mapped_begin:
    jmp SHORT mapped_packet
mapped_next_row:
    dec dword ptr [rowsRemaining]
    je rle_done
    mov edi, dword ptr [surfaceRowStride]
    add dword ptr [rowDestination], edi
    mov edi, dword ptr [rowDestination]
mapped_packet:
    mov al, byte ptr [esi]
    inc esi
    mov ecx, eax
    and ecx, edx
    je mapped_next_row
    or al, al
    jns mapped_literal
    mov al, byte ptr [esi]
    inc esi
    or al, al
    jne mapped_color
    add edi, ecx
    jmp SHORT mapped_packet
mapped_color:
    mov al, byte ptr [ebx + eax]
mapped_repeat:
    mov byte ptr [edi], al
    inc edi
    dec ecx
    jne mapped_repeat
    jmp SHORT mapped_packet

; Aligned fill in the mapped decoder. Its next packet uses the same shared frame.
RleMappedAlignedFill LABEL NEAR
    mov ah, al
    mov ebx, eax
    rol eax, 16
    or eax, ebx
    AlignedPacket stosb, stosd
    xor eax, eax
    jmp SHORT mapped_packet
mapped_literal:
    ; Translate each literal index through EBX.
    mov al, byte ptr [esi]
    mov al, byte ptr [ebx + eax]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec ecx
    jne mapped_literal
    jmp SHORT mapped_packet

; Aligned copy tail of the mapped decoder. Return to its packet reader.
RleMappedAlignedCopy LABEL NEAR
    AlignedPacket movsb, movsd
    xor eax, eax
    jmp SHORT mapped_packet
RenderSpecialImage ENDP

; Align the next framed entry after the last packet-loop branch.
ALIGN 4

; Copy nonzero bytes of a packed rectangle, optionally translating through a palette.
DrawTransparentBitmap PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov ebx, dword ptr [ebp + DrawTransparentBitmap_y]
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + ebx*4]
    add edi, dword ptr [gShellSurfacePixels]
    add edi, dword ptr [ebp + DrawTransparentBitmap_x]
    mov esi, dword ptr [ebp + DrawTransparentBitmap_pixels]
    xor eax, eax
    mov ebx, dword ptr [ebp + DrawTransparentBitmap_palette]
    or ebx, ebx
    je plain_row
mapped_row:
    mov ecx, dword ptr [ebp + DrawTransparentBitmap_width]
    push edi
    cmp ecx, 4
    jl mapped_pixel
mapped_skip_zeros:
    ; Skip groups of four transparent pixels before starting the byte loop.
    mov edx, dword ptr [esi]
    or edx, edx
    jne mapped_pixel
    sub ecx, 4
    add esi, 4
    add edi, 4
    cmp ecx, 4
    jge mapped_skip_zeros
    or ecx, ecx
    je mapped_row_done
mapped_pixel:
    mov al, byte ptr [esi]
    or al, al
    je mapped_advance
    mov al, byte ptr [ebx + eax]
    mov byte ptr [edi], al
mapped_advance:
    inc esi
    inc edi
    dec ecx
    jne mapped_pixel
mapped_row_done:
    pop edi
    add edi, dword ptr [surfaceRowStride]
    dec dword ptr [ebp + DrawTransparentBitmap_height]
    jne mapped_row
    jmp SHORT bitmap_done
plain_row:
    mov ecx, dword ptr [ebp + DrawTransparentBitmap_width]
    push edi
    cmp ecx, 4
    jl plain_pixel
plain_skip_zeros:
    mov edx, dword ptr [esi]
    or edx, edx
    jne plain_pixel
    sub ecx, 4
    add esi, 4
    add edi, 4
    cmp ecx, 4
    jge plain_skip_zeros
    or ecx, ecx
    je plain_row_done
plain_pixel:
    mov al, byte ptr [esi]
    or al, al
    je plain_advance
    mov byte ptr [edi], al
plain_advance:
    inc esi
    inc edi
    dec ecx
    jne plain_pixel
plain_row_done:
    pop edi
    add edi, dword ptr [surfaceRowStride]
    dec dword ptr [ebp + DrawTransparentBitmap_height]
    jne plain_row
bitmap_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
DrawTransparentBitmap ENDP

; Write a two-by-two block using the low byte of the fourth argument.
SimMarkPosition PROC NEAR
    enter 0, 0
    mov eax, dword ptr [ebp + SimMarkPosition_y]
    mov edx, dword ptr [surfaceRowOffsets]
    mov edx, dword ptr [edx + eax*4]
    add edx, dword ptr [gShellSurfacePixels]
    add edx, dword ptr [ebp + SimMarkPosition_x]
    mov eax, dword ptr [ebp + SimMarkPosition_color]
    mov byte ptr [edx], al
    mov byte ptr [edx + 1], al
    add edx, dword ptr [surfaceRowStride]
    mov byte ptr [edx], al
    mov byte ptr [edx + 1], al
    leave
    ret
SimMarkPosition ENDP
_TEXT ENDS
END
