section	.text
global  swap_blocks
; ebp + 8 - generated index
; ebp + 12 - address of start
; ebp + 16 - ImageInfo*

; extern void swap_blocks(int i, char* start, ImageInfo* ptr);
; Swaps the block pointed at by start with block number i. Block nr 0 is in the borrom left of the puzzle
; Return value: void
swap_blocks:
	push	ebp 
	mov    	ebp,esp
	push	ebx
	push	esi
	push	edi
	sub		esp, 0xc ; alocating memory for width_for_block(0x4), height_for_block(0x8)
	mov		ebx, [ebp+16] ; ImgInfo*
	mov		eax, [ebx+8] ; linebytes
	mov		edi, [ebx+12] ; start buffer
	xor		edx, edx
	mov		DWORD[ebp-0x4], 4
	div		DWORD[ebp-0x4] ; width_for_block = linebytes/width(4)
	mov		[ebp-0x4], eax
	mov		eax, [ebx+4] ; ImgInfo->height
	mov		DWORD[ebp-0x8], 3 ; height_for_block = ImgInfo->height/height(3)
	xor		edx, edx
	div 	DWORD[ebp-0x8] ; eax = height_for_block
	mov		[ebp-0x8], eax
	mov		ebx, [ebx+8] ; linebytes
	mov		eax, [ebp+8] ; generated index

	; calculating start adress of a block nr i
	shr		eax, 2 ; getting the row of block nr i(0,1,2,3)
	imul	eax, ebx ; linebytes * row
	imul	eax, [ebp-0x8] ; linebytes * row * height
	mov		edx, [ebp+8]
	and		edx, 3 ; getting column of block nr i(0,1,2)
	imul	edx, [ebp-0x4] ; column * width 
	add		eax, edx ; eax holds the distance from left bottom pixel to the left bottom pixel of block nr i
	add		edi, eax ;pointer to generated block
	; calculating address of the pixel above left upper pixel of start block, used to check boundaries
	mov		eax, [ebp-0x8] ; height_for_block
	imul	eax, ebx ; height_for_block * linebytes
	mov		esi, [ebp+12] ;address of start
	add		eax, esi ; eax - points to end of block
					 ; edi - points to start of block
	cmp		esi, edi 
	je		exit ; check if we are not swaping one block with itself
	mov		ecx, [ebp-0x4] ; width_of_block
repaint_block_loop:
	; swapping bytes
	mov 	dh, [edi]
	mov		dl, [esi] 
	mov		[esi], dh
	mov		[edi], dl
	inc		esi
	inc		edi
	dec		ecx
	jnz		repaint_block_loop ; we have swapped an entire line
shift_pointer:
	mov		ebx, [ebp+16] ; ImgInfo*
	add		edi, [ebx+0x8] ; ; calculating address of the last pixel in a line above *edi
	add		esi, [ebx+0x8] ; calculating address of the last pixel in a line above *esi
	mov		ecx, [ebp-0x4] ; restoring width_for_block
	sub		edi, ecx ; now edi points add first pixel in line
	sub		esi, ecx ; now esi points add first pixel in line
	cmp		esi, eax ; check if we haven't reached end of block
	jge		exit
	jmp		repaint_block_loop
exit:
	add		esp, 0xc
	pop 	edi
	pop		esi
	pop		ebx
	pop		ebp
	ret