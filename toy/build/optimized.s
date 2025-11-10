	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	subq	$1080, %rsp
	.cfi_def_cfa_offset 1136
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	56(%rsp), %rsi
	movl	$.Lread_line_fmt, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.2, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	28(%rsp), %rsi
	movl	$.Lscanf_fmt, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	28(%rsp), %r15d
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.5, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	10(%rsp), %rsi
	movl	$.Lscanf_fmt.8, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movzwl	10(%rsp), %r12d
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.9, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	24(%rsp), %rsi
	movl	$.Lscanf_fmt.32, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	24(%rsp), %r13d
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.13, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	48(%rsp), %rsi
	movl	$.Lscanf_fmt.16, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movq	48(%rsp), %r14
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.17, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	20(%rsp), %rsi
	movl	$.Lscanf_fmt.20, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movss	20(%rsp), %xmm0
	movss	%xmm0, 12(%rsp)
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.21, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	40(%rsp), %rsi
	movl	$.Lscanf_fmt.24, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movsd	40(%rsp), %xmm0
	movsd	%xmm0, 32(%rsp)
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.25, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	8(%rsp), %rsi
	movl	$.Lscanf_fmt.28, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movsbl	8(%rsp), %ebp
	movl	$.Lfmt.30, %edi
	movl	$.Llynx.string.constant.29, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	16(%rsp), %rsi
	movl	$.Lscanf_fmt.32, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	cmpl	$0, 16(%rsp)
	movss	12(%rsp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movl	$.L__unnamed_1, %eax
	movl	$.L__unnamed_2, %ebx
	cmoveq	%rax, %rbx
	movl	$.Lfmt.34, %edi
	leaq	56(%rsp), %rsi
	movl	%r15d, %edx
	movl	%r12d, %ecx
	movl	%r13d, %r8d
	movq	%r14, %r9
	movsd	32(%rsp), %xmm1
	movb	$2, %al
	pushq	%rbx
	.cfi_adjust_cfa_offset 8
	pushq	%rbp
	.cfi_adjust_cfa_offset 8
	callq	printf@PLT
	addq	$16, %rsp
	.cfi_adjust_cfa_offset -16
	xorl	%eax, %eax
	addq	$1080, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
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
	.asciz	"Enter a line of text: "
	.size	.Llynx.string.constant, 23

	.type	.Lread_line_fmt,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lread_line_fmt:
	.asciz	"%1023[^\n]"
	.size	.Lread_line_fmt, 10

	.type	.Llynx.string.constant.2,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.2:
	.asciz	"Enter a byte value: "
	.size	.Llynx.string.constant.2, 21

	.type	.Lscanf_fmt,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt:
	.asciz	"%hhd"
	.size	.Lscanf_fmt, 5

	.type	.Llynx.string.constant.5,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.5:
	.asciz	"Enter a short value: "
	.size	.Llynx.string.constant.5, 22

	.type	.Lscanf_fmt.8,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt.8:
	.asciz	"%hd"
	.size	.Lscanf_fmt.8, 4

	.type	.Llynx.string.constant.9,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.9:
	.asciz	"Enter an int value: "
	.size	.Llynx.string.constant.9, 21

	.type	.Llynx.string.constant.13,@object
	.p2align	4
.Llynx.string.constant.13:
	.asciz	"Enter a long value: "
	.size	.Llynx.string.constant.13, 21

	.type	.Lscanf_fmt.16,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt.16:
	.asciz	"%ld"
	.size	.Lscanf_fmt.16, 4

	.type	.Llynx.string.constant.17,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.17:
	.asciz	"Enter a float value: "
	.size	.Llynx.string.constant.17, 22

	.type	.Lscanf_fmt.20,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt.20:
	.asciz	"%f"
	.size	.Lscanf_fmt.20, 3

	.type	.Llynx.string.constant.21,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.21:
	.asciz	"Enter a double value: "
	.size	.Llynx.string.constant.21, 23

	.type	.Lscanf_fmt.24,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt.24:
	.asciz	"%lf"
	.size	.Lscanf_fmt.24, 4

	.type	.Llynx.string.constant.25,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.25:
	.asciz	"Enter a char value: "
	.size	.Llynx.string.constant.25, 21

	.type	.Lscanf_fmt.28,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lscanf_fmt.28:
	.asciz	"%c"
	.size	.Lscanf_fmt.28, 3

	.type	.Llynx.string.constant.29,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.29:
	.asciz	"Enter a boolean value (0 = false, 1 = true): "
	.size	.Llynx.string.constant.29, 46

	.type	.Lfmt.30,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt.30:
	.asciz	"%s"
	.size	.Lfmt.30, 3

	.type	.Lscanf_fmt.32,@object
.Lscanf_fmt.32:
	.asciz	"%d"
	.size	.Lscanf_fmt.32, 3

	.type	.Lfmt.34,@object
.Lfmt.34:
	.asciz	"You entered:\\nLine: %s\\nByte: %d\\nShort: %d\\nInt: %d\\nLong: %ld\\nFloat: %f\\nDouble: %lf\\nChar: %c\\nBool: %s"
	.size	.Lfmt.34, 108

	.type	.L__unnamed_1,@object
.L__unnamed_1:
	.asciz	"false"
	.size	.L__unnamed_1, 6

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"true"
	.size	.L__unnamed_2, 5

	.section	".note.GNU-stack","",@progbits
