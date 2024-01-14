/*
 * Simple test of (un)protecting a code buffer.
 */

#include <lightning.h>
#include <stdio.h>
#include <assert.h>

#define MARKER 10

int
main(int argc, char *argv[])
{
    jit_state_t		 *_jit;
    jit_node_t           *load, *label, *ok;
    unsigned char        *ptr;
    void		(*function)(void);
    int			  mmap_prot, mmap_flags;

    init_jit(argv[0]);
    _jit = jit_new_state();

    jit_prolog();

    load = jit_movi(JIT_R0, 0);
    jit_ldr_c(JIT_R0, JIT_R0);
    ok = jit_forward();
    jit_patch_at(jit_beqi(JIT_R0, MARKER), ok);
    jit_prepare();
    jit_pushargi(1);
    jit_finishi(exit);
    label = jit_indirect();
    jit_skip(1);	      /* Reserves enough space for a byte.  */
    jit_patch_at(load, label);
    jit_link(ok);
    jit_prepare();
    jit_pushargi((jit_word_t)"%s\n");
    jit_ellipsis();
    jit_pushargi((jit_word_t)"ok");
    jit_finishi(printf);

    function = jit_emit();
    if (function == NULL)
	abort();

    jit_unprotect ();
    ptr = jit_address (label);
    *ptr = MARKER;
    jit_protect ();

    jit_clear_state();

    (*function)();

    jit_destroy_state();
    finish_jit();

    return (0);
}
