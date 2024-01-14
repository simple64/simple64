#if PROTO
#define fallback_save(r0)		_fallback_save(_jit, r0)
static void _fallback_save(jit_state_t*, jit_int32_t);
#define fallback_load(r0)		_fallback_load(_jit, r0)
static void _fallback_load(jit_state_t*, jit_int32_t);
#define fallback_save_regs(r0)		_fallback_save_regs(_jit, r0)
static void _fallback_save_regs(jit_state_t*, jit_int32_t);
#define fallback_load_regs(r0)		_fallback_load_regs(_jit, r0)
static void _fallback_load_regs(jit_state_t*, jit_int32_t);
#define fallback_calli(i0, i1)		_fallback_calli(_jit, i0, i1)
static void _fallback_calli(jit_state_t*, jit_word_t, jit_word_t);
#define fallback_casx(r0,r1,r2,r3,im)	_fallback_casx(_jit,r0,r1,r2,r3,im)
static void _fallback_casx(jit_state_t *, jit_int32_t, jit_int32_t,
			   jit_int32_t, jit_int32_t, jit_word_t);
#define fallback_clo(r0,r1)		_fallback_clo(_jit,r0,r1)
static void _fallback_clo(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_clz(r0,r1)		_fallback_clz(_jit,r0,r1)
static void _fallback_clz(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_cto(r0,r1)		_fallback_cto(_jit,r0,r1)
static void _fallback_cto(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_ctz(r0,r1)		_fallback_ctz(_jit,r0,r1)
static void _fallback_ctz(jit_state_t*, jit_int32_t, jit_int32_t);
#  if defined(__ia64__)
#    define fallback_patch_jmpi(inst,lbl)				\
    do {								\
	sync();								\
	patch_at(jit_code_jmpi, inst, lbl);				\
    } while (0)
#  else
#    define fallback_patch_jmpi(inst,lbl) fallback_patch_at(inst,lbl)
#  endif
#  if defined(__arm__)
#    define fallback_patch_at(inst,lbl)	patch_at(arm_patch_jump,inst,lbl)
#  elif defined(__ia64__)
#    define fallback_patch_at(inst,lbl)					\
    do {								\
	sync();								\
	patch_at(jit_code_bnei, inst, lbl);				\
    } while (0);
#  else
#    define fallback_patch_at(inst,lbl)	patch_at(inst,lbl)
#  endif
#  if defined(__mips__)
#    define fallback_jmpi(i0)		jmpi(i0,NULL,1)
#  elif defined(__arm__)
#    define fallback_jmpi(i0)		jmpi_p(i0,1)
#  elif defined(__s390__) || defined(__s390x__)
#    define fallback_jmpi(i0)		jmpi(i0,1)
#  else
#    define fallback_jmpi(i0)		jmpi(i0)
#  endif
#  if defined(__mips__)
#    define fallback_bnei(i0,r0,i1)	bnei(i0,r0,i1,NULL)
#  elif defined(__s390__) || defined(__s390x__)
#    define fallback_bnei(i0,r0,i1)	bnei_p(i0,r0,i1)
#  else
#    define fallback_bnei(i0,r0,i1)	bnei(i0,r0,i1)
#  endif
#  if defined(__s390__) || defined(__s390x__)
#    define fallback_bmsr(i0,r0,r1)	bmsr_p(i0,r0,r1)
#  else
#    define fallback_bmsr(i0,r0,r1)	bmsr(i0,r0,r1)
#  endif
#endif

#if CODE
static void
_fallback_save(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		offset, regno, spec;
    for (offset = 0; offset < JIT_R_NUM; offset++) {
	spec =  _rvs[offset].spec;
	regno = jit_regno(spec);
	if (regno == r0) {
	    if (!(spec & jit_class_sav))
		stxi(_jitc->function->regoff[JIT_R(offset)], rn(JIT_FP), regno);
	    break;
	}
    }
}

static void
_fallback_load(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		offset, regno, spec;
    for (offset = 0; offset < JIT_R_NUM; offset++) {
	spec =  _rvs[offset].spec;
	regno = jit_regno(spec);
	if (regno == r0) {
	    if (!(spec & jit_class_sav))
		ldxi(regno, rn(JIT_FP), _jitc->function->regoff[JIT_R(offset)]);
	    break;
	}
    }
}

static void
_fallback_save_regs(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		regno, spec;
    for (regno = 0; regno < _jitc->reglen; regno++) {
	spec =  _rvs[regno].spec;
	if ((jit_regset_tstbit(&_jitc->regarg, regno) ||
	     jit_regset_tstbit(&_jitc->reglive, regno)) &&
	    !(spec & jit_class_sav)) {
	    if (!_jitc->function->regoff[regno]) {
		_jitc->function->regoff[regno] =
		    jit_allocai(spec & jit_class_gpr ?
				sizeof(jit_word_t) : sizeof(jit_float64_t));
		_jitc->again = 1;
	    }
	    if ((spec & jit_class_gpr) && rn(regno) == r0)
		continue;
	    jit_regset_setbit(&_jitc->regsav, regno);
	    if (spec & jit_class_gpr)
		emit_stxi(_jitc->function->regoff[regno], JIT_FP, regno);
	    else
		emit_stxi_d(_jitc->function->regoff[regno], JIT_FP, regno);
	}
    }
}

static void
_fallback_load_regs(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		regno, spec;
    for (regno = 0; regno < _jitc->reglen; regno++) {
	spec =  _rvs[regno].spec;
	if ((jit_regset_tstbit(&_jitc->regarg, regno) ||
	     jit_regset_tstbit(&_jitc->reglive, regno)) &&
	    !(spec & jit_class_sav)) {
	    if ((spec & jit_class_gpr) && rn(regno) == r0)
		continue;
	    jit_regset_setbit(&_jitc->regsav, regno);
	    if (spec & jit_class_gpr)
		emit_ldxi(regno, JIT_FP, _jitc->function->regoff[regno]);
	    else
		emit_ldxi_d(regno, JIT_FP, _jitc->function->regoff[regno]);
	}
    }
}

static void
_fallback_calli(jit_state_t *_jit, jit_word_t i0, jit_word_t i1)
{
#  if defined(__arm__)
    movi(rn(_R0), i1);
#  elif defined(__hppa__)
    movi(_R26_REGNO, i1);
#  endif
#  if defined(__arm__)
    calli(i0, jit_exchange_p());
#  elif defined(__mips__)
    calli(i0, NULL, 0);
#  elif defined(__powerpc__) && _CALL_SYSV
    calli(i0, 0);
#  elif defined(__s390__) || defined(__s390x__)
    calli(i0, 0);
#  else
    calli(i0);
#  endif
}

static void
_fallback_casx(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	       jit_int32_t r2, jit_int32_t r3, jit_word_t i0)
{
    jit_int32_t		r1_reg, iscasi;
    jit_word_t		jump, done;
    /* XXX only attempts to fallback cas for lightning jit code */
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    if ((iscasi = r1 == _NOREG)) {
	r1_reg = jit_get_reg(jit_class_gpr|jit_class_sav);
	r1 = rn(r1_reg);
	movi(r1, i0);
    }
    fallback_save_regs(r0);
    fallback_calli((jit_word_t)pthread_mutex_lock, (jit_word_t)&mutex);
    fallback_load(r1);
    ldr(r0, r1);
    fallback_load(r2);
    eqr(r0, r0, r2);
    fallback_save(r0);
    jump = fallback_bnei(_jit->pc.w, r0, 1);
    fallback_load(r3);
#  if __WORDSIZE == 32
    str_i(r1, r3);
#  else
    str_l(r1, r3);
#  endif
    /* done: */
    done = _jit->pc.w;
    fallback_calli((jit_word_t)pthread_mutex_unlock, (jit_word_t)&mutex);
    fallback_load(r0);
    fallback_patch_at(jump, done);
    fallback_load_regs(r0);
    if (iscasi)
	jit_unget_reg(r1_reg);
}

static void
_fallback_clo(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		clz, done;
    comr(r0, r1);
    clz = fallback_bnei(_jit->pc.w, r0, 0);
    movi(r0, __WORDSIZE);
    done = fallback_jmpi(_jit->pc.w);
    fallback_patch_at(clz, _jit->pc.w);
    fallback_clz(r0, r0);
    fallback_patch_jmpi(done, _jit->pc.w);
}

static void
_fallback_clz(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		r1_reg, r2, r2_reg;
    jit_word_t		clz, l32, l16, l8, l4, l2, l1;
    l32 = fallback_bnei(_jit->pc.w, r1, 0);
    movi(r0, __WORDSIZE);
    clz = fallback_jmpi(_jit->pc.w);
    fallback_patch_at(l32, _jit->pc.w);
    r2_reg = jit_get_reg(jit_class_gpr);
    r2 = rn(r2_reg);
    r1_reg = jit_get_reg(jit_class_gpr);
    movr(rn(r1_reg), r1);
    r1 = rn(r1_reg);
    movi(r0, 0);
#  if __WORDSIZE == 64
    movi(r2, 0xffffffff00000000UL);
    l32 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 32);
    addi(r0, r0, 32);
    fallback_patch_at(l32, _jit->pc.w);
    lshi(r2, r2, 16);
#  else
    movi(r2, 0xffff0000UL);
#  endif
    l16 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 16);
    addi(r0, r0, 16);
    fallback_patch_at(l16, _jit->pc.w);
    lshi(r2, r2, 8);
    l8 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 8);
    addi(r0, r0, 8);
    fallback_patch_at(l8, _jit->pc.w);
    lshi(r2, r2, 4);
    l4 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 4);
    addi(r0, r0, 4);
    fallback_patch_at(l4, _jit->pc.w);
    lshi(r2, r2, 2);
    l2 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 2);
    addi(r0, r0, 2);
    fallback_patch_at(l2, _jit->pc.w);
    lshi(r2, r2, 1);
    l1 = fallback_bmsr(_jit->pc.w, r1, r2);
    addi(r0, r0, 1);
    fallback_patch_at(l1, _jit->pc.w);
    fallback_patch_jmpi(clz, _jit->pc.w);
    jit_unget_reg(r2_reg);
    jit_unget_reg(r1_reg);
}

static void
_fallback_cto(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		ctz, done;
    comr(r0, r1);
    ctz = fallback_bnei(_jit->pc.w, r0, 0);
    movi(r0, __WORDSIZE);
    done = fallback_jmpi(_jit->pc.w);
    fallback_patch_at(ctz, _jit->pc.w);
    fallback_ctz(r0, r0);
    fallback_patch_jmpi(done, _jit->pc.w);
}

static void
_fallback_ctz(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		r1_reg, r2, r2_reg;
    jit_word_t		ctz, l32, l16, l8, l4, l2, l1;
    l32 = fallback_bnei(_jit->pc.w, r1, 0);
    movi(r0, __WORDSIZE);
    ctz = fallback_jmpi(_jit->pc.w);
    fallback_patch_at(l32, _jit->pc.w);
    r2_reg = jit_get_reg(jit_class_gpr);
    r2 = rn(r2_reg);
    r1_reg = jit_get_reg(jit_class_gpr);
    movr(rn(r1_reg), r1);
    r1 = rn(r1_reg);
    movi(r0, 0);
#  if __WORDSIZE == 64
    movi(r2, 0xffffffffUL);
    l32 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 32);
    addi(r0, r0, 32);
    fallback_patch_at(l32, _jit->pc.w);
    rshi(r2, r2, 16);
#  else
    movi(r2, 0xffffUL);
#  endif
    l16 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 16);
    addi(r0, r0, 16);
    fallback_patch_at(l16, _jit->pc.w);
    rshi(r2, r2, 8);
    l8 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 8);
    addi(r0, r0, 8);
    fallback_patch_at(l8, _jit->pc.w);
    rshi(r2, r2, 4);
    l4 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 4);
    addi(r0, r0, 4);
    fallback_patch_at(l4, _jit->pc.w);
    rshi(r2, r2, 2);
    l2 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 2);
    addi(r0, r0, 2);
    fallback_patch_at(l2, _jit->pc.w);
    rshi(r2, r2, 1);
    l1 = fallback_bmsr(_jit->pc.w, r1, r2);
    addi(r0, r0, 1);
    fallback_patch_at(l1, _jit->pc.w);
    fallback_patch_jmpi(ctz, _jit->pc.w);
    jit_unget_reg(r2_reg);
    jit_unget_reg(r1_reg);
}
#endif
