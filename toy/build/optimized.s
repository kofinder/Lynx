	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$24, %edi
	callq	LYNX_GC_ALLOC_chatuser@PLT
	movl	$.Llynx.string.constant, %esi
	movl	$.Llynx.string.constant.1, %edx
	movq	%rax, %rdi
	movl	$30, %ecx
	movl	$1, %r8d
	callq	_ZN8ChatUserC1ESsSsib
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.section	.text._ZN8ChatUserC1ESsSsib,"axG",@progbits,_ZN8ChatUserC1ESsSsib,comdat
	.weak	_ZN8ChatUserC1ESsSsib
	.p2align	4, 0x90
	.type	_ZN8ChatUserC1ESsSsib,@function
_ZN8ChatUserC1ESsSsib:
	.cfi_startproc
	movq	%rsi, 16(%rdi)
	movq	%rdx, (%rdi)
	movl	%ecx, 12(%rdi)
	andl	$1, %r8d
	movb	%r8b, 8(%rdi)
	retq
.Lfunc_end1:
	.size	_ZN8ChatUserC1ESsSsib, .Lfunc_end1-_ZN8ChatUserC1ESsSsib
	.cfi_endproc

	.text
	.globl	ub_demo_function
	.p2align	4, 0x90
	.type	ub_demo_function,@function
ub_demo_function:
	.cfi_startproc
	shll	%cl, %eax
	addl	%eax, %ecx
	leal	(%rcx,%rcx), %edx
	addl	%ecx, %edx
	addl	%eax, %edx
	leal	(%rcx,%rdx), %eax
	addl	$1, %eax
	movl	%eax, -4(%rsp)
	xorl	%ecx, %ecx
	testb	%cl, %cl
	jne	.LBB2_2
	addl	$1, %eax
	retq
.LBB2_2:
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	sarl	%ecx
	movl	%ecx, %eax
	retq
.Lfunc_end2:
	.size	ub_demo_function, .Lfunc_end2-ub_demo_function
	.cfi_endproc

	.type	.Llynx.string.constant,@object
	.section	.rodata,"a",@progbits
.Llynx.string.constant:
	.asciz	"id123"
	.size	.Llynx.string.constant, 6

	.type	.Llynx.string.constant.1,@object
.Llynx.string.constant.1:
	.asciz	"Ko Thein"
	.size	.Llynx.string.constant.1, 9

	.section	".note.GNU-stack","",@progbits
