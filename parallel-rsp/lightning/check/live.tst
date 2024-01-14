.data	16
ok:
.c	"ok"

.code
	jmpi main

check_r0:
	prolog
        movi %v0 exit_r0
	movi %r0 1
	movi %r2 10
	// on x86 this changes %rax on other arches could use %r0 as temporary
	divi %r1 %r2 3
        live %r0
	// %r0 must still be 1
        jmpr %v0
exit_r0:
        retr %r0
	epilog

main:
	prolog
	calli check_r0
	retval %r1
	beqi r0_ok %r1 1
	calli @abort
r0_ok:
	prepare
		pushargi ok
	finishi @puts
	ret
	epilog
