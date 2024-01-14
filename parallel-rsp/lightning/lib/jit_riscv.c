/*
 * Copyright (C) 2019-2023  Free Software Foundation, Inc.
 *
 * This file is part of GNU lightning.
 *
 * GNU lightning is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU lightning is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * Authors:
 *	Paulo Cesar Pereira de Andrade
 */

/* callee save					  + variadic arguments
 * align16(ra+fp+s[1-9]+s10+s11+fs[0-9]+fs10+fs11)+align16(a[0-7]) */
#define stack_framesize			(208 + 64)

#define jit_arg_reg_p(i)		((i) >= 0 && (i) < 8)
#define jit_arg_f_reg_p(i)		((i) >= 0 && (i) < 8)

/*
 * Types
 */
typedef jit_pointer_t jit_va_list_t;

/*
 * Prototypes
 */
#define compute_framesize()		_compute_framesize(_jit)
static void _compute_framesize(jit_state_t*);
#if __WORDSIZE == 64
#  define load_const(r0, i0)		_load_const(_jit, r0, i0)
static void _load_const(jit_state_t*, jit_int32_t, jit_word_t);
static jit_word_t hash_const(jit_word_t);
#  define put_const(i0)			_put_const(_jit, i0)
static void _put_const(jit_state_t*, jit_word_t);
#  define get_const(i0)			_get_const(_jit, i0)
static jit_word_t _get_const(jit_state_t*, jit_word_t);
#endif
#define patch(instr, node)		_patch(_jit, instr, node)
static void _patch(jit_state_t*,jit_word_t,jit_node_t*);

#define PROTO				1
#  include "jit_riscv-cpu.c"
#  include "jit_riscv-fpu.c"
#  include "jit_fallback.c"
#undef PROTO

/*
 * Initialization
 */
jit_register_t		_rvs[] = {
    { 0x00,				"zero" },
    { 0x01,				"ra" },
    { 0x02,				"sp" },
    { 0x03,				"gp" },
#if 0		/* Pretend it does not exist, so _NOREG can be used in
		 * a 64 bit bitmask */
    { 0x04,				"tp" },
#endif
    { rc(gpr) | 0x05,			"t0" },
    { rc(gpr) | 0x06,			"t1" },
    { rc(gpr) | 0x07,			"t2" },
    { rc(gpr) | 0x1c,			"t3" },
    { rc(gpr) | 0x1d,			"t4" },
    { rc(gpr) | 0x1e,			"t5" },
    { rc(gpr) | 0x1f,			"t6" },
    { 0x08,				"fp" },
    { rc(sav) | rc(gpr) | 0x09,		"s1" },
    { rc(sav) | rc(gpr) | 0x12,		"s2" },
    { rc(sav) | rc(gpr) | 0x13,		"s3" },
    { rc(sav) | rc(gpr) | 0x14,		"s4" },
    { rc(sav) | rc(gpr) | 0x15,		"s5" },
    { rc(sav) | rc(gpr) | 0x16,		"s6" },
    { rc(sav) | rc(gpr) | 0x17,		"s7" },
    { rc(sav) | rc(gpr) | 0x18,		"s8" },
    { rc(sav) | rc(gpr) | 0x19,		"s9" },
    { rc(sav) | rc(gpr) | 0x1a,		"s10" },
    { rc(sav) | rc(gpr) | 0x1b,		"s11" },
    { rc(arg) | rc(gpr) | 0x11,		"a7" },
    { rc(arg) | rc(gpr) | 0x10,		"a6" },
    { rc(arg) | rc(gpr) | 0x0f,		"a5" },
    { rc(arg) | rc(gpr) | 0x0e,		"a4" },
    { rc(arg) | rc(gpr) | 0x0d,		"a3" },
    { rc(arg) | rc(gpr) | 0x0c,		"a2" },
    { rc(arg) | rc(gpr) | 0x0b,		"a1" },
    { rc(arg) | rc(gpr) | 0x0a,		"a0" },
    { rc(fpr) | 0x00,			"ft0" },
    { rc(fpr) | 0x01,			"ft1" },
    { rc(fpr) | 0x02,			"ft2" },
    { rc(fpr) | 0x03,			"ft3" },
    { rc(fpr) | 0x04,			"ft4" },
    { rc(fpr) | 0x05,			"ft5" },
    { rc(fpr) | 0x06,			"ft6" },
    { rc(fpr) | 0x07,			"ft7" },
    { rc(fpr) | 0x1c,			"ft8" },
    { rc(fpr) | 0x1d,			"ft9" },
    { rc(fpr) | 0x1e,			"ft10" },
    { rc(fpr) | 0x1f,			"ft11" },
    { rc(sav) | rc(fpr) | 0x08,		"fs0" },
    { rc(sav) | rc(fpr) | 0x09,		"fs1" },
    { rc(sav) | rc(fpr) | 0x12,		"fs2" },
    { rc(sav) | rc(fpr) | 0x13,		"fs3" },
    { rc(sav) | rc(fpr) | 0x14,		"fs4" },
    { rc(sav) | rc(fpr) | 0x15,		"fs5" },
    { rc(sav) | rc(fpr) | 0x16,		"fs6" },
    { rc(sav) | rc(fpr) | 0x17,		"fs7" },
    { rc(sav) | rc(fpr) | 0x18,		"fs8" },
    { rc(sav) | rc(fpr) | 0x19,		"fs9" },
    { rc(sav) | rc(fpr) | 0x1a,		"fs10" },
    { rc(sav) | rc(fpr) | 0x1b,		"fs11" },
    { rc(arg) | rc(fpr) | 0x11,		"fa7" },
    { rc(arg) | rc(fpr) | 0x10,		"fa6" },
    { rc(arg) | rc(fpr) | 0x0f,		"fa5" },
    { rc(arg) | rc(fpr) | 0x0e,		"fa4" },
    { rc(arg) | rc(fpr) | 0x0d,		"fa3" },
    { rc(arg) | rc(fpr) | 0x0c,		"fa2" },
    { rc(arg) | rc(fpr) | 0x0b,		"fa1" },
    { rc(arg) | rc(fpr) | 0x0a,		"fa0" },
    { _NOREG,				"<none>" },
};

static jit_int32_t iregs[] = {
    _S1, _S2, _S3, _S4, _S5, _S6, _S7, _S8, _S9, _S10, _S11
};

static jit_int32_t fregs[] = {
    _FS0, _FS1, _FS2, _FS3, _FS4, _FS5, _FS6, _FS7, _FS8, _FS9, _FS10, _FS11
};

/*
 * Implementation
 */
void
jit_get_cpu(void)
{
}

void
_jit_init(jit_state_t *_jit)
{
    _jitc->reglen = jit_size(_rvs) - 1;
    jit_carry = _NOREG;
}

void
_jit_prolog(jit_state_t *_jit)
{
    jit_int32_t		 offset;

    if (_jitc->function)
	jit_epilog();
    assert(jit_regset_cmp_ui(&_jitc->regarg, 0) == 0);
    jit_regset_set_ui(&_jitc->regsav, 0);
    offset = _jitc->functions.offset;
    if (offset >= _jitc->functions.length) {
	jit_realloc((jit_pointer_t *)&_jitc->functions.ptr,
		    _jitc->functions.length * sizeof(jit_function_t),
		    (_jitc->functions.length + 16) * sizeof(jit_function_t));
	_jitc->functions.length += 16;
    }
    _jitc->function = _jitc->functions.ptr + _jitc->functions.offset++;
    _jitc->function->self.size = stack_framesize;
    _jitc->function->self.argi = _jitc->function->self.argf =
	_jitc->function->self.alen = 0;
    _jitc->function->self.aoff = 0;
    _jitc->function->self.call = jit_call_default;
    jit_alloc((jit_pointer_t *)&_jitc->function->regoff,
	      _jitc->reglen * sizeof(jit_int32_t));

    /* _no_link here does not mean the jit_link() call can be removed
     * by rewriting as:
     * _jitc->function->prolog = jit_new_node(jit_code_prolog);
     */
    _jitc->function->prolog = jit_new_node_no_link(jit_code_prolog);
    jit_link(_jitc->function->prolog);
    _jitc->function->prolog->w.w = offset;
    _jitc->function->epilog = jit_new_node_no_link(jit_code_epilog);
    /*	u:	label value
     *	v:	offset in blocks vector
     *	w:	offset in functions vector
     */
    _jitc->function->epilog->w.w = offset;

    jit_regset_new(&_jitc->function->regset);
}

jit_int32_t
_jit_allocai(jit_state_t *_jit, jit_int32_t length)
{
    assert(_jitc->function);
    jit_check_frame();
    switch (length) {
	case 0:	case 1:						break;
	case 2:		_jitc->function->self.aoff &= -2;	break;
	case 3:	case 4:	_jitc->function->self.aoff &= -4;	break;
	default:	_jitc->function->self.aoff &= -8;	break;
    }
    _jitc->function->self.aoff -= length;
    if (!_jitc->realize) {
	jit_inc_synth_ww(allocai, _jitc->function->self.aoff, length);
	jit_dec_synth();
    }
    return (_jitc->function->self.aoff);
}

void
_jit_allocar(jit_state_t *_jit, jit_int32_t u, jit_int32_t v)
{
    jit_int32_t		 r0;
    assert(_jitc->function);
    jit_inc_synth_ww(allocar, u, v);
    if (!_jitc->function->allocar) {
	_jitc->function->aoffoff = jit_allocai(sizeof(jit_int32_t));
	_jitc->function->allocar = 1;
    }
    r0 = jit_get_reg(jit_class_gpr);
    jit_negr(r0, v);
    jit_andi(r0, r0, -16);
    jit_ldxi_i(u, JIT_FP, _jitc->function->aoffoff);
    jit_addr(u, u, r0);
    jit_addr(JIT_SP, JIT_SP, r0);
    jit_stxi_i(_jitc->function->aoffoff, JIT_FP, u);
    jit_unget_reg(r0);
    jit_dec_synth();
}

void
_jit_ret(jit_state_t *_jit)
{
    jit_node_t		*instr;
    assert(_jitc->function);
    jit_inc_synth(ret);
    /* jump to epilog */
    instr = jit_jmpi();
    jit_patch_at(instr, _jitc->function->epilog);
    jit_dec_synth();
}

void
_jit_retr(jit_state_t *_jit, jit_int32_t u, jit_code_t code)
{
    jit_code_inc_synth_w(code, u);
    jit_movr(JIT_RET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti(jit_state_t *_jit, jit_word_t u, jit_code_t code)
{
    jit_code_inc_synth_w(code, u);
    jit_movi(JIT_RET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_retr_f(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr_f, u);
    if (u != JIT_FRET)
	jit_movr_f(JIT_FRET, u);
    else
	jit_live(JIT_FRET);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_inc_synth_f(reti_f, u);
    jit_movi_f(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_retr_d(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr_d, u);
    if (u != JIT_FRET)
	jit_movr_d(JIT_FRET, u);
    else
	jit_live(JIT_FRET);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_inc_synth_d(reti_d, u);
    jit_movi_d(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_epilog(jit_state_t *_jit)
{
    assert(_jitc->function);
    assert(_jitc->function->epilog->next == NULL);
    jit_link(_jitc->function->epilog);
    _jitc->function = NULL;
}

jit_bool_t
_jit_arg_register_p(jit_state_t *_jit, jit_node_t *u)
{
    if (u->code >= jit_code_arg_c && u->code <= jit_code_arg)
	return (jit_arg_reg_p(u->u.w));
    assert(u->code == jit_code_arg_f || u->code == jit_code_arg_d);
    return (jit_arg_f_reg_p(u->u.w) || jit_arg_reg_p(u->u.w - 8));
}

void
_jit_ellipsis(jit_state_t *_jit)
{
    jit_inc_synth(ellipsis);
    jit_check_frame();
    if (_jitc->prepare) {
	jit_link_prepare();
	assert(!(_jitc->function->call.call & jit_call_varargs));
	_jitc->function->call.call |= jit_call_varargs;
    }
    else {
	jit_link_prolog();
	assert(!(_jitc->function->self.call & jit_call_varargs));
	_jitc->function->self.call |= jit_call_varargs;
	_jitc->function->vagp = _jitc->function->self.argi;
    }
    jit_dec_synth();
}

void
_jit_va_push(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(va_push, u);
    jit_pushargr(u);
    jit_dec_synth();
}

jit_node_t *
_jit_arg(jit_state_t *_jit, jit_code_t code)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    assert(!(_jitc->function->self.call & jit_call_varargs));
#if STRONG_TYPE_CHECKING
    assert(code >= jit_code_arg_c && code <= jit_code_arg);
#endif
    if (jit_arg_reg_p(_jitc->function->self.argi))
	offset = _jitc->function->self.argi++;
    else {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    node = jit_new_node_ww(code, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

jit_node_t *
_jit_arg_f(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    assert(!(_jitc->function->self.call & jit_call_varargs));
    if (jit_arg_f_reg_p(_jitc->function->self.argf))
	offset = _jitc->function->self.argf++;
    else if (jit_arg_reg_p(_jitc->function->self.argi)) {
	offset = _jitc->function->self.argi++;
	offset += 8;
    }
    else {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    node = jit_new_node_ww(jit_code_arg_f, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

jit_node_t *
_jit_arg_d(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    assert(!(_jitc->function->self.call & jit_call_varargs));
    if (jit_arg_f_reg_p(_jitc->function->self.argf))
	offset = _jitc->function->self.argf++;
    else if (jit_arg_reg_p(_jitc->function->self.argi)) {
	offset = _jitc->function->self.argi++;
	offset += 8;
    }
    else {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    node = jit_new_node_ww(jit_code_arg_d, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

void
_jit_getarg_c(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_c);
    jit_inc_synth_wp(getarg_c, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_c(u, JIT_RA0 - v->u.w);
    else {
	jit_node_t	*node = jit_ldxi_c(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_getarg_uc(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_c);
    jit_inc_synth_wp(getarg_uc, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_uc(u, JIT_RA0 - v->u.w);
    else {
	jit_node_t	*node = jit_ldxi_uc(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_getarg_s(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_s);
    jit_inc_synth_wp(getarg_s, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_s(u, JIT_RA0 - v->u.w);
    else {
	jit_node_t	*node = jit_ldxi_s(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_getarg_us(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_s);
    jit_inc_synth_wp(getarg_us, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_us(u, JIT_RA0 - v->u.w);
    else {
	jit_node_t	*node = jit_ldxi_us(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_getarg_i(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_i);
    jit_inc_synth_wp(getarg_i, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_i(u, JIT_RA0 - v->u.w);
    else {
	jit_node_t	*node = jit_ldxi_i(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_getarg_ui(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_i);
    jit_inc_synth_wp(getarg_ui, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_ui(u, JIT_RA0 - v->u.w);
    else {
	jit_node_t	*node = jit_ldxi_ui(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_getarg_l(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_l);
    jit_inc_synth_wp(getarg_l, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr(u, JIT_RA0 - v->u.w);
    else {
	jit_node_t	*node = jit_ldxi_l(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_putargr(jit_state_t *_jit, jit_int32_t u, jit_node_t *v, jit_code_t code)
{
    assert_putarg_type(code, v->code);
    jit_code_inc_synth_wp(code, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr(JIT_RA0 - v->u.w, u);
    else {
	jit_node_t	*node = jit_stxi(v->u.w, JIT_FP, u);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_putargi(jit_state_t *_jit, jit_word_t u, jit_node_t *v, jit_code_t code)
{
    jit_int32_t		regno;
    assert_putarg_type(code, v->code);
    jit_code_inc_synth_wp(code, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movi(JIT_RA0 - v->u.w, u);
    else {
	jit_node_t	*node;
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	node = jit_stxi(v->u.w, JIT_FP, regno);
	jit_link_alist(node);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_getarg_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_wp(getarg_f, u, v);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_f(u, JIT_FA0 - v->u.w);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_w_f(u, JIT_RA0 - (v->u.w - 8));
    else {
	jit_node_t	*node = jit_ldxi_f(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_putargr_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_wp(putargr_f, u, v);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_f(JIT_FA0 - v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_f_w(JIT_RA0 - (v->u.w - 8), u);
    else {
	jit_node_t	*node = jit_stxi_f(v->u.w, JIT_FP, u);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_putargi_f(jit_state_t *_jit, jit_float32_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_fp(putargi_f, u, v);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movi_f(JIT_FA0 - v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movi_f_w(JIT_RA0 - (v->u.w - 8), u);
    else {
	jit_node_t	*node;
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
	node = jit_stxi_f(v->u.w, JIT_FP, regno);
	jit_link_alist(node);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_getarg_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(getarg_d, u, v);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_d(u, JIT_FA0 - v->u.w);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_w_d(u, JIT_RA0 - (v->u.w - 8));
    else {
	jit_node_t	*node = jit_ldxi_d(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_putargr_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(putargr_d, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr_d(JIT_FA0 - v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movr_d_w(JIT_RA0 - (v->u.w - 8), u);
    else {
	jit_node_t	*node = jit_stxi_d(v->u.w, JIT_FP, u);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

void
_jit_putargi_d(jit_state_t *_jit, jit_float64_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_dp(putargi_d, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movi_d(JIT_FA0 - v->u.w, u);
    else if (jit_arg_reg_p(v->u.w - 8))
	jit_movi_d_w(JIT_RA0 - (v->u.w - 8), u);
    else {
	jit_node_t	*node;
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
	node = jit_stxi_d(v->u.w, JIT_FP, regno);
	jit_link_alist(node);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_pushargr(jit_state_t *_jit, jit_int32_t u, jit_code_t code)
{
    assert(_jitc->function);
    jit_code_inc_synth_w(code, u);
    jit_link_prepare();
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	jit_stxi(_jitc->function->call.size, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    jit_dec_synth();
}

void
_jit_pushargi(jit_state_t *_jit, jit_word_t u, jit_code_t code)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_code_inc_synth_w(code, u);
    jit_link_prepare();
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(_jitc->function->call.size, JIT_SP, regno);
	jit_unget_reg(regno);
	_jitc->function->call.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    jit_dec_synth();
}

void
_jit_pushargr_f(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr_f, u);
    jit_link_prepare();
    if (jit_arg_f_reg_p(_jitc->function->call.argf) &&
	!(_jitc->function->call.call & jit_call_varargs)) {
	jit_movr_f(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
    else if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr_f_w(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	jit_stxi_f(_jitc->function->call.size, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    jit_dec_synth();
}

void
_jit_pushargi_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_int32_t		regno;
    assert(_jitc->function);
    jit_inc_synth_f(pushargi_f, u);
    jit_link_prepare();
    if (jit_arg_f_reg_p(_jitc->function->call.argf) &&
	!(_jitc->function->call.call & jit_call_varargs)) {
	jit_movi_f(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
    else if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi_f_w(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
 	jit_stxi_f(_jitc->function->call.size, JIT_SP, regno);
	jit_unget_reg(regno);
	_jitc->function->call.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    jit_dec_synth();
}

void
_jit_pushargr_d(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr_d, u);
    jit_link_prepare();
    if (jit_arg_f_reg_p(_jitc->function->call.argf) &&
	!(_jitc->function->call.call & jit_call_varargs)) {
	jit_movr_d(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
    else if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr_d_w(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	jit_stxi_d(_jitc->function->call.size, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    jit_dec_synth();
}

void
_jit_pushargi_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_int32_t		regno;
    assert(_jitc->function);
    jit_inc_synth_d(pushargi_d, u);
    jit_link_prepare();
    if (jit_arg_f_reg_p(_jitc->function->call.argf) &&
	!(_jitc->function->call.call & jit_call_varargs)) {
	jit_movi_d(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
    else if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi_d_w(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
 	jit_stxi_d(_jitc->function->call.size, JIT_SP, regno);
	jit_unget_reg(regno);
	_jitc->function->call.size += sizeof(jit_word_t);
	jit_check_frame();
    }
    jit_dec_synth();
}

jit_bool_t
_jit_regarg_p(jit_state_t *_jit, jit_node_t *node, jit_int32_t regno)
{
    jit_int32_t		spec;
    spec = jit_class(_rvs[regno].spec);
    if (spec & jit_class_arg) {
	regno = JIT_RA0 - regno;
	if (regno >= 0 && regno < node->v.w)
	    return (1);
	if (spec & jit_class_fpr) {
	    regno = JIT_FA0 - regno;
	    if (regno >= 0 && regno < node->w.w)
		return (1);
	}
    }

    return (0);
}

void
_jit_finishr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_node_t		*node;
    assert(_jitc->function);
    jit_check_frame();
    jit_inc_synth_w(finishr, r0);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    node = jit_callr(r0);
    node->v.w = _jitc->function->self.argi;
    node->w.w = _jitc->function->call.argf;
    _jitc->function->call.argi = _jitc->function->call.argf =
	_jitc->function->call.size = 0;
    _jitc->prepare = 0;
    jit_dec_synth();
}

jit_node_t *
_jit_finishi(jit_state_t *_jit, jit_pointer_t i0)
{
    jit_node_t		*node;
    assert(_jitc->function);
    jit_check_frame();
    jit_inc_synth_w(finishi, (jit_word_t)i0);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    node = jit_calli(i0);
    node->v.w = _jitc->function->call.argi;
    node->w.w = _jitc->function->call.argf;
    _jitc->function->call.argi = _jitc->function->call.argf =
	_jitc->function->call.size = 0;
    _jitc->prepare = 0;
    jit_dec_synth();
    return (node);
}

void
_jit_retval_c(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_c, r0);
    jit_extr_c(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_uc(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_uc, r0);
    jit_extr_uc(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_s(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_s, r0);
    jit_extr_s(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_us(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_us, r0);
    jit_extr_us(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_i(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_i, r0);
    jit_extr_i(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_ui(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_ui, r0);
    jit_extr_ui(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_l(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_l, r0);
    if (r0 != JIT_RET)
	jit_movr(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_f(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_f, r0);
    if (r0 != JIT_FRET)
	jit_movr_f(r0, JIT_FRET);
    jit_dec_synth();
}

void
_jit_retval_d(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_d, r0);
    if (r0 != JIT_FRET)
	jit_movr_d(r0, JIT_FRET);
    jit_dec_synth();
}

jit_pointer_t
_emit_code(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_node_t		*temp;
    jit_word_t		 word;
    jit_word_t		 value;
    jit_int32_t		 offset;
    struct {
	jit_node_t	*node;
	jit_uint8_t	*data;
	jit_word_t	 word;
	jit_function_t	 func;
#if DEVEL_DISASSEMBLER
	jit_word_t	 prevw;
#endif
	jit_int32_t	 const_offset;
	jit_int32_t	 patch_offset;
    } undo;
#if DEVEL_DISASSEMBLER
    jit_word_t		 prevw;
#endif

#if __WORDSIZE == 64
    if (!_jitc->consts.hash.table) {
	jit_alloc((jit_pointer_t *)&_jitc->consts.hash.table,
		  16 * sizeof(jit_const_t *));
	_jitc->consts.hash.size = 16;
	jit_alloc((jit_pointer_t *)&_jitc->consts.pool.ptr,
		  sizeof(jit_const_t *));
	jit_alloc((jit_pointer_t *)_jitc->consts.pool.ptr,
		  1024 * sizeof(jit_const_t));
	_jitc->consts.pool.length = 1;
    }
    /* Reset table if starting over jit generation */
    else
	memset(_jitc->consts.hash.table, 0,
	       _jitc->consts.hash.size * sizeof(jit_word_t));
    for (offset = 0; offset < _jitc->consts.pool.length; offset++) {
	jit_int32_t	 i;
	jit_const_t	*list = _jitc->consts.pool.ptr[offset];
	for (i = 0; i < 1023; ++i, ++list)
	    list->next = list + 1;
	if (offset + 1 < _jitc->consts.pool.length)
	    list->next = _jitc->consts.pool.ptr[offset + 1];
	else
	    list->next = NULL;
    }
    _jitc->consts.pool.list = _jitc->consts.pool.ptr[0];
    _jitc->consts.hash.count = 0;
    if (!_jitc->consts.vector.instrs) {
	jit_alloc((jit_pointer_t *)&_jitc->consts.vector.instrs,
		  16 * sizeof(jit_word_t));
	jit_alloc((jit_pointer_t *)&_jitc->consts.vector.values,
		  16 * sizeof(jit_word_t));
	_jitc->consts.vector.length = 16;
    }
    _jitc->consts.vector.offset = 0;
#endif

    _jitc->function = NULL;

    jit_reglive_setup();

    undo.word = 0;
    undo.node = NULL;
    undo.const_offset = undo.patch_offset = 0;
#  define assert_data(node)		/**/
#define case_rr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w), rn(node->v.w));		\
		break
#define case_rw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w), node->v.w);		\
		break
#define case_wr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w, rn(node->v.w));		\
		break
#define case_rrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w),				\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_rrrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_rrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w), rn(node->v.w), node->w.w);	\
		break
#define case_rrrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), node->w.w);		\
		break
#define case_rrf(name)							\
	    case jit_code_##name##i_f:					\
		assert_data(node);					\
		name##i_f(rn(node->u.w), rn(node->v.w), node->w.f);	\
		break
#define case_rrd(name)							\
	    case jit_code_##name##i_d:					\
		assert_data(node);					\
		name##i_d(rn(node->u.w), rn(node->v.w), node->w.d);	\
		break
#define case_wrr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w, rn(node->v.w), rn(node->w.w));	\
		break
#define case_brr(name, type)						\
	    case jit_code_##name##r##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##r##type(temp->u.w, rn(node->v.w),		\
				  rn(node->w.w));			\
		else {							\
		    word = name##r##type(_jit->pc.w,			\
					 rn(node->v.w), rn(node->w.w));	\
		    patch(word, node);					\
		}							\
		break
#define case_brw(name, type)						\
	    case jit_code_##name##i##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i##type(temp->u.w,				\
				  rn(node->v.w), node->w.w);		\
		else {							\
		    word = name##i##type(_jit->pc.w,			\
					 rn(node->v.w), node->w.w);	\
		    patch(word, node);					\
		}							\
		break;
#define case_brf(name)							\
	    case jit_code_##name##i_f:					\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i_f(temp->u.w, rn(node->v.w), node->w.f);	\
		else {							\
		    word = name##i_f(_jit->pc.w, rn(node->v.w),		\
				node->w.f);				\
		    patch(word, node);					\
		}							\
		break
#define case_brd(name)							\
	    case jit_code_##name##i_d:					\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i_d(temp->u.w, rn(node->v.w), node->w.d);	\
		else {							\
		    word = name##i_d(_jit->pc.w, rn(node->v.w),		\
				node->w.d);				\
		    patch(word, node);					\
		}							\
		break
#if DEVEL_DISASSEMBLER
    prevw = _jit->pc.w;
#endif
    for (node = _jitc->head; node; node = node->next) {
	if (_jit->pc.uc >= _jitc->code.end)
	    return (NULL);

#if DEVEL_DISASSEMBLER
	node->offset = (jit_uword_t)_jit->pc.w - (jit_uword_t)prevw;
	prevw = _jit->pc.w;
#endif
	value = jit_classify(node->code);
	jit_regarg_set(node, value);
	switch (node->code) {
	    case jit_code_align:
		/* Must align to a power of two */
		assert(!(node->u.w & (node->u.w - 1)));
		if ((word = _jit->pc.w & (node->u.w - 1)))
		    nop(node->u.w - word);
		break;
	    case jit_code_skip:
	        nop((node->u.w + 3) & ~3);
		break;
	    case jit_code_note:		case jit_code_name:
		node->u.w = _jit->pc.w;
		break;
	    case jit_code_label:
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		break;
		case_rrr(add,);
		case_rrw(add,);
		case_rrr(addc,);
		case_rrw(addc,);
		case_rrr(addx,);
		case_rrw(addx,);
		case_rrr(sub,);
		case_rrw(sub,);
		case_rrr(subc,);
		case_rrw(subc,);
		case_rrr(subx,);
		case_rrw(subx,);
		case_rrw(rsb,);
		case_rrr(mul,);
		case_rrw(mul,);
		case_rrrr(qmul,);
		case_rrrw(qmul,);
		case_rrrr(qmul, _u);
		case_rrrw(qmul, _u);
		case_rrr(div,);
		case_rrw(div,);
		case_rrr(div, _u);
		case_rrw(div, _u);
		case_rrrr(qdiv,);
		case_rrrw(qdiv,);
		case_rrrr(qdiv, _u);
		case_rrrw(qdiv, _u);
		case_rrr(rem,);
		case_rrw(rem,);
		case_rrr(rem, _u);
		case_rrw(rem, _u);
		case_rrr(lsh,);
		case_rrw(lsh,);
		case_rrr(rsh,);
		case_rrw(rsh,);
		case_rrr(rsh, _u);
		case_rrw(rsh, _u);
		case_rr(neg,);
		case_rr(com,);
#define clor(r0, r1)	fallback_clo(r0, r1)
#define clzr(r0, r1)	fallback_clz(r0, r1)
#define ctor(r0, r1)	fallback_cto(r0, r1)
#define ctzr(r0, r1)	fallback_ctz(r0, r1)
		case_rr(clo,);
		case_rr(clz,);
		case_rr(cto,);
		case_rr(ctz,);
		case_rrr(and,);
		case_rrw(and,);
		case_rrr(or,);
		case_rrw(or,);
		case_rrr(xor,);
		case_rrw(xor,);
		case_rr(trunc, _f_i);
		case_rr(trunc, _d_i);
		case_rr(trunc, _f_l);
		case_rr(trunc, _d_l);
		case_rr(ld, _c);
		case_rw(ld, _c);
		case_rr(ld, _uc);
		case_rw(ld, _uc);
		case_rr(ld, _s);
		case_rw(ld, _s);
		case_rr(ld, _us);
		case_rw(ld, _us);
		case_rr(ld, _i);
		case_rw(ld, _i);
		case_rr(ld, _ui);
		case_rw(ld, _ui);
		case_rr(ld, _l);
		case_rw(ld, _l);
		case_rrr(ldx, _c);
		case_rrw(ldx, _c);
		case_rrr(ldx, _uc);
		case_rrw(ldx, _uc);
		case_rrr(ldx, _s);
		case_rrw(ldx, _s);
		case_rrr(ldx, _us);
		case_rrw(ldx, _us);
		case_rrr(ldx, _i);
		case_rrw(ldx, _i);
		case_rrr(ldx, _ui);
		case_rrw(ldx, _ui);
		case_rrr(ldx, _l);
		case_rrw(ldx, _l);
		case_rr(st, _c);
		case_wr(st, _c);
		case_rr(st, _s);
		case_wr(st, _s);
		case_rr(st, _i);
		case_wr(st, _i);
		case_rr(st, _l);
		case_wr(st, _l);
		case_rrr(stx, _c);
		case_wrr(stx, _c);
		case_rrr(stx, _s);
		case_wrr(stx, _s);
		case_rrr(stx, _i);
		case_wrr(stx, _i);
		case_rrr(stx, _l);
		case_wrr(stx, _l);
		case_rr(hton, _us);
		case_rr(hton, _ui);
		case_rr(hton, _ul);
		case_rr(bswap, _us);
		case_rr(bswap, _ui);
		case_rr(bswap, _ul);
		case_rr(ext, _c);
		case_rr(ext, _uc);
		case_rr(ext, _s);
		case_rr(ext, _us);
		case_rr(ext, _i);
		case_rr(ext, _ui);
	    case jit_code_casr:
		casr(rn(node->u.w), rn(node->v.w),
		     rn(node->w.q.l), rn(node->w.q.h));
		break;
	    case jit_code_casi:
		casi(rn(node->u.w), node->v.w,
		     rn(node->w.q.l), rn(node->w.q.h));
		break;
		case_rrr(movn,);
		case_rrr(movz,);
		case_rr(mov,);
	    case jit_code_movi:
		if (node->flag & jit_flag_node) {
		    temp = node->v.n;
		    if (temp->code == jit_code_data ||
			(temp->code == jit_code_label &&
			 (temp->flag & jit_flag_patch)))
			movi(rn(node->u.w), temp->u.w);
		    else {
			assert(temp->code == jit_code_label ||
			       temp->code == jit_code_epilog);
			word = movi_p(rn(node->u.w), temp->u.w);
			patch(word, node);
		    }
		}
		else
		    movi(rn(node->u.w), node->v.w);
		break;
		case_rrr(lt,);
		case_rrw(lt,);
		case_rrr(lt, _u);
		case_rrw(lt, _u);
		case_rrr(le,);
		case_rrw(le,);
		case_rrr(le, _u);
		case_rrw(le, _u);
		case_rrr(eq,);
		case_rrw(eq,);
		case_rrr(ge,);
		case_rrw(ge,);
		case_rrr(ge, _u);
		case_rrw(ge, _u);
		case_rrr(gt,);
		case_rrw(gt,);
		case_rrr(gt, _u);
		case_rrw(gt, _u);
		case_rrr(ne,);
		case_rrw(ne,);
		case_brr(blt,);
		case_brw(blt,);
		case_brr(blt, _u);
		case_brw(blt, _u);
		case_brr(ble,);
		case_brw(ble,);
		case_brr(ble, _u);
		case_brw(ble, _u);
		case_brr(beq,);
		case_brw(beq,);
		case_brr(bge,);
		case_brw(bge,);
		case_brr(bge, _u);
		case_brw(bge, _u);
		case_brr(bgt,);
		case_brw(bgt,);
		case_brr(bgt, _u);
		case_brw(bgt, _u);
		case_brr(bne,);
		case_brw(bne,);
		case_brr(boadd,);
		case_brw(boadd,);
		case_brr(boadd, _u);
		case_brw(boadd, _u);
		case_brr(bxadd,);
		case_brw(bxadd,);
		case_brr(bxadd, _u);
		case_brw(bxadd, _u);
		case_brr(bosub,);
		case_brw(bosub,);
		case_brr(bosub, _u);
		case_brw(bosub, _u);
		case_brr(bxsub,);
		case_brw(bxsub,);
		case_brr(bxsub, _u);
		case_brw(bxsub, _u);
		case_brr(bms,);
		case_brw(bms,);
		case_brr(bmc,);
		case_brw(bmc,);
		case_rrr(add, _f);
		case_rrf(add);
		case_rrr(sub, _f);
		case_rrf(sub);
		case_rrf(rsb);
		case_rrr(mul, _f);
		case_rrf(mul);
		case_rrr(div, _f);
		case_rrf(div);
		case_rr(abs, _f);
		case_rr(neg, _f);
		case_rr(sqrt, _f);
		case_rr(ext, _f);
		case_rr(ld, _f);
		case_rw(ld, _f);
		case_rrr(ldx, _f);
		case_rrw(ldx, _f);
		case_rr(st, _f);
		case_wr(st, _f);
		case_rrr(stx, _f);
		case_wrr(stx, _f);
		case_rr(mov, _f);
	    case jit_code_movi_f:
		assert_data(node);
		movi_f(rn(node->u.w), node->v.f);
		break;
		case_rr(ext, _d_f);
		case_rrr(lt, _f);
		case_rrf(lt);
		case_rrr(le, _f);
		case_rrf(le);
		case_rrr(eq, _f);
		case_rrf(eq);
		case_rrr(ge, _f);
		case_rrf(ge);
		case_rrr(gt, _f);
		case_rrf(gt);
		case_rrr(ne, _f);
		case_rrf(ne);
		case_rrr(unlt, _f);
		case_rrf(unlt);
		case_rrr(unle, _f);
		case_rrf(unle);
		case_rrr(uneq, _f);
		case_rrf(uneq);
		case_rrr(unge, _f);
		case_rrf(unge);
		case_rrr(ungt, _f);
		case_rrf(ungt);
		case_rrr(ltgt, _f);
		case_rrf(ltgt);
		case_rrr(ord, _f);
		case_rrf(ord);
		case_rrr(unord, _f);
		case_rrf(unord);
		case_brr(blt, _f);
		case_brf(blt);
		case_brr(ble, _f);
		case_brf(ble);
		case_brr(beq, _f);
		case_brf(beq);
		case_brr(bge, _f);
		case_brf(bge);
		case_brr(bgt, _f);
		case_brf(bgt);
		case_brr(bne, _f);
		case_brf(bne);
		case_brr(bunlt, _f);
		case_brf(bunlt);
		case_brr(bunle, _f);
		case_brf(bunle);
		case_brr(buneq, _f);
		case_brf(buneq);
		case_brr(bunge, _f);
		case_brf(bunge);
		case_brr(bungt, _f);
		case_brf(bungt);
		case_brr(bltgt, _f);
		case_brf(bltgt);
		case_brr(bord, _f);
		case_brf(bord);
		case_brr(bunord, _f);
		case_brf(bunord);
		case_rrr(add, _d);
		case_rrd(add);
		case_rrr(sub, _d);
		case_rrd(sub);
		case_rrd(rsb);
		case_rrr(mul, _d);
		case_rrd(mul);
		case_rrr(div, _d);
		case_rrd(div);
		case_rr(abs, _d);
		case_rr(neg, _d);
		case_rr(sqrt, _d);
		case_rr(ext, _d);
		case_rr(ld, _d);
		case_rw(ld, _d);
		case_rrr(ldx, _d);
		case_rrw(ldx, _d);
		case_rr(st, _d);
		case_wr(st, _d);
		case_rrr(stx, _d);
		case_wrr(stx, _d);
		case_rr(mov, _d);
	    case jit_code_movi_d:
		assert_data(node);
		movi_d(rn(node->u.w), node->v.d);
		break;
		case_rr(ext, _f_d);
		case_rrr(lt, _d);
		case_rrd(lt);
		case_rrr(le, _d);
		case_rrd(le);
		case_rrr(eq, _d);
		case_rrd(eq);
		case_rrr(ge, _d);
		case_rrd(ge);
		case_rrr(gt, _d);
		case_rrd(gt);
		case_rrr(ne, _d);
		case_rrd(ne);
		case_rrr(unlt, _d);
		case_rrd(unlt);
		case_rrr(unle, _d);
		case_rrd(unle);
		case_rrr(uneq, _d);
		case_rrd(uneq);
		case_rrr(unge, _d);
		case_rrd(unge);
		case_rrr(ungt, _d);
		case_rrd(ungt);
		case_rrr(ltgt, _d);
		case_rrd(ltgt);
		case_rrr(ord, _d);
		case_rrd(ord);
		case_rrr(unord, _d);
		case_rrd(unord);
		case_brr(blt, _d);
		case_brd(blt);
		case_brr(ble, _d);
		case_brd(ble);
		case_brr(beq, _d);
		case_brd(beq);
		case_brr(bge, _d);
		case_brd(bge);
		case_brr(bgt, _d);
		case_brd(bgt);
		case_brr(bne, _d);
		case_brd(bne);
		case_brr(bunlt, _d);
		case_brd(bunlt);
		case_brr(bunle, _d);
		case_brd(bunle);
		case_brr(buneq, _d);
		case_brd(buneq);
		case_brr(bunge, _d);
		case_brd(bunge);
		case_brr(bungt, _d);
		case_brd(bungt);
		case_brr(bltgt, _d);
		case_brd(bltgt);
		case_brr(bord, _d);
		case_brd(bord);
		case_brr(bunord, _d);
		case_brd(bunord);
	    case jit_code_jmpr:
		jit_check_frame();
		jmpr(rn(node->u.w));
		break;
	    case jit_code_jmpi:
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    if (temp->flag & jit_flag_patch)
			jmpi(temp->u.w);
		    else {
			word = _jit->code.length -
			    (_jit->pc.uc - _jit->code.ptr);
			if (simm20_p(word))
			    word = jmpi(_jit->pc.w);
			else
			word = jmpi_p(_jit->pc.w);
			patch(word, node);
		    }
		}
		else {
		    jit_check_frame();
		    jmpi(node->u.w);
		}
		break;
	    case jit_code_callr:
		jit_check_frame();
		callr(rn(node->u.w));
		break;
	    case jit_code_calli:
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    if (temp->flag & jit_flag_patch)
			calli(temp->u.w);
		    else {
			word = _jit->code.length -
			    (_jit->pc.uc - _jit->code.ptr);
			if (simm20_p(word))
			    word = calli(_jit->pc.w);
			else
			    word = calli_p(_jit->pc.w);
			patch(word, node);
		    }
		}
		else {
		    jit_check_frame();
		    calli(node->u.w);
		}
		break;
	    case jit_code_prolog:
		_jitc->function = _jitc->functions.ptr + node->w.w;
		undo.node = node;
		undo.word = _jit->pc.w;
		memcpy(&undo.func, _jitc->function, sizeof(undo.func));
#if DEVEL_DISASSEMBLER
		undo.prevw = prevw;
#endif
		undo.patch_offset = _jitc->patches.offset;
	    restart_function:
		compute_framesize();
		patch_alist(0);
		_jitc->again = 0;
		prolog(node);
		break;
	    case jit_code_epilog:
		assert(_jitc->function == _jitc->functions.ptr + node->w.w);
		if (_jitc->again) {
		    for (temp = undo.node->next;
			 temp != node; temp = temp->next) {
			if (temp->code == jit_code_label ||
			    temp->code == jit_code_epilog)
			    temp->flag &= ~jit_flag_patch;
		    }
		    temp->flag &= ~jit_flag_patch;
		    node = undo.node;
		    _jit->pc.w = undo.word;
		    /* undo.func.self.aoff and undo.func.regset should not
		     * be undone, as they will be further updated, and are
		     * the reason of the undo. */
		    undo.func.self.aoff = _jitc->function->frame +
			_jitc->function->self.aoff;
		    undo.func.need_frame = _jitc->function->need_frame;
		    jit_regset_set(&undo.func.regset, &_jitc->function->regset);
		    /* allocar information also does not need to be undone */
		    undo.func.aoffoff = _jitc->function->aoffoff;
		    undo.func.allocar = _jitc->function->allocar;
		    /* this will be recomputed but undo anyway to have it
		     * better self documented.*/
		    undo.func.need_stack = _jitc->function->need_stack;
		    memcpy(_jitc->function, &undo.func, sizeof(undo.func));
#if DEVEL_DISASSEMBLER
		    prevw = undo.prevw;
#endif
		    _jitc->patches.offset = undo.patch_offset;
		    patch_alist(1);
		    goto restart_function;
		}
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		epilog(node);
		_jitc->function = NULL;
		break;
	    case jit_code_movr_w_f:
		movr_w_f(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movr_f_w:
		movr_f_w(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movi_f_w:
		assert_data(node);
		movi_f_w(rn(node->u.w), node->v.f);
		break;
	    case jit_code_movr_w_d:
		movr_w_d(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movr_d_w:
		movr_d_w(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_movi_d_w:
		assert_data(node);
		movi_d_w(rn(node->u.w), node->v.d);
		break;
	    case jit_code_va_start:
		vastart(rn(node->u.w));
		break;
	    case jit_code_va_arg:
		vaarg(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_va_arg_d:
		vaarg_d(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_live:			case jit_code_ellipsis:
	    case jit_code_va_push:
	    case jit_code_allocai:		case jit_code_allocar:
	    case jit_code_arg_c:		case jit_code_arg_s:
	    case jit_code_arg_i:
	    case jit_code_arg_l:
	    case jit_code_arg_f:		case jit_code_arg_d:
	    case jit_code_va_end:
	    case jit_code_ret:
	    case jit_code_retr_c:		case jit_code_reti_c:
	    case jit_code_retr_uc:		case jit_code_reti_uc:
	    case jit_code_retr_s:		case jit_code_reti_s:
	    case jit_code_retr_us:		case jit_code_reti_us:
	    case jit_code_retr_i:		case jit_code_reti_i:
	    case jit_code_retr_ui:		case jit_code_reti_ui:
	    case jit_code_retr_l:		case jit_code_reti_l:
	    case jit_code_retr_f:		case jit_code_reti_f:
	    case jit_code_retr_d:		case jit_code_reti_d:
	    case jit_code_getarg_c:		case jit_code_getarg_uc:
	    case jit_code_getarg_s:		case jit_code_getarg_us:
	    case jit_code_getarg_i:		case jit_code_getarg_ui:
	    case jit_code_getarg_l:
	    case jit_code_getarg_f:		case jit_code_getarg_d:
	    case jit_code_putargr_c:		case jit_code_putargi_c:
	    case jit_code_putargr_uc:		case jit_code_putargi_uc:
	    case jit_code_putargr_s:		case jit_code_putargi_s:
	    case jit_code_putargr_us:		case jit_code_putargi_us:
	    case jit_code_putargr_i:		case jit_code_putargi_i:
	    case jit_code_putargr_ui:		case jit_code_putargi_ui:
	    case jit_code_putargr_l:		case jit_code_putargi_l:
	    case jit_code_putargr_f:		case jit_code_putargi_f:
	    case jit_code_putargr_d:		case jit_code_putargi_d:
	    case jit_code_pushargr_c:		case jit_code_pushargi_c:
	    case jit_code_pushargr_uc:		case jit_code_pushargi_uc:
	    case jit_code_pushargr_s:		case jit_code_pushargi_s:
	    case jit_code_pushargr_us:		case jit_code_pushargi_us:
	    case jit_code_pushargr_i:		case jit_code_pushargi_i:
	    case jit_code_pushargr_ui:		case jit_code_pushargi_ui:
	    case jit_code_pushargr_l:		case jit_code_pushargi_l:
	    case jit_code_pushargr_f:		case jit_code_pushargi_f:
	    case jit_code_pushargr_d:		case jit_code_pushargi_d:
	    case jit_code_retval_c:		case jit_code_retval_uc:
	    case jit_code_retval_s:		case jit_code_retval_us:
	    case jit_code_retval_i:
	    case jit_code_retval_ui:		case jit_code_retval_l:
	    case jit_code_retval_f:		case jit_code_retval_d:
	    case jit_code_prepare:
	    case jit_code_finishr:		case jit_code_finishi:
		break;
	    default:
		abort();
	}
	if (jit_carry != _NOREG) {
	    switch (node->code) {
		case jit_code_note:
		case jit_code_addcr:		case jit_code_addci:
		case jit_code_addxr:		case jit_code_addxi:
		case jit_code_subcr:		case jit_code_subci:
		case jit_code_subxr:		case jit_code_subxi:
		    break;
		default:
		    jit_unget_reg(jit_carry);
		    jit_carry = _NOREG;
		    break;
	    }
	}
	jit_regarg_clr(node, value);
	assert(_jitc->regarg == 0 ||
	       (jit_carry != _NOREG && _jitc->regarg == (1 << jit_carry)));
	assert(_jitc->synth == 0);
	/* update register live state */
	jit_reglive(node);
    }
#undef case_brw
#undef case_brr
#undef case_wrr
#undef case_rrw
#undef case_rrr
#undef case_wr
#undef case_rw
#undef case_rr

#if __WORDSIZE == 64
    /* Record all constants to be patched */
    for (offset = 0; offset < _jitc->patches.offset; offset++) {
	node = _jitc->patches.ptr[offset].node;
	value = node->code == jit_code_movi ? node->v.n->u.w : node->u.n->u.w;
	put_const(value);
    }
    /* Record all direct constants */
    for (offset = 0; offset < _jitc->consts.vector.offset; offset++)
	put_const(_jitc->consts.vector.values[offset]);
    /* Now actually inject constants at the end of code buffer */
    if (_jitc->consts.hash.count) {
	jit_const_t	*entry;
	/* Insert nop if aligned at 4 bytes */
	if (_jit->pc.w % sizeof(jit_word_t))
	    nop(_jit->pc.w % sizeof(jit_word_t));
	for (offset = 0; offset < _jitc->consts.hash.size; offset++) {
	    entry = _jitc->consts.hash.table[offset];
	    for (; entry; entry = entry->next) {
		/* Make sure to not write out of bounds */
		if (_jit->pc.uc >= _jitc->code.end)
		    return (NULL);
		entry->address = _jit->pc.w;
		*_jit->pc.ul++ = entry->value;
	    }
	}
    }
#endif

    for (offset = 0; offset < _jitc->patches.offset; offset++) {
	node = _jitc->patches.ptr[offset].node;
	word = _jitc->patches.ptr[offset].inst;
	value = node->code == jit_code_movi ? node->v.n->u.w : node->u.n->u.w;
	patch_at(word, value);
    }

#if __WORDSIZE == 64
    /* Patch direct complex constants */
    if (_jitc->consts.vector.instrs) {
	for (offset = 0; offset < _jitc->consts.vector.offset; offset++)
	    patch_at(_jitc->consts.vector.instrs[offset],
		     _jitc->consts.vector.values[offset]);
	jit_free((jit_pointer_t *)&_jitc->consts.vector.instrs);
	jit_free((jit_pointer_t *)&_jitc->consts.vector.values);
    }

    /* Hash table no longer need */
    if (_jitc->consts.hash.table) {
	jit_free((jit_pointer_t *)&_jitc->consts.hash.table);
	for (offset = 0; offset < _jitc->consts.pool.length; offset++)
	    jit_free((jit_pointer_t *)_jitc->consts.pool.ptr + offset);
	jit_free((jit_pointer_t *)&_jitc->consts.pool.ptr);
    }
#endif

    jit_flush(_jit->code.ptr, _jit->pc.uc);

    return (_jit->code.ptr);
}

#define CODE				1
#  include "jit_riscv-cpu.c"
#  include "jit_riscv-fpu.c"
#  include "jit_fallback.c"
#undef CODE

static void
_load_const(jit_state_t *_jit, jit_int32_t reg, jit_word_t value)
{
    if (_jitc->consts.vector.offset >= _jitc->consts.vector.length) {
	jit_word_t	new_size = _jitc->consts.vector.length *
				   2 * sizeof(jit_word_t);
	jit_realloc((jit_pointer_t *)&_jitc->consts.vector.instrs,
		    _jitc->consts.vector.length * sizeof(jit_word_t), new_size);
	jit_realloc((jit_pointer_t *)&_jitc->consts.vector.values,
		    _jitc->consts.vector.length * sizeof(jit_word_t), new_size);
	_jitc->consts.vector.length *= 2;
    }
    _jitc->consts.vector.instrs[_jitc->consts.vector.offset] = _jit->pc.w;
    _jitc->consts.vector.values[_jitc->consts.vector.offset] = value;
    ++_jitc->consts.vector.offset;
    /* Resolve later the pc relative address */
    put_const(value);
    AUIPC(reg, 0);
    ADDI(reg, reg, 0);
    LD(reg, reg, 0);
}

static jit_word_t
hash_const(jit_word_t value)
{
    const jit_uint8_t	*ptr;
    jit_word_t		 i, key;
    for (i = key = 0, ptr = (jit_uint8_t *)&value; i < 4; ++i)
	key = (key << (key & 1)) ^ ptr[i];
    return (key);

}

static void
_put_const(jit_state_t *_jit, jit_word_t value)
{
    jit_word_t		 key;
    jit_const_t		*entry;

    /* Check if already inserted in table */
    key = hash_const(value) % _jitc->consts.hash.size;
    for (entry = _jitc->consts.hash.table[key]; entry; entry = entry->next) {
	if (entry->value == value)
	    return;
    }

    /* Check if need to increase pool size */
    if (_jitc->consts.pool.list->next == NULL) {
	jit_const_t	*list;
	jit_word_t	 offset;
	jit_word_t	 new_size = (_jitc->consts.pool.length + 1) *
				    sizeof(jit_const_t*);
	jit_realloc((jit_pointer_t *)&_jitc->consts.pool.ptr,
		    _jitc->consts.pool.length * sizeof(jit_const_t*), new_size);
	jit_alloc((jit_pointer_t *)
		  _jitc->consts.pool.ptr + _jitc->consts.pool.length,
		  1024 * sizeof(jit_const_t));
	list = _jitc->consts.pool.ptr[_jitc->consts.pool.length];
	_jitc->consts.pool.list->next = list;
	for (offset = 0; offset < 1023; ++offset, ++list)
	    list->next = list + 1;
	list->next = NULL;
	++_jitc->consts.pool.length;
    }

    /* Rehash if more than 75% used table */
    if (_jitc->consts.hash.count > (_jitc->consts.hash.size / 4) * 3) {
	jit_word_t	  i, k;
	jit_const_t	 *next;
	jit_const_t	**table;
	jit_alloc((jit_pointer_t *)&table,
		  _jitc->consts.hash.size * 2 * sizeof(jit_const_t *));
	for (i = 0; i < _jitc->consts.hash.size; ++i) {
	    for (entry = _jitc->consts.hash.table[i]; entry; entry = next) {
		next = entry->next;
		k = hash_const(entry->value) % (_jitc->consts.hash.size * 2);
		entry->next = table[k];
		table[k] = entry;
	    }
	}
	jit_free((jit_pointer_t *)&_jitc->consts.hash.table);
	_jitc->consts.hash.size *= 2;
	_jitc->consts.hash.table = table;
    }

    /* Insert in hash */
    entry = _jitc->consts.pool.list;
    _jitc->consts.pool.list =  entry->next;
    ++_jitc->consts.hash.count;
    entry->value = value;
    entry->next = _jitc->consts.hash.table[key];
    _jitc->consts.hash.table[key] = entry;
}

static jit_word_t
_get_const(jit_state_t *_jit, jit_word_t value)
{
    jit_word_t		 key;
    jit_const_t		*entry;
    key = hash_const(value) % _jitc->consts.hash.size;
    for (entry = _jitc->consts.hash.table[key]; entry; entry = entry->next) {
	if (entry->value == value)
	    return (entry->address);
    }
    /* Only the final patch should call get_const() */
    abort();
}

void
jit_flush(void *fptr, void *tptr)
{
#if defined(__GNUC__)
    jit_word_t		f, t, s;

    s = sysconf(_SC_PAGE_SIZE);
    f = (jit_word_t)fptr & -s;
    t = (((jit_word_t)tptr) + s - 1) & -s;
    __clear_cache((void *)f, (void *)t);
#endif
}

void
_emit_ldxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    ldxi(rn(r0), rn(r1), i0);
}

void
_emit_stxi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    stxi(i0, rn(r0), rn(r1));
}

void
_emit_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    ldxi_d(rn(r0), rn(r1), i0);
}

void
_emit_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    stxi_d(i0, rn(r0), rn(r1));
}

#if __WORDSIZE != 64
# error "only 64 bit ports tested"
#endif
static void
_compute_framesize(jit_state_t *_jit)
{
    jit_int32_t		reg;
    _jitc->framesize = 16;	/* ra+fp */
    for (reg = 0; reg < jit_size(iregs); reg++)
	if (jit_regset_tstbit(&_jitc->function->regset, iregs[reg]))
	    _jitc->framesize += sizeof(jit_word_t);

    for (reg = 0; reg < jit_size(fregs); reg++)
	if (jit_regset_tstbit(&_jitc->function->regset, fregs[reg]))
	    _jitc->framesize += sizeof(jit_float64_t);

    /* Space to store variadic arguments */
    if (_jitc->function->self.call & jit_call_varargs)
	_jitc->framesize += (8 - _jitc->function->vagp) * 8;

    /* Make sure functions called have a 16 byte aligned stack */
    _jitc->framesize = (_jitc->framesize + 15) & -16;
}

static void
_patch(jit_state_t *_jit, jit_word_t instr, jit_node_t *node)
{
    jit_int32_t		 flag;

    assert(node->flag & jit_flag_node);
    if (node->code == jit_code_movi)
	flag = node->v.n->flag;
    else
	flag = node->u.n->flag;
    assert(!(flag & jit_flag_patch));
    if (_jitc->patches.offset >= _jitc->patches.length) {
	jit_realloc((jit_pointer_t *)&_jitc->patches.ptr,
		    _jitc->patches.length * sizeof(jit_patch_t),
		    (_jitc->patches.length + 1024) * sizeof(jit_patch_t));
	_jitc->patches.length += 1024;
    }
    _jitc->patches.ptr[_jitc->patches.offset].inst = instr;
    _jitc->patches.ptr[_jitc->patches.offset].node = node;
    ++_jitc->patches.offset;
}
