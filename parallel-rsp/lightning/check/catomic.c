#include <lightning.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#if DEBUG
volatile
#endif
jit_word_t	lock;
pthread_t	tids[4];

#if DEBUG
int debug_offset(void)
{
    int		i;
    pthread_t	self = pthread_self();
    for (i = 0; i < 4; ++i)
	if (tids[i] == self)
	    return i;
    return -1;
}

void debug_spin(void)
{
    printf("  spin %d : %ld\n", debug_offset(), lock);
}

void debug_lock(void)
{
    printf("  lock %d : %ld\n", debug_offset(), lock);
}

void debug_unlock(void)
{
    printf("unlock %d : %ld\n", debug_offset(), lock);
}
#define DEBUG_SPIN()	jit_calli(debug_spin)
#define DEBUG_LOCK()	jit_calli(debug_lock)
#define DEBUG_UNLOCK()	jit_calli(debug_unlock)
#else
#define DEBUG_SPIN()	/**/
#define DEBUG_LOCK()	/**/
#define DEBUG_UNLOCK()	/**/
#endif

void alarm_handler(int unused)
{
    _exit(1);
}

int
main(int argc, char *argv[])
{
    jit_state_t		*_jit;
    void		(*code)(void);
    jit_node_t		 *jmpi_main, *label;
    jit_node_t		 *func0, *func1, *func2, *func3;
    jit_node_t		 *patch0, *patch1, *patch2, *patch3;

    /* If there is any bug, do not hang in "make check" */
    signal(SIGALRM, alarm_handler);
    alarm(5);
  
    init_jit(argv[0]);
    _jit = jit_new_state();

    jmpi_main = jit_jmpi();

#define defun(name, line)					\
    jit_name(#name);						\
    jit_note("catomic.c", line);				\
    name = jit_label();						\
     jit_prolog();						\
    jit_movi(JIT_V0, (jit_word_t)&lock);			\
    jit_movi(JIT_V1, 0);					\
    jit_movi(JIT_V2, line);					\
    /* spin until get the lock */				\
    DEBUG_SPIN();						\
    label = jit_label();					\
    jit_casr(JIT_R0, JIT_V0, JIT_V1, JIT_V2);			\
    jit_patch_at(jit_beqi(JIT_R0, 0), label);			\
    /* lock acquired */						\
    DEBUG_LOCK();						\
    jit_prepare();						\
    /* pretend to be doing something useful for 0.01 sec
     * while holding the lock */				\
    jit_pushargi(10000);					\
    jit_finishi(usleep);					\
    /* release lock */						\
    DEBUG_UNLOCK();						\
    jit_movi(JIT_V1, 0);					\
    jit_str(JIT_V0, JIT_V1);					\
    /* Now test casi */						\
    jit_movi(JIT_V1, 0);					\
    jit_movi(JIT_V2, line);					\
    /* spin until get the lock */				\
    DEBUG_SPIN();						\
    label = jit_label();					\
    jit_casi(JIT_R0, (jit_word_t)&lock, JIT_V1, JIT_V2);	\
    jit_patch_at(jit_beqi(JIT_R0, 0), label);			\
    /* lock acquired */						\
    DEBUG_LOCK();						\
    jit_prepare();						\
    /* pretend to be doing something useful for 0.01 sec
     * while holding the lock */				\
    jit_pushargi(10000);					\
    jit_finishi(usleep);					\
    jit_prepare();						\
    /* for make check, just print "ok" */			\
    jit_pushargi((jit_word_t)"ok");				\
    /*jit_pushargi((jit_word_t)#name);*/			\
    jit_finishi(puts);						\
    /* release lock */						\
    DEBUG_UNLOCK();						\
    jit_movi(JIT_V1, 0);					\
    jit_str(JIT_V0, JIT_V1);					\
    jit_ret();							\
    jit_epilog();
    defun(func0, __LINE__);
    defun(func1, __LINE__);
    defun(func2, __LINE__);
    defun(func3, __LINE__);

    jit_patch(jmpi_main);
    jit_name("main");
    jit_note("catomic.c", __LINE__);
    jit_prolog();

#define start(tid)						\
    /* set JIT_R0 to thread function */				\
    jit_patch_at(jit_movi(JIT_R0, 0), func##tid);		\
    jit_prepare();						\
    /* pthread_t first argument */				\
    jit_pushargi((jit_word_t)(tids + tid));			\
    /* pthread_attr_t second argument */			\
    jit_pushargi((jit_word_t)NULL);				\
    /* start routine third argument */				\
    jit_pushargr(JIT_R0);					\
    /* argument to start routine fourth argument */		\
    jit_pushargi((jit_word_t)NULL);				\
    /* start thread */						\
    jit_finishi(pthread_create);
    /* spawn four threads */
    start(0);
    start(1);
    start(2);
    start(3);

#define join(tid)						\
    /* load pthread_t value in JIT_R0 */			\
    jit_movi(JIT_R0, (jit_word_t)tids);				\
    if (__WORDSIZE == 64 && sizeof(pthread_t) == 4)		\
	jit_ldxi_i(JIT_R0, JIT_R0, tid * sizeof(pthread_t));	\
    else							\
	jit_ldxi(JIT_R0, JIT_R0, tid * sizeof(pthread_t));	\
    jit_prepare();						\
    jit_pushargr(JIT_R0);					\
    jit_pushargi((jit_word_t)NULL);				\
    jit_finishi(pthread_join);
    /* wait for threads to finish */
    join(0);
    join(1);
    join(2);
    join(3);

    jit_prepare();
    jit_pushargi((jit_word_t)"ok");
    jit_finishi(puts);

    jit_ret();
    jit_epilog();

    code = jit_emit();

#if DEBUG
    jit_disassemble();
#endif

    jit_clear_state();

    /* let first thread acquire the lock */
    lock = 0;
    
    (*code)();
    jit_destroy_state();

    finish_jit();

    return (0);
}
