	.text
	.file	"main"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3
.LCPI0_0:
	.quad	0x40a3880000000000
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.LformatString, %edi
	movl	$720, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movsd	.LCPI0_0(%rip), %xmm0
	movl	$.LformatString.1, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$20, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.globl	factorial_head_recursion
	.p2align	4, 0x90
	.type	factorial_head_recursion,@function
factorial_head_recursion:
	.cfi_startproc
	movl	$1, %eax
	testl	%edi, %edi
	js	.LBB1_5
	cmpl	$1, %edi
	je	.LBB1_5
	movl	$1, %eax
	movl	%edi, %ecx
	.p2align	4, 0x90
.LBB1_3:
	imull	%edi, %eax
	subl	$1, %ecx
	jb	.LBB1_5
	cmpl	$2, %edi
	movl	%ecx, %edi
	jne	.LBB1_3
.LBB1_5:
	retq
.Lfunc_end1:
	.size	factorial_head_recursion, .Lfunc_end1-factorial_head_recursion
	.cfi_endproc

	.globl	factorial_tail_recursion
	.p2align	4, 0x90
	.type	factorial_tail_recursion,@function
factorial_tail_recursion:
	.cfi_startproc
	movl	%esi, %eax
	testl	%edi, %edi
	js	.LBB2_5
	cmpl	$1, %edi
	je	.LBB2_5
	movl	%edi, %ecx
	.p2align	4, 0x90
.LBB2_3:
	imull	%edi, %eax
	subl	$1, %ecx
	jb	.LBB2_5
	cmpl	$2, %edi
	movl	%ecx, %edi
	jne	.LBB2_3
.LBB2_5:
	retq
.Lfunc_end2:
	.size	factorial_tail_recursion, .Lfunc_end2-factorial_tail_recursion
	.cfi_endproc

	.globl	profit_calculation
	.p2align	4, 0x90
	.type	profit_calculation,@function
profit_calculation:
	.cfi_startproc
	subsd	%xmm1, %xmm0
	retq
.Lfunc_end3:
	.size	profit_calculation, .Lfunc_end3-profit_calculation
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
	jne	.LBB4_2
	addl	$1, %eax
	retq
.LBB4_2:
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	sarl	%ecx
	movl	%ecx, %eax
	retq
.Lfunc_end4:
	.size	ub_demo_function, .Lfunc_end4-ub_demo_function
	.cfi_endproc

	.type	.LformatString,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.LformatString:
	.asciz	"%d\n"
	.size	.LformatString, 4

	.type	.LformatString.1,@object
.LformatString.1:
	.asciz	"%lf\n"
	.size	.LformatString.1, 5

	.section	".note.GNU-stack","",@progbits
