	.file	"main"
	.text
	.globl	main
	.p2align	4
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.Lfmt.12, %edi
	movl	$.Llynx.string.constant, %esi
	movl	$4144, %edx
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.Lfmt.12, %edi
	movl	$.Llynx.string.constant.3, %esi
	movl	$4144, %edx
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.Lfmt.12, %edi
	movl	$.Llynx.string.constant.7, %esi
	movl	$128, %edx
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.Lfmt.12, %edi
	movl	$.Llynx.string.constant.11, %esi
	movl	$256, %edx
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
.Llynx.string.constant:
	.asciz	"PopCount: "
	.size	.Llynx.string.constant, 11

	.type	.Llynx.string.constant.3,@object
	.p2align	4, 0x0
.Llynx.string.constant.3:
	.asciz	"countLeadingZeros: "
	.size	.Llynx.string.constant.3, 20

	.type	.Llynx.string.constant.7,@object
	.p2align	4, 0x0
.Llynx.string.constant.7:
	.asciz	"countTrailingZeros: "
	.size	.Llynx.string.constant.7, 21

	.type	.Llynx.string.constant.11,@object
.Llynx.string.constant.11:
	.asciz	"bitReverse: "
	.size	.Llynx.string.constant.11, 13

	.type	.Lfmt.12,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt.12:
	.asciz	"%s %d\n"
	.size	.Lfmt.12, 7

	.section	".note.GNU-stack","",@progbits
