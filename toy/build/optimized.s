	.file	"main"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0
.LCPI0_0:
	.quad	0x4031520ce0000000
.LCPI0_1:
	.quad	0x4281ae3a40000000
.LCPI0_2:
	.quad	0x7ff0000000000000
.LCPI0_3:
	.quad	0x4016d0ac60000000
.LCPI0_4:
	.quad	0x40207527c0000000
.LCPI0_5:
	.quad	0x4003d12500000000
.LCPI0_6:
	.quad	0xbfeffe0000000000
.LCPI0_7:
	.quad	0xbf96a07f00000000
.LCPI0_9:
	.quad	0x3ff9145420000000
.LCPI0_11:
	.quad	0x4072c00000000000
	.section	.rodata.cst4,"aM",@progbits,4
	.p2align	2, 0x0
.LCPI0_8:
	.long	0x43960000
.LCPI0_10:
	.long	0x40a00000
	.text
	.globl	main
	.p2align	4
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movsd	.LCPI0_0(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_1(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.3, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_2(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.7, %esi
	movb	$1, %al
	callq	printf@PLT
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.11, %esi
	movsd	.LCPI0_2(%rip), %xmm0
	movb	$1, %al
	callq	printf@PLT
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.15, %esi
	movsd	.LCPI0_2(%rip), %xmm0
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_3(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.19, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_4(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.23, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_5(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.27, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_6(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.31, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_7(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.35, %esi
	movb	$1, %al
	callq	printf@PLT
	movss	.LCPI0_8(%rip), %xmm0
	callq	tanf@PLT
	cvtss2sd	%xmm0, %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.39, %esi
	movb	$1, %al
	callq	printf@PLT
	movss	.LCPI0_8(%rip), %xmm0
	callq	acosf@PLT
	cvtss2sd	%xmm0, %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.43, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_9(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.47, %esi
	movb	$1, %al
	callq	printf@PLT
	movss	.LCPI0_10(%rip), %xmm1
	movss	.LCPI0_8(%rip), %xmm0
	callq	atan2f@PLT
	cvtss2sd	%xmm0, %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.51, %esi
	movb	$1, %al
	callq	printf@PLT
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.55, %esi
	movsd	.LCPI0_2(%rip), %xmm0
	movb	$1, %al
	callq	printf@PLT
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.59, %esi
	movsd	.LCPI0_2(%rip), %xmm0
	movb	$1, %al
	callq	printf@PLT
	movss	.LCPI0_8(%rip), %xmm0
	callq	tanhf@PLT
	cvtss2sd	%xmm0, %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.63, %esi
	movb	$1, %al
	callq	printf@PLT
	movsd	.LCPI0_11(%rip), %xmm0
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.67, %esi
	movb	$1, %al
	callq	printf@PLT
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.71, %esi
	movsd	.LCPI0_11(%rip), %xmm0
	movb	$1, %al
	callq	printf@PLT
	movl	$.Lfmt.76, %edi
	movl	$.Llynx.string.constant.75, %esi
	movsd	.LCPI0_11(%rip), %xmm0
	movb	$1, %al
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
	.asciz	"sqrt: "
	.size	.Llynx.string.constant, 7

	.type	.Llynx.string.constant.3,@object
.Llynx.string.constant.3:
	.asciz	"pow: "
	.size	.Llynx.string.constant.3, 6

	.type	.Llynx.string.constant.7,@object
.Llynx.string.constant.7:
	.asciz	"exp: "
	.size	.Llynx.string.constant.7, 6

	.type	.Llynx.string.constant.11,@object
.Llynx.string.constant.11:
	.asciz	"exp2: "
	.size	.Llynx.string.constant.11, 7

	.type	.Llynx.string.constant.15,@object
.Llynx.string.constant.15:
	.asciz	"exp10: "
	.size	.Llynx.string.constant.15, 8

	.type	.Llynx.string.constant.19,@object
.Llynx.string.constant.19:
	.asciz	"log: "
	.size	.Llynx.string.constant.19, 6

	.type	.Llynx.string.constant.23,@object
.Llynx.string.constant.23:
	.asciz	"log2: "
	.size	.Llynx.string.constant.23, 7

	.type	.Llynx.string.constant.27,@object
.Llynx.string.constant.27:
	.asciz	"log10: "
	.size	.Llynx.string.constant.27, 8

	.type	.Llynx.string.constant.31,@object
.Llynx.string.constant.31:
	.asciz	"sin: "
	.size	.Llynx.string.constant.31, 6

	.type	.Llynx.string.constant.35,@object
.Llynx.string.constant.35:
	.asciz	"cos: "
	.size	.Llynx.string.constant.35, 6

	.type	.Llynx.string.constant.39,@object
.Llynx.string.constant.39:
	.asciz	"tan: "
	.size	.Llynx.string.constant.39, 6

	.type	.Llynx.string.constant.43,@object
.Llynx.string.constant.43:
	.asciz	"acos: "
	.size	.Llynx.string.constant.43, 7

	.type	.Llynx.string.constant.47,@object
.Llynx.string.constant.47:
	.asciz	"atan: "
	.size	.Llynx.string.constant.47, 7

	.type	.Llynx.string.constant.51,@object
.Llynx.string.constant.51:
	.asciz	"atan2: "
	.size	.Llynx.string.constant.51, 8

	.type	.Llynx.string.constant.55,@object
.Llynx.string.constant.55:
	.asciz	"sinh: "
	.size	.Llynx.string.constant.55, 7

	.type	.Llynx.string.constant.59,@object
.Llynx.string.constant.59:
	.asciz	"cosh: "
	.size	.Llynx.string.constant.59, 7

	.type	.Llynx.string.constant.63,@object
.Llynx.string.constant.63:
	.asciz	"tanh: "
	.size	.Llynx.string.constant.63, 7

	.type	.Llynx.string.constant.67,@object
.Llynx.string.constant.67:
	.asciz	"floor: "
	.size	.Llynx.string.constant.67, 8

	.type	.Llynx.string.constant.71,@object
.Llynx.string.constant.71:
	.asciz	"ceil: "
	.size	.Llynx.string.constant.71, 7

	.type	.Llynx.string.constant.75,@object
.Llynx.string.constant.75:
	.asciz	"trunc: "
	.size	.Llynx.string.constant.75, 8

	.type	.Lfmt.76,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lfmt.76:
	.asciz	"%s %f\n"
	.size	.Lfmt.76, 7

	.section	".note.GNU-stack","",@progbits
