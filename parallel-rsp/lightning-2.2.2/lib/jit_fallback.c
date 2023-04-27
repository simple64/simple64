#if PROTO
#define USE_BIT_TABLES			1
#define USE_BITSWAP_UNROLLED		0
#define USE_BITSWAP_LOOP		0
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
#if !defined(__arm__)
#  ifndef movr_w_f
#    define movr_w_f(r0, r1)		fallback_movr_w_f(r0, r1)
#    define fallback_movr_w_f(r0,r1)	_fallback_movr_w_f(_jit,r0,r1)
static void _fallback_movr_w_f(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  ifndef movr_f_w
#    define movr_f_w(r0, r1)		fallback_movr_f_w(r0, r1)
#    define fallback_movr_f_w(r0,r1)	_fallback_movr_f_w(_jit,r0,r1)
static void _fallback_movr_f_w(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  if __WORDSIZE == 32
#    ifndef movr_ww_d
#      define movr_ww_d(r0, r1, r2)	fallback_movr_ww_d(r0, r1, r2)
#      define fallback_movr_ww_d(r0,r1,r2) _fallback_movr_ww_d(_jit,r0,r1,r2)
static void _fallback_movr_ww_d(jit_state_t*,
				jit_int32_t,jit_int32_t,jit_int32_t);
#    endif
#    ifndef movr_d_ww
#      define fallback_movr_d_ww(r0,r1,r2) _fallback_movr_d_ww(_jit,r0,r1,r2)
static void _fallback_movr_d_ww(jit_state_t*,
				jit_int32_t,jit_int32_t,jit_int32_t);
#      define movr_d_ww(r0, r1, r2)	fallback_movr_d_ww(r0, r1, r2)
#    endif
#  else
#    ifndef movr_w_d
#      define movr_w_d(r0, r1)		fallback_movr_w_d(r0, r1);
#      define fallback_movr_w_d(r0,r1)	_fallback_movr_w_d(_jit,r0,r1)
static void _fallback_movr_w_d(jit_state_t*, jit_int32_t, jit_int32_t);
#    endif
#    ifndef movr_d_w
#      define movr_d_w(r0, r1)		fallback_movr_d_w(r0, r1);
#      define fallback_movr_d_w(r0,r1)	_fallback_movr_d_w(_jit,r0,r1)
static void _fallback_movr_d_w(jit_state_t*, jit_int32_t, jit_int32_t);
#    endif
#  endif
#endif
#ifndef unldr
#  define unldr(r0, r1, i0)		fallback_unldr(r0, r1, i0)
#endif
#ifndef unldi
#  define unldi(r0, i0, i1)		fallback_unldi(r0, i0, i1)
#endif
#ifndef unstr
#  define unstr(r0, r1, i0)		fallback_unstr(r0, r1, i0)
#endif
#ifndef unsti
#  define unsti(i0, r0, i1)		fallback_unsti(i0, r0, i1)
#endif
#define fallback_clo(r0,r1)		_fallback_clo(_jit,r0,r1)
static void _fallback_clo(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_clz(r0,r1)		_fallback_clz(_jit,r0,r1)
static void _fallback_clz(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_cto(r0,r1)		_fallback_cto(_jit,r0,r1)
static void _fallback_cto(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_ctz(r0,r1)		_fallback_ctz(_jit,r0,r1)
static void _fallback_ctz(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_rbit(r0,r1)		_fallback_rbit(_jit, r0, r1)
static void _fallback_rbit(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_popcnt(r0,r1)		_fallback_popcnt(_jit, r0, r1)
static void _fallback_popcnt(jit_state_t*, jit_int32_t, jit_int32_t);
#define fallback_lrotr(r0, r1, r2)	_fallback_lrotr(_jit, r0, r1, r2)
static void _fallback_lrotr(jit_state_t*, jit_int32_t,jit_int32_t,jit_int32_t);
#define fallback_lroti(r0, r1, i0)	_fallback_lroti(_jit, r0, r1, i0)
static void _fallback_lroti(jit_state_t*, jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_rrotr(r0, r1, r2)	_fallback_rrotr(_jit, r0, r1, r2)
static void _fallback_rrotr(jit_state_t*, jit_int32_t,jit_int32_t,jit_int32_t);
#define fallback_rroti(r0, r1, i0)	_fallback_rroti(_jit, r0, r1, i0)
static void _fallback_rroti(jit_state_t*, jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_ext(r0,r1,i0,i1)	_fallback_ext(_jit,r0,r1,i0,i1)
static void _fallback_ext(jit_state_t*,
			  jit_int32_t,jit_int32_t,jit_word_t,jit_word_t);
#define fallback_ext_u(r0,r1,i0,i1)	_fallback_ext_u(_jit,r0,r1,i0,i1)
static void _fallback_ext_u(jit_state_t*,
			    jit_int32_t,jit_int32_t,jit_word_t,jit_word_t);
#define fallback_dep(r0,r1,i0,i1)	_fallback_dep(_jit,r0,r1,i0,i1)
static void _fallback_dep(jit_state_t*,
			  jit_int32_t,jit_int32_t,jit_word_t,jit_word_t);
#define fallback_qlshr(r0,r1,r2,r3)	_fallback_qlshr(_jit,r0,r1,r2,r3)
static void _fallback_qlshr(jit_state_t *_jit,
			    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define fallback_qlshi(r0,r1,r2,i0)	_fallback_qlshi(_jit,r0,r1,r2,i0)
static void _fallback_qlshi(jit_state_t *_jit,
			    jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_qlshr_u(r0,r1,r2,r3)	_fallback_qlshr_u(_jit,r0,r1,r2,r3)
static void _fallback_qlshr_u(jit_state_t *_jit,
			      jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define fallback_qlshi_u(r0,r1,r2,i0)	_fallback_qlshi_u(_jit,r0,r1,r2,i0)
static void _fallback_qlshi_u(jit_state_t *_jit,
			      jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_qrshr(r0,r1,r2,r3)	_fallback_qrshr(_jit,r0,r1,r2,r3)
static void _fallback_qrshr(jit_state_t *_jit,
			    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define fallback_qrshi(r0,r1,r2,i0)	_fallback_qrshi(_jit,r0,r1,r2,i0)
static void _fallback_qrshi(jit_state_t *_jit,
			    jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_qrshr_u(r0,r1,r2,r3)	_fallback_qrshr_u(_jit,r0,r1,r2,r3)
static void _fallback_qrshr_u(jit_state_t *_jit,
			      jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#define fallback_qrshi_u(r0,r1,r2,i0)	_fallback_qrshi_u(_jit,r0,r1,r2,i0)
static void _fallback_qrshi_u(jit_state_t *_jit,
			      jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#define unldr2(r0, r1, sign)		_unldr2(_jit, r0, r1, sign)
static void _unldr2(jit_state_t*,jit_int32_t,jit_int32_t,jit_bool_t);
#define unldi2(r0, i1, sign)		_unldi2(_jit, r0, i1, sign)
static void _unldi2(jit_state_t*,jit_int32_t,jit_word_t,jit_bool_t);
#define unldr3(r0, r1, sign)		_unldr3(_jit, r0, r1, sign)
static void _unldr3(jit_state_t*,jit_int32_t,jit_int32_t,jit_bool_t);
#define unldi3(r0, i1, sign)		_unldi3(_jit, r0, i1, sign)
static void _unldi3(jit_state_t*,jit_int32_t,jit_word_t,jit_bool_t);
#if __WORDSIZE == 32
#  define unldr4(r0, r1)		_unldr4(_jit, r0, r1)
static void _unldr4(jit_state_t*,jit_int32_t,jit_int32_t);
#  define unldi4(r0, i1)		_unldi4(_jit, r0, i1)
static void _unldi4(jit_state_t*,jit_int32_t,jit_word_t);
#else
#  define unldr4(r0, r1, sign)		_unldr4(_jit, r0, r1, sign)
static void _unldr4(jit_state_t*,jit_int32_t,jit_int32_t,jit_bool_t);
#  define unldi4(r0, i1, sign)		_unldi4(_jit, r0, i1, sign)
static void _unldi4(jit_state_t*,jit_int32_t,jit_word_t,jit_bool_t);
#  define unldr5(r0, r1, sign)		_unldr5(_jit, r0, r1, sign)
static void _unldr5(jit_state_t*,jit_int32_t,jit_int32_t,jit_bool_t);
#  define unldi5(r0, i1, sign)		_unldi5(_jit, r0, i1, sign)
static void _unldi5(jit_state_t*,jit_int32_t,jit_word_t,jit_bool_t);
#  define unldr6(r0, r1, sign)		_unldr6(_jit, r0, r1, sign)
static void _unldr6(jit_state_t*,jit_int32_t,jit_int32_t,jit_bool_t);
#  define unldi6(r0, i1, sign)		_unldi6(_jit, r0, i1, sign)
static void _unldi6(jit_state_t*,jit_int32_t,jit_word_t,jit_bool_t);
#  define unldr7(r0, r1, sign)		_unldr7(_jit, r0, r1, sign)
static void _unldr7(jit_state_t*,jit_int32_t,jit_int32_t,jit_bool_t);
#  define unldi7(r0, i1, sign)		_unldi7(_jit, r0, i1, sign)
static void _unldi7(jit_state_t*,jit_int32_t,jit_word_t,jit_bool_t);
#  define unldr8(r0, r1)		_unldr8(_jit, r0, r1)
static void _unldr8(jit_state_t*,jit_int32_t,jit_int32_t);
#  define unldi8(r0, i1)		_unldi8(_jit, r0, i1)
static void _unldi8(jit_state_t*,jit_int32_t,jit_word_t);
#endif
#define unstr2(r0, r1)			_unstr2(_jit, r0, r1)
static void _unstr2(jit_state_t*,jit_int32_t,jit_int32_t);
#define unsti2(r0, i0)			_unsti2(_jit, r0, i0)
static void _unsti2(jit_state_t*,jit_int32_t,jit_word_t);
#define unstr3(r0, r1)			_unstr3(_jit, r0, r1)
static void _unstr3(jit_state_t*,jit_int32_t,jit_int32_t);
#define unsti3(r0, i0)			_unsti3(_jit, r0, i0)
static void _unsti3(jit_state_t*,jit_int32_t,jit_word_t);
#define unstr4(r0, r1)			_unstr4(_jit, r0, r1)
static void _unstr4(jit_state_t*,jit_int32_t,jit_int32_t);
#define unsti4(r0, i0)			_unsti4(_jit, r0, i0)
static void _unsti4(jit_state_t*,jit_int32_t,jit_word_t);
#if __WORDSIZE == 64
#  define unstr5(r0, r1)		_unstr5(_jit, r0, r1)
static void _unstr5(jit_state_t*,jit_int32_t,jit_int32_t);
#  define unsti5(r0, i0)		_unsti5(_jit, r0, i0)
static void _unsti5(jit_state_t*,jit_int32_t,jit_word_t);
#  define unstr6(r0, r1)		_unstr6(_jit, r0, r1)
static void _unstr6(jit_state_t*,jit_int32_t,jit_int32_t);
#  define unsti6(r0, i0)		_unsti6(_jit, r0, i0)
static void _unsti6(jit_state_t*,jit_int32_t,jit_word_t);
#  define unstr7(r0, r1)		_unstr7(_jit, r0, r1)
static void _unstr7(jit_state_t*,jit_int32_t,jit_int32_t);
#  define unsti7(r0, i0)		_unsti7(_jit, r0, i0)
static void _unsti7(jit_state_t*,jit_int32_t,jit_word_t);
#  define unstr8(r0, r1)		_unstr8(_jit, r0, r1)
static void _unstr8(jit_state_t*,jit_int32_t,jit_int32_t);
#  define unsti8(r0, i0)		_unsti8(_jit, r0, i0)
static void _unsti8(jit_state_t*,jit_int32_t,jit_word_t);
#endif
#define unldrw(r0, r1, i0)		_unldrw(_jit, r0, r1, i0)
static void _unldrw(jit_state_t*,jit_int32_t,jit_int32_t, jit_word_t);
#define unldiw(r0, i0, i1)		_unldiw(_jit, r0, i0, i1)
static void _unldiw(jit_state_t*,jit_int32_t,jit_word_t, jit_word_t);
#define unldx(r0, i0)			_unldx(_jit, r0, i0)
static void _unldx(jit_state_t*,jit_int32_t,jit_word_t);
#define unldx_u(r0, i0)		_unldx_u(_jit, r0, i0)
static void _unldx_u(jit_state_t*,jit_int32_t,jit_word_t);
#define fallback_unldr(r0, r1, i0)	_fallback_unldr(_jit, r0, r1, i0)
static void _fallback_unldr(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_unldi(r0, i0, i1)	_fallback_unldi(_jit, r0, i0, i1)
static void _fallback_unldi(jit_state_t*,jit_int32_t,jit_word_t,jit_word_t);
#define fallback_unldr_u(r0, r1, i0)	_fallback_unldr_u(_jit, r0, r1, i0)
static void _fallback_unldr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_unldi_u(r0, i0, i1)	_fallback_unldi_u(_jit, r0, i0, i1)
static void _fallback_unldi_u(jit_state_t*,jit_int32_t,jit_word_t,jit_word_t);
#define fallback_unstr(r0, r1, i0)	_fallback_unstr(_jit, r0, r1, i0)
static void _fallback_unstr(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define fallback_unsti(i0, r0, i1)	_fallback_unsti(_jit, i0, r0, i1)
static void _fallback_unsti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#if !defined(__arm__)
#  define fallback_unldr_x(r0, r1, i0)	_fallback_unldr_x(_jit, r0, r1, i0)
static void _fallback_unldr_x(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define fallback_unldi_x(r0, i0, i1)	_fallback_unldi_x(_jit, r0, i0, i1)
static void _fallback_unldi_x(jit_state_t*,jit_int32_t,jit_word_t,jit_word_t);
#  define fallback_unstr_x(r0, r1, i0)	_fallback_unstr_x(_jit, r0, r1, i0)
static void _fallback_unstr_x(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define fallback_unsti_x(i0, r0, i1)	_fallback_unsti_x(_jit, i0, r0, i1)
static void _fallback_unsti_x(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#endif
#  if defined(__s390__) || defined(__s390x__)
#    define fallback_jit_get_reg(flags)	jit_get_reg_but_zero(flags)
#  else
#    define fallback_jit_get_reg(flags)	jit_get_reg(flags)
#  endif
#  if defined(__ia64__)
#    define fallback_flush()		sync()
#  elif defined(__mips__)
#    define fallback_flush()		flush()
#  else
#    define fallback_flush()		/**/
#  endif
#  if defined(__mips__)
#    define fallback_jmpi(i0)		jmpi(i0, 1)
#  elif defined(__arm__)
#    define fallback_jmpi(i0)		jmpi_p(i0, 1)
#  elif defined(__s390__) || defined(__s390x__)
#    define fallback_jmpi(i0)		jmpi(i0, 1)
#  else
#    define fallback_jmpi(i0)		jmpi(i0)
#  endif
#  if defined(__s390__) || defined(__s390x__)
#    define fallback_beqr(i0,r0,i1)	beqr_p(i0,r0,i1)
#    define fallback_beqi(i0,r0,i1)	beqi_p(i0,r0,i1)
#    define fallback_bner(i0,r0,r1)	bner_p(i0,r0,r1)
#    define fallback_bnei(i0,r0,i1)	bnei_p(i0,r0,i1)
#    define fallback_blei(i0,r0,i1)	blei_p(i0,r0,i1)
#    define fallback_bmsr(i0,r0,r1)	bmsr_p(i0,r0,r1)
#    define fallback_bmsi(i0,r0,i1)	bmsi_p(i0,r0,i1)
#  else
#    define fallback_beqr(i0,r0,r1)	beqr(i0,r0,r1)
#    define fallback_beqi(i0,r0,i1)	beqi(i0,r0,i1)
#    define fallback_bner(i0,r0,r1)	bner(i0,r0,r1)
#    define fallback_bnei(i0,r0,i1)	bnei(i0,r0,i1)
#    define fallback_blei(i0,r0,i1)	blei(i0,r0,i1)
#    define fallback_bmsr(i0,r0,r1)	bmsr(i0,r0,r1)
#    define fallback_bmsi(i0,r0,i1)	bmsi(i0,r0,i1)
#  endif
#  if defined(__ia64__)
#    define fallback_patch_jmpi(inst, lbl)				\
	patch_at(jit_code_jmpi, inst, lbl)
#    define fallback_patch_beqr(inst, lbl)				\
	patch_at(jit_code_beqr, inst, lbl)
#    define fallback_patch_beqi(inst, lbl)				\
	patch_at(jit_code_beqi, inst, lbl)
#    define fallback_patch_bner(inst, lbl)				\
	patch_at(jit_code_bner, inst, lbl)
#    define fallback_patch_bnei(inst, lbl)				\
	patch_at(jit_code_bnei, inst, lbl)
#    define fallback_patch_blei(inst, lbl)				\
	patch_at(jit_code_blei, inst, lbl)
#    define fallback_patch_bmsr(inst, lbl)				\
	patch_at(jit_code_bmsr, inst, lbl)
#    define fallback_patch_bmsi(inst, lbl)				\
	patch_at(jit_code_bmsi, inst, lbl)
#  elif defined(__arm__)
#    define fallback_patch_jmpi(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
#    define fallback_patch_beqr(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
#    define fallback_patch_beqi(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
#    define fallback_patch_bner(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
#    define fallback_patch_bnei(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
#    define fallback_patch_blei(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
#    define fallback_patch_bmsr(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
#    define fallback_patch_bmsi(inst, lbl)				\
	patch_at(arm_patch_jump,inst, lbl)
 #  else
#    define fallback_patch_jmpi(inst, lbl)				\
	patch_at(inst, lbl)
#    define fallback_patch_beqr(inst, lbl)				\
	patch_at(inst, lbl)
#    define fallback_patch_beqi(inst, lbl)				\
	patch_at(inst, lbl)
#    define fallback_patch_bner(inst, lbl)				\
	patch_at(inst, lbl)
#    define fallback_patch_bnei(inst, lbl)				\
	patch_at(inst, lbl)
#    define fallback_patch_blei(inst, lbl)				\
	patch_at(inst, lbl)
#    define fallback_patch_bmsr(inst, lbl)				\
	patch_at(inst, lbl)
#    define fallback_patch_bmsi(inst, lbl)				\
	patch_at(inst, lbl)
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
    calli(i0, 0);
#  elif defined(__powerpc__) && _CALL_SYSV
    calli(i0, 0);
#  elif defined(__s390__) || defined(__s390x__)
    calli(i0, 0);
#  else
    calli(i0);
#  endif
}

#ifdef NEED_FALLBACK_CASX
static void
_fallback_casx(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	       jit_int32_t r2, jit_int32_t r3, jit_word_t i0)
{
    jit_int32_t		r1_reg, iscasi;
    jit_word_t		jump, done;
    /* XXX only attempts to fallback cas for lightning jit code */
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    if ((iscasi = r1 == _NOREG)) {
	r1_reg = fallback_jit_get_reg(jit_class_gpr|jit_class_sav);
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
    fallback_flush();
    done = _jit->pc.w;
    fallback_calli((jit_word_t)pthread_mutex_unlock, (jit_word_t)&mutex);
    fallback_load(r0);
    fallback_flush();
    fallback_patch_bnei(jump, done);
    fallback_load_regs(r0);
    if (iscasi)
	jit_unget_reg(r1_reg);
}
#endif

#ifdef fallback_movr_w_f
static void
_fallback_movr_w_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (!_jitc->function->cvt_offset) {
	_jitc->function->cvt_offset = jit_allocai(8);
	_jitc->again = 1;
    }
    stxi_i(_jitc->function->cvt_offset, rn(JIT_FP), r1);
    /* Useful for special debug builds */
#  if defined(__i386__) || defined(__x86_64__)
    if (jit_cpu.sse2)
	sse_ldxi_f(r0, rn(JIT_FP), _jitc->function->cvt_offset);
    else
	x87_ldxi_f(r0, rn(JIT_FP), _jitc->function->cvt_offset);
#  else
    ldxi_f(r0, rn(JIT_FP), _jitc->function->cvt_offset);
#  endif
}
#endif

#ifdef fallback_movr_f_w
static void
_fallback_movr_f_w(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (!_jitc->function->cvt_offset) {
	_jitc->function->cvt_offset = jit_allocai(8);
	_jitc->again = 1;
    }
#  if defined(__i386__) || defined(__x86_64__)
    if (jit_cpu.sse2)
	sse_stxi_f(_jitc->function->cvt_offset, rn(JIT_FP), r1);
    else
	x87_stxi_f(_jitc->function->cvt_offset, rn(JIT_FP), r1);
#  else
    stxi_f(_jitc->function->cvt_offset, rn(JIT_FP), r1);
#  endif
    ldxi_i(r0, rn(JIT_FP), _jitc->function->cvt_offset);
}
#endif

#if __WORDSIZE == 32
#  ifdef fallback_movr_ww_d
static void
_fallback_movr_ww_d(jit_state_t *_jit,
		    jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (!_jitc->function->cvt_offset) {
	_jitc->function->cvt_offset = jit_allocai(8);
	_jitc->again = 1;
    }
    stxi_i(_jitc->function->cvt_offset, rn(JIT_FP),
	   __BYTE_ORDER == __LITTLE_ENDIAN ? r1 : r2);
    stxi_i(_jitc->function->cvt_offset + 4, rn(JIT_FP),
	   __BYTE_ORDER == __LITTLE_ENDIAN ? r2 : r1);
#    if defined(__i386__) || defined(__x86_64__)
    if (jit_cpu.sse2)
	sse_ldxi_d(r0, rn(JIT_FP), _jitc->function->cvt_offset);
    else
	x87_ldxi_d(r0, rn(JIT_FP), _jitc->function->cvt_offset);
#    else
    ldxi_d(r0, rn(JIT_FP), _jitc->function->cvt_offset);
#    endif
}
#  endif

#  ifdef fallback_movr_d_ww
static void
_fallback_movr_d_ww(jit_state_t *_jit,
		    jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (!_jitc->function->cvt_offset) {
	_jitc->function->cvt_offset = jit_allocai(8);
	_jitc->again = 1;
    }
#    if defined(__i386__) || defined(__x86_64__)
    if (jit_cpu.sse2)
	sse_stxi_d(_jitc->function->cvt_offset, rn(JIT_FP), r2);
    else
	x87_stxi_d(_jitc->function->cvt_offset, rn(JIT_FP), r2);
#    else
    stxi_d(_jitc->function->cvt_offset, rn(JIT_FP), r2);
#    endif
    ldxi_i(__BYTE_ORDER == __LITTLE_ENDIAN ? r0 : r1,
	   rn(JIT_FP), _jitc->function->cvt_offset);
    ldxi_i(__BYTE_ORDER == __LITTLE_ENDIAN ? r1 : r0,
	   rn(JIT_FP), _jitc->function->cvt_offset + 4);
}
#  endif

#else
#  ifdef fallback_movr_w_d
static void
_fallback_movr_w_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (!_jitc->function->cvt_offset) {
	_jitc->function->cvt_offset = jit_allocai(8);
	_jitc->again = 1;
    }
    stxi_l(_jitc->function->cvt_offset, rn(JIT_FP), r1);
#    if defined(__i386__) || defined(__x86_64__)
    if (jit_cpu.sse2)
	sse_ldxi_d(r0, rn(JIT_FP), _jitc->function->cvt_offset);
    else
	x87_ldxi_d(r0, rn(JIT_FP), _jitc->function->cvt_offset);
#    else
    ldxi_d(r0, rn(JIT_FP), _jitc->function->cvt_offset);
#    endif
}
#  endif

#  ifdef fallback_movr_d_w
static void
_fallback_movr_d_w(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (!_jitc->function->cvt_offset) {
	_jitc->function->cvt_offset = jit_allocai(8);
	_jitc->again = 1;
    }
#    if defined(__i386__) || defined(__x86_64__)
    if (jit_cpu.sse2)
	sse_stxi_d(_jitc->function->cvt_offset, rn(JIT_FP), r1);
    else
	x87_stxi_d(_jitc->function->cvt_offset, rn(JIT_FP), r1);
#    else
    stxi_d(_jitc->function->cvt_offset, rn(JIT_FP), r1);
#    endif
    ldxi_l(r0, rn(JIT_FP), _jitc->function->cvt_offset);
}
#  endif
#endif

static void
_fallback_clo(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		clz, done;
    comr(r0, r1);
    clz = fallback_bnei(_jit->pc.w, r0, 0);
    movi(r0, __WORDSIZE);
    fallback_flush();
    done = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(clz, _jit->pc.w);
    fallback_clz(r0, r0);
    fallback_flush();
    fallback_patch_jmpi(done, _jit->pc.w);
}

static void
_fallback_clz(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#  if USE_BIT_TABLES
    /*		t0 = __WORDSIZE - 8;
     *	loop:
     *		t1 = r1 >> t0;
     *		if (t1)
     *			goto done;
     *		t0 -= 8;
     *		if (t0)
     *			goto loop;
     *		t1 = r1;
     *	done:
     *		r0 = __WORDSIZE - 8 - t0 + clz_tab[t1]
     */
     /* Table below is count of leading zeros of 8 bit values. */
    static const jit_uint8_t clz_tab[256] = {
	8,7,6,6,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    jit_int32_t		t0, t1;
    jit_word_t		loop, done;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    t1 = fallback_jit_get_reg(jit_class_gpr);
    movi(rn(t0), __WORDSIZE - 8);
    fallback_flush();
    loop = _jit->pc.w;
    rshr_u(rn(t1), r1, rn(t0));
    done = fallback_bnei(_jit->pc.w, rn(t1), 0);
    subi(rn(t0), rn(t0), 8);
    fallback_bnei(loop, rn(t0), 0);
    movr(rn(t1), r1);
    fallback_flush();
    fallback_patch_bnei(done, _jit->pc.w);
    rsbi(r0, rn(t0), __WORDSIZE - 8);
    movi(rn(t0), (jit_word_t)clz_tab);
    ldxr_uc(rn(t1), rn(t0), rn(t1));
    addr(r0, r0, rn(t1));
    jit_unget_reg(t1);
    jit_unget_reg(t0);
#  else
    jit_int32_t		r1_reg, r2, r2_reg;
    jit_word_t		clz, l32, l16, l8, l4, l2, l1;
    l32 = fallback_bnei(_jit->pc.w, r1, 0);
    movi(r0, __WORDSIZE);
    clz = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(l32, _jit->pc.w);
    r2_reg = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(r2_reg);
    r1_reg = fallback_jit_get_reg(jit_class_gpr);
    movr(rn(r1_reg), r1);
    r1 = rn(r1_reg);
    movi(r0, 0);
#    if __WORDSIZE == 64
    movi(r2, 0xffffffff00000000UL);
    l32 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 32);
    addi(r0, r0, 32);
    fallback_flush();
    fallback_patch_bmsr(l32, _jit->pc.w);
    lshi(r2, r2, 16);
#    else
    movi(r2, 0xffff0000UL);
#    endif
    l16 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 16);
    addi(r0, r0, 16);
    fallback_flush();
    fallback_patch_bmsr(l16, _jit->pc.w);
    lshi(r2, r2, 8);
    l8 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 8);
    addi(r0, r0, 8);
    fallback_flush();
    fallback_patch_bmsr(l8, _jit->pc.w);
    lshi(r2, r2, 4);
    l4 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 4);
    addi(r0, r0, 4);
    fallback_flush();
    fallback_patch_bmsr(l4, _jit->pc.w);
    lshi(r2, r2, 2);
    l2 = fallback_bmsr(_jit->pc.w, r1, r2);
    lshi(r1, r1, 2);
    addi(r0, r0, 2);
    fallback_flush();
    fallback_patch_bmsr(l2, _jit->pc.w);
    lshi(r2, r2, 1);
    l1 = fallback_bmsr(_jit->pc.w, r1, r2);
    addi(r0, r0, 1);
    fallback_flush();
    fallback_patch_bmsr(l1, _jit->pc.w);
    fallback_patch_jmpi(clz, _jit->pc.w);
    jit_unget_reg(r2_reg);
    jit_unget_reg(r1_reg);
#  endif
}

static void
_fallback_cto(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		ctz, done;
    comr(r0, r1);
    ctz = fallback_bnei(_jit->pc.w, r0, 0);
    movi(r0, __WORDSIZE);
    done = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(ctz, _jit->pc.w);
    fallback_ctz(r0, r0);
    fallback_flush();
    fallback_patch_jmpi(done, _jit->pc.w);
}

static void
_fallback_ctz(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#  if USE_BIT_TABLES
    /* Adapted from http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightModLookup
     * Table for 64 bits was recomputed choosing 67 as next prime number.
     * The cost of the modulo might not compensate and could be better to
     * use the alternate version (or rbit and use clz).
     */
    jit_int32_t		t0;
#    if __WORDSIZE == 32
    static const jit_uint8_t mod37[] = {
	32,  0,  1, 26,  2, 23, 27,  0,  3, 16, 24, 30, 28, 11,  0, 13,
	 4,  7, 17,  0, 25, 22, 31, 15, 29, 10, 12,  6,  0, 21, 14,  9,
	 5, 20, 8, 19, 18
    };
    /* return mod37[(-r1 & r1) % 37]; */
#    else
    static const jit_uint8_t mod67[] = {
	64,  0,  1, 39,  2, 15, 40, 23,  3, 12, 16, 59, 41, 19, 24, 54,
         4,  0, 13, 10, 17, 62, 60, 28, 42, 30, 20, 51, 25, 44, 55, 47,
         5, 32,  0, 38, 14, 22, 11, 58, 18, 53, 63,  9, 61, 27, 29, 50,
        43, 46, 31, 37, 21, 57, 52,  8, 26, 49, 45, 36, 56,  7, 48, 35,
         6, 34, 33
    };
    /* return mod67[(-r1 & r1) % 67]; */
#    endif
    t0 = fallback_jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	negr(rn(t0), r1);
	andr(r0, rn(t0), r1);
    }
    else {
	negr(r0, r1);
	andr(r0, r0, r1);
    }
#    if __WORDSIZE == 32
    remi_u(r0, r0, 37);
    movi(rn(t0), (jit_word_t)mod37);
#    else
    remi_u(r0, r0, 67);
    movi(rn(t0), (jit_word_t)mod67);
#    endif
    ldxr_uc(r0, rn(t0), r0);
    jit_unget_reg(t0);
#  else
    jit_int32_t		r1_reg, r2, r2_reg;
    jit_word_t		ctz, l32, l16, l8, l4, l2, l1;
    l32 = fallback_bnei(_jit->pc.w, r1, 0);
    movi(r0, __WORDSIZE);
    ctz = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(l32, _jit->pc.w);
    r2_reg = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(r2_reg);
    r1_reg = fallback_jit_get_reg(jit_class_gpr);
    movr(rn(r1_reg), r1);
    r1 = rn(r1_reg);
    movi(r0, 0);
#    if __WORDSIZE == 64
    movi(r2, 0xffffffffUL);
    l32 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 32);
    addi(r0, r0, 32);
    fallback_flush();
    fallback_patch_bmsr(l32, _jit->pc.w);
    rshi(r2, r2, 16);
#    else
    movi(r2, 0xffffUL);
#    endif
    l16 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 16);
    addi(r0, r0, 16);
    fallback_flush();
    fallback_patch_bmsr(l16, _jit->pc.w);
    rshi(r2, r2, 8);
    l8 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 8);
    addi(r0, r0, 8);
    fallback_flush();
    fallback_patch_bmsr(l8, _jit->pc.w);
    rshi(r2, r2, 4);
    l4 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 4);
    addi(r0, r0, 4);
    fallback_flush();
    fallback_patch_bmsr(l4, _jit->pc.w);
    rshi(r2, r2, 2);
    l2 = fallback_bmsr(_jit->pc.w, r1, r2);
    rshi_u(r1, r1, 2);
    addi(r0, r0, 2);
    fallback_flush();
    fallback_patch_bmsr(l2, _jit->pc.w);
    rshi(r2, r2, 1);
    l1 = fallback_bmsr(_jit->pc.w, r1, r2);
    addi(r0, r0, 1);
    fallback_flush();
    fallback_patch_bmsr(l1, _jit->pc.w);
    fallback_patch_jmpi(ctz, _jit->pc.w);
    jit_unget_reg(r2_reg);
    jit_unget_reg(r1_reg);
#  endif
}

static void
_fallback_rbit(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#  if USE_BIT_TABLES
    /*		t0 = r1;
     *		t1 = t0 & 0xff;
     *		t2 = swap_tab;
     *		r0 = t2[t1];
     *		t3 = 8;
     *	loop:
     *		t1 = t0 >> t3;
     *		t1 &= 0xff;
     *		r0 <<= 8;
     *		r0 |= t2[t1];
     *		t3 += 8;
     *		if (t3 < __WORDSIZE)
     *		    goto loop;
     */
    jit_word_t		loop;
    jit_int32_t		t0, r1_reg, t1, t2, t3;
    static const unsigned char swap_tab[256] = {
	 0, 128, 64, 192, 32, 160,  96, 224,
	16, 144, 80, 208, 48, 176, 112, 240,
	 8, 136, 72, 200, 40, 168, 104, 232,
	24, 152, 88, 216 ,56, 184, 120, 248,
	 4, 132, 68, 196, 36, 164, 100, 228,
	20, 148, 84, 212, 52, 180, 116, 244,
	12, 140, 76, 204, 44, 172, 108, 236,
	28, 156, 92, 220, 60, 188, 124, 252,
	 2, 130, 66, 194, 34, 162,  98, 226,
	18, 146, 82, 210, 50, 178, 114, 242,
	10, 138, 74, 202, 42, 170, 106, 234,
	26, 154, 90, 218, 58, 186, 122, 250,
	 6, 134, 70, 198, 38, 166, 102, 230,
	22, 150, 86, 214, 54, 182, 118, 246,
	14, 142, 78, 206, 46, 174, 110, 238,
	30, 158, 94, 222, 62, 190, 126, 254,
	 1, 129, 65, 193, 33, 161,  97, 225,
	17, 145, 81, 209, 49, 177, 113, 241,
	 9, 137, 73, 201, 41, 169, 105, 233,
	25, 153, 89, 217, 57, 185, 121, 249,
	 5, 133, 69, 197, 37, 165, 101, 229,
	21, 149, 85, 213, 53, 181, 117, 245,
	13, 141, 77, 205, 45, 173, 109, 237,
	29, 157, 93, 221, 61, 189, 125, 253,
	 3, 131, 67, 195, 35, 163,  99, 227,
	19, 147, 83, 211, 51, 179, 115, 243,
	11, 139, 75, 203, 43, 171, 107, 235,
	27, 155, 91, 219, 59, 187, 123, 251,
	 7, 135, 71, 199, 39, 167, 103, 231,
	23, 151, 87, 215, 55, 183, 119, 247,
	15, 143, 79, 207, 47, 175, 111, 239,
	31, 159, 95, 223, 63, 191, 127, 255
    };
    if (r0 == r1) {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r1_reg = rn(t0);
    }
    else {
	t0 = JIT_NOREG;
	r1_reg = r1;
    }
    t1 = fallback_jit_get_reg(jit_class_gpr);
    t2 = fallback_jit_get_reg(jit_class_gpr);
    t3 = fallback_jit_get_reg(jit_class_gpr);
    if (r0 == r1)
	movr(rn(t0), r1);
    extr_uc(rn(t1), r1_reg);
    movi(rn(t2), (jit_word_t)swap_tab);
    ldxr_uc(r0, rn(t2), rn(t1));
    movi(rn(t3), 8);
    fallback_flush();
    loop = _jit->pc.w;
    rshr(rn(t1), r1_reg, rn(t3));
    extr_uc(rn(t1), rn(t1));
    lshi(r0, r0, 8);
    ldxr_uc(rn(t1), rn(t2), rn(t1));
    orr(r0, r0, rn(t1));
    addi(rn(t3), rn(t3), 8);
    blti(loop, rn(t3), __WORDSIZE);
    jit_unget_reg(t3);
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    if (t0 != JIT_NOREG)
	jit_unget_reg(t0);
#  elif USE_BITSWAP_UNROLLED
/* http://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel */
/*
unsigned int v; // 32-bit word to reverse bit order

// swap odd and even bits
v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
// swap consecutive pairs
v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
// swap nibbles ...
v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
// swap bytes
v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
// swap 2-byte long pairs
v = ( v >> 16             ) | ( v               << 16);
 */
    jit_int32_t		t0, t1, t2, t3, t4;
    movr(r0, r1);
    t0 = fallback_jit_get_reg(jit_class_gpr);
    t1 = fallback_jit_get_reg(jit_class_gpr);
    t2 = fallback_jit_get_reg(jit_class_gpr);
    movi(rn(t0), __WORDSIZE == 32 ? 0x55555555L : 0x5555555555555555L);
    rshi_u(rn(t1), r0, 1);		/* t1 = v >> 1 */
    andr(rn(t1), rn(t1), rn(t0));	/* t1 &= t0 */
    andr(rn(t2), r0, rn(t0));		/* t2 = v & t0*/
    lshi(rn(t2), rn(t2), 1);		/* t2 <<= 1 */
    orr(r0, rn(t1), rn(t2));		/* v = t1 | t2 */
    movi(rn(t0), __WORDSIZE == 32 ? 0x33333333L : 0x3333333333333333L);
    rshi_u(rn(t1), r0, 2);		/* t1 = v >> 2 */
    andr(rn(t1), rn(t1), rn(t0));	/* t1 &= t0 */
    andr(rn(t2), r0, rn(t0));		/* t2 = v & t0*/
    lshi(rn(t2), rn(t2), 2);		/* t2 <<= 2 */
    orr(r0, rn(t1), rn(t2));		/* v = t1 | t2 */
    movi(rn(t0), __WORDSIZE == 32 ? 0x0f0f0f0fL : 0x0f0f0f0f0f0f0f0fL);
    rshi_u(rn(t1), r0, 4);		/* t1 = v >> 4 */
    andr(rn(t1), rn(t1), rn(t0));	/* t1 &= t0 */
    andr(rn(t2), r0, rn(t0));		/* t2 = v & t0*/
    lshi(rn(t2), rn(t2), 4);		/* t2 <<= 4 */
    orr(r0, rn(t1), rn(t2));		/* v = t1 | t2 */
    movi(rn(t0), __WORDSIZE == 32 ?  0x00ff00ffL : 0x00ff00ff00ff00ffL);
    rshi_u(rn(t1), r0, 8);		/* t1 = v >> 8 */
    andr(rn(t1), rn(t1), rn(t0));	/* t1 &= t0 */
    andr(rn(t2), r0, rn(t0));		/* t2 = v & t0*/
    lshi(rn(t2), rn(t2), 8);		/* t2 <<= 8 */
    orr(r0, rn(t1), rn(t2));		/* v = t1 | t2 */
#    if __WORDSIZE == 32
    rshi_u(rn(t1), r0, 16);		/* t1 = v >> 16 */
    lshi(rn(t2), r0, 16);		/* t2 = v << 16 */
    orr(r0, rn(t1), rn(t2));		/* v = t1 | t2 */
#    else
    movi(rn(t0), 0x0000ffff0000ffffL);
    rshi_u(rn(t1), r0, 16);		/* t1 = v >> 16 */
    andr(rn(t1), rn(t1), rn(t0));	/* t1 &= t0 */
    andr(rn(t2), r0, rn(t0));		/* t2 = v & t0*/
    lshi(rn(t2), rn(t2), 16);		/* t2 <<= 16 */
    orr(r0, rn(t1), rn(t2));		/* v = t1 | t2 */
    rshi_u(rn(t1), r0, 32);		/* t1 = v >> 32 */
    lshi(rn(t2), r0, 32);		/* t2 = v << 32 */
    orr(r0, rn(t1), rn(t2));		/* v = t1 | t2 */
#    endif
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
#  elif USE_BITSWAP_LOOP
/* http://graphics.stanford.edu/~seander/bithacks.html#ReverseParallel */
/*
unsigned int s = sizeof(v) * CHAR_BIT; // bit size; must be power of 2
unsigned int mask = ~0;
while ((s >>= 1) > 0)
{
  mask ^= (mask << s);
  v = ((v >> s) & mask) | ((v << s) & ~mask);
}
*/
    jit_int32_t		s, mask;
    jit_word_t		loop, done, t0, t1;
    movr(v, r1);
    s = fallback_jit_get_reg(jit_class_gpr);
    movi(rn(s), __WORDSIZE);			/* s = sizeof(v) * CHAR_BIT; */
    mask = fallback_jit_get_reg(jit_class_gpr);
    movi(rn(mask), ~0L);			/* mask = ~0; */
    flush();
    loop = _jit->pc.w;				/* while ((s >>= 1) > 0) */
    rshi(rn(s), rn(s), 1);			/*        (s >>= 1) */
    done = blei(_jit->pc.w, rn(s), 0);		/* no loop if s <= 0 */
    t0 = fallback_jit_get_reg(jit_class_gpr);
    lshr(rn(t0), rn(mask), rn(s));		/* t0 = (mask << s) */
    xorr(rn(mask), rn(mask), rn(t0));		/* mask ^= t0 */
    rshr(rn(t0), v, rn(s));			/* t0 = v >> s */
    andr(rn(t0), rn(t0), rn(mask));		/* t0 = t0 & mask */
    t1 = fallback_jit_get_reg(jit_class_gpr);
    lshr(rn(t1), v, rn(s));			/* t1 = v << s */
    comr(v, rn(mask));				/* v = ~mask */
    andr(rn(t1), v, rn(t1));			/* t1 = t1 & v */
    orr(v, rn(t0), rn(t1));			/* v = t0 | t1 */
    jmpi(loop);
    flush();
    patch_at(done, _jit->pc.w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    jit_unget_reg(mask);
    jit_unget_reg(s);
#  endif
}

static void
_fallback_popcnt(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* Same approach as rbitr */
    /*		t0 = r1;
     *		t1 = t0 & 0xff;
     *		t2 = pop_tab;
     *		r0 = t2[t1];
     *		t3 = 8;
     *	loop:
     *		t1 = t0 >> t3;
     *		t1 &= 0xff;
     *		r0 <<= 8;
     *		r0 |= t2[t1];
     *		t3 += 8;
     *		if (t3 < __WORDSIZE)
     *		    goto loop;
     */
    jit_word_t		loop;
    jit_int32_t		t0, r1_reg, t1, t2, t3;
    static const unsigned char pop_tab[256] = {
	0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
	1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
	1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
	2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
	3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
    };
    if (r0 == r1) {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r1_reg = rn(t0);
    }
    else {
	t0 = JIT_NOREG;
	r1_reg = r1;
    }
    t1 = fallback_jit_get_reg(jit_class_gpr);
    t2 = fallback_jit_get_reg(jit_class_gpr);
    t3 = fallback_jit_get_reg(jit_class_gpr);
    if (r0 == r1)
	movr(rn(t0), r1);
    extr_uc(rn(t1), r1_reg);
    movi(rn(t2), (jit_word_t)pop_tab);
    ldxr_uc(r0, rn(t2), rn(t1));
    movi(rn(t3), 8);
    fallback_flush();
    loop = _jit->pc.w;
    rshr(rn(t1), r1_reg, rn(t3));
    extr_uc(rn(t1), rn(t1));
    ldxr_uc(rn(t1), rn(t2), rn(t1));
    addr(r0, r0, rn(t1));
    addi(rn(t3), rn(t3), 8);
    blti(loop, rn(t3), __WORDSIZE);
    jit_unget_reg(t3);
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    if (t0 != JIT_NOREG)
	jit_unget_reg(t0);
}

static void
_fallback_lrotr(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    /* r0 = (r1 << r2) | (r1 >> (__WORDSIZE - r2)) */
    jit_int32_t		t0, t1;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    if (r0 == r1 || r0 == r2) {
	t1 = fallback_jit_get_reg(jit_class_gpr);
	lshr(rn(t0), r1, r2);
	rsbi(rn(t1), r2, __WORDSIZE);
	rshr_u(rn(t1), r1, rn(t1));
	orr(r0, rn(t0), rn(t1));
	jit_unget_reg(t1);
    }
    else {
	lshr(r0, r1, r2);
	rsbi(rn(t0), r2, __WORDSIZE);
	rshr_u(rn(t0), r1, rn(t0));
	orr(r0, r0, rn(t0));
    }
    jit_unget_reg(t0);
}

static void
_fallback_lroti(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    lshi(rn(t0), r1, i0);
    rshi_u(r0, r1, __WORDSIZE - i0);
    orr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_fallback_rrotr(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    /* r0 = (r1 >> r2) | (r1 << (__WORDSIZE - r2)) */
    jit_int32_t		t0, t1;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    if (r0 == r1 || r0 == r2) {
	t1 = fallback_jit_get_reg(jit_class_gpr);
	rshr_u(rn(t0), r1, r2);
	rsbi(rn(t1), r2, __WORDSIZE);
	lshr(rn(t1), r1, rn(t1));
	orr(r0, rn(t0), rn(t1));
	jit_unget_reg(t1);
    }
    else {
	rshr_u(r0, r1, r2);
	rsbi(rn(t0), r2, __WORDSIZE);
	lshr(rn(t0), r1, rn(t0));
	orr(r0, r0, rn(t0));
    }
    jit_unget_reg(t0);
}

static void
_fallback_rroti(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    rshi_u(rn(t0), r1, i0);
    lshi(r0, r1, __WORDSIZE - i0);
    orr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_fallback_ext(jit_state_t *_jit,
	      jit_int32_t r0, jit_int32_t r1, jit_word_t i0, jit_word_t i1)
{
    assert(i0 >= 0 && i1 >= 1 && i0 + i1 <= __WORDSIZE);
    if (i1 == __WORDSIZE)
	movr(r0, r1);
    else {
#  if __BYTE_ORDER == __BIG_ENDIAN
	i0 = __WORDSIZE - (i0 + i1);
#  endif
	if (__WORDSIZE - (i0 + i1)) {
	    lshi(r0, r1, __WORDSIZE - (i0 + i1));
	    rshi(r0, r0, __WORDSIZE - i1);
	}
	else
	    rshi(r0, r1, __WORDSIZE - i1);
    }
}

static void
_fallback_ext_u(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_word_t i0, jit_word_t i1)
{
    assert(i0 >= 0 && i1 >= 1 && i0 + i1 <= __WORDSIZE);
    if (i1 == __WORDSIZE)
	movr(r0, r1);
    else {
#  if __BYTE_ORDER == __BIG_ENDIAN
	i0 = __WORDSIZE - (i0 + i1);
#  endif
	if (i0)
	    rshi_u(r0, r1, i0);
	andi(r0, r0, (1L << i1) - 1);
    }
}

static void
_fallback_dep(jit_state_t *_jit,
	      jit_int32_t r0, jit_int32_t r1, jit_word_t i0, jit_word_t i1)
{
    jit_int32_t		t0;
    jit_word_t		mask;
    assert(i0 >= 0 && i1 >= 1 && i0 + i1 <= __WORDSIZE);
    if (i1 == __WORDSIZE)
	movr(r0, r1);
    else {
#  if __BYTE_ORDER == __BIG_ENDIAN
	i0 = __WORDSIZE - (i0 + i1);
#  endif
	mask = (1L << i1) - 1;
	t0 = fallback_jit_get_reg(jit_class_gpr);
	andi(rn(t0), r1, mask);
	if (i0) {
	    lshi(rn(t0), rn(t0), i0);
	    mask <<= i0;
	}
	andi(r0, r0, ~mask);
	orr(r0, r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_fallback_qlshr(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    /*		r1 = __WORDSIZE - r3;
     *		if (r1 != __WORDSIZE) {
     *			r0 = r2 << r3;
     *			if (r3 != 0)
     *				r1 = r2 >> r1;
     *			else
     *				r1 = 0;
     *		}
     *		else {
     *			r1 = r2;
     *			r0 = 0;
     *		}
     */
    jit_int32_t		t0, s0, t2, s2, t3, s3;
    jit_word_t		over, zero, done, done_over;
    s0 = fallback_jit_get_reg(jit_class_gpr);
    t0 = rn(s0);
    if (r0 == r2 || r1 == r2) {
	s2 = fallback_jit_get_reg(jit_class_gpr);
	t2 = rn(s2);
	movr(t2, r2);
    }
    else
	t2 = r2;
    if (r0 == r3 || r1 == r3) {
	s3 = fallback_jit_get_reg(jit_class_gpr);
	t3 = rn(s3);
	movr(t3, r3);
    }
    else
	t3 = r3;
    rsbi(t0, t3, __WORDSIZE);
    lshr(r0, t2, t3);
    rshr(r1, t2, t0);
    zero = fallback_beqi(_jit->pc.w, t3, 0);
    over = fallback_beqi(_jit->pc.w, t3, __WORDSIZE);
    done = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_jmpi(over, _jit->pc.w);
    /* overflow */
    movi(r0, 0);
    done_over = fallback_jmpi(_jit->pc.w);
    /* zero */
    fallback_flush();
    fallback_patch_beqi(zero, _jit->pc.w);
    rshi(r1, t2, __WORDSIZE - 1);
    fallback_flush();
    fallback_patch_jmpi(done, _jit->pc.w);
    fallback_patch_jmpi(done_over, _jit->pc.w);
    jit_unget_reg(s0);
    if (t2 != r2)
	jit_unget_reg(s2);
    if (t3 != r3)
	jit_unget_reg(s3);
}

static void
_fallback_qlshi(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    assert((jit_uword_t)i0 <= __WORDSIZE);
    if (i0 == 0) {
	if (r0 != r2) {
	    movr(r0, r2);
	    rshi(r1, r2, __WORDSIZE - 1);
	}
	else
	    rshi(r1, r2, __WORDSIZE - 1);
    }
    else if (i0 != __WORDSIZE) {
	rshi(r1, r2, __WORDSIZE - i0);
	lshi(r0, r2, i0);
    }
    else {
	movr(r1, r2);
	movi(r0, 0);
    }
}

static void
_fallback_qlshr_u(jit_state_t *_jit, jit_int32_t r0,
		  jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    /*		r1 = __WORDSIZE - r3;
     *		if (r1 != __WORDSIZE) {
     *			r0 = r2 << r3;
     *			if (r3 != 0)
     *				r1 = (unsigned)r2 >> r1;
     *			else
     *				r1 = 0;
     *		}
     *		else {
     *			r1 = r2;
     *			r0 = 0;
     *		}
     */
    jit_int32_t		t0, s0, t2, s2, t3, s3;
    jit_word_t		over, zero, done, done_over;
    s0 = fallback_jit_get_reg(jit_class_gpr);
    t0 = rn(s0);
    if (r0 == r2 || r1 == r2) {
	s2 = fallback_jit_get_reg(jit_class_gpr);
	t2 = rn(s2);
	movr(t2, r2);
    }
    else
	t2 = r2;
    if (r0 == r3 || r1 == r3) {
	s3 = fallback_jit_get_reg(jit_class_gpr);
	t3 = rn(s3);
	movr(t3, r3);
    }
    else
	t3 = r3;
    rsbi(t0, t3, __WORDSIZE);
    lshr(r0, t2, t3);
    rshr_u(r1, t2, t0);
    zero = fallback_beqi(_jit->pc.w, t3, 0);
    over = fallback_beqi(_jit->pc.w, t3, __WORDSIZE);
    done = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_jmpi(over, _jit->pc.w);
    /* overflow */
    movi(r0, 0);
    done_over = fallback_jmpi(_jit->pc.w);
    /* zero */
    fallback_flush();
    fallback_patch_beqi(zero, _jit->pc.w);
    movi(r1, 0);
    fallback_flush();
    fallback_patch_jmpi(done, _jit->pc.w);
    fallback_patch_jmpi(done_over, _jit->pc.w);
    jit_unget_reg(s0);
    if (t2 != r2)
	jit_unget_reg(s2);
    if (t3 != r3)
	jit_unget_reg(s3);
}

static void
_fallback_qlshi_u(jit_state_t *_jit, jit_int32_t r0,
		  jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    assert(i0 <= __WORDSIZE);
    if (i0 == 0) {
	movr(r0, r2);
	movi(r1, 0);
    }
    else if (i0 != __WORDSIZE) {
	rshi_u(r1, r2, __WORDSIZE - i0);
	lshi(r0, r2, i0);
    }
    else {
	movr(r1, r2);
	movi(r0, 0);
    }
}

static void
_fallback_qrshr(jit_state_t *_jit, jit_int32_t r0,
		jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    /*		r1 = __WORDSIZE - r3;
     *		if (r1 != __WORDSIZE) {
     *			r0 = r2 >> r3;
     *			if (r3 != 0)
     *				r1 = r2 << r1;
     *			else
     *				r1 = r2 >> (__WORDSIZE - 1);
     *		}
     *		else {
     *			r1 = r2;
     *			r0 = r2 >> (__WORDSIZE - 1);
     *		}
     */
    jit_int32_t		t0, s0, t2, s2, t3, s3;
    jit_word_t		over, zero, done, done_over;
    s0 = fallback_jit_get_reg(jit_class_gpr);
    t0 = rn(s0);
    if (r0 == r2 || r1 == r2) {
	s2 = fallback_jit_get_reg(jit_class_gpr);
	t2 = rn(s2);
	movr(t2, r2);
    }
    else
	t2 = r2;
    if (r0 == r3 || r1 == r3) {
	s3 = fallback_jit_get_reg(jit_class_gpr);
	t3 = rn(s3);
	movr(t3, r3);
    }
    else
	t3 = r3;
    rsbi(t0, t3, __WORDSIZE);
    rshr(r0, t2, t3);
    lshr(r1, t2, t0);
    zero = fallback_beqi(_jit->pc.w, t3, 0);
    over = fallback_beqi(_jit->pc.w, t3, __WORDSIZE);
    done = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_jmpi(over, _jit->pc.w);
    /* underflow */
    rshi(r0, t2, __WORDSIZE - 1);
    done_over = fallback_jmpi(_jit->pc.w);
    /* zero */
    fallback_flush();
    fallback_patch_beqi(zero, _jit->pc.w);
    rshi(r1, t2, __WORDSIZE - 1);
    fallback_flush();
    fallback_patch_jmpi(done, _jit->pc.w);
    fallback_patch_jmpi(done_over, _jit->pc.w);
    jit_unget_reg(s0);
    if (t2 != r2)
	jit_unget_reg(s2);
    if (t3 != r3)
	jit_unget_reg(s3);
}

static void
_fallback_qrshi(jit_state_t *_jit, jit_int32_t r0,
		jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    assert((jit_uword_t)i0 <= __WORDSIZE);
    if (i0 == 0) {
	if (r0 != r2) {
	    movr(r0, r2);
	    rshi(r1, r2, __WORDSIZE - 1);
	}
	else
	    rshi(r1, r2, __WORDSIZE - 1);
    }
    else if (i0 != __WORDSIZE) {
	lshi(r1, r2, __WORDSIZE - i0);
	rshi(r0, r2, i0);
    }
    else {
	movr(r1, r2);
	rshi(r0, r2, __WORDSIZE - 1);
   }
}

static void
_fallback_qrshr_u(jit_state_t *_jit, jit_int32_t r0,
		  jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    /*		r1 = __WORDSIZE - r3;
     *		if (r1 != __WORDSIZE) {
     *			r0 = (unsigned)r2 >> r3;
     *			if (r3 != 0)
     *				r1 = r2 << r1;
     *			else
     *				r1 = 0;
     *		}
     *		else {
     *			r1 = r2;
     *			r0 = 0;
     *		}
     */
    jit_int32_t		t0, s0, t2, s2, t3, s3;
    jit_word_t		over, zero, done, done_over;
    s0 = fallback_jit_get_reg(jit_class_gpr);
    t0 = rn(s0);
    if (r0 == r2 || r1 == r2) {
	s2 = fallback_jit_get_reg(jit_class_gpr);
	t2 = rn(s2);
	movr(t2, r2);
    }
    else
	t2 = r2;
    if (r0 == r3 || r1 == r3) {
	s3 = fallback_jit_get_reg(jit_class_gpr);
	t3 = rn(s3);
	movr(t3, r3);
    }
    else
	t3 = r3;
    rsbi(t0, t3, __WORDSIZE);
    rshr_u(r0, t2, t3);
    lshr(r1, t2, t0);
    zero = fallback_beqi(_jit->pc.w, t3, 0);
    over = fallback_beqi(_jit->pc.w, t3, __WORDSIZE);
    done = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_jmpi(over, _jit->pc.w);
    /* underflow */
    movi(r0, 0);
    done_over = fallback_jmpi(_jit->pc.w);
    /* zero */
    fallback_flush();
    fallback_patch_beqi(zero, _jit->pc.w);
    movi(r1, 0);
    fallback_flush();
    fallback_patch_jmpi(done, _jit->pc.w);
    fallback_patch_jmpi(done_over, _jit->pc.w);
    jit_unget_reg(s0);
    if (t2 != r2)
	jit_unget_reg(s2);
    if (t3 != r3)
	jit_unget_reg(s3);
}

static void
_fallback_qrshi_u(jit_state_t *_jit, jit_int32_t r0,
		  jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    assert((jit_uword_t)i0 <= __WORDSIZE);
    if (i0 == 0) {
	movr(r0, r2);
	movi(r1, 0);
    }
    else if (i0 != __WORDSIZE) {
	lshi(r1, r2, __WORDSIZE - i0);
	rshi_u(r0, r2, i0);
    }
    else {
	movr(r1, r2);
	movi(r0, 0);
    }
}

static void
_unldr2(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    jit_word_t		un2, al;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r1, r2);
    if (sign)
	ldr_s(r0, r1);
    else
	ldr_us(r0, r1);
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    if (sign)
	ldxi_c(r2, r1, 1);
    else
	ldxi_uc(r2, r1, 1);
    lshi(r2, r2, 8);
#  else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 8);
    ldxi_uc(r2, r1, 1);
#  endif
    orr(r0, r0, r2);
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unldi2(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    if ((i1 & -2) == i1) {
	if (sign)
	    ldi_s(r0, i1);
	else
	    ldi_us(r0, i1);
    }
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	if (sign)
	    ldi_c(r2, i1 + 1);
	else
	    ldi_uc(r2, i1 + 1);
	lshi(r2, r2, 8);
#  else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 8);
	ldi_uc(r2, i1 + 1);
#  endif
	orr(r0, r0, r2);
	jit_unget_reg(t0);
    }
}

static void
_unldr3(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    jit_word_t		un2, or;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r1, r2);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_us(r0, r1);
    if (sign)
	ldxi_c(r2, r1, 2);
    else
	ldxi_uc(r2, r1, 2);
    lshi(r2, r2, 16);
#  else
    if (sign)
	ldr_s(r0, r1);
    else
	ldr_us(r0, r1);
    lshi(r0, r0, 8);
    ldxi_uc(r2, r1, 2);
#  endif
    or = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    if (sign)
	ldxi_s(r2, r1, 1);
    else
	ldxi_us(r2, r1, 1);
    lshi(r2, r2, 8);
#  else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 16);
    ldxi_us(r2, r1, 1);
#  endif
    fallback_flush();
    fallback_patch_jmpi(or, _jit->pc.w);
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldi3(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & -2) == i1) {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_us(r0, i1);
	if (sign)
	    ldi_c(r2, i1 + 2);
	else
	    ldi_uc(r2, i1 + 2);
	lshi(r2, r2, 16);
#  else
	if (sign)
	    ldi_s(r0, i1);
	else
	    ldi_us(r0, i1);
	lshi(r0, r0, 8);
	ldi_uc(r2, i1 + 2);
#  endif
    }
    else {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	if (sign)
	    ldi_s(r2, i1 + 1);
	else
	    ldi_us(r2, i1 + 1);
	lshi(r2, r2, 8);
#  else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 16);
	ldi_us(r2, i1 + 1);
#  endif
    }
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldr4(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1
#  if __WORDSIZE == 64
       , jit_bool_t sign
#  endif
       )
{
    jit_int32_t		t0, r2;
    jit_word_t		un4, un2, un3, al, or, or3;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -4);
    un4 = fallback_bner(_jit->pc.w, r1, r2);
#  if __WORDSIZE == 64
    if (sign)
#  endif
	ldr_i(r0, r1);
#  if __WORDSIZE == 64
    else
	ldr_ui(r0, r1);
#  endif
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un4, _jit->pc.w);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r1, r2);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_us(r0, r1);
#    if __WORDSIZE == 64
    if (sign)
	ldxi_s(r2, r1, 2);
    else
#    endif
	ldxi_us(r2, r1, 2);
    lshi(r2, r2, 16);
#  else
#    if __WORDSIZE == 64
    if (sign)
	ldr_s(r0, r1);
    else
#    endif
	ldr_us(r0, r1);
    lshi(r0, r0, 16);
    ldxi_us(r2, r1, 2);
#  endif
    or = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_i(r2, r1, 1);
    lshi(r2, r2, 8);
#    if __WORDSIZE == 64
    if (sign)
	extr_i(r2, r2);
    else
	extr_ui(r2, r2);
#    endif
#  else
#    if __WORDSIZE == 64
    if (sign)
	ldr_c(r0, r1);
    else
#    endif
	ldr_uc(r0, r1);
    lshi(r0, r0, 24);
#    if __WORDSIZE == 32
    ldxi(r2, r1, 1);
#    else
    ldxi_ui(r2, r1, 1);
#    endif
    rshi_u(r2, r2, 8);
#  endif
    or3 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#    if __WORDSIZE == 32
    ldxi(r0, r1, -1);
#    else
    ldxi_ui(r0, r1, -1);
#    endif
    rshi_u(r0, r0, 8);
#    if __WORDSIZE == 64
    if (sign)
	ldxi_c(r2, r1, 3);
    else
#    endif
	ldxi_uc(r2, r1, 3);
    lshi(r2, r2, 24);
#  else
    ldxi_i(r0, r1, -1);
    lshi(r0, r0, 8);
#    if __WORDSIZE == 64
    if (sign)
	extr_i(r0, r0);
    else
	extr_ui(r0, r0);
#    endif
    ldxi_uc(r2, r1, 3);
#  endif
    fallback_flush();
    fallback_patch_jmpi(or, _jit->pc.w);
    fallback_patch_jmpi(or3, _jit->pc.w);
    orr(r0, r0, r2);
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unldi4(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1
#  if __WORDSIZE == 64
       , jit_bool_t sign
#  endif
       )
{
    jit_int32_t		t0, r2;
    if ((i1 & -4) == i1) {
#  if __WORDSIZE == 64
	if (sign)
#  endif
	    ldi_i(r0, i1);
#  if __WORDSIZE == 64
	else
	    ldi_ui(r0, i1);
#  endif
    }
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
	if ((i1 & -2) == i1) {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_us(r0, i1);
#    if __WORDSIZE == 64
	    if (sign)
		ldi_s(r2, i1 + 2);
	    else
#    endif
		ldi_us(r2, i1 + 2);
	    lshi(r2, r2, 16);
#  else
#    if __WORDSIZE == 64
	    if (sign)
		ldi_s(r0, i1);
	    else
#  endif
		ldi_us(r0, i1);
	    lshi(r0, r0, 16);
	    ldi_us(r2, i1 + 2);
#  endif
	}
	else if ((i1 & 3) == 3) {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_uc(r0, i1);
	    ldi_i(r2, i1 + 1);
	    lshi(r2, r2, 8);
#    if __WORDSIZE == 64
	    if (sign)
		extr_i(r2, r2);
	    else
		extr_ui(r2, r2);
#    endif
#  else
#    if __WORDSIZE == 64
	    if (sign)
		ldi_c(r0, i1);
	    else
#    endif
		ldi_uc(r0, i1);
	    lshi(r0, r0, 24);
#    if __WORDSIZE == 32
	    ldi(r2, i1 + 1);
#    else
	    ldi_ui(r2, i1 + 1);
#    endif
	    rshi_u(r2, r2, 8);
#  endif
	}
	else {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#    if __WORDSIZE == 32
	    ldi(r0, i1 - 1);
#    else
	    ldi_ui(r0, i1 - 1);
#    endif
	    rshi_u(r0, r0, 8);
#    if __WORDSIZE == 64
	    if (sign)
		ldi_c(r2, i1 + 3);
	    else
#    endif
		ldi_uc(r2, i1 + 3);
	    lshi(r2, r2, 24);
#  else
	    ldi_i(r0, i1 - 1);
	    lshi(r0, r0, 8);
#    if __WORDSIZE == 64
	    if (sign)
		extr_i(r0, r0);
	    else
		extr_ui(r0, r0);
#    endif
	    ldi_uc(r2, i1 + 3);
#  endif
	}
	orr(r0, r0, r2);
	jit_unget_reg(t0);
    }
}

#  if __WORDSIZE == 64
static void
_unldr5(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    jit_word_t		un4, un2, un3, or, or4, or3;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -4);
    un4 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_ui(r0, r1);
    if (sign)
	ldxi_c(r2, r1, 4);
    else
	ldxi_uc(r2, r1, 4);
    lshi(r2, r2, 32);
#    else
    if (sign)
	ldr_i(r0, r1);
    else
	ldr_ui(r0, r1);
    lshi(r0, r0, 8);
    ldxi_uc(r2, r1, 4);
#    endif
    or = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un4, _jit->pc.w);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_us(r0, r1);
    ldxi_us(r2, r1, 2);
    lshi(r2, r2, 16);
    orr(r0, r0, r2);
    if (sign)
	ldxi_c(r2, r1, 4);
    else
	ldxi_uc(r2, r1, 4);
    lshi(r2, r2, 32);
#    else
    if (sign)
	ldr_s(r0, r1);
    else
	ldr_us(r0, r1);
    lshi(r0, r0, 24);
    ldxi_us(r2, r1, 2);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_uc(r2, r1, 4);
#    endif
    or4 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    if (sign)
	ldxi_i(r2, r1, 1);
    else
	ldxi_ui(r2, r1, 1);
    lshi(r2, r2, 8);
#    else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 32);
    ldxi_ui(r2, r1, 1);
#    endif
    or3 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    if (sign)
	ldxi_s(r2, r1, 3);
    else
	ldxi_us(r2, r1, 3);
    lshi(r2, r2, 24);
#    else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 32);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 16);
    orr(r0, r0, r2);
    ldxi_us(r2, r1, 3);
#    endif
    fallback_flush();
    fallback_patch_jmpi(or, _jit->pc.w);
    fallback_patch_jmpi(or4, _jit->pc.w);
    fallback_patch_jmpi(or3, _jit->pc.w);
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldi5(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & -4) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_ui(r0, i1);
	if (sign)
	    ldi_c(r2, i1 + 4);
	else
	    ldi_uc(r2, i1 + 4);
	lshi(r2, r2, 32);
#    else
	if (sign)
	    ldi_i(r0, i1);
	else
	    ldi_ui(r0, i1);
	lshi(r0, r0, 8);
	ldi_uc(r2, i1 + 4);
#    endif
    }
    else if ((i1 & -2) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_us(r0, i1);
	ldi_us(r2, i1 + 2);
	lshi(r2, r2, 16);
	orr(r0, r0, r2);
	if (sign)
	    ldi_c(r2, i1 + 4);
	else
	    ldi_uc(r2, i1 + 4);
	lshi(r2, r2, 32);
#    else
	if (sign)
	    ldi_s(r0, i1);
	else
	    ldi_us(r0, i1);
	lshi(r0, r0, 24);
	ldi_us(r2, i1 + 2);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	ldi_uc(r2, i1 + 4);
#    endif
    }
    else if ((i1 & 3) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	if (sign)
	    ldi_i(r2, i1 + 1);
	else
	    ldi_ui(r2, i1 + 1);
	lshi(r2, r2, 8);
#    else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 32);
	ldi_ui(r2, i1 + 1);
#    endif
    }
    else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	ldi_us(r2, i1 + 1);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	if (sign)
	    ldi_s(r2, i1 + 3);
	else
	    ldi_us(r2, i1 + 3);
	lshi(r2, r2, 24);
#    else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 32);
	ldi_us(r2, i1 + 1);
	lshi(r2, r2, 16);
	orr(r0, r0, r2);
	ldi_us(r2, i1 + 3);
#    endif
    }
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldr6(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    jit_word_t		un4, un2, un3, or, or2, or3;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -4);
    un4 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_ui(r0, r1);
    if (sign)
	ldxi_s(r2, r1, 4);
    else
	ldxi_us(r2, r1, 4);
    lshi(r2, r2, 32);
#    else
    if (sign)
	ldr_i(r0, r1);
    else
	ldr_ui(r0, r1);
    lshi(r0, r0, 16);
    ldxi_us(r2, r1, 4);
#    endif
    or = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un4, _jit->pc.w);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_us(r0, r1);
    if (sign)
	ldxi_i(r2, r1, 2);
    else
	ldxi_ui(r2, r1, 2);
    lshi(r2, r2, 16);
#   else
    if (sign)
	ldr_s(r0, r1);
    else
	ldr_us(r0, r1);
    lshi(r0, r0, 32);
    ldxi_ui(r2, r1, 2);
#    endif
    or2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_ui(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    if (sign)
	ldxi_c(r2, r1, 5);
    else
	ldxi_uc(r2, r1, 5);
    lshi(r2, r2, 40);
#    else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 40);
    ldxi_ui(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_uc(r2, r1, 5);
#    endif
    or3 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un3, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_us(r2, r1, 3);
    lshi(r2, r2, 24);
    orr(r0, r0, r2);
    if (sign)
	ldxi_c(r2, r1, 5);
    else
	ldxi_uc(r2, r1, 5);
    lshi(r2, r2, 40);
#    else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 40);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 24);
    orr(r0, r0, r2);
    ldxi_us(r2, r1, 3);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_uc(r2, r1, 5);
#    endif
    fallback_flush();
    fallback_patch_jmpi(or, _jit->pc.w);
    fallback_patch_jmpi(or2, _jit->pc.w);
    fallback_patch_jmpi(or3, _jit->pc.w);
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldi6(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & -4) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_ui(r0, i1);
	if (sign)
	    ldi_s(r2, i1 + 4);
	else
	    ldi_us(r2, i1 + 4);
	lshi(r2, r2, 32);
#    else
	if (sign)
	    ldi_i(r0, i1);
	else
	    ldi_ui(r0, i1);
	lshi(r0, r0, 16);
	ldi_us(r2, i1 + 4);
#    endif
    }
    else if ((i1 & -2) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_us(r0, i1);
	if (sign)
	    ldi_i(r2, i1 + 2);
	else
	    ldi_ui(r2, i1 + 2);
	lshi(r2, r2, 16);
#   else
	if (sign)
	    ldi_s(r0, i1);
	else
	    ldi_us(r0, i1);
	lshi(r0, r0, 32);
	ldi_ui(r2, i1 + 2);
#    endif
    }
    else if ((i1 & 3) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	ldi_ui(r2, i1 + 1);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	if (sign)
	    ldi_c(r2, i1 + 5);
	else
	    ldi_uc(r2, i1 + 5);
	lshi(r2, r2, 40);
#    else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 40);
	ldi_ui(r2, i1 + 1);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	ldi_uc(r2, i1 + 5);
#    endif
    }
    else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	ldi_us(r2, i1 + 1);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	ldi_us(r2, i1 + 3);
	lshi(r2, r2, 24);
	orr(r0, r0, r2);
	if (sign)
	    ldi_c(r2, i1 + 5);
	else
	    ldi_uc(r2, i1 + 5);
	lshi(r2, r2, 40);
#    else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 40);
	ldi_us(r2, i1 + 1);
	lshi(r2, r2, 24);
	orr(r0, r0, r2);
	ldi_us(r2, i1 + 3);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	ldi_uc(r2, i1 + 5);
#    endif
    }
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldr7(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    jit_word_t		un4, un2, un3, or, or2, or3;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -4);
    un4 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_ui(r0, r1);
    ldxi_us(r2, r1, 4);
    lshi(r2, r2, 32);
    orr(r0, r0, r2);
    if (sign)
	ldxi_c(r2, r1, 6);
    else
	ldxi_uc(r2, r1, 6);
    lshi(r2, r2, 48);
#    else
    if (sign)
	ldr_i(r0, r1);
    else
	ldr_ui(r0, r1);
    lshi(r0, r0, 24);
    ldxi_us(r2, r1, 4);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_uc(r2, r1, 6);
#    endif
    or = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un4, _jit->pc.w);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_us(r0, r1);
    ldxi_ui(r2, r1, 2);
    lshi(r2, r2, 16);
    orr(r0, r0, r2);
    if (sign)
	ldxi_c(r2, r1, 6);
    else
	ldxi_uc(r2, r1, 6);
    lshi(r2, r2, 48);
#    else
    if (sign)
	ldr_s(r0, r1);
    else
	ldr_us(r0, r1);
    lshi(r0, r0, 40);
    ldxi_ui(r2, r1, 2);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_uc(r2, r1, 6);
#    endif
    or2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_ui(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    if (sign)
	ldxi_s(r2, r1, 5);
    else
	ldxi_us(r2, r1, 5);
    lshi(r2, r2, 40);
#  else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 48);
    ldxi_ui(r2, r1, 1);
    lshi(r2, r2, 16);
    orr(r0, r0, r2);
    ldxi_us(r2, r1, 5);
#    endif
    or3 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    if (sign)
	ldxi_i(r2, r1, 3);
    else
	ldxi_ui(r2, r1, 3);
    lshi(r2, r2, 24);
#    else
    if (sign)
	ldr_c(r0, r1);
    else
	ldr_uc(r0, r1);
    lshi(r0, r0, 48);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 32);
    orr(r0, r0, r2);
    ldxi_ui(r2, r1, 3);
#    endif
    fallback_flush();
    fallback_patch_jmpi(or, _jit->pc.w);
    fallback_patch_jmpi(or2, _jit->pc.w);
    fallback_patch_jmpi(or3, _jit->pc.w);
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldi7(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1, jit_bool_t sign)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & -4) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_ui(r0, i1);
	ldi_us(r2, i1 + 4);
	lshi(r2, r2, 32);
	orr(r0, r0, r2);
	if (sign)
	    ldi_c(r2, i1 + 6);
	else
	    ldi_uc(r2, i1 + 6);
	lshi(r2, r2, 48);
#    else
	if (sign)
	    ldi_i(r0, i1);
	else
	    ldi_ui(r0, i1);
	lshi(r0, r0, 24);
	ldi_us(r2, i1 + 4);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	ldi_uc(r2, i1 + 6);
#    endif
    }
    else if ((i1 & -2) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_us(r0, i1);
	ldi_ui(r2, i1 + 2);
	lshi(r2, r2, 16);
	orr(r0, r0, r2);
	if (sign)
	    ldi_c(r2, i1 + 6);
	else
	    ldi_uc(r2, i1 + 6);
	lshi(r2, r2, 48);
#    else
	if (sign)
	    ldi_s(r0, i1);
	else
	    ldi_us(r0, i1);
	lshi(r0, r0, 40);
	ldi_ui(r2, i1 + 2);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	ldi_uc(r2, i1 + 6);
#    endif
    }
    else if ((i1 & 3) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	ldi_ui(r2, i1 + 1);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	if (sign)
	    ldi_s(r2, i1 + 5);
	else
	    ldi_us(r2, i1 + 5);
	lshi(r2, r2, 40);
#  else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 48);
	ldi_ui(r2, i1 + 1);
	lshi(r2, r2, 16);
	orr(r0, r0, r2);
	ldi_us(r2, i1 + 5);
#    endif
    }
    else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	ldi_uc(r0, i1);
	ldi_us(r2, i1 + 1);
	lshi(r2, r2, 8);
	orr(r0, r0, r2);
	if (sign)
	    ldi_i(r2, i1 + 3);
	else
	    ldi_ui(r2, i1 + 3);
	lshi(r2, r2, 24);
#    else
	if (sign)
	    ldi_c(r0, i1);
	else
	    ldi_uc(r0, i1);
	lshi(r0, r0, 48);
	ldi_us(r2, i1 + 1);
	lshi(r2, r2, 32);
	orr(r0, r0, r2);
	ldi_ui(r2, i1 + 3);
#    endif
    }
    orr(r0, r0, r2);
    jit_unget_reg(t0);
}

static void
_unldr8(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un8, un4, un2, un7, un6, un5, un3, al,
		        or, or2, or7, or6, or5, or3;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -8);
    un8 = fallback_bner(_jit->pc.w, r1, r2);
    ldr_l(r0, r1);
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un8, _jit->pc.w);
    andi(r2, r1, -4);
    un4 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_ui(r0, r1);
    ldxi_i(r2, r1, 4);
    lshi(r2, r2, 32);
#    else
    ldr_i(r0, r1);
    ldxi_ui(r2, r1, 4);
    lshi(r0, r0, 32);
#    endif
    or = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un4, _jit->pc.w);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r1, r2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_us(r0, r1);
    ldxi_ui(r2, r1, 2);
    lshi(r2, r2, 16);
    orr(r0, r0, r2);
    ldxi_s(r2, r1, 6);
    lshi(r2, r2, 48);
#    else
    ldr_s(r0, r1);
    lshi(r0, r0, 48);
    ldxi_ui(r2, r1, 2);
    lshi(r2, r2, 16);
    orr(r0, r0, r2);
    ldxi_us(r2, r1, 6);
#    endif
    or2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
    andi(r2, r1, 7);
    un7 = fallback_bnei(_jit->pc.w, r2, 7);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_l(r2, r1, 1);
    lshi(r2, r2, 8);
#    else
    ldr_c(r0, r1);
    ldxi_l(r2, r1, 1);
    rshi_u(r2, r2, 8);
    lshi(r0, r0, 56);
#    endif
    or7 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un7, _jit->pc.w);
    un6 = fallback_bnei(_jit->pc.w, r2, 6);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_us(r0, r1);
    ldxi_l(r2, r1, 2);
    lshi(r2, r2, 16);
#    else
    ldr_s(r0, r1);
    lshi(r0, r0, 48);
    ldxi_l(r2, r1, 2);
    rshi_u(r2, r2, 16);
#    endif
    or6 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un6, _jit->pc.w);
    un5 = fallback_bnei(_jit->pc.w, r2, 5);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    ldxi_ui(r0, r1, -1);
    rshi_u(r0, r0, 8);
    ldxi_ui(r2, r1, 3);
    lshi(r2, r2, 24);
    orr(r0, r0, r2);
    ldxi_c(r2, r1, 7);
    lshi(r2, r2, 56);
#  else
    ldxi_i(r0, r1, -1);
    lshi(r0, r0, 40);
    ldxi_ui(r2, r1, 3);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_uc(r2, r1, 7);
#  endif
    or5 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un5, _jit->pc.w);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_ui(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_i(r2, r1, 5);
    lshi(r2, r2, 40);
#    else
    ldr_c(r0, r1);
    lshi(r0, r0, 56);
    ldxi_ui(r2, r1, 1);
    lshi(r2, r2, 24);
    orr(r0, r0, r2);
    ldxi_ui(r2, r1, 5);
    rshi_u(r2, r2, 8);
#    endif
    or3 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    ldr_uc(r0, r1);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_ui(r2, r1, 3);
    lshi(r2, r2, 24);
    orr(r0, r0, r2);
    ldxi_c(r2, r1, 7);
    lshi(r2, r2, 56);
#    else
    ldr_c(r0, r1);
    lshi(r0, r0, 56);
    ldxi_us(r2, r1, 1);
    lshi(r2, r2, 40);
    orr(r0, r0, r2);
    ldxi_ui(r2, r1, 3);
    lshi(r2, r2, 8);
    orr(r0, r0, r2);
    ldxi_uc(r2, r1, 7);
#    endif
    fallback_flush();
    fallback_patch_jmpi(or, _jit->pc.w);
    fallback_patch_jmpi(or2, _jit->pc.w);
    fallback_patch_jmpi(or7, _jit->pc.w);
    fallback_patch_jmpi(or6, _jit->pc.w);
    fallback_patch_jmpi(or5, _jit->pc.w);
    fallback_patch_jmpi(or3, _jit->pc.w);
    orr(r0, r0, r2);
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unldi8(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    if ((i1 & -8) == i1)
	ldi_l(r0, i1);
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
	if ((i1 & -4) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_ui(r0, i1);
	    ldi_i(r2, i1 + 4);
	    lshi(r2, r2, 32);
#    else
	    ldi_i(r0, i1);
	    ldi_ui(r2, i1 + 4);
	    lshi(r0, r0, 32);
#    endif
	}
	else if ((i1 & -2) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_us(r0, i1);
	    ldi_ui(r2, i1 + 2);
	    lshi(r2, r2, 16);
	    orr(r0, r0, r2);
	    ldi_s(r2, i1 + 6);
	    lshi(r2, r2, 48);
#    else
	    ldi_s(r0, i1);
	    lshi(r0, r0, 48);
	    ldi_ui(r2, i1 + 2);
	    lshi(r2, r2, 16);
	    orr(r0, r0, r2);
	    ldi_us(r2, i1 + 6);
#    endif
	}
	else if ((i1 & 7) == 7) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_uc(r0, i1);
	    ldi_l(r2, i1 + 1);
	    lshi(r2, r2, 8);
#    else
	    ldi_c(r0, i1);
	    ldi_l(r2, i1 + 1);
	    rshi_u(r2, r2, 8);
	    lshi(r0, r0, 56);
#    endif
	}
	else if ((i1 & 7) == 6) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_us(r0, i1);
	    ldi_l(r2, i1 + 2);
	    lshi(r2, r2, 16);
#    else
	    ldi_s(r0, i1);
	    lshi(r0, r0, 48);
	    ldi_l(r2, i1 + 2);
	    rshi_u(r2, r2, 16);
#    endif
	}
	else if ((i1 & 7) == 5) {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_ui(r0, i1 - 1);
	    rshi_u(r0, r0, 8);
	    ldi_ui(r2, i1 + 3);
	    lshi(r2, r2, 24);
	    orr(r0, r0, r2);
	    ldi_c(r2, i1 + 7);
	    lshi(r2, r2, 56);
#  else
	    ldi_i(r0, i1 - 1);
	    lshi(r0, r0, 40);
	    ldi_ui(r2, i1 + 3);
	    lshi(r2, r2, 8);
	    orr(r0, r0, r2);
	    ldi_uc(r2, i1 + 7);
#  endif
	}
	else if ((i1 & 7) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_uc(r0, i1);
	    ldi_ui(r2, i1 + 1);
	    lshi(r2, r2, 8);
	    orr(r0, r0, r2);
	    ldi_i(r2, i1 + 5);
	    lshi(r2, r2, 40);
#    else
	    ldi_c(r0, i1);
	    lshi(r0, r0, 56);
	    ldi_ui(r2, i1 + 1);
	    lshi(r2, r2, 24);
	    orr(r0, r0, r2);
	    ldi_ui(r2, i1 + 5);
	    rshi_u(r2, r2, 8);
#    endif
	}
	else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    ldi_uc(r0, i1);
	    ldi_us(r2, i1 + 1);
	    lshi(r2, r2, 8);
	    orr(r0, r0, r2);
	    ldi_ui(r2, i1 + 3);
	    lshi(r2, r2, 24);
	    orr(r0, r0, r2);
	    ldi_c(r2, i1 + 7);
	    lshi(r2, r2, 56);
#    else
	    ldi_c(r0, i1);
	    lshi(r0, r0, 56);
	    ldi_us(r2, i1 + 1);
	    lshi(r2, r2, 40);
	    orr(r0, r0, r2);
	    ldi_ui(r2, i1 + 3);
	    lshi(r2, r2, 8);
	    orr(r0, r0, r2);
	    ldi_uc(r2, i1 + 7);
#    endif
	}
	orr(r0, r0, r2);
	jit_unget_reg(t0);
    }
}
#  endif

static void
_unstr2(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un2, al;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r2, r1);
    str_s(r1, r0);
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi_u(r2, r0, 8);
    stxi_c(1, r1, r2);
#else
    stxi_c(1, r1, r0);
    rshi_u(r2, r0, 8);
    str_c(r1, r2);
#endif
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unsti2(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    if ((i1 & -2) == i1)
	sti_s(i1, r0);
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
#if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi_u(r2, r0, 8);
	sti_c(1 + i1, r2);
#else
	sti_c(1 + i1, r0);
	rshi_u(r2, r0, 8);
	sti_c(i1, r2);
#endif
	jit_unget_reg(t0);
    }
}

static void
_unstr3(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un2, al;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r2, r1);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    str_s(r1, r0);
    rshi(r2, r0, 16);
    stxi_c(2, r1, r2);
#  else
    stxi_c(2, r1, r0);
    rshi(r2, r0, 8);
    str_s(r1, r2);
#  endif
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_s(1, r1, r2);
#  else
    stxi_s(1, r1, r0);
    rshi(r2, r0, 16);
    str_c(r1, r2);
#  endif
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unsti3(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & -2) == i1) {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_s(i1, r0);
	rshi(r2, r0, 16);
	sti_c(2 + i1, r2);
#  else
	sti_c(2 + i1, r0);
	rshi(r2, r0, 8);
	sti_s(i1, r2);
#  endif
    }
    else {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi(r2, r0, 8);
	sti_s(1 + i1, r2);
#  else
	sti_s(1 + i1, r0);
	rshi(r2, r0, 16);
	sti_c(i1, r2);
#  endif
    }
    jit_unget_reg(t0);
}

static void
_unstr4(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un4, un2, al, al2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -4);
    un4 = fallback_bner(_jit->pc.w, r2, r1);
    str_i(r1, r0);
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un4, _jit->pc.w);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r2, r1);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    str_s(r1, r0);
    rshi(r2, r0, 16);
    stxi_s(2, r1, r2);
#  else
    stxi_s(2, r1, r0);
    rshi(r2, r0, 16);
    str_s(r1, r2);
#  endif
    al2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    stxi_c(3, r1, r2);
#  else
    stxi_c(3, r1, r0);
    rshi(r2, r0, 8);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    str_c(r1, r2);
#  endif
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    fallback_patch_jmpi(al2, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unsti4(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    if ((i1 & -4) == i1)
	sti_i(i1, r0);
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
	if ((i1 & -2) == i1) {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    sti_s(i1, r0);
	    rshi(r2, r0, 16);
	    sti_s(2 + i1, r2);
#  else
	    sti_s(2 + i1, r0);
	    rshi(r2, r0, 16);
	    sti_s(i1, r2);
#  endif
	}
	else {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    sti_c(i1, r0);
	    rshi(r2, r0, 8);
	    sti_s(1 + i1, r2);
	    rshi(r2, r2, 16);
	    sti_c(3 + i1, r2);
#  else
	    sti_c(3 + i1, r0);
	    rshi(r2, r0, 8);
	    sti_s(1 + i1, r2);
	    rshi(r2, r2, 16);
	    sti_c(i1, r2);
#  endif
	}
	jit_unget_reg(t0);
    }
}

#  if __WORDSIZE == 64
static void
_unstr5(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un3, un2, un1, al, al2, al1;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_i(1, r1, r2);
#    else
    stxi_i(1, r1, r0);
    rshi(r2, r0, 32);
    str_c(r1, r2);
#    endif
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
    un2 = fallback_bnei(_jit->pc.w, r2, 2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_s(r1, r0);
    rshi(r2, r0, 16);
    stxi_s(2, r1, r2);
    rshi(r2, r2, 16);
    stxi_c(4, r1, r2);
#    else
    stxi_c(4, r1, r0);
    rshi(r2, r0, 8);
    stxi_s(2, r1, r2);
    rshi(r2, r2, 16);
    str_s(r1, r2);
#    endif
    al2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un2, _jit->pc.w);
    un1 = fallback_bnei(_jit->pc.w, r2, 1);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    stxi_s(3, r1, r2);
#    else
    stxi_s(3, r1, r0);
    rshi(r2, r0, 16);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    str_c(r1, r2);
#    endif
    al1 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un1, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_i(r1, r0);
    rshi(r2, r0, 32);
    stxi_c(4, r1, r2);
#    else
    stxi_c(4, r1, r0);
    rshi(r2, r0, 8);
    str_i(r1, r2);
#    endif
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    fallback_patch_jmpi(al2, _jit->pc.w);
    fallback_patch_jmpi(al1, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unsti5(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & 3) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi(r2, r0, 8);
	sti_i(1 + i1, r2);
#    else
	sti_i(1 + i1, r0);
	rshi(r2, r0, 32);
	sti_c(i1, r2);
#    endif
    }
    else if ((i1 & 3) == 2) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_s(i1, r0);
	rshi(r2, r0, 16);
	sti_s(2 + i1, r2);
	rshi(r2, r2, 16);
	sti_c(4 + i1, r2);
#    else
	sti_c(4 + i1, r0);
	rshi(r2, r0, 8);
	sti_s(2 + i1, r2);
	rshi(r2, r2, 16);
	sti_s(i1, r2);
#    endif
    }
    else if ((i1 & 3) == 1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi(r2, r0, 8);
	sti_s(1 + i1, r2);
	rshi(r2, r2, 16);
	sti_s(3 + i1, r2);
#    else
	sti_s(3 + i1, r0);
	rshi(r2, r0, 16);
	sti_s(1 + i1, r2);
	rshi(r2, r2, 16);
	sti_c(i1, r2);
#    endif
    }
    else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_i(i1, r0);
	rshi(r2, r0, 32);
	sti_c(4 + i1, r2);
#    else
	sti_c(4 + i1, r0);
	rshi(r2, r0, 8);
	sti_i(i1, r2);
#    endif
    }
    jit_unget_reg(t0);
}

static void
_unstr6(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un3, un2, un1, al, al2, al1;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_i(1, r1, r2);
    rshi(r2, r2, 32);
    stxi_c(5, r1, r2);
#    else
    stxi_c(5, r1, r0);
    rshi(r2, r0, 8);
    stxi_i(1, r1, r2);
    rshi(r2, r2, 32);
    str_c(r1, r2);
#    endif
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
    un2 = fallback_bnei(_jit->pc.w, r2, 2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_s(r1, r0);
    rshi(r2, r0, 16);
    stxi_s(2, r1, r2);
    rshi(r2, r2, 16);
    stxi_s(4, r1, r2);
#    else
    stxi_s(4, r1, r0);
    rshi(r2, r0, 16);
    stxi_s(2, r1, r2);
    rshi(r2, r2, 16);
    str_s(r1, r2);
#    endif
    al2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un2, _jit->pc.w);
    un1 = fallback_bnei(_jit->pc.w, r2, 1);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    stxi_s(3, r1, r2);
    rshi(r2, r2, 16);
    stxi_c(5, r1, r2);
#    else
    stxi_c(5, r1, r0);
    rshi(r2, r0, 8);
    stxi_s(3, r1, r2);
    rshi(r2, r2, 16);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    str_c(r1, r2);
#    endif
    al1 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un1, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_i(r1, r0);
    rshi(r2, r0, 32);
    stxi_s(4, r1, r2);
#    else
    stxi_s(4, r1, r0);
    rshi(r2, r0, 16);
    str_i(r1, r2);
#    endif
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    fallback_patch_jmpi(al2, _jit->pc.w);
    fallback_patch_jmpi(al1, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unsti6(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & 3) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi(r2, r0, 8);
	sti_i(1 + i1, r2);
	rshi(r2, r2, 32);
	sti_c(5 + i1, r2);
#    else
	sti_c(5 + i1, r0);
	rshi(r2, r0, 8);
	sti_i(1 + i1, r2);
	rshi(r2, r2, 32);
	sti_c(i1, r2);
#    endif
    }
    else if ((i1 & 3) == 2) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_s(i1, r0);
	rshi(r2, r0, 16);
	sti_s(2 + i1, r2);
	rshi(r2, r2, 16);
	sti_s(4 + i1, r2);
#    else
	sti_s(4 + i1, r0);
	rshi(r2, r0, 16);
	sti_s(2 + i1, r2);
	rshi(r2, r2, 16);
	sti_s(i1, r2);
#    endif
    }
    else if ((i1 & 3) == 1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi(r2, r0, 8);
	sti_s(1 + i1, r2);
	rshi(r2, r2, 16);
	sti_s(3 + i1, r2);
	rshi(r2, r2, 16);
	sti_c(5 + i1, r2);
#    else
	sti_c(5 + i1, r0);
	rshi(r2, r0, 8);
	sti_s(3 + i1, r2);
	rshi(r2, r2, 16);
	sti_s(1 + i1, r2);
	rshi(r2, r2, 16);
	sti_c(i1, r2);
#    endif
    }
    else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_i(i1, r0);
	rshi(r2, r0, 32);
	sti_s(4 + i1, r2);
#    else
	sti_s(4 + i1, r0);
	rshi(r2, r0, 16);
	sti_i(i1, r2);
#    endif
    }
    jit_unget_reg(t0);
}

static void
_unstr7(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un3, un2, un1, al, al2, al1;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_i(1, r1, r2);
    rshi(r2, r2, 32);
    stxi_s(5, r1, r2);
#    else
    stxi_s(5, r1, r0);
    rshi(r2, r0, 16);
    stxi_i(1, r1, r2);
    rshi(r2, r2, 32);
    str_c(r1, r2);
#    endif
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
    un2 = fallback_bnei(_jit->pc.w, r2, 2);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_s(r1, r0);
    rshi(r2, r0, 16);
    stxi_i(2, r1, r2);
    rshi(r2, r2, 32);
    stxi_c(6, r1, r2);
#    else
    stxi_c(6, r1, r0);
    rshi(r2, r0, 8);
    stxi_i(2, r1, r2);
    rshi(r2, r2, 32);
    str_s(r1, r2);
#    endif
    al2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un2, _jit->pc.w);
    un1 = fallback_bnei(_jit->pc.w, r2, 1);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    stxi_i(3, r1, r2);
#    else
    stxi_i(3, r1, r0);
    rshi(r2, r0, 32);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    str_c(r1, r2);
#    endif
    al1 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un1, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_i(r1, r0);
    rshi(r2, r0, 32);
    stxi_s(4, r1, r2);
    rshi(r2, r2, 16);
    stxi_c(6, r1, r2);
#    else
    stxi_c(6, r1, r0);
    rshi(r2, r0, 8);
    stxi_s(4, r1, r2);
    rshi(r2, r2, 16);
    str_i(r1, r2);
#    endif
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    fallback_patch_jmpi(al2, _jit->pc.w);
    fallback_patch_jmpi(al1, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unsti7(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if ((i1 & 3) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi(r2, r0, 8);
	sti_i(1 + i1, r2);
	rshi(r2, r2, 32);
	sti_s(5 + i1, r2);
#    else
	sti_s(5 + i1, r0);
	rshi(r2, r0, 16);
	sti_i(1 + i1, r2);
	rshi(r2, r2, 32);
	sti_c(i1, r2);
#    endif
    }
    else if ((i1 & 3) == 2) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_s(i1, r0);
	rshi(r2, r0, 16);
	sti_i(2 + i1, r2);
	rshi(r2, r2, 32);
	sti_c(6 + i1, r2);
#    else
	sti_c(6 + i1, r0);
	rshi(r2, r0, 8);
	sti_i(2 + i1, r2);
	rshi(r2, r2, 32);
	sti_s(i1, r2);
#    endif
    }
    else if ((i1 & 3) == 1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_c(i1, r0);
	rshi(r2, r0, 8);
	sti_s(1 + i1, r2);
	rshi(r2, r2, 16);
	sti_i(3 + i1, r2);
#    else
	sti_i(3 + i1, r0);
	rshi(r2, r0, 32);
	sti_s(1 + i1, r2);
	rshi(r2, r2, 16);
	sti_c(i1, r2);
#    endif
    }
    else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	sti_i(i1, r0);
	rshi(r2, r0, 32);
	sti_s(4 + i1, r2);
	rshi(r2, r2, 16);
	sti_c(6 + i1, r2);
#    else
	sti_c(6 + i1, r0);
	rshi(r2, r0, 8);
	sti_s(4 + i1, r2);
	rshi(r2, r2, 16);
	sti_i(i1, r2);
#    endif
    }
    jit_unget_reg(t0);
}

static void
_unstr8(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0, r2;
    jit_word_t		un8, un4, un2, un3, al, al4, al2, al3;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    andi(r2, r1, -8);
    un8 = fallback_bner(_jit->pc.w, r2, r1);
    str_l(r1, r0);
    al = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un8, _jit->pc.w);
    andi(r2, r1, -4);
    un4 = fallback_bner(_jit->pc.w, r2, r1);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    str_i(r1, r0);
    rshi(r2, r0, 32);
    stxi_i(4, r1, r2);
#  else
    stxi_i(4, r1, r0);
    rshi(r2, r0, 32);
    str_i(r1, r2);
#  endif
    al4 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un4, _jit->pc.w);
    andi(r2, r1, -2);
    un2 = fallback_bner(_jit->pc.w, r2, r1);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_s(r1, r0);
    rshi(r2, r0, 16);
    stxi_i(2, r1, r2);
    rshi(r2, r2, 32);
    stxi_s(6, r1, r2);
#    else
    stxi_s(6, r1, r0);
    rshi(r2, r0, 16);
    stxi_i(2, r1, r2);
    rshi(r2, r2, 32);
    str_s(r1, r2);
#    endif
    al2 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bner(un2, _jit->pc.w);
    andi(r2, r1, 3);
    un3 = fallback_bnei(_jit->pc.w, r2, 3);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_i(1, r1, r2);
    rshi(r2, r2, 32);
    stxi_s(5, r1, r2);
    rshi(r2, r2, 16);
    stxi_c(7, r1, r2);
#    else
    stxi_c(7, r1, r0);
    rshi(r2, r0, 8);
    stxi_s(5, r1, r2);
    rshi(r2, r2, 16);
    stxi_i(1, r1, r2);
    rshi(r2, r2, 32);
    str_c(r1, r2);
#    endif
    al3 = fallback_jmpi(_jit->pc.w);
    fallback_flush();
    fallback_patch_bnei(un3, _jit->pc.w);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
    str_c(r1, r0);
    rshi(r2, r0, 8);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    stxi_i(3, r1, r2);
    rshi(r2, r2, 32);
    stxi_c(7, r1, r2);
#    else
    stxi_c(7, r1, r0);
    rshi(r2, r0, 8);
    stxi_i(3, r1, r2);
    rshi(r2, r2, 32);
    stxi_s(1, r1, r2);
    rshi(r2, r2, 16);
    str_c(r1, r2);
#    endif
    fallback_flush();
    fallback_patch_jmpi(al, _jit->pc.w);
    fallback_patch_jmpi(al4, _jit->pc.w);
    fallback_patch_jmpi(al2, _jit->pc.w);
    fallback_patch_jmpi(al3, _jit->pc.w);
    jit_unget_reg(t0);
}

static void
_unsti8(jit_state_t *_jit, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
    if ((i1 & -8) == i1)
	sti_l(i1, r0);
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
	if ((i1 & -4) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    sti_i(i1, r0);
	    rshi(r2, r0, 32);
	    sti_i(4 + i1, r2);
#    else
	    sti_i(4 + i1, r0);
	    rshi(r2, r0, 32);
	    sti_i(i1, r2);
#    endif
	}
	else if ((i1 & -2) == i1) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    sti_s(i1, r0);
	    rshi(r2, r0, 16);
	    sti_i(2 + i1, r2);
	    rshi(r2, r2, 32);
	    sti_s(6 + i1, r2);
#    else
	    sti_s(6 + i1, r0);
	    rshi(r2, r0, 16);
	    sti_i(2 + i1, r2);
	    rshi(r2, r2, 32);
	    sti_s(i1, r2);
#    endif
	}
	else if ((i1 & 3) == 3) {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    sti_c(i1, r0);
	    rshi(r2, r0, 8);
	    sti_i(1 + i1, r2);
	    rshi(r2, r2, 32);
	    sti_s(5 + i1, r2);
	    rshi(r2, r2, 16);
	    sti_c(7 + i1, r2);
#    else
	    sti_c(7 + i1, r0);
	    rshi(r2, r0, 8);
	    sti_s(5 + i1, r2);
	    rshi(r2, r2, 16);
	    sti_i(1 + i1, r2);
	    rshi(r2, r2, 32);
	    sti_c(i1, r2);
#    endif
	}
	else {
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    sti_c(i1, r0);
	    rshi(r2, r0, 8);
	    sti_s(1 + i1, r2);
	    rshi(r2, r2, 16);
	    sti_i(3 + i1, r2);
	    rshi(r2, r2, 32);
	    sti_c(7 + i1, r2);
#    else
	    sti_c(7 + i1, r0);
	    rshi(r2, r0, 8);
	    sti_i(3 + i1, r2);
	    rshi(r2, r2, 32);
	    sti_s(1 + i1, r2);
	    rshi(r2, r2, 16);
	    sti_c(i1, r2);
#    endif
	}
	jit_unget_reg(t0);
    }
}
#  endif


static void
_unldrw(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_word_t		cross, done;
    jit_int32_t		t0, t1, t2, t3;
    t0 = fallback_jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	t1 = fallback_jit_get_reg(jit_class_gpr);
	movr(rn(t1), r1);
	r1 = rn(t1);
    }
    else
	t1 = JIT_NOREG;
    andi(rn(t0), r1, -(jit_word_t)sizeof(jit_word_t));
    ldr(r0, rn(t0));
    done = fallback_beqr(_jit->pc.w, rn(t0), r1);
    t2 = fallback_jit_get_reg(jit_class_gpr);
    andi(rn(t2), r1, sizeof(jit_word_t) - 1);
    t3 = fallback_jit_get_reg(jit_class_gpr);
    addi(rn(t3), rn(t2), i0);
    cross = fallback_blei(_jit->pc.w, rn(t3), sizeof(jit_word_t));
    ldxi(rn(t0), rn(t0), sizeof(jit_word_t));
    fallback_flush();
    fallback_patch_blei(cross, _jit->pc.w);
    jit_unget_reg(t3);
    lshi(rn(t2), rn(t2), 3);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    rshr_u(r0, r0, rn(t2));
    rsbi(rn(t2), rn(t2), __WORDSIZE);
    lshr(rn(t0), rn(t0), rn(t2));
#else
    lshr(r0, r0, rn(t2));
    rsbi(rn(t2), rn(t2), __WORDSIZE);
    rshr_u(rn(t0), rn(t0), rn(t2));
#endif
    jit_unget_reg(t2);
    orr(r0, r0, rn(t0));
    fallback_flush();
    fallback_patch_beqr(done, _jit->pc.w);
    jit_unget_reg(t0);
    if (t1 != JIT_NOREG)
	jit_unget_reg(t1);
}

static void
_unldiw(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    jit_word_t		addr;
    jit_int32_t		t0, sh;
    addr = i0 & -(jit_word_t)sizeof(jit_word_t);
    ldi(r0, addr);
    if (i0 != addr) {
	sh = (i0 & (sizeof(jit_word_t) - 1)) << 3;
	if (sh + i1 > sizeof(jit_word_t)) {
	    t0 = fallback_jit_get_reg(jit_class_gpr);
	    ldi(rn(t0), addr + sizeof(jit_word_t));
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    rshi_u(r0, r0, sh);
	    lshi(rn(t0), rn(t0), __WORDSIZE - sh);
#else
	    lshi(r0, r0, sh);
	    rshi_u(rn(t0), rn(t0), __WORDSIZE - sh);
#endif
	    orr(r0, r0, rn(t0));
	    jit_unget_reg(t0);
	}
    }
}

static void
_unldx(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    switch (i0) {
	case 2:
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    extr_s(r0, r0);
#else
	    rshi(r0, r0, __WORDSIZE - 16);
#endif
	    break;
	case 3:
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 24);
#endif
	    rshi(r0, r0, __WORDSIZE - 24);
	    break;
#if __WORDSIZE == 32
	default:
#else
	case 4:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    extr_i(r0, r0);
#  else
	    rshi(r0, r0, __WORDSIZE - 32);
#  endif
#endif
	    break;
#if __WORDSIZE == 64
	case 5:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 40);
#  endif
	    rshi(r0, r0, __WORDSIZE - 40);
	    break;
	case 6:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 48);
#  endif
	    rshi(r0, r0, __WORDSIZE - 48);
	    break;
	case 7:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 56);
#  endif
	    rshi(r0, r0, __WORDSIZE - 56);
	    break;
	default:
	    break;
#endif
    }
}

static void
_unldx_u(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    switch (i0) {
	case 2:
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    extr_us(r0, r0);
#else
	    rshi_u(r0, r0, __WORDSIZE - 16);
#endif
	    break;
	case 3:
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 24);
#endif
	    rshi_u(r0, r0, __WORDSIZE - 24);
	    break;
#if __WORDSIZE == 32
	default:
#else
	case 4:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    extr_ui(r0, r0);
#  else
	    rshi_u(r0, r0, __WORDSIZE - 32);
#  endif
#endif
	    break;
#if __WORDSIZE == 64
	case 5:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 40);
#  endif
	    rshi_u(r0, r0, __WORDSIZE - 40);
	    break;
	case 6:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 48);
#  endif
	    rshi_u(r0, r0, __WORDSIZE - 48);
	    break;
	case 7:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
	    lshi(r0, r0, __WORDSIZE - 56);
#  endif
	    rshi_u(r0, r0, __WORDSIZE - 56);
	    break;
	default:
	    break;
#endif
    }
}

static void
_fallback_unldr(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0, r2;
    assert(i0 >= 1 && i0 <= (__WORDSIZE >> 3));
    if (i0 == 1)
	ldr_c(r0, r1);
    else if (_jitc->unld_algorithm != 0) {
	if (r0 == r1) {
	    t0 = fallback_jit_get_reg(jit_class_gpr);
	    r2 = rn(t0);
	    movr(r2, r0);
	}
	else
	    r2 = r1;
	switch (i0) {
	    case 2:	unldr2(r0, r2, 1);	break;
	    case 3:	unldr3(r0, r2, 1);	break;
#  if __WORDSIZE == 32
	    default:	unldr4(r0, r2);	  	break;
#  else
	    case 4:	unldr4(r0, r2, 1);	break;
	    case 5:	unldr5(r0, r2, 1);	break;
	    case 6:	unldr6(r0, r2, 1);	break;
	    case 7:	unldr7(r0, r2, 1);	break;
	    default:	unldr8(r0, r2);	   	break;
#  endif
	}
	if (i0 > 1 && r0 == r1)
	    jit_unget_reg(t0);
    }
    else {
	unldrw(r0, r1, i0);
	unldx(r0, i0);
    }
}

static void
_fallback_unldi(jit_state_t *_jit,
		jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    assert(i1 >= 1 && i1 <= (__WORDSIZE >> 3));
    if (i1 == 1)
	ldi_c(r0, i0);
    else if (_jitc->unld_algorithm != 0) {
	switch (i1) {
	    case 2:	unldi2(r0, i0, 1);	break;
	    case 3:	unldi3(r0, i0, 1);	break;
#  if __WORDSIZE == 32
	    default:	unldi4(r0, i0);		break;
#  else
	    case 4:	unldi4(r0, i0, 1);	break;
	    case 5:	unldi5(r0, i0, 1);	break;
	    case 6:	unldi6(r0, i0, 1);	break;
	    case 7:	unldi7(r0, i0, 1);	break;
	    default:	unldi8(r0, i0);		break;
#  endif
	}
    }
    else {
	unldiw(r0, i0, i1);
	unldx(r0, i1);
    }
}

static void
_fallback_unldr_u(jit_state_t *_jit,
		  jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0, r2;
    assert(i0 >= 1 && i0 <= (__WORDSIZE >> 3));
    if (i0 == 1)
	ldr_uc(r0, r1);
    else if (_jitc->unld_algorithm != 0) {
	if (r0 == r1) {
	    t0 = fallback_jit_get_reg(jit_class_gpr);
	    r2 = rn(t0);
	    movr(r2, r0);
	}
	else
	    r2 = r1;
	switch (i0) {
	    case 2:	unldr2(r0, r2, 0);	break;
	    case 3:	unldr3(r0, r2, 0);	break;
#  if __WORDSIZE == 32
	    default:	unldr4(r0, r2);		break;
#  else
	    case 4:	unldr4(r0, r2, 0);	break;
	    case 5:	unldr5(r0, r2, 0);	break;
	    case 6:	unldr6(r0, r2, 0);	break;
	    case 7:	unldr7(r0, r2, 0);	break;
	    default:	unldr8(r0, r2);		break;
#  endif
	}
	if (i0 > 1 && r0 == r1)
	    jit_unget_reg(t0);
    }
    else {
	unldrw(r0, r1, i0);
	unldx_u(r0, i0);
    }
}

static void
_fallback_unldi_u(jit_state_t *_jit,
		  jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    assert(i1 >= 1 && i1 <= (__WORDSIZE >> 3));
    if (i1 == 1)
	ldi_uc(r0, i0);
    else if (_jitc->unld_algorithm != 0) {
	switch (i1) {
	    case 2:	unldi2(r0, i0, 0);	break;
	    case 3:	unldi3(r0, i0, 0);	break;
#  if __WORDSIZE == 32
	    default:	unldi4(r0, i0);		break;
#  else
	    case 4:	unldi4(r0, i0, 0);	break;
	    case 5:	unldi5(r0, i0, 0);	break;
	    case 6:	unldi6(r0, i0, 0);	break;
	    case 7:	unldi7(r0, i0, 0);	break;
	    default:	unldi8(r0, i0);		break;
#  endif
	}

    }
    else {
	unldiw(r0, i0, i1);
	unldx_u(r0, i1);
    }
}

static void
_fallback_unstr(jit_state_t *_jit,
		jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_word_t		done;
    jit_int32_t		t0, t1, t2, t3, t4, t5;
    assert(i0 > 0 && i0 <= (__WORDSIZE >> 3));
    if (i0 == 1)
	str_c(r0, r1);
    else if (_jitc->unst_algorithm == 0) {
	switch (i0) {
	    case 2:	unstr2(r1, r0);		break;
	    case 3:	unstr3(r1, r0);		break;
#  if __WORDSIZE == 32
	    default:	unstr4(r1, r0);		break;
#  else
	    case 4:	unstr4(r1, r0);		break;
	    case 5:	unstr5(r1, r0);		break;
	    case 6:	unstr6(r1, r0);		break;
	    case 7:	unstr7(r1, r0);		break;
	    default:	unstr8(r1, r0);		break;
#  endif
	}
    }
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	t1 = fallback_jit_get_reg(jit_class_gpr);
	t2 = fallback_jit_get_reg(jit_class_gpr);
	t3 = fallback_jit_get_reg(jit_class_gpr);
	/* Zero out top bits and keep value to store in t0 */
	if (i0 != sizeof(jit_word_t)) {
	    lshi(rn(t3), r1, (sizeof(jit_word_t) - i0) << 3);
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    rshi_u(rn(t3), rn(t3), (sizeof(jit_word_t) - i0) << 3);
#endif
	}
	else
	    movr(rn(t3), r1);
	/* Check alignment */
	andi(rn(t2), r0, sizeof(jit_word_t) - 1);
	/* Multiply by 8 */
	lshi(rn(t2), rn(t2), 3);
	/* Split values to store (assume will need two stores) */
#if __BYTE_ORDER == __LITTLE_ENDIAN
	qlshr_u(rn(t0), rn(t1), rn(t3), rn(t2));
#else
	qrshr_u(rn(t0), rn(t1), rn(t3), rn(t2));
#endif
	/* Generate masks for values in memory */
	if (i0 == sizeof(jit_word_t))
	    movi(rn(t3), -1);
	else {
#if __BYTE_ORDER == __BIG_ENDIAN
	    movi(rn(t3), ((1L << (i0 << 3)) - 1) <<
		 ((sizeof(jit_word_t) - i0) << 3));
#else
	    movi(rn(t3), (1L << (i0 << 3)) - 1);
#endif
	}
#if __BYTE_ORDER == __LITTLE_ENDIAN
	qlshr_u(rn(t2), rn(t3), rn(t3), rn(t2));
#else
	qrshr_u(rn(t2), rn(t3), rn(t3), rn(t2));
#endif
	comr(rn(t2), rn(t2));
	comr(rn(t3), rn(t3));
	/* Allocate two extra registers later in case need temps for *q?shr_u */
	t4 = fallback_jit_get_reg(jit_class_gpr);
	t5 = fallback_jit_get_reg(jit_class_gpr);
	/* Store words */
	andi(rn(t4), r0, -(jit_word_t)sizeof(jit_word_t));
	ldr(rn(t5), rn(t4));
	andr(rn(t5), rn(t5), rn(t2));
	orr(rn(t0), rn(t0), rn(t5));
	str(rn(t4), rn(t0));
	/* Make sure to not read/write on possibly unmaped memory */
	addi(rn(t5), rn(t4), i0);
	done = fallback_blei(_jit->pc.w, rn(t5), sizeof(jit_word_t));
	/* Store second word if vlaue crosses a word boundary */
	ldxi(rn(t5), rn(t4), sizeof(jit_word_t));
	andr(rn(t5), rn(t5), rn(t3));
	orr(rn(t1), rn(t1), rn(t5));
	stxi(sizeof(jit_word_t), rn(t4), rn(t1));
	/* Finished */
	fallback_flush();
	fallback_patch_blei(done, _jit->pc.w);
	/* Generic/simple algorithm needs 6 temporaries, as it cannot
	 * change any of the argument registers, might need to truncate
	 * the value to store, and need a pair for values to store and
	 * another for the masks. */
	jit_unget_reg(t5);
	jit_unget_reg(t4);
	jit_unget_reg(t3);
	jit_unget_reg(t2);
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
}

static void
_fallback_unsti(jit_state_t *_jit,
		jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		done, address;
    jit_int32_t		t0, t1, t2, t3, t4;
    assert(i1 > 0 && i1 <= (__WORDSIZE >> 3));
    if (i1 == 1)
	sti_c(i0, r0);
    else if (_jitc->unst_algorithm == 0) {
	switch (i1) {
	    case 1:	sti_c(i0, r0);		break;
	    case 2:	unsti2(r0, i0);		break;
	    case 3:	unsti3(r0, i0);		break;
#  if __WORDSIZE == 32
	    default:	unsti4(r0, i0);		break;
#  else
	    case 4:	unsti4(r0, i0);		break;
	    case 5:	unsti5(r0, i0);		break;
	    case 6:	unsti6(r0, i0);		break;
	    case 7:	unsti7(r0, i0);		break;
	    default:	unsti8(r0, i0);		break;
#  endif
    }
    }
    else {
	t0 = fallback_jit_get_reg(jit_class_gpr);
	t1 = fallback_jit_get_reg(jit_class_gpr);
	t2 = fallback_jit_get_reg(jit_class_gpr);
	t3 = fallback_jit_get_reg(jit_class_gpr);
	t4 = fallback_jit_get_reg(jit_class_gpr);
	/* Zero out top bits and keep value to store in t0 */
	if (i1 != sizeof(jit_word_t)) {
	    lshi(rn(t2), r0, (sizeof(jit_word_t) - i1) << 3);
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    rshi_u(rn(t2), rn(t2), (sizeof(jit_word_t) - i1) << 3);
	    qlshi_u(rn(t0), rn(t1), rn(t2),
		    (i0 & (sizeof(jit_word_t) - 1)) << 3);
#else
	    qrshi_u(rn(t0), rn(t1), rn(t2),
		    (i0 & (sizeof(jit_word_t)) - 1) << 3);
#endif
	}
	else {
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    qlshi_u(rn(t0), rn(t1), r0,
		    (i0 & (sizeof(jit_word_t)) - 1) << 3);
#else
	    qrshi_u(rn(t0), rn(t1), r0,
		    (i0 & (sizeof(jit_word_t)) - 1) << 3);
#endif
	}
	/* Generate masks for values in memory */
	if (i1 == sizeof(jit_word_t))
	    movi(rn(t2), -1);
	else {
#if __BYTE_ORDER == __BIG_ENDIAN
	    movi(rn(t2), ((1L << (i1 << 3)) - 1) <<
		 ((sizeof(jit_word_t) - i1) << 3));
#else
	    movi(rn(t2), (1L << (i1 << 3)) - 1);
#endif
	}
#if __BYTE_ORDER == __LITTLE_ENDIAN
	qlshi_u(rn(t2), rn(t3), rn(t2), (i0 & (sizeof(jit_word_t)) - 1) << 3);
#else
	qrshi_u(rn(t2), rn(t3), rn(t2), (i0 & (sizeof(jit_word_t)) - 1) << 3);
#endif
	comr(rn(t2), rn(t2));
	comr(rn(t3), rn(t3));
	/* Store words */
	address = i0 & -(jit_word_t)sizeof(jit_word_t);
	ldi(rn(t4), address);
	andr(rn(t4), rn(t4), rn(t2));
	orr(rn(t0), rn(t0), rn(t4));
	sti(address, rn(t0));
	if (address + i1 > sizeof(jit_word_t)) {
	    address += sizeof(jit_word_t);
	    ldi(rn(t4), address);
	    andr(rn(t4), rn(t4), rn(t3));
	    orr(rn(t1), rn(t1), rn(t4));
	    sti(address, rn(t1));
	}
	jit_unget_reg(t4);
	jit_unget_reg(t3);
	jit_unget_reg(t2);
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
}

#  ifdef fallback_unldr_x
static void
_fallback_unldr_x(jit_state_t *_jit,
		  jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0, r2;
#    if __WORDSIZE == 32
    jit_int32_t		t1, r3;
#    endif
    assert(i0 == 4 || i0 == 8);
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if (i0 == 4) {
	unldr(r2, r1, 4);
	movr_w_f(r0, r2);
    }
    else {
#    if __WORDSIZE == 32
	t1 = fallback_jit_get_reg(jit_class_gpr);
	r3 = rn(t1);
#      if __BYTE_ORDER == __LITTLE_ENDIAN
	unldr(r2, r1, 4);
	addi(r3, r1, 4);
	unldr(r3, r3, 4);
#      else
	unldr(r3, r1, 4);
	addi(r2, r1, 4);
	unldr(r2, r2, 4);
#      endif
	movr_ww_d(r0, r2, r3);
	jit_unget_reg(t1);
#    else
	unldr(r2, r1, 8);
	movr_w_d(r0, r2);
#    endif
    }
    jit_unget_reg(t0);
}
#  endif

#  ifdef fallback_unldi_x
static void
_fallback_unldi_x(jit_state_t *_jit,
		  jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
#    if __WORDSIZE == 32
    jit_int32_t		t1, r3;
#    endif
    assert(i1 == 4 || i1 == 8);
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if (i1 == 4) {
	unldi(r2, i0, 4);
	movr_w_f(r0, r2);
    }
    else {
#    if __WORDSIZE == 32
	t1 = fallback_jit_get_reg(jit_class_gpr);
	r3 = rn(t1);
#      if __BYTE_ORDER == __LITTLE_ENDIAN
	unldi(r2, i0, 4);
	unldi(r3, i0 + 4, 4);
#      else
	unldi(r3, i0, 4);
	unldi(r2, i0 + 4, 4);
#      endif
	movr_ww_d(r0, r3, r2);
	jit_unget_reg(t1);
#    else
	unldi(r2, i0, 8);
	movr_w_d(r0, r2);
#    endif
    }
    jit_unget_reg(t0);
}
#  endif

#  ifdef fallback_unstr_x
static void
_fallback_unstr_x(jit_state_t *_jit,
		  jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0, r2;
#    if __WORDSIZE == 32
    jit_int32_t		t1, r3;
#    endif
    assert(i0 == 4 || i0 == 8);
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if (i0 == 4) {
	movr_f_w(r2, r1);
	unstr(r0, r2, 4);
    }
    else {
#    if __WORDSIZE == 32
	t1 = fallback_jit_get_reg(jit_class_gpr);
	r3 = rn(t1);
	movr_d_ww(r2, r3, r1);
#      if __BYTE_ORDER == __LITTLE_ENDIAN
	unstr(r0, r2, 4);
	addi(r2, r0, 4);
	unstr(r2, r3, 4);
#      else
	unstr(r0, r3, 4);
	addi(r3, r0, 4);
	unstr(r3, r2, 4);
#      endif
	jit_unget_reg(t1);
#    else
	movr_d_w(r2, r1);
	unstr(r0, r2, 8);
#    endif
    }
    jit_unget_reg(t0);
}
#  endif

#  ifdef fallback_unsti_x
static void
_fallback_unsti_x(jit_state_t *_jit,
		  jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
#    if __WORDSIZE == 32
    jit_int32_t		t1, r3;
#    endif
    assert(i1 == 4 || i1 == 8);
    t0 = fallback_jit_get_reg(jit_class_gpr);
    r2 = rn(t0);
    if (i1 == 4) {
	movr_f_w(r2, r0);
	unsti(i0, r2, 4);
    }
    else {
#    if __WORDSIZE == 32
	t1 = fallback_jit_get_reg(jit_class_gpr);
	r3 = rn(t1);
	movr_d_ww(r2, r3, r0);
#      if __BYTE_ORDER == __LITTLE_ENDIAN
	unsti(i0, r3, 4);
	unsti(i0 + 4, r2, 4);
#      else
	unsti(i0, r2, 4);
	unsti(i0 + 4, r3, 4);
#      endif
	jit_unget_reg(t1);
#    else
	movr_d_w(r2, r0);
	unsti(i0, r2, 8);
#    endif
    }
    jit_unget_reg(t0);
}
#  endif
#endif
