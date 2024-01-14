/*
 * Simple test for x86_64 rip relative access that can also be useful
 * on other ports when data is close to instruction pointer.
 */

#include <lightning.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#if defined(__sgi)
#  include <fcntl.h>
#endif
#include <unistd.h>

#ifndef MAP_ANON
#  define MAP_ANON			MAP_ANONYMOUS
#  ifndef MAP_ANONYMOUS
#    define MAP_ANONYMOUS		0
#  endif
#endif

#if !defined(__sgi)
#define  mmap_fd			-1
#endif

int
main(int argc, char *argv[])
{
    jit_uint8_t		 *ptr;
    jit_state_t		 *_jit;
    jit_word_t		  length;
#if defined(__sgi)
    int			  mmap_fd;
#endif
    void		(*function)(void);
    int			  mmap_prot, mmap_flags, result, pagesize;
    int			  mult;

#if defined(__ia64__)
    mult = 8;
#else
    mult = 2;
#endif
    pagesize = sysconf(_SC_PAGESIZE);
    if (pagesize < 4096)
	pagesize = 4096;

#if defined(__sgi)
    mmap_fd = open("/dev/zero", O_RDWR);
#endif

    mmap_prot = PROT_READ | PROT_WRITE;
#if !(__OpenBSD__ || __APPLE__)
    mmap_prot |= PROT_EXEC;
#endif
#if __NetBSD__
    mmap_prot = PROT_MPROTECT(mmap_prot);
    mmap_flags = 0;
#else
    mmap_flags = MAP_PRIVATE;
#endif
    mmap_flags |= MAP_ANON;
    ptr = mmap(NULL, pagesize * mult,  mmap_prot, mmap_flags, mmap_fd, 0);
    assert(ptr != MAP_FAILED);
#if defined(__sgi)
    close(mmap_fd);
#endif

    init_jit(argv[0]);
    _jit = jit_new_state();

    jit_prolog();
    jit_movi(JIT_R0, 'c');
    jit_sti_c(ptr + 0, JIT_R0);
    jit_movi(JIT_R0, 'C');
    jit_sti_c(ptr + 1, JIT_R0);

    jit_movi(JIT_R0, 's');
    jit_sti_s(ptr + 2, JIT_R0);
    jit_movi(JIT_R0, 'S');
    jit_sti_s(ptr + 4, JIT_R0);

    jit_movi(JIT_R0, 'i');
    jit_sti_i(ptr + 8, JIT_R0);
#if __WORDSIZE == 64
    jit_movi(JIT_R0, 'I');
    jit_sti_i(ptr + 12, JIT_R0);

    jit_movi(JIT_R0, 'l');
    jit_sti_l(ptr + 16, JIT_R0);
#endif
    jit_movi_f(JIT_F0, 1.0);
    jit_sti_f(ptr + 24, JIT_F0);
    jit_movi_d(JIT_F0, 2.0);
    jit_sti_d(ptr + 32, JIT_F0);

    jit_ldi_c(JIT_R0, ptr + 0);
    jit_ldi_s(JIT_R1, ptr + 2);
    jit_ldi_i(JIT_R2, ptr + 8);
#if __WORDSIZE == 64
    jit_ldi_l(JIT_V0, ptr + 16);
#endif
    jit_prepare();
#if __WORDSIZE == 64
    jit_pushargi((jit_word_t)"%c %c %c %c\n");
#else
    jit_pushargi((jit_word_t)"%c %c %c l\n");
#endif
    jit_ellipsis();
    jit_pushargr(JIT_R0);
    jit_pushargr(JIT_R1);
    jit_pushargr(JIT_R2);
#if __WORDSIZE == 64
    jit_pushargr(JIT_V0);
#endif
    jit_finishi(printf);

    jit_ldi_uc(JIT_R0, ptr + 1);
    jit_ldi_us(JIT_R1, ptr + 4);
#if __WORDSIZE == 64
    jit_ldi_ui(JIT_R2, ptr + 12);
#endif
    jit_prepare();
#if __WORDSIZE == 64
    jit_pushargi((jit_word_t)"%c %c %c\n");
#else
    jit_pushargi((jit_word_t)"%c %c I\n");
#endif
    jit_ellipsis();
    jit_pushargr(JIT_R0);
    jit_pushargr(JIT_R1);
#if __WORDSIZE == 64
    jit_pushargr(JIT_R2);
#endif
    jit_finishi(printf);

    jit_ldi_f(JIT_F0, ptr + 24);
    jit_extr_f_d(JIT_F0, JIT_F0);
    jit_ldi_d(JIT_F1, ptr + 32);

    jit_prepare();
    jit_pushargi((jit_word_t)"%.1f %.1f\n");
    jit_ellipsis();
    jit_pushargr_d(JIT_F0);
    jit_pushargr_d(JIT_F1);
    jit_finishi(printf);

    jit_realize();

    jit_set_code(ptr + pagesize, pagesize * (mult - 1));

 #if __NetBSD__ || __OpenBSD__ || __APPLE__
    result = mprotect(ptr, pagesize, PROT_READ | PROT_WRITE);
    assert(result == 0);
#endif
    function = jit_emit();
    if (function == NULL)
	abort();

    //jit_disassemble();
    jit_clear_state();
#if __NetBSD__ ||  __OpenBSD__ || __APPLE__
    result = mprotect(ptr + pagesize, pagesize, PROT_READ | PROT_EXEC);
    assert(result == 0);
#endif
    (*function)();
    jit_destroy_state();
    finish_jit();

    munmap(ptr, pagesize * mult);

    return (0);
}
