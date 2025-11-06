	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$1040, %rsp
	.cfi_def_cfa_offset 1056
	.cfi_offset %rbx, -16
	movl	$.Lfmt.2, %edi
	movl	$.Llynx.string.constant, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	16(%rsp), %rbx
	movl	$.Lread_line_fmt, %edi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	$.Lfmt.2, %edi
	movl	$.Llynx.string.constant.1, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	leaq	12(%rsp), %rsi
	movl	$.Lscanf_fmt, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	12(%rsp), %ecx
	movl	$.Lfmt.5, %edi
	movl	$.Llynx.string.constant.4, %esi
	movq	%rbx, %rdx
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$32, %eax
	addq	$1040, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
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

	.type	.Lfmt.2,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt.2:
	.asciz	"%s"
	.size	.Lfmt.2, 3

	.type	.Lscanf_fmt,@object
.Lscanf_fmt:
	.asciz	"%d"
	.size	.Lscanf_fmt, 3

	.type	.Llynx.string.constant.4,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant.4:
	.asciz	"Name: {}, Age: {}"
	.size	.Llynx.string.constant.4, 18

	.type	.Lfmt.5,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt.5:
	.asciz	"%s %s %d\n"
	.size	.Lfmt.5, 10

	.section	".note.GNU-stack","",@progbits
