.data	8
ok:
.c	"ok\n"

#define CMOVR(N, T, OP, I0, I1, V, R0, R1, R2)		\
	movi %R1 I0					\
	movi %R2 I1					\
	movi %R0 V					\
	OP##r##T %R0 %R1 %R2				\
	beqi OP##T##N##r_##R0##R1##R2 %R0 V		\
	calli @abort					\
OP##T##N##r_##R0##R1##R2:

/* reg0 = reg1 op reg0 */
#define CMOVR1(N, T, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I1					\
	movi %R1 I0					\
	movi %R2 V					\
	OP##r##T %R0 %R1 %R0				\
	beqr OP##T##N##r_1##R0##R1##R2 %R0 %R2		\
	calli @abort					\
OP##T##N##r_1##R0##R1##R2:

#define TEST_CMOV1(N, OP, I0, I1, V, R0, R1, R2)	\
	CMOVR(N, , OP, I0, I1, V, R0, R1, R2)		\
	CMOVR1(N, , OP, I0, I1, V, R0, R1, R2)		\

#define TEST_CMOV(N, OP, I0, I1, V)			\
	TEST_CMOV1(N, OP, I0, I1, V, v0, v1, v2)	\
	TEST_CMOV1(N, OP, I0, I1, V, v0, v1, r0)	\
	TEST_CMOV1(N, OP, I0, I1, V, v0, v1, r1)	\
	TEST_CMOV1(N, OP, I0, I1, V, v0, v1, r2)	\
	TEST_CMOV1(N, OP, I0, I1, V, v1, v2, r1)	\
	TEST_CMOV1(N, OP, I0, I1, V, v1, v2, r2)	\
	TEST_CMOV1(N, OP, I0, I1, V, v2, r0, r1)	\
	TEST_CMOV1(N, OP, I0, I1, V, v2, r0, r2)	\
	TEST_CMOV1(N, OP, I0, I1, V, r0, r1, r2)

#define MOVZR(N, I0, I1, V)	TEST_CMOV(N, movz, I0, I1, V)
#define MOVNR(N, I0, I1, V)	TEST_CMOV(N, movn, I0, I1, V)

.code
	prolog

	MOVZR(0, 0x0, 0x0, 0x0)
	MOVZR(1, 0xf7de, 0x0, 0xf7de)

	MOVZR(2, 0x0, 0xdead, 0xdead)
	MOVZR(3, 0xf7de, 0xdead, 0xdead)

	MOVNR(0, 0x0, 0x0, 0x0)
	MOVNR(1, 0xf7de, 0x0, 0x0)

	MOVNR(2, 0x0, 0xdead, 0x0)
	MOVNR(3, 0xf7de, 0xdead, 0xf7de)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
