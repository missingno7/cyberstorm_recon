; Lane s46: readable hand-assembly reconstruction, pinned WASM 10.6 candidate.
; Names are working labels; see REPORT.md for encoding blockers and object cuts.
.486
.model flat

TILE_SIDE EQU 64
TILE_LAST EQU TILE_SIDE-1
MASK_GROUP_PIXELS EQU 8
DWORD_PIXELS EQU 4

DrawTerrainTile_image EQU 8
DrawTerrainTile_screen_x EQU 12
DrawTerrainTile_screen_y EQU 16
DrawTerrainTile_orientation EQU 20
DrawTerrainTileClipped_image EQU 8
DrawTerrainTileClipped_screen_x EQU 12
DrawTerrainTileClipped_screen_y EQU 16
DrawTerrainTileClipped_flags EQU 20
DrawTerrainTileClipped_width EQU 24
DrawTerrainTileClipped_height EQU 28

EXTRN terrainSpanRows:BYTE ; oracle target 004906B0; external data, not embedded code
EXTRN terrainSourceStarts:DWORD ; oracle target 004907B4; external data, not embedded code
EXTRN terrainSourceSteps:DWORD ; oracle target 004907C4; external data, not embedded code
EXTRN terrainSourceStep:DWORD ; oracle target 004907D4; external data, not embedded code
EXTRN W?gShellSurfacePixels$npnv:DWORD
gShellSurfacePixels EQU W?gShellSurfacePixels$npnv ; oracle target 00513890; external data, not embedded code
EXTRN surfaceRowOffsets:DWORD ; oracle target 005138A4; external data, not embedded code
EXTRN surfaceRowStride:DWORD ; oracle target 005138A8; external data, not embedded code
EXTRN W?gDrawRemap$npnv:DWORD
gDrawRemap EQU W?gDrawRemap$npnv ; oracle target 005138E8; external data, not embedded code
EXTRN W?viewportClipLeft$ni:DWORD
viewportClipLeft EQU W?viewportClipLeft$ni ; oracle target 005138FC; external data, not embedded code
EXTRN W?viewportClipTop$ni:DWORD
viewportClipTop EQU W?viewportClipTop$ni ; oracle target 00513900; external data, not embedded code
EXTRN W?viewportClipRight$ni:DWORD
viewportClipRight EQU W?viewportClipRight$ni ; oracle target 00513904; external data, not embedded code
EXTRN W?viewportClipBottom$ni:DWORD
viewportClipBottom EQU W?viewportClipBottom$ni ; oracle target 00513908; external data, not embedded code
EXTRN RenderImage:NEAR ; existing label; C-linkage fixup remains unbound until byte equality

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC DrawTerrainTile
PUBLIC DrawTerrainTileClipped

; Draw row spans from a tile, optionally reflecting the source horizontally.
DrawTerrainTile PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov ebx, dword ptr [ebp + DrawTerrainTile_screen_y]
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + ebx*4]
    add edi, dword ptr [gShellSurfacePixels]
    add edi, dword ptr [ebp + DrawTerrainTile_screen_x]
    mov esi, dword ptr [ebp + DrawTerrainTile_image]
    add esi, dword ptr [esi]
    mov eax, dword ptr [ebp + DrawTerrainTile_orientation]
    mov ebx, dword ptr [eax*4 + terrainSourceSteps]
    add esi, dword ptr [eax*4 + terrainSourceStarts]
    mov dword ptr [terrainSourceStep], ebx
    test eax, 1
    jne tile_reverse
    mov ebx, OFFSET terrainSpanRows
    xor eax, eax
    xor ecx, ecx
tile_forward_row:
    mov cl, byte ptr [ebx]
    cmp cl, 0FFh
    je tile_done
    push esi
    push edi
    add edi, ecx
    add esi, ecx
    mov cl, byte ptr [ebx + 1]
    or cl, cl
    je tile_forward_dwords
tile_forward_leading_bytes:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec cl
    jne tile_forward_leading_bytes
tile_forward_dwords:
    mov cl, byte ptr [ebx + 2]
tile_forward_copy_dword:
    mov eax, dword ptr [esi]
    mov dword ptr [edi], eax
    add esi, 4
    add edi, 4
    dec cl
    jne tile_forward_copy_dword
    mov cl, byte ptr [ebx + 3]
    or cl, cl
    je tile_forward_next_row
tile_forward_trailing_bytes:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    inc esi
    inc edi
    dec cl
    jne tile_forward_trailing_bytes
tile_forward_next_row:
    pop edi
    pop esi
    add esi, dword ptr [terrainSourceStep]
    add edi, dword ptr [surfaceRowStride]
    add ebx, 4
    jmp SHORT tile_forward_row
tile_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
    ; Mirrored source: reverse each dword with BSWAP and walk source backwards.
tile_reverse:
    mov ebx, OFFSET terrainSpanRows
    xor eax, eax
    xor ecx, ecx
tile_reverse_row:
    mov cl, byte ptr [ebx]
    cmp cl, 0FFh
    je tile_reverse_done
    push esi
    push edi
    add edi, ecx
    sub esi, ecx
    mov cl, byte ptr [ebx + 1]
    or cl, cl
    je tile_reverse_dwords
tile_reverse_leading_bytes:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    dec esi
    inc edi
    dec cl
    jne tile_reverse_leading_bytes
tile_reverse_dwords:
    mov cl, byte ptr [ebx + 2]
tile_reverse_copy_dword:
    mov eax, dword ptr [esi - 3]
    bswap eax
    mov dword ptr [edi], eax
    sub esi, 4
    add edi, 4
    dec cl
    jne tile_reverse_copy_dword
    mov cl, byte ptr [ebx + 3]
    or cl, cl
    je tile_reverse_next_row
tile_reverse_trailing_bytes:
    mov al, byte ptr [esi]
    mov byte ptr [edi], al
    dec esi
    inc edi
    dec cl
    jne tile_reverse_trailing_bytes
tile_reverse_next_row:
    pop edi
    pop esi
    add esi, dword ptr [terrainSourceStep]
    add edi, dword ptr [surfaceRowStride]
    add ebx, 4
    jmp SHORT tile_reverse_row
tile_reverse_done:
    jmp SHORT tile_done
DrawTerrainTile ENDP
    EVEN ; one NOP between these two routines

; Fast transparent 64x64 blit, with a general RenderImage fallback.
DrawTerrainTileClipped PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    push ebp
    cmp dword ptr [ebp + DrawTerrainTileClipped_width], TILE_SIDE
    jne clip_general_image
    cmp dword ptr [ebp + DrawTerrainTileClipped_height], TILE_SIDE
    jne clip_general_image
    cmp dword ptr [ebp + DrawTerrainTileClipped_flags], 0
    jne clip_general_image
    mov eax, dword ptr [ebp + DrawTerrainTileClipped_screen_x]
    mov ebx, eax
    add ebx, TILE_LAST
    cmp ebx, dword ptr [viewportClipLeft]
    jl clip_done
    cmp eax, dword ptr [viewportClipRight]
    jg clip_done
    cmp eax, dword ptr [viewportClipLeft]
    jl clip_general_image
    cmp ebx, dword ptr [viewportClipRight]
    jle clip_vertical
clip_general_image:
    push dword ptr [ebp + DrawTerrainTileClipped_height]
    push dword ptr [ebp + DrawTerrainTileClipped_width]
    push dword ptr [ebp + DrawTerrainTileClipped_flags]
    push dword ptr [ebp + DrawTerrainTileClipped_screen_y]
    push dword ptr [ebp + DrawTerrainTileClipped_screen_x]
    push dword ptr [ebp + DrawTerrainTileClipped_image]
    call RenderImage
    add esp, 018h
    pop ebp
    pop ebx
    pop edi
    pop esi
    leave
    ret
clip_vertical:
    xor ecx, ecx
    mov eax, dword ptr [ebp + DrawTerrainTileClipped_screen_y]
    cmp eax, dword ptr [viewportClipBottom]
    jg clip_done
    mov ebx, eax
    add ebx, TILE_LAST
    cmp ebx, dword ptr [viewportClipTop]
    jl clip_done
    sub eax, dword ptr [viewportClipTop]
    jge clip_bottom
    add dword ptr [ebp + DrawTerrainTileClipped_height], eax
    neg eax
    shl eax, 6
    mov ecx, eax
    mov eax, dword ptr [viewportClipTop]
    mov dword ptr [ebp + DrawTerrainTileClipped_screen_y], eax
clip_bottom:
    sub ebx, dword ptr [viewportClipBottom]
    jle clip_height_ready
    sub dword ptr [ebp + DrawTerrainTileClipped_height], ebx
clip_height_ready:
    cmp dword ptr [ebp + DrawTerrainTileClipped_height], 0
    jle clip_done
    ; String operations use the flat data selector for both DS and ES.
    cld
    push ds
    pop es
    mov ebx, dword ptr [ebp + DrawTerrainTileClipped_screen_y]
    mov edi, dword ptr [surfaceRowOffsets]
    mov edi, dword ptr [edi + ebx*4]
    add edi, dword ptr [gShellSurfacePixels]
    add edi, dword ptr [ebp + DrawTerrainTileClipped_screen_x]
    mov esi, dword ptr [ebp + DrawTerrainTileClipped_image]
    add esi, dword ptr [esi]
    add esi, ecx
    mov ecx, dword ptr [ebp + DrawTerrainTileClipped_height]
    mov ch, cl
    ; After frame arguments are loaded EBP holds the optional palette remap.
    mov ebp, dword ptr [gDrawRemap]
    or ebp, ebp
    jne remapped_tile
    xor edx, edx
    ; CH counts scanlines; CL counts sixteen groups of four pixels.
transparent_row:
    mov cl, 010h
    push edi
    ; Preserve destination pixels wherever the source byte is zero.
transparent_dword:
    mov eax, dword ptr [esi]
    or eax, eax
    je transparent_next
    mov ebx, dword ptr [edi]
    or al, al
    je transparent_byte1
    mov bl, al
transparent_byte1:
    or ah, ah
    je transparent_high_word
    mov bh, ah
transparent_high_word:
    rol eax, 010h
    rol ebx, 010h
    or al, al
    je transparent_byte3
    mov bl, al
transparent_byte3:
    or ah, ah
    je transparent_store
    mov bh, ah
transparent_store:
    rol ebx, 010h
    mov dword ptr [edi], ebx
transparent_next:
    add esi, 4
    add edi, 4
    dec cl
    jne transparent_dword
    pop edi
    add edi, dword ptr [surfaceRowStride]
    dec ch
    jne transparent_row
clip_done:
    pop ebp
    pop ebx
    pop edi
    pop esi
    leave
    ret
    ; Remap each nonzero source index before merging with destination.
remapped_tile:
    xor edx, edx
remapped_row:
    mov cl, 010h
    push edi
remapped_dword:
    mov eax, dword ptr [esi]
    or eax, eax
    je remapped_next
    mov ebx, dword ptr [edi]
    or al, al
    je remapped_byte1
    mov dl, al
    mov bl, byte ptr [ebp + edx]
remapped_byte1:
    or ah, ah
    je remapped_high_word
    mov dl, ah
    mov bh, byte ptr [ebp + edx]
remapped_high_word:
    rol eax, 010h
    rol ebx, 010h
    or al, al
    je remapped_byte3
    mov dl, al
    mov bl, byte ptr [ebp + edx]
remapped_byte3:
    or ah, ah
    je remapped_store
    mov dl, ah
    mov bh, byte ptr [ebp + edx]
remapped_store:
    rol ebx, 010h
    mov dword ptr [edi], ebx
remapped_next:
    add esi, 4
    add edi, 4
    dec cl
    jne remapped_dword
    pop edi
    add edi, dword ptr [surfaceRowStride]
    dec ch
    jne remapped_row
    pop ebp
    pop ebx
    pop edi
    pop esi
    leave
    ret
DrawTerrainTileClipped ENDP
_TEXT ENDS
END
