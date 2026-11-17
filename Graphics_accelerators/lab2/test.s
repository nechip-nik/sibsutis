	.file	"test.c"
	.text
	.globl	lat_add_i64
	.type	lat_add_i64, @function
lat_add_i64:
.LFB5781:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$1, -32(%rbp)
	movl	$3, -28(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -24(%rbp)
	jmp	.L3
.L4:
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	movl	-28(%rbp), %eax
	addl	%eax, -32(%rbp)
	incq	-24(%rbp)
.L3:
	cmpq	$99999999, -24(%rbp)
	jbe	.L4
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L6
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L7
.L6:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L7:
	vmovsd	.LC0(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5781:
	.size	lat_add_i64, .-lat_add_i64
	.globl	tp_add_i64
	.type	tp_add_i64, @function
tp_add_i64:
.LFB5782:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	$1, -88(%rbp)
	movq	$2, -80(%rbp)
	movq	$3, -72(%rbp)
	movq	$4, -64(%rbp)
	movq	$5, -48(%rbp)
	movq	$6, -40(%rbp)
	movq	$7, -32(%rbp)
	movq	$8, -24(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -56(%rbp)
	jmp	.L11
.L12:
	movq	-48(%rbp), %rax
	addq	%rax, -88(%rbp)
	movq	-40(%rbp), %rax
	addq	%rax, -80(%rbp)
	movq	-32(%rbp), %rax
	addq	%rax, -72(%rbp)
	movq	-24(%rbp), %rax
	addq	%rax, -64(%rbp)
	incq	-56(%rbp)
.L11:
	cmpq	$999999999, -56(%rbp)
	jbe	.L12
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L14
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L15
.L14:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L15:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5782:
	.size	tp_add_i64, .-tp_add_i64
	.globl	lat_mul_i64
	.type	lat_mul_i64, @function
lat_mul_i64:
.LFB5783:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	$3, -40(%rbp)
	movq	$5, -24(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L19
.L20:
	movq	-40(%rbp), %rax
	imulq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	incq	-32(%rbp)
.L19:
	cmpq	$999999999, -32(%rbp)
	jbe	.L20
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L22
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L23
.L22:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L23:
	vmovsd	.LC0(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5783:
	.size	lat_mul_i64, .-lat_mul_i64
	.globl	tp_mul_i64
	.type	tp_mul_i64, @function
tp_mul_i64:
.LFB5784:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	$3, -88(%rbp)
	movq	$5, -80(%rbp)
	movq	$7, -72(%rbp)
	movq	$11, -64(%rbp)
	movq	$2, -48(%rbp)
	movq	$3, -40(%rbp)
	movq	$5, -32(%rbp)
	movq	$7, -24(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -56(%rbp)
	jmp	.L27
.L28:
	movq	-88(%rbp), %rax
	imulq	-48(%rbp), %rax
	movq	%rax, -88(%rbp)
	movq	-80(%rbp), %rax
	imulq	-40(%rbp), %rax
	movq	%rax, -80(%rbp)
	movq	-72(%rbp), %rax
	imulq	-32(%rbp), %rax
	movq	%rax, -72(%rbp)
	movq	-64(%rbp), %rax
	imulq	-24(%rbp), %rax
	movq	%rax, -64(%rbp)
	incq	-56(%rbp)
.L27:
	cmpq	$999999999, -56(%rbp)
	jbe	.L28
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L30
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L31
.L30:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L31:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5784:
	.size	tp_mul_i64, .-tp_mul_i64
	.globl	lat_add_f64
	.type	lat_add_f64, @function
lat_add_f64:
.LFB5785:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	vmovsd	.LC2(%rip), %xmm0
	vmovsd	%xmm0, -40(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L35
.L36:
	vmovsd	-40(%rbp), %xmm0
	vaddsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -40(%rbp)
	incq	-32(%rbp)
.L35:
	cmpq	$999999999, -32(%rbp)
	jbe	.L36
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L38
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L39
.L38:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L39:
	vmovsd	.LC0(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5785:
	.size	lat_add_f64, .-lat_add_f64
	.globl	tp_add_f64
	.type	tp_add_f64, @function
tp_add_f64:
.LFB5786:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	vmovsd	.LC2(%rip), %xmm0
	vmovsd	%xmm0, -64(%rbp)
	vmovsd	.LC4(%rip), %xmm0
	vmovsd	%xmm0, -56(%rbp)
	vmovsd	.LC5(%rip), %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	.LC6(%rip), %xmm0
	vmovsd	%xmm0, -40(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L43
.L44:
	vmovsd	-64(%rbp), %xmm0
	vaddsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -64(%rbp)
	vmovsd	-56(%rbp), %xmm0
	vaddsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -56(%rbp)
	vmovsd	-48(%rbp), %xmm0
	vaddsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	-40(%rbp), %xmm0
	vaddsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -40(%rbp)
	incq	-32(%rbp)
.L43:
	cmpq	$999999999, -32(%rbp)
	jbe	.L44
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L46
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L47
.L46:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L47:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5786:
	.size	tp_add_f64, .-tp_add_f64
	.globl	lat_mul_f64
	.type	lat_mul_f64, @function
lat_mul_f64:
.LFB5787:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	vmovsd	.LC7(%rip), %xmm0
	vmovsd	%xmm0, -40(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L51
.L52:
	vmovsd	-40(%rbp), %xmm0
	vmulsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -40(%rbp)
	incq	-32(%rbp)
.L51:
	cmpq	$999999999, -32(%rbp)
	jbe	.L52
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L54
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L55
.L54:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L55:
	vmovsd	.LC0(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5787:
	.size	lat_mul_f64, .-lat_mul_f64
	.globl	tp_mul_f64
	.type	tp_mul_f64, @function
tp_mul_f64:
.LFB5788:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	vmovsd	.LC8(%rip), %xmm0
	vmovsd	%xmm0, -64(%rbp)
	vmovsd	.LC9(%rip), %xmm0
	vmovsd	%xmm0, -56(%rbp)
	vmovsd	.LC10(%rip), %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	.LC11(%rip), %xmm0
	vmovsd	%xmm0, -40(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L59
.L60:
	vmovsd	-64(%rbp), %xmm0
	vmulsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -64(%rbp)
	vmovsd	-56(%rbp), %xmm0
	vmulsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -56(%rbp)
	vmovsd	-48(%rbp), %xmm0
	vmulsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	-40(%rbp), %xmm0
	vmulsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -40(%rbp)
	incq	-32(%rbp)
.L59:
	cmpq	$999999999, -32(%rbp)
	jbe	.L60
	rdtsc
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L62
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L63
.L62:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L63:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5788:
	.size	tp_mul_f64, .-tp_mul_f64
	.section	.rodata
.LC12:
	.string	"  %-28s %-10s : %6.3f\n"
	.text
	.type	report, @function
report:
.LFB5789:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	vmovsd	%xmm0, -24(%rbp)
	movq	-24(%rbp), %rcx
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	leaq	.LC12(%rip), %rdi
	vmovq	%rcx, %xmm0
	movq	%rax, %rsi
	movl	$1, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5789:
	.size	report, .-report
	.section	.rodata
	.align 8
.LC13:
	.base64	"TiA9ICVsbHUg0LjRgtC10YDQsNGG0LjQuSDQvdCwINGC0LXRgdGCCgA="
.LC14:
	.base64	"0KbQtdC70L7Rh9C40YHQu9C10L3QvdGL0LU6AA=="
.LC15:
	.string	"latency"
.LC16:
	.string	"ADD (int64)"
.LC17:
	.string	"throughput"
.LC18:
	.string	"MUL (int64)"
	.align 8
.LC19:
	.base64	"CtChINC/0LvQsNCy0LDRjtGJ0LXQuSDRgtC+0YfQutC+0LkgKGRvdWJsZSk6AA=="
.LC20:
	.string	"ADD (f64)"
.LC21:
	.string	"MUL (f64)"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5790:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC13(%rip), %rax
	movl	$1000000000, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC14(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	call	lat_add_i64
	vmovq	%xmm0, %rax
	leaq	.LC15(%rip), %rcx
	leaq	.LC16(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_add_i64
	vmovq	%xmm0, %rax
	leaq	.LC17(%rip), %rcx
	leaq	.LC16(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	lat_mul_i64
	vmovq	%xmm0, %rax
	leaq	.LC15(%rip), %rcx
	leaq	.LC18(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_mul_i64
	vmovq	%xmm0, %rax
	leaq	.LC17(%rip), %rcx
	leaq	.LC18(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	leaq	.LC19(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	call	lat_add_f64
	vmovq	%xmm0, %rax
	leaq	.LC15(%rip), %rcx
	leaq	.LC20(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_add_f64
	vmovq	%xmm0, %rax
	leaq	.LC17(%rip), %rcx
	leaq	.LC20(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	lat_mul_f64
	vmovq	%xmm0, %rax
	leaq	.LC15(%rip), %rcx
	leaq	.LC21(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_mul_f64
	vmovq	%xmm0, %rax
	leaq	.LC17(%rip), %rcx
	leaq	.LC21(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5790:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1104006501
	.align 8
.LC1:
	.long	0
	.long	1106103653
	.align 8
.LC2:
	.long	0
	.long	1072693248
	.align 8
.LC3:
	.long	450359963
	.long	1072693248
	.align 8
.LC4:
	.long	0
	.long	1073741824
	.align 8
.LC5:
	.long	0
	.long	1074266112
	.align 8
.LC6:
	.long	0
	.long	1074790400
	.align 8
.LC7:
	.long	-611603343
	.long	1072693352
	.align 8
.LC8:
	.long	-1717986918
	.long	1072798105
	.align 8
.LC9:
	.long	858993459
	.long	1072902963
	.align 8
.LC10:
	.long	-858993459
	.long	1073007820
	.align 8
.LC11:
	.long	1717986918
	.long	1073112678
	.ident	"GCC: (Ubuntu 15.2.0-16ubuntu1) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
