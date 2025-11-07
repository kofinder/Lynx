	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$1048, %rsp
	.cfi_def_cfa_offset 1072
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movl	$.Lfmt.9, %edi
	movl	$.Llynx.string.constant, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	24(%rsp), %rbx
	movl	$.Lread_line_fmt, %edi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	$.Lfmt.9, %edi
	movl	$.Llynx.string.constant.1, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	20(%rsp), %rsi
	movl	$.Lscanf_fmt.11, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	20(%rsp), %ebp
	movl	$.Lfmt.9, %edi
	movl	$.Llynx.string.constant.4, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	16(%rsp), %rsi
	movl	$.Lscanf_fmt.7, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movss	16(%rsp), %xmm0
	movss	%xmm0, 8(%rsp)
	movl	$.Lfmt.9, %edi
	movl	$.Llynx.string.constant.8, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	12(%rsp), %rsi
	movl	$.Lscanf_fmt.11, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	cmpl	$0, 12(%rsp)
	movss	8(%rsp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movl	$.L__unnamed_1, %eax
	movl	$.L__unnamed_2, %ecx
	cmoveq	%rax, %rcx
	movl	$.Lfmt.13, %edi
	movq	%rbx, %rsi
	movl	%ebp, %edx
	movb	$1, %al
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$1048, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
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

	.type	.Llynx.string.constant,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant:
	.asciz	"Enter your name: "
	.size	.Llynx.string.constant, 18

	.type	.Lread_line_fmt,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lread_line_fmt:
	.asciz	"%1023[^\n]"
	.size	.Lread_line_fmt, 10

	.type	.Llynx.string.constant.1,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.1:
	.asciz	"Enter your age: "
	.size	.Llynx.string.constant.1, 17

	.type	.Llynx.string.constant.4,@object
	.p2align	4
.Llynx.string.constant.4:
	.asciz	"Enter a height: "
	.size	.Llynx.string.constant.4, 17

	.type	.Lscanf_fmt.7,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt.7:
	.asciz	"%f"
	.size	.Lscanf_fmt.7, 3

	.type	.Llynx.string.constant.8,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.8:
	.asciz	"Are you single? (0 = false, 1 = true): "
	.size	.Llynx.string.constant.8, 40

	.type	.Lfmt.9,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt.9:
	.asciz	"%s"
	.size	.Lfmt.9, 3

	.type	.Lscanf_fmt.11,@object
.Lscanf_fmt.11:
	.asciz	"%d"
	.size	.Lscanf_fmt.11, 3

	.type	.Lfmt.13,@object
.Lfmt.13:
	.asciz	"Name: %s, Age: %d, Height: %f, Status: %s"
	.size	.Lfmt.13, 42

	.type	.L__unnamed_1,@object
.L__unnamed_1:
	.asciz	"false"
	.size	.L__unnamed_1, 6

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"true"
	.size	.L__unnamed_2, 5

	.section	".note.GNU-stack","",@progbits
