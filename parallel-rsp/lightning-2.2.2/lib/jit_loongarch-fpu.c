/*
 * Copyright (C) 2022-2023  Free Software Foundation, Inc.
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

#if PROTO
# define       FADD_S(fd, fj, fk)	  orrr(0x201,	   fk,	  fj,	 fd)
# define       FADD_D(fd, fj, fk)	  orrr(0x202,	   fk,	  fj,	 fd)
# define       FSUB_S(fd, fj, fk)	  orrr(0x205,	   fk,	  fj,	 fd)
# define       FSUB_D(fd, fj, fk)	  orrr(0x206,	   fk,	  fj,	 fd)
# define       FMUL_S(fd, fj, fk)	  orrr(0x209,	   fk,	  fj,	 fd)
# define       FMUL_D(fd, fj, fk)	  orrr(0x20a,	   fk,	  fj,	 fd)
# define       FDIV_S(fd, fj, fk)	  orrr(0x20d,	   fk,	  fj,	 fd)
# define       FDIV_D(fd, fj, fk)	  orrr(0x20e,	   fk,	  fj,	 fd)
# define       FMAX_S(fd, fj, fk)	  orrr(0x211,	   fk,	  fj,	 fd)
# define       FMAX_D(fd, fj, fk)	  orrr(0x212,	   fk,	  fj,	 fd)
# define       FMIN_S(fd, fj, fk)	  orrr(0x215,	   fk,	  fj,	 fd)
# define       FMIN_D(fd, fj, fk)	  orrr(0x216,	   fk,	  fj,	 fd)
# define      FMAXA_S(fd, fj, fk)	  orrr(0x219,	   fk,	  fj,	 fd)
# define      FMAXA_D(fd, fj, fk)	  orrr(0x21a,	   fk,	  fj,	 fd)
# define      FMINA_S(fd, fj, fk)	  orrr(0x21d,	   fk,	  fj,	 fd)
# define      FMINA_D(fd, fj, fk)	  orrr(0x21e,	   fk,	  fj,	 fd)
# define    FSCALEB_S(fd, fj, fk)	  orrr(0x221,	   fk,	  fj,	 fd)
# define    FSCALEB_D(fd, fj, fk)	  orrr(0x222,	   fk,	  fj,	 fd)
# define  FCOPYSIGN_S(fd, fj, fk)	  orrr(0x225,	   fk,	  fj,	 fd)
# define  FCOPYSIGN_D(fd, fj, fk)	  orrr(0x226,	   fk,	  fj,	 fd)
# define       FABS_S(fd, fj)		 ou5rr(0x228,	 0x01,	  fj,	 fd)
# define       FABS_D(fd, fj)		 ou5rr(0x228,	 0x02,	  fj,	 fd)
# define       FNEG_S(fd, fj)		 ou5rr(0x228,	 0x05,	  fj,	 fd)
# define       FNEG_D(fd, fj)		 ou5rr(0x228,	 0x06,	  fj,	 fd)
# define      FGLOB_S(fd, fj)		 ou5rr(0x228,	 0x09,	  fj,	 fd)
# define      FGLOB_D(fd, fj)		 ou5rr(0x228,	 0x0a,	  fj,	 fd)
# define     FCLASS_S(fd, fj)		 ou5rr(0x228,	 0x0d,	  fj,	 fd)
# define     FCLASS_D(fd, fj)		 ou5rr(0x228,	 0x0e,	  fj,	 fd)
# define      FSQRT_S(fd, fj)		 ou5rr(0x228,	 0x11,	  fj,	 fd)
# define      FSQRT_D(fd, fj)		 ou5rr(0x228,	 0x12,	  fj,	 fd)
# define     FRECIP_S(fd, fj)		 ou5rr(0x228,	 0x15,	  fj,	 fd)
# define     FRECIP_D(fd, fj)		 ou5rr(0x228,	 0x16,	  fj,	 fd)
# define     FRSQRT_S(fd, fj)		 ou5rr(0x228,	 0x19,	  fj,	 fd)
# define     FRSQRT_D(fd, fj)		 ou5rr(0x228,	 0x1a,	  fj,	 fd)
# define       FMOV_S(fd, fj)		 ou5rr(0x229,	 0x05,	  fj,	 fd)
# define       FMOV_D(fd, fj)		 ou5rr(0x229,	 0x06,	  fj,	 fd)
# define   MOVGR2FR_W(fd, rj)		 ou5rr(0x229,	 0x09,	  rj,	 fd)
# define   MOVGR2FR_D(fd, rj)		 ou5rr(0x229,	 0x0a,	  rj,	 fd)
# define  MOVGR2FRH_W(fd, rj)		 ou5rr(0x229,	 0x0b,	  rj,	 fd)
# define   MOVFR2GR_S(rd, fj)		 ou5rr(0x229,	 0x0d,	  fj,	 rd)
# define   MOVFR2GR_D(rd, fj)		 ou5rr(0x229,	 0x0e,	  fj,	 rd)
# define  MOVFRH2GR_S(rd, fj)		 ou5rr(0x229,	 0x0f,	  fj,	 rd)
# define   MOVGR2FCSR(fc, rj)		 ou5rr(0x229,	 0x10,	  rj,	 fc)
# define   MOVFCSR2GR(rd, fc)		 ou5rr(0x229,	 0x12,	  fc,	 rd)
# define     MOVFR2CF(cd, fj)		 o5r23(0x229,	 0x14,	  fj, 0, cd)
# define     MOVCF2FR(fd, cj)		 o523r(0x229,	 0x15, 0, cj,	 fd)
# define     MOVGR2CF(cd, fj)		 o5r23(0x229,	 0x16,	  fj, 0, cd)
# define     MOVCF2GR(fd, cj)		 o523r(0x229,	 0x17, 0, cj,	 fd)
# define     FCVT_S_D(fd, fj)		 ou5rr(0x232,	 0x06,	  fj,	 fd)
# define     FCVT_D_S(fd, fj)		 ou5rr(0x232,	 0x09,	  fj,	 fd)
# define  FTINTRM_W_S(fd, fj)		 ou5rr(0x234,	 0x01,	  fj,	 fd)
# define  FTINTRM_W_D(fd, fj)		 ou5rr(0x234,	 0x02,	  fj,	 fd)
# define  FTINTRM_L_S(fd, fj)	 	 ou5rr(0x234,	 0x09,	  fj,	 fd)
# define  FTINTRM_L_D(fd, fj)		 ou5rr(0x234,	 0x0a,	  fj,	 fd)
# define  FTINTRP_W_S(fd, fj)		 ou5rr(0x234,	 0x11,	  fj,	 fd)
# define  FTINTRP_W_D(fd, fj)		 ou5rr(0x234,	 0x12,	  fj,	 fd)
# define  FTINTRP_L_S(fd, fj)		 ou5rr(0x234,	 0x19,	  fj,	 fd)
# define  FTINTRP_L_D(fd, fj)		 ou5rr(0x234,	 0x1a,	  fj,	 fd)
# define  FTINTRZ_W_S(fd, fj)		 ou5rr(0x235,	 0x01,	  fj,	 fd)
# define  FTINTRZ_W_D(fd, fj)		 ou5rr(0x235,	 0x02,	  fj,	 fd)
# define  FTINTRZ_L_S(fd, fj)		 ou5rr(0x235,	 0x09,	  fj,	 fd)
# define  FTINTRZ_L_D(fd, fj)		 ou5rr(0x235,	 0x0a,	  fj,	 fd)
# define FTINTRNE_W_S(fd, fj)		 ou5rr(0x235,	 0x11,	  fj,	 fd)
# define FTINTRNE_W_D(fd, fj)		 ou5rr(0x235,	 0x12,	  fj,	 fd)
# define FTINTRNE_L_S(fd, fj)		 ou5rr(0x235,	 0x19,	  fj,	 fd)
# define FTINTRNE_L_D(fd, fj)		 ou5rr(0x235,	 0x1a,	  fj,	 fd)
# define    FTINT_W_S(fd, fj)		 ou5rr(0x236,	 0x01,	  fj,	 fd)
# define    FTINT_W_D(fd, fj)		 ou5rr(0x236,	 0x02,	  fj,	 fd)
# define    FTINT_L_S(fd, fj)		 ou5rr(0x236,	 0x09,	  fj,	 fd)
# define    FTINT_L_D(fd, fj)		 ou5rr(0x236,	 0x0a,	  fj,	 fd)
# define    FFINT_S_W(fd, fj)		 ou5rr(0x23a,	 0x04,	  fj,	 fd)
# define    FFINT_S_L(fd, fj)		 ou5rr(0x23a,	 0x06,	  fj,	 fd)
# define    FFINT_D_W(fd, fj)		 ou5rr(0x23a,	 0x08,	  fj,	 fd)
# define    FFINT_D_L(fd, fj)		 ou5rr(0x23a,	 0x0a,	  fj,	 fd)
# define      FRINT_S(fd, fj)		 ou5rr(0x23c,	 0x11,	  fj,	 fd)
# define      FRINT_D(fd, fj)		 ou5rr(0x23c,	 0x12,	  fj,	 fd)
# define      FMADD_S(fd, fj, fk, fa)	 orrrr(0x081,  fa, fk,	  fj,	 fd)
# define      FMADD_D(fd, fj, fk, fa)	 orrrr(0x082,  fa, fk,	  fj,	 fd)
# define      FMSUB_S(fd, fj, fk, fa)	 orrrr(0x085,  fa, fk,	  fj,	 fd)
# define      FMSUB_D(fd, fj, fk, fa)	 orrrr(0x086,  fa, fk,	  fj,	 fd)
# define     FNMADD_S(fd, fj, fk, fa)	 orrrr(0x089,  fa, fk,	  fj,	 fd)
# define     FNMADD_D(fd, fj, fk, fa)	 orrrr(0x08a,  fa, fk,	  fj,	 fd)
# define     FNMSUB_S(fd, fj, fk, fa)	 orrrr(0x08d,  fa, fk,	  fj,	 fd)
# define     FNMSUB_D(fd, fj, fk, fa)	 orrrr(0x08e,  fa, fk,	  fj,	 fd)
/* No QNaN exception */
# define	_CAF		0x00
# define	_CUN		0x08
# define	_CEQ		0x04
# define	_CUEQ		0x0c
# define	_CLT		0x02
# define	_CULT		0x0a
# define	_CLE		0x06
# define	_CULE		0x0e
# define	_CNE		0x10
# define	_COR		0x14
# define	_CUNE		0x18
/* QNaN exception */
# define	_SAF		0x01
# define	_SUN		0x09
# define	_SEQ		0x05
# define	_SUEQ		0x0D
# define	_SLT		0x03
# define	_SULT		0x0B
# define	_SLE		0x07
# define	_SULE		0x0F
# define	_SNE		0x11
# define	_SOR		0x15
# define	_SUNE		0x19
# define  FCMP_cond_S(cc, cd, fj, fk) ou5rru2u3(0x0c1,  cc, fk,	  fj, 0, cd)
# define   FCMP_CAF_S(cd, fj, fk)  FCMP_cond_S( _CAF, cd, fj, fk)
# define   FCMP_CUN_S(cd, fj, fk)  FCMP_cond_S( _CUN, cd, fj, fk)
# define   FCMP_CEQ_S(cd, fj, fk)  FCMP_cond_S( _CEQ, cd, fj, fk)
# define  FCMP_CUEQ_S(cd, fj, fk)  FCMP_cond_S(_CUEQ, cd, fj, fk)
# define   FCMP_CLT_S(cd, fj, fk)  FCMP_cond_S( _CLT, cd, fj, fk)
# define  FCMP_CULT_S(cd, fj, fk)  FCMP_cond_S(_CULT, cd, fj, fk)
# define   FCMP_CLE_S(cd, fj, fk)  FCMP_cond_S( _CLE, cd, fj, fk)
# define  FCMP_CULE_S(cd, fj, fk)  FCMP_cond_S(_CULE, cd, fj, fk)
# define   FCMP_CNE_S(cd, fj, fk)  FCMP_cond_S( _CNE, cd, fj, fk)
# define   FCMP_COR_S(cd, fj, fk)  FCMP_cond_S( _COR, cd, fj, fk)
# define  FCMP_CUNE_S(cd, fj, fk)  FCMP_cond_S(_CUNE, cd, fj, fk)
# define   FCMP_SAF_S(cd, fj, fk)  FCMP_cond_S( _CAF, cd, fj, fk)
# define   FCMP_SUN_S(cd, fj, fk)  FCMP_cond_S( _CUN, cd, fj, fk)
# define   FCMP_SEQ_S(cd, fj, fk)  FCMP_cond_S( _CEQ, cd, fj, fk)
# define  FCMP_SUEQ_S(cd, fj, fk)  FCMP_cond_S(_CUEQ, cd, fj, fk)
# define   FCMP_SLT_S(cd, fj, fk)  FCMP_cond_S( _CLT, cd, fj, fk)
# define  FCMP_SULT_S(cd, fj, fk)  FCMP_cond_S(_CULT, cd, fj, fk)
# define   FCMP_SLE_S(cd, fj, fk)  FCMP_cond_S( _CLE, cd, fj, fk)
# define  FCMP_SULE_S(cd, fj, fk)  FCMP_cond_S(_CULE, cd, fj, fk)
# define   FCMP_SNE_S(cd, fj, fk)  FCMP_cond_S( _CNE, cd, fj, fk)
# define   FCMP_SOR_S(cd, fj, fk)  FCMP_cond_S( _COR, cd, fj, fk)
# define  FCMP_SUNE_S(cd, fj, fk)  FCMP_cond_S(_CUNE, cd, fj, fk)
# define  FCMP_cond_D(cc, cd, fj, fk) ou5rru2u3(0x0c2,  cc, fk,	  fj, 0, cd)
# define   FCMP_CAF_D(cd, fj, fk)  FCMP_cond_D( _CAF, cd, fj, fk)
# define   FCMP_CUN_D(cd, fj, fk)  FCMP_cond_D( _CUN, cd, fj, fk)
# define   FCMP_CEQ_D(cd, fj, fk)  FCMP_cond_D( _CEQ, cd, fj, fk)
# define  FCMP_CUEQ_D(cd, fj, fk)  FCMP_cond_D(_CUEQ, cd, fj, fk)
# define   FCMP_CLT_D(cd, fj, fk)  FCMP_cond_D( _CLT, cd, fj, fk)
# define  FCMP_CULT_D(cd, fj, fk)  FCMP_cond_D(_CULT, cd, fj, fk)
# define   FCMP_CLE_D(cd, fj, fk)  FCMP_cond_D( _CLE, cd, fj, fk)
# define  FCMP_CULE_D(cd, fj, fk)  FCMP_cond_D(_CULE, cd, fj, fk)
# define   FCMP_CNE_D(cd, fj, fk)  FCMP_cond_D( _CNE, cd, fj, fk)
# define   FCMP_COR_D(cd, fj, fk)  FCMP_cond_D( _COR, cd, fj, fk)
# define  FCMP_CUNE_D(cd, fj, fk)  FCMP_cond_D(_CUNE, cd, fj, fk)
# define   FCMP_SAF_D(cd, fj, fk)  FCMP_cond_D( _CAF, cd, fj, fk)
# define   FCMP_SUN_D(cd, fj, fk)  FCMP_cond_D( _CUN, cd, fj, fk)
# define   FCMP_SEQ_D(cd, fj, fk)  FCMP_cond_D( _CEQ, cd, fj, fk)
# define  FCMP_SUEQ_D(cd, fj, fk)  FCMP_cond_D(_CUEQ, cd, fj, fk)
# define   FCMP_SLT_D(cd, fj, fk)  FCMP_cond_D( _CLT, cd, fj, fk)
# define  FCMP_SULT_D(cd, fj, fk)  FCMP_cond_D(_CULT, cd, fj, fk)
# define   FCMP_SLE_D(cd, fj, fk)  FCMP_cond_D( _CLE, cd, fj, fk)
# define  FCMP_SULE_D(cd, fj, fk)  FCMP_cond_D(_CULE, cd, fj, fk)
# define   FCMP_SNE_D(cd, fj, fk)  FCMP_cond_D( _CNE, cd, fj, fk)
# define   FCMP_SOR_D(cd, fj, fk)  FCMP_cond_D( _COR, cd, fj, fk)
# define  FCMP_SUNE_D(cd, fj, fk)  FCMP_cond_D(_CUNE, cd, fj, fk)
# define 	FSEL(fd, fj, fk, u3)	ou3rrr(0x340,  u3, fk,	  fj,	 fd)
# define        FLD_S(fd, rj, si12)	os12rr(0x0ac,	 si12,	  rj,	 fd)
# define        FST_S(fd, rj, si12)	os12rr(0x0ad,	 si12,	  rj,	 fd)
# define        FLD_D(fd, rj, si12)	os12rr(0x0ae,	 si12,	  rj,	 fd)
# define        FST_D(fd, rj, si12)	os12rr(0x0af,	 si12,	  rj,	 fd)
# define       FLDX_S(fd, rj, rk)	 orrr(0x7060,	   rk,	  rj,	 fd)
# define       FLDX_D(fd, rj, rk)	 orrr(0x7068,	   rk,	  rj,	 fd)
# define       FSTX_S(fd, rj, rk)	 orrr(0x7070,	   rk,	  rj,	 fd)
# define       FSTX_D(fd, rj, rk)	 orrr(0x7078,	   rk,	  rj,	 fd)
# define      FLDGT_S(fd, rj, rk)	 orrr(0x70e8,	   rk,	  rj,	 fd)
# define      FLDGT_D(fd, rj, rk)	 orrr(0x70e9,	   rk,	  rj,	 fd)
# define      FLDLE_S(fd, rj, rk)	 orrr(0x70ea,	   rk,	  rj,	 fd)
# define      FLDLE_D(fd, rj, rk)	 orrr(0x70eb,	   rk,	  rj,	 fd)
# define      FSTGT_S(fd, rj, rk)	 orrr(0x70ec,	   rk,	  rj,	 fd)
# define      FSTGT_D(fd, rj, rk)	 orrr(0x70ed,	   rk,	  rj,	 fd)
# define      FSTLE_S(fd, rj, rk)	 orrr(0x70ee,	   rk,	  rj,	 fd)
# define      FSTLE_D(fd, rj, rk)	 orrr(0x70ef,	   rk,	  rj,	 fd)
# define truncr_f_i(r0, r1)		_truncr_f_i(_jit, r0, r1)
static void _truncr_f_i(jit_state_t*, jit_int32_t, jit_int32_t);
# define truncr_d_i(r0, r1)		_truncr_d_i(_jit, r0, r1)
static void _truncr_d_i(jit_state_t*, jit_int32_t, jit_int32_t);
# define truncr_f_l(r0, r1)		_truncr_f_l(_jit, r0, r1)
static void _truncr_f_l(jit_state_t*, jit_int32_t, jit_int32_t);
# define truncr_d_l(r0, r1)		_truncr_d_l(_jit, r0, r1)
static void _truncr_d_l(jit_state_t*, jit_int32_t, jit_int32_t);
# define addr_f(r0, r1, r2)		FADD_S(r0, r1, r2)
# define addi_f(r0, r1, i0)		_addi_f(_jit, r0, r1, i0)
static void _addi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define subr_f(r0, r1, r2)		FSUB_S(r0, r1, r2)
# define subi_f(r0, r1, i0)		_subi_f(_jit, r0, r1, i0)
static void _subi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define rsbr_f(r0, r1, r2)		FSUB_S(r0, r2, r1)
# define rsbi_f(r0, r1, i0)		_rsbi_f(_jit, r0, r1, i0)
static void _rsbi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define mulr_f(r0, r1, r2)		FMUL_S(r0, r1, r2)
# define muli_f(r0, r1, i0)		_muli_f(_jit, r0, r1, i0)
static void _muli_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define divr_f(r0, r1, r2)		FDIV_S(r0, r1, r2)
# define divi_f(r0, r1, i0)		_divi_f(_jit, r0, r1, i0)
static void _divi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define absr_f(r0, r1)			FABS_S(r0, r1)
# define negr_f(r0, r1)			FNEG_S(r0, r1)
# define sqrtr_f(r0, r1)		FSQRT_S(r0, r1)
# define fmar_f(r0, r1, r2, r3)		FMADD_S(r0, r1, r2, r3)
# define fmsr_f(r0, r1, r2, r3)		FMSUB_S(r0, r1, r2, r3)
# define fnmar_f(r0, r1, r2, r3)	FNMADD_S(r0, r1, r2, r3)
# define fnmsr_f(r0, r1, r2, r3)	FNMSUB_S(r0, r1, r2, r3)
# define extr_f(r0, r1)			_extr_f(_jit, r0, r1)
static void _extr_f(jit_state_t*, jit_int32_t, jit_int32_t);
# define ldr_f(r0, r1)			FLD_S(r0, r1, 0)
# define ldi_f(r0, i0)			_ldi_f(_jit, r0, i0)
static void _ldi_f(jit_state_t*, jit_int32_t, jit_word_t);
# define ldxr_f(r0, r1, r2)		FLDX_S(r0, r1, r2)
# define ldxi_f(r0, r1, i0)		_ldxi_f(_jit, r0, r1, i0)
static void _ldxi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define unldr_x(r0, r1, i0)		_unldr_x(_jit, r0, r1, i0)
static void _unldr_x(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define unldi_x(r0, i0, i1)		_unldi_x(_jit, r0, i0, i1)
static void _unldi_x(jit_state_t*, jit_int32_t, jit_word_t, jit_word_t);
# define str_f(r0, r1)			FST_S(r1, r0, 0)
# define sti_f(i0, r0)			_sti_f(_jit, i0, r0)
static void _sti_f(jit_state_t*, jit_word_t, jit_int32_t);
# define stxr_f(r0, r1, r2)		FSTX_S(r2, r1, r0)
# define stxi_f(i0, r0, r1)		_stxi_f(_jit, i0, r0, r1)
static void _stxi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
#define unstr_x(r0, r1, i0)		_unstr_x(_jit, r0, r1, i0)
static void _unstr_x(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#define unsti_x(i0, r0, i1)		_unsti_x(_jit, i0, r0, i1)
static void _unsti_x(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define movr_f(r0, r1)			FMOV_S(r0, r1)
# define movi_f(r0, i0)			_movi_f(_jit, r0, i0)
static void _movi_f(jit_state_t*, jit_int32_t, jit_float32_t);
#  define movr_f_w(r0, r1)		MOVFR2GR_S(r0, r1)
#  define movr_w_f(r0, r1)		MOVGR2FR_W(r0, r1)
#define movi_w_f(r0, i0)		_movi_w_f(_jit, r0, i0)
static void _movi_w_f(jit_state_t*, jit_int32_t, jit_word_t);
# define extr_d_f(r0, r1)		FCVT_S_D(r0, r1)
# define ltr_f(r0, r1, r2)		_ltr_f(_jit, r0, r1, r2)
static void _ltr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define lti_f(r0, r1, i0)		_lti_f(_jit, r0, r1, i0)
static void _lti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define ler_f(r0, r1, r2)		_ler_f(_jit, r0, r1, r2)
static void _ler_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define lei_f(r0, r1, i0)		_lei_f(_jit, r0, r1, i0)
static void _lei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define eqr_f(r0, r1, r2)		_eqr_f(_jit, r0, r1, r2)
static void _eqr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define eqi_f(r0, r1, i0)		_eqi_f(_jit, r0, r1, i0)
static void _eqi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define ger_f(r0, r1, r2)		_ger_f(_jit, r0, r1, r2)
static void _ger_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define gei_f(r0, r1, i0)		_gei_f(_jit, r0, r1, i0)
static void _gei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define gtr_f(r0, r1, r2)		_gtr_f(_jit, r0, r1, r2)
static void _gtr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define gti_f(r0, r1, i0)		_gti_f(_jit, r0, r1, i0)
static void _gti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define ner_f(r0, r1, r2)		_ner_f(_jit, r0, r1, r2)
static void _ner_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define nei_f(r0, r1, i0)		_nei_f(_jit, r0, r1, i0)
static void _nei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define unltr_f(r0, r1, r2)		_unltr_f(_jit, r0, r1, r2)
static void _unltr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define unlti_f(r0, r1, i0)		_unlti_f(_jit, r0, r1, i0)
static void _unlti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define unler_f(r0, r1, r2)		_unler_f(_jit, r0, r1, r2)
static void _unler_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define unlei_f(r0, r1, i1)		_unlei_f(_jit, r0, r1, i1)
static void _unlei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define uneqr_f(r0, r1, r2)		_uneqr_f(_jit, r0, r1, r2)
static void _uneqr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define uneqi_f(r0, r1, i0)		_uneqi_f(_jit, r0, r1, i0)
static void _uneqi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define unger_f(r0, r1, r2)		_unger_f(_jit, r0, r1, r2)
static void _unger_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ungei_f(r0, r1, i0)		_ungei_f(_jit, r0, r1, i0)
static void _ungei_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define ungtr_f(r0, r1, r2)		_ungtr_f(_jit, r0, r1, r2)
static void _ungtr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ungti_f(r0, r1, i0)		_ungti_f(_jit, r0, r1, i0)
static void _ungti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define ltgtr_f(r0, r1, r2)		_ltgtr_f(_jit, r0, r1, r2)
static void _ltgtr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ltgti_f(r0, r1, i0)		_ltgti_f(_jit, r0, r1, i0)
static void _ltgti_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define ordr_f(r0, r1, r2)		_ordr_f(_jit, r0, r1, r2)
static void _ordr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ordi_f(r0, r1, i0)		_ordi_f(_jit, r0, r1, i0)
static void _ordi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define unordr_f(r0, r1, r2)		_unordr_f(_jit, r0, r1, r2)
static void _unordr_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define unordi_f(r0, r1, i0)		_unordi_f(_jit, r0, r1, i0)
static void _unordi_f(jit_state_t*, jit_int32_t, jit_int32_t, jit_float32_t);
# define bltr_f(i0, r0, r1)		_bltr_f(_jit, i0, r0, r1)
static jit_word_t _bltr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blti_f(i0, r0, i1)		_blti_f(_jit, i0, r0, i1)
static jit_word_t _blti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t);
# define bler_f(i0, r0, r1)		_bler_f(_jit, i0, r0, r1)
static jit_word_t _bler_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blei_f(i0, r0, i1)		_blei_f(_jit, i0, r0, i1)
static jit_word_t _blei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t);
# define beqr_f(i0, r0, r1)		_beqr_f(_jit, i0, r0, r1)
static jit_word_t _beqr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define beqi_f(i0, r0, i1)		_beqi_f(_jit, i0, r0, i1)
static jit_word_t _beqi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t);
# define bger_f(i0, r0, r1)		_bger_f(_jit, i0, r0, r1)
static jit_word_t _bger_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgei_f(i0, r0, i1)		_bgei_f(_jit, i0, r0, i1)
static jit_word_t _bgei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t);
# define bgtr_f(i0, r0, r1)		_bgtr_f(_jit,i0, r0, r1)
static jit_word_t _bgtr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgti_f(i0, r0, i1)		_bgti_f(_jit, i0, r0, i1)
static jit_word_t _bgti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t);
# define bner_f(i0, r0, r1)		_bner_f(_jit, i0, r0, r1)
static jit_word_t _bner_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bnei_f(i0, r0, i1)		_bnei_f(_jit, i0, r0, i1)
static jit_word_t _bnei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t);
# define bunltr_f(i0, r0, r1)		_bunltr_f(_jit, i0, r0, r1)
static jit_word_t _bunltr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bunlti_f(i0, r0, i1)		_bunlti_f(_jit, i0, r0, i1)
static jit_word_t _bunlti_f(jit_state_t*, jit_word_t,jit_int32_t,jit_float32_t);
# define bunler_f(i0, r0, r1)		_bunler_f(_jit, i0, r0, r1)
static jit_word_t _bunler_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bunlei_f(i0, r0, i1)		_bunlei_f(_jit, i0, r0, i1)
static jit_word_t _bunlei_f(jit_state_t*, jit_word_t,jit_int32_t,jit_float32_t);
# define buneqr_f(i0, r0, r1)		_buneqr_f(_jit, i0, r0, r1)
static jit_word_t _buneqr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define buneqi_f(i0, r0, i1)		_buneqi_f(_jit, i0, r0, i1)
static jit_word_t _buneqi_f(jit_state_t*, jit_word_t,jit_int32_t,jit_float32_t);
# define bunger_f(i0, r0, r1)		_bunger_f(_jit, i0, r0, r1)
static jit_word_t _bunger_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bungei_f(i0, r0, i1)		_bungei_f(_jit, i0, r0, i1)
static jit_word_t _bungei_f(jit_state_t*, jit_word_t,jit_int32_t,jit_float32_t);
# define bungtr_f(i0, r0, r1)		_bungtr_f(_jit, i0, r0, r1)
static jit_word_t _bungtr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bungti_f(i0, r0, i1)		_bungti_f(_jit, i0, r0, i1)
static jit_word_t _bungti_f(jit_state_t*, jit_word_t,jit_int32_t,jit_float32_t);
# define bltgtr_f(i0, r0, r1)		_bltgtr_f(_jit, i0, r0, r1)
static jit_word_t _bltgtr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bltgti_f(i0, r0, i1)		_bltgti_f(_jit, i0, r0, i1)
static jit_word_t _bltgti_f(jit_state_t*, jit_word_t,jit_int32_t,jit_float32_t);
# define bordr_f(i0, r0, r1)		_bordr_f(_jit, i0, r0, r1)
static jit_word_t _bordr_f(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bordi_f(i0, r0, i1)		_bordi_f(_jit, i0, r0, i1)
static jit_word_t _bordi_f(jit_state_t*, jit_word_t, jit_int32_t,jit_float32_t);
# define bunordr_f(i0, r0, r1)		_bunordr_f(_jit, i0, r0, r1)
static jit_word_t _bunordr_f(jit_state_t*, jit_word_t, jit_int32_t,jit_int32_t);
# define bunordi_f(i0, r0, i1)		_bunordi_f(_jit, i0,r0, i1)
static jit_word_t _bunordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
# define addr_d(r0, r1, r2)		FADD_D(r0, r1, r2)
# define addi_d(r0, r1, i0)		_addi_d(_jit, r0, r1, i0)
static void _addi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define subr_d(r0, r1, r2)		FSUB_D(r0, r1, r2)
# define subi_d(r0, r1, i0)		_subi_d(_jit, r0, r1, i0)
static void _subi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define rsbr_d(r0, r1, r2)		FSUB_D(r0, r2, r1)
# define rsbi_d(r0, r1, i0)		_rsbi_d(_jit, r0, r1, i0)
static void _rsbi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define mulr_d(r0, r1, r2)		FMUL_D(r0, r1, r2)
# define muli_d(r0, r1, i0)		_muli_d(_jit, r0, r1, i0)
static void _muli_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define divr_d(r0, r1, r2)		FDIV_D(r0, r1, r2)
# define divi_d(r0, r1, i0)		_divi_d(_jit, r0, r1, i0)
static void _divi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define absr_d(r0, r1)			FABS_D(r0, r1)
# define negr_d(r0, r1)			FNEG_D(r0, r1)
# define sqrtr_d(r0, r1)		FSQRT_D(r0, r1)
# define fmar_d(r0, r1, r2, r3)		FMADD_D(r0, r1, r2, r3)
# define fmsr_d(r0, r1, r2, r3)		FMSUB_D(r0, r1, r2, r3)
# define fnmar_d(r0, r1, r2, r3)	FNMADD_D(r0, r1, r2, r3)
# define fnmsr_d(r0, r1, r2, r3)	FNMSUB_D(r0, r1, r2, r3)
# define extr_d(r0, r1)			_extr_d(_jit, r0, r1)
static void _extr_d(jit_state_t*, jit_int32_t, jit_int32_t);
# define ldr_d(r0, r1)			FLD_D(r0, r1, 0)
# define ldi_d(r0, i0)			_ldi_d(_jit, r0, i0)
static void _ldi_d(jit_state_t*, jit_int32_t, jit_word_t);
# define ldxr_d(r0, r1, r2)		FLDX_D(r0, r1, r2)
# define ldxi_d(r0, r1, i0)		_ldxi_d(_jit, r0, r1, i0)
static void _ldxi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define str_d(r0, r1)			FST_D(r1, r0, 0)
# define sti_d(i0, r0)			_sti_d(_jit, i0, r0)
static void _sti_d(jit_state_t*, jit_word_t, jit_int32_t);
# define stxr_d(r0, r1, r2)		FSTX_D(r2, r1, r0)
# define stxi_d(i0, r0, r1)		_stxi_d(_jit, i0, r0, r1)
static void _stxi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define movr_d(r0, r1)			FMOV_D(r0, r1)
# define movi_d(r0, i0)			_movi_d(_jit, r0, i0)
static void _movi_d(jit_state_t*, jit_int32_t, jit_float64_t);
#  define movr_d_w(r0, r1)		MOVFR2GR_D(r0, r1)
#  define movr_w_d(r0, r1)		MOVGR2FR_D(r0, r1)
#define movi_w_d(r0, i0)		_movi_w_d(_jit, r0, i0)
static void _movi_w_d(jit_state_t*, jit_int32_t, jit_word_t);
# define extr_f_d(r0, r1)		FCVT_D_S(r0, r1)
# define ltr_d(r0, r1, r2)		_ltr_d(_jit, r0, r1, r2)
static void _ltr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define lti_d(r0, r1, i0)		_lti_d(_jit, r0, r1, i0)
static void _lti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define ler_d(r0, r1, r2)		_ler_d(_jit, r0, r1, r2)
static void _ler_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define lei_d(r0, r1, i0)		_lei_d(_jit, r0, r1, i0)
static void _lei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define eqr_d(r0, r1, r2)		_eqr_d(_jit, r0, r1, r2)
static void _eqr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define eqi_d(r0, r1, i0)		_eqi_d(_jit, r0, r1, i0)
static void _eqi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define ger_d(r0, r1, r2)		_ger_d(_jit, r0, r1, r2)
static void _ger_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define gei_d(r0, r1, i0)		_gei_d(_jit, r0, r1, i0)
static void _gei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define gtr_d(r0, r1, r2)		_gtr_d(_jit, r0, r1, r2)
static void _gtr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define gti_d(r0, r1, i0)		_gti_d(_jit, r0, r1, i0)
static void _gti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define ner_d(r0, r1, r2)		_ner_d(_jit, r0, r1, r2)
static void _ner_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define nei_d(r0, r1, i0)		_nei_d(_jit, r0, r1, i0)
static void _nei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define unltr_d(r0, r1, r2)		_unltr_d(_jit, r0, r1, r2)
static void _unltr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define unlti_d(r0, r1, i0)		_unlti_d(_jit, r0, r1, i0)
static void _unlti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define unler_d(r0, r1, r2)		_unler_d(_jit, r0, r1, r2)
static void _unler_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define unlei_d(r0, r1, i0)		_unlei_d(_jit, r0, r1, i0)
static void _unlei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define uneqr_d(r0, r1, r2)		_uneqr_d(_jit, r0, r1, r2)
static void _uneqr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define uneqi_d(r0, r1, i0)		_uneqi_d(_jit, r0, r1, i0)
static void _uneqi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define unger_d(r0, r1, r2)		_unger_d(_jit, r0, r1, r2)
static void _unger_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ungei_d(r0, r1, i0)		_ungei_d(_jit, r0, r1, i0)
static void _ungei_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define ungtr_d(r0, r1, r2)		_ungtr_d(_jit, r0, r1, r2)
static void _ungtr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ungti_d(r0, r1, i0)		_ungti_d(_jit, r0, r1, i0)
static void _ungti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define ltgtr_d(r0, r1, r2)		_ltgtr_d(_jit, r0, r1, r2)
static void _ltgtr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ltgti_d(r0, r1, i0)		_ltgti_d(_jit, r0, r1, i0)
static void _ltgti_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define ordr_d(r0, r1, r2)		_ordr_d(_jit, r0, r1, r2)
static void _ordr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define ordi_d(r0, r1, i0)		_ordi_d(_jit, r0, r1, i0)
static void _ordi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define unordr_d(r0, r1, r2)		_unordr_d(_jit, r0, r1, r2)
static void _unordr_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define unordi_d(r0, r1, i0)		_unordi_d(_jit, r0, r1, i0)
static void _unordi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_float64_t);
# define bltr_d(i0, r0, r1)		_bltr_d(_jit, i0, r0, r1)
static jit_word_t _bltr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blti_d(i0, r0, i1)		_blti_d(_jit, i0, r0, i1)
static jit_word_t _blti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t);
# define bler_d(i0, r0, r1)		_bler_d(_jit, i0, r0, r1)
static jit_word_t _bler_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blei_d(i0, r0, i1)		_blei_d(_jit, i0,r0, i1)
static jit_word_t _blei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t);
# define beqr_d(i0, r0, r1)		_beqr_d(_jit, i0, r0, r1)
static jit_word_t _beqr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define beqi_d(i0, r0, i1)		_beqi_d(_jit, i0, r0, i1)
static jit_word_t _beqi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t);
# define bger_d(i0, r0, r1)		_bger_d(_jit, i0, r0, r1)
static jit_word_t _bger_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgei_d(i0, r0, i1)		_bgei_d(_jit, i0, r0, i1)
static jit_word_t _bgei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t);
# define bgtr_d(i0, r0, r1)		_bgtr_d(_jit, i0, r0, r1)
static jit_word_t _bgtr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgti_d(i0, r0, i1)		_bgti_d(_jit, i0, r0, i1)
static jit_word_t _bgti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t);
# define bner_d(i0, r0, r1)		_bner_d(_jit, i0, r0, r1)
static jit_word_t _bner_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bnei_d(i0, r0, i1)		_bnei_d(_jit, i0, r0, i1)
static jit_word_t _bnei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t);
# define bunltr_d(i0, r0, r1)		_bunltr_d(_jit, i0, r0, r1)
static jit_word_t _bunltr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bunlti_d(i0, r0, i1)		_bunlti_d(_jit, i0, r0, i1)
static jit_word_t _bunlti_d(jit_state_t*, jit_word_t,jit_int32_t,jit_float64_t);
# define bunler_d(i0, r0, r1)		_bunler_d(_jit, i0, r0, r1)
static jit_word_t _bunler_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bunlei_d(i0, r0, i1)		_bunlei_d(_jit, i0, r0, i1)
static jit_word_t _bunlei_d(jit_state_t*, jit_word_t,jit_int32_t,jit_float64_t);
# define buneqr_d(i0, r0, r1)		_buneqr_d(_jit, i0, r0, r1)
static jit_word_t _buneqr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define buneqi_d(i0, r0, i1)		_buneqi_d(_jit, i0,r0, i1)
static jit_word_t _buneqi_d(jit_state_t*, jit_word_t,jit_int32_t,jit_float64_t);
# define bunger_d(i0, r0, r1)		_bunger_d(_jit, i0, r0, r1)
static jit_word_t _bunger_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bungei_d(i0, r0, i1)		_bungei_d(_jit, i0, r0, i1)
static jit_word_t _bungei_d(jit_state_t*, jit_word_t,jit_int32_t,jit_float64_t);
# define bungtr_d(i0, r0, r1)		_bungtr_d(_jit, i0, r0, r1)
static jit_word_t _bungtr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bungti_d(i0, r0, i1)		_bungti_d(_jit, i0, r0, i1)
static jit_word_t _bungti_d(jit_state_t*, jit_word_t,jit_int32_t,jit_float64_t);
# define bltgtr_d(i0, r0, r1)		_bltgtr_d(_jit, i0, r0, r1)
static jit_word_t _bltgtr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bltgti_d(i0, r0, i1)		_bltgti_d(_jit, i0, r0, i1)
static jit_word_t _bltgti_d(jit_state_t*, jit_word_t,jit_int32_t,jit_float64_t);
# define bordr_d(i0, r0, r1)		_bordr_d(_jit, i0, r0, r1)
static jit_word_t _bordr_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bordi_d(i0, r0, i1)		_bordi_d(_jit, i0, r0, i1)
static jit_word_t _bordi_d(jit_state_t*, jit_word_t, jit_int32_t,jit_float64_t);
# define bunordr_d(i0, r0, r1)		_bunordr_d(_jit, i0, r0, r1)
static jit_word_t _bunordr_d(jit_state_t*, jit_word_t, jit_int32_t,jit_int32_t);
# define bunordi_d(i0, r0, i1)		_bunordi_d(_jit, i0, r0, i1)
static jit_word_t _bunordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
# define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif /* PROTO */

#if CODE
static void
_truncr_f_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);
    FTINTRZ_W_S(rn(reg), r1);
    MOVFR2GR_S(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);
    FTINTRZ_W_D(rn(reg), r1);
    MOVFR2GR_S(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_truncr_f_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);
    FTINTRZ_L_S(rn(reg), r1);
    MOVFR2GR_D(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_truncr_d_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);
    FTINTRZ_L_D(rn(reg), r1);
    MOVFR2GR_D(r0, rn(reg));
    jit_unget_reg(reg);
}

#  define fpr_opi(name, type, size)					\
static void								\
_##name##i_##type(jit_state_t *_jit,					\
		  jit_int32_t r0, jit_int32_t r1,			\
		  jit_float##size##_t i0)				\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    movi_##type(rn(reg), i0);						\
    name##r_##type(r0, r1, rn(reg));					\
    jit_unget_reg(reg);							\
}
#  define fopi(name)			fpr_opi(name, f, 32)
#  define dopi(name)			fpr_opi(name, d, 64)

#  define fpr_bopi(name, type, size)					\
static jit_word_t							\
_b##name##i_##type(jit_state_t *_jit,					\
		  jit_word_t i0, jit_int32_t r0,			\
		  jit_float##size##_t i1)				\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    movi_##type(rn(reg), i1);						\
    word = b##name##r_##type(i0, r0, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define fbopi(name)			fpr_bopi(name, f, 32)
#  define dbopi(name)			fpr_bopi(name, d, 64)

fopi(add)
fopi(sub)
fopi(rsb)
fopi(mul)
fopi(div)

static void
_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    MOVGR2FR_D(r0, r1);
    FFINT_S_L(r0, r0);
}

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FLD_S(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ldr_f(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FLD_S(r0, r1, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ldxr_f(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_unldr_x(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (jit_unaligned_p())
	fallback_unldr_x(r0, r1, i0);
    else
	generic_unldr_x(r0, r1, i0);
}

static void
_unldi_x(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    if (jit_unaligned_p())
	fallback_unldi_x(r0, i0, i1);
    else
	generic_unldi_x(r0, i0, i1);
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FST_S(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	str_f(rn(t0), r0);
	jit_unget_reg(t0);
    }
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FST_S(r1, r0, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	stxr_f(rn(t0), r0, r1);
	jit_unget_reg(t0);
    }
}

static void
_unstr_x(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (jit_unaligned_p())
	fallback_unstr_x(r0, r1, i0);
    else
	generic_unstr_x(r0, r1, i0);
}

static void
_unsti_x(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    if (jit_unaligned_p())
	fallback_unsti_x(i0, r0, i1);
    else
	fallback_unsti_x(i0, r0, i1);
}

static void
_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t i0)
{
    union {
	jit_int32_t	 i;
	jit_float32_t	 f;
    } data;
    jit_int32_t		 reg;
    data.f = i0;
    if (data.i == 0)
	movr_w_f(r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.i);
	movr_w_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_movi_w_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    movr_w_f(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ltr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLT_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(lt)

static void
_ler_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLE_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(le)

static void
_eqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CEQ_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(eq)

static void
_ger_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLE_S(0, r2, r1);
    MOVCF2GR(r0, 0);
}
fopi(ge)

static void
_gtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLT_S(0, r2, r1);
    MOVCF2GR(r0, 0);
}
fopi(gt)

static void
_ner_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CUNE_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(ne)

static void
_unltr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULT_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(unlt)

static void
_unler_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULE_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(unle)

static void
_uneqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CUEQ_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(uneq)

static void
_unger_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULE_S(0, r2, r1);
    MOVCF2GR(r0, 0);
}
fopi(unge)

static void
_ungtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULT_S(0, r2, r1);
    MOVCF2GR(r0, 0);
}
fopi(ungt)

static void
_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CNE_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(ltgt)

static void
_ordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_COR_S(0, r1, r2);
    MOVCF2GR(r0, 0);
}
fopi(ord)

static void
_unordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_COR_S(0, r1, r2);
    MOVCF2GR(r0, 0);
    XORI(r0, r0, 1);
}
fopi(unord)

static jit_word_t
_bltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLT_S(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(lt)

static jit_word_t
_bler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLE_S(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(le)

static jit_word_t
_beqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CEQ_S(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(eq)

static jit_word_t
_bger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLE_S(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(ge)

static jit_word_t
_bgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLT_S(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(gt)

static jit_word_t
_bner_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CUNE_S(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(ne)

static jit_word_t
_bunltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULT_S(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(unlt)

static jit_word_t
_bunler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULE_S(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(unle)

static jit_word_t
_buneqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CUEQ_S(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(uneq)

static jit_word_t
_bunger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULE_S(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(unge)

static jit_word_t
_bungtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULT_S(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(ungt)

static jit_word_t
_bltgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CNE_S(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(ltgt)

static jit_word_t
_bordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_COR_S(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(ord)

static jit_word_t
_bunordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_COR_S(0, r2, r1);
    w = _jit->pc.w;
    BCEQZ(0, (i0 - w) >> 2);
    return (w);
}
fbopi(unord)

dopi(add)
dopi(sub)
dopi(rsb)
dopi(mul)
dopi(div)

static void
_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    MOVGR2FR_D(r0, r1);
    FFINT_D_L(r0, r0);
}

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FLD_D(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ldr_d(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FLD_D(r0, r1, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ldxr_d(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FST_D(r0, _ZERO_REGNO, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	str_d(rn(t0), r0);
	jit_unget_reg(t0);
    }
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    if (can_sign_extend_si12_p(i0))
	FST_D(r1, r0, i0);
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	stxr_d(rn(t0), r0, r1);
	jit_unget_reg(t0);
    }
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t i0)
{
    union {
	jit_word_t	 w;
	jit_float64_t	 d;
    } data;
    jit_int32_t		 reg;
    data.d = i0;
    if (data.w == 0)
	movr_w_d(r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), data.w);
	movr_w_d(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_movi_w_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    movr_w_d(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLT_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(lt)

static void
_ler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLE_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(le)

static void
_eqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CEQ_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(eq)

static void
_ger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLE_D(0, r2, r1);
    MOVCF2GR(r0, 0);
}
dopi(ge)

static void
_gtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_SLT_D(0, r2, r1);
    MOVCF2GR(r0, 0);
}
dopi(gt)

static void
_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CUNE_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(ne)

static void
_unltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULT_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(unlt)

static void
_unler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULE_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(unle)

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CUEQ_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(uneq)

static void
_unger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULE_D(0, r2, r1);
    MOVCF2GR(r0, 0);
}
dopi(unge)

static void
_ungtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CULT_D(0, r2, r1);
    MOVCF2GR(r0, 0);
}
dopi(ungt)

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_CNE_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(ltgt)

static void
_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_COR_D(0, r1, r2);
    MOVCF2GR(r0, 0);
}
dopi(ord)

static void
_unordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    FCMP_COR_D(0, r1, r2);
    MOVCF2GR(r0, 0);
    XORI(r0, r0, 1);
}
dopi(unord)

static jit_word_t
_bltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLT_D(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(lt)

static jit_word_t
_bler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLE_D(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(le)

static jit_word_t
_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CEQ_D(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(eq)

static jit_word_t
_bger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLE_D(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(ge)

static jit_word_t
_bgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_SLT_D(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(gt)

static jit_word_t
_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CUNE_D(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(ne)

static jit_word_t
_bunltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULT_D(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(unlt)

static jit_word_t
_bunler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULE_D(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(unle)

static jit_word_t
_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CUEQ_D(0, r1, r2);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(uneq)

static jit_word_t
_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULE_D(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(unge)

static jit_word_t
_bungtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CULT_D(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(ungt)

static jit_word_t
_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_CNE_D(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(ltgt)

static jit_word_t
_bordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_COR_D(0, r2, r1);
    w = _jit->pc.w;
    BCNEZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(ord)

static jit_word_t
_bunordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    FCMP_COR_D(0, r2, r1);
    w = _jit->pc.w;
    BCEQZ(0, (i0 - w) >> 2);
    return (w);
}
dbopi(unord)

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Load argument. */
    ldr_d(r0, r1);
    /* Update va_list. */
    addi(r1, r1, sizeof(jit_float64_t));
}

#endif /* CODE */
