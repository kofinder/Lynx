	.file	"main"
	.text
	.globl	main
	.p2align	4
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	$32, %edi
	callq	GC_malloc@PLT
	movq	%rax, %rbx
	movaps	.Lflat_array_const(%rip), %xmm0
	movups	%xmm0, (%rax)
	movaps	.Lflat_array_const+16(%rip), %xmm0
	movups	%xmm0, 16(%rax)
	movl	$24, %edi
	callq	GC_malloc@PLT
	movq	%rbx, (%rax)
	movq	$2, 8(%rax)
	movq	$2, 16(%rax)
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	.Lstr_a,@object
	.section	.rodata,"a",@progbits
.Lstr_a:
	.asciz	"a"
	.size	.Lstr_a, 2

	.type	.Lmap_key_str_a,@object
	.p2align	3, 0x0
.Lmap_key_str_a:
	.quad	.Lstr_a
	.quad	0
	.size	.Lmap_key_str_a, 16

	.type	.Lstr_b,@object
.Lstr_b:
	.asciz	"b"
	.size	.Lstr_b, 2

	.type	.Lmap_key_str_b,@object
	.p2align	3, 0x0
.Lmap_key_str_b:
	.quad	.Lstr_b
	.quad	0
	.size	.Lmap_key_str_b, 16

	.type	.Lmap_key.0,@object
	.p2align	3, 0x0
.Lmap_key.0:
	.quad	.Lmap_key_str_a
	.size	.Lmap_key.0, 16

	.type	.Lmap_key.1,@object
	.p2align	3, 0x0
.Lmap_key.1:
	.quad	.Lmap_key_str_b
	.size	.Lmap_key.1, 16

	.type	.Lmap_value.0,@object
	.p2align	2, 0x0
.Lmap_value.0:
	.long	1
	.size	.Lmap_value.0, 4

	.type	.Lmap_value.1,@object
	.p2align	2, 0x0
.Lmap_value.1:
	.long	2
	.size	.Lmap_value.1, 4

	.type	.Lflat_array_const,@object
	.p2align	4, 0x0
.Lflat_array_const:
	.quad	.Lmap_key.0
	.quad	.Lmap_value.0
	.quad	.Lmap_key.1
	.quad	.Lmap_value.1
	.size	.Lflat_array_const, 32

	.section	".note.GNU-stack","",@progbits
