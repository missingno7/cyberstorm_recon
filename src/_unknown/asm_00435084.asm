; Lane s46: readable hand-assembly reconstruction, pinned WASM 10.6 candidate.
; Names are working labels; see REPORT.md for encoding blockers and object cuts.
.486
.model flat

TILE_SIDE EQU 64
TILE_LAST EQU TILE_SIDE-1
MASK_GROUP_PIXELS EQU 8
DWORD_PIXELS EQU 4

MaskTerrainTile_dst EQU 8
MaskTerrainTile_src EQU 12
MaskTerrainTile_mask EQU 16
MaskTerrainTile_flip_x EQU 20
MaskTerrainTile_flip_y EQU 24

EXTRN maskRowStarts:DWORD ; oracle target 00490670; external data, not embedded code
EXTRN maskRowSteps:DWORD ; oracle target 00490680; external data, not embedded code

_TEXT SEGMENT DWORD PUBLIC USE32 'CODE'
PUBLIC MaskTerrainTile

; Copy selected bytes of a 64x64 tile. BH counts rows, CL groups, CH bits.
MaskTerrainTile PROC NEAR
    enter 0, 0
    push esi
    push edi
    push ebx
    mov eax, dword ptr [ebp + MaskTerrainTile_mask]
    mov esi, dword ptr [ebp + MaskTerrainTile_src]
    mov edi, dword ptr [ebp + MaskTerrainTile_dst]
    mov ebx, dword ptr [ebp + MaskTerrainTile_flip_x]
    xor ebx, dword ptr [ebp + MaskTerrainTile_flip_y]
    and ebx, 3
    add eax, dword ptr [ebx*4 + maskRowStarts]
    mov edx, dword ptr [ebx*4 + maskRowSteps]
    test ebx, 1
    jne mask_reverse
    mov bh, TILE_SIDE
mask_forward_row:
    mov cl, MASK_GROUP_PIXELS
mask_forward_group:
    mov bl, byte ptr [eax]
    or bl, bl
    je mask_forward_skip_group
    cmp bl, 0FFh
    mov ch, MASK_GROUP_PIXELS
    jne mask_forward_bit
    movsd
    movsd
    jmp SHORT mask_forward_next_group
mask_forward_skip_pixel:
    inc esi
    inc edi
    dec ch
    je mask_forward_next_group
mask_forward_bit:
    shr bl, 1
    jae mask_forward_skip_pixel
    movsb
    dec ch
    jne mask_forward_bit
    jmp SHORT mask_forward_next_group
mask_forward_skip_group:
    add edi, 8
    add esi, 8
mask_forward_next_group:
    inc eax
    dec cl
    jne mask_forward_group
    add eax, edx
    dec bh
    jne mask_forward_row
    jmp SHORT mask_done
    ; Opposite mask bit order for a mirrored tile.
mask_reverse:
    mov bh, TILE_SIDE
mask_reverse_row:
    mov cl, MASK_GROUP_PIXELS
mask_reverse_group:
    mov bl, byte ptr [eax]
    or bl, bl
    je mask_reverse_skip_group
    cmp bl, 0FFh
    mov ch, MASK_GROUP_PIXELS
    jne mask_reverse_bit
    movsd
    movsd
    jmp SHORT mask_reverse_next_group
mask_reverse_skip_pixel:
    inc esi
    inc edi
    dec ch
    je mask_reverse_next_group
mask_reverse_bit:
    shl bl, 1
    jae mask_reverse_skip_pixel
    movsb
    dec ch
    jne mask_reverse_bit
    jmp SHORT mask_reverse_next_group
mask_reverse_skip_group:
    add edi, 8
    add esi, 8
mask_reverse_next_group:
    dec eax
    dec cl
    jne mask_reverse_group
    add eax, edx
    dec bh
    jne mask_reverse_row
mask_done:
    pop ebx
    pop edi
    pop esi
    leave
    ret
MaskTerrainTile ENDP
_TEXT ENDS
END
