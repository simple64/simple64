/*
 * Copyright (C) 2012-2023  Free Software Foundation, Inc.
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

/* avoid using it due to partial stalls */
#define USE_INC_DEC			0

#if PROTO
#  if __X32 || __X64_32
#    define WIDE			0
#    define ldi(u, v)			ldi_i(u, v)
#    define ldr(u, v)			ldr_i(u, v)
#    define ldxr(u, v, w)		ldxr_i(u, v, w)
#    define ldxi(u, v, w)		ldxi_i(u, v, w)
#    define sti(u, v)			sti_i(u, v)
#    define stxi(u, v, w)		stxi_i(u, v, w)
#    define can_sign_extend_int_p(im)	1
#    define can_zero_extend_int_p(im)	1
#    define fits_uint32_p(im)		1
#  else
#    define WIDE			1
#    define ldi(u, v)			ldi_l(u, v)
#    define ldr(u, v)			ldr_l(u, v)
#    define ldxr(u, v, w)		ldxr_l(u, v, w)
#    define ldxi(u, v, w)		ldxi_l(u, v, w)
#    define sti(u, v)			sti_l(u, v)
#    define stxi(u, v, w)		stxi_l(u, v, w)
#    define can_sign_extend_int_p(im)					\
	(((im) >= 0 && (long long)(im) <=  0x7fffffffLL) ||		\
	 ((im) <  0 && (long long)(im) >  -0x80000000LL))
#    define can_zero_extend_int_p(im)					\
	((im) >= 0 && (im) < 0x80000000LL)
#    define fits_uint32_p(im)		(((im) & 0xffffffff00000000LL) == 0)
#  endif
#  if __X32 || __CYGWIN__ || __X64_32 || _WIN32
#      define reg8_p(rn)						\
      ((rn) >= _RAX_REGNO && (rn) <= _RBX_REGNO)
#  else
#      define reg8_p(rn)		1
#  endif
#  define _RAX_REGNO			0
#  define _RCX_REGNO			1
#  define _RDX_REGNO			2
#  define _RBX_REGNO			3
#  define _RSP_REGNO			4
#  define _RBP_REGNO			5
#  define _RSI_REGNO			6
#  define _RDI_REGNO			7
#  define _R8_REGNO			8
#  define _R9_REGNO			9
#  define _R10_REGNO			10
#  define _R11_REGNO			11
#  define _R12_REGNO			12
#  define _R13_REGNO			13
#  define _R14_REGNO			14
#  define _R15_REGNO			15
#  define r7(reg)			((reg) & 7)
#  define r8(reg)			((reg) & 15)
#  define _SCL1				0x00
#  define _SCL2				0x01
#  define _SCL4				0x02
#  define _SCL8				0x03
#  define X86_ADD			0
#  define X86_OR			1 << 3
#  define X86_ADC			2 << 3
#  define X86_SBB			3 << 3
#  define X86_AND			4 << 3
#  define X86_SUB			5 << 3
#  define X86_XOR			6 << 3
#  define X86_CMP			7 << 3
#  define X86_ROL			0
#  define X86_ROR			1
#  define X86_RCL			2
#  define X86_RCR			3
#  define X86_SHL			4
#  define X86_SHR			5
#  define X86_SAR			7
#  define X86_NOT			2
#  define X86_NEG			3
#  define X86_MUL			4
#  define X86_IMUL			5
#  define X86_DIV			6
#  define X86_IDIV			7
#  define X86_CC_O			0x0
#  define X86_CC_NO			0x1
#  define X86_CC_NAE			0x2
#  define X86_CC_B			0x2
#  define X86_CC_C			0x2
#  define X86_CC_AE			0x3
#  define X86_CC_NB			0x3
#  define X86_CC_NC			0x3
#  define X86_CC_E			0x4
#  define X86_CC_Z			0x4
#  define X86_CC_NE			0x5
#  define X86_CC_NZ			0x5
#  define X86_CC_BE			0x6
#  define X86_CC_NA			0x6
#  define X86_CC_A			0x7
#  define X86_CC_NBE			0x7
#  define X86_CC_S			0x8
#  define X86_CC_NS			0x9
#  define X86_CC_P			0xa
#  define X86_CC_PE			0xa
#  define X86_CC_NP			0xb
#  define X86_CC_PO			0xb
#  define X86_CC_L			0xc
#  define X86_CC_NGE			0xc
#  define X86_CC_GE			0xd
#  define X86_CC_NL			0xd
#  define X86_CC_LE			0xe
#  define X86_CC_NG			0xe
#  define X86_CC_G			0xf
#  define X86_CC_NLE			0xf
#  define mrm(md, r, m)			*_jit->pc.uc++ = (md<<6) | (r<<3) | m
#  define sib(sc, i, b)			*_jit->pc.uc++ = (sc<<6) | (i<<3) | b
#  define ic(c)				*_jit->pc.uc++ = c
#  define is(s)				*_jit->pc.us++ = s
#  define ii(i)				*_jit->pc.ui++ = i
#  if __X64 && !__X64_32
#    define il(l)			*_jit->pc.ul++ = l
#  else
#    define il(l)			ii(l)
#  endif
#  define rex(l, w, r, x, b)		_rex(_jit, l, w, r, x, b)
static void
_rex(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define rx(rd, md, rb, ri, ms)	_rx(_jit, rd, md, rb, ri, ms)
static void
_rx(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nop(n)			_nop(_jit, n)
static void _nop(jit_state_t*, jit_int32_t);
#  define emms()			is(0x770f)
#  define lea(md, rb, ri, ms, rd)	_lea(_jit, md, rb, ri, ms, rd)
static void
_lea(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define pushr(r0)			_pushr(_jit, r0)
static void _pushr(jit_state_t*, jit_int32_t) maybe_unused;
#  define popr(r0)			_popr(_jit, r0)
static void _popr(jit_state_t*, jit_int32_t) maybe_unused;
#  define xchgr(r0, r1)			_xchgr(_jit, r0, r1)
static void _xchgr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define testr(r0, r1)			_testr(_jit, r0, r1)
static void _testr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define testi(r0, i0)			_testi(_jit, r0, i0)
static void _testi(jit_state_t*, jit_int32_t, jit_word_t);
#  define cc(code, r0)			_cc(_jit, code, r0)
static void _cc(jit_state_t*, jit_int32_t, jit_int32_t);
#  define icmpr(r0, r1)			alur(X86_CMP, r0, r1)
#  define alur(code, r0, r1)		_alur(_jit, code, r0, r1)
static void _alur(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define icmpi(r0, i0)			alui(X86_CMP, r0, i0)
#  define alui(code, r0, i0)		_alui(_jit, code, r0, i0)
static void _alui(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define iaddr(r0, r1)			alur(X86_ADD, r0, r1)
#  define save(r0)			_save(_jit, r0)
static void _save(jit_state_t*, jit_int32_t);
#  define load(r0)			_load(_jit, r0)
static void _load(jit_state_t*, jit_int32_t);
#  define addr(r0, r1, r2)		_addr(_jit, r0, r1, r2)
static void _addr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define iaddi(r0, i0)			alui(X86_ADD, r0, i0)
#  define addi(r0, r1, i0)		_addi(_jit, r0, r1, i0)
static void _addi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#define addcr(r0, r1, r2)		_addcr(_jit, r0, r1, r2)
static void _addcr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#define addci(r0, r1, i0)		_addci(_jit, r0, r1, i0)
static void _addci(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define iaddxr(r0, r1)		_iaddxr(_jit, r0, r1)
static void _iaddxr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define addxr(r0, r1, r2)		_addxr(_jit, r0, r1, r2)
static void _addxr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define iaddxi(r0, i0)		alui(X86_ADC, r0, i0)
#  define addxi(r0, r1, i0)		_addxi(_jit, r0, r1, i0)
static void _addxi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define isubr(r0, r1)			alur(X86_SUB, r0, r1)
#  define subr(r0, r1, r2)		_subr(_jit, r0, r1, r2)
static void _subr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define isubi(r0, i0)			alui(X86_SUB, r0, i0)
#  define subi(r0, r1, i0)		_subi(_jit, r0, r1, i0)
static void _subi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define subcr(r0, r1, r2)		_subcr(_jit, r0, r1, r2)
static void _subcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subci(r0, r1, i0)		_subci(_jit, r0, r1, i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define isubxr(r0, r1)		alur(X86_SBB, r0, r1)
#  define subxr(r0, r1, r2)		_subxr(_jit, r0, r1, r2)
static void _subxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define isubxi(r0, i0)		alui(X86_SBB, r0, i0)
#  define subxi(r0, r1, i0)		_subxi(_jit, r0, r1, i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define imulr(r0, r1)			_imulr(_jit, r0, r1)
static void _imulr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define imuli(r0, r1, i0)		_imuli(_jit, r0, r1, i0)
static void _imuli(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define mulr(r0, r1, r2)		_mulr(_jit, r0, r1, r2)
static void _mulr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define muli(r0, r1, i0)		_muli(_jit, r0, r1, i0)
static void _muli(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define umulr(r0)			unr(X86_IMUL, r0)
#  define umulr_u(r0)			unr(X86_MUL, r0)
#  define qmulr(r0, r1, r2, r3)		_iqmulr(_jit, r0, r1, r2, r3, 1)
#  define qmulr_u(r0, r1, r2, r3)	_iqmulr(_jit, r0, r1, r2, r3, 0)
#  define iqmulr(r0, r1, r2, r3, sign)	_iqmulr(_jit, r0, r1, r2, r3, sign)
static void _iqmulr(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t,jit_int32_t, jit_bool_t);
#  define qmuli(r0, r1, r2, i0)		_iqmuli(_jit, r0, r1, r2, i0, 1)
#  define qmuli_u(r0, r1, r2, i0)	_iqmuli(_jit, r0, r1, r2, i0, 0)
#  define iqmuli(r0, r1, r2, i0, sign)	_iqmuli(_jit, r0, r1, r2, i0, sign)
static void _iqmuli(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t,jit_word_t, jit_bool_t);
#  define sign_extend_rdx_rax()		_sign_extend_rdx_rax(_jit)
static void _sign_extend_rdx_rax(jit_state_t*);
#  define idivr(r0)			unr(X86_IDIV, r0)
#  define idivr_u(r0)			unr(X86_DIV, r0)
#  define divremr(r0, r1, r2, i0, i1)	_divremr(_jit, r0, r1, r2, i0, i1)
static void
_divremr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,
	 jit_bool_t,jit_bool_t);
#  define divremi(r0, r1, i0, i1, i2)	_divremi(_jit, r0, r1, i0, i1, i2)
static void
_divremi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t,jit_bool_t,jit_bool_t);
#  define divr(r0, r1, r2)		divremr(r0, r1, r2, 1, 1)
#  define divi(r0, r1, i0)		divremi(r0, r1, i0, 1, 1)
#  define divr_u(r0, r1, r2)		divremr(r0, r1, r2, 0, 1)
#  define divi_u(r0, r1, i0)		divremi(r0, r1, i0, 0, 1)
#  define qdivr(r0, r1, r2, r3)		_iqdivr(_jit, r0, r1, r2, r3, 1)
#  define qdivr_u(r0, r1, r2, r3)	_iqdivr(_jit, r0, r1, r2, r3, 0)
#  define iqdivr(r0, r1, r2, r3, sign)	_iqdivr(_jit, r0, r1, r2, r3, sign)
static void _iqdivr(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t,jit_int32_t, jit_bool_t);
#  define qdivi(r0, r1, r2, i0)		_iqdivi(_jit, r0, r1, r2, i0, 1)
#  define qdivi_u(r0, r1, r2, i0)	_iqdivi(_jit, r0, r1, r2, i0, 0)
#  define iqdivi(r0, r1, r2, i0, sign)	_iqdivi(_jit, r0, r1, r2, i0, sign)
static void _iqdivi(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t,jit_word_t, jit_bool_t);
#  define remr(r0, r1, r2)		divremr(r0, r1, r2, 1, 0)
#  define remi(r0, r1, i0)		divremi(r0, r1, i0, 1, 0)
#  define remr_u(r0, r1, r2)		divremr(r0, r1, r2, 0, 0)
#  define remi_u(r0, r1, i0)		divremi(r0, r1, i0, 0, 0)
#  define iandr(r0, r1)			alur(X86_AND, r0, r1)
#  define andr(r0, r1, r2)		_andr(_jit, r0, r1, r2)
static void _andr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define iandi(r0, i0)			alui(X86_AND, r0, i0)
#  define andi(r0, r1, i0)		_andi(_jit, r0, r1, i0)
static void _andi(jit_state_t*, jit_int32_t,jit_int32_t,jit_word_t);
#  define iorr(r0, r1)			alur(X86_OR, r0, r1)
#  define orr(r0, r1, r2)		_orr(_jit, r0, r1, r2)
static void _orr(jit_state_t*, jit_int32_t,jit_int32_t,jit_int32_t);
#  define iori(r0, i0)			alui(X86_OR, r0, i0)
#  define ori(r0, r1, i0)		_ori(_jit, r0, r1, i0)
static void _ori(jit_state_t*, jit_int32_t,jit_int32_t,jit_word_t);
#  define ixorr(r0, r1)			alur(X86_XOR, r0, r1)
#  define xorr(r0, r1, r2)		_xorr(_jit, r0, r1, r2)
static void _xorr(jit_state_t*, jit_int32_t,jit_int32_t,jit_int32_t);
#  define ixori(r0, i0)			alui(X86_XOR, r0, i0)
#  define xori(r0, r1, i0)		_xori(_jit, r0, r1, i0)
static void _xori(jit_state_t*, jit_int32_t,jit_int32_t,jit_word_t);
#  define irotshr(code, r0)		_irotshr(_jit, code, r0)
static void _irotshr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define rotshr(code, r0, r1, r2)	_rotshr(_jit, code, r0, r1, r2)
static void
_rotshr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define irotshi(code, r0, i0)		_irotshi(_jit, code, r0, i0)
static void _irotshi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define rotshi(code, r0, r1, i0)	_rotshi(_jit, code, r0, r1, i0)
static void
_rotshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define lshr(r0, r1, r2)		rotshr(X86_SHL, r0, r1, r2)
#  define lshi(r0, r1, i0)		_lshi(_jit, r0, r1, i0)
static void _lshi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define rshr(r0, r1, r2)		rotshr(X86_SAR, r0, r1, r2)
#  define rshi(r0, r1, i0)		rotshi(X86_SAR, r0, r1, i0)
#  define rshr_u(r0, r1, r2)		rotshr(X86_SHR, r0, r1, r2)
#  define rshi_u(r0, r1, i0)		rotshi(X86_SHR, r0, r1, i0)
#  define unr(code, r0)			_unr(_jit, code, r0)
static void _unr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define inegr(r0)			unr(X86_NEG, r0)
#  define negr(r0, r1)			_negr(_jit, r0, r1)
static void _negr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define icomr(r0)			unr(X86_NOT, r0)
#  define comr(r0, r1)			_comr(_jit, r0, r1)
static void _comr(jit_state_t*, jit_int32_t, jit_int32_t);
#  if USE_INC_DEC
#    define incr(r0, r1)		_incr(_jit, r0, r1)
static void _incr(jit_state_t*, jit_int32_t, jit_int32_t);
#    define decr(r0, r1)		_decr(_jit, r0, r1)
static void _decr(jit_state_t*, jit_int32_t, jit_int32_t);
#  endif
#  define clor(r0, r1)			_clor(_jit, r0, r1)
static void _clor(jit_state_t*, jit_int32_t, jit_int32_t);
#  define clzr(r0, r1)			_clzr(_jit, r0, r1)
static void _clzr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ctor(r0, r1)			_ctor(_jit, r0, r1)
static void _ctor(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ctzr(r0, r1)			_ctzr(_jit, r0, r1)
static void _ctzr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define cr(code, r0, r1, r2)		_cr(_jit, code, r0, r1, r2)
static void
_cr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ci(code, r0, r1, i0)		_ci(_jit, code, r0, r1, i0)
static void
_ci(jit_state_t *_jit, jit_int32_t, jit_int32_t, jit_int32_t, jit_word_t);
#  define ci0(code, r0, r1)		_ci0(_jit, code, r0, r1)
static void _ci0(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ltr(r0, r1, r2)		_ltr(_jit, r0, r1, r2)
static void _ltr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define lti(r0, r1, i0)			_lti(_jit, r0, r1, i0)
static void _lti(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define ltr_u(r0, r1, r2)		_ltr_u(_jit, r0, r1, r2)
static void _ltr_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define lti_u(r0, r1, i0)		ci(X86_CC_B, r0, r1, i0)
#  define ler(r0, r1, r2)		_ler(_jit, r0, r1, r2)
static void _ler(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define lei(r0, r1, i0)		ci(X86_CC_LE, r0, r1, i0)
#  define ler_u(r0, r1, r2)		_ler_u(_jit, r0, r1, r2)
static void _ler_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define lei_u(r0, r1, i0)		_lei_u(_jit, r0, r1, i0)
static void _lei_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define eqr(r0, r1, r2)		_eqr(_jit, r0, r1, r2)
static void _eqr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define eqi(r0, r1, i0)		_eqi(_jit, r0, r1, i0)
static void _eqi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define ger(r0, r1, r2)		_ger(_jit, r0, r1, r2)
static void _ger(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define gei(r0, r1, i0)		_gei(_jit, r0, r1, i0)
static void _gei(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define ger_u(r0, r1, r2)		_ger_u(_jit, r0, r1, r2)
static void _ger_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define gei_u(r0, r1, i0)		_gei_u(_jit, r0, r1, i0)
static void _gei_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define gtr(r0, r1, r2)		_gtr(_jit, r0, r1, r2)
static void _gtr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define gti(r0, r1, i0)		_ci(_jit, X86_CC_G, r0, r1, i0)
#  define gtr_u(r0, r1, r2)		_gtr_u(_jit, r0, r1, r2)
static void _gtr_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define gti_u(r0, r1, i0)		_gti_u(_jit, r0, r1, i0)
static void _gti_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define ner(r0, r1, r2)		_ner(_jit, r0, r1, r2)
static void _ner(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define nei(r0, r1, i0)		_nei(_jit, r0, r1, i0)
static void _nei(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define movr(r0, r1)			_movr(_jit, r0, r1)
static void _movr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define imovi(r0, i0)			_imovi(_jit, r0, i0)
static void _imovi(jit_state_t*, jit_int32_t, jit_word_t);
#  define movi(r0, i0)			_movi(_jit, r0, i0)
static
#  if CAN_RIP_ADDRESS
jit_word_t
#  else
void
#  endif
_movi(jit_state_t*, jit_int32_t, jit_word_t);
#  define movi_p(r0, i0)		_movi_p(_jit, r0, i0)
static jit_word_t _movi_p(jit_state_t*, jit_int32_t, jit_word_t);
#  define movcr(r0, r1)			_movcr(_jit, r0, r1)
static void _movcr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movcr_u(r0, r1)		_movcr_u(_jit, r0, r1)
static void _movcr_u(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movsr(r0, r1)			_movsr(_jit, r0, r1)
static void _movsr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movsr_u(r0, r1)		_movsr_u(_jit, r0, r1)
static void _movsr_u(jit_state_t*,jit_int32_t,jit_int32_t);
#  define casx(r0, r1, r2, r3, i0)	_casx(_jit, r0, r1, r2, r3, i0)
static void _casx(jit_state_t *_jit,jit_int32_t,jit_int32_t,
		  jit_int32_t,jit_int32_t,jit_word_t);
#define casr(r0, r1, r2, r3)		casx(r0, r1, r2, r3, 0)
#define casi(r0, i0, r1, r2)		casx(r0, _NOREG, r1, r2, i0)
#define movnr(r0, r1, r2)		_movnr(_jit, r0, r1, r2)
static void _movnr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#define movzr(r0, r1, r2)		_movzr(_jit, r0, r1, r2)
static void _movzr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  if __X64 && !__X64_32
#    define movir(r0, r1)		_movir(_jit, r0, r1)
static void _movir(jit_state_t*,jit_int32_t,jit_int32_t);
#    define movir_u(r0, r1)		_movir_u(_jit, r0, r1)
static void _movir_u(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#  define bswapr_us(r0, r1)		_bswapr_us(_jit, r0, r1)
static void _bswapr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define bswapr_ui(r0, r1)		_bswapr_ui(_jit, r0, r1)
static void _bswapr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  if __X64 && !__X64_32
#define bswapr_ul(r0, r1)		_bswapr_ul(_jit, r0, r1)
static void _bswapr_ul(jit_state_t*,jit_int32_t,jit_int32_t);
#endif
#  define extr_c(r0, r1)		_extr_c(_jit, r0, r1)
static void _extr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_uc(r0, r1)		_extr_uc(_jit, r0, r1)
static void _extr_uc(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_s(r0, r1)		movsr(r0, r1)
#  define extr_us(r0, r1)		movsr_u(r0, r1)
#  if __X64 && !__X64_32
#    define extr_i(r0, r1)		movir(r0, r1)
#    define extr_ui(r0, r1)		movir_u(r0, r1)
#  endif
#  define ldr_c(r0, r1)			_ldr_c(_jit, r0, r1)
static void _ldr_c(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ldi_c(r0, i0)			_ldi_c(_jit, r0, i0)
static void _ldi_c(jit_state_t*, jit_int32_t, jit_word_t);
#  define ldr_uc(r0, r1)		_ldr_uc(_jit, r0, r1)
static void _ldr_uc(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ldi_uc(r0, i0)		_ldi_uc(_jit, r0, i0)
static void _ldi_uc(jit_state_t*, jit_int32_t, jit_word_t);
#  define ldr_s(r0, r1)			_ldr_s(_jit, r0, r1)
static void _ldr_s(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ldi_s(r0, i0)			_ldi_s(_jit, r0, i0)
static void _ldi_s(jit_state_t*, jit_int32_t, jit_word_t);
#  define ldr_us(r0, r1)		_ldr_us(_jit, r0, r1)
static void _ldr_us(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ldi_us(r0, i0)		_ldi_us(_jit, r0, i0)
static void _ldi_us(jit_state_t*, jit_int32_t, jit_word_t);
#  if __X32 || !__X64_32
#    define ldr_i(r0, r1)		_ldr_i(_jit, r0, r1)
static void _ldr_i(jit_state_t*, jit_int32_t, jit_int32_t);
#    define ldi_i(r0, i0)		_ldi_i(_jit, r0, i0)
static void _ldi_i(jit_state_t*, jit_int32_t, jit_word_t);
#  endif
#  if __X64
#    if __X64_32
#      define ldr_i(r0, r1)		_ldr_ui(_jit, r0, r1)
#      define ldi_i(r0, i0)		_ldi_ui(_jit, r0, i0)
#    else
#      define ldr_ui(r0, r1)		_ldr_ui(_jit, r0, r1)
#      define ldi_ui(r0, i0)		_ldi_ui(_jit, r0, i0)
#    endif
static void _ldr_ui(jit_state_t*, jit_int32_t, jit_int32_t);
static void _ldi_ui(jit_state_t*, jit_int32_t, jit_word_t);
#    if !__X64_32
#      define ldr_l(r0, r1)		_ldr_l(_jit, r0, r1)
static void _ldr_l(jit_state_t*, jit_int32_t, jit_int32_t);
#      define ldi_l(r0, i0)		_ldi_l(_jit, r0, i0)
static void _ldi_l(jit_state_t*, jit_int32_t, jit_word_t);
#    endif
#  endif
#  define ldxr_c(r0, r1, r2)		_ldxr_c(_jit, r0, r1, r2)
static void _ldxr_c(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ldxi_c(r0, r1, i0)		_ldxi_c(_jit, r0, r1, i0)
static void _ldxi_c(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define ldxr_uc(r0, r1, r2)		_ldxr_uc(_jit, r0, r1, r2)
static void _ldxr_uc(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ldxi_uc(r0, r1, i0)		_ldxi_uc(_jit, r0, r1, i0)
static void _ldxi_uc(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define ldxr_s(r0, r1, r2)		_ldxr_s(_jit, r0, r1, r2)
static void _ldxr_s(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ldxi_s(r0, r1, i0)		_ldxi_s(_jit, r0, r1, i0)
static void _ldxi_s(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define ldxr_us(r0, r1, r2)		_ldxr_us(_jit, r0, r1, r2)
static void _ldxr_us(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define ldxi_us(r0, r1, i0)		_ldxi_us(_jit, r0, r1, i0)
static void _ldxi_us(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  if __X32 || !__X64_32
#    define ldxr_i(r0, r1, r2)		_ldxr_i(_jit, r0, r1, r2)
static void _ldxr_i(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#    define ldxi_i(r0, r1, i0)		_ldxi_i(_jit, r0, r1, i0)
static void _ldxi_i(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  endif
#  if __X64
#    if __X64_32
#      define ldxr_i(r0, r1, r2)	_ldxr_ui(_jit, r0, r1, r2)
#      define ldxi_i(r0, r1, i0)	_ldxi_ui(_jit, r0, r1, i0)
#    else
#      define ldxr_ui(r0, r1, r2)	_ldxr_ui(_jit, r0, r1, r2)
#      define ldxi_ui(r0, r1, i0)	_ldxi_ui(_jit, r0, r1, i0)
#    endif
static void _ldxr_ui(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
static void _ldxi_ui(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#    if !__X64_32
#      define ldxr_l(r0, r1, r2)	_ldxr_l(_jit, r0, r1, r2)
static void _ldxr_l(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#      define ldxi_l(r0, r1, i0)	_ldxi_l(_jit, r0, r1, i0)
static void _ldxi_l(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#    endif
#  endif
#  define str_c(r0, r1)			_str_c(_jit, r0, r1)
static void _str_c(jit_state_t*, jit_int32_t, jit_int32_t);
#  define sti_c(i0, r0)			_sti_c(_jit, i0, r0)
static void _sti_c(jit_state_t*, jit_word_t, jit_int32_t);
#  define str_s(r0, r1)			_str_s(_jit, r0, r1)
static void _str_s(jit_state_t*, jit_int32_t, jit_int32_t);
#  define sti_s(i0, r0)			_sti_s(_jit, i0, r0)
static void _sti_s(jit_state_t*, jit_word_t, jit_int32_t);
#  define str_i(r0, r1)			_str_i(_jit, r0, r1)
static void _str_i(jit_state_t*, jit_int32_t, jit_int32_t);
#  define sti_i(i0, r0)			_sti_i(_jit, i0, r0)
static void _sti_i(jit_state_t*, jit_word_t, jit_int32_t);
#  if __X64 && !__X64_32
#    define str_l(r0, r1)		_str_l(_jit, r0, r1)
static void _str_l(jit_state_t*, jit_int32_t, jit_int32_t);
#    define sti_l(i0, r0)		_sti_l(_jit, i0, r0)
static void _sti_l(jit_state_t*, jit_word_t, jit_int32_t);
#  endif
#  define stxr_c(r0, r1, r2)		_stxr_c(_jit, r0, r1, r2)
static void _stxr_c(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define stxi_c(i0, r0, r1)		_stxi_c(_jit, i0, r0, r1)
static void _stxi_c(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define stxr_s(r0, r1, r2)		_stxr_s(_jit, r0, r1, r2)
static void _stxr_s(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define stxi_s(i0, r0, r1)		_stxi_s(_jit, i0, r0, r1)
static void _stxi_s(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define stxr_i(r0, r1, r2)		_stxr_i(_jit, r0, r1, r2)
static void _stxr_i(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define stxi_i(i0, r0, r1)		_stxi_i(_jit, i0, r0, r1)
static void _stxi_i(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  if __X64 && !__X64_32
#    define stxr_l(r0, r1, r2)		_stxr_l(_jit, r0, r1, r2)
static void _stxr_l(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#    define stxi_l(i0, r0, r1)		_stxi_l(_jit, i0, r0, r1)
static void _stxi_l(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  endif
#  define jcc(code, i0)			_jcc(_jit, code, i0)
#  define jo(i0)			jcc(X86_CC_O, i0)
#  define jno(i0)			jcc(X86_CC_NO, i0)
#  define jnae(i0)			jcc(X86_CC_NAE, i0)
#  define jb(i0)			jcc(X86_CC_B, i0)
#  define jc(i0)			jcc(X86_CC_C, i0)
#  define jae(i0)			jcc(X86_CC_AE, i0)
#  define jnb(i0)			jcc(X86_CC_NB, i0)
#  define jnc(i0)			jcc(X86_CC_NC, i0)
#  define je(i0)			jcc(X86_CC_E, i0)
#  define jz(i0)			jcc(X86_CC_Z, i0)
#  define jne(i0)			jcc(X86_CC_NE, i0)
#  define jnz(i0)			jcc(X86_CC_NZ, i0)
#  define jbe(i0)			jcc(X86_CC_BE, i0)
#  define jna(i0)			jcc(X86_CC_NA, i0)
#  define ja(i0)			jcc(X86_CC_A, i0)
#  define jnbe(i0)			jcc(X86_CC_NBE, i0)
#  define js(i0)			jcc(X86_CC_S, i0)
#  define jns(i0)			jcc(X86_CC_NS, i0)
#  define jp(i0)			jcc(X86_CC_P, i0)
#  define jpe(i0)			jcc(X86_CC_PE, i0)
#  define jnp(i0)			jcc(X86_CC_NP, i0)
#  define jpo(i0)			jcc(X86_CC_PO, i0)
#  define jl(i0)			jcc(X86_CC_L, i0)
#  define jnge(i0)			jcc(X86_CC_NGE, i0)
#  define jge(i0)			jcc(X86_CC_GE, i0)
#  define jnl(i0)			jcc(X86_CC_NL, i0)
#  define jle(i0)			jcc(X86_CC_LE, i0)
#  define jng(i0)			jcc(X86_CC_NG, i0)
#  define jg(i0)			jcc(X86_CC_G, i0)
#  define jnle(i0)			jcc(X86_CC_NLE, i0)
static jit_word_t _jcc(jit_state_t*, jit_int32_t, jit_word_t);
#  define jccs(code, i0)		_jccs(_jit, code, i0)
#  define jos(i0)			jccs(X86_CC_O, i0)
#  define jnos(i0)			jccs(X86_CC_NO, i0)
#  define jnaes(i0)			jccs(X86_CC_NAE, i0)
#  define jbs(i0)			jccs(X86_CC_B, i0)
#  define jcs(i0)			jccs(X86_CC_C, i0)
#  define jaes(i0)			jccs(X86_CC_AE, i0)
#  define jnbs(i0)			jccs(X86_CC_NB, i0)
#  define jncs(i0)			jccs(X86_CC_NC, i0)
#  define jes(i0)			jccs(X86_CC_E, i0)
#  define jzs(i0)			jccs(X86_CC_Z, i0)
#  define jnes(i0)			jccs(X86_CC_NE, i0)
#  define jnzs(i0)			jccs(X86_CC_NZ, i0)
#  define jbes(i0)			jccs(X86_CC_BE, i0)
#  define jnas(i0)			jccs(X86_CC_NA, i0)
#  define jas(i0)			jccs(X86_CC_A, i0)
#  define jnbes(i0)			jccs(X86_CC_NBE, i0)
#  define jss(i0)			jccs(X86_CC_S, i0)
#  define jnss(i0)			jccs(X86_CC_NS, i0)
#  define jps(i0)			jccs(X86_CC_P, i0)
#  define jpes(i0)			jccs(X86_CC_PE, i0)
#  define jnps(i0)			jccs(X86_CC_NP, i0)
#  define jpos(i0)			jccs(X86_CC_PO, i0)
#  define jls(i0)			jccs(X86_CC_L, i0)
#  define jnges(i0)			jccs(X86_CC_NGE, i0)
#  define jges(i0)			jccs(X86_CC_GE, i0)
#  define jnls(i0)			jccs(X86_CC_NL, i0)
#  define jles(i0)			jccs(X86_CC_LE, i0)
#  define jngs(i0)			jccs(X86_CC_NG, i0)
#  define jgs(i0)			jccs(X86_CC_G, i0)
#  define jnles(i0)			jccs(X86_CC_NLE, i0)
static jit_word_t _jccs(jit_state_t*, jit_int32_t, jit_word_t);
#  define jcr(code, i0, r0, r1)		_jcr(_jit, code, i0, r0, r1)
static jit_word_t _jcr(jit_state_t*,
		       jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define jci(code, i0, r0, i1)		_jci(_jit, code, i0, r0, i1)
static jit_word_t _jci(jit_state_t*,
		       jit_int32_t,jit_word_t,jit_int32_t,jit_word_t);
#  define jci0(code, i0, r0)		_jci0(_jit, code, i0, r0)
static jit_word_t _jci0(jit_state_t*, jit_int32_t, jit_word_t, jit_int32_t);
#  define bltr(i0, r0, r1)		_bltr(_jit, i0, r0, r1)
static jit_word_t _bltr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define blti(i0, r0, i1)		_blti(_jit, i0, r0, i1)
static jit_word_t _blti(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bltr_u(i0, r0, r1)		_bltr_u(_jit, i0, r0, r1)
static jit_word_t _bltr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define blti_u(i0, r0, i1)		_blti_u(_jit, i0, r0, i1)
static jit_word_t _blti_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bler(i0, r0, r1)		_bler(_jit, i0, r0, r1)
static jit_word_t _bler(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define blei(i0, r0, i1)		_blei(_jit, i0, r0, i1)
static jit_word_t _blei(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bler_u(i0, r0, r1)		_bler_u(_jit, i0, r0, r1)
static jit_word_t _bler_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define blei_u(i0, r0, i1)		_blei_u(_jit, i0, r0, i1)
static jit_word_t _blei_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define beqr(i0, r0, r1)		_beqr(_jit, i0, r0, r1)
static jit_word_t _beqr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define beqi(i0, r0, i1)		_beqi(_jit, i0, r0, i1)
static jit_word_t _beqi(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bger(i0, r0, r1)		_bger(_jit, i0, r0, r1)
static jit_word_t _bger(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define bgei(i0, r0, i1)		_bgei(_jit, i0, r0, i1)
static jit_word_t _bgei(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bger_u(i0, r0, r1)		_bger_u(_jit, i0, r0, r1)
static jit_word_t _bger_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define bgei_u(i0, r0, i1)		_bgei_u(_jit, i0, r0, i1)
static jit_word_t _bgei_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bgtr(i0, r0, r1)		_bgtr(_jit, i0, r0, r1)
static jit_word_t _bgtr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define bgti(i0, r0, i1)		_bgti(_jit, i0, r0, i1)
static jit_word_t _bgti(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bgtr_u(i0, r0, r1)		_bgtr_u(_jit, i0, r0, r1)
static jit_word_t _bgtr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define bgti_u(i0, r0, i1)		_bgti_u(_jit, i0, r0, i1)
static jit_word_t _bgti_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bner(i0, r0, r1)		_bner(_jit, i0, r0, r1)
static jit_word_t _bner(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#  define bnei(i0, r0, i1)		_bnei(_jit, i0, r0, i1)
static jit_word_t _bnei(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bmsr(i0, r0, r1)		_bmsr(_jit, i0, r0, r1)
static jit_word_t _bmsr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bmsi(i0, r0, i1)		_bmsi(_jit, i0, r0, i1)
static jit_word_t _bmsi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bmcr(i0, r0, r1)		_bmcr(_jit, i0, r0, r1)
static jit_word_t _bmcr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bmci(i0, r0, i1)		_bmci(_jit, i0, r0, i1)
static jit_word_t _bmci(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr(i0, r0, r1)		_boaddr(_jit, i0, r0, r1)
static jit_word_t _boaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define boaddi(i0, r0, i1)		_boaddi(_jit, i0, r0, i1)
static jit_word_t _boaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr_u(i0, r0, r1)		_boaddr_u(_jit, i0, r0, r1)
static jit_word_t _boaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define boaddi_u(i0, r0, i1)		_boaddi_u(_jit, i0, r0, i1)
static jit_word_t _boaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxaddr(i0, r0, r1)		_bxaddr(_jit, i0, r0, r1)
static jit_word_t _bxaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxaddi(i0, r0, i1)		_bxaddi(_jit, i0, r0, i1)
static jit_word_t _bxaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxaddr_u(i0, r0, r1)		_bxaddr_u(_jit, i0, r0, r1)
static jit_word_t _bxaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxaddi_u(i0, r0, i1)		_bxaddi_u(_jit, i0, r0, i1)
static jit_word_t _bxaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr(i0, r0, r1)		_bosubr(_jit, i0, r0, r1)
static jit_word_t _bosubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bosubi(i0, r0, i1)		_bosubi(_jit, i0, r0, i1)
static jit_word_t _bosubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr_u(i0, r0, r1)		_bosubr_u(_jit, i0, r0, r1)
static jit_word_t _bosubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bosubi_u(i0, r0, i1)		_bosubi_u(_jit, i0, r0, i1)
static jit_word_t _bosubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxsubr(i0, r0, r1)		_bxsubr(_jit, i0, r0, r1)
static jit_word_t _bxsubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxsubi(i0, r0, i1)		_bxsubi(_jit, i0, r0, i1)
static jit_word_t _bxsubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxsubr_u(i0, r0, r1)		_bxsubr_u(_jit, i0, r0, r1)
static jit_word_t _bxsubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxsubi_u(i0, r0, i1)		_bxsubi_u(_jit, i0, r0, i1)
static jit_word_t _bxsubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define callr(r0)			_callr(_jit, r0)
static void _callr(jit_state_t*, jit_int32_t);
#  define calli(i0)			_calli(_jit, i0)
static jit_word_t _calli(jit_state_t*, jit_word_t);
#  if __X64
#    define calli_p(i0)			_calli_p(_jit, i0)
static jit_word_t _calli_p(jit_state_t*, jit_word_t);
#  else
#    define calli_p(i0)			calli(i0)
#  endif
#  define jmpr(r0)			_jmpr(_jit, r0)
static void _jmpr(jit_state_t*, jit_int32_t);
#  define jmpi(i0)			_jmpi(_jit, i0)
static jit_word_t _jmpi(jit_state_t*, jit_word_t);
#  if __X64
#    define jmpi_p(i0)			_jmpi_p(_jit, i0)
static jit_word_t _jmpi_p(jit_state_t*, jit_word_t);
#  else
#    define jmpi_p(i0)			jmpi(i0)
#  endif
#  define jmpsi(i0)			_jmpsi(_jit, i0)
static jit_word_t _jmpsi(jit_state_t*, jit_uint8_t);
#  define prolog(node)			_prolog(_jit, node)
static void _prolog(jit_state_t*, jit_node_t*);
#  define epilog(node)			_epilog(_jit, node)
static void _epilog(jit_state_t*, jit_node_t*);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#  define vaarg_d(r0, r1, i0)		_vaarg_d(_jit, r0, r1, i0)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_bool_t);
#  define patch_at(instr, label)	_patch_at(_jit, instr, label)
static void _patch_at(jit_state_t*, jit_word_t, jit_word_t);
#  if !defined(HAVE_FFSL)
#    if __X32
#      define ffsl(i)			__builtin_ffs(i)
#    else
#      define ffsl(l)			__builtin_ffsl(l)
#    endif
#  endif
#  define jit_cmov_p()			jit_cpu.cmov
#endif

#if CODE
static void
_rex(jit_state_t *_jit, jit_int32_t l, jit_int32_t w,
     jit_int32_t r, jit_int32_t x, jit_int32_t b)
{
#if __X64
    jit_int32_t	v = 0x40 | (w << 3);

    if (r != _NOREG)
	v |= (r & 8) >> 1;
    if (x != _NOREG)
	v |= (x & 8) >> 2;
    if (b != _NOREG)
	v |= (b & 8) >> 3;
    if (l || v != 0x40)
	ic(v);
#endif
}

static void
_rx(jit_state_t *_jit, jit_int32_t rd, jit_int32_t md,
    jit_int32_t rb, jit_int32_t ri, jit_int32_t ms)
{
    if (ri == _NOREG) {
	if (rb == _NOREG) {
	    /* Use ms == _SCL8 to tell it is a %rip relative displacement */
#if __X64
	    if (ms == _SCL8)
#endif
		mrm(0x00, r7(rd), 0x05);
#if __X64
	    else {
		mrm(0x00, r7(rd), 0x04);
		sib(_SCL1, 0x04, 0x05);
	    }
#endif
	    ii(md);
	}
	else if (r7(rb) == _RSP_REGNO) {
	    if (md == 0) {
		mrm(0x00, r7(rd), 0x04);
		sib(ms, 0x04, 0x04);
	    }
	    else if ((jit_int8_t)md == md) {
		mrm(0x01, r7(rd), 0x04);
		sib(ms, 0x04, 0x04);
		ic(md);
	    }
	    else {
		mrm(0x02, r7(rd), 0x04);
		sib(ms, 0x04, 0x04);
		ii(md);
	    }
	}
	else {
	    if (md == 0 && r7(rb) != _RBP_REGNO)
		mrm(0x00, r7(rd), r7(rb));
	    else if ((jit_int8_t)md == md) {
		mrm(0x01, r7(rd), r7(rb));
		ic(md);
	    }
	    else {
		mrm(0x02, r7(rd), r7(rb));
		ii(md);
	    }
	}
    }
    else if (rb == _NOREG) {
	mrm(0x00, r7(rd), 0x04);
	sib(ms, r7(ri), 0x05);
	ii(md);
    }
    else if (r8(ri) != _RSP_REGNO) {
	if (md == 0 && r7(rb) != _RBP_REGNO) {
	    mrm(0x00, r7(rd), 0x04);
	    sib(ms, r7(ri), r7(rb));
	}
	else if ((jit_int8_t)md == md) {
	    mrm(0x01, r7(rd), 0x04);
	    sib(ms, r7(ri), r7(rb));
	    ic(md);
	}
	else {
	    mrm(0x02, r7(rd), 0x04);
	    sib(ms, r7(ri), r7(rb));
	    ic(md);
	}
    }
    else {
	fprintf(stderr, "illegal index register");
	abort();
    }
}

static void
_nop(jit_state_t *_jit, jit_int32_t count)
{
    jit_int32_t		i;
    while (count) {
	if (count > 9)
	    i = 9;
	else
	    i = count;
	switch (i) {
	    case 0:
		break;
	    case 1:	    /* NOP */
		ic(0x90);   break;
	    case 2:	    /* 66 NOP */
		ic(0x66);   ic(0x90);
		break;
	    case 3:	    /* NOP DWORD ptr [EAX] */
		ic(0x0f);   ic(0x1f);	    ic(0x00);
		break;
	    case 4:	    /* NOP DWORD ptr [EAX + 00H] */
		ic(0x0f);   ic(0x1f);	    ic(0x40);	    ic(0x00);
		break;
	    case 5:	    /* NOP DWORD ptr [EAX + EAX*1 + 00H] */
		ic(0x0f);   ic(0x1f);	    ic(0x44);	    ic(0x00);
		ic(0x00);
		break;
	    case 6:	    /* 66 NOP DWORD ptr [EAX + EAX*1 + 00H] */
		ic(0x66);   ic(0x0f);	    ic(0x1f);	    ic(0x44);
		ic(0x00);   ic(0x00);
		break;
	    case 7:	    /* NOP DWORD ptr [EAX + 00000000H] */
		ic(0x0f);   ic(0x1f);	    ic(0x80);	    ii(0x0000);
		break;
	    case 8:	    /* NOP DWORD ptr [EAX + EAX*1 + 00000000H] */
		ic(0x0f);   ic(0x1f);	    ic(0x84);	    ic(0x00);
		ii(0x0000);
		break;
	    case 9:	    /* 66 NOP DWORD ptr [EAX + EAX*1 + 00000000H] */
		ic(0x66);   ic(0x0f);	    ic(0x1f);	    ic(0x84);
		ic(0x00);   ii(0x0000);
		break;
	}
	count -= i;
    }
}
static void
_lea(jit_state_t *_jit, jit_int32_t md, jit_int32_t rb,
     jit_int32_t ri, jit_int32_t ms, jit_int32_t rd)
{
    rex(0, WIDE, rd, ri, rb);
    ic(0x8d);
    rx(rd, md, rb, ri, ms);
}

static void
_pushr(jit_state_t *_jit, jit_int32_t r0)
{
    rex(0, WIDE, 0, 0, r0);
    ic(0x50 | r7(r0));
}

static void
_popr(jit_state_t *_jit, jit_int32_t r0)
{
    rex(0, WIDE, 0, 0, r0);
    ic(0x58 | r7(r0));
}

static void
_xchgr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r1, _NOREG, r0);
    ic(0x87);
    mrm(0x03, r7(r1), r7(r0));
}

static void
_testr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r1, _NOREG, r0);
    ic(0x85);
    mrm(0x03, r7(r1), r7(r0));
}

static void
_testi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    rex(0, WIDE, _NOREG, _NOREG, r0);
    if (r0 == _RAX_REGNO)
	ic(0xa9);
    else {
	ic(0xf7);
	mrm(0x03, 0x00, r7(r0));
    }
    ii(i0);
}

static void
_cc(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0)
{
    rex(0, 0, _NOREG, _NOREG, r0);
    ic(0x0f);
    ic(0x90 | code);
    mrm(0x03, 0x00, r7(r0));
}

static void
_alur(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r1, _NOREG, r0);
    ic(code | 0x01);
    mrm(0x03, r7(r1), r7(r0));
}

static void
_alui(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, _NOREG, _NOREG, r0);
	if ((jit_int8_t)i0 == i0) {
	    ic(0x83);
	    ic(0xc0 | code | r7(r0));
	    ic(i0);
	}
	else {
	    if (r0 == _RAX_REGNO)
		ic(code | 0x05);
	    else {
		ic(0x81);
		ic(0xc0 | code | r7(r0));
	    }
	    ii(i0);
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	alur(code, r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_save(jit_state_t *_jit, jit_int32_t r0)
{
    if (!_jitc->function->regoff[r0]) {
	_jitc->function->regoff[r0] = jit_allocai(sizeof(jit_word_t));
	_jitc->again = 1;
    }
    assert(!jit_regset_tstbit(&_jitc->regsav, r0));
    jit_regset_setbit(&_jitc->regsav, r0);
    stxi(_jitc->function->regoff[r0], _RBP_REGNO, r0);
}

static void
_load(jit_state_t *_jit, jit_int32_t r0)
{
    assert(_jitc->function->regoff[r0]);
    assert(jit_regset_tstbit(&_jitc->regsav, r0));
    jit_regset_clrbit(&_jitc->regsav, r0);
    ldxi(r0, _RBP_REGNO, _jitc->function->regoff[r0]);
}

static void
_addr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1)
	iaddr(r0, r2);
    else if (r0 == r2)
	iaddr(r0, r1);
    else
	lea(0, r1, r2, _SCL1, r0);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	movr(r0, r1);
#if USE_INC_DEC
    else if (i0 == 1)
	incr(r0, r1);
    else if (i0 == -1)
	decr(r0, r1);
#endif
    else if (can_sign_extend_int_p(i0)) {
	if (r0 == r1)
	    iaddi(r0, i0);
	else
	    lea(i0, r1, _NOREG, _SCL1, r0);
    }
    else if (r0 != r1) {
	movi(r0, i0);
	iaddr(r0, r1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	iaddr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	iaddr(r0, r1);
    else {
	movr(r0, r1);
	iaddr(r0, r2);
    }
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	movr(r0, r1);
	iaddi(r0, i0);
    }
    else if (r0 == r1) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	iaddr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movi(r0, i0);
	iaddr(r0, r1);
    }
}

static void
_iaddxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* FIXME: this is not doing what I did expect for the simple test case:
     *		mov  $0xffffffffffffffff, %rax	-- rax = 0xffffffffffffffff (-1)
     *		mov  $0xffffffffffffffff, %r10	-- r10 = 0xffffffffffffffff (-1)
     *		mov  $0x1, %r11d		-- r11 = 1
     *		xor  %rbx, %rbx			-- rbx = 0
     * (gdb) p $eflags
     * $1 = [ PF ZF IF ]
     *		add  %r11, %rax			-- r11 = 0x10000000000000000 (0)
     *				    does not fit in 64 bit ^
     * (gdb) p $eflags
     * $2 = [ CF PF AF ZF IF ]
     *		adcx %r10, %rbx			-- r10 = 0xffffffffffffffff (-1)
     * (gdb) p $eflags
     * $3 = [ CF PF AF ZF IF ]
     * (gdb) p/x $r10
     * $4 = 0xffffffffffffffff
     * but, r10 should be zero, as it is:
     * -1 (%r10) + 0 (%rbx) + carry (!!eflags.CF)
     * FIXME: maybe should only use ADCX in the third operation onward, that
     * is, after the first ADC? In either case, the add -1+0+carry should
     * have used and consumed the carry? At least this is what is expected
     * in Lightning...
     */
#if 0
    /* Significantly longer instruction, but avoid cpu stalls as only
     * the carry flag is used in a sequence. */
    if (jit_cpu.adx) {
	/* ADCX */
	ic(0x66);
	rex(0, WIDE, r1, _NOREG, r0);
	ic(0x0f);
	ic(0x38);
	ic(0xf6);
	mrm(0x03, r7(r1), r7(r0));
    }
    else
#endif
	alur(X86_ADC, r0, r1);
}

static void
_addxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r2)
	iaddxr(r0, r1);
    else {
	movr(r0, r1);
	iaddxr(r0, r2);
    }
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (
#if 0
	/* Do not mix ADC and ADCX */
	!jit_cpu.adx &&
#endif
	can_sign_extend_int_p(i0)) {
	movr(r0, r1);
	iaddxi(r0, i0);
    }
    else if (r0 == r1) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	iaddxr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movi(r0, i0);
	iaddxr(r0, r1);
    }
}

static void
_subr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	ixorr(r0, r0);
    else if (r0 == r2) {
	isubr(r0, r1);
	inegr(r0);
    }
    else {
	movr(r0, r1);
	isubr(r0, r2);
    }
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	movr(r0, r1);
#if USE_INC_DEC
    else if (i0 == 1)
	decr(r0, r1);
    else if (i0 == -1)
	incr(r0, r1);
#endif
    else if (can_sign_extend_int_p(i0)) {
	if (r0 == r1)
	    isubi(r0, i0);
	else
	    lea(-i0, r1, _NOREG, _SCL1, r0);
    }
    else if (r0 != r1) {
	movi(r0, -i0);
	iaddr(r0, r1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	isubr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2 && r0 != r1) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r0);
	movr(r0, r1);
	isubr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr(r0, r1);
	isubr(r0, r2);
    }
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    movr(r0, r1);
    if (can_sign_extend_int_p(i0))
	isubi(r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	isubr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r2 && r0 != r1) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r0);
	movr(r0, r1);
	isubxr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	movr(r0, r1);
	isubxr(r0, r2);
    }
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    movr(r0, r1);
    if (can_sign_extend_int_p(i0))
	isubxi(r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	imovi(rn(reg), i0);
	isubxr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_rsbi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    subi(r0, r1, i0);
    negr(r0, r0);
}

static void
_imulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xaf);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_imuli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, r0, _NOREG, r1);
	if ((jit_int8_t)i0 == i0) {
	    ic(0x6b);
	    mrm(0x03, r7(r0), r7(r1));
	    ic(i0);
	}
	else {
	    ic(0x69);
	    mrm(0x03, r7(r0), r7(r1));
	    ii(i0);
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	imulr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_mulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r0 == r1)
	imulr(r0, r2);
    else if (r0 == r2)
	imulr(r0, r1);
    else {
	movr(r0, r1);
	imulr(r0, r2);
    }
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    switch (i0) {
	case 0:
	    ixorr(r0, r0);
	    break;
	case 1:
	    movr(r0, r1);
	    break;
	case -1:
	    negr(r0, r1);
	    break;
	case 2:
	    lea(0, _NOREG, r1, _SCL2, r0);
	    break;
	case 4:
	    lea(0, _NOREG, r1, _SCL4, r0);
	    break;
	case 8:
	    lea(0, _NOREG, r1, _SCL8, r0);
	    break;
	default:
	    if (i0 > 0 && !(i0 & (i0 - 1)))
		lshi(r0, r1, ffsl(i0) - 1);
	    else if (can_sign_extend_int_p(i0))
		imuli(r0, r1, i0);
	    else if (r0 != r1) {
		movi(r0, i0);
		imulr(r0, r1);
	    }
	    else
		imuli(r0, r0, i0);
	    break;
    }
}

#define savset(rn)							\
    if (r0 != rn) {							\
	sav |= 1 << rn;							\
	if (r1 != rn && r2 != rn)					\
	    set |= 1 << rn;						\
    }
#define isavset(rn)							\
    if (r0 != rn) {							\
	sav |= 1 << rn;							\
	if (r1 != rn)							\
	    set |= 1 << rn;						\
    }
#define qsavset(rn)							\
    if (r0 != rn && r1 != rn) {						\
	sav |= 1 << rn;							\
	if (r2 != rn && r3 != rn)					\
	    set |= 1 << rn;						\
    }
#define allocr(rn, rv)							\
    if (set & (1 << rn))						\
	(void)jit_get_reg(rv|jit_class_gpr|jit_class_named);		\
    if (sav & (1 << rn)) {						\
	if ( jit_regset_tstbit(&_jitc->regsav, rv) ||			\
	    !jit_regset_tstbit(&_jitc->reglive, rv))			\
	    sav &= ~(1 << rn);						\
	else								\
	    save(rv);							\
    }
#define clear(rn, rv)							\
    if (set & (1 << rn))						\
	jit_unget_reg(rv);						\
    if (sav & (1 << rn))						\
	load(rv);
static void
_iqmulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		mul;
    jit_int32_t		sav;
    jit_int32_t		set;

    sav = set = 0;
    qsavset(_RDX_REGNO);
    qsavset(_RAX_REGNO);
    allocr(_RDX_REGNO, _RDX);
    allocr(_RAX_REGNO, _RAX);

    if (r3 == _RAX_REGNO)
	mul = r2;
    else {
	mul = r3;
	movr(_RAX_REGNO, r2);
    }
    if (sign)
	umulr(mul);
    else
	umulr_u(mul);

    if (r0 == _RDX_REGNO && r1 == _RAX_REGNO)
	xchgr(_RAX_REGNO, _RDX_REGNO);
    else {
	if (r0 != _RDX_REGNO)
	    movr(r0, _RAX_REGNO);
	movr(r1, _RDX_REGNO);
	if (r0 == _RDX_REGNO)
	    movr(r0, _RAX_REGNO);
    }

    clear(_RDX_REGNO, _RDX);
    clear(_RAX_REGNO, _RAX);
}

static void
_iqmuli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;

    if (i0 == 0) {
	ixorr(r0, r0);
	ixorr(r1, r1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	if (sign)
	    qmulr(r0, r1, r2, rn(reg));
	else
	    qmulr_u(r0, r1, r2, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sign_extend_rdx_rax(jit_state_t *_jit)
{
    rex(0, WIDE, 0, 0, 0);
    ic(0x99);
}

static void
_divremr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2,
	 jit_bool_t sign, jit_bool_t divide)
{
    jit_int32_t		div;
    jit_int32_t		reg;
    jit_int32_t		set;
    jit_int32_t		sav;
    jit_int32_t		use;

    sav = set = use = 0;
    savset(_RDX_REGNO);
    savset(_RAX_REGNO);
    allocr(_RDX_REGNO, _RDX);
    allocr(_RAX_REGNO, _RAX);

    if (r2 == _RAX_REGNO) {
	if (r0 == _RAX_REGNO || r0 == _RDX_REGNO) {
	    if ((reg = jit_get_reg(jit_class_gpr|jit_class_chk)) == JIT_NOREG)
		reg = jit_get_reg((r1 == _RCX_REGNO ? _RBX : _RCX) |
				  jit_class_gpr|jit_class_named);
	    use = 1;
	    div = rn(reg);
	    movr(div, _RAX_REGNO);
	    if (r1 != _RAX_REGNO)
		movr(_RAX_REGNO, r1);
	}
	else {
	    if (r0 == r1)
		xchgr(r0, _RAX_REGNO);
	    else {
		if (r0 != _RAX_REGNO)
		    movr(r0, _RAX_REGNO);
		if (r1 != _RAX_REGNO)
		    movr(_RAX_REGNO, r1);
	    }
	    div = r0;
	}
    }
    else if (r2 == _RDX_REGNO) {
	if (r0 == _RAX_REGNO || r0 == _RDX_REGNO) {
	    if ((reg = jit_get_reg(jit_class_gpr|jit_class_chk)) == JIT_NOREG)
		reg = jit_get_reg((r1 == _RCX_REGNO ? _RBX : _RCX) |
				  jit_class_gpr|jit_class_named);
	    use = 1;
	    div = rn(reg);
	    movr(div, _RDX_REGNO);
	    if (r1 != _RAX_REGNO)
		movr(_RAX_REGNO, r1);
	}
	else {
	    if (r1 != _RAX_REGNO)
		movr(_RAX_REGNO, r1);
	    movr(r0, _RDX_REGNO);
	    div = r0;
	}
    }
    else {
	if (r1 != _RAX_REGNO)
	    movr(_RAX_REGNO, r1);
	div = r2;
    }

    if (sign) {
	sign_extend_rdx_rax();
	idivr(div);
    }
    else {
	ixorr(_RDX_REGNO, _RDX_REGNO);
	idivr_u(div);
    }

    if (use)
	jit_unget_reg(reg);

    if (divide)
	movr(r0, _RAX_REGNO);
    else
	movr(r0, _RDX_REGNO);

    clear(_RDX_REGNO, _RDX);
    clear(_RAX_REGNO, _RAX);
}

static void
_divremi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0,
	 jit_bool_t sign, jit_bool_t divide)
{
    jit_int32_t		reg;
    jit_int32_t		div;
    jit_int32_t		sav;
    jit_int32_t		set;
    jit_int32_t		use;

    if (divide) {
	switch (i0) {
	    case 1:
		movr(r0, r1);
		return;
	    case -1:
		if (sign) {
		    negr(r0, r1);
		    return;
		}
		break;
	    default:
		if (i0 > 0 && !(i0 & (i0 - 1))) {
		    movr(r0, r1);
		    if (sign)
			rshi(r0, r0, ffsl(i0) - 1);
		    else
			rshi_u(r0, r0, ffsl(i0) - 1);
		    return;
		}
		break;
	}
    }
    else if (i0 == 1 || (sign && i0 == -1)) {
	ixorr(r0, r0);
	return;
    }
    else if (!sign && i0 > 0 && !(i0 & (i0 - 1))) {
	if (can_sign_extend_int_p(i0)) {
	    movr(r0, r1);
	    iandi(r0, i0 - 1);
	}
	else if (r0 != r1) {
	    movi(r0, i0 - 1);
	    iandr(r0, r1);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0 - 1);
	    iandr(r0, rn(reg));
	    jit_unget_reg(reg);
	}
	return;
    }

    sav = set = use = 0;
    isavset(_RDX_REGNO);
    isavset(_RAX_REGNO);
    allocr(_RDX_REGNO, _RDX);
    allocr(_RAX_REGNO, _RAX);

    if (r0 == _RAX_REGNO || r0 == _RDX_REGNO || r0 == r1) {
	if ((reg = jit_get_reg(jit_class_gpr|jit_class_chk)) == JIT_NOREG)
	    reg = jit_get_reg((r1 == _RCX_REGNO ? _RBX : _RCX) |
			      jit_class_gpr|jit_class_named);
	use = 1;
	div = rn(reg);
    }
    else
	div = r0;

    movi(div, i0);
    movr(_RAX_REGNO, r1);

    if (sign) {
	sign_extend_rdx_rax();
	idivr(div);
    }
    else {
	ixorr(_RDX_REGNO, _RDX_REGNO);
	idivr_u(div);
    }

    if (use)
	jit_unget_reg(reg);

    if (divide)
	movr(r0, _RAX_REGNO);
    else
	movr(r0, _RDX_REGNO);

    clear(_RDX_REGNO, _RDX);
    clear(_RAX_REGNO, _RAX);
}

static void
_iqdivr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		div;
    jit_int32_t		reg;
    jit_int32_t		sav;
    jit_int32_t		set;
    jit_int32_t		use;

    sav = set = use = 0;
    qsavset(_RDX_REGNO);
    qsavset(_RAX_REGNO);
    allocr(_RDX_REGNO, _RDX);
    allocr(_RAX_REGNO, _RAX);
    if (r3 == _RAX_REGNO) {
	if (r0 == _RAX_REGNO || r0 == _RDX_REGNO) {
	    if ((reg = jit_get_reg(jit_class_gpr|jit_class_chk)) == JIT_NOREG)
		reg = jit_get_reg((r1 == _RCX_REGNO ? _RBX : _RCX) |
				  jit_class_gpr|jit_class_named);
	    use = 1;
	    div = rn(reg);
	    movr(div, _RAX_REGNO);
	    if (r2 != _RAX_REGNO)
		movr(_RAX_REGNO, r2);
	}
	else {
	    if (r0 == r2)
		xchgr(r0, _RAX_REGNO);
	    else {
		if (r0 != _RAX_REGNO)
		    movr(r0, _RAX_REGNO);
		if (r2 != _RAX_REGNO)
		    movr(_RAX_REGNO, r2);
	    }
	    div = r0;
	}
    }
    else if (r3 == _RDX_REGNO) {
	if (r0 == _RAX_REGNO || r0 == _RDX_REGNO) {
	    if ((reg = jit_get_reg(jit_class_gpr|jit_class_chk)) == JIT_NOREG)
		reg = jit_get_reg((r1 == _RCX_REGNO ? _RBX : _RCX) |
				  jit_class_gpr|jit_class_named);
	    use = 1;
	    div = rn(reg);
	    movr(div, _RDX_REGNO);
	    if (r2 != _RAX_REGNO)
		movr(_RAX_REGNO, r2);
	}
	else {
	    if (r2 != _RAX_REGNO)
		movr(_RAX_REGNO, r2);
	    movr(r0, _RDX_REGNO);
	    div = r0;
	}
    }
    else {
	if (r2 != _RAX_REGNO)
	    movr(_RAX_REGNO, r2);
	div = r3;
    }
    if (sign) {
	sign_extend_rdx_rax();
	idivr(div);
    }
    else {
	ixorr(_RDX_REGNO, _RDX_REGNO);
	idivr_u(div);
    }
    if (use)
	jit_unget_reg(reg);

    if (r0 == _RDX_REGNO && r1 == _RAX_REGNO)
	xchgr(_RAX_REGNO, _RDX_REGNO);
    else {
	if (r0 != _RDX_REGNO)
	    movr(r0, _RAX_REGNO);
	movr(r1, _RDX_REGNO);
	if (r0 == _RDX_REGNO)
	    movr(r0, _RAX_REGNO);
    }

    clear(_RDX_REGNO, _RDX);
    clear(_RAX_REGNO, _RAX);
}

static void
_iqdivi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;

    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (sign)
	qdivr(r0, r1, r2, rn(reg));
    else
	qdivr_u(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
}
#undef clear
#undef allocr
#undef savset

static void
_andr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movr(r0, r1);
    else if (r0 == r1)
	iandr(r0, r2);
    else if (r0 == r2)
	iandr(r0, r1);
    else {
	movr(r0, r1);
	iandr(r0, r2);
    }
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;

    if (i0 == 0)
	ixorr(r0, r0);
    else if (i0 == -1)
	movr(r0, r1);
    else if (r0 == r1) {
	if (can_sign_extend_int_p(i0))
	    iandi(r0, i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    iandr(r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	movi(r0, i0);
	iandr(r0, r1);
    }
}

static void
_orr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movr(r0, r1);
    else if (r0 == r1)
	iorr(r0, r2);
    else if (r0 == r2)
	iorr(r0, r1);
    else {
	movr(r0, r1);
	iorr(r0, r2);
    }
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	movr(r0, r1);
    else if (i0 == -1)
	movi(r0, -1);
    else if (can_sign_extend_int_p(i0)) {
	movr(r0, r1);
	iori(r0, i0);
    }
    else if (r0 != r1) {
	movi(r0, i0);
	iorr(r0, r1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	iorr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_xorr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	ixorr(r0, r0);
    else if (r0 == r1)
	ixorr(r0, r2);
    else if (r0 == r2)
	ixorr(r0, r1);
    else {
	movr(r0, r1);
	ixorr(r0, r2);
    }
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	movr(r0, r1);
    else if (i0 == -1)
	comr(r0, r1);
    else if (can_sign_extend_int_p(i0)) {
	movr(r0, r1);
	ixori(r0, i0);
    }
    else if (r0 != r1) {
	movi(r0, i0);
	ixorr(r0, r1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ixorr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_irotshr(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0)
{
    rex(0, WIDE, _RCX_REGNO, _NOREG, r0);
    ic(0xd3);
    mrm(0x03, code, r7(r0));
}

static void
_rotshr(jit_state_t *_jit, jit_int32_t code,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_int32_t		use;

    if (r0 == _RCX_REGNO) {
	reg = jit_get_reg(jit_class_gpr);
	movr(rn(reg), r1);
	if (r2 != _RCX_REGNO)
	    movr(_RCX_REGNO, r2);
	irotshr(code, rn(reg));
	movr(_RCX_REGNO, rn(reg));
	jit_unget_reg(reg);
    }
    else if (r2 != _RCX_REGNO) {
	use = !jit_reg_free_p(_RCX);
	if (use) {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), _RCX_REGNO);
	}
	else
	    reg = 0;
	if (r1 == _RCX_REGNO) {
	    if (r0 == r2)
		xchgr(r0, _RCX_REGNO);
	    else {
		movr(r0, r1);
		movr(_RCX_REGNO, r2);
	    }
	}
	else {
	    movr(_RCX_REGNO, r2);
	    movr(r0, r1);
	}
	irotshr(code, r0);
	if (use) {
	    movr(_RCX_REGNO, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	movr(r0, r1);
	irotshr(code, r0);
    }
}

static void
_irotshi(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0, jit_word_t i0)
{
    rex(0, WIDE, _NOREG, _NOREG, r0);
    if (i0 == 1) {
	ic(0xd1);
	mrm(0x03, code, r7(r0));
    }
    else {
	ic(0xc1);
	mrm(0x03, code, r7(r0));
	ic(i0);
    }
}

static void
_rotshi(jit_state_t *_jit, jit_int32_t code,
	jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    movr(r0, r1);
    if (i0)
	irotshi(code, r0, i0);
}

static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0 == 0)
	movr(r0, r1);
    else if (i0 <= 3)
	lea(0, _NOREG, r1, i0 == 1 ? _SCL2 : i0 == 2 ? _SCL4 : _SCL8, r0);
    else
	rotshi(X86_SHL, r0, r1, i0);
}

static void
_unr(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0)
{
    rex(0, WIDE, _NOREG, _NOREG, r0);
    ic(0xf7);
    mrm(0x03, code, r7(r0));
}

static void
_negr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 == r1)
	inegr(r0);
    else {
	ixorr(r0, r0);
	isubr(r0, r1);
    }
}

static void
_comr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    movr(r0, r1);
    icomr(r0);
}

#if USE_INC_DEC
static void
_incr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    movr(r0, r1);
#  if __X64
    rex(0, WIDE, _NOREG, _NOREG, r0);
    ic(0xff);
    ic(0xc0 | r7(r0));
#  else
    ic(0x40 | r7(r0));
#  endif
}

static void
_decr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    movr(r0, r1);
#  if __X64
    rex(0, WIDE, _NOREG, _NOREG, r0);
    ic(0xff);
    ic(0xc8 | r7(r0));
#  else
    ic(0x48 | r7(r0));
#  endif
}
#endif

static void
_clor(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    comr(r0, r1);
    clzr(r0, r0);
}

static void
_clzr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, x;
    /* LZCNT */
    if (jit_cpu.abm)
	ic(0xf3);
    /* else BSR */
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xbd);
    mrm(0x3, r7(r0), r7(r1));
    if (!jit_cpu.abm) {
	/* jump if undefined: r1 == 0 */
	w = jccs(X86_CC_E, _jit->pc.w);
	/* count leading zeros */
	rsbi(r0, r0, __WORDSIZE - 1);
	/* done */
	x = jmpsi(_jit->pc.w);
	/* if r1 == 0 */
	patch_at(w, _jit->pc.w);
	movi(r0, __WORDSIZE);
	/* not undefined */
	patch_at(x, _jit->pc.w);
    }
    /* LZCNT has defined behavior for value zero and count leading zeros */
}

static void
_ctor(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    comr(r0, r1);
    ctzr(r0, r0);
}

static void
_ctzr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    if (!jit_cpu.abm) {
	if (jit_cmov_p())
	    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill|jit_class_chk);
	else
	    t0 = _NOREG;
	if (t0 != _NOREG)
	    movi(rn(t0), __WORDSIZE);
    }
    /* TZCNT */
    if (jit_cpu.abm)
	ic(0xf3);
    /* else BSF */
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xbc);
    mrm(0x3, r7(r0), r7(r1));
    if (!jit_cpu.abm) {
	/* No conditional move or need spill/reload a temporary */
	if (t0 == _NOREG) {
	    w = jccs(X86_CC_E, _jit->pc.w);
	    movi(r0, __WORDSIZE);
	    patch_at(w, _jit->pc.w);
	}
	else {
	    /* CMOVE */
	    rex(0, WIDE, r0, _NOREG, rn(t0));
	    ic(0x0f);
	    ic(0x44);
	    mrm(0x3, r7(r0), r7(rn(t0)));
	    jit_unget_reg(t0);
	}
    }
    /* TZCNT has defined behavior for value zero */
}

static void
_cr(jit_state_t *_jit,
    jit_int32_t code, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    jit_bool_t		same;
    if (reg8_p(r0)) {
	same = r0 == r1 || r0 == r2;
	if (!same)
	    ixorr(r0, r0);
	icmpr(r1, r2);
	if (same)
	    imovi(r0, 0);
	cc(code, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	ixorr(rn(reg), rn(reg));
	icmpr(r1, r2);
	cc(code, rn(reg));
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ci(jit_state_t *_jit,
    jit_int32_t code, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_bool_t		same;
    if (reg8_p(r0)) {
	same = r0 == r1;
	if (!same)
	    ixorr(r0, r0);
	icmpi(r1, i0);
	if (same)
	    imovi(r0, 0);
	cc(code, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	ixorr(rn(reg), rn(reg));
	icmpi(r1, i0);
	cc(code, rn(reg));
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ci0(jit_state_t *_jit, jit_int32_t code, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    jit_bool_t		same;
    if (reg8_p(r0)) {
	same = r0 == r1;
	if (!same)
	    ixorr(r0, r0);
	testr(r1, r1);
	if (same)
	    imovi(r0, 0);
	cc(code, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	ixorr(rn(reg), rn(reg));
	testr(r1, r1);
	cc(code, rn(reg));
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ltr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 0);
    else
	cr(X86_CC_L, r0, r1, r2);
}

static void
_lti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0)
	ci(X86_CC_L, r0, r1, i0);
    else
	ci0(X86_CC_S, r0, r1);
}

static void
_ltr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 0);
    else
	cr(X86_CC_B, r0, r1, r2);
}

static void
_ler(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	cr(X86_CC_LE, r0, r1, r2);
}

static void
_ler_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	cr(X86_CC_BE, r0, r1, r2);
}

static void
_lei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0)
	ci(X86_CC_BE, r0, r1, i0);
    else
	ci0(X86_CC_E, r0, r1);
}

static void
_eqr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	cr(X86_CC_E, r0, r1, r2);
}

static void
_eqi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0)
	ci(X86_CC_E, r0, r1, i0);
    else
	ci0(X86_CC_E, r0, r1);
}

static void
_ger(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	cr(X86_CC_GE, r0, r1, r2);
}

static void
_gei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0)
	ci(X86_CC_GE, r0, r1, i0);
    else
	ci0(X86_CC_NS, r0, r1);
}

static void
_ger_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 1);
    else
	cr(X86_CC_AE, r0, r1, r2);
}

static void
_gei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0)
	ci(X86_CC_AE, r0, r1, i0);
    else
	ci0(X86_CC_NB, r0, r1);
}

static void
_gtr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 0);
    else
	cr(X86_CC_G, r0, r1, r2);
}

static void
_gtr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 0);
    else
	cr(X86_CC_A, r0, r1, r2);
}

static void
_gti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0)
	ci(X86_CC_A, r0, r1, i0);
    else
	ci0(X86_CC_NE, r0, r1);
}

static void
_ner(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (r1 == r2)
	movi(r0, 0);
    else
	cr(X86_CC_NE, r0, r1, r2);
}

static void
_nei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0)
	ci(X86_CC_NE, r0, r1, i0);
    else
	ci0(X86_CC_NE, r0, r1);
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1) {
	rex(0, 1, r1, _NOREG, r0);
	ic(0x89);
	ic(0xc0 | (r1 << 3) | r7(r0));
    }
}

static void
_imovi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
#if __X64
#  if !__X64_32
    if (fits_uint32_p(i0)) {
#  endif
	rex(0, 0, _NOREG, _NOREG, r0);
	ic(0xb8 | r7(r0));
	ii(i0);
#  if !__X64_32
    }
    else if (can_sign_extend_int_p(i0)) {
	rex(0, 1, _NOREG, _NOREG, r0);
	ic(0xc7);
	ic(0xc0 | r7(r0));
	ii(i0);
    }
    else {
	rex(0, 1, _NOREG, _NOREG, r0);
	ic(0xb8 | r7(r0));
	il(i0);
    }
#  endif
#else
    ic(0xb8 | r7(r0));
    ii(i0);
#endif
}

#if CAN_RIP_ADDRESS
static jit_word_t
#else
static void
#endif
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
#if CAN_RIP_ADDRESS
    jit_word_t		w, rel;
    w = _jit->pc.w;
    rel = i0 - (w + 8);
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	/* lea rel(%rip), %r0 */
	rex(0, WIDE, r0, _NOREG, _NOREG);
	w = _jit->pc.w;
	ic(0x8d);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (i0)
	imovi(r0, i0);
    else
	ixorr(r0, r0);
#if CAN_RIP_ADDRESS
    return (w);
#endif
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    rex(0, WIDE, _NOREG, _NOREG, r0);
    w = _jit->pc.w;
    ic(0xb8 | r7(r0));
    il(i0);
    return (w);
}

static void
_movcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xbe);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_movcr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xb6);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_movsr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xbf);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_movsr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xb7);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_casx(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
      jit_int32_t r2, jit_int32_t r3, jit_word_t i0)
{
    jit_int32_t		save_rax, restore_rax;
    jit_int32_t		ascasr_reg, ascasr_use;
    if (r0 != _RAX_REGNO) {		/* result not in %rax */
	if (r2 != _RAX_REGNO) {		/* old value not in %rax */
	    save_rax = jit_get_reg(jit_class_gpr);
	    movr(rn(save_rax), _RAX_REGNO);
	    restore_rax = 1;
	}
	else
	    restore_rax = 0;
    }
    else
	restore_rax = 0;
    if (r2 != _RAX_REGNO)
	movr(_RAX_REGNO, r2);
    if (r1 == _NOREG) {			/* using immediate address */
	if (!can_sign_extend_int_p(i0)) {
	    ascasr_reg = jit_get_reg(jit_class_gpr);
	    if (ascasr_reg == _RAX) {
		ascasr_reg = jit_get_reg(jit_class_gpr);
		jit_unget_reg(_RAX);
	    }
	    ascasr_use = 1;
	    movi(rn(ascasr_reg), i0);
	}
	else
	    ascasr_use = 0;
    }
    else
	ascasr_use = 0;
    ic(0xf0);		/* lock */
    if (ascasr_use)
	rex(0, WIDE, r3, _NOREG, rn(ascasr_reg));
    else
	rex(0, WIDE, r3, _NOREG, r1);
    ic(0x0f);
    ic(0xb1);
    if (r1 != _NOREG)			/* casr */
	rx(r3, 0, r1, _NOREG, _SCL1);
    else {				/* casi */
	if (ascasr_use)
	    rx(r3, 0, rn(ascasr_reg), _NOREG, _SCL1);	/* address in reg */
	else
	    rx(r3, i0, _NOREG, _NOREG, _SCL1);		/* address in offset */
    }
    cc(X86_CC_E, r0);
    if (r0 != _RAX_REGNO)
	movr(r0, _RAX_REGNO);
    if (restore_rax) {
	movr(_RAX_REGNO, rn(save_rax));
	jit_unget_reg(save_rax);
    }
    if (ascasr_use)
	jit_unget_reg(ascasr_reg);
}

static void
_movnr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    assert(jit_cmov_p());

    testr(r2, r2);

    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0x45);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_movzr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    assert(jit_cmov_p());

    testr(r2, r2);

    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0x44);
    mrm(0x03, r7(r0), r7(r1));
}

#if __X64
static void
_movir(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, 1, r0, _NOREG, r1);
    ic(0x63);
    mrm(0x03, r7(r0), r7(r1));
}

static void
_movir_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, 0, r1, _NOREG, r0);
    ic(0x89);
    ic(0xc0 | (r1 << 3) | r7(r0));
}
#endif

static void
_bswapr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    extr_us(r0, r1);
    ic(0x66);
    rex(0, 0, _NOREG, _NOREG, r0);
    ic(0xc1);
    mrm(0x03, X86_ROR, r7(r0));
    ic(8);
}

static void
_bswapr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    movr(r0, r1);
    rex(0, 0, _NOREG, _NOREG, r0);
    ic(0x0f);
    ic(0xc8 | r7(r0));
}

#if __X64 && !__X64_32
static void
_bswapr_ul(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    movr(r0, r1);
    rex(0, 1, _NOREG, _NOREG, r0);
    ic(0x0f);
    ic(0xc8 | r7(r0));
}
#endif

static void
_extr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (reg8_p(r1))
	movcr(r0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	movr(rn(reg), r1);
	movcr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_extr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (reg8_p(r1))
	movcr_u(r0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	movr(rn(reg), r1);
	movcr_u(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xbe);
    rx(r0, 0, r1, _NOREG, _SCL1);
}

static void
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xbe);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xbe);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_c(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xb6);
    rx(r0, 0, r1, _NOREG, _SCL1);
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xb6);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xb6);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_uc(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xbf);
    rx(r0, 0, r1, _NOREG, _SCL1);
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xbf);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xbf);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_s(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x0f);
    ic(0xb7);
    rx(r0, 0, r1, _NOREG, _SCL1);
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xb7);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x0f);
	ic(0xb7);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_us(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

#if __X32 || !__X64_32
static void
_ldr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if __X64
    rex(0, WIDE, r0, _NOREG, r1);
    ic(0x63);
#else
    ic(0x8b);
#endif
    rx(r0, 0, r1, _NOREG, _SCL1);
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x63);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
#if __X64
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x63);
#else
	ic(0x8b);
#endif
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_i(r0, rn(reg));
	jit_unget_reg(reg);
    }
}
#endif

#if __X64
static void
_ldr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, 0, r0, _NOREG, r1);
    ic(0x63);
    rx(r0, 0, r1, _NOREG, _SCL1);
}

static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#  if !__X64_32
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, 0, r0, _NOREG, _NOREG);
	ic(0x63);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
	rex(0, 0, r0, _NOREG, _NOREG);
	ic(0x63);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
#  if __X64_32
	ldr_i(r0, rn(reg));
#  else
	ldr_ui(r0, rn(reg));
#  endif
	jit_unget_reg(reg);
    }
}

#  if !__X64_32
static void
_ldr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, 1, r0, _NOREG, r1);
    ic(0x8b);
    rx(r0, 0, r1, _NOREG, _SCL1);
}

static void
_ldi_l(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x8b);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x8b);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_l(r0, rn(reg));
	jit_unget_reg(reg);
    }
}
#  endif
#endif

static void
_ldxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    addr(r0, r1, r2);
    ldr_c(r0, r0);
#else
    rex(0, WIDE, r0, r1, r2);
    ic(0x0f);
    ic(0xbe);
    rx(r0, 0, r2, r1, _SCL1);
#endif
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, r0, _NOREG, r1);
	ic(0x0f);
	ic(0xbe);
	rx(r0, i0, r1, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_c(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    addr(r0, r1, r2);
    ldr_uc(r0, r0);
#else
    rex(0, WIDE, r0, r1, r2);
    ic(0x0f);
    ic(0xb6);
    rx(r0, 0, r2, r1, _SCL1);
#endif
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, r0, _NOREG, r1);
	ic(0x0f);
	ic(0xb6);
	rx(r0, i0, r1, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_uc(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    addr(r0, r1, r2);
    ldr_s(r0, r0);
#else
    rex(0, WIDE, r0, r1, r2);
    ic(0x0f);
    ic(0xbf);
    rx(r0, 0, r2, r1, _SCL1);
#endif
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, r0, _NOREG, r1);
	ic(0x0f);
	ic(0xbf);
	rx(r0, i0, r1, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_s(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    addr(r0, r1, r2);
    ldr_us(r0, r0);
#else
    rex(0, WIDE, r0, r1, r2);
    ic(0x0f);
    ic(0xb7);
    rx(r0, 0, r2, r1, _SCL1);
#endif
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, r0, _NOREG, r1);
	ic(0x0f);
	ic(0xb7);
	rx(r0, i0, r1, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_us(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

#if __X64 || !__X64_32
static void
_ldxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64
    rex(0, WIDE, r0, r1, r2);
    ic(0x63);
#else
    ic(0x8b);
#endif
    rx(r0, 0, r2, r1, _SCL1);
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
#if __X64
	rex(0, WIDE, r0, _NOREG, r1);
	ic(0x63);
#else
	ic(0x8b);
#endif
	rx(r0, i0, r1, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_i(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}
#endif

#if __X64
static void
_ldxr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    addr(r0, r1, r2);
    /* to avoid confusion with macro renames */
    _ldr_ui(_jit, r0, r0);
#else
    rex(0, 0, r0, r1, r2);
    ic(0x8b);
    rx(r0, 0, r2, r1, _SCL1);
#endif
}

static void
_ldxi_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, 0, r0, _NOREG, r1);
	ic(0x8b);
	rx(r0, i0, r1, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
#  if __X64_32
	ldxr_i(r0, r1, rn(reg));
#  else
	ldxr_ui(r0, r1, rn(reg));
#  endif
	jit_unget_reg(reg);
    }
}

#  if !__X64_32
static void
_ldxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    rex(0, 1, r0, r1, r2);
    ic(0x8b);
    rx(r0, 0, r2, r1, _SCL1);
}

static void
_ldxi_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, 1, r0, _NOREG, r1);
	ic(0x8b);
	rx(r0, i0, r1, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_l(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}
#  endif
#endif

static void
_str_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (reg8_p(r1)) {
	rex(0, 0, r1, _NOREG, r0);
	ic(0x88);
	rx(r1, 0, r0, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	movr(rn(reg), r1);
	rex(0, 0, rn(reg), _NOREG, r0);
	ic(0x88);
	rx(rn(reg), 0, r0, _NOREG, _SCL1);
	jit_unget_reg(reg);
    }
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 16 : rel + 16;
    if (can_sign_extend_int_p(rel)) {
	if (reg8_p(r0)) {
	    rex(0, 0, r0, _NOREG, _NOREG);
	    ic(0x88);
	    rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	    movr(rn(reg), r0);
	    rex(0, 0, rn(reg), _NOREG, _NOREG);
	    ic(0x88);
	    rx(rn(reg), i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
	    jit_unget_reg(reg);
	}
    }
    else
#endif
    if (address_p(i0)) {
	if (reg8_p(r0)) {
	    rex(0, 0, r0, _NOREG, _NOREG);
	    ic(0x88);
	    rx(r0, i0, _NOREG, _NOREG, _SCL1);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	    movr(rn(reg), r0);
	    rex(0, 0, rn(reg), _NOREG, _NOREG);
	    ic(0x88);
	    rx(rn(reg), i0, _NOREG, _NOREG, _SCL1);
	    jit_unget_reg(reg);
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_c(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_str_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    ic(0x66);
    rex(0, 0, r1, _NOREG, r0);
    ic(0x89);
    rx(r1, 0, r0, _NOREG, _SCL1);
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	ic(0x66);
	rex(0, 0, r0, _NOREG, _NOREG);
	ic(0x89);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
	ic(0x66);
	rex(0, 0, r0, _NOREG, _NOREG);
	ic(0x89);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_s(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_str_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, 0, r1, _NOREG, r0);
    ic(0x89);
    rx(r1, 0, r0, _NOREG, _SCL1);
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, 0, r0, _NOREG, _NOREG);
	ic(0x89);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (address_p(i0)) {
	rex(0, 0, r0, _NOREG, _NOREG);
	ic(0x89);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_i(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

#if __X64 && !__X64_32
static void
_str_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    rex(0, 1, r1, _NOREG, r0);
    ic(0x89);
    rx(r1, 0, r0, _NOREG, _SCL1);
}

static void
_sti_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
#if CAN_RIP_ADDRESS
    jit_word_t		rel = i0 - _jit->pc.w;
    rel = rel < 0 ? rel - 8 : rel + 8;
    if (can_sign_extend_int_p(rel)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x89);
	rx(r0, i0 - (_jit->pc.w + 5), _NOREG, _NOREG, _SCL8);
    }
    else
#endif
    if (can_sign_extend_int_p(i0)) {
	rex(0, WIDE, r0, _NOREG, _NOREG);
	ic(0x89);
	rx(r0, i0, _NOREG, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_l(rn(reg), r0);
	jit_unget_reg(reg);
    }
}
#endif

static void
_stxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
#if __X64_32
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_c(rn(reg), r2);
    jit_unget_reg(reg);
#else
    if (reg8_p(r2)) {
	rex(0, 0, r2, r1, r0);
	ic(0x88);
	rx(r2, 0, r0, r1, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	movr(rn(reg), r2);
	rex(0, 0, rn(reg), r1, r0);
	ic(0x88);
	rx(rn(reg), 0, r0, r1, _SCL1);
	jit_unget_reg(reg);
    }
#endif
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	if (reg8_p(r1)) {
	    rex(0, 0, r1, _NOREG, r0);
	    ic(0x88);
	    rx(r1, i0, r0, _NOREG, _SCL1);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr|jit_class_rg8);
	    movr(rn(reg), r1);
	    rex(0, 0, rn(reg), _NOREG, r0);
	    ic(0x88);
	    rx(rn(reg), i0, r0, _NOREG, _SCL1);
	    jit_unget_reg(reg);
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_c(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}

static void
_stxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_s(rn(reg), r2);
    jit_unget_reg(reg);
#else
    ic(0x66);
    rex(0, 0, r2, r1, r0);
    ic(0x89);
    rx(r2, 0, r0, r1, _SCL1);
#endif
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	ic(0x66);
	rex(0, 0, r1, _NOREG, r0);
	ic(0x89);
	rx(r1, i0, r0, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_s(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}

static void
_stxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
#if __X64_32
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_i(rn(reg), r2);
    jit_unget_reg(reg);
#else
    rex(0, 0, r2, r1, r0);
    ic(0x89);
    rx(r2, 0, r0, r1, _SCL1);
#endif
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, 0, r1, _NOREG, r0);
	ic(0x89);
	rx(r1, i0, r0, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_i(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}

#if __X64 && !__X64_32
static void
_stxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    rex(0, 1, r2, r1, r0);
    ic(0x89);
    rx(r2, 0, r0, r1, _SCL1);
}

static void
_stxi_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i0)) {
	rex(0, 1, r1, _NOREG, r0);
	ic(0x89);
	rx(r1, i0, r0, _NOREG, _SCL1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_l(rn(reg), r0, r1);
	jit_unget_reg(reg);
    }
}
#endif

static jit_word_t
_jccs(jit_state_t *_jit, jit_int32_t code, jit_word_t i0)
{
    jit_word_t		d;
    jit_word_t		w;
    w = _jit->pc.w;
    d = i0 - (w + 1);
    ic(0x70 | code);
    ic(d);
    return (w);
}

static jit_word_t
_jcc(jit_state_t *_jit, jit_int32_t code, jit_word_t i0)
{
    jit_word_t		d;
    jit_word_t		w;
    w = _jit->pc.w;
    ic(0x0f);
    d = i0 - (w + 6);
    ic(0x80 | code);
    ii(d);
    return (w);
}

static jit_word_t
_jcr(jit_state_t *_jit,
     jit_int32_t code, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    alur(X86_CMP, r0, r1);
    return (jcc(code, i0));
}

static jit_word_t
_jci(jit_state_t *_jit,
     jit_int32_t code, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    alui(X86_CMP, r0, i1);
    return (jcc(code, i0));
}

static jit_word_t
_jci0(jit_state_t *_jit, jit_int32_t code, jit_word_t i0, jit_int32_t r0)
{
    testr(r0, r0);
    return (jcc(code, i0));
}

static jit_word_t
_bltr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    return (jcr(X86_CC_L, i0, r0, r1));
}

static jit_word_t
_blti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_L, i0, r0, i1);
    else		w = jci0(X86_CC_S, i0, r0);
    return (w);
}

static jit_word_t
_bltr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    return (jcr(X86_CC_B, i0, r0, r1));
}

static jit_word_t
_blti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_B, i0, r0, i1);
    else		w = jci0(X86_CC_B, i0, r0);
    return (w);
}

static jit_word_t
_bler(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    if (r0 == r1)	w = jmpi(i0);
    else		w = jcr (X86_CC_LE, i0, r0, r1);
    return (w);
}

static jit_word_t
_blei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_LE, i0, r0, i1);
    else		w = jci0(X86_CC_LE, i0, r0);
    return (w);
}

static jit_word_t
_bler_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    if (r0 == r1)	w = jmpi(i0);
    else		w = jcr (X86_CC_BE, i0, r0, r1);
    return (w);
}

static jit_word_t
_blei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_BE, i0, r0, i1);
    else		w = jci0(X86_CC_BE, i0, r0);
    return (w);
}

static jit_word_t
_beqr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    if (r0 == r1)	w = jmpi(i0);
    else		w = jcr (X86_CC_E, i0, r0, r1);
    return (w);
}

static jit_word_t
_beqi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_E, i0, r0, i1);
    else		w = jci0(X86_CC_E, i0, r0);
    return (w);
}

static jit_word_t
_bger(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    if (r0 == r1)	w = jmpi(i0);
    else		w = jcr (X86_CC_GE, i0, r0, r1);
    return (w);
}

static jit_word_t
_bgei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_GE, i0, r0, i1);
    else		w = jci0(X86_CC_NS, i0, r0);
    return (w);
}

static jit_word_t
_bger_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    if (r0 == r1)	w = jmpi(i0);
    else		w = jcr (X86_CC_AE, i0, r0, r1);
    return (w);
}

static jit_word_t
_bgei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_AE, i0, r0, i1);
    else		w = jmpi(i0);
    return (w);
}

static jit_word_t
_bgtr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    return (jcr(X86_CC_G, i0, r0, r1));
}

static jit_word_t
_bgti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    return (jci(X86_CC_G, i0, r0, i1));
}

static jit_word_t
_bgtr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    return (jcr(X86_CC_A, i0, r0, r1));
}

static jit_word_t
_bgti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_A, i0, r0, i1);
    else		w = jci0(X86_CC_NE, i0, r0);
    return (w);
}

static jit_word_t
_bner(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    return (jcr(X86_CC_NE, i0, r0, r1));
}

static jit_word_t
_bnei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1)		w = jci (X86_CC_NE, i0, r0, i1);
    else		w = jci0(X86_CC_NE, i0, r0);
    return (w);
}

static jit_word_t
_bmsr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    testr(r0, r1);
    return (jnz(i0));
}

static jit_word_t
_bmsi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_zero_extend_int_p(i1))
	testi(r0, i1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	testr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (jnz(i0));
}

static jit_word_t
_bmcr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    testr(r0, r1);
    return (jz(i0));
}

static jit_word_t
_bmci(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_zero_extend_int_p(i1))
	testi(r0, i1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	testr(r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (jz(i0));
}

static jit_word_t
_boaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    iaddr(r0, r1);
    return (jo(i0));
}

static jit_word_t
_boaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	iaddi(r0, i1);
	return (jo(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (boaddr(i0, r0, rn(reg)));
}

static jit_word_t
_boaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    iaddr(r0, r1);
    return (jc(i0));
}

static jit_word_t
_boaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	iaddi(r0, i1);
	return (jc(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (boaddr_u(i0, r0, rn(reg)));
}

static jit_word_t
_bxaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    iaddr(r0, r1);
    return (jno(i0));
}

static jit_word_t
_bxaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	iaddi(r0, i1);
	return (jno(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (bxaddr(i0, r0, rn(reg)));
}

static jit_word_t
_bxaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    iaddr(r0, r1);
    return (jnc(i0));
}

static jit_word_t
_bxaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	iaddi(r0, i1);
	return (jnc(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (bxaddr_u(i0, r0, rn(reg)));
}

static jit_word_t
_bosubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    isubr(r0, r1);
    return (jo(i0));
}

static jit_word_t
_bosubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	isubi(r0, i1);
	return (jo(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (bosubr(i0, r0, rn(reg)));
}

static jit_word_t
_bosubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    isubr(r0, r1);
    return (jc(i0));
}

static jit_word_t
_bosubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	isubi(r0, i1);
	return (jc(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (bosubr_u(i0, r0, rn(reg)));
}

static jit_word_t
_bxsubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    isubr(r0, r1);
    return (jno(i0));
}

static jit_word_t
_bxsubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	isubi(r0, i1);
	return (jno(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (bxsubr(i0, r0, rn(reg)));
}

static jit_word_t
_bxsubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    isubr(r0, r1);
    return (jnc(i0));
}

static jit_word_t
_bxsubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (can_sign_extend_int_p(i1)) {
	isubi(r0, i1);
	return (jnc(i0));
    }
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    jit_unget_reg(reg);
    return (bxsubr_u(i0, r0, rn(reg)));
}

static void
_callr(jit_state_t *_jit, jit_int32_t r0)
{
    rex(0, 0, _NOREG, _NOREG, r0);
    ic(0xff);
    mrm(0x03, 0x02, r7(r0));
}

static jit_word_t
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_word_t		d;
    jit_word_t		l = _jit->pc.w + 5;
    d = i0 - l;
#if __X64
    if (
#  if __X64_32
	!((d < 0) ^ (l < 0)) &&
#  endif
	(jit_int32_t)d == d) {
#endif
	w = _jit->pc.w;
	ic(0xe8);
	ii(d);
#if __X64
    }
    else
	w = calli_p(i0);
#endif
    return (w);
}

#if __X64
static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    w = movi_p(rn(reg), i0);
    callr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}
#endif

static void
_jmpr(jit_state_t *_jit, jit_int32_t r0)
{
    rex(0, 0, _NOREG, _NOREG, r0);
    ic(0xff);
    mrm(0x03, 0x04, r7(r0));
}

static jit_word_t
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_word_t		d;
    jit_word_t		l = _jit->pc.w + 5;
    d = i0 - l;
#if __X64
    if (
#  if __X64_32
	!((d < 0) ^ (l < 0)) &&
#  endif
	(jit_int32_t)d == d) {
#endif
	w = _jit->pc.w;
	ic(0xe9);
	ii(d);
#if __X64
    }
    else
	w = jmpi_p(i0);
#endif
    return (w);
}

#if __X64
static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    w = movi_p(rn(reg), i0);
    jmpr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}
#endif

static jit_word_t
_jmpsi(jit_state_t *_jit, jit_uint8_t i0)
{
    jit_word_t		w = _jit->pc.w;
    ic(0xeb);
    ic(i0);
    return (w);
}

static void
_prolog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg, offs;
    if (_jitc->function->define_frame || _jitc->function->assume_frame) {
	jit_int32_t	frame = -_jitc->function->frame;
	jit_check_frame();
	assert(_jitc->function->self.aoff >= frame);
	if (_jitc->function->assume_frame)
	    return;
	_jitc->function->self.aoff = frame;
    }
    if (_jitc->function->allocar)
	_jitc->function->self.aoff &= -16;
#if __X64 && (__CYGWIN__ || _WIN32)
    _jitc->function->stack = (((/* first 32 bytes must be allocated */
				(_jitc->function->self.alen > 32 ?
				 _jitc->function->self.alen : 32) -
				/* align stack at 16 bytes */
				_jitc->function->self.aoff) + 15) & -16);
#else
    _jitc->function->stack = (((_jitc->function->self.alen -
			       _jitc->function->self.aoff) + 15) & -16);
#endif

    if (_jitc->function->stack)
	_jitc->function->need_stack = 1;

    if (!_jitc->function->need_frame && !_jitc->function->need_stack) {
	/* check if any callee save register needs to be saved */
	for (reg = 0; reg < _jitc->reglen; ++reg)
	    if (jit_regset_tstbit(&_jitc->function->regset, reg) &&
		(_rvs[reg].spec & jit_class_sav)) {
		_jitc->function->need_stack = 1;
		break;
	    }
    }

    if (_jitc->function->need_frame || _jitc->function->need_stack)
	subi(_RSP_REGNO, _RSP_REGNO, jit_framesize());
    /* callee save registers */
    for (reg = 0, offs = REAL_WORDSIZE; reg < jit_size(iregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, iregs[reg])) {
	    stxi(offs, _RSP_REGNO, rn(iregs[reg]));
	    offs += REAL_WORDSIZE;
	}
    }
#if __X64 && (__CYGWIN__ || _WIN32)
    for (reg = 0; reg < jit_size(fregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, fregs[reg])) {
	    sse_stxi_d(offs, _RSP_REGNO, rn(fregs[reg]));
	    offs += sizeof(jit_float64_t);
	}
    }
#endif

    if (_jitc->function->need_frame) {
	stxi(0, _RSP_REGNO, _RBP_REGNO);
	movr(_RBP_REGNO, _RSP_REGNO);
    }

    /* alloca */
    if (_jitc->function->stack)
	subi(_RSP_REGNO, _RSP_REGNO, _jitc->function->stack);
    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _RBP_REGNO, rn(reg));
	jit_unget_reg(reg);
    }

#if __X64 && !(__CYGWIN__ || _WIN32)
    if (_jitc->function->self.call & jit_call_varargs) {
	jit_word_t	nofp_code;

	/* Save gp registers in the save area, if any is a vararg */
	for (reg = first_gp_from_offset(_jitc->function->vagp);
	     jit_arg_reg_p(reg); ++reg)
	    stxi(_jitc->function->vaoff + first_gp_offset +
		 reg * 8, _RBP_REGNO, rn(JIT_RA0 - reg));

	reg = first_fp_from_offset(_jitc->function->vafp);
	if (jit_arg_f_reg_p(reg)) {
	    /* Skip over if no float registers were passed as argument */
	    /* test %al, %al */
	    ic(0x84);
	    ic(0xc0);
	    nofp_code = jes(0);

	    /* Save fp registers in the save area, if any is a vararg */
	    /* Note that the full 16 byte xmm is not saved, because
	     * lightning only handles float and double, and, while
	     * attempting to provide a va_list compatible pointer as
	     * jit_va_start return, does not guarantee it (on all ports). */
	    for (; jit_arg_f_reg_p(reg); ++reg)
		sse_stxi_d(_jitc->function->vaoff + first_fp_offset +
			   reg * va_fp_increment, _RBP_REGNO, rn(_XMM0 - reg));

	    patch_at(nofp_code, _jit->pc.w);
	}
    }
#endif
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg, offs;
    if (_jitc->function->assume_frame)
	return;
    if (_jitc->function->need_frame)
	movr(_RSP_REGNO, _RBP_REGNO);

    /* callee save registers */
    for (reg = 0, offs = REAL_WORDSIZE; reg < jit_size(iregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, iregs[reg])) {
	    ldxi(rn(iregs[reg]), _RSP_REGNO, offs);
	    offs += REAL_WORDSIZE;
	}
    }
#if __X64 && (__CYGWIN__ || _WIN32)
    for (reg = 0; reg < jit_size(fregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, fregs[reg])) {
	    sse_ldxi_d(rn(fregs[reg]), _RSP_REGNO, offs);
	    offs += sizeof(jit_float64_t);
	}
    }
#endif

    if (_jitc->function->need_frame) {
	ldxi(_RBP_REGNO, _RSP_REGNO, 0);
	addi(_RSP_REGNO, _RSP_REGNO, jit_framesize());
    }
    /* This condition does not happen as much as expected because
     * it is not safe to not create a frame pointer if any function
     * is called, even jit functions, as those might call external
     * functions. */
    else if (_jitc->function->need_stack)
	addi(_RSP_REGNO, _RSP_REGNO, jit_framesize());

    ic(0xc3);
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
#if __X32 || __CYGWIN__ || _WIN32
    assert(_jitc->function->self.call & jit_call_varargs);
    addi(r0, _RBP_REGNO, jit_selfsize());
#else
    jit_int32_t		reg;

    assert(_jitc->function->self.call & jit_call_varargs);

    /* Return jit_va_list_t in the register argument */
    addi(r0, _RBP_REGNO, _jitc->function->vaoff);
    reg = jit_get_reg(jit_class_gpr);

    /* Initialize gp offset in the save area. */
    movi(rn(reg), _jitc->function->vagp);
    stxi_i(offsetof(jit_va_list_t, gpoff), r0, rn(reg));

    /* Initialize fp offset in the save area. */
    movi(rn(reg), _jitc->function->vafp);
    stxi_i(offsetof(jit_va_list_t, fpoff), r0, rn(reg));

    /* Initialize overflow pointer to the first stack argument. */
    addi(rn(reg), _RBP_REGNO, jit_selfsize());
    stxi(offsetof(jit_va_list_t, over), r0, rn(reg));

    /* Initialize register save area pointer. */
    addi(rn(reg), r0, first_gp_offset);
    stxi(offsetof(jit_va_list_t, save), r0, rn(reg));

    jit_unget_reg(reg);
#endif
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if __X32 || __CYGWIN__ || _WIN32
    assert(_jitc->function->self.call & jit_call_varargs);
    ldr(r0, r1);
    addi(r1, r1, va_gp_increment);
#else
    jit_int32_t		rg0;
    jit_int32_t		rg1;
    jit_word_t		ge_code;
    jit_word_t		lt_code;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg(jit_class_gpr);
    rg1 = jit_get_reg(jit_class_gpr);

    /* Load the gp offset in save area in the first temporary. */
    ldxi_i(rn(rg0), r1, offsetof(jit_va_list_t, gpoff));

    /* Jump over if there are no remaining arguments in the save area. */
    icmpi(rn(rg0), va_gp_max_offset);
    ge_code = jaes(0);

    /* Load the save area pointer in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, save));

    /* Load the vararg argument in the first argument. */
    ldxr(r0, rn(rg1), rn(rg0));

    /* Update the gp offset. */
    addi(rn(rg0), rn(rg0), 8);
    stxi_i(offsetof(jit_va_list_t, gpoff), r1, rn(rg0));

    /* Will only need one temporary register below. */
    jit_unget_reg(rg1);

    /* Jump over overflow code. */
    lt_code = jmpsi(0);

    /* Where to land if argument is in overflow area. */
    patch_at(ge_code, _jit->pc.w);

    /* Load overflow pointer. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, over));

    /* Load argument. */
    ldr(r0, rn(rg0));

    /* Update overflow pointer. */
    addi(rn(rg0), rn(rg0), va_gp_increment);
    stxi(offsetof(jit_va_list_t, over), r1, rn(rg0));

    /* Where to land if argument is in save area. */
    patch_at(lt_code, _jit->pc.w);

    jit_unget_reg(rg0);
#endif
}

/* The x87 boolean argument tells if will put the result in a x87
 * register if non false, in a sse register otherwise. */
static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_bool_t x87)
{
#if __X32 || __CYGWIN__ || _WIN32
    assert(_jitc->function->self.call & jit_call_varargs);
    if (x87)
	x87_ldr_d(r0, r1);
    else
	sse_ldr_d(r0, r1);
    addi(r1, r1, 8);
#else
    jit_int32_t		rg0;
    jit_int32_t		rg1;
    jit_word_t		ge_code;
    jit_word_t		lt_code;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg(jit_class_gpr);
    rg1 = jit_get_reg(jit_class_gpr);

    /* Load the fp offset in save area in the first temporary. */
    ldxi_i(rn(rg0), r1, offsetof(jit_va_list_t, fpoff));

    /* Jump over if there are no remaining arguments in the save area. */
    icmpi(rn(rg0), va_fp_max_offset);
    ge_code = jaes(0);

    /* Load the save area pointer in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, save));

    /* Load the vararg argument in the first argument. */
    if (x87)
	x87_ldxr_d(r0, rn(rg1), rn(rg0));
    else
	sse_ldxr_d(r0, rn(rg1), rn(rg0));

    /* Update the fp offset. */
    addi(rn(rg0), rn(rg0), va_fp_increment);
    stxi_i(offsetof(jit_va_list_t, fpoff), r1, rn(rg0));

    /* Will only need one temporary register below. */
    jit_unget_reg(rg1);

    /* Jump over overflow code. */
    lt_code = jmpsi(0);

    /* Where to land if argument is in overflow area. */
    patch_at(ge_code, _jit->pc.w);

    /* Load overflow pointer. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, over));

    /* Load argument. */
    if (x87)
	x87_ldr_d(r0, rn(rg0));
    else
	sse_ldr_d(r0, rn(rg0));

    /* Update overflow pointer. */
    addi(rn(rg0), rn(rg0), 8);
    stxi(offsetof(jit_va_list_t, over), r1, rn(rg0));

    /* Where to land if argument is in save area. */
    patch_at(lt_code, _jit->pc.w);

    jit_unget_reg(rg0);
#endif
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    jit_word_t		 disp;
    jit_uint8_t		*code = (jit_uint8_t *)instr;
    ++instr;
    switch (code[0]) {
	/* movi_p */
	case 0xb8 ... 0xbf:
	    *(jit_word_t *)instr = label;
	    break;
	    /* forward pc relative address known to be in range */
#if CAN_RIP_ADDRESS
	/* movi */
	case 0x8d:
	    ++instr;
	    goto apply;
#endif
	/* jcc */
	case 0x0f:
	    ++instr;
	    if (code[1] < 0x80 || code[1] > 0x8f)
		goto fail;
	/* calli */
	case 0xe8:
	/* jmpi */
	case 0xe9:
#if CAN_RIP_ADDRESS
	apply:
#endif
	    disp = label - (instr + 4);
	    assert((jit_int32_t)disp == disp);
	    *(jit_int32_t *)instr = disp;
	    break;
	    /* jccs */
	case 0x70 ... 0x7f:
	    /* jmpsi */
	case 0xeb:
	    disp = label - (instr + 1);
	    assert((jit_int8_t)disp == disp);
	    *(jit_int8_t *)instr = disp;
	    break;
	default:
	fail:
	    abort();
    }
}
#endif
