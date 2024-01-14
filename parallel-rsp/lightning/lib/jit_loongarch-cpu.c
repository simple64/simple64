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
# define ii(i)				*_jit->pc.ui++ = (i)
# define can_sign_extend_si12_p(s12)	((s12) <= 2047 && (s12) >= -2048)
# define can_zero_extend_u12_p(u12)	((u12) <= 4095 && (u12) >= 0)
# define can_sign_extend_si16_p(s16)	((s16) <= 32767 && (s16) >= -32768)
# define can_sign_extend_si21_p(s21)	((s21) <= 1048575 && (s21) >= -1048576)
# define can_sign_extend_si26_p(s26)					\
	((s26) <= 33554431 && (s26) >= -33554432)
# define can_sign_extend_si32_p(s32)					\
	((s32) <= 2147483647LL && (s32) >= -2147483648LL)
# define _ZERO_REGNO			0
# define _RA_REGNO			1
# define _SP_REGNO			3
# define _FP_REGNO			22
# define ldr(u, v)			ldr_l(u, v)
# define ldi(u, v)			ldi_l(u, v)
# define ldxi(u, v, w)			ldxi_l(u, v, w)
# define sti(u, v)			sti_l(u, v)
# define stxi(u, v, w)			stxi_l(u, v, w)
# define orrr(op, rk, rj, rd)		_orrr(_jit, op, rk, rj, rd)
# define ou5rr(op, i5, rj, rd)		_orrr(_jit, op, i5, rj, rd)
# define orru5(op, rk, rj, i5)		_orrr(_jit, op, rk, rj, i5)
static void _orrr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou2rrr(op, i2, rk, rj, rd)	_ou2rrr(_jit, op, i2, rk, rj, rd)
static void _ou2rrr(jit_state_t*,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou3rrr(op, u3, rk, rj, rd)	_ou3rrr(_jit, op, u3, rk, rj, rd)
static void _ou3rrr(jit_state_t*,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou6rr(op, u6, rj, rd)		_ou6rr(_jit, op, u6, rj, rd)
static void _ou6rr(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
# define ou5u1u5rr(op,m5,u1,l5,rj,rd)	_ou5u1u5rr(_jit,op,m5,u1,l5,rj,rd)
static void _ou5u1u5rr(jit_state_t*,jit_int32_t,jit_int32_t,
		       jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou6u6rr(op, m6, l6, rj, rd)	_ou6u6rr(_jit, op, m6, l6, rj, rd)
static void _ou6u6rr(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define o5r23(op, i5, rj, i2, rd)	_o5r23(_jit, op, i5, rj, i2, rd)
static void _o5r23(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define o523r(op, i5, i2, rj, i3)	_o523r(_jit, op, i5, i2, rj, i3)
static void _o523r(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define os12rr(op, i12, rj, rd)	_os12rr(_jit, op, i12, rj, rd)
# define os12ru5(op, i12, rj, u5)	_os12rr(_jit, op, i12, rj, u5)
static void _os12rr(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou12rr(op, u12, rj, rd)	_ou12rr(_jit, op, u12, rj, rd)
static void _ou12rr(jit_state_t*,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou14u5r(op, u14, u5, rd)	_ou14u5r(_jit, op, u14, u5, rd)
static void _osu14u5r(jit_state_t*,
		      jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define os14rr(op, s14, rj, rd)	_os14rr(_jit, op, s14, rj, rd)
static void _os14rr(jit_state_t*,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou8rr(op, u8, rd, rj)		_ou8rr(_jit, op, u8, rd, rj)
# define ou8u5r(op, u8, u5, rj)		_ou8rr(_jit, op, u8, u5, rj)
static void _ou8rr(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
# define ou15(op, u15)			_ou15(_jit, op, u15)
static void _ou15(jit_state_t*, jit_int32_t,jit_int32_t);
# define orrrr(op, ra, rk, rj, rd)	_orrrr(_jit, op, ra, rk, rj, rd)
static void _orrrr(jit_state_t*,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou5rru2u3(op,u5,rj,rk,u2,u3)	_ou5rru2u3(_jit, op, u5, rj, rk, u2, u3)
static void _ou5rru2u3(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,
		       jit_int32_t,jit_int32_t,jit_int32_t);
# define os16rr(op, s16, rj, rd)	_os16rr(_jit, op, s16, rj, rd)
static void _os16rr(jit_state_t*,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define os20r(op, s20, rd)		_os20r(_jit, op, s20, rd)
static void _os20r(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
# define orj21(op, rj, j21)		_orj21(_jit, op, rj, j21)
static void _orj21(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
# define ou2u3j21(op, u2, u3, j21)	_ou2u3j21(_jit, op, u2, u3, j21)
static void _o2cj21(jit_state_t*,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define oj16rr(op, j16, rj, rd)	_oj16rr(_jit, op, j16, rj, rd)
static void _oj16rr(jit_state_t*,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
# define oj26(op, j26)			_oj26(_jit, op, j26)
static void _oj26(jit_state_t*, jit_int32_t,jit_int32_t);
# define       CLO_W(rd, rj)		 ou5rr(0x000,	     0x04,    rj,    rd)
# define       CLZ_W(rd, rj)		 ou5rr(0x000,	     0x05,    rj,    rd)
# define       CTO_W(rd, rj)		 ou5rr(0x000,	     0x06,    rj,    rd)
# define       CTZ_W(rd, rj)		 ou5rr(0x000,	     0x07,    rj,    rd)
# define       CLO_D(rd, rj)		 ou5rr(0x000,	     0x08,    rj,    rd)
# define       CLZ_D(rd, rj)		 ou5rr(0x000,	     0x09,    rj,    rd)
# define       CTO_D(rd, rj)		 ou5rr(0x000,	     0x0a,    rj,    rd)
# define       CTZ_D(rd, rj)		 ou5rr(0x000,	     0x0b,    rj,    rd)
# define     REVB_2H(rd, rj)		 ou5rr(0x000,	     0x0c,    rj,    rd)
# define     REVB_4H(rd, rj)		 ou5rr(0x000,	     0x0d,    rj,    rd)
# define     REVB_2W(rd, rj)		 ou5rr(0x000,	     0x0e,    rj,    rd)
# define      REVB_D(rd, rj)		 ou5rr(0x000,	     0x0f,    rj,    rd)
# define     REVH_2W(rd, rj)		 ou5rr(0x000,	     0x10,    rj,    rd)
# define      REVH_D(rd, rj)		 ou5rr(0x000,	     0x11,    rj,    rd)
# define   BITREV_4B(rd, rj)		 ou5rr(0x000,	     0x12,    rj,    rd)
# define   BITREV_8B(rd, rj)		 ou5rr(0x000,	     0x13,    rj,    rd)
# define    BITREV_W(rd, rj)		 ou5rr(0x000,	     0x14,    rj,    rd)
# define    BITREV_D(rd, rj)		 ou5rr(0x000,	     0x15,    rj,    rd)
# define     EXT_W_H(rd, rj)		 ou5rr(0x000,	     0x16,    rj,    rd)
# define     EXT_W_B(rd, rj)		 ou5rr(0x000,	     0x17,    rj,    rd)
# define   RDTIMEL_W(rd, rj)		 ou5rr(0x000,	     0x18,    rj,    rd)
# define   RDTIMEH_W(rd, rj)		 ou5rr(0x000,	     0x19,    rj,    rd)
# define    RDTIME_D(rd, rj)		 ou5rr(0x000,	     0x20,    rj,    rd)
# define      CPUCFG(rd, rj)		 ou5rr(0x000,	     0x21,    rj,    rd)
# define    ASRTLE_D(	 rj, rk)	 orru5(0x002,	       rk,    rj,  0x00)
# define    ASRTGT_D(	 rj, rk)	 orru5(0x003,	       rk,    rj,  0x00)
# define      ALSL_W(rd, rj, rk, sa2)	ou2rrr(0x002,	  sa2, rk,    rj,    rd)
# define     ALSL_WU(rd, rj, rk, sa2)	ou2rrr(0x003,	  sa2, rk,    rj,    rd)
# define  BYTEPICK_W(rd, rj, rk, sa2)	ou2rrr(0x004,	  sa2, rk,    rj,    rd)
# define  BYTEPICK_D(rd, rj, rk, sa3)	ou3rrr(0x003,	  sa3, rk,    rj,    rd)
# define       ADD_W(rd, rj, rk)	  orrr(0x020,	       rk,    rj,    rd)
# define       ADD_D(rd, rj, rk)	  orrr(0x021,	       rk,    rj,    rd)
# define       SUB_W(rd, rj, rk)	  orrr(0x022,	       rk,    rj,    rd)
# define       SUB_D(rd, rj, rk)	  orrr(0x023,	       rk,    rj,    rd)
# define	 SLT(rd, rj, rk)	  orrr(0x024,	       rk,    rj,    rd)
# define	SLTU(rd, rj, rk)	  orrr(0x025,	       rk,    rj,    rd)
# define     MASKEQZ(rd, rj, rk)	  orrr(0x026,	       rk,    rj,    rd)
# define     MASKNEZ(rd, rj, rk)	  orrr(0x027,	       rk,    rj,    rd)
# define	 NOR(rd, rj, rk)	  orrr(0x028,	       rk,    rj,    rd)
# define	 AND(rd, rj, rk)	  orrr(0x029,	       rk,    rj,    rd)
# define	  OR(rd, rj, rk)	  orrr(0x02a,	       rk,    rj,    rd)
# define	 XOR(rd, rj, rk)	  orrr(0x02b,	       rk,    rj,    rd)
# define	 ORN(rd, rj, rk)	  orrr(0x02c,	       rk,    rj,    rd)
# define	ANDN(rd, rj, rk)	  orrr(0x02d,	       rk,    rj,    rd)
# define       SLL_W(rd, rj, rk)	  orrr(0x02e,	       rk,    rj,    rd)
# define       SRL_W(rd, rj, rk)	  orrr(0x02f,	       rk,    rj,    rd)
# define       SRA_W(rd, rj, rk)	  orrr(0x030,	       rk,    rj,    rd)
# define       SLL_D(rd, rj, rk)	  orrr(0x031,	       rk,    rj,    rd)
# define       SRL_D(rd, rj, rk)	  orrr(0x032,	       rk,    rj,    rd)
# define       SRA_D(rd, rj, rk)	  orrr(0x033,	       rk,    rj,    rd)
# define      ROTR_W(rd, rj, rk)	  orrr(0x036,	       rk,    rj,    rd)
# define      ROTR_D(rd, rj, rk)	  orrr(0x037,	       rk,    rj,    rd)
# define       MUL_W(rd, rj, rk)	  orrr(0x038,	       rk,    rj,    rd)
# define      MULH_W(rd, rj, rk)	  orrr(0x039,	       rk,    rj,    rd)
# define     MULH_WU(rd, rj, rk)	  orrr(0x03a,	       rk,    rj,    rd)
# define       MUL_D(rd, rj, rk)	  orrr(0x03b,	       rk,    rj,    rd)
# define      MULH_D(rd, rj, rk)	  orrr(0x03c,	       rk,    rj,    rd)
# define     MULH_DU(rd, rj, rk)	  orrr(0x03d,	       rk,    rj,    rd)
# define    MULW_D_W(rd, rj, rk)	  orrr(0x03e,	       rk,    rj,    rd)
# define   MULW_D_WU(rd, rj, rk)	  orrr(0x03f,	       rk,    rj,    rd)
# define       DIV_W(rd, rj, rk)	  orrr(0x040,	       rk,    rj,    rd)
# define       MOD_W(rd, rj, rk)	  orrr(0x041,	       rk,    rj,    rd)
# define      DIV_WU(rd, rj, rk)	  orrr(0x042,	       rk,    rj,    rd)
# define      MOD_WU(rd, rj, rk)	  orrr(0x043,	       rk,    rj,    rd)
# define       DIV_D(rd, rj, rk)	  orrr(0x044,	       rk,    rj,    rd)
# define       MOD_D(rd, rj, rk)	  orrr(0x045,	       rk,    rj,    rd)
# define      DIV_DU(rd, rj, rk)	  orrr(0x046,	       rk,    rj,    rd)
# define      MOD_DU(rd, rj, rk)	  orrr(0x047,	       rk,    rj,    rd)
# define   CRC_W_B_W(rd, rj, rk)	  orrr(0x048,	       rk,    rj,    rd)
# define   CRC_W_H_W(rd, rj, rk)	  orrr(0x049,	       rk,    rj,    rd)
# define   CRC_W_W_W(rd, rj, rk)	  orrr(0x04a,	       rk,    rj,    rd)
# define   CRC_W_D_W(rd, rj, rk)	  orrr(0x04b,	       rk,    rj,    rd)
# define  CRCC_W_B_W(rd, rj, rk)	  orrr(0x04c,	       rk,    rj,    rd)
# define  CRCC_W_H_W(rd, rj, rk)	  orrr(0x04d,	       rk,    rj,    rd)
# define  CCRC_W_W_W(rd, rj, rk)	  orrr(0x04e,	       rk,    rj,    rd)
# define  CCRC_W_D_W(rd, rj, rk)	  orrr(0x04f,	       rk,    rj,    rd)
# define       BREAK(code)		  ou15(0x054,			   code)
# define	DBCL(code)		  ou15(0x055,			   code)
# define     SYSCALL(code)		  ou15(0x056,			   code)
# define      ALSL_D(rd, rj, rk, sa2)	ou2rrr(0x016,	  sa2, rk,    rj,    rd)
# define      SLLI_W(rd, rj, ui5)	 ou5rr(0x081,	      ui5,    rj,    rd)
# define      SLLI_D(rd, rj, ui6)	 ou6rr(0x041,	      ui6,    rj,    rd)
# define      SRLI_W(rd, rj, ui5)	 ou5rr(0x089,	      ui5,    rj,    rd)
# define      SRLI_D(rd, rj, ui6)	 ou6rr(0x045,	      ui6,    rj,    rd)
# define      SRAI_W(rd, rj, ui5)	 ou5rr(0x091,	      ui5,    rj,    rd)
# define      SRAI_D(rd, rj, ui6)	 ou6rr(0x049,	      ui6,    rj,    rd)
# define     ROTRI_W(rd, rj, ui5)	 ou5rr(0x099,	      ui5,    rj,    rd)
# define     ROTRI_D(rd, rj, ui6)	 ou6rr(0x04d,	      ui6,    rj,    rd)
# define   BSTRINS_W(rd, rj, m5, l5) ou5u1u5rr(0x003, m5, 0x0, l5,    rj,    rd)
# define  BSTRPICK_W(rd, rj, m5, l5) ou5u1u5rr(0x003, m5, 0x1, l5,    rj,    rd)
# define   BSTRINS_D(rd, rj, m6, l6)   ou6u6rr(0x002, m6,      l6,    rj,    rd)
# define  BSTRPICK_D(rd, rj, m6, l6)   ou6u6rr(0x003, m6,      l6,    rj,    rd)
# define	SLTI(rd, rj,  i12)	os12rr(0x008,	      i12,    rj,    rd)
# define       SLTUI(rd, rj,  i12)	os12rr(0x009,	      i12,    rj,    rd)
# define      ADDI_W(rd, rj, si12)	os12rr(0x00a,	     si12,    rj,    rd)
# define      ADDI_D(rd, rj, si12)	os12rr(0x00b,	     si12,    rj,    rd)
# define     LU52I_D(rd, rj,  i12)	os12rr(0x00c,	      i12,    rj,    rd)
# define	ANDI(rd, rj,  i12)	ou12rr(0x00d,	      i12,    rj,    rd)
# define	 ORI(rd, rj,  i12)	ou12rr(0x00e,	      i12,    rj,    rd)
# define	XORI(rd, rj,  i12)	ou12rr(0x00f,	      i12,    rj,    rd)
# define       CSRRD(rd, csr)	       ou14u5r(0x004,	      csr,  0x00,    rd)
# define       CSRWR(rd, csr)	       ou14u5r(0x004,	      csr,  0x01,    rd)
# define     CSRXCHG(rd, rj, csr)      ou14u5r(0x004,	      csr,    rj,    rd)
# define       CACOP(i5, rj, si12)     os12ru5(0x018,	     si12,    rj,    i5)
# define       LDDIR(rd, rj, level)	 ou8rr(0x190,	    level,    rj,    rd)
# define       LDPTE(	 rj, level)	ou8u5r(0x191,	    level,    rj,  0x00)
# define   IOCSRRD_B(rd, rj)		 ou5rr(0xc90,	     0x00,    rj,    rd)
# define   IOCSRRD_H(rd, rj)		 ou5rr(0xc90,	     0x01,    rj,    rd)
# define   IOCSRRD_W(rd, rj)		 ou5rr(0xc90,	     0x02,    rj,    rd)
# define   IOCSRRD_D(rd, rj)		 ou5rr(0xc90,	     0x03,    rj,    rd)
# define   IOCSRWR_B(rd, rj)		 ou5rr(0xc90,	     0x04,    rj,    rd)
# define   IOCSRWR_H(rd, rj)		 ou5rr(0xc90,	     0x05,    rj,    rd)
# define   IOCSRWR_W(rd, rj)		 ou5rr(0xc90,	     0x06,    rj,    rd)
# define   IOCSRWR_D(rd, rj)		 ou5rr(0xc90,	     0x07,    rj,    rd)
# define      TLBCLR()			    ii(			      0x6482000)
# define    TLBFLUSH()			    ii(			      0x6482400)
# define     TLBSRCH()			    ii(			      0x6482800)
# define       TLBRD()			    ii(			      0x6482c00)
# define       TLBWR()			    ii(			      0x6483000)
# define     TLBFILL()			    ii(			      0x6483400)
# define	ERTN()			    ii(			      0x6483800)
# define	IDLE(level)		  ou15(0xc91,			  level)
# define      INVTLB(op, rj, rk)	 orru5(0xc93,	       rk,   rj,     op)
# define   ADDU16I_D(rd, rj, si16)	os16rr(0x004,	     si16,   rj,     rd)
# define     LU12I_W(rd, si20)		 os20r(0x00a,		   si20,     rd)
# define     LU32I_D(rd, si20)		 os20r(0x00b,		   si20,     rd)
# define      PCADDI(rd, si20)		 os20r(0x00c,		   si20,     rd)
# define   PCALAU12I(rd, si20)		 os20r(0x00d,		   si20,     rd)
# define   PCADDU12I(rd, si20)		 os20r(0x00e,		   si20,     rd)
# define   PCADDU18I(rd, si20)		 os20r(0x00f,		   si20,     rd)
# define	LL_W(rd, rj, si14)	os14rr(0x020,	     si14,   rj,     rd)
# define	SC_W(rd, rj, si14)	os14rr(0x021,	     si14,   rj,     rd)
# define	LL_D(rd, rj, si14)	os14rr(0x022,	     si14,   rj,     rd)
# define	SC_D(rd, rj, si14)	os14rr(0x023,	     si14,   rj,     rd)
# define     LDPTR_W(rd, rj, si14)	os14rr(0x024,	     si14,   rj,     rd)
# define     SDPTR_W(rd, rj, si14)	os14rr(0x025,	     si14,   rj,     rd)
# define     LDPTR_D(rd, rj, si14)	os14rr(0x026,	     si14,   rj,     rd)
# define     SDPTR_D(rd, rj, si14)	os14rr(0x027,	     si14,   rj,     rd)
# define	LD_B(rd, rj, si12)	os12rr(0x0a0,	     si12,   rj,     rd)
# define	LD_H(rd, rj, si12)	os12rr(0x0a1,	     si12,   rj,     rd)
# define	LD_W(rd, rj, si12)	os12rr(0x0a2,	     si12,   rj,     rd)
# define	LD_D(rd, rj, si12)	os12rr(0x0a3,	     si12,   rj,     rd)
# define	ST_B(rd, rj, si12)	os12rr(0x0a4,	     si12,   rj,     rd)
# define	ST_H(rd, rj, si12)	os12rr(0x0a5,	     si12,   rj,     rd)
# define	ST_W(rd, rj, si12)	os12rr(0x0a6,	     si12,   rj,     rd)
# define	ST_D(rd, rj, si12)	os12rr(0x0a7,	     si12,   rj,     rd)
# define       LD_BU(rd, rj, si12)	os12rr(0x0a8,	     si12,   rj,     rd)
# define       LD_HU(rd, rj, si12)	os12rr(0x0a9,	     si12,   rj,     rd)
# define       LD_WU(rd, rj, si12)	os12rr(0x0aa,	     si12,   rj,     rd)
# define       PRELD(hint, rj, si12)   os12ru5(0x0ab,	     si12,   rj ,  hint)
# define       LDX_B(rd, rj, rk)	 orrr(0x7000,	       rk,   rj,     rd)
# define       LDX_H(rd, rj, rk)	 orrr(0x7008,	       rk,   rj,     rd)
# define       LDX_W(rd, rj, rk)	 orrr(0x7010,	       rk,   rj,     rd)
# define       LDX_D(rd, rj, rk)	 orrr(0x7018,	       rk,   rj,     rd)
# define       STX_B(rd, rj, rk)	 orrr(0x7020,	       rk,   rj,     rd)
# define       STX_H(rd, rj, rk)	 orrr(0x7028,	       rk,   rj,     rd)
# define       STX_W(rd, rj, rk)	 orrr(0x7030,	       rk,   rj,     rd)
# define       STX_D(rd, rj, rk)	 orrr(0x7038,	       rk,   rj,     rd)
# define      LDX_BU(rd, rj, rk)	 orrr(0x7040,	       rk,   rj,     rd)
# define      LDX_HU(rd, rj, rk)	 orrr(0x7048,	       rk,   rj,     rd)
# define      LDX_WU(rd, rj, rk)	 orrr(0x7050,	       rk,   rj,     rd)
# define      PRELDX(hint, rj, rk)	orru5(0x7058,	       rk,   rj,   hint)
# define    AMSWAP_W(rd, rj, rk)	 orrr(0x70c0,	       rk,   rj,     rd)
# define    AMSWAP_D(rd, rj, rk)	 orrr(0x70c1,	       rk,   rj,     rd)
# define     AMADD_W(rd, rj, rk)	 orrr(0x70c2,	       rk,   rj,     rd)
# define     AMADD_D(rd, rj, rk)	 orrr(0x70c3,	       rk,   rj,     rd)
# define     AMAND_W(rd, rj, rk)	 orrr(0x70c4,	       rk,   rj,     rd)
# define     AMAND_D(rd, rj, rk)	 orrr(0x70c5,	       rk,   rj,     rd)
# define      AMOR_W(rd, rj, rk)	 orrr(0x70c6,	       rk,   rj,     rd)
# define      AMOR_D(rd, rj, rk)	 orrr(0x70c7,	       rk,   rj,     rd)
# define     AMXOR_W(rd, rj, rk)	 orrr(0x70c8,	       rk,   rj,     rd)
# define     AMXOR_D(rd, rj, rk)	 orrr(0x70c9,	       rk,   rj,     rd)
# define     AMMAX_W(rd, rj, rk)	 orrr(0x70ca,	       rk,   rj,     rd)
# define     AMMAX_D(rd, rj, rk)	 orrr(0x70cb,	       rk,   rj,     rd)
# define     AMMIN_W(rd, rj, rk)	 orrr(0x70cc,	       rk,   rj,     rd)
# define     AMMIN_D(rd, rj, rk)	 orrr(0x70cd,	       rk,   rj,     rd)
# define    AMMAX_WU(rd, rj, rk)	 orrr(0x70ce,	       rk,   rj,     rd)
# define    AMMAX_DU(rd, rj, rk)	 orrr(0x70cf,	       rk,   rj,     rd)
# define    AMMIN_WU(rd, rj, rk)	 orrr(0x70d0,	       rk,   rj,     rd)
# define    AMMIN_DU(rd, rj, rk)	 orrr(0x70d1,	       rk,   rj,     rd)
# define AMSWAP_DB_W(rd, rj, rk)	 orrr(0x70d2,	       rk,   rj,     rd)
# define AMSWAP_DB_D(rd, rj, rk)	 orrr(0x70d3,	       rk,   rj,     rd)
# define  AMADD_DB_W(rd, rj, rk)	 orrr(0x70d4,	       rk,   rj,     rd)
# define  AMADD_DB_D(rd, rj, rk)	 orrr(0x70d5,	       rk,   rj,     rd)
# define  AMAND_DB_W(rd, rj, rk)	 orrr(0x70d6,	       rk,   rj,     rd)
# define  AMAND_DB_D(rd, rj, rk)	 orrr(0x70d7,	       rk,   rj,     rd)
# define   AMOR_DB_W(rd, rj, rk)	 orrr(0x70d8,	       rk,   rj,     rd)
# define   AMOR_DB_D(rd, rj, rk)	 orrr(0x70d9,	       rk,   rj,     rd)
# define  AMXOR_DB_W(rd, rj, rk)	 orrr(0x70da,	       rk,   rj,     rd)
# define  AMXOR_DB_D(rd, rj, rk)	 orrr(0x70db,	       rk,   rj,     rd)
# define  AMMAX_DB_W(rd, rj, rk)	 orrr(0x70dc,	       rk,   rj,     rd)
# define  AMMAX_DB_D(rd, rj, rk)	 orrr(0x70dd,	       rk,   rj,     rd)
# define  AMMIN_DB_W(rd, rj, rk)	 orrr(0x70de,	       rk,   rj,     rd)
# define  AMMIN_DB_D(rd, rj, rk)	 orrr(0x70df,	       rk,   rj,     rd)
# define AMMAX_DB_WU(rd, rj, rk)	 orrr(0x70e0,	       rk,   rj,     rd)
# define AMMAX_DB_DU(rd, rj, rk)	 orrr(0x70e1,	       rk,   rj,     rd)
# define AMMIN_DB_WU(rd, rj, rk)	 orrr(0x70e2,	       rk,   rj,     rd)
# define AMMIN_DB_DU(rd, rj, rk)	 orrr(0x70e3,	       rk,   rj,     rd)
# define	DBAR(hint)		 ou15(0x70e4,			   hint)
# define	IBAR(hint)		 ou15(0x70e5,			   hint)
# define      LDGT_B(rd, rj, rk)	 orrr(0x70f0,	       rk,   rj,     rd)
# define      LDGT_H(rd, rj, rk)	 orrr(0x70f1,	       rk,   rj,     rd)
# define      LDGT_W(rd, rj, rk)	 orrr(0x70f2,	       rk,   rj,     rd)
# define      LDGT_D(rd, rj, rk)	 orrr(0x70f3,	       rk,   rj,     rd)
# define      LDLE_B(rd, rj, rk)	 orrr(0x70f4,	       rk,   rj,     rd)
# define      LDLE_H(rd, rj, rk)	 orrr(0x70f5,	       rk,   rj,     rd)
# define      LDLE_W(rd, rj, rk)	 orrr(0x70f6,	       rk,   rj,     rd)
# define      LDLE_D(rd, rj, rk)	 orrr(0x70f7,	       rk,   rj,     rd)
# define      STGT_B(rd, rj, rk)	 orrr(0x70f8,	       rk,   rj,     rd)
# define      STGT_H(rd, rj, rk)	 orrr(0x70f9,	       rk,   rj,     rd)
# define      STGT_W(rd, rj, rk)	 orrr(0x70fa,	       rk,   rj,     rd)
# define      STGT_D(rd, rj, rk)	 orrr(0x70fb,	       rk,   rj,     rd)
# define      STLE_B(rd, rj, rk)	 orrr(0x70fc,	       rk,   rj,     rd)
# define      STLE_H(rd, rj, rk)	 orrr(0x70rd,	       rk,   rj,     rd)
# define      STLE_W(rd, rj, rk)	 orrr(0x70fe,	       rk,   rj,     rd)
# define      STLE_D(rd, rj, rk)	 orrr(0x70ff,	       rk,   rj,     rd)
# define	BEQZ(	 rj, offs)	 orj21(0x010,		     rj,   offs)
# define	BNEZ(	 rj, offs)	 orj21(0x011,		     rj,   offs)
# define       BCEQZ(	 cj, offs)    ou2u3j21(0x012,		0x0, cj,   offs)
# define       BCNEZ(	 cj, offs)    ou2u3j21(0x012,		0x1, cj,   offs)
# define	JIRL(rd, rj, offs)	oj16rr(0x013,	     offs,   rj,     rd)
# define	   B(offs)		  oj26(0x014,			   offs)
# define	  BL(offs)		  oj26(0x015,			   offs)
# define	 BEQ(rj, rd, offs)	oj16rr(0x016,	     offs,   rj,     rd)
# define	 BNE(rj, rd, offs)	oj16rr(0x017,	     offs,   rj,     rd)
# define	 BLT(rj, rd, offs)	oj16rr(0x018,	     offs,   rj,     rd)
# define	 BGE(rj, rd, offs)	oj16rr(0x019,	     offs,   rj,     rd)
# define	BLTU(rj, rd, offs)	oj16rr(0x01a,	     offs,   rj,     rd)
# define	BGEU(rj, rd, offs)	oj16rr(0x01b,	     offs,   rj,     rd)
# define NOP()				ANDI(_ZERO_REGNO, _ZERO_REGNO, 0)
# define nop(i0)			_nop(_jit, i0)
# define comr(r0, r1)			NOR(r0, r1, r1)
# define negr(r0, r1)			subr(r0, _ZERO_REGNO, r1)
# define clor(r0, r1)			CLO_D(r0, r1)
# define clzr(r0, r1)			CLZ_D(r0, r1)
# define ctor(r0, r1)			CTO_D(r0, r1)
# define ctzr(r0, r1)			CTZ_D(r0, r1)
static void _nop(jit_state_t*,jit_int32_t);
# define movr(r0, r1)			_movr(_jit, r0, r1)
static void _movr(jit_state_t*, jit_int32_t, jit_int32_t);
# define movi(r0, i0)			_movi(_jit, r0, i0)
static void _movi(jit_state_t*, jit_int32_t, jit_word_t);
# define movi_p(r0, i0)			_movi_p(_jit, r0, i0)
static jit_word_t _movi_p(jit_state_t*, jit_int32_t, jit_word_t);
#  define movnr(r0, r1, r2)		_movnr(_jit, r0, r1, r2)
static void _movnr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define movzr(r0, r1, r2)		_movzr(_jit, r0, r1, r2)
static void _movzr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define casx(r0, r1, r2, r3, i0)	_casx(_jit, r0, r1, r2, r3, i0)
static void _casx(jit_state_t *_jit,jit_int32_t,jit_int32_t,
		  jit_int32_t,jit_int32_t,jit_word_t);
#define casr(r0, r1, r2, r3)		casx(r0, r1, r2, r3, 0)
#define casi(r0, i0, r1, r2)		casx(r0, _NOREG, r1, r2, i0)
# define addr(r0, r1, r2)		ADD_D(r0, r1, r2)
# define addi(r0, r1, i0)		_addi(_jit, r0, r1, i0)
static void _addi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define addcr(r0, r1, r2)		_addcr(_jit, r0, r1, r2)
static void _addcr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define addci(r0, r1, i0)		_addci(_jit, r0, r1, i0)
static void _addci(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define addxr(r0, r1, r2)		_addxr(_jit, r0, r1, r2)
static void _addxr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define addxi(r0, r1, i0)		_addxi(_jit, r0, r1, i0)
static void _addxi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define subr(r0, r1, r2)		SUB_D(r0, r1, r2)
# define subi(r0, r1, i0)		_subi(_jit, r0, r1, i0)
static void _subi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define subcr(r0, r1, r2)		_subcr(_jit, r0, r1, r2)
static void _subcr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define subci(r0, r1, i0)		_subci(_jit, r0, r1, i0)
static void _subci(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define subxr(r0, r1, r2)		_subxr(_jit, r0, r1, r2)
static void _subxr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define subxi(r0, r1, i0)		_subxi(_jit, r0, r1, i0)
static void _subxi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define mulr(r0, r1, r2)		MUL_D(r0, r1, r2)
# define muli(r0, r1, i0)		_muli(_jit, r0, r1, i0)
static void _muli(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define qmulr(r0, r1, r2, r3)		iqmulr(r0, r1, r2, r3, 1)
# define qmulr_u(r0, r1, r2, r3)	iqmulr(r0, r1, r2, r3, 0)
# define iqmulr(r0, r1, r2, r3, sign)	_iqmulr(_jit, r0, r1, r2, r3, sign)
static void _iqmulr(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t, jit_int32_t, jit_bool_t);
# define qmuli(r0, r1, r2, i0)		iqmuli(r0, r1, r2, i0, 1)
# define qmuli_u(r0, r1, r2, i0)	iqmuli(r0, r1, r2, i0, 0)
# define iqmuli(r0, r1, r2, i0, sign)	_iqmuli(_jit, r0, r1, r2, i0, sign)
static void _iqmuli(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t, jit_word_t, jit_bool_t);
#  define divr(r0, r1, r2)		DIV_D(r0, r1, r2)
#  define divi(r0, r1, i0)		_divi(_jit, r0, r1, i0)
static void _divi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define divr_u(r0, r1, r2)		DIV_DU(r0, r1, r2)
#  define divi_u(r0, r1, i0)		_divi_u(_jit, r0, r1, i0)
static void _divi_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define qdivr(r0, r1, r2, r3)		iqdivr(r0, r1, r2, r3, 1)
#  define qdivr_u(r0, r1, r2, r3)	iqdivr(r0, r1, r2, r3, 0)
#  define iqdivr(r0, r1, r2, r3, sign)	_iqdivr(_jit, r0, r1, r2, r3, sign)
static void _iqdivr(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t, jit_int32_t, jit_bool_t);
#  define qdivi(r0, r1, r2, i0)		iqdivi(r0, r1, r2, i0, 1)
#  define qdivi_u(r0, r1, r2, i0)	iqdivi(r0, r1, r2, i0, 0)
#  define iqdivi(r0, r1, r2, i0, sign)	_iqdivi(_jit, r0, r1, r2, i0, sign)
static void _iqdivi(jit_state_t*, jit_int32_t, jit_int32_t,
		    jit_int32_t, jit_word_t, jit_bool_t);
#  define remr(r0, r1, r2)		MOD_D(r0, r1, r2)
#  define remi(r0, r1, i0)		_remi(_jit, r0, r1, i0)
static void _remi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define remr_u(r0, r1, r2)		MOD_DU(r0, r1, r2)
#  define remi_u(r0, r1, i0)		_remi_u(_jit, r0, r1, i0)
static void _remi_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define lshr(r0, r1, r2)		SLL_D(r0, r1, r2)
# define lshi(r0, r1, i0)		SLLI_D(r0, r1, i0)
# define rshr(r0, r1, r2)		SRA_D(r0, r1, r2)
# define rshi(r0, r1, i0)		SRAI_D(r0, r1, i0)
# define rshr_u(r0, r1, r2)		SRL_D(r0, r1, r2)
# define rshi_u(r0, r1, i0)		SRLI_D(r0, r1, i0)
# define andr(r0, r1, r2)		AND(r0, r1, r2)
# define andi(r0, r1, i0)		_andi(_jit, r0, r1, i0)
static void _andi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define orr(r0, r1, r2)		OR(r0, r1, r2)
# define ori(r0, r1, i0)		_ori(_jit, r0, r1, i0)
static void _ori(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define xorr(r0, r1, r2)		XOR(r0, r1, r2)
# define xori(r0, r1, i0)		_xori(_jit, r0, r1, i0)
static void _xori(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ldr_c(r0, r1)			LD_B(r0, r1, 0)
# define ldi_c(r0, i0)			_ldi_c(_jit, r0, i0)
static void _ldi_c(jit_state_t*, jit_int32_t, jit_word_t);
# define ldr_uc( r0, r1)		LD_BU(r0, r1, 0)
# define ldi_uc(r0, i0)			_ldi_uc(_jit, r0, i0)
static void _ldi_uc(jit_state_t*, jit_int32_t, jit_word_t);
# define ldr_s(r0, r1)			LD_H(r0, r1, 0)
# define ldi_s(r0, i0)			_ldi_s(_jit, r0, i0)
static void _ldi_s(jit_state_t*, jit_int32_t, jit_word_t);
# define ldr_us(r0, r1)			LD_HU(r0, r1, 0)
# define ldi_us(r0, i0)			_ldi_us(_jit, r0, i0)
static void _ldi_us(jit_state_t*, jit_int32_t, jit_word_t);
# define ldr_i(r0, r1)			LD_W(r0, r1, 0)
# define ldi_i(r0, i0)			_ldi_i(_jit, r0, i0)
static void _ldi_i(jit_state_t*, jit_int32_t, jit_word_t);
# define ldr_ui(r0, r1)			LD_WU(r0, r1, 0)
# define ldi_ui(r0, i0)			_ldi_ui(_jit, r0, i0)
static void _ldi_ui(jit_state_t*, jit_int32_t, jit_word_t);
# define ldr_l(r0, r1)			LD_D(r0, r1, 0)
# define ldi_l(r0, i0)			_ldi_l(_jit, r0, i0)
static void _ldi_l(jit_state_t*, jit_int32_t, jit_word_t);
# define ldxr_c(r0, r1, r2)		LDX_B(r0, r1, r2)
# define ldxi_c(r0, r1, i0)		_ldxi_c(_jit, r0, r1, i0)
static void _ldxi_c(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ldxr_uc(r0, r1, r2)		LDX_BU(r0, r1, r2)
# define ldxi_uc(r0, r1, i0)		_ldxi_uc(_jit,r0, r1, i0)
static void _ldxi_uc(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ldxr_s(r0, r1, r2)		LDX_H(r0, r1, r2)
# define ldxi_s(r0, r1, i0)		_ldxi_s(_jit, r0, r1, i0)
static void _ldxi_s(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ldxr_us(r0, r1, r2)		LDX_HU(r0, r1, r2)
# define ldxi_us(r0, r1, i0)		_ldxi_us(_jit, r0, r1, i0)
static void _ldxi_us(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ldxr_i(r0, r1, r2)		LDX_W(r0, r1, r2)
# define ldxi_i(r0, r1, i0)		_ldxi_i(_jit, r0, r1, i0)
static void _ldxi_i(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ldxr_ui(r0, r1, r2)		LDX_WU(r0, r1, r2)
# define ldxi_ui(r0, r1, i0)		_ldxi_ui(_jit, r0, r1, i0)
static void _ldxi_ui(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ldxr_l(r0, r1, r2)		LDX_D(r0, r1, r2)
# define ldxi_l(r0, r1, i0)		_ldxi_l(_jit, r0, r1, i0)
static void _ldxi_l(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define str_c(r0, r1)			ST_B(r1, r0, 0)
# define sti_c(i0, r0)			_sti_c(_jit, i0, r0)
static void _sti_c(jit_state_t*, jit_word_t, jit_int32_t);
# define str_s(r0, r1)			ST_H(r1, r0, 0)
# define sti_s(i0, r0)			_sti_s(_jit, i0, r0)
static void _sti_s(jit_state_t*, jit_word_t, jit_int32_t);
# define str_i(r0, r1)			ST_W(r1, r0, 0)
# define sti_i(i0, r0)			_sti_i(_jit, i0, r0)
static void _sti_i(jit_state_t*, jit_word_t, jit_int32_t);
# define str_l(r0, r1)			ST_D(r1, r0, 0)
# define sti_l(i0, r0)			_sti_l(_jit, i0, r0)
static void _sti_l(jit_state_t*, jit_word_t, jit_int32_t);
# define stxr_c(r0, r1, r2)		STX_B(r2, r1, r0)
# define stxi_c(i0, r0, r1)		_stxi_c(_jit,i0, r0, r1)
static void _stxi_c(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define stxr_s(r0, r1, r2)		STX_H(r2, r1, r0)
# define stxi_s(i0, r0, r1)		_stxi_s(_jit, i0, r0, r1)
static void _stxi_s(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define stxr_i(r0, r1, r2)		STX_W(r2, r1, r0)
# define stxi_i(i0, r0, r1)		_stxi_i(_jit, i0, r0, r1)
static void _stxi_i(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define stxr_l(r0, r1, r2)		STX_D(r2, r1, r0)
# define stxi_l(i0, r0, r1)		_stxi_l(_jit, i0, r0, r1)
static void _stxi_l(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bswapr_us(r0,r1)		_bswapr_us(_jit,r0,r1)
static void _bswapr_us(jit_state_t*, jit_int32_t, jit_int32_t);
# define bswapr_ui(r0,r1)		_bswapr_ui(_jit,r0,r1)
static void _bswapr_ui(jit_state_t*, jit_int32_t, jit_int32_t);
# define bswapr_ul(r0, r1)		REVB_D(r0, r1)
# define extr_c(r0, r1)			EXT_W_B(r0, r1)
# define extr_uc(r0, r1)		BSTRPICK_D(r0, r1, 7, 0)
# define extr_s(r0, r1)			EXT_W_H(r0, r1)
# define extr_us(r0, r1)		BSTRPICK_D(r0, r1, 15, 0)
# define extr_i(r0, r1)			SLLI_W(r0, r1, 0)
# define extr_ui(r0, r1)		BSTRPICK_D(r0, r1, 31, 0)
# define ltr(r0, r1, r2)		SLT(r0, r1, r2)
# define lti(r0, r1, i0)		_lti(_jit, r0, r1, i0)
static void _lti(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ltr_u(r0, r1, r2)		SLTU(r0, r1, r2)
# define lti_u(r0, r1, i0)		_lti_u(_jit, r0, r1, i0)
static void _lti_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ler(r0, r1, r2)		_ler(_jit, r0, r1, r2)
static void _ler(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define lei(r0, r1, i0)		_lei(_jit, r0, r1, i0)
static void _lei(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ler_u(r0, r1, r2)		_ler_u(_jit, r0, r1, r2)
static void _ler_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define lei_u(r0, r1, i0)		_lei_u(_jit, r0, r1, i0)
static void _lei_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define eqr(r0, r1, r2)		_eqr(_jit, r0, r1, r2)
static void _eqr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define eqi(r0, r1, i0)		_eqi(_jit, r0, r1, i0)
static void _eqi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ger(r0, r1, r2)		_ger(_jit, r0, r1, r2)
static void _ger(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define gei(r0, r1, i0)		_gei(_jit, r0, r1, i0)
static void _gei(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ger_u(r0, r1, r2)		_ger_u(_jit, r0, r1, r2)
static void _ger_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define gei_u(r0, r1, i0)		_gei_u(_jit, r0, r1, i0)
static void _gei_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define gtr(r0, r1, r2)		SLT(r0, r2, r1)
# define gti(r0, r1, i0)		_gti(_jit, r0, r1, i0)
static void _gti(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define gtr_u(r0, r1, r2)		SLTU(r0, r2, r1)
# define gti_u(r0, r1, i0)		_gti_u(_jit, r0, r1, i0)
static void _gti_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define ner(r0, r1, r2)		_ner(_jit, r0, r1, r2)
static void _ner(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
# define nei(r0, r1, i0)		_nei(_jit, r0, r1, i0)
static void _nei(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
# define bltr(i0, r0, r1)		_bltr(_jit, i0, r0, r1)
static jit_word_t _bltr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blti(i0, r0, i1)		_blti(_jit, i0, r0, i1)
static jit_word_t _blti(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bltr_u(i0, r0, r1)		_bltr_u(_jit, i0, r0, r1)
static jit_word_t _bltr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blti_u(i0, r0, i1)		_blti_u(_jit, i0, r0, i1)
static jit_word_t _blti_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bler(i0, r0, r1)		_bler(_jit, i0, r0, r1)
static jit_word_t _bler(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blei(i0, r0, i1)		_blei(_jit, i0, r0, i1)
static jit_word_t _blei(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bler_u(i0, r0, r1)		_bler_u(_jit, i0, r0, r1)
static jit_word_t _bler_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define blei_u(i0, r0, i1)		_blei_u(_jit, i0, r0, i1)
static jit_word_t _blei_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define beqr(i0, r0, r1)		_beqr(_jit, i0, r0, r1)
static jit_word_t _beqr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define beqi(i0, r0, i1)		_beqi(_jit, i0, r0, i1)
static jit_word_t _beqi(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bger(i0, r0, r1)		_bger(_jit, i0, r0, r1)
static jit_word_t _bger(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgei(i0, r0, i1)		_bgei(_jit, i0, r0, i1)
static jit_word_t _bgei(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bger_u(i0, r0, r1)		_bger_u(_jit, i0, r0, r1)
static jit_word_t _bger_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgei_u(i0, r0, i1)		_bgei_u(_jit, i0, r0, i1)
static jit_word_t _bgei_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bgtr(i0, r0, r1)		_bgtr(_jit, i0, r0, r1)
static jit_word_t _bgtr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgti(i0, r0, i1)		_bgti(_jit, i0, r0, i1)
static jit_word_t _bgti(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bgtr_u(i0, r0, r1)		_bgtr_u(_jit, i0, r0, r1)
static jit_word_t _bgtr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bgti_u(i0, r0, i1)		_bgti_u(_jit, i0, r0, i1)
static jit_word_t _bgti_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bner(i0, r0, r1)		_bner(_jit, i0, r0, r1)
static jit_word_t _bner(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bnei(i0, r0, i1)		_bnei(_jit, i0, r0, i1)
static jit_word_t _bnei(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define jmpr(r0)			JIRL(_ZERO_REGNO, r0, 0)
# define jmpi(i0)			_jmpi(_jit, i0)
static jit_word_t _jmpi(jit_state_t*, jit_word_t);
# define jmpi_p(i0)			_jmpi_p(_jit, i0)
static jit_word_t _jmpi_p(jit_state_t*, jit_word_t);
# define boaddr(i0, r0, r1)		_boaddr(_jit, i0, r0, r1)
static jit_word_t _boaddr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define boaddi(i0, r0, i1)		_boaddi(_jit, i0, r0, i1)
static jit_word_t _boaddi(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define boaddr_u(i0, r0, r1)		_boaddr_u(_jit, i0, r0, r1)
static jit_word_t _boaddr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define boaddi_u(i0, r0, i1)		_boaddi_u(_jit, i0, r0, i1)
static jit_word_t _boaddi_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bxaddr(i0, r0, r1)		_bxaddr(_jit, i0, r0, r1)
static jit_word_t _bxaddr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bxaddi(i0, r0, i1)		_bxaddi(_jit, i0, r0, i1)
static jit_word_t _bxaddi(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bxaddr_u(i0, r0, r1)		_bxaddr_u(_jit, i0, r0, r1)
static jit_word_t _bxaddr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bxaddi_u(i0, r0, i1)		_bxaddi_u(_jit, i0, r0, i1)
static jit_word_t _bxaddi_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bosubr(i0, r0, r1)		_bosubr(_jit, i0, r0, r1)
static jit_word_t _bosubr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bosubi(i0, r0, i1)		_bosubi(_jit, i0, r0, i1)
static jit_word_t _bosubi(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bosubr_u(i0, r0, r1)		_bosubr_u(_jit, i0, r0, r1)
static jit_word_t _bosubr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bosubi_u(i0, r0, i1)		_bosubi_u(_jit, i0, r0, i1)
static jit_word_t _bosubi_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bxsubr(i0, r0, r1)		_bxsubr(_jit, i0, r0, r1)
static jit_word_t _bxsubr(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bxsubi(i0, r0, i1)		_bxsubi(_jit, i0, r0, i1)
static jit_word_t _bxsubi(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bxsubr_u(i0, r0, r1)		_bxsubr_u(_jit, i0, r0, r1)
static jit_word_t _bxsubr_u(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);
# define bxsubi_u(i0, r0, i1)		_bxsubi_u(_jit, i0, r0, i1)
static jit_word_t _bxsubi_u(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
# define bmsr(br, r0, r1)		_bmsr(_jit, br, r0, r1)
static jit_word_t _bmsr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
# define bmsi(br, r0, i0)		_bmsi(_jit, br, r0, i0)
static jit_word_t _bmsi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
# define bmcr(br, r0, r1)		_bmcr(_jit, br, r0, r1)
static jit_word_t _bmcr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
# define bmci(br, r0, i0)		_bmci(_jit, br, r0, i0)
static jit_word_t _bmci(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
# define callr(r0)			JIRL(_RA_REGNO, r0, 0)
# define calli(i0)			_calli(_jit, i0)
static jit_word_t _calli(jit_state_t*, jit_word_t);
# define calli_p(i0)			_calli_p(_jit, i0)
static jit_word_t _calli_p(jit_state_t*, jit_word_t);
# define prolog(i0)			_prolog(_jit, i0)
static void _prolog(jit_state_t*, jit_node_t*);
# define epilog(i0)			_epilog(_jit, i0)
static void _epilog(jit_state_t*, jit_node_t*);
# define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
# define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#define patch_at(jump, label)		_patch_at(_jit, jump, label)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
#endif

#if CODE
static void
_orrr(jit_state_t *_jit,
      jit_int32_t op, jit_int32_t rk, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0xffff));
    assert(!(rk &   ~0x1f));
    assert(!(rj &   ~0x1f));
    assert(!(rd &   ~0x1f));
    ii((op << 15) | (rk << 10) | (rj << 5) | rd);
}

static void
_ou2rrr(jit_state_t *_jit, jit_int32_t op,
	jit_int32_t u2, jit_int32_t rk, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0x7ff));
    assert(!(u2 &     ~3));
    assert(!(rk &  ~0x1f));
    assert(!(rj &  ~0x1f));
    assert(!(rd &  ~0x1f));
    ii((op << 17) | (u2 << 15) | (rk << 10) | (rj << 5) | rd);
}

static void
_ou3rrr(jit_state_t *_jit, jit_int32_t op,
	jit_int32_t u3, jit_int32_t rk, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0x3ff));
    assert(!(u3 &     ~7));
    assert(!(rk &  ~0x1f));
    assert(!(rj &  ~0x1f));
    assert(!(rd &  ~0x1f));
    ii((op << 18) | (u3 << 15) | (rk << 10) | (rj << 5) | rd);
}

static void
_ou6rr(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t u6, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0x3ff));
    assert(!(u6 &  ~0x3f));
    assert(!(rj &  ~0x1f));
    assert(!(rd &  ~0x1f));
    ii((op << 16) | (u6 << 10) | (rj << 5) | rd);
}

static void
_ou5u1u5rr(jit_state_t *_jit, jit_int32_t op, jit_int32_t m5,
	   jit_int32_t u1, jit_int32_t l5, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0x7ff));
    assert(!(m5 &  ~0x1f));
    assert(!(u1  &     ~1));
    assert(!(l5 &  ~0x1f));
    assert(!(rj &  ~0x1f));
    assert(!(rd &  ~0x1f));
    ii((op << 21) | (m5 << 16) | (u1 << 15) | (l5 << 10) | (rj << 5) | rd);
}

static void
_ou6u6rr(jit_state_t *_jit, jit_int32_t op,
	 jit_int32_t m6, jit_int32_t l6, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0x3ff));
    assert(!(m6 &  ~0x3f));
    assert(!(l6 &  ~0x3f));
    assert(!(rj &  ~0x1f));
    assert(!(rd &  ~0x1f));
    ii((op << 22) | (m6 << 16) | (l6 << 10) | (rj << 5) | rd);
}

static void
_o5r23(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t i5, jit_int32_t rj, jit_int32_t i2, jit_int32_t i3)
{
    assert(!(op & ~0xffff));
    assert(!(i5 &   ~0x3f));
    assert(!(rj &   ~0x3f));
    assert(!(i2 &    ~0x3));
    assert(!(i3 &   ~0x1f));
    ii((op << 15) | (i5 << 10) | (rj << 5) | (i2 << 3) | i3);
}

static void
_o523r(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t i5, jit_int32_t i2, jit_int32_t i3, jit_int32_t rd)
{
    assert(!(op & ~0xffff));
    assert(!(i5 &   ~0x3f));
    assert(!(i2 &    ~0x3));
    assert(!(i3 &    ~0x7));
    assert(!(rd &   ~0x3f));
    ii((op << 15) | (i5 << 10) | (i2 << 8) | (i3 << 5) | rd);
}

static void
_os12rr(jit_state_t *_jit,
	jit_int32_t op, jit_int32_t s12, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op  &  ~0x3ff));
    assert(s12   <=   2047 && s12 >= -2048);	 s12 &= 0xfff;
    assert(!(rj  &   ~0x1f));
    assert(!(rd  &   ~0x1f));
    ii((op << 22) | (s12 << 10) | (rj << 5) | rd);
}

static void
_ou12rr(jit_state_t *_jit,
	jit_int32_t op, jit_int32_t u12, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op  & ~0x3ff));
    assert(!(u12 & ~0xfff));
    assert(!(rj  &  ~0x1f));
    assert(!(rd  &  ~0x1f));
    ii((op << 22) | (u12 << 10) | (rj << 5) | rd);
}

static void
_ou14u5r(jit_state_t *_jit,
	 jit_int32_t op, jit_int32_t u14, jit_int32_t u5, jit_int32_t rd)
{
    assert(!(op  &   ~0xff));
    assert(!(u14 & ~0x3fff));
    assert(!(u5 &    ~0x1f));
    assert(!(rd &    ~0x1f));
    ii((op << 24) | (u14 << 10) | (u5 << 5) | rd);
}

static void
_os14rr(jit_state_t *_jit,
	jit_int32_t op, jit_int32_t s14, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op &   ~0xff));
    assert(s14  <=    8191 && s14 >= -8192);		s14 &= 0x3fff;
    assert(!(rj &    ~0x1f));
    assert(!(rd &    ~0x1f));
    ii((op << 24) | (s14 << 10) | (rj << 5) | rd);
}

static void
_ou8rr(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t u8, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0x3fff));
    assert(!(u8 &   ~0xff));
    assert(!(rj &   ~0x1f));
    assert(!(rd &   ~0x1f));
    ii((op << 18) | (u8 << 10) | (rj << 5) | rd);
}

static void
_ou15(jit_state_t *_jit, jit_int32_t op, jit_int32_t u15)
{
    assert(!(op  & ~0x1ffff));
    assert(!(u15 &  ~0x7fff));
    ii((op << 15) | u15);
}

static void
_orrrr(jit_state_t *_jit, jit_int32_t op,
       jit_int32_t ra, jit_int32_t rk, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op & ~0xfff));
    assert(!(ra &  ~0x1f));
    assert(!(rk &  ~0x1f));
    assert(!(rj &  ~0x1f));
    assert(!(rd &  ~0x1f));
    ii((op << 20) | (ra << 15) | (rk << 10) | (rj << 5) | rd);
}

static void
_ou5rru2u3(jit_state_t *_jit,jit_int32_t op, jit_int32_t u5,
	   jit_int32_t rk, jit_int32_t rj, jit_int32_t u2, jit_int32_t u3)
{
    assert(!(op & ~0xfff));
    assert(!(u5 &  ~0x1f));
    assert(!(rk &  ~0x1f));
    assert(!(rj &  ~0x1f));
    assert(!(u2 &     ~3));
    assert(!(u3 &     ~7));
    ii((op << 20) | (u5 << 15) | (rk << 10) | (rj << 5) | (u2 << 3) | u3);
}

static void
_os16rr(jit_state_t *_jit,
	jit_int32_t op, jit_int32_t s16, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op &   ~0x3f));
    assert(s16 <=    32767 && s16 >= -32768);		s16 &= 0xffff;
    assert(!(rj &    ~0x1f));
    assert(!(rd &    ~0x1f));
    ii((op << 26) | (s16 << 10) | (rj << 5) | rd);
}

static void
_os20r(jit_state_t *_jit, jit_int32_t op, jit_int32_t s20, jit_int32_t rd)
{
    assert(!(op &  ~0x7f));
    assert(s20 <= 524287 && s20 >= -524288);		s20 &= 0xfffff;
    assert(!(rd &  ~0x1f));
    ii((op << 25) | (s20 << 5) | rd);
}

static void
_orj21(jit_state_t *_jit, jit_int32_t op, jit_int32_t rj ,jit_int32_t j21)
{
    assert(!(op  &   ~0x7f));
    assert(j21  <= 1048575 && j21 >= -1048576);		j21 &= 0x1fffff;
    assert(!(rj  &   ~0x1f));
    ii((op << 26) | ((j21 & 0xffff) << 10) | (rj << 5) | (j21 >> 16));
}

static void
_ou2u3j21(jit_state_t *_jit,
	jit_int32_t op, jit_int32_t u2, jit_int32_t u3, jit_int32_t j21)
{
    assert(!(op  &     ~0x7f));
    assert(j21  <= 1048575 && j21 >= -1048576);		j21 &= 0x1fffff;
    assert(!(u2  &        ~3));
    assert(!(u3  &        ~7));
    ii((op << 26) | ((j21 & 0xffff) << 10) | (u2 << 8) | (u3 << 5) | (j21 >> 16));
}

static void
_oj16rr(jit_state_t *_jit,
	jit_int32_t op, jit_int32_t j16, jit_int32_t rj, jit_int32_t rd)
{
    assert(!(op  &  ~0x7f));
    assert(j16  <=  32767 && j16 >= -32768);		j16 &= 0xffff;
    assert(!(rd  &  ~0x1f));
    assert(!(rj  &  ~0x1f));
    ii((op << 26) | (j16 << 10) | (rj << 5) | rd);
}

static void
_oj26(jit_state_t *_jit,  jit_int32_t op, jit_int32_t j26)
{
    assert(!(op  &      ~0x7f));
    assert(j26 <= 33554431 && j26 >= -33554432);	j26 &= 0x3ffffff;
    ii((op << 26) | ((j26 & 0x1ffffff) << 10) | (j26 >> 16));
}

static void
_nop(jit_state_t *_jit, jit_int32_t i0)
{
    for (; i0 > 0; i0 -= 4)
	NOP();
    assert(i0 == 0);
}

static void
_movnr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    MASKEQZ(rn(reg), r1, r2);
    MASKNEZ(r0, r0, r2);
    OR(r0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_movzr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    MASKNEZ(rn(reg), r1, r2);
    MASKEQZ(r0, r0, r2);
    OR(r0, r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_casx(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
      jit_int32_t r2, jit_int32_t r3, jit_word_t i0)
{
    jit_int32_t		t0, r1_reg, iscasi;
    jit_word_t		retry, done, jump0, jump1;
    if ((iscasi = (r1 == _NOREG))) {
	r1_reg = jit_get_reg(jit_class_gpr);
	r1 = rn(r1_reg);
	movi(r1, i0);
    }
    t0 = jit_get_reg(jit_class_gpr);
    movi(r0, 0);		/* Assume will fail */
    DBAR(0);			/* Barrier */
    retry = _jit->pc.w;
    LL_D(rn(t0), r1, 0);	/* Load current value */
    jump0 = _jit->pc.w;
    BNE(rn(t0), r2, 0);		/* If not equal, already done and r0 is zero */
    movr(r0, r3);		/* Make r0 an inout argument */
    SC_D(r0, r1, 0);		/* r0 is 0 if failed, 1 if succeed */
    jump1 = _jit->pc.w;
    BEQ(r0, _ZERO_REGNO, 0);
    /* FIXME Not certain what 0x700 hint means. Copied from C generated code */
    DBAR(0x700);
    done = _jit->pc.w;
    patch_at(jump0, done);
    patch_at(jump1, retry);
    jit_unget_reg(t0);
    if (iscasi)
	jit_unget_reg(r1_reg);
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	OR(r0, r1, _ZERO_REGNO);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    if (i0 == 0)
	OR(r0, _ZERO_REGNO, _ZERO_REGNO);
    else if (can_sign_extend_si12_p(i0))
	ADDI_D(r0, _ZERO_REGNO, i0);
    else if (!(i0 & 0xffff) && can_sign_extend_si16_p(i0 >> 16))
	ADDU16I_D(r0, _ZERO_REGNO, i0 >> 16);
    else {
	jit_word_t		w = i0 - _jit->pc.w;
	/* If loading some constant reachable address */
	if (can_sign_extend_si32_p(w)) {
	    jit_int32_t		lo = (jit_int32_t)w << 20 >> 20;
	    jit_int32_t		hi = w - lo;
	    PCADDU12I(r0, hi >> 12);
	    if (lo)
		ADDI_D(r0, r0, lo);
	}
	else {
	    jit_int32_t     _00_11, _12_31, _32_51, _52_63;
	    _00_11 =  i0	&   0xfff;
	    _12_31 = (i0 >> 12) & 0xfffff;
	    _32_51 = (i0 >> 32) & 0xfffff;
	    _52_63 = (i0 >> 52) &   0xfff;
	    if (_12_31) {
		LU12I_W(r0, _12_31 << 12 >> 12);
		if (_00_11)
		    ORI(r0, r0, _00_11);
	    }
	    else
		ORI(r0, _ZERO_REGNO, _00_11);
	    if (_32_51 || (_12_31 & 0x80000))
		LU32I_D(r0, _32_51 << 12 >> 12);
	    if (_52_63 || (_12_31 & 0x80000) || (_32_51 & 0x80000))
		LU52I_D(r0, r0, _52_63 << 20 >> 20);
	}
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t	_00_11, _12_31, _32_51, _52_63;
    _00_11 =  i0        &   0xfff;
    _12_31 = (i0 >> 12) & 0xfffff;
    _32_51 = (i0 >> 32) & 0xfffff;
    _52_63 = (i0 >> 52) &   0xfff;
    w = _jit->pc.w;
    LU12I_W(r0, _12_31 << 12 >> 12);
    ORI(r0, r0, _00_11);
    LU32I_D(r0, _32_51 << 12 >> 12);
    LU52I_D(r0, r0, _52_63 << 20 >> 20);
    return (w);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	movr(r0, r1);
    else if (can_sign_extend_si12_p(i0))
	ADDI_D(r0, r1, i0);
    else if (!(i0 & 0xffff) && can_sign_extend_si16_p(i0 >> 16))
	ADDU16I_D(r0, r1, i0 >> 16);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	t0 = jit_get_reg(jit_class_gpr);
	addr(rn(t0), r1, r2);
	SLTU(rn(jit_carry), rn(t0), r1);
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
    else {
	addr(r0, r1, r2);
	SLTU(rn(jit_carry), r0, r1);
    }
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    t0 = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	if (can_sign_extend_si12_p(i0))
	    ADDI_D(rn(t0), r1, i0);
	else if (!(i0 & 0xffff) && can_sign_extend_si16_p(i0 >> 16))
	    ADDU16I_D(rn(t0), r1, i0 >> 16);
	else {
	    movi(rn(t0), i0);
	    addr(rn(t0), r1, rn(t0));
	}
	SLTU(rn(jit_carry), rn(t0), r1);
	movr(r0, rn(t0));
    }
    else {
	if (can_sign_extend_si12_p(i0))
	    ADDI_D(r0, r1, i0);
	else if (!(i0 & 0xffff) && can_sign_extend_si16_p(i0 >> 16))
	    ADDU16I_D(r0, r1, i0 >> 16);
	else {
	    movi(rn(t0), i0);
	    addr(r0, r1, rn(t0));
	}
	SLTU(rn(jit_carry), r0, r1);
    }
    jit_unget_reg(t0);
}

static void
_addxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    addcr(r0, r1, r2);
    addcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    addci(r0, r1, i0);
    addcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	movr(r0, r1);
    else if (can_sign_extend_si12_p(-i0))
	ADDI_D(r0, r1, -i0);
    else if (!(-i0 & 0xffff) && can_sign_extend_si16_p(-i0 >> 16))
	ADDU16I_D(r0, r1, -i0 >> 16);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	t0 = jit_get_reg(jit_class_gpr);
	subr(rn(t0), r1, r2);
	SLTU(rn(jit_carry), r1, rn(t0));
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
    else {
	subr(r0, r1, r2);
	SLTU(rn(jit_carry), r1, r0);
    }
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    t0 = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	if (can_sign_extend_si12_p(-i0))
	    ADDI_D(rn(t0), r1, -i0);
	else if (!(-i0 & 0xffff) && can_sign_extend_si16_p(-i0 >> 16))
	    ADDU16I_D(rn(t0), r1, -i0 >> 16);
	else {
	    movi(rn(t0), i0);
	    subr(rn(t0), r1, rn(t0));
	}
	SLTU(rn(jit_carry), r1, rn(t0));
	movr(r0, rn(t0));
    }
    else {
	if (can_sign_extend_si12_p(-i0))
	    ADDI_D(r0, r1, -i0);
	else if (!(-i0 & 0xffff) && can_sign_extend_si16_p(-i0 >> 16))
	    ADDU16I_D(r0, r1, -i0 >> 16);
	else {
	    movi(rn(t0), i0);
	    subr(r0, r1, rn(t0));
	}
	SLTU(rn(jit_carry), r1, r0);
    }
    jit_unget_reg(t0);
}

static void
_subxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    subcr(r0, r1, r2);
    subcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    subci(r0, r1, i0);
    subcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_rsbi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    subi(r0, r1, i0);
    negr(r0, r0);
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    mulr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_iqmulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		t0;
    if (r0 == r2 || r0 == r3) {
	t0 = jit_get_reg(jit_class_gpr);
	mulr(rn(t0), r2, r3);
    }
    else
	mulr(r0, r2, r3);
    if (sign)
	MULH_D(r1, r2, r3);
    else
	MULH_DU(r1, r2, r3);
    if (r0 == r2 || r0 == r3) {
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_iqmuli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    iqmulr(r0, r1, r2, rn(reg), sign);
    jit_unget_reg(reg);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    divr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    divr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_iqdivr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		t0;
    if (r0 == r2 || r0 == r3)
	t0 = jit_get_reg(jit_class_gpr);
    else
	t0 = _NOREG;
    if (sign) {
	if (t0 == _NOREG)
	    DIV_D(r0, r2, r3);
	else
	    DIV_D(rn(t0), r2, r3);
	MOD_D(r1, r2, r3);
    }
    else {
	if (t0 == _NOREG)
	    DIV_DU(r0, r2, r3);
	else
	    DIV_DU(rn(t0), r2, r3);
	MOD_DU(r1, r2, r3);
    }
    if (t0 != _NOREG) {
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_iqdivi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    iqdivr(r0, r1, r2, rn(reg), sign);
    jit_unget_reg(reg);
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_zero_extend_u12_p(i0))
        ANDI(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
        AND(r0, r1, rn(reg));
        jit_unget_reg(reg);
    }
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_zero_extend_u12_p(i0))
        ORI(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
        OR(r0, r1, rn(reg));
        jit_unget_reg(reg);
    }
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_zero_extend_u12_p(i0))
        XORI(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
        XOR(r0, r1, rn(reg));
        jit_unget_reg(reg);
    }
}

static void
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_B(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LD_B(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_BU(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LD_BU(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_H(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LD_H(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_HU(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LD_HU(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_W(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LD_W(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_WU(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LD_WU(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_ldi_l(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_D(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LD_D(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_B(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDX_B(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_BU(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDX_BU(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_H(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDX_H(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_HU(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDX_HU(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_W(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDX_W(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_WU(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDX_WU(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	LD_D(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDX_D(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_B(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ST_B(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_H(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ST_H(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_W(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ST_W(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_sti_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_D(r0, _ZERO_REGNO, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ST_D(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_B(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	STX_B(r1, r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_H(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	STX_H(r1, r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_W(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	STX_W(r1, r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_stxi_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	ST_D(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	STX_D(r1, r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_bswapr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    REVB_2H(r0, r1);
    extr_us(r0, r0);
}

static void
_bswapr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    REVB_2W(r0, r1);
    extr_ui(r0, r0);
}

static void
_lti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	SLTI(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ltr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_lti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0))
	SLTUI(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ltr_u(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ler(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLT(r0, r2, r1);
    XORI(r0, r0, 1);
}

static void
_lei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0 + 1))
        SLTI(r0, r1, i0 + 1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ler(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ler_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLTU(r0, r2, r1);
    XORI(r0, r0, 1);
}

static void
_lei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0 + 1))
        SLTUI(r0, r1, i0 + 1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ler_u(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_eqr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    subr(r0, r1, r2);
    SLTU(r0, _ZERO_REGNO, r0);
    XORI(r0, r0, 1);
}

static void
_eqi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0) {
	subi(r0, r1, i0);
	SLTU(r0, _ZERO_REGNO, r0);
    }
    else
	SLTU(r0, _ZERO_REGNO, r1);
    XORI(r0, r0, 1);
}

static void
_ger(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLT(r0, r1, r2);
    XORI(r0, r0, 1);
}

static void
_gei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0)) {
        SLTI(r0, r1, i0);
        XORI(r0, r0, 1);
    } else {
        reg = jit_get_reg(jit_class_gpr);
        movi(rn(reg), i0);
        ger(r0, r1, rn(reg));
        jit_unget_reg(reg);
    }
}

static void
_ger_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLTU(r0, r1, r2);
    XORI(r0, r0, 1);
}

static void
_gei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_si12_p(i0)) {
        SLTUI(r0, r1, i0);
        XORI(r0, r0, 1);
    } else {
        reg = jit_get_reg(jit_class_gpr);
        movi(rn(reg), i0);
        ger_u(r0, r1, rn(reg));
        jit_unget_reg(reg);
    }
}

static void
_gti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	SLT(r0, _ZERO_REGNO, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	SLT(r0, rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_gti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	SLTU(r0, _ZERO_REGNO, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	SLTU(r0, rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_ner(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    subr(r0, r1, r2);
    SLTU(r0, _ZERO_REGNO, r0);
}

static void
_nei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0) {
	subi(r0, r1, i0);
	SLTU(r0, _ZERO_REGNO, r0);
    }
    else
	SLTU(r0, _ZERO_REGNO, r1);
}

static jit_word_t
_bltr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BLT(r0, r1, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_blti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BLT(r0, _ZERO_REGNO, (i0 - w) >> 2);
    }
    if (i1) {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = _jit->pc.w;
	BLT(r0, rn(reg), (i0 - w) >> 2);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bltr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BLTU(r0, r1, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_blti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0)
	w = bltr_u(i0, r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = _jit->pc.w;
	BLTU(r0, rn(reg), (i0 - w) >> 2);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bler(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLT(rn(reg), r1, r0);
    w = _jit->pc.w;
    BEQZ(rn(reg), (i0 - w) >> 2);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_blei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bler(i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bler_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLTU(rn(reg), r1, r0);
    w = _jit->pc.w;
    BEQZ(rn(reg), (i0 - w) >> 2);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_blei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0)
	w = bler_u(i0, r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = bler_u(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_beqr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BEQ(r0, r1, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_beqi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BEQZ(r0, (i0 - w) >> 2);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = beqr(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bger(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BGE(r0, r1, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_bgei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
   jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0)
	w = bger(i0, r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = bger(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bger_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BGEU(r0, r1, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_bgei_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
   jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	B((i0 - w) >> 2);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = bger_u(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bgtr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLT(rn(reg), r1, r0);
    w = _jit->pc.w;
    BNEZ(rn(reg), (i0 - w) >> 2);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bgtr(i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgtr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLTU(rn(reg), r1, r0);
    w = _jit->pc.w;
    BNEZ(rn(reg), (i0 - w) >> 2);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgti_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(reg), i1);
    w = bgtr_u(i0, r0, rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bner(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BNE(r0, r1, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_bnei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (i1 == 0) {
	w = _jit->pc.w;
	BNEZ(r0, (i0 - w) >> 2);
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = bner(i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d, w;
    w = _jit->pc.w;
    d = (i0 - w) >> 2;
    if (can_sign_extend_si26_p(i0))
	B(d);
    else
	w = jmpi_p(i0);
    return (w);
}

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

static jit_word_t
_boaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, b;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = r1 < 0; */
    SLT(rn(t0), r1, _ZERO_REGNO);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 + r1 */
    addr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNEZ(rn(t0), 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    b = _jit->pc.w;
    B(0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(b, _jit->pc.w);
    w = _jit->pc.w;
    BNEZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i1);
    w = boaddr(i0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    SLTU(rn(t1), rn(t0), r0);
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BNEZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i1);
    w = boaddr_u(i0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, b;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = r1 < 0; */
    SLT(rn(t0), r1, _ZERO_REGNO);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 + r1 */
    addr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNEZ(rn(t0), 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    b = _jit->pc.w;
    B(0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(b, _jit->pc.w);
    w = _jit->pc.w;
    BEQZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i1);
    w = bxaddr(i0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    SLTU(rn(t1), rn(t0), r0);
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BEQZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i1);
    w = bxaddr_u(i0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, b;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = 0 < r1; */
    SLT(rn(t0), _ZERO_REGNO, r1);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 - r1 */
    subr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNE(rn(t0), _ZERO_REGNO, 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    b = _jit->pc.w;
    B(0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(b, _jit->pc.w);
    w = _jit->pc.w;
    BNEZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i1);
    w = bosubr(i0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    SLTU(rn(t1), r0, rn(t0));
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BNEZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubi_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bosubr_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, b;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = r1 < 0; */
    SLT(rn(t0), _ZERO_REGNO, r1);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 - r1 */
    subr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNEZ(rn(t0), 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    b = _jit->pc.w;
    B(0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(b, _jit->pc.w);
    w = _jit->pc.w;
    BEQZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i1);
    w = bxsubr(i0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    SLTU(rn(t1), r0, rn(t0));
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BEQZ(rn(t1), (i0 - w) >> 2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i1);
    w = bxsubr_u(i0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmsr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    AND(rn(t0), r0, r1);
    w = _jit->pc.w;
    BNEZ(rn(t0), (i0 - w) >> 2);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmsi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    andi(rn(t0), r0, i1);
    w = _jit->pc.w;
    BNEZ(rn(t0), (i0 - w) >> 2);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmcr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    AND(rn(t0), r0, r1);
    w = _jit->pc.w;
    BEQZ(rn(t0), (i0 - w) >> 2);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmci(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    andi(rn(t0), r0, i1);
    w = _jit->pc.w;
    BEQZ(rn(t0), (i0 - w) >> 2);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d, w;
    w = _jit->pc.w;
    d = (i0 - w) >> 2;
    if (can_sign_extend_si26_p(i0))
	BL(d);
    else
	w = calli_p(i0);
    return (w);
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t	w;
    jit_word_t	reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    w = movi_p(rn(reg), i0);
    callr(rn(reg));
    jit_unget_reg(reg);
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
    _jitc->function->stack = ((_jitc->function->self.alen -
			      /* align stack at 16 bytes */
			      _jitc->function->self.aoff) + 15) & -16;

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
	subi(_SP_REGNO, _SP_REGNO, jit_framesize());
    if (_jitc->function->need_frame) {
	stxi(0, _SP_REGNO, _RA_REGNO);
	stxi(8, _SP_REGNO, _FP_REGNO);
    }
    /* callee save registers */
    for (reg = 0, offs = 16; reg < jit_size(iregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, iregs[reg])) {
	    stxi(offs, _SP_REGNO, rn(iregs[reg]));
	    offs += sizeof(jit_word_t);
	}
    }
    for (reg = 0; reg < jit_size(fregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, fregs[reg])) {
	    stxi_d(offs, _SP_REGNO, rn(fregs[reg]));
	    offs += sizeof(jit_float64_t);
	}
    }

    if (_jitc->function->need_frame)
	movr(_FP_REGNO, _SP_REGNO);
    if (_jitc->function->stack)
	subi(_SP_REGNO, _SP_REGNO, _jitc->function->stack);
    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
    }
    if (_jitc->function->self.call & jit_call_varargs) {
	for (reg = _jitc->function->vagp; jit_arg_reg_p(reg); ++reg)
	    stxi(jit_framesize() - ((8 - reg) * 8),
		 _FP_REGNO, rn(JIT_RA0 - reg));
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg, offs;
    if (_jitc->function->assume_frame)
	return;
    if (_jitc->function->need_frame) {
	movr(_SP_REGNO, _FP_REGNO);
	ldxi(_RA_REGNO, _SP_REGNO, 0);
	ldxi(_FP_REGNO, _SP_REGNO, 8);
    }

    /* callee save registers */
    for (reg = 0, offs = 16; reg < jit_size(iregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, iregs[reg])) {
	    ldxi(rn(iregs[reg]), _SP_REGNO, offs);
	    offs += sizeof(jit_word_t);
	}
    }
    for (reg = 0; reg < jit_size(fregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, fregs[reg])) {
	    ldxi_d(rn(fregs[reg]), _SP_REGNO, offs);
	    offs += sizeof(jit_float64_t);
	}
    }

    if (_jitc->function->need_frame || _jitc->function->need_stack)
	addi(_SP_REGNO, _SP_REGNO, jit_framesize());
    JIRL(_ZERO_REGNO, _RA_REGNO, 0);
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Initialize va_list to the first stack argument. */
    if (jit_arg_reg_p(_jitc->function->vagp))
	addi(r0, _FP_REGNO, jit_framesize() - ((8 - _jitc->function->vagp) * 8));
    else
	addi(r0, _FP_REGNO, jit_selfsize());
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Load argument. */
    ldr(r0, r1);
    /* Update va_list. */
    addi(r1, r1, sizeof(jit_word_t));
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    jit_uint32_t	 c;
    union {
	jit_uint32_t	*i;
	jit_word_t	 w;
    } u;
    u.w = instr;
    c = u.i[0];
    /* movi_p? */
    if ((c >> 25) == 10) {				/* LU12I_W */
	jit_int32_t	_00_11, _12_31, _32_51, _52_63;
	_00_11 =  label        &   0xfff;
	_12_31 = (label >> 12) & 0xfffff;
	_32_51 = (label >> 32) & 0xfffff;
	_52_63 = (label >> 52) &   0xfff;
	u.i[0] &= ~(0xfffff << 5);
	u.i[0] |= _12_31 << 5;
	assert((u.i[1] >> 22) == 14);			/* ORI */
	u.i[1] &= ~(0xfff << 10);
	u.i[1] |= _00_11 << 10;
	assert((u.i[2] >> 25) == 11);			/* LU32I_D */
	u.i[2] &= ~(0xfffff << 5);
	u.i[2] |= _32_51 << 5;
	assert((u.i[3] >> 22) == 12);			/* LU52I_D */
	u.i[3] &= ~(0xfff << 10);
	u.i[3] |= _52_63 << 0;
    }
    else if ((c >> 26) >= 22 && (c >> 26) <= 27) {
	/* B{EQ,NE,LT,GE,LTU,GEU} */
	jit_word_t	disp = (label - instr) >> 2;
	assert(can_sign_extend_si16_p(disp));
	u.i[0] &= ~(0xffff << 10);
	u.i[0] |= (disp & 0xffff) << 10;
    }
    else if ((c >> 26) == 20 || (c >> 26) == 21) {	/* B or BL */
	jit_word_t disp = (label - instr) >> 2;
	assert(can_sign_extend_si26_p(disp));
	disp &= 0x3ffffff;
	u.i[0] &= ~0x3ffffff;
	u.i[0] |= ((disp & 0xffff) << 10) | (disp >> 16);
    }
    else if ((c >> 26) >= 16 && (c >> 26) <= 18) {	/* B{,C}{EQ,NE}Z */
	jit_word_t disp = (label - instr) >> 2;
	assert(can_sign_extend_si21_p(disp));
	disp &= 0x1fffff;
	u.i[0] &= ~((0xffff << 10) | 0x1f);
	u.i[0] |= ((disp & 0xffff) << 10) | (disp >> 16);
    }
    else
	abort();
}
#endif
