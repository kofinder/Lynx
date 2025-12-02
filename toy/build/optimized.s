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
	xorps	%xmm0, %xmm0
	movb	$1, %al
	callq	printf@PLT
	movl	$64, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	.Llynx.string.constant,@object
	.section	.rodata,"a",@progbits
.Llynx.string.constant:
	.asciz	"Result ====>:"
	.size	.Llynx.string.constant, 14

	.type	.Lfmt,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt:
	.asciz	"%s %f\n"
	.size	.Lfmt, 7

	.section	".note.GNU-stack","",@progbits
