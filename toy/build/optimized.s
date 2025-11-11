	.text
	.file	"main"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.Lfmt.4, %edi
	movl	$.Llynx.string.constant, %esi
	movl	$2147483647, %edx
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.Lfmt.4, %edi
	movl	$.Llynx.string.constant.3, %esi
	movl	$10, %edx
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	.Llynx.string.constant,@object
	.section	.rodata,"a",@progbits
	.p2align	4
.Llynx.string.constant:
	.asciz	"Max Integer limit is: "
	.size	.Llynx.string.constant, 23

	.type	.Llynx.string.constant.3,@object
	.p2align	4
.Llynx.string.constant.3:
	.asciz	"instance method call check: "
	.size	.Llynx.string.constant.3, 29

	.type	.Lfmt.4,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt.4:
	.asciz	"%s %d\n"
	.size	.Lfmt.4, 7

	.section	".note.GNU-stack","",@progbits
