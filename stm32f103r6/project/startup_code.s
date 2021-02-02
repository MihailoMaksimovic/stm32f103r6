.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.extern _msp_stack_pointer

.weak exti_0_handler // zamenio
.thumb_set exti_0_handler, default_handler// zamenio

.weak tim1_update_handler
.thumb_set tim1_update_handler, default_handler

.section .vector_table, "a"
.word _msp_stack_pointer
.word reset_handler
.rept 21
	.word default_handler
.endr
.word exti_0_handler // zamenio
.rept 17
	.word default_handler
.endr

.word tim1_update_handler
.rept 42
	.word default_handler
.endr

.extern main

.extern _lma_data_start
.extern _vma_data_start
.extern _vma_data_end

.section .text.reset_handler
.type reset_handler, %function
reset_handler:
	ldr r0, =_lma_data_start
	ldr r1, =_vma_data_start
	ldr r2, =_vma_data_end
	cmp r1, r2
	beq branch_to_main
copy_loop:
	ldr r3, [r0], #4
	str r3, [r1], #4
	cmp r1, r2
	blo copy_loop
branch_to_main:
	b main
infinite_loop_1:
	b infinite_loop_1

.section .text.default_handler
.type default_handler, %function
default_handler:
infinite_loop_2:
	b infinite_loop_2
	



