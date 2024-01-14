.data	32
fmt:
.c	"%d\n"
.code
	prolog
        skip    4
        prepare
                pushargi fmt
                ellipsis
                pushargi 10
        finishi @printf
        ret
        epilog
