section	.text
global  swap_blocks
; rdi - block_A_start
; rsi - block_B_start
; rdx - ImageInfo

; extern void swap_blocks(char *block_A_start, char *block_B_start, ImageInfo* info);
; Swaps blocks of given begin adresses
; Return value: void
swap_blocks:
	push	rbp
	mov    	rbp, rsp
	push 	r12

	mov r12, [rdx+8] ; r12 - line bytes

	;r8-line
	;r9-column
	mov r8, 79 ; 80 lines per block
swap_loop_new_line:
	mov r9, 232 ; the last pixel in the line of a block
swap_loop_begin:
	mov rax, r8
	mul r12
	add rax, r9 ; rax is the offset

	; Swap 8 bytes in a row
	mov r10, QWORD [rdi+rax]
	mov r11, QWORD [rsi+rax]
	mov QWORD [rdi+rax], r11
	mov QWORD [rsi+rax], r10

	sub r9, 8 ; swap 8 bytes at a time (each line is 80 pixels)
	jns swap_loop_begin ; the column number is >= 0
	sub r8, 1
	jns swap_loop_new_line ; the line is >=0

	pop 	r12
	pop		rbp
	ret