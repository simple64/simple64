/*
 * Copyright (C) 2013-2023  Free Software Foundation, Inc.
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

/* callee save
 * align16(lr+fp+x19+x2[0-8]+v8+v9+v1[0-15]) */
#define stack_framesize			160

#define jit_arg_reg_p(i)		((i) >= 0 && (i) < 8)
#define jit_arg_f_reg_p(i)		((i) >= 0 && (i) < 8)

#if __APPLE__
typedef jit_pointer_t jit_va_list_t;
#else
typedef struct jit_qreg {
    jit_float64_t	l;
    jit_float64_t	h;
} jit_qreg_t;

#define va_gp_top_offset		offsetof(jit_va_list_t, q0)
#define va_fp_top_offset		sizeof(jit_va_list_t)
typedef struct jit_va_list {
    jit_pointer_t	stack;
    jit_pointer_t	gptop;
    jit_pointer_t	fptop;
    jit_int32_t		gpoff;
    jit_int32_t		fpoff;

    jit_int64_t		x0;
    jit_int64_t		x1;
    jit_int64_t		x2;
    jit_int64_t		x3;
    jit_int64_t		x4;
    jit_int64_t		x5;
    jit_int64_t		x6;
    jit_int64_t		x7;

    jit_qreg_t		q0;
    jit_qreg_t		q1;
    jit_qreg_t		q2;
    jit_qreg_t		q3;
    jit_qreg_t		q4;
    jit_qreg_t		q5;
    jit_qreg_t		q6;
    jit_qreg_t		q7;
} jit_va_list_t;
#endif

/*
 * Prototypes
 */
#define compute_framesize()		_compute_framesize(_jit)
static void _compute_framesize(jit_state_t*);
#define patch(instr, node)		_patch(_jit, instr, node)
static void _patch(jit_state_t*,jit_word_t,jit_node_t*);

/* libgcc */
extern void __clear_cache(void *, void *);

#define PROTO				1
#  include "jit_aarch64-cpu.c"
#  include "jit_aarch64-fpu.c"
#undef PROTO

/*
 * Initialization
 */
jit_register_t		_rvs[] = {
    { rc(gpr) | 0x08,			"x8" },
#if __APPLE__
    { 0x12,				"x18" },
#else
    { rc(gpr) | 0x12,			"x18" },
#endif
    { rc(gpr) | 0x11,			"x17" },
    { rc(gpr) | 0x10,			"x16" },
    { rc(gpr) | 0x09,			"x9" },
    { rc(gpr) | 0x0a,			"x10" },
    { rc(gpr) | 0x0b,			"x11" },
    { rc(gpr) | 0x0c,			"x12" },
    { rc(gpr) | 0x0d,			"x13" },
    { rc(gpr) | 0x0e,			"x14" },
    { rc(gpr) | 0x0f,			"x15" },
    { rc(sav) | rc(gpr) | 0x13,		"x19" },
    { rc(sav) | rc(gpr) | 0x14,		"x20" },
    { rc(sav) | rc(gpr) | 0x15,		"x21" },
    { rc(sav) | rc(gpr) | 0x16,		"x22" },
    { rc(sav) | rc(gpr) | 0x17,		"x23" },
    { rc(sav) | rc(gpr) | 0x18,		"x24" },
    { rc(sav) | rc(gpr) | 0x19,		"x25" },
    { rc(sav) | rc(gpr) | 0x1a,		"x26" },
    { rc(sav) | rc(gpr) | 0x1b,		"x27" },
    { rc(sav) | rc(gpr) | 0x1c,		"x28" },
    { 0x1f,				"sp" },
    { 0x1e,				"lr" },
    { 0x1d,				"fp" },
    { rc(arg) | rc(gpr) | 0x07,		"x7" },
    { rc(arg) | rc(gpr) | 0x06,		"x6" },
    { rc(arg) | rc(gpr) | 0x05,		"x5" },
    { rc(arg) | rc(gpr) | 0x04,		"x4" },
    { rc(arg) | rc(gpr) | 0x03,		"x3" },
    { rc(arg) | rc(gpr) | 0x02,		"x2" },
    { rc(arg) | rc(gpr) | 0x01,		"x1" },
    { rc(arg) | rc(gpr) | 0x00,		"x0" },
    { rc(fpr) | 0x1f,			"v31" },
    { rc(fpr) | 0x1e,			"v30" },
    { rc(fpr) | 0x1d,			"v29" },
    { rc(fpr) | 0x1c,			"v28" },
    { rc(fpr) | 0x1b,			"v27" },
    { rc(fpr) | 0x1a,			"v26" },
    { rc(fpr) | 0x19,			"v25" },
    { rc(fpr) | 0x18,			"v24" },
    { rc(fpr) | 0x17,			"v23" },
    { rc(fpr) | 0x16,			"v22" },
    { rc(fpr) | 0x15,			"v21" },
    { rc(fpr) | 0x14,			"v20" },
    { rc(fpr) | 0x13,			"v19" },
    { rc(fpr) | 0x12,			"v18" },
    { rc(fpr) | 0x11,			"v17" },
    { rc(fpr) | 0x10,			"v16" },
    { rc(sav) | rc(fpr) | 0x08,		"v8" },
    { rc(sav) | rc(fpr) | 0x09,		"v9" },
    { rc(sav) | rc(fpr) | 0x0a,		"v10" },
    { rc(sav) | rc(fpr) | 0x0b,		"v11" },
    { rc(sav) | rc(fpr) | 0x0c,		"v12" },
    { rc(sav) | rc(fpr) | 0x0d,		"v13" },
    { rc(sav) | rc(fpr) | 0x0e,		"v14" },
    { rc(sav) | rc(fpr) | 0x0f,		"v15" },
    { rc(arg) | rc(fpr) | 0x07,		"v7" },
    { rc(arg) | rc(fpr) | 0x06,		"v6" },
    { rc(arg) | rc(fpr) | 0x05,		"v5" },
    { rc(arg) | rc(fpr) | 0x04,		"v4" },
    { rc(arg) | rc(fpr) | 0x03,		"v3" },
    { rc(arg) | rc(fpr) | 0x02,		"v2" },
    { rc(arg) | rc(fpr) | 0x01,		"v1" },
    { rc(arg) | rc(fpr) | 0x00,		"v0" },
    { _NOREG,				"<none>" },
};

static jit_int32_t iregs[] = {
    _R19, _R20, _R21, _R22, _R23, _R24, _R25, _R26, _R27, _R28
};

static jit_int32_t fregs[] = {
    _V8, _V9, _V10, _V11, _V12, _V13, _V14, _V15
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
    jit_int32_t		 r0, r1;
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
    /* Cannot "addr sp, sp, reg" because in this context "sp" is "[w|x]zr",
     * the zero register */
#if 0
    jit_addr(JIT_SP, JIT_SP, r0);
#else
    r1 = jit_get_reg(jit_class_gpr);
    /* note that "mov r1, sp" does not work, but the proper encoding
     * can be triggered before actually emiting with "add r1, sp, 0" */
    jit_addi(r1, JIT_SP, 0);
    jit_addr(r1, r1, r0);
    jit_addi(JIT_SP, r1, 0);
    jit_unget_reg(r1);
#endif
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
    return (jit_arg_f_reg_p(u->u.w));
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

#if !__APPLE_
	/* Allocate va_list like object in the stack,
	 * with enough space to save all argument
	 * registers, and use fixed offsets for them. */
	_jitc->function->vaoff = jit_allocai(sizeof(jit_va_list_t));

	/* Initialize gp offset in save area. */
	if (jit_arg_reg_p(_jitc->function->self.argi))
	    _jitc->function->vagp = (8 - _jitc->function->self.argi) * -8;
	else
	    _jitc->function->vagp = 0;

	/* Initialize fp offset in save area. */
	if (jit_arg_f_reg_p(_jitc->function->self.argf))
	    _jitc->function->vafp = (8 - _jitc->function->self.argf) * -16;
	else
	    _jitc->function->vafp = 0;
#endif
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
    if (jit_arg_reg_p(_jitc->function->self.argi))
	offset = _jitc->function->self.argi++;
    else {
#if PACKED_STACK || STRONG_TYPE_CHECKING
	assert(code >= jit_code_arg_c && code <= jit_code_arg);
#endif
#if PACKED_STACK
	_jitc->function->self.size +=
	    _jitc->function->self.size & ((1 << (code - jit_code_arg_c)) - 1);
#endif
	offset = _jitc->function->self.size;
#if PACKED_STACK
	_jitc->function->self.size += 1 << (code - jit_code_arg_c);
#else
	_jitc->function->self.size += sizeof(jit_word_t);
#endif
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
    else {
#if PACKED_STACK
	_jitc->function->self.size +=
	    _jitc->function->self.size & (sizeof(jit_float32_t) - 1);
#endif
	offset = _jitc->function->self.size;
#if PACKED_STACK
	_jitc->function->self.size += sizeof(jit_float32_t);
#else
	_jitc->function->self.size += sizeof(jit_word_t);
#endif
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
    else {
#if PACKED_STACK
	_jitc->function->self.size +=
	    _jitc->function->self.size & (sizeof(jit_float64_t) - 1);
#endif
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_float64_t);
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
    if (jit_arg_reg_p(v->u.w)) {
#if PACKED_STACK
	jit_movr(u, JIT_RA0 - v->u.w);
#else
	jit_extr_c(u, JIT_RA0 - v->u.w);
#endif
    }
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
    if (jit_arg_reg_p(v->u.w)) {
#if PACKED_STACK
	jit_movr(u, JIT_RA0 - v->u.w);
#else
	jit_extr_uc(u, JIT_RA0 - v->u.w);
#endif
    }
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
    if (jit_arg_reg_p(v->u.w)) {
#if PACKED_STACK
	jit_movr(u, JIT_RA0 - v->u.w);
#else
	jit_extr_s(u, JIT_RA0 - v->u.w);
#endif
    }
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
    if (jit_arg_reg_p(v->u.w)) {
#if PACKED_STACK
	jit_movr(u, JIT_RA0 - v->u.w);
#else
	jit_extr_us(u, JIT_RA0 - v->u.w);
#endif
    }
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
    if (jit_arg_reg_p(v->u.w)) {
#if PACKED_STACK || __WORDSIZE == 32
	jit_movr(u, JIT_RA0 - v->u.w);
#else
	jit_extr_i(u, JIT_RA0 - v->u.w);
#endif
    }
    else {
	jit_node_t	*node = jit_ldxi_i(u, JIT_FP, v->u.w);
	jit_link_alist(node);
    }
    jit_dec_synth();
}

#if __WORDSIZE == 64
void
_jit_getarg_ui(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert_arg_type(v->code, jit_code_arg_i);
    jit_inc_synth_wp(getarg_ui, u, v);
    if (jit_arg_reg_p(v->u.w)) {
#if PACKED_STACK
	jit_movr(u, JIT_RA0 - v->u.w);
#else
	jit_extr_ui(u, JIT_RA0 - v->u.w);
#endif
    }
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
#endif

void
_jit_putargr(jit_state_t *_jit, jit_int32_t u, jit_node_t *v, jit_code_t code)
{
    assert_putarg_type(code, v->code);
    jit_code_inc_synth_wp(code, u, v);
    if (jit_arg_reg_p(v->u.w)) {
	jit_int32_t	regno = JIT_RA0 - v->u.w;
#if PACKED_STACK
	switch (code) {
	    case jit_code_putargr_c:	jit_extr_c(regno, u);	break;
	    case jit_code_putargr_uc:	jit_extr_uc(regno, u);	break;
	    case jit_code_putargr_s:	jit_extr_s(regno, u);	break;
	    case jit_code_putargr_us:	jit_extr_us(regno, u);	break;
#  if __WORDISZE == 32
	    case jit_code_putargr_i:	jit_movr(regno, u);	break;
#  else
	    case jit_code_putargr_i:	jit_extr_i(regno, u);	break;
	    case jit_code_putargr_ui:	jit_extr_ui(regno, u);	break;
	    case jit_code_putargr_l:	jit_movr(regno, u);	break;
#  endif
	    default:			abort();		break;
	}
#else
	jit_movr(regno, u);
#endif
    }
    else {
	jit_node_t	*node;
#if PACKED_STACK
	switch (code) {
	    case jit_code_putargr_c:	case jit_code_putargr_uc:
		node = jit_stxi_c(v->u.w, JIT_FP, u);		break;
	    case jit_code_putargr_s:	case jit_code_putargr_us:
		node = jit_stxi_s(v->u.w, JIT_FP, u);		break;
#  if __WORDSIZE == 32
	    case jit_code_putargr_i:
		node = jit_stxi(v->u.w, JIT_FP, u);		break;
#  else
	    case jit_code_putargr_i:	case jit_code_putargr_ui:
		node = jit_stxi_i(v->u.w, JIT_FP, u);		break;
	    case jit_code_putargr_l:
		node = jit_stxi(v->u.w, JIT_FP, u);		break;
#  endif
	    default:			abort();		break;
	}
#else
	node = jit_stxi(v->u.w, JIT_FP, u);
#endif
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
#if PACKED_STACK
    switch (code) {
	case jit_code_putargi_c:	u = (jit_int8_t)u;	break;
	case jit_code_putargi_uc:	u = (jit_uint8_t)u;	break;
	case jit_code_putargi_s:	u = (jit_int16_t)u;	break;
	case jit_code_putargi_us:	u = (jit_uint16_t)u;	break;
#  if __WORDSIZE == 32
	case jit_code_putargi_i:				break;
#  else
	case jit_code_putargi_i:	u = (jit_int32_t)u;	break;
	case jit_code_putargi_ui:	u = (jit_uint32_t)u;	break;
	case jit_code_putargi_l:				break;
#  endif
	default:			abort();		break;
    }
#endif
    if (jit_arg_reg_p(v->u.w))
	jit_movi(JIT_RA0 - v->u.w, u);
    else {
	jit_node_t	*node;
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
#if PACKED_STACK
	switch (code) {
	    case jit_code_putargi_c:	case jit_code_putargi_uc:
		node = jit_stxi_c(v->u.w, JIT_FP, regno);	break;
	    case jit_code_putargi_s:	case jit_code_putargi_us:
		node = jit_stxi_s(v->u.w, JIT_FP, regno);	break;
#  if __WORDSIZE == 32
	    case jit_code_putargi_i:
		node = jit_stxi(v->u.w, JIT_FP, regno);		break;
#  else
	    case jit_code_putargi_i:	case jit_code_putargi_ui:
		node = jit_stxi_i(v->u.w, JIT_FP, regno);	break;
	    case jit_code_putargi_l:
		node = jit_stxi(v->u.w, JIT_FP, regno);		break;
#  endif
	    default:			abort();		break;
	}
#else
	node = jit_stxi(v->u.w, JIT_FP, regno);
#endif
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
    if (jit_arg_reg_p(v->u.w))
	jit_movr_f(u, JIT_FA0 - v->u.w);
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
	jit_int32_t	regno = JIT_RA0 - _jitc->function->call.argi;
#if PACKED_STACK
	switch (code) {
	    case jit_code_pushargr_c:	jit_extr_c(regno, u);	break;
	    case jit_code_pushargr_uc:	jit_extr_uc(regno, u);	break;
	    case jit_code_pushargr_s:	jit_extr_s(regno, u);	break;
	    case jit_code_pushargr_us:	jit_extr_us(regno, u);	break;
#  if __WORDISZE == 32
	    case jit_code_pushargr_i:	jit_movr(regno, u);	break;
#  else
	    case jit_code_pushargr_i:	jit_extr_i(regno, u);	break;
	    case jit_code_pushargr_ui:	jit_extr_ui(regno, u);	break;
	    case jit_code_pushargr_l:	jit_movr(regno, u);	break;
#  endif
	    default:			abort();		break;
	}
#else
	jit_movr(regno, u);
#endif
#if __APPLE__
	if (_jitc->function->call.call & jit_call_varargs) {
	    assert(code == jit_code_pushargr);
	    jit_stxi(_jitc->function->call.size, JIT_SP, u);
	    _jitc->function->call.size += sizeof(jit_word_t);
	}
#endif
	++_jitc->function->call.argi;
    }
    else {
#if PACKED_STACK
	_jitc->function->call.size +=
	    _jitc->function->call.size &
		((1 << ((code - jit_code_pushargr_c) >> 2)) - 1);
	switch (code) {
	    case jit_code_pushargr_c:	case jit_code_pushargr_uc:
		jit_stxi_c(_jitc->function->call.size, JIT_SP, u);
		break;
	    case jit_code_pushargr_s:	case jit_code_pushargr_us:
		jit_stxi_s(_jitc->function->call.size, JIT_SP, u);
		break;
#  if __WORDSIZE == 32
	    case jit_code_pushargr_i:
		jit_stxi(_jitc->function->call.size, JIT_SP, u);
		break;
#  else
	    case jit_code_pushargr_i:	case jit_code_pushargr_ui:
		jit_stxi_i(_jitc->function->call.size, JIT_SP, u);
		break;
	    case jit_code_pushargr_l:
		jit_stxi(_jitc->function->call.size, JIT_SP, u);
		break;
#  endif
	    default:
		abort();
		break;
	}
	_jitc->function->call.size += 1 << ((code - jit_code_pushargr_c) >> 2);
#else
	jit_stxi(_jitc->function->call.size, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
#endif
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
#if PACKED_STACK
    switch (code) {
	case jit_code_pushargi_c:	u = (jit_int8_t)u;	break;
	case jit_code_pushargi_uc:	u = (jit_uint8_t)u;	break;
	case jit_code_pushargi_s:	u = (jit_int16_t)u;	break;
	case jit_code_pushargi_us:	u = (jit_uint16_t)u;	break;
#  if __WORDSIZE == 32
	case jit_code_pushargi_i:				break;
#  else
	case jit_code_pushargi_i:	u = (jit_int32_t)u;	break;
	case jit_code_pushargi_ui:	u = (jit_uint32_t)u;	break;
	case jit_code_pushargi_l:				break;
#  endif
	default:			abort();		break;
    }
#endif
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	regno = JIT_RA0 - _jitc->function->call.argi;
	jit_movi(regno, u);
#if __APPLE__
	if (_jitc->function->call.call & jit_call_varargs) {
	    assert(code == jit_code_pushargi);
	    jit_stxi(_jitc->function->call.size, JIT_SP, regno);
	    _jitc->function->call.size += sizeof(jit_word_t);
	}
#endif
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
#if PACKED_STACK
	_jitc->function->call.size +=
	    _jitc->function->call.size &
		((1 << ((code - jit_code_pushargr_c) >> 2)) - 1);
	switch (code) {
	    case jit_code_pushargi_c:	case jit_code_pushargi_uc:
		jit_stxi_c(_jitc->function->call.size, JIT_SP, regno);
		break;
	    case jit_code_pushargi_s:	case jit_code_pushargi_us:
		jit_stxi_s(_jitc->function->call.size, JIT_SP, regno);
		break;
#  if __WORDSIZE == 32
	    case jit_code_pushargi_i:
		jit_stxi(_jitc->function->call.size, JIT_SP, regno);
		break;
#  else
	    case jit_code_pushargi_i:	case jit_code_pushargi_ui:
		jit_stxi_i(_jitc->function->call.size, JIT_SP, regno);
		break;
	    case jit_code_pushargi_l:
		jit_stxi(_jitc->function->call.size, JIT_SP, regno);
		break;
#  endif
	    default:
		abort();
		break;
	}
	_jitc->function->call.size += 1 << ((code - jit_code_pushargr_c) >> 2);
#else
	jit_stxi(_jitc->function->call.size, JIT_SP, regno);
	_jitc->function->call.size += sizeof(jit_word_t);
#endif
	jit_unget_reg(regno);
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
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movr_f(JIT_FA0 - _jitc->function->call.argf, u);
#if __APPLE__
	if (_jitc->function->call.call & jit_call_varargs) {
	    assert(sizeof(jit_float32_t) == sizeof(jit_word_t));
	    jit_stxi_f(_jitc->function->call.size, JIT_SP,
		       JIT_FA0 - _jitc->function->call.argf);
	    _jitc->function->call.size += sizeof(jit_word_t);
	}
#endif
	++_jitc->function->call.argf;
    }
    else {
#if PACKED_STACK
	_jitc->function->call.size +=
	    _jitc->function->call.size & (sizeof(jit_float32_t) - 1);
	jit_stxi_f(_jitc->function->call.size, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_float32_t);
#else
	jit_stxi_f(_jitc->function->call.size, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
#endif
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
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movi_f(JIT_FA0 - _jitc->function->call.argf, u);
#if __APPLE__
	if (_jitc->function->call.call & jit_call_varargs) {
	    assert(sizeof(jit_float32_t) == sizeof(jit_word_t));
	    jit_stxi_f(_jitc->function->call.size, JIT_SP,
		       JIT_FA0 - _jitc->function->call.argf);
	    _jitc->function->call.size += sizeof(jit_word_t);
	}
#endif
	++_jitc->function->call.argf;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
#if PACKED_STACK
	_jitc->function->call.size +=
	    _jitc->function->call.size & (sizeof(jit_float32_t) - 1);
	jit_stxi_f(_jitc->function->call.size, JIT_SP, regno);
	_jitc->function->call.size += sizeof(jit_float32_t);
#else
 	jit_stxi_f(_jitc->function->call.size, JIT_SP, regno);
	_jitc->function->call.size += sizeof(jit_word_t);
#endif
	jit_unget_reg(regno);
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
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movr_d(JIT_FA0 - _jitc->function->call.argf, u);
#if __APPLE__
	if (_jitc->function->call.call & jit_call_varargs) {
	    assert(sizeof(jit_float64_t) == sizeof(jit_word_t));
	    jit_stxi_d(_jitc->function->call.size, JIT_SP,
		       JIT_FA0 - _jitc->function->call.argf);
	    _jitc->function->call.size += sizeof(jit_float64_t);
	}
#endif
	++_jitc->function->call.argf;
    }
    else {
#if PACKED_STACK
	_jitc->function->call.size +=
	    _jitc->function->call.size & (sizeof(jit_float64_t) - 1);
#endif
	jit_stxi_d(_jitc->function->call.size, JIT_SP, u);
	_jitc->function->call.size += sizeof(jit_float64_t);
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
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movi_d(JIT_FA0 - _jitc->function->call.argf, u);
#if __APPLE__
	if (_jitc->function->call.call & jit_call_varargs) {
	    assert(sizeof(jit_float64_t) == sizeof(jit_word_t));
	    jit_stxi_d(_jitc->function->call.size, JIT_SP,
		       JIT_FA0 - _jitc->function->call.argf);
	    _jitc->function->call.size += sizeof(jit_float64_t);
	}
#endif
	++_jitc->function->call.argf;
    }
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
#if PACKED_STACK
	_jitc->function->call.size +=
	    _jitc->function->call.size & (sizeof(jit_float64_t) - 1);
#endif
 	jit_stxi_d(_jitc->function->call.size, JIT_SP, regno);
	jit_unget_reg(regno);
	_jitc->function->call.size += sizeof(jit_float64_t);
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
#if PACKED_STACK
    _jitc->function->call.size +=
	_jitc->function->call.size & (sizeof(jit_word_t) - 1);
#endif
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
#if PACKED_STACK
    _jitc->function->call.size +=
	_jitc->function->call.size & (sizeof(jit_word_t) - 1);
#endif
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
#if __WORDSIZE == 32
    jit_movr(r0, JIT_RET);
#else
    jit_extr_i(r0, JIT_RET);
#endif
    jit_dec_synth();
}

#if __WORDSIZE == 64
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
    jit_movr(r0, JIT_RET);
    jit_dec_synth();
}
#endif

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
				     node->w.f);			\
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
				     node->w.d);			\
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
		case_rr(mov,);
		case_rrr(movn,);
		case_rrr(movz,);
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
			if (s26_p(word))
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
		jit_check_frame();
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    if (temp->flag & jit_flag_patch)
			calli(temp->u.w);
		    else {
			word = _jit->code.length -
			    (_jit->pc.uc - _jit->code.ptr);
			if (s26_p(word))
			    word = calli(_jit->pc.w);
			else
			    word = calli_p(_jit->pc.w);
			patch(word, node);
		    }
		}
		else
		    calli(node->u.w);
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
#  if __WORDSIZE == 64
	    case jit_code_arg_l:
#  endif
	    case jit_code_arg_f:		case jit_code_arg_d:
	    case jit_code_va_end:
	    case jit_code_ret:
	    case jit_code_retr_c:		case jit_code_reti_c:
	    case jit_code_retr_uc:		case jit_code_reti_uc:
	    case jit_code_retr_s:		case jit_code_reti_s:
	    case jit_code_retr_us:		case jit_code_reti_us:
	    case jit_code_retr_i:		case jit_code_reti_i:
#if __WORDSIZE == 64
	    case jit_code_retr_ui:		case jit_code_reti_ui:
	    case jit_code_retr_l:		case jit_code_reti_l:
#endif
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
#if __WORDSIZE == 64
	    case jit_code_putargr_ui:		case jit_code_putargi_ui:
	    case jit_code_putargr_l:		case jit_code_putargi_l:
#endif
	    case jit_code_putargr_f:		case jit_code_putargi_f:
	    case jit_code_putargr_d:		case jit_code_putargi_d:
	    case jit_code_pushargr_c:		case jit_code_pushargi_c:
	    case jit_code_pushargr_uc:		case jit_code_pushargi_uc:
	    case jit_code_pushargr_s:		case jit_code_pushargi_s:
	    case jit_code_pushargr_us:		case jit_code_pushargi_us:
	    case jit_code_pushargr_i:		case jit_code_pushargi_i:
#if __WORDSIZE == 64
	    case jit_code_pushargr_ui:		case jit_code_pushargi_ui:
	    case jit_code_pushargr_l:		case jit_code_pushargi_l:
#endif
	    case jit_code_pushargr_f:		case jit_code_pushargi_f:
	    case jit_code_pushargr_d:		case jit_code_pushargi_d:
	    case jit_code_retval_c:		case jit_code_retval_uc:
	    case jit_code_retval_s:		case jit_code_retval_us:
	    case jit_code_retval_i:
#if __WORDSIZE == 64
	    case jit_code_retval_ui:		case jit_code_retval_l:
#endif
	    case jit_code_retval_f:		case jit_code_retval_d:
	    case jit_code_prepare:
	    case jit_code_finishr:		case jit_code_finishi:
		break;
	    default:
		abort();
	}
	jit_regarg_clr(node, value);
	assert(_jitc->regarg == 0 && _jitc->synth == 0);
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

    for (offset = 0; offset < _jitc->patches.offset; offset++) {
	node = _jitc->patches.ptr[offset].node;
	word = _jitc->patches.ptr[offset].inst;
	value = node->code == jit_code_movi ? node->v.n->u.w : node->u.n->u.w;
	patch_at(word, value);
    }

    jit_flush(_jit->code.ptr, _jit->pc.uc);

    return (_jit->code.ptr);
}

#define CODE				1
#  include "jit_aarch64-cpu.c"
#  include "jit_aarch64-fpu.c"
#undef CODE

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
