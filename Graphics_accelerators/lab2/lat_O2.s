	.file	"latency_full.c"
	.text
	.p2align 4
	.globl	lat_add_i64
	.type	lat_add_i64, @function
lat_add_i64:
.LFB24:
	.cfi_startproc
	endbr64
	vxorps	%xmm0, %xmm0, %xmm0
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%edx, %ecx
	movl	%eax, %esi
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	movq	$300000001, -8(%rsp)
	movq	-8(%rsp), %rdi
	orq	%rdx, %rax
	movq	%rcx, %rdx
	salq	$32, %rdx
	orq	%rsi, %rdx
	subq	%rdx, %rax
	js	.L2
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
.L2:
	movq	%rax, %rdx
	andl	$1, %eax
	shrq	%rdx
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE24:
	.size	lat_add_i64, .-lat_add_i64
	.p2align 4
	.globl	tp_add_i64
	.type	tp_add_i64, @function
tp_add_i64:
.LFB25:
	.cfi_startproc
	endbr64
	vxorps	%xmm0, %xmm0, %xmm0
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%edx, %ecx
	movl	%eax, %esi
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	movl	$2600000010, %edi
	orq	%rdx, %rax
	movq	%rcx, %rdx
	movq	%rdi, -8(%rsp)
	movq	-8(%rsp), %rdi
	salq	$32, %rdx
	orq	%rsi, %rdx
	subq	%rdx, %rax
	js	.L6
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
.L6:
	movq	%rax, %rdx
	andl	$1, %eax
	shrq	%rdx
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE25:
	.size	tp_add_i64, .-tp_add_i64
	.p2align 4
	.globl	lat_mul_i64
	.type	lat_mul_i64, @function
lat_mul_i64:
.LFB26:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	movl	$3, %ecx
	movq	%rdx, %rsi
	orq	%rax, %rsi
	movl	$100000000, %eax
	.p2align 4
	.p2align 4
	.p2align 3
.L9:
	decq	%rax
	leaq	(%rcx,%rcx,4), %rcx
	jne	.L9
	vxorps	%xmm0, %xmm0, %xmm0
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	movq	%rcx, -8(%rsp)
	movq	-8(%rsp), %rcx
	orq	%rax, %rdx
	subq	%rsi, %rdx
	js	.L10
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
.L10:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE26:
	.size	lat_mul_i64, .-lat_mul_i64
	.p2align 4
	.globl	tp_mul_i64
	.type	tp_mul_i64, @function
tp_mul_i64:
.LFB27:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	movl	%eax, %eax
	salq	$32, %rdx
	movl	$11, %esi
	movl	$7, %ecx
	orq	%rax, %rdx
	movl	$5, %edi
	movl	$100000000, %eax
	movl	$3, %r8d
	movq	%rdx, %r9
	.p2align 5
	.p2align 4
	.p2align 3
.L14:
	leaq	0(,%rsi,8), %rdx
	addq	%r8, %r8
	leaq	(%rdi,%rdi,2), %rdi
	leaq	(%rcx,%rcx,4), %rcx
	subq	%rsi, %rdx
	decq	%rax
	movq	%rdx, %rsi
	jne	.L14
	vxorps	%xmm0, %xmm0, %xmm0
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	addq	%rdi, %r8
	addq	%rsi, %rcx
	orq	%rax, %rdx
	addq	%r8, %rcx
	subq	%r9, %rdx
	movq	%rcx, -8(%rsp)
	movq	-8(%rsp), %rcx
	js	.L15
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
.L15:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE27:
	.size	tp_mul_i64, .-tp_mul_i64
	.p2align 4
	.globl	lat_add_f64
	.type	lat_add_f64, @function
lat_add_f64:
.LFB28:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vmovsd	.LC2(%rip), %xmm0
	vmovsd	.LC3(%rip), %xmm1
	salq	$32, %rdx
	movl	%eax, %eax
	movq	%rdx, %rcx
	orq	%rax, %rcx
	movl	$100000000, %eax
	.p2align 4
	.p2align 4
	.p2align 3
.L19:
	vaddsd	%xmm1, %xmm0, %xmm0
	subq	$2, %rax
	vaddsd	%xmm1, %xmm0, %xmm0
	jne	.L19
	vxorps	%xmm1, %xmm1, %xmm1
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	vmovsd	%xmm0, -8(%rsp)
	vmovsd	-8(%rsp), %xmm0
	orq	%rax, %rdx
	subq	%rcx, %rdx
	js	.L20
	vcvtsi2sdq	%rdx, %xmm1, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
.L20:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm1, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE28:
	.size	lat_add_f64, .-lat_add_f64
	.p2align 4
	.globl	tp_add_f64
	.type	tp_add_f64, @function
tp_add_f64:
.LFB29:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vmovsd	.LC4(%rip), %xmm2
	vmovsd	.LC5(%rip), %xmm3
	salq	$32, %rdx
	movl	%eax, %eax
	vmovsd	.LC6(%rip), %xmm4
	vmovsd	.LC2(%rip), %xmm1
	movq	%rdx, %rcx
	vmovsd	.LC3(%rip), %xmm0
	orq	%rax, %rcx
	movl	$100000000, %eax
	.p2align 5
	.p2align 4
	.p2align 3
.L25:
	decq	%rax
	vaddsd	%xmm0, %xmm1, %xmm1
	vaddsd	%xmm0, %xmm4, %xmm4
	vaddsd	%xmm0, %xmm3, %xmm3
	vaddsd	%xmm0, %xmm2, %xmm2
	jne	.L25
	vxorps	%xmm0, %xmm0, %xmm0
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vaddsd	%xmm4, %xmm1, %xmm1
	salq	$32, %rdx
	movl	%eax, %eax
	orq	%rax, %rdx
	subq	%rcx, %rdx
	vaddsd	%xmm3, %xmm1, %xmm1
	vaddsd	%xmm2, %xmm1, %xmm1
	vmovsd	%xmm1, -8(%rsp)
	vmovsd	-8(%rsp), %xmm1
	js	.L26
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
.L26:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE29:
	.size	tp_add_f64, .-tp_add_f64
	.p2align 4
	.globl	lat_mul_f64
	.type	lat_mul_f64, @function
lat_mul_f64:
.LFB30:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vmovsd	.LC7(%rip), %xmm0
	vmovsd	.LC3(%rip), %xmm1
	salq	$32, %rdx
	movl	%eax, %eax
	movq	%rdx, %rcx
	orq	%rax, %rcx
	movl	$100000000, %eax
	.p2align 4
	.p2align 4
	.p2align 3
.L30:
	vmulsd	%xmm1, %xmm0, %xmm0
	subq	$2, %rax
	vmulsd	%xmm1, %xmm0, %xmm0
	jne	.L30
	vxorps	%xmm1, %xmm1, %xmm1
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	vmovsd	%xmm0, -8(%rsp)
	vmovsd	-8(%rsp), %xmm0
	orq	%rax, %rdx
	subq	%rcx, %rdx
	js	.L31
	vcvtsi2sdq	%rdx, %xmm1, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
.L31:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm1, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE30:
	.size	lat_mul_f64, .-lat_mul_f64
	.p2align 4
	.globl	tp_mul_f64
	.type	tp_mul_f64, @function
tp_mul_f64:
.LFB31:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vmovsd	.LC8(%rip), %xmm2
	vmovsd	.LC9(%rip), %xmm3
	salq	$32, %rdx
	movl	%eax, %eax
	vmovsd	.LC10(%rip), %xmm4
	vmovsd	.LC11(%rip), %xmm1
	movq	%rdx, %rcx
	vmovsd	.LC3(%rip), %xmm0
	orq	%rax, %rcx
	movl	$100000000, %eax
	.p2align 5
	.p2align 4
	.p2align 3
.L36:
	decq	%rax
	vmulsd	%xmm0, %xmm1, %xmm1
	vmulsd	%xmm0, %xmm4, %xmm4
	vmulsd	%xmm0, %xmm3, %xmm3
	vmulsd	%xmm0, %xmm2, %xmm2
	jne	.L36
	vxorps	%xmm0, %xmm0, %xmm0
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vaddsd	%xmm4, %xmm1, %xmm1
	salq	$32, %rdx
	movl	%eax, %eax
	orq	%rax, %rdx
	subq	%rcx, %rdx
	vaddsd	%xmm3, %xmm1, %xmm1
	vaddsd	%xmm2, %xmm1, %xmm1
	vmovsd	%xmm1, -8(%rsp)
	vmovsd	-8(%rsp), %xmm1
	js	.L37
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
.L37:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE31:
	.size	tp_mul_f64, .-tp_mul_f64
	.p2align 4
	.globl	lat_fma_f64
	.type	lat_fma_f64, @function
lat_fma_f64:
.LFB32:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vmovsd	.LC2(%rip), %xmm0
	vmovsd	.LC3(%rip), %xmm2
	salq	$32, %rdx
	movl	%eax, %eax
	vmovsd	.LC12(%rip), %xmm1
	movq	%rdx, %rcx
	orq	%rax, %rcx
	movl	$100000000, %eax
	.p2align 4
	.p2align 4
	.p2align 3
.L41:
	vfmadd132sd	%xmm2, %xmm1, %xmm0
	subq	$2, %rax
	vfmadd132sd	%xmm2, %xmm1, %xmm0
	jne	.L41
	vxorps	%xmm1, %xmm1, %xmm1
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	salq	$32, %rdx
	movl	%eax, %eax
	vmovsd	%xmm0, -8(%rsp)
	vmovsd	-8(%rsp), %xmm0
	orq	%rax, %rdx
	subq	%rcx, %rdx
	js	.L42
	vcvtsi2sdq	%rdx, %xmm1, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
.L42:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm1, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC0(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE32:
	.size	lat_fma_f64, .-lat_fma_f64
	.p2align 4
	.globl	tp_fma_f64
	.type	tp_fma_f64, @function
tp_fma_f64:
.LFB33:
	.cfi_startproc
	endbr64
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vmovsd	.LC9(%rip), %xmm3
	vmovsd	.LC10(%rip), %xmm4
	salq	$32, %rdx
	movl	%eax, %eax
	vmovsd	.LC11(%rip), %xmm5
	vmovsd	.LC2(%rip), %xmm2
	movq	%rdx, %rcx
	vmovsd	.LC3(%rip), %xmm1
	vmovsd	.LC12(%rip), %xmm0
	orq	%rax, %rcx
	movl	$100000000, %eax
	.p2align 5
	.p2align 4
	.p2align 3
.L47:
	decq	%rax
	vfmadd132sd	%xmm1, %xmm0, %xmm2
	vfmadd132sd	%xmm1, %xmm0, %xmm5
	vfmadd132sd	%xmm1, %xmm0, %xmm4
	vfmadd132sd	%xmm1, %xmm0, %xmm3
	jne	.L47
	vxorps	%xmm0, %xmm0, %xmm0
#APP
# 9 "latency_full.c" 1
	rdtsc
# 0 "" 2
#NO_APP
	vaddsd	%xmm5, %xmm2, %xmm2
	salq	$32, %rdx
	movl	%eax, %eax
	orq	%rax, %rdx
	subq	%rcx, %rdx
	vaddsd	%xmm4, %xmm2, %xmm2
	vaddsd	%xmm3, %xmm2, %xmm2
	vmovsd	%xmm2, -8(%rsp)
	vmovsd	-8(%rsp), %xmm1
	js	.L48
	vcvtsi2sdq	%rdx, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
.L48:
	movq	%rdx, %rax
	andl	$1, %edx
	shrq	%rax
	orq	%rdx, %rax
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vaddsd	%xmm0, %xmm0, %xmm0
	vdivsd	.LC1(%rip), %xmm0, %xmm0
	ret
	.cfi_endproc
.LFE33:
	.size	tp_fma_f64, .-tp_fma_f64
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC13:
	.base64	"TiA9ICVsbHUg0LjRgtC10YDQsNGG0LjQuSDQvdCwINGC0LXRgdGCCgA="
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC14:
	.base64	"0KbQtdC70L7Rh9C40YHQu9C10L3QvdGL0LU6AA=="
.LC15:
	.string	"latency"
.LC16:
	.string	"ADD (int64)"
	.section	.rodata.str1.8
	.align 8
.LC17:
	.base64	"ICAlLTI4cyAlLTEwcyA6ICU2LjNmINGC0LDQutGC0L7Qsi/QvtC/CgA="
	.section	.rodata.str1.1
.LC18:
	.string	"throughput"
.LC19:
	.string	"MUL (int64)"
	.section	.rodata.str1.8
	.align 8
.LC20:
	.base64	"CtChINC/0LvQsNCy0LDRjtGJ0LXQuSDRgtC+0YfQutC+0LkgKGRvdWJsZSk6AA=="
	.section	.rodata.str1.1
.LC21:
	.string	"ADD (f64)"
.LC22:
	.string	"MUL (f64)"
.LC23:
	.string	"FMA (f64)"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB35:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	movl	$100000000, %edx
	leaq	.LC13(%rip), %rsi
	movl	$2, %edi
	call	__printf_chk@PLT
	leaq	.LC14(%rip), %rdi
	call	puts@PLT
	call	lat_add_i64
	leaq	.LC15(%rip), %rcx
	leaq	.LC16(%rip), %rdx
	leaq	.LC17(%rip), %rsi
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	tp_add_i64
	leaq	.LC18(%rip), %rcx
	leaq	.LC16(%rip), %rdx
	leaq	.LC17(%rip), %rsi
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	lat_mul_i64
	movl	$2, %edi
	leaq	.LC15(%rip), %rcx
	leaq	.LC19(%rip), %rdx
	leaq	.LC17(%rip), %rsi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	tp_mul_i64
	leaq	.LC18(%rip), %rcx
	leaq	.LC19(%rip), %rdx
	leaq	.LC17(%rip), %rsi
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	leaq	.LC20(%rip), %rdi
	call	puts@PLT
	call	lat_add_f64
	leaq	.LC17(%rip), %rsi
	leaq	.LC15(%rip), %rcx
	leaq	.LC21(%rip), %rdx
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	tp_add_f64
	leaq	.LC17(%rip), %rsi
	leaq	.LC18(%rip), %rcx
	leaq	.LC21(%rip), %rdx
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	lat_mul_f64
	leaq	.LC17(%rip), %rsi
	leaq	.LC15(%rip), %rcx
	leaq	.LC22(%rip), %rdx
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	tp_mul_f64
	leaq	.LC17(%rip), %rsi
	leaq	.LC18(%rip), %rcx
	leaq	.LC22(%rip), %rdx
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	lat_fma_f64
	leaq	.LC17(%rip), %rsi
	leaq	.LC15(%rip), %rcx
	leaq	.LC23(%rip), %rdx
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	call	tp_fma_f64
	leaq	.LC17(%rip), %rsi
	leaq	.LC18(%rip), %rcx
	leaq	.LC23(%rip), %rdx
	movl	$2, %edi
	vzeroupper
	movl	$1, %eax
	call	__printf_chk@PLT
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE35:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
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
	.long	1074790400
	.align 8
.LC5:
	.long	0
	.long	1074266112
	.align 8
.LC6:
	.long	0
	.long	1073741824
	.align 8
.LC7:
	.long	-611603343
	.long	1072693352
	.align 8
.LC8:
	.long	1717986918
	.long	1073112678
	.align 8
.LC9:
	.long	-858993459
	.long	1073007820
	.align 8
.LC10:
	.long	858993459
	.long	1072902963
	.align 8
.LC11:
	.long	-1717986918
	.long	1072798105
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
