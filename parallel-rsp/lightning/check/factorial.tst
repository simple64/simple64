.data	32
str:
.c	"%.0lf\n"
.code
	jmpi main
/*
 *	double factorial(unsigned long n) {
 *		double r = 1;
 *		while (n > 1) {
 *			r *= n;
 *			--n;
 *		}
 *		return r;
 *	}
 */
factorial:
	prolog
	arg $n
	getarg %r0 $n
	movi_d %f0 1.0
	extr_d %f1 %r0
	movr_d %f2 %f0
loop:
	bltr_d done %f1 %f2
	mulr_d %f0 %f0 %f1
	subr_d %f1 %f1 %f2
	jmpi loop
done:
	retr_d %f0
	epilog

/*
 *	int main(int argc, char *argv[]) {
 *		unsigned long v;
 *		double d;
 * 		if (argc == 2)
 *			v = strtoul(argv[1], NULL, 0);
 *		else
 *			v = 32;
 *		d = factorial(v);
 *		printf("%.0lf\n", d);
 *		return 0;
 *	}
 */
main:
	prolog
	arg $argc
	arg $argv
	getarg %r0 $argc
	bnei default %r0 2
	getarg %v0 $argv
	ldxi %r0 %v0 $(__WORDSIZE >> 3)
	prepare
		pushargr %r0
		pushargi 0
		pushargi 0
	finishi @strtoul
	retval %v0
	jmpi call
default:
    movi %v0 32
call:
	prepare
		pushargr %v0
	finishi factorial
	retval_d %f0
	prepare
		pushargi str
		ellipsis
		pushargr_d %f0
	finishi @printf
	reti 0
	epilog
