/* If the fallback clor, clzr, ctor and ctzr are used, it might be better
 * to implement it as functions, as inlined it is almost as large as a
 * function.
 * Below is an example of how to do it.
 */

.data	4096
str_clo:
.c	"clo"
str_clz:
.c	"clz"
str_cto:
.c	"cto"
str_ctz:
.c	"ctz"
print_fmt:
#if __WORDSIZE == 64
.c	"%s (0x%016lx) %s = %d\n"
#else
.c	"%s (0x%08lx) %s = %d\n"
#endif
ok:
.c	"ok\n"

#define BIT2(OP, ARG, RES, R0, R1)			\
	movi %R1 ARG					\
	OP##r %R0 %R1					\
	beqi OP##R0##R1##ARG %R0 RES			\
	calli @abort					\
OP##R0##R1##ARG:

#define BIT1(OP, ARG, RES, V0, V1, V2, R0, R1, R2)	\
	BIT2(OP, ARG, RES, V0, V0)			\
	BIT2(OP, ARG, RES, V0, V1)			\
	BIT2(OP, ARG, RES, V0, V2)			\
	BIT2(OP, ARG, RES, V0, R0)			\
	BIT2(OP, ARG, RES, V0, R1)			\
	BIT2(OP, ARG, RES, V0, R2)

#define  BIT(OP, ARG, RES, V0, V1, V2, R0, R1, R2)	\
	BIT1(OP, ARG, RES, V1, V2, R0, R1, R2, V0)	\
	BIT1(OP, ARG, RES, V2, R0, R1, R2, V0, V1)	\
	BIT1(OP, ARG, RES, R0, R1, R2, V0, V1, V2)	\
	BIT1(OP, ARG, RES, R1, R2, V0, V1, V2, R0)	\
	BIT1(OP, ARG, RES, R2, V0, V1, V2, R0, R1)

#define  CLO(ARG, RES)					\
	 BIT(clo, ARG, RES, v0, v1, v2, r0, r1, r2)
#define  CLZ(ARG, RES)					\
	 BIT(clz, ARG, RES, v0, v1, v2, r0, r1, r2)
#define  CTO(ARG, RES)					\
	 BIT(cto, ARG, RES, v0, v1, v2, r0, r1, r2)
#define  CTZ(ARG, RES)					\
	 BIT(ctz, ARG, RES, v0, v1, v2, r0, r1, r2)

.code
	jmpi main
/*
	jit_uword_t cto(jit_uword_t r0) {
		r0 = ~r0;
		if (r0 == 0)
			r0 = __WORDSIZE;
		else
			r0 = ctz(r0);
		return r0;
	} 
 */
name cto
cto:
	prolog
	arg $in
	getarg %r0 $in
	comr %r0 %r0
	bnei do_cto %r0 0
	movi %r0 __WORDSIZE
	jmpi done_cto
do_cto:
	prepare
		pushargr %r0
	finishi ctz
	retval %r0
done_cto:
	retr %r0
	epilog

/*
	jit_uword_t clo(jit_uword_t r0) {
		r0 = ~r0;
		if (r0 == 0)
			r0 = __WORDSIZE;
		else
			r0 = clz(r0);
		return r0;
	} 
 */
name clo
clo:
	prolog
	arg $in
	getarg %r0 $in
	comr %r0 %r0
	bnei do_clo %r0 0
	movi %r0 __WORDSIZE
	jmpi done_clo
do_clo:
	prepare
		pushargr %r0
	finishi clz
	retval %r0
done_clo:
	retr %r0
	epilog

/*
	jit_uword_t clz(jit_word_t r1) {
		jit_uword_t	r0, r2;
		if (r1 == 0)
			r0 = __WORDSIZE;
		else {
			r0 = 0;
	#if __WORDSIZE == 64
			r2 = 0xffffffff00000000UL;
			if (!(r1 & r2)) {
				r1 <<= 32;
				r0 += 32;
			}
			r2 <<= 16;
	#else
			r2 = 0xffff0000UL;
	#endif
			if (!(r1 & r2)) {
				r1 <<= 16;
				r0 += 16;
			}
			r2 <<= 8;
			if (!(r1 & r2)) {
				r1 <<= 8;
				r0 += 8;
			}
			r2 <<= 4;
			if (!(r1 & r2)) {
				r1 <<= 4;
				r0 += 4;
			}
			r2 <<= 2;
			if (!(r1 & r2)) {
				r1 <<= 2;
				r0 += 2;
			}
			r2 <<= 1;
			if (!(r1 & r2))
				r0 += 1;
		}
		return r0;
	} 
 */
name clz
clz:
	prolog
	arg $in
	getarg %r1 $in
	bnei lun %r1 0
	reti __WORDSIZE
lun:
	movi %r0 0
#if __WORDSIZE == 64
	movi %r2 0xffffffff00000000
	bmsr l32 %r1 %r2
	lshi %r1 %r1 32
	addi %r0 %r0 32
l32:
	lshi %r2 %r2 16
#else
	movi %r2 0xffff0000
#endif
	bmsr l16 %r1 %r2
	lshi %r1 %r1 16
	addi %r0 %r0 16
l16:
	lshi %r2 %r2 8
	bmsr  l8 %r1 %r2
	lshi %r1 %r1 8
	addi %r0 %r0 8
l8:
	lshi %r2 %r2 4
	bmsr  l4 %r1 %r2
	lshi %r1 %r1 4
	addi %r0 %r0 4
l4:
	lshi %r2 %r2 2
	bmsr  l2 %r1 %r2
	lshi %r1 %r1 2
	addi %r0 %r0 2
l2:
	lshi %r2 %r2 1
	bmsr  l1 %r1 %r2
	addi %r0 %r0 1
l1:
	retr %r0
	epilog

/*
	jit_uword_t ctz(jit_uword_t r1) {
		jit_uword_t	r0, r2;
		if (r1 == 0)
			r0 = __WORDSIZE;
		else {
			r0 = 0;
	#if __WORDSIZE == 64
			r2 = 0xffffffffUL;;
			if (!(r1 & r2)) {
				r1 >>= 32;
				r0 += 32;
			}
			r2 >>= 16;
	#else
			r2 = 0xffffUL;;
	#endif
			if (!(r1 & r2)) {
				r1 >>= 16;
				r0 += 16;
			}
			r2 >>= 8;
			if (!(r1 & r2)) {
				r1 >>= 8;
				r0 += 8;
			}
			r2 >>= 4;
			if (!(r1 & r2)) {
				r1 >>= 4;
				r0 += 4;
			}
			r2 >>= 2;
			if (!(r1 & r2)) {
				r1 >>= 2;
				r0 += 2;
			}
			r2 >>= 1;
			if (!(r1 & r2))
				r0 += 1;
		}
		return r0;
	}
*/
name	ctz
ctz:
	prolog
	arg $in
	getarg %r1 $in
	bnei tun %r1 0
	reti __WORDSIZE
tun:
#if __WORDSIZE == 64
	movi %r0 0
	movi %r2 0xffffffff
	bmsr t32 %r1 %r2
	rshi_u %r1 %r1 32
	addi %r0 %r0 32
t32:
	rshi %r2 %r2 16
#else
	movi %r2 0xffff
#endif
	bmsr t16 %r1 %r2
	rshi_u %r1 %r1 16
	addi %r0 %r0 16
t16:
	rshi %r2 %r2 8
	bmsr  t8 %r1 %r2
	rshi_u %r1 %r1 8
	addi %r0 %r0 8
t8:
	rshi %r2 %r2 4
	bmsr  t4 %r1 %r2
	rshi_u %r1 %r1 4
	addi %r0 %r0 4
t4:
	rshi %r2 %r2 2
	bmsr  t2 %r1 %r2
	rshi_u %r1 %r1 2
	addi %r0 %r0 2
t2:
	rshi %r2 %r2 1
	bmsr  t1 %r1 %r2
	addi %r0 %r0 1
t1:
	retr %r0
	epilog

/*
	char *bitsprint(char *v0, jit_uword_t v1) {
		jit_uword_t r0, r1;
		memset(v0, '0', __WORDSIZE);
		v0[__WORDSIZE] = 0;
		for (r0 = 1L << (__WORDSIZE - 1), r1 = 0; r0; r0 >>= 1, ++r1) {
			if (v1 & r0)
				v0[r1] = '1';
		}
		return v0;
	}
 */
name bitsprint
bitsprint:
	prolog
	arg $buf
	arg $val
	getarg %v0 $buf
	getarg %v1 $val
	prepare
		pushargr %v0
		pushargi '0'
		pushargi __WORDSIZE
	finishi @memset
	movi %r0 0
	addi %r1 %v0 __WORDSIZE
	str_c %r1 %r0
	movi %r0 $(1 << (__WORDSIZE - 1))
	movi %r1 0
	movi %r2 '1'
bitloop:
	bmcr bitzero %v1 %r0
	stxr_c %r1 %v0 %r2
bitzero:
	addi %r1 %r1 1
	rshi_u %r0 %r0 1
	bnei bitloop %r0 0
	retr %v0
	epilog

/*
	#if 0
	int main(int argc, char *argv[]) {
		jit_uword_t	 r0, v0, v1, v2;
		char		 buf[80];
	#if __WORDSIZE == 64
		char		*fmt = "%s (0x%016lx) %s = %d\n";
		v0 = 0x8000000000000000UL;
		v2 = 0xffffffffffffffffUL;
	#else
		char		*fmt = "%s (0x%08lx) %s = %d\n";
		v0 = 0x80000000UL;
		v2 = 0xffffffffUL;
	#endif
		do {
			v1 = v0 - 1;
			r0 = clz(v0);
			bitsprint(buf, v0);
			printf(fmt, "clz", v0, buf, r0);
			r0 = clo(v2);
			bitsprint(buf, v2);
			printf(fmt, "clo", v2, buf, r0);
			r0 = ctz(v0);
			bitsprint(buf, v0);
			printf(fmt, "ctz", v0, buf, r0);
			r0 = cto(v1);
			bitsprint(buf, v1);
			printf(fmt, "cto", v1, buf, r0);
			v0 >>= 1;
			v2 <<= 1;
		} while ((jit_word_t)v1 > -1);
		return 0;
	}
	#endif
 */

/* Make it "#if 1" for a "debug mode", that helps in regenerating tables,
 * or temporary state while implementing optimized port specific versions. */
#if 0
#define CALL_FUNC	1
	name main
main:
	prolog
	allocai 80 $buf
#if __WORDSIZE == 64
	movi %v0 0x8000000000000000
	movi %v2 0xffffffffffffffff
#else
	movi %v0 0x80000000
	movi %v2 0xffffffff
#endif
loop:
	subi %v1 %v0 1
	addi %r1 %fp $buf
	prepare
		pushargr %r1
		pushargr %v0
	finishi bitsprint
#if CALL_FUNC
	prepare
		pushargr %v0
	finishi clz
	retval %r0
#else
	clzr %r0 %v0
#endif
	addi %r1 %fp $buf
	prepare
		pushargi print_fmt
		ellipsis
		pushargi str_clz
		pushargr %v0
		pushargr %r1
		pushargr %r0
	finishi @printf
	addi %r1 %fp $buf
	prepare
		pushargr %r1
		pushargr %v2
	finishi bitsprint
#if CALL_FUNC
	prepare
		pushargr %v2
	finishi clo
	retval %r0
#else
	clor %r0 %v2
#endif
	addi %r1 %fp $buf
	prepare
		pushargi print_fmt
		ellipsis
		pushargi str_clo
		pushargr %v2
		pushargr %r1
		pushargr %r0
	finishi @printf
	addi %r1 %fp $buf
	prepare
		pushargr %r1
		pushargr %v0
	finishi bitsprint
#if CALL_FUNC
	prepare
		pushargr %v0
	finishi ctz
	retval %r0
#else
	ctzr %r0 %v0
#endif
	addi %r1 %fp $buf
	prepare
		pushargi print_fmt
		ellipsis
		pushargi str_ctz
		pushargr %v0
		pushargr %r1
		pushargr %r0
	finishi @printf
	addi %r1 %fp $buf
	prepare
		pushargr %r1
		pushargr %v1
	finishi bitsprint
#if CALL_FUNC
	prepare
		pushargr %v1
	finishi cto
	retval %r0
#else
	ctor %r0 %v1
#endif
	addi %r1 %fp $buf
	prepare
		pushargi print_fmt
		ellipsis
		pushargi str_cto
		pushargr %v1
		pushargr %r1
		pushargr %r0
	finishi @printf
	rshi_u %v0 %v0 1
	lshi %v2 %v2 1
	bgti loop %v1 -1
	ret
	epilog
#else

	name main
main:
	prolog
#if __WORDSIZE == 32
	CLZ(0x80000000, 0)
	CLO(0xffffffff, 32)
	CTZ(0x80000000, 31)
	CTO(0x7fffffff, 31)
	CLZ(0x40000000, 1)
	CLO(0xfffffffe, 31)
	CTZ(0x40000000, 30)
	CTO(0x3fffffff, 30)
	CLZ(0x20000000, 2)
	CLO(0xfffffffc, 30)
	CTZ(0x20000000, 29)
	CTO(0x1fffffff, 29)
	CLZ(0x10000000, 3)
	CLO(0xfffffff8, 29)
	CTZ(0x10000000, 28)
	CTO(0x0fffffff, 28)
	CLZ(0x08000000, 4)
	CLO(0xfffffff0, 28)
	CTZ(0x08000000, 27)
	CTO(0x07ffffff, 27)
	CLZ(0x04000000, 5)
	CLO(0xffffffe0, 27)
	CTZ(0x04000000, 26)
	CTO(0x03ffffff, 26)
	CLZ(0x02000000, 6)
	CLO(0xffffffc0, 26)
	CTZ(0x02000000, 25)
	CTO(0x01ffffff, 25)
	CLZ(0x01000000, 7)
	CLO(0xffffff80, 25)
	CTZ(0x01000000, 24)
	CTO(0x00ffffff, 24)
	CLZ(0x00800000, 8)
	CLO(0xffffff00, 24)
	CTZ(0x00800000, 23)
	CTO(0x007fffff, 23)
	CLZ(0x00400000, 9)
	CLO(0xfffffe00, 23)
	CTZ(0x00400000, 22)
	CTO(0x003fffff, 22)
	CLZ(0x00200000, 10)
	CLO(0xfffffc00, 22)
	CTZ(0x00200000, 21)
	CTO(0x001fffff, 21)
	CLZ(0x00100000, 11)
	CLO(0xfffff800, 21)
	CTZ(0x00100000, 20)
	CTO(0x000fffff, 20)
	CLZ(0x00080000, 12)
	CLO(0xfffff000, 20)
	CTZ(0x00080000, 19)
	CTO(0x0007ffff, 19)
	CLZ(0x00040000, 13)
	CLO(0xffffe000, 19)
	CTZ(0x00040000, 18)
	CTO(0x0003ffff, 18)
	CLZ(0x00020000, 14)
	CLO(0xffffc000, 18)
	CTZ(0x00020000, 17)
	CTO(0x0001ffff, 17)
	CLZ(0x00010000, 15)
	CLO(0xffff8000, 17)
	CTZ(0x00010000, 16)
	CTO(0x0000ffff, 16)
	CLZ(0x00008000, 16)
	CLO(0xffff0000, 16)
	CTZ(0x00008000, 15)
	CTO(0x00007fff, 15)
	CLZ(0x00004000, 17)
	CLO(0xfffe0000, 15)
	CTZ(0x00004000, 14)
	CTO(0x00003fff, 14)
	CLZ(0x00002000, 18)
	CLO(0xfffc0000, 14)
	CTZ(0x00002000, 13)
	CTO(0x00001fff, 13)
	CLZ(0x00001000, 19)
	CLO(0xfff80000, 13)
	CTZ(0x00001000, 12)
	CTO(0x00000fff, 12)
	CLZ(0x00000800, 20)
	CLO(0xfff00000, 12)
	CTZ(0x00000800, 11)
	CTO(0x000007ff, 11)
	CLZ(0x00000400, 21)
	CLO(0xffe00000, 11)
	CTZ(0x00000400, 10)
	CTO(0x000003ff, 10)
	CLZ(0x00000200, 22)
	CLO(0xffc00000, 10)
	CTZ(0x00000200, 9)
	CTO(0x000001ff, 9)
	CLZ(0x00000100, 23)
	CLO(0xff800000, 9)
	CTZ(0x00000100, 8)
	CTO(0x000000ff, 8)
	CLZ(0x00000080, 24)
	CLO(0xff000000, 8)
	CTZ(0x00000080, 7)
	CTO(0x0000007f, 7)
	CLZ(0x00000040, 25)
	CLO(0xfe000000, 7)
	CTZ(0x00000040, 6)
	CTO(0x0000003f, 6)
	CLZ(0x00000020, 26)
	CLO(0xfc000000, 6)
	CTZ(0x00000020, 5)
	CTO(0x0000001f, 5)
	CLZ(0x00000010, 27)
	CLO(0xf8000000, 5)
	CTZ(0x00000010, 4)
	CTO(0x0000000f, 4)
	CLZ(0x00000008, 28)
	CLO(0xf0000000, 4)
	CTZ(0x00000008, 3)
	CTO(0x00000007, 3)
	CLZ(0x00000004, 29)
	CLO(0xe0000000, 3)
	CTZ(0x00000004, 2)
	CTO(0x00000003, 2)
	CLZ(0x00000002, 30)
	CLO(0xc0000000, 2)
	CTZ(0x00000002, 1)
	CTO(0x00000001, 1)
	CLZ(0x00000001, 31)
	CLO(0x80000000, 1)
	CTZ(0x00000001, 0)
	CTO(0x00000000, 0)
	CLZ(0x00000000, 32)
	CLO(0x00000000, 0)
	CTZ(0x00000000, 32)
	CTO(0xffffffff, 32)
#else
	CLZ(0x8000000000000000, 0)
	CLO(0xffffffffffffffff, 64)
	CTZ(0x8000000000000000, 63)
	CTO(0x7fffffffffffffff, 63)
	CLZ(0x4000000000000000, 1)
	CLO(0xfffffffffffffffe, 63)
	CTZ(0x4000000000000000, 62)
	CTO(0x3fffffffffffffff, 62)
	CLZ(0x2000000000000000, 2)
	CLO(0xfffffffffffffffc, 62)
	CTZ(0x2000000000000000, 61)
	CTO(0x1fffffffffffffff, 61)
	CLZ(0x1000000000000000, 3)
	CLO(0xfffffffffffffff8, 61)
	CTZ(0x1000000000000000, 60)
	CTO(0x0fffffffffffffff, 60)
	CLZ(0x0800000000000000, 4)
	CLO(0xfffffffffffffff0, 60)
	CTZ(0x0800000000000000, 59)
	CTO(0x07ffffffffffffff, 59)
	CLZ(0x0400000000000000, 5)
	CLO(0xffffffffffffffe0, 59)
	CTZ(0x0400000000000000, 58)
	CTO(0x03ffffffffffffff, 58)
	CLZ(0x0200000000000000, 6)
	CLO(0xffffffffffffffc0, 58)
	CTZ(0x0200000000000000, 57)
	CTO(0x01ffffffffffffff, 57)
	CLZ(0x0100000000000000, 7)
	CLO(0xffffffffffffff80, 57)
	CTZ(0x0100000000000000, 56)
	CTO(0x00ffffffffffffff, 56)
	CLZ(0x0080000000000000, 8)
	CLO(0xffffffffffffff00, 56)
	CTZ(0x0080000000000000, 55)
	CTO(0x007fffffffffffff, 55)
	CLZ(0x0040000000000000, 9)
	CLO(0xfffffffffffffe00, 55)
	CTZ(0x0040000000000000, 54)
	CTO(0x003fffffffffffff, 54)
	CLZ(0x0020000000000000, 10)
	CLO(0xfffffffffffffc00, 54)
	CTZ(0x0020000000000000, 53)
	CTO(0x001fffffffffffff, 53)
	CLZ(0x0010000000000000, 11)
	CLO(0xfffffffffffff800, 53)
	CTZ(0x0010000000000000, 52)
	CTO(0x000fffffffffffff, 52)
	CLZ(0x0008000000000000, 12)
	CLO(0xfffffffffffff000, 52)
	CTZ(0x0008000000000000, 51)
	CTO(0x0007ffffffffffff, 51)
	CLZ(0x0004000000000000, 13)
	CLO(0xffffffffffffe000, 51)
	CTZ(0x0004000000000000, 50)
	CTO(0x0003ffffffffffff, 50)
	CLZ(0x0002000000000000, 14)
	CLO(0xffffffffffffc000, 50)
	CTZ(0x0002000000000000, 49)
	CTO(0x0001ffffffffffff, 49)
	CLZ(0x0001000000000000, 15)
	CLO(0xffffffffffff8000, 49)
	CTZ(0x0001000000000000, 48)
	CTO(0x0000ffffffffffff, 48)
	CLZ(0x0000800000000000, 16)
	CLO(0xffffffffffff0000, 48)
	CTZ(0x0000800000000000, 47)
	CTO(0x00007fffffffffff, 47)
	CLZ(0x0000400000000000, 17)
	CLO(0xfffffffffffe0000, 47)
	CTZ(0x0000400000000000, 46)
	CTO(0x00003fffffffffff, 46)
	CLZ(0x0000200000000000, 18)
	CLO(0xfffffffffffc0000, 46)
	CTZ(0x0000200000000000, 45)
	CTO(0x00001fffffffffff, 45)
	CLZ(0x0000100000000000, 19)
	CLO(0xfffffffffff80000, 45)
	CTZ(0x0000100000000000, 44)
	CTO(0x00000fffffffffff, 44)
	CLZ(0x0000080000000000, 20)
	CLO(0xfffffffffff00000, 44)
	CTZ(0x0000080000000000, 43)
	CTO(0x000007ffffffffff, 43)
	CLZ(0x0000040000000000, 21)
	CLO(0xffffffffffe00000, 43)
	CTZ(0x0000040000000000, 42)
	CTO(0x000003ffffffffff, 42)
	CLZ(0x0000020000000000, 22)
	CLO(0xffffffffffc00000, 42)
	CTZ(0x0000020000000000, 41)
	CTO(0x000001ffffffffff, 41)
	CLZ(0x0000010000000000, 23)
	CLO(0xffffffffff800000, 41)
	CTZ(0x0000010000000000, 40)
	CTO(0x000000ffffffffff, 40)
	CLZ(0x0000008000000000, 24)
	CLO(0xffffffffff000000, 40)
	CTZ(0x0000008000000000, 39)
	CTO(0x0000007fffffffff, 39)
	CLZ(0x0000004000000000, 25)
	CLO(0xfffffffffe000000, 39)
	CTZ(0x0000004000000000, 38)
	CTO(0x0000003fffffffff, 38)
	CLZ(0x0000002000000000, 26)
	CLO(0xfffffffffc000000, 38)
	CTZ(0x0000002000000000, 37)
	CTO(0x0000001fffffffff, 37)
	CLZ(0x0000001000000000, 27)
	CLO(0xfffffffff8000000, 37)
	CTZ(0x0000001000000000, 36)
	CTO(0x0000000fffffffff, 36)
	CLZ(0x0000000800000000, 28)
	CLO(0xfffffffff0000000, 36)
	CTZ(0x0000000800000000, 35)
	CTO(0x00000007ffffffff, 35)
	CLZ(0x0000000400000000, 29)
	CLO(0xffffffffe0000000, 35)
	CTZ(0x0000000400000000, 34)
	CTO(0x00000003ffffffff, 34)
	CLZ(0x0000000200000000, 30)
	CLO(0xffffffffc0000000, 34)
	CTZ(0x0000000200000000, 33)
	CTO(0x00000001ffffffff, 33)
	CLZ(0x0000000100000000, 31)
	CLO(0xffffffff80000000, 33)
	CTZ(0x0000000100000000, 32)
	CTO(0x00000000ffffffff, 32)
	CLZ(0x0000000080000000, 32)
	CLO(0xffffffff00000000, 32)
	CTZ(0x0000000080000000, 31)
	CTO(0x000000007fffffff, 31)
	CLZ(0x0000000040000000, 33)
	CLO(0xfffffffe00000000, 31)
	CTZ(0x0000000040000000, 30)
	CTO(0x000000003fffffff, 30)
	CLZ(0x0000000020000000, 34)
	CLO(0xfffffffc00000000, 30)
	CTZ(0x0000000020000000, 29)
	CTO(0x000000001fffffff, 29)
	CLZ(0x0000000010000000, 35)
	CLO(0xfffffff800000000, 29)
	CTZ(0x0000000010000000, 28)
	CTO(0x000000000fffffff, 28)
	CLZ(0x0000000008000000, 36)
	CLO(0xfffffff000000000, 28)
	CTZ(0x0000000008000000, 27)
	CTO(0x0000000007ffffff, 27)
	CLZ(0x0000000004000000, 37)
	CLO(0xffffffe000000000, 27)
	CTZ(0x0000000004000000, 26)
	CTO(0x0000000003ffffff, 26)
	CLZ(0x0000000002000000, 38)
	CLO(0xffffffc000000000, 26)
	CTZ(0x0000000002000000, 25)
	CTO(0x0000000001ffffff, 25)
	CLZ(0x0000000001000000, 39)
	CLO(0xffffff8000000000, 25)
	CTZ(0x0000000001000000, 24)
	CTO(0x0000000000ffffff, 24)
	CLZ(0x0000000000800000, 40)
	CLO(0xffffff0000000000, 24)
	CTZ(0x0000000000800000, 23)
	CTO(0x00000000007fffff, 23)
	CLZ(0x0000000000400000, 41)
	CLO(0xfffffe0000000000, 23)
	CTZ(0x0000000000400000, 22)
	CTO(0x00000000003fffff, 22)
	CLZ(0x0000000000200000, 42)
	CLO(0xfffffc0000000000, 22)
	CTZ(0x0000000000200000, 21)
	CTO(0x00000000001fffff, 21)
	CLZ(0x0000000000100000, 43)
	CLO(0xfffff80000000000, 21)
	CTZ(0x0000000000100000, 20)
	CTO(0x00000000000fffff, 20)
	CLZ(0x0000000000080000, 44)
	CLO(0xfffff00000000000, 20)
	CTZ(0x0000000000080000, 19)
	CTO(0x000000000007ffff, 19)
	CLZ(0x0000000000040000, 45)
	CLO(0xffffe00000000000, 19)
	CTZ(0x0000000000040000, 18)
	CTO(0x000000000003ffff, 18)
	CLZ(0x0000000000020000, 46)
	CLO(0xffffc00000000000, 18)
	CTZ(0x0000000000020000, 17)
	CTO(0x000000000001ffff, 17)
	CLZ(0x0000000000010000, 47)
	CLO(0xffff800000000000, 17)
	CTZ(0x0000000000010000, 16)
	CTO(0x000000000000ffff, 16)
	CLZ(0x0000000000008000, 48)
	CLO(0xffff000000000000, 16)
	CTZ(0x0000000000008000, 15)
	CTO(0x0000000000007fff, 15)
	CLZ(0x0000000000004000, 49)
	CLO(0xfffe000000000000, 15)
	CTZ(0x0000000000004000, 14)
	CTO(0x0000000000003fff, 14)
	CLZ(0x0000000000002000, 50)
	CLO(0xfffc000000000000, 14)
	CTZ(0x0000000000002000, 13)
	CTO(0x0000000000001fff, 13)
	CLZ(0x0000000000001000, 51)
	CLO(0xfff8000000000000, 13)
	CTZ(0x0000000000001000, 12)
	CTO(0x0000000000000fff, 12)
	CLZ(0x0000000000000800, 52)
	CLO(0xfff0000000000000, 12)
	CTZ(0x0000000000000800, 11)
	CTO(0x00000000000007ff, 11)
	CLZ(0x0000000000000400, 53)
	CLO(0xffe0000000000000, 11)
	CTZ(0x0000000000000400, 10)
	CTO(0x00000000000003ff, 10)
	CLZ(0x0000000000000200, 54)
	CLO(0xffc0000000000000, 10)
	CTZ(0x0000000000000200, 9)
	CTO(0x00000000000001ff, 9)
	CLZ(0x0000000000000100, 55)
	CLO(0xff80000000000000, 9)
	CTZ(0x0000000000000100, 8)
	CTO(0x00000000000000ff, 8)
	CLZ(0x0000000000000080, 56)
	CLO(0xff00000000000000, 8)
	CTZ(0x0000000000000080, 7)
	CTO(0x000000000000007f, 7)
	CLZ(0x0000000000000040, 57)
	CLO(0xfe00000000000000, 7)
	CTZ(0x0000000000000040, 6)
	CTO(0x000000000000003f, 6)
	CLZ(0x0000000000000020, 58)
	CLO(0xfc00000000000000, 6)
	CTZ(0x0000000000000020, 5)
	CTO(0x000000000000001f, 5)
	CLZ(0x0000000000000010, 59)
	CLO(0xf800000000000000, 5)
	CTZ(0x0000000000000010, 4)
	CTO(0x000000000000000f, 4)
	CLZ(0x0000000000000008, 60)
	CLO(0xf000000000000000, 4)
	CTZ(0x0000000000000008, 3)
	CTO(0x0000000000000007, 3)
	CLZ(0x0000000000000004, 61)
	CLO(0xe000000000000000, 3)
	CTZ(0x0000000000000004, 2)
	CTO(0x0000000000000003, 2)
	CLZ(0x0000000000000002, 62)
	CLO(0xc000000000000000, 2)
	CTZ(0x0000000000000002, 1)
	CTO(0x0000000000000001, 1)
	CLZ(0x0000000000000001, 63)
	CLO(0x8000000000000000, 1)
	CTZ(0x0000000000000001, 0)
	CTO(0x0000000000000000, 0)
	CLZ(0x0000000000000000, 64)
	CLO(0x0000000000000000, 0)
	CTZ(0x0000000000000000, 64)
	CTO(0xffffffffffffffff, 64)
#endif
	prepare
		pushargi ok
	finishi @printf	
	reti 0
	epilog
#endif
