	.file	"21CS02006_A1.c"
	.text
	.comm	A,4000000,32
	.comm	B,4000000,32
	.comm	C,4000000,32
	.section	.rodata
	.align 8
.LC1:
	.string	"Matrix multiplication using Naive took %.6f seconds\n"
	.text
	.globl	multiplyMatrices
	.type	multiplyMatrices, @function
multiplyMatrices:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-48(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday@PLT
	movl	$0, -68(%rbp)
	jmp	.L2
.L7:
	movl	$0, -64(%rbp)
	jmp	.L3
.L6:
	movl	$0, -60(%rbp)
	jmp	.L4
.L5:
	movl	-68(%rbp), %eax
	cltq
	imulq	$1000, %rax, %rdx
	movl	-64(%rbp), %eax
	cltq
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	leaq	C(%rip), %rax
	addq	%rdx, %rax
	movl	(%rax), %ecx
	movl	-68(%rbp), %eax
	cltq
	imulq	$1000, %rax, %rdx
	movl	-60(%rbp), %eax
	cltq
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	leaq	A(%rip), %rax
	addq	%rdx, %rax
	movl	(%rax), %edx
	movl	-60(%rbp), %eax
	cltq
	imulq	$1000, %rax, %rsi
	movl	-64(%rbp), %eax
	cltq
	addq	%rsi, %rax
	leaq	0(,%rax,4), %rsi
	leaq	B(%rip), %rax
	addq	%rsi, %rax
	movl	(%rax), %eax
	imull	%eax, %edx
	movl	-68(%rbp), %eax
	cltq
	imulq	$1000, %rax, %rsi
	movl	-64(%rbp), %eax
	cltq
	addq	%rsi, %rax
	leaq	0(,%rax,4), %rsi
	leaq	C(%rip), %rax
	addq	%rsi, %rax
	addl	%ecx, %edx
	movl	%edx, (%rax)
	addl	$1, -60(%rbp)
.L4:
	cmpl	$999, -60(%rbp)
	jle	.L5
	addl	$1, -64(%rbp)
.L3:
	cmpl	$999, -64(%rbp)
	jle	.L6
	addl	$1, -68(%rbp)
.L2:
	cmpl	$999, -68(%rbp)
	jle	.L7
	leaq	-32(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday@PLT
	movq	-32(%rbp), %rdx
	movq	-48(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm1
	movq	-24(%rbp), %rdx
	movq	-40(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC0(%rip), %xmm2
	divsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -56(%rbp)
	movq	-56(%rbp), %rax
	movq	%rax, -88(%rbp)
	movsd	-88(%rbp), %xmm0
	leaq	.LC1(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	nop
	movq	-8(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L8
	call	__stack_chk_fail@PLT
.L8:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	multiplyMatrices, .-multiplyMatrices
	.globl	matrix_multiply
	.type	matrix_multiply, @function
matrix_multiply:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -8(%rbp)
	movl	$250, -20(%rbp)
	movq	-8(%rbp), %rax
	movl	%eax, %edx
	movl	-20(%rbp), %eax
	imull	%edx, %eax
	movl	%eax, -16(%rbp)
	cmpq	$3, -8(%rbp)
	je	.L10
	movl	-16(%rbp), %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	jmp	.L11
.L10:
	movl	$1000, %eax
.L11:
	movl	%eax, -12(%rbp)
	movl	-16(%rbp), %eax
	movl	%eax, -32(%rbp)
	jmp	.L12
.L17:
	movl	$0, -28(%rbp)
	jmp	.L13
.L16:
	movl	-28(%rbp), %eax
	cltq
	movl	-32(%rbp), %edx
	movslq	%edx, %rdx
	imulq	$1000, %rdx, %rdx
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	leaq	C(%rip), %rax
	movl	$0, (%rdx,%rax)
	movl	$0, -24(%rbp)
	jmp	.L14
.L15:
	movl	-28(%rbp), %eax
	cltq
	movl	-32(%rbp), %edx
	movslq	%edx, %rdx
	imulq	$1000, %rdx, %rdx
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	leaq	C(%rip), %rax
	movl	(%rdx,%rax), %edx
	movl	-24(%rbp), %eax
	cltq
	movl	-32(%rbp), %ecx
	movslq	%ecx, %rcx
	imulq	$1000, %rcx, %rcx
	addq	%rcx, %rax
	leaq	0(,%rax,4), %rcx
	leaq	A(%rip), %rax
	movl	(%rcx,%rax), %ecx
	movl	-28(%rbp), %eax
	cltq
	movl	-24(%rbp), %esi
	movslq	%esi, %rsi
	imulq	$1000, %rsi, %rsi
	addq	%rsi, %rax
	leaq	0(,%rax,4), %rsi
	leaq	B(%rip), %rax
	movl	(%rsi,%rax), %eax
	imull	%ecx, %eax
	leal	(%rdx,%rax), %ecx
	movl	-28(%rbp), %eax
	cltq
	movl	-32(%rbp), %edx
	movslq	%edx, %rdx
	imulq	$1000, %rdx, %rdx
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	leaq	C(%rip), %rax
	movl	%ecx, (%rdx,%rax)
	addl	$1, -24(%rbp)
.L14:
	cmpl	$999, -24(%rbp)
	jle	.L15
	addl	$1, -28(%rbp)
.L13:
	cmpl	$999, -28(%rbp)
	jle	.L16
	addl	$1, -32(%rbp)
.L12:
	movl	-32(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jl	.L17
	movl	$0, %edi
	call	pthread_exit@PLT
	.cfi_endproc
.LFE7:
	.size	matrix_multiply, .-matrix_multiply
	.section	.rodata
	.align 8
.LC2:
	.string	"Matrix multiplication using %d Threads took %.6f seconds\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$144, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -120(%rbp)
	jmp	.L19
.L22:
	movl	$0, -116(%rbp)
	jmp	.L20
.L21:
	call	rand@PLT
	movl	%eax, %ecx
	movslq	%ecx, %rdx
	movq	%rdx, %rax
	salq	$30, %rax
	addq	%rdx, %rax
	shrq	$32, %rax
	movl	%eax, %edx
	sarl	$29, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	-116(%rbp), %eax
	cltq
	movl	-120(%rbp), %edx
	movslq	%edx, %rdx
	imulq	$1000, %rdx, %rdx
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	leaq	A(%rip), %rax
	movl	%ecx, (%rdx,%rax)
	addl	$1, -116(%rbp)
.L20:
	cmpl	$999, -116(%rbp)
	jle	.L21
	addl	$1, -120(%rbp)
.L19:
	cmpl	$999, -120(%rbp)
	jle	.L22
	movl	$0, -112(%rbp)
	jmp	.L23
.L26:
	movl	$0, -108(%rbp)
	jmp	.L24
.L25:
	call	rand@PLT
	movl	%eax, %ecx
	movslq	%ecx, %rdx
	movq	%rdx, %rax
	salq	$30, %rax
	addq	%rdx, %rax
	shrq	$32, %rax
	movl	%eax, %edx
	sarl	$29, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	-108(%rbp), %eax
	cltq
	movl	-112(%rbp), %edx
	movslq	%edx, %rdx
	imulq	$1000, %rdx, %rdx
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	leaq	B(%rip), %rax
	movl	%ecx, (%rdx,%rax)
	addl	$1, -108(%rbp)
.L24:
	cmpl	$999, -108(%rbp)
	jle	.L25
	addl	$1, -112(%rbp)
.L23:
	cmpl	$999, -112(%rbp)
	jle	.L26
	movl	$0, %eax
	call	multiplyMatrices
	leaq	-80(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday@PLT
	movq	$0, -104(%rbp)
	jmp	.L27
.L28:
	movq	-104(%rbp), %rax
	leaq	-48(%rbp), %rdx
	movq	-104(%rbp), %rcx
	salq	$3, %rcx
	leaq	(%rdx,%rcx), %rdi
	movq	%rax, %rcx
	leaq	matrix_multiply(%rip), %rdx
	movl	$0, %esi
	call	pthread_create@PLT
	addq	$1, -104(%rbp)
.L27:
	cmpq	$3, -104(%rbp)
	jle	.L28
	movq	$0, -96(%rbp)
	jmp	.L29
.L30:
	movq	-96(%rbp), %rax
	movq	-48(%rbp,%rax,8), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	addq	$1, -96(%rbp)
.L29:
	cmpq	$3, -96(%rbp)
	jle	.L30
	leaq	-64(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	gettimeofday@PLT
	movq	-64(%rbp), %rdx
	movq	-80(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm1
	movq	-56(%rbp), %rdx
	movq	-72(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC0(%rip), %xmm2
	divsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -88(%rbp)
	movq	-88(%rbp), %rax
	movq	%rax, -136(%rbp)
	movsd	-136(%rbp), %xmm0
	movl	$4, %esi
	leaq	.LC2(%rip), %rdi
	movl	$1, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L32
	call	__stack_chk_fail@PLT
.L32:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 7.5.0-6ubuntu2) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
