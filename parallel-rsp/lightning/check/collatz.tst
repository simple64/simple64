.data	32
str:
.c	"%lu\n"
.code
	jmpi main
/*
 *	unsigned long collatz(unsigned long n) {
 *		unsigned long r = n;
 *		if (!(r & 1)) {
 *			r = r / 2;
 *			return r;
 *		}
 *		r = r * 3;
 *		r = r + 1;
 *		return r;
 *	}
 */
collatz:
	prolog
	arg $n
	getarg %r0 $n
	bmsi odd %r0 1
	//divi_u %r0 %r0 2
	rshi_u %r0 %r0 1
	retr %r0
odd:
	muli %r0 %r0 3
	addi %r0 %r0 1
	retr %r0
	epilog

/*
 *	int main(int argc, char *argv[]) {
 *		unsigned long v;
 * 		if (argc == 2)
 *			v = strtoul(argv[1], NULL, 0);
 *		else
 *			v = (1L << __WORDSIZE / 2) - 1;
 *		while (1) {
 *			printf("%ld\n", v);
 *			if (v <= 1)
 *				break;
 *			v = collatz(v);
 *		}
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
	jmpi loop
default:
    movi %v0 $((1 << __WORDSIZE / 2)  - 1)
loop:
	prepare
		pushargi str
		ellipsis
		pushargr %v0
	finishi @printf
	blei_u done %v0 1
	prepare
		pushargr %v0
	finishi collatz
	retval %v0
	jmpi loop
done:
	reti 0
	epilog
