	.file	"latency_full.c"
	.text
	.type	rdtsc, @function
rdtsc:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%eax, -8(%rbp)
	movl	%edx, -4(%rbp)
	movl	-4(%rbp), %eax
	salq	$32, %rax
	movq	%rax, %rdx
	movl	-8(%rbp), %eax
	orq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	rdtsc, .-rdtsc
	.globl	lat_add_i64
	.type	lat_add_i64, @function
lat_add_i64:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	$1, -40(%rbp)
	movq	$3, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L4
.L5:
	movq	-24(%rbp), %rax
	addq	%rax, -40(%rbp)
	incq	-32(%rbp)
.L4:
	cmpq	$99999999, -32(%rbp)
	jbe	.L5
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
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
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	lat_add_i64, .-lat_add_i64
	.globl	tp_add_i64
	.type	tp_add_i64, @function
tp_add_i64:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	$1, -88(%rbp)
	movq	$2, -80(%rbp)
	movq	$3, -72(%rbp)
	movq	$4, -64(%rbp)
	movq	$5, -48(%rbp)
	movq	$6, -40(%rbp)
	movq	$7, -32(%rbp)
	movq	$8, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -56(%rbp)
	jmp	.L10
.L11:
	movq	-48(%rbp), %rax
	addq	%rax, -88(%rbp)
	movq	-40(%rbp), %rax
	addq	%rax, -80(%rbp)
	movq	-32(%rbp), %rax
	addq	%rax, -72(%rbp)
	movq	-24(%rbp), %rax
	addq	%rax, -64(%rbp)
	incq	-56(%rbp)
.L10:
	cmpq	$99999999, -56(%rbp)
	jbe	.L11
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-88(%rbp), %rdx
	movq	-80(%rbp), %rax
	addq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rax, %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -96(%rbp)
	movq	-96(%rbp), %rax
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L12
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L13
.L12:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L13:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	tp_add_i64, .-tp_add_i64
	.globl	lat_mul_i64
	.type	lat_mul_i64, @function
lat_mul_i64:
.LFB3:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	$3, -40(%rbp)
	movq	$5, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L16
.L17:
	movq	-40(%rbp), %rax
	imulq	-24(%rbp), %rax
	movq	%rax, -40(%rbp)
	incq	-32(%rbp)
.L16:
	cmpq	$99999999, -32(%rbp)
	jbe	.L17
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L18
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L19
.L18:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L19:
	vmovsd	.LC0(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	lat_mul_i64, .-lat_mul_i64
	.globl	tp_mul_i64
	.type	tp_mul_i64, @function
tp_mul_i64:
.LFB4:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	$3, -88(%rbp)
	movq	$5, -80(%rbp)
	movq	$7, -72(%rbp)
	movq	$11, -64(%rbp)
	movq	$2, -48(%rbp)
	movq	$3, -40(%rbp)
	movq	$5, -32(%rbp)
	movq	$7, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -56(%rbp)
	jmp	.L22
.L23:
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
.L22:
	cmpq	$99999999, -56(%rbp)
	jbe	.L23
	call	rdtsc
	movq	%rax, -8(%rbp)
	movq	-88(%rbp), %rdx
	movq	-80(%rbp), %rax
	addq	%rax, %rdx
	movq	-72(%rbp), %rax
	addq	%rax, %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	%rax, -96(%rbp)
	movq	-96(%rbp), %rax
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L24
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L25
.L24:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L25:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	tp_mul_i64, .-tp_mul_i64
	.globl	lat_add_f64
	.type	lat_add_f64, @function
lat_add_f64:
.LFB5:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	vmovsd	.LC2(%rip), %xmm0
	vmovsd	%xmm0, -40(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L28
.L29:
	vmovsd	-40(%rbp), %xmm0
	vaddsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -40(%rbp)
	incq	-32(%rbp)
.L28:
	cmpq	$99999999, -32(%rbp)
	jbe	.L29
	call	rdtsc
	movq	%rax, -8(%rbp)
	vmovsd	-40(%rbp), %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	-48(%rbp), %xmm0
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
	vmovsd	.LC0(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	lat_add_f64, .-lat_add_f64
	.globl	tp_add_f64
	.type	tp_add_f64, @function
tp_add_f64:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
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
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L34
.L35:
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
.L34:
	cmpq	$99999999, -32(%rbp)
	jbe	.L35
	call	rdtsc
	movq	%rax, -8(%rbp)
	vmovsd	-64(%rbp), %xmm0
	vaddsd	-56(%rbp), %xmm0, %xmm0
	vaddsd	-48(%rbp), %xmm0, %xmm0
	vaddsd	-40(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -72(%rbp)
	vmovsd	-72(%rbp), %xmm0
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L36
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L37
.L36:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L37:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	tp_add_f64, .-tp_add_f64
	.globl	lat_mul_f64
	.type	lat_mul_f64, @function
lat_mul_f64:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	vmovsd	.LC7(%rip), %xmm0
	vmovsd	%xmm0, -40(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L40
.L41:
	vmovsd	-40(%rbp), %xmm0
	vmulsd	-24(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -40(%rbp)
	incq	-32(%rbp)
.L40:
	cmpq	$99999999, -32(%rbp)
	jbe	.L41
	call	rdtsc
	movq	%rax, -8(%rbp)
	vmovsd	-40(%rbp), %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	-48(%rbp), %xmm0
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L42
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L43
.L42:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L43:
	vmovsd	.LC0(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	lat_mul_f64, .-lat_mul_f64
	.globl	tp_mul_f64
	.type	tp_mul_f64, @function
tp_mul_f64:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
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
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -32(%rbp)
	jmp	.L46
.L47:
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
.L46:
	cmpq	$99999999, -32(%rbp)
	jbe	.L47
	call	rdtsc
	movq	%rax, -8(%rbp)
	vmovsd	-64(%rbp), %xmm0
	vaddsd	-56(%rbp), %xmm0, %xmm0
	vaddsd	-48(%rbp), %xmm0, %xmm0
	vaddsd	-40(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -72(%rbp)
	vmovsd	-72(%rbp), %xmm0
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L48
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L49
.L48:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L49:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	tp_mul_f64, .-tp_mul_f64
	.globl	lat_fma_f64
	.type	lat_fma_f64, @function
lat_fma_f64:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	vmovsd	.LC2(%rip), %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -32(%rbp)
	vmovsd	.LC12(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -40(%rbp)
	jmp	.L52
.L53:
	vmovsd	-48(%rbp), %xmm0
	vmulsd	-32(%rbp), %xmm0, %xmm0
	vmovsd	-24(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -48(%rbp)
	incq	-40(%rbp)
.L52:
	cmpq	$99999999, -40(%rbp)
	jbe	.L53
	call	rdtsc
	movq	%rax, -8(%rbp)
	vmovsd	-48(%rbp), %xmm0
	vmovsd	%xmm0, -56(%rbp)
	vmovsd	-56(%rbp), %xmm0
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
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	lat_fma_f64, .-lat_fma_f64
	.globl	tp_fma_f64
	.type	tp_fma_f64, @function
tp_fma_f64:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$80, %rsp
	vmovsd	.LC2(%rip), %xmm0
	vmovsd	%xmm0, -72(%rbp)
	vmovsd	.LC8(%rip), %xmm0
	vmovsd	%xmm0, -64(%rbp)
	vmovsd	.LC9(%rip), %xmm0
	vmovsd	%xmm0, -56(%rbp)
	vmovsd	.LC10(%rip), %xmm0
	vmovsd	%xmm0, -48(%rbp)
	vmovsd	.LC3(%rip), %xmm0
	vmovsd	%xmm0, -32(%rbp)
	vmovsd	.LC12(%rip), %xmm0
	vmovsd	%xmm0, -24(%rbp)
	call	rdtsc
	movq	%rax, -16(%rbp)
	movq	$0, -40(%rbp)
	jmp	.L58
.L59:
	vmovsd	-72(%rbp), %xmm0
	vmulsd	-32(%rbp), %xmm0, %xmm0
	vmovsd	-24(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -72(%rbp)
	vmovsd	-64(%rbp), %xmm0
	vmulsd	-32(%rbp), %xmm0, %xmm0
	vmovsd	-24(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -64(%rbp)
	vmovsd	-56(%rbp), %xmm0
	vmulsd	-32(%rbp), %xmm0, %xmm0
	vmovsd	-24(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -56(%rbp)
	vmovsd	-48(%rbp), %xmm0
	vmulsd	-32(%rbp), %xmm0, %xmm0
	vmovsd	-24(%rbp), %xmm1
	vaddsd	%xmm0, %xmm1, %xmm0
	vmovsd	%xmm0, -48(%rbp)
	incq	-40(%rbp)
.L58:
	cmpq	$99999999, -40(%rbp)
	jbe	.L59
	call	rdtsc
	movq	%rax, -8(%rbp)
	vmovsd	-72(%rbp), %xmm0
	vaddsd	-64(%rbp), %xmm0, %xmm0
	vaddsd	-56(%rbp), %xmm0, %xmm0
	vaddsd	-48(%rbp), %xmm0, %xmm0
	vmovsd	%xmm0, -80(%rbp)
	vmovsd	-80(%rbp), %xmm0
	movq	-8(%rbp), %rax
	subq	-16(%rbp), %rax
	testq	%rax, %rax
	js	.L60
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	jmp	.L61
.L60:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
.L61:
	vmovsd	.LC1(%rip), %xmm1
	vdivsd	%xmm1, %xmm0, %xmm0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	tp_fma_f64, .-tp_fma_f64
	.section	.rodata
	.align 8
.LC13:
	.base64	"ICAlLTI4cyAlLTEwcyA6ICU2LjNmINGC0LDQutGC0L7Qsi/QvtC/CgA="
	.text
	.type	report, @function
report:
.LFB11:
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
	leaq	.LC13(%rip), %rdi
	vmovq	%rcx, %xmm0
	movq	%rax, %rsi
	movl	$1, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	report, .-report
	.section	.rodata
	.align 8
.LC14:
	.base64	"TiA9ICVsbHUg0LjRgtC10YDQsNGG0LjQuSDQvdCwINGC0LXRgdGCCgA="
.LC15:
	.base64	"0KbQtdC70L7Rh9C40YHQu9C10L3QvdGL0LU6AA=="
.LC16:
	.string	"latency"
.LC17:
	.string	"ADD (int64)"
.LC18:
	.string	"throughput"
.LC19:
	.string	"MUL (int64)"
	.align 8
.LC20:
	.base64	"CtChINC/0LvQsNCy0LDRjtGJ0LXQuSDRgtC+0YfQutC+0LkgKGRvdWJsZSk6AA=="
.LC21:
	.string	"ADD (f64)"
.LC22:
	.string	"MUL (f64)"
.LC23:
	.string	"FMA (f64)"
	.text
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC14(%rip), %rax
	movl	$100000000, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	.LC15(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	call	lat_add_i64
	vmovq	%xmm0, %rax
	leaq	.LC16(%rip), %rcx
	leaq	.LC17(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_add_i64
	vmovq	%xmm0, %rax
	leaq	.LC18(%rip), %rcx
	leaq	.LC17(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	lat_mul_i64
	vmovq	%xmm0, %rax
	leaq	.LC16(%rip), %rcx
	leaq	.LC19(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_mul_i64
	vmovq	%xmm0, %rax
	leaq	.LC18(%rip), %rcx
	leaq	.LC19(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	leaq	.LC20(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	call	lat_add_f64
	vmovq	%xmm0, %rax
	leaq	.LC16(%rip), %rcx
	leaq	.LC21(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_add_f64
	vmovq	%xmm0, %rax
	leaq	.LC18(%rip), %rcx
	leaq	.LC21(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	lat_mul_f64
	vmovq	%xmm0, %rax
	leaq	.LC16(%rip), %rcx
	leaq	.LC22(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_mul_f64
	vmovq	%xmm0, %rax
	leaq	.LC18(%rip), %rcx
	leaq	.LC22(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	lat_fma_f64
	vmovq	%xmm0, %rax
	leaq	.LC16(%rip), %rcx
	leaq	.LC23(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	call	tp_fma_f64
	vmovq	%xmm0, %rax
	leaq	.LC18(%rip), %rcx
	leaq	.LC23(%rip), %rdx
	vmovq	%rax, %xmm0
	movq	%rcx, %rsi
	movq	%rdx, %rdi
	call	report
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1100470148
	.align 8
.LC1:
	.long	0
	.long	1102567300
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
	.align 8
.LC12:
	.long	-1698910392
	.long	1048238066
	.ident	"GCC: (Ubuntu 15.2.0-4ubuntu4) 15.2.0"
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
