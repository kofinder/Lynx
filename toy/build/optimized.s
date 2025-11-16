	.file	"main"
	.text
	.globl	main
	.p2align	4
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.Lfmt, %edi
	movl	$.Llynx.string.constant, %esi
	movl	$-2147483648, %edx
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$32, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	.Llynx.string.constant,@object
	.section	.rodata,"a",@progbits
	.p2align	4, 0x0
.Llynx.string.constant:
	.asciz	"Max Integer limit is: "
	.size	.Llynx.string.constant, 23

	.type	.Lfmt,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt:
	.asciz	"%s %d\n"
	.size	.Lfmt, 7

	.section	".note.GNU-stack","",@progbits
