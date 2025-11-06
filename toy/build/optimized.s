	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.Lscanf_fmt.1, %edi
	callq	puts@PLT
	leaq	4(%rsp), %rsi
	movl	$.Lscanf_fmt.1, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	$32, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

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
	jne	.LBB1_2
	addl	$1, %eax
	retq
.LBB1_2:
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	sarl	%ecx
	movl	%ecx, %eax
	retq
.Lfunc_end1:
	.size	ub_demo_function, .Lfunc_end1-ub_demo_function
	.cfi_endproc

	.type	.Lscanf_fmt.1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt.1:
	.asciz	"%d"
	.size	.Lscanf_fmt.1, 3

	.section	".note.GNU-stack","",@progbits
