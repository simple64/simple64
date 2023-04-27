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

#if PROTO
#  define BE_P				(__BYTE_ORDER == __BIG_ENDIAN)
#  define LE_P				(__BYTE_ORDER == __LITTLE_ENDIAN)
#  define MIPS_fmt_S			0x10		/* float32 */
#  define MIPS_fmt_D			0x11		/* float64 */
#  define MIPS_fmt_W			0x14		/* int32 */
#  define MIPS_fmt_L			0x15		/* int64 */
#  define MIPS_fmt_PS			0x16		/* 2 x float32 */
#  define MIPS_fmt_S_PU			0x20
#  define MIPS_fmt_S_PL			0x26
#  define MIPS_condn_S			0x14		/* release 6 */
#  define MIPS_condn_D			0x15		/* release 6 */
#  define MIPS_ADD_fmt			0x00
#  define MIPS_LWXC1			0x00
#  define MIPS_SUB_fmt			0x01
#  define MIPS_LDXC1			0x01
#  define MIPS_MUL_fmt			0x02
#  define MIPS_DIV_fmt			0x03
#  define MIPS_SQRT_fmt			0x04
#  define MIPS_ABS_fmt			0x05
#  define MIPS_LUXC1			0x05
#  define MIPS_MOV_fmt			0x06
#  define MIPS_NEG_fmt			0x07
#  define MIPS_SWXC1			0x08
#  define MIPS_ROUND_fmt_L		0x08
#  define MIPS_TRUNC_fmt_L		0x09
#  define MIPS_SDXC1			0x09
#  define MIPS_CEIL_fmt_L		0x0a
#  define MIPS_FLOOR_fmt_L		0x0b
#  define MIPS_ROUND_fmt_W		0x0c
#  define MIPS_TRUNC_fmt_W		0x0d
#  define MIPS_SUXC1			0x0d
#  define MIPS_CEIL_fmt_W		0x0e
#  define MIPS_FLOOR_fmt_W		0x0f
#  define MIPS_RECIP			0x15
#  define MIPS_RSQRT			0x16
#  define MIPS_MADDF			0x18
#  define MIPS_MSUBF			0x19
#  define MIPS_ALNV_PS			0x1e
#  define MIPS_CVT_fmt_S		0x20
#  define MIPS_CVT_fmt_D		0x21
#  define MIPS_CVT_fmt_W		0x24
#  define MIPS_CVT_fmt_L		0x25
#  define MIPS_PLL			0x2c
#  define MIPS_PLU			0x2d
#  define MIPS_PUL			0x2e
#  define MIPS_PUU			0x2f
#  define MIPS_MADD_fmt_S		0x20
#  define MIPS_MADD_fmt_D		0x21
#  define MIPS_MADD_fmt_PS		0x22
#  define MIPS_MSUB_fmt_S		0x28
#  define MIPS_MSUB_fmt_D		0x29
#  define MIPS_MSUB_fmt_PS		0x2a
#  define MIPS_NMADD_fmt_S		0x30
#  define MIPS_NMADD_fmt_D		0x31
#  define MIPS_NMADD_fmt_PS		0x32
#  define MIPS_NMSUB_fmt_S		0x38
#  define MIPS_NMSUB_fmt_D		0x39
#  define MIPS_NMSUB_fmt_PS		0x3a
#  define MIPS_cond_F			0x30
#  define MIPS_cond_UN			0x31
#  define MIPS_cond_EQ			0x32
#  define MIPS_cond_UEQ			0x33
#  define MIPS_cond_OLT			0x34
#  define MIPS_cond_ULT			0x35
#  define MIPS_cond_OLE			0x36
#  define MIPS_cond_ULE			0x37
#  define MIPS_cond_SF			0x38
#  define MIPS_cond_NGLE		0x39
#  define MIPS_cond_SEQ			0x3a
#  define MIPS_cond_NGL			0x3b
#  define MIPS_cond_LT			0x3c
#  define MIPS_cond_NGE			0x3d
#  define MIPS_cond_LE			0x3e
#  define MIPS_cond_UGT			0x3f
/* Mips release 6 */
#  define MIPS_cmp_AF			0x00
#  define MIPS_cmp_UN			0x01
#  define MIPS_cmp_EQ			0x02
#  define MIPS_cmp_UEQ			0x03
#  define MIPS_cmp_LT			0x04
#  define MIPS_cmp_ULT			0x05
#  define MIPS_cmp_LE			0x06
#  define MIPS_cmp_ULE			0x07
#  define MIPS_cmp_SAF			0x08
#  define MIPS_cmp_SUN			0x09
#  define MIPS_cmp_SEQ			0x0a
#  define MIPS_cmp_SUEQ			0x0b
#  define MIPS_cmp_SLT			0x0c
#  define MIPS_cmp_SULT			0x0d
#  define MIPS_cmp_SLE			0x0e
#  define MIPS_cmp_SULE			0x0f
#  define ADD_S(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_S,ft,fs,fd,MIPS_ADD_fmt)
#  define ADD_D(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_D,ft,fs,fd,MIPS_ADD_fmt)
#  define SUB_S(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_S,ft,fs,fd,MIPS_SUB_fmt)
#  define SUB_D(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_D,ft,fs,fd,MIPS_SUB_fmt)
#  define MUL_S(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_S,ft,fs,fd,MIPS_MUL_fmt)
#  define MUL_D(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_D,ft,fs,fd,MIPS_MUL_fmt)
#  define DIV_S(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_S,ft,fs,fd,MIPS_DIV_fmt)
#  define DIV_D(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_D,ft,fs,fd,MIPS_DIV_fmt)
#  define ABS_S(fd,fs)			hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_ABS_fmt)
#  define ABS_D(fd,fs)			hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_ABS_fmt)
#  define NEG_S(fd,fs)			hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_NEG_fmt)
#  define NEG_D(fd,fs)			hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_NEG_fmt)
#  define SQRT_S(fd,fs)			hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_SQRT_fmt)
#  define SQRT_D(fd,fs)			hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_SQRT_fmt)
#  define MFC1(rt, fs)			hrrrit(MIPS_COP1,MIPS_MF,rt,fs,0,0)
#  define MFHC1(rt, fs)			hrrrit(MIPS_COP1,MIPS_MFH,rt,fs,0,0)
#  define MTC1(rt, fs)			hrrrit(MIPS_COP1,MIPS_MT,rt,fs,0,0)
#  define MTHC1(rt, fs)			hrrrit(MIPS_COP1,MIPS_MTH,rt,fs,0,0)
#  define DMFC1(rt, fs)			hrrrit(MIPS_COP1,MIPS_DMF,rt,fs,0,0)
#  define DMTC1(rt, fs)			hrrrit(MIPS_COP1,MIPS_DMT,rt,fs,0,0)
#  define CVT_D_S(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_CVT_fmt_D)
#  define CVT_D_W(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_W,0,fs,fd,MIPS_CVT_fmt_D)
#  define CVT_D_L(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_L,0,fs,fd,MIPS_CVT_fmt_D)
#  define CVT_L_S(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_CVT_fmt_L)
#  define CVT_L_D(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_CVT_fmt_L)
#  define CVT_PS_S(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_CVT_fmt_PS)
#  define CVT_S_D(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_CVT_fmt_S)
#  define CVT_S_W(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_W,0,fs,fd,MIPS_CVT_fmt_S)
#  define CVT_S_L(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_L,0,fs,fd,MIPS_CVT_fmt_S)
#  define CVT_S_PL(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_PS,0,fs,fd,MIPS_CVT_fmt_S_PL)
#  define CVT_S_PU(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_PS,0,fs,fd,MIPS_CVT_fmt_S_PU)
#  define CVT_W_S(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_CVT_fmt_W)
#  define CVT_W_D(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_CVT_fmt_W)
#  define TRUNC_L_S(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_TRUNC_fmt_L)
#  define TRUNC_L_D(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_TRUNC_fmt_L)
#  define TRUNC_W_S(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_TRUNC_fmt_W)
#  define TRUNC_W_D(fd,fs)		hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_TRUNC_fmt_W)
#  define MADD_S(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_MADD_fmt_S)
#  define MADD_D(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_MADD_fmt_D)
#  define MSUB_S(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_MSUB_fmt_S)
#  define MSUB_D(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_MSUB_fmt_D)
#  define NMADD_S(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_NMADD_fmt_S)
#  define NMADD_D(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_NMADD_fmt_D)
#  define NMSUB_S(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_NMSUB_fmt_S)
#  define NMSUB_D(fd,fr,fs,ft)		hrrrit(MIPS_COP1X,fr,ft,fs,fd,MIPS_NMSUB_fmt_D)
/* fd = fd + (fs * ft) */
#  define MADDF_S(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_S,ft,fs,fd,MIPS_MADDF)
#  define MADDF_D(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_D,ft,fs,fd,MIPS_MADDF)
/* fd = fd - (fs * ft) */
#  define MSUBF_S(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_S,ft,fs,fd,MIPS_MSUBF)
#  define MSUBF_D(fd,fs,ft)		hrrrit(MIPS_COP1,MIPS_fmt_D,ft,fs,fd,MIPS_MSUBF)
#  define LWC1(rt, of, rb)		hrri(MIPS_LWC1, rb, rt, of)
#  define SWC1(rt, of, rb)		hrri(MIPS_SWC1, rb, rt, of)
#  define LDC1(rt, of, rb)		hrri(MIPS_LDC1, rb, rt, of)
#  define SDC1(rt, of, rb)		hrri(MIPS_SDC1, rb, rt, of)
#  define MOV_S(fd, fs)			hrrrit(MIPS_COP1,MIPS_fmt_S,0,fs,fd,MIPS_MOV_fmt)
#  define MOV_D(fd, fs)			hrrrit(MIPS_COP1,MIPS_fmt_D,0,fs,fd,MIPS_MOV_fmt)
#  define BC1F(im)			hrri(MIPS_COP1,MIPS_BC,MIPS_BCF,im)
#  define BC1EQZ(ft,im)			hrri(MIPS_COP1,MIPS_BC1EQZ,ft,im)
#  define BC1T(im)			hrri(MIPS_COP1,MIPS_BC,MIPS_BCT,im)
#  define BC1NEZ(ft,im)			hrri(MIPS_COP1,MIPS_BC1NEZ,ft,im)
#  define C_F_S(fs,ft)			c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_F)
#  define C_F_D(fs,ft)			c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_F)
#  define C_F_PS(fs,ft)			c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_F)
#  define C_UN_S(fs,ft)			c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_UN)
#  define C_UN_D(fs,ft)			c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_UN)
#  define C_UN_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_UN)
#  define C_EQ_S(fs,ft)			c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_EQ)
#  define C_EQ_D(fs,ft)			c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_EQ)
#  define C_EQ_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_EQ)
#  define C_UEQ_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_UEQ)
#  define C_UEQ_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_UEQ)
#  define C_UEQ_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_UEQ)
#  define C_OLT_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_OLT)
#  define C_OLT_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_OLT)
#  define C_OLT_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_OLT)
#  define C_ULT_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_ULT)
#  define C_ULT_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_ULT)
#  define C_ULT_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_ULT)
#  define C_OLE_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_OLE)
#  define C_OLE_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_OLE)
#  define C_OLE_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_OLE)
#  define C_ULE_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_ULE)
#  define C_ULE_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_ULE)
#  define C_ULE_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_ULE)
#  define C_SF_S(fs,ft)			c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_SF)
#  define C_SF_D(fs,ft)			c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_SF)
#  define C_SF_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_SF)
#  define C_NGLE_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_NGLE)
#  define C_NGLE_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_NGLE)
#  define C_NGLE_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_NGLE)
#  define C_SEQ_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_SEQ)
#  define C_SEQ_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_SEQ)
#  define C_SEQ_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_SEQ)
#  define C_NGL_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_NGL)
#  define C_NGL_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_NGL)
#  define C_NGL_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_NGL)
#  define C_NLT_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_NLT)
#  define C_NLT_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_NLT)
#  define C_NLT_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_NLT)
#  define C_NGE_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_NGE)
#  define C_NGE_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_NGE)
#  define C_NGE_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_NGE)
#  define C_NLE_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_NLE)
#  define C_NLE_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_NLE)
#  define C_NLE_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_NLE)
#  define C_UGT_S(fs,ft)		c_cond_fmt(MIPS_fmt_S,ft,fs,MIPS_cond_UGT)
#  define C_UGT_D(fs,ft)		c_cond_fmt(MIPS_fmt_D,ft,fs,MIPS_cond_UGT)
#  define C_UGT_PS(fs,ft)		c_cond_fmt(MIPS_fmt_PS,ft,fs,MIPS_cond_UGT)
#  define c_cond_fmt(fm,ft,fs,cc)	_c_cond_fmt(_jit,fm,ft,fs,cc)
static void
_c_cond_fmt(jit_state_t *_jit, jit_int32_t fm,
	    jit_int32_t ft, jit_int32_t fs, jit_int32_t cc);
#  define CMP_AF_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_AF)
#  define CMP_AF_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_AF)
#  define CMP_UN_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_UN)
#  define CMP_UN_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_UN)
#  define CMP_EQ_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_EQ)
#  define CMP_EQ_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_EQ)
#  define CMP_UEQ_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_UEQ)
#  define CMP_UEQ_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_UEQ)
#  define CMP_LT_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_LT)
#  define CMP_LT_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_LT)
#  define CMP_ULT_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_ULT)
#  define CMP_ULT_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_ULT)
#  define CMP_LE_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_LE)
#  define CMP_LE_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_LE)
#  define CMP_ULE_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_ULE)
#  define CMP_ULE_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_ULE)
#  define CMP_SAF_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SAF)
#  define CMP_SAF_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SAF)
#  define CMP_SUN_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SUN)
#  define CMP_SUN_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SUN)
#  define CMP_SEQ_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SEQ)
#  define CMP_SEQ_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SEQ)
#  define CMP_SUEQ_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SUEQ)
#  define CMP_SUEQ_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SUEQ)
#  define CMP_SLT_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SLT)
#  define CMP_SLT_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SLT)
#  define CMP_SULT_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SULT)
#  define CMP_SULT_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SULT)
#  define CMP_SLE_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SLE)
#  define CMP_SLE_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SLE)
#  define CMP_SULE_S(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_S,fd,ft,fs,MIPS_cmp_SULE)
#  define CMP_SULE_D(fd,fs,ft)		cmp_cond_fmt(MIPS_condn_D,fd,ft,fs,MIPS_cmp_SULE)
#  define cmp_cond_fmt(fm,fd,ft,fs,cn)	_cmp_cond_fmt(_jit,fm,fd,ft,fs,cn)
static void
_cmp_cond_fmt(jit_state_t *_jit, jit_int32_t fm, jit_int32_t fd,
	      jit_int32_t ft, jit_int32_t fs, jit_int32_t cn);
#  define addr_f(r0,r1,r2)		ADD_S(r0,r1,r2)
#  define addi_f(r0,r1,i0)		_addi_f(_jit,r0,r1,i0)
static void _addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define addr_d(r0,r1,r2)		ADD_D(r0,r1,r2)
#  define addi_d(r0,r1,i0)		_addi_d(_jit,r0,r1,i0)
static void _addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define subr_f(r0,r1,r2)		SUB_S(r0,r1,r2)
#  define subi_f(r0,r1,i0)		_subi_f(_jit,r0,r1,i0)
static void _subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define subr_d(r0,r1,r2)		SUB_D(r0,r1,r2)
#  define subi_d(r0,r1,i0)		_subi_d(_jit,r0,r1,i0)
static void _subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define rsbr_f(r0,r1,r2)		subr_f(r0,r2,r1)
#  define rsbi_f(r0,r1,i0)		_rsbi_f(_jit,r0,r1,i0)
static void _rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define rsbr_d(r0,r1,r2)		subr_d(r0,r2,r1)
#  define rsbi_d(r0,r1,i0)		_rsbi_d(_jit,r0,r1,i0)
static void _rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define mulr_f(r0,r1,r2)		MUL_S(r0,r1,r2)
#  define muli_f(r0,r1,i0)		_muli_f(_jit,r0,r1,i0)
static void _muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define mulr_d(r0,r1,r2)		MUL_D(r0,r1,r2)
#  define muli_d(r0,r1,i0)		_muli_d(_jit,r0,r1,i0)
static void _muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define divr_f(r0,r1,r2)		DIV_S(r0,r1,r2)
#  define divi_f(r0,r1,i0)		_divi_f(_jit,r0,r1,i0)
static void _divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define divr_d(r0,r1,r2)		DIV_D(r0,r1,r2)
#  define divi_d(r0,r1,i0)		_divi_d(_jit,r0,r1,i0)
static void _divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define absr_f(r0,r1)			ABS_S(r0,r1)
#  define absr_d(r0,r1)			ABS_D(r0,r1)
#  define negr_f(r0,r1)			NEG_S(r0,r1)
#  define negr_d(r0,r1)			NEG_D(r0,r1)
#  define sqrtr_f(r0,r1)		SQRT_S(r0,r1)
#  define sqrtr_d(r0,r1)		SQRT_D(r0,r1)
#  define movr_w_f(r0, r1)		MTC1(r1, r0)
#  define movr_f_w(r0, r1)		MFC1(r0, r1)
# define movi_w_f(r0, i0)		_movi_w_f(_jit, r0, i0)
static void _movi_w_f(jit_state_t*, jit_int32_t, jit_word_t);
#  define extr_f(r0, r1)		_extr_f(_jit, r0, r1)
static void _extr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define truncr_f_i(r0, r1)		_truncr_f_i(_jit, r0, r1)
static void _truncr_f_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  if __WORDSIZE == 64
#    define truncr_f_l(r0, r1)		_truncr_f_l(_jit, r0, r1)
static void _truncr_f_l(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#  define extr_d_f(r0, r1)		CVT_S_D(r0, r1)
#  define ldr_f(r0, r1)			LWC1(r0, 0, r1)
#  define ldi_f(r0, i0)			_ldi_f(_jit, r0, i0)
static void _ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_f(r0, r1, r2)		_ldxr_f(_jit, r0, r1, r2)
static void _ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_f(r0, r1, i0)		_ldxi_f(_jit, r0, r1, i0)
static void _ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define unldr_x(r0, r1, i0)		_unldr_x(_jit, r0, r1, i0)
static void _unldr_x(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define unldi_x(r0, i0, i1)		_unldi_x(_jit, r0, i0, i1)
static void _unldi_x(jit_state_t*, jit_int32_t, jit_word_t, jit_word_t);
#  define str_f(r0, r1)			SWC1(r1, 0, r0)
#  define sti_f(i0, r0)			_sti_f(_jit, i0, r0)
static void _sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_f(r0, r1, r2)		_stxr_f(_jit, r0, r1, r2)
static void _stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_f(i0, r0, r1)		_stxi_f(_jit, i0, r0, r1)
static void _stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define unstr_x(r0, r1, i0)		_unstr_x(_jit, r0, r1, i0)
static void _unstr_x(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define unsti_x(i0, r0, i1)		_unsti_x(_jit, i0, r0, i1)
static void _unsti_x(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define fmar_f(r0, r1, r2, r3)	_fmar_f(_jit, r0, r1, r2, r3)
static void _fmar_f(jit_state_t*,
		    jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define fmsr_f(r0, r1, r2, r3)	_fmsr_f(_jit, r0, r1, r2, r3)
static void _fmsr_f(jit_state_t*,
		    jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define fnmar_f(r0, r1, r2, r3)	_fnmar_f(_jit, r0, r1, r2, r3)
static void _fnmar_f(jit_state_t*,
		     jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define fnmsr_f(r0, r1, r2, r3)	_fnmsr_f(_jit, r0, r1, r2, r3)
static void _fnmsr_f(jit_state_t*,
		     jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define movr_f(r0, r1)		_movr_f(_jit, r0, r1)
static void _movr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_f(r0, i0)		_movi_f(_jit, r0, i0)
static void _movi_f(jit_state_t*,jit_int32_t,jit_float32_t*);
#  if NEW_ABI
#    if __WORDSIZE == 32
#      define movi64(r0, i0)		_movi64(_jit, r0, i0)
static void _movi64(jit_state_t*,jit_int32_t,jit_int64_t);
#    else
#      define movi64(r0, i0)		movi(r0, i0)
#    endif
#    define movr_w_d(r0, r1)		DMTC1(r1, r0)
#    define movr_d_w(r0, r1)		DMFC1(r0, r1)
#    define movi_w_d(r0, i0)		_movi_w_d(_jit, r0, i0)
static void _movi_w_d(jit_state_t*, jit_int32_t, jit_word_t);
#  else
#    define movr_ww_d(r0, r1, r2)	_movr_ww_d(_jit, r0, r1, r2)
static void _movr_ww_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define movr_d_ww(r0, r1, r2)	_movr_d_ww(_jit, r0, r1, r2)
static void _movr_d_ww(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define movi_ww_d(r0, i0, i1)	_movi_ww_d(_jit, r0, i0, i1)
static void _movi_ww_d(jit_state_t*, jit_int32_t, jit_word_t, jit_word_t);
#  endif
#  define extr_d(r0, r1)		_extr_d(_jit, r0, r1)
static void _extr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define truncr_d_i(r0, r1)		_truncr_d_i(_jit, r0, r1)
static void _truncr_d_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  if __WORDSIZE == 64
#    define truncr_d_l(r0, r1)		_truncr_d_l(_jit, r0, r1)
static void _truncr_d_l(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#  define ldr_d(r0, r1)			_ldr_d(_jit, r0, r1)
static void _ldr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_d(r0, i0)			_ldi_d(_jit, r0, i0)
static void _ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_d(r0, r1, r2)		_ldxr_d(_jit, r0, r1, r2)
static void _ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_d(r0, r1, i0)		_ldxi_d(_jit, r0, r1, i0)
static void _ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_d(r0, r1)			_str_d(_jit, r0, r1)
static void _str_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sti_d(i0, r0)			_sti_d(_jit, i0, r0)
static void _sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_d(r0, r1, r2)		_stxr_d(_jit, r0, r1, r2)
static void _stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_d(i0, r0, r1)		_stxi_d(_jit, i0, r0, r1)
static void _stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define fmar_d(r0, r1, r2, r3)	_fmar_d(_jit, r0, r1, r2, r3)
static void _fmar_d(jit_state_t*,
		    jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define fmsr_d(r0, r1, r2, r3)	_fmsr_d(_jit, r0, r1, r2, r3)
static void _fmsr_d(jit_state_t*,
		    jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define fnmar_d(r0, r1, r2, r3)	_fnmar_d(_jit, r0, r1, r2, r3)
static void _fnmar_d(jit_state_t*,
		     jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define fnmsr_d(r0, r1, r2, r3)	_fnmsr_d(_jit, r0, r1, r2, r3)
static void _fnmsr_d(jit_state_t*,
		     jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define movr_d(r0, r1)		_movr_d(_jit, r0, r1)
static void _movr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi_d(r0, i0)		_movi_d(_jit, r0, i0)
static void _movi_d(jit_state_t*,jit_int32_t,jit_float64_t*);
#  define ltr_f(r0, r1, r2)		_ltr_f(_jit, r0, r1, r2)
static void _ltr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lti_f(r0, r1, i2)		_lti_f(_jit, r0, r1, i2)
static void _lti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ler_f(r0, r1, r2)		_ler_f(_jit, r0, r1, r2)
static void _ler_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lei_f(r0, r1, i2)		_lei_f(_jit, r0, r1, i2)
static void _lei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define eqr_f(r0, r1, r2)		_eqr_f(_jit, r0, r1, r2)
static void _eqr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define eqi_f(r0, r1, i2)		_eqi_f(_jit, r0, r1, i2)
static void _eqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ger_f(r0, r1, r2)		_ger_f(_jit, r0, r1, r2)
static void _ger_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gei_f(r0, r1, i2)		_gei_f(_jit, r0, r1, i2)
static void _gei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define gtr_f(r0, r1, r2)		_gtr_f(_jit, r0, r1, r2)
static void _gtr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gti_f(r0, r1, i2)		_gti_f(_jit, r0, r1, i2)
static void _gti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ner_f(r0, r1, r2)		_ner_f(_jit, r0, r1, r2)
static void _ner_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nei_f(r0, r1, i2)		_nei_f(_jit, r0, r1, i2)
static void _nei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define unltr_f(r0, r1, r2)		_unltr_f(_jit, r0, r1, r2)
static void _unltr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unlti_f(r0, r1, i2)		_unlti_f(_jit, r0, r1, i2)
static void _unlti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define unler_f(r0, r1, r2)		_unler_f(_jit, r0, r1, r2)
static void _unler_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unlei_f(r0, r1, i2)		_unlei_f(_jit, r0, r1, i2)
static void _unlei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define uneqr_f(r0, r1, r2)		_uneqr_f(_jit, r0, r1, r2)
static void _uneqr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define uneqi_f(r0, r1, i2)		_uneqi_f(_jit, r0, r1, i2)
static void _uneqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define unger_f(r0, r1, r2)		_unger_f(_jit, r0, r1, r2)
static void _unger_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ungei_f(r0, r1, i2)		_ungei_f(_jit, r0, r1, i2)
static void _ungei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ungtr_f(r0, r1, r2)		_ungtr_f(_jit, r0, r1, r2)
static void _ungtr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ungti_f(r0, r1, i2)		_ungti_f(_jit, r0, r1, i2)
static void _ungti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ltgtr_f(r0, r1, r2)		_ltgtr_f(_jit, r0, r1, r2)
static void _ltgtr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ltgti_f(r0, r1, i2)		_ltgti_f(_jit, r0, r1, i2)
static void _ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define ordr_f(r0, r1, r2)		_ordr_f(_jit, r0, r1, r2)
static void _ordr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ordi_f(r0, r1, i2)		_ordi_f(_jit, r0, r1, i2)
static void _ordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define unordr_f(r0, r1, r2)		_unordr_f(_jit, r0, r1, r2)
static void _unordr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unordi_f(r0, r1, i2)		_unordi_f(_jit, r0, r1, i2)
static void _unordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t*);
#  define bltr_f(i0, r0, r1)		_bltr_f(_jit, i0, r0, r1)
static jit_word_t _bltr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti_f(i0, r0, i1)		_blti_f(_jit, i0, r0, i1)
static jit_word_t
_blti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bler_f(i0, r0, r1)		_bler_f(_jit, i0, r0, r1)
static jit_word_t _bler_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei_f(i0, r0, i1)		_blei_f(_jit, i0, r0, i1)
static jit_word_t
_blei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define beqr_f(i0, r0, r1)		_beqr_f(_jit, i0, r0, r1)
static jit_word_t _beqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define beqi_f(i0, r0, i1)		_beqi_f(_jit, i0, r0, i1)
static jit_word_t
_beqi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bger_f(i0, r0, r1)		_bger_f(_jit, i0, r0, r1)
static jit_word_t _bger_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei_f(i0, r0, i1)		_bgei_f(_jit, i0, r0, i1)
static jit_word_t
_bgei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bgtr_f(i0, r0, r1)		_bgtr_f(_jit, i0, r0, r1)
static jit_word_t _bgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti_f(i0, r0, i1)		_bgti_f(_jit, i0, r0, i1)
static jit_word_t
_bgti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bner_f(i0, r0, r1)		_bner_f(_jit, i0, r0, r1)
static jit_word_t _bner_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bnei_f(i0, r0, i1)		_bnei_f(_jit, i0, r0, i1)
static jit_word_t
_bnei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bunltr_f(i0, r0, r1)		_bunltr_f(_jit, i0, r0, r1)
static jit_word_t _bunltr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlti_f(i0, r0, i1)		_bunlti_f(_jit, i0, r0, i1)
static jit_word_t
_bunlti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bunler_f(i0, r0, r1)		_bunler_f(_jit, i0, r0, r1)
static jit_word_t _bunler_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlei_f(i0, r0, i1)		_bunlei_f(_jit, i0, r0, i1)
static jit_word_t
_bunlei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define buneqr_f(i0, r0, r1)		_buneqr_f(_jit, i0, r0, r1)
static jit_word_t _buneqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi_f(i0, r0, i1)		_buneqi_f(_jit, i0, r0, i1)
static jit_word_t
_buneqi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bunger_f(i0, r0, r1)		_bunger_f(_jit, i0, r0, r1)
static jit_word_t _bunger_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungei_f(i0, r0, i1)		_bungei_f(_jit, i0, r0, i1)
static jit_word_t
_bungei_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bungtr_f(i0, r0, r1)		_bungtr_f(_jit, i0, r0, r1)
static jit_word_t _bungtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungti_f(i0, r0, i1)		_bungti_f(_jit, i0, r0, i1)
static jit_word_t
_bungti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bltgtr_f(i0, r0, r1)		_bltgtr_f(_jit, i0, r0, r1)
static jit_word_t _bltgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti_f(i0, r0, i1)		_bltgti_f(_jit, i0, r0, i1)
static jit_word_t
_bltgti_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bordr_f(i0, r0, r1)		_bordr_f(_jit, i0, r0, r1)
static jit_word_t _bordr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bordi_f(i0, r0, i1)		_bordi_f(_jit, i0, r0, i1)
static jit_word_t
_bordi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define bunordr_f(i0, r0, r1)		_bunordr_f(_jit, i0, r0, r1)
static jit_word_t _bunordr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunordi_f(i0, r0, i1)		_bunordi_f(_jit, i0, r0, i1)
static jit_word_t
_bunordi_f(jit_state_t*, jit_word_t, jit_int32_t, jit_float32_t*);
#  define extr_f_d(r0, r1)		CVT_D_S(r0, r1)
#  define ltr_d(r0, r1, r2)		_ltr_d(_jit, r0, r1, r2)
static void _ltr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lti_d(r0, r1, i2)		_lti_d(_jit, r0, r1, i2)
static void _lti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ler_d(r0, r1, r2)		_ler_d(_jit, r0, r1, r2)
static void _ler_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lei_d(r0, r1, i2)		_lei_d(_jit, r0, r1, i2)
static void _lei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define eqr_d(r0, r1, r2)		_eqr_d(_jit, r0, r1, r2)
static void _eqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define eqi_d(r0, r1, i2)		_eqi_d(_jit, r0, r1, i2)
static void _eqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ger_d(r0, r1, r2)		_ger_d(_jit, r0, r1, r2)
static void _ger_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gei_d(r0, r1, i2)		_gei_d(_jit, r0, r1, i2)
static void _gei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define gtr_d(r0, r1, r2)		_gtr_d(_jit, r0, r1, r2)
static void _gtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gti_d(r0, r1, i2)		_gti_d(_jit, r0, r1, i2)
static void _gti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ner_d(r0, r1, r2)		_ner_d(_jit, r0, r1, r2)
static void _ner_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nei_d(r0, r1, i2)		_nei_d(_jit, r0, r1, i2)
static void _nei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unltr_d(r0, r1, r2)		_unltr_d(_jit, r0, r1, r2)
static void _unltr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unlti_d(r0, r1, i2)		_unlti_d(_jit, r0, r1, i2)
static void _unlti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unler_d(r0, r1, r2)		_unler_d(_jit, r0, r1, r2)
static void _unler_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unlei_d(r0, r1, i2)		_unlei_d(_jit, r0, r1, i2)
static void _unlei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define uneqr_d(r0, r1, r2)		_uneqr_d(_jit, r0, r1, r2)
static void _uneqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define uneqi_d(r0, r1, i2)		_uneqi_d(_jit, r0, r1, i2)
static void _uneqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unger_d(r0, r1, r2)		_unger_d(_jit, r0, r1, r2)
static void _unger_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ungei_d(r0, r1, i2)		_ungei_d(_jit, r0, r1, i2)
static void _ungei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ungtr_d(r0, r1, r2)		_ungtr_d(_jit, r0, r1, r2)
static void _ungtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ungti_d(r0, r1, i2)		_ungti_d(_jit, r0, r1, i2)
static void _ungti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ltgtr_d(r0, r1, r2)		_ltgtr_d(_jit, r0, r1, r2)
static void _ltgtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ltgti_d(r0, r1, i2)		_ltgti_d(_jit, r0, r1, i2)
static void _ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define ordr_d(r0, r1, r2)		_ordr_d(_jit, r0, r1, r2)
static void _ordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ordi_d(r0, r1, i2)		_ordi_d(_jit, r0, r1, i2)
static void _ordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define unordr_d(r0, r1, r2)		_unordr_d(_jit, r0, r1, r2)
static void _unordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define unordi_d(r0, r1, i2)		_unordi_d(_jit, r0, r1, i2)
static void _unordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t*);
#  define bltr_d(i0, r0, r1)		_bltr_d(_jit, i0, r0, r1)
static jit_word_t _bltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti_d(i0, r0, i1)		_blti_d(_jit, i0, r0, i1)
static jit_word_t
_blti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bler_d(i0, r0, r1)		_bler_d(_jit, i0, r0, r1)
static jit_word_t _bler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei_d(i0, r0, i1)		_blei_d(_jit, i0, r0, i1)
static jit_word_t
_blei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define beqr_d(i0, r0, r1)		_beqr_d(_jit, i0, r0, r1)
static jit_word_t _beqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define beqi_d(i0, r0, i1)		_beqi_d(_jit, i0, r0, i1)
static jit_word_t
_beqi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bger_d(i0, r0, r1)		_bger_d(_jit, i0, r0, r1)
static jit_word_t _bger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei_d(i0, r0, i1)		_bgei_d(_jit, i0, r0, i1)
static jit_word_t
_bgei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bgtr_d(i0, r0, r1)		_bgtr_d(_jit, i0, r0, r1)
static jit_word_t _bgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti_d(i0, r0, i1)		_bgti_d(_jit, i0, r0, i1)
static jit_word_t
_bgti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bner_d(i0, r0, r1)		_bner_d(_jit, i0, r0, r1)
static jit_word_t _bner_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bnei_d(i0, r0, i1)		_bnei_d(_jit, i0, r0, i1)
static jit_word_t
_bnei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bunltr_d(i0, r0, r1)		_bunltr_d(_jit, i0, r0, r1)
static jit_word_t _bunltr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlti_d(i0, r0, i1)		_bunlti_d(_jit, i0, r0, i1)
static jit_word_t
_bunlti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bunler_d(i0, r0, r1)		_bunler_d(_jit, i0, r0, r1)
static jit_word_t _bunler_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunlei_d(i0, r0, i1)		_bunlei_d(_jit, i0, r0, i1)
static jit_word_t
_bunlei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define buneqr_d(i0, r0, r1)		_buneqr_d(_jit, i0, r0, r1)
static jit_word_t _buneqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define buneqi_d(i0, r0, i1)		_buneqi_d(_jit, i0, r0, i1)
static jit_word_t
_buneqi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bunger_d(i0, r0, r1)		_bunger_d(_jit, i0, r0, r1)
static jit_word_t _bunger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungei_d(i0, r0, i1)		_bungei_d(_jit, i0, r0, i1)
static jit_word_t
_bungei_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bungtr_d(i0, r0, r1)		_bungtr_d(_jit, i0, r0, r1)
static jit_word_t _bungtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bungti_d(i0, r0, i1)		_bungti_d(_jit, i0, r0, i1)
static jit_word_t
_bungti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bltgtr_d(i0, r0, r1)		_bltgtr_d(_jit, i0, r0, r1)
static jit_word_t _bltgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bltgti_d(i0, r0, i1)		_bltgti_d(_jit, i0, r0, i1)
static jit_word_t
_bltgti_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bordr_d(i0, r0, r1)		_bordr_d(_jit, i0, r0, r1)
static jit_word_t _bordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bordi_d(i0, r0, i1)		_bordi_d(_jit, i0, r0, i1)
static jit_word_t
_bordi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define bunordr_d(i0, r0, r1)		_bunordr_d(_jit, i0, r0, r1)
static jit_word_t _bunordr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bunordi_d(i0, r0, i1)		_bunordi_d(_jit, i0, r0, i1)
static jit_word_t
_bunordi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_float64_t*);
#  define vaarg_d(r0, r1)		_vaarg_d(_jit, r0, r1)
static void _vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
static void
_c_cond_fmt(jit_state_t *_jit, jit_int32_t fm,
	    jit_int32_t ft, jit_int32_t fs, jit_int32_t cc)
{
    jit_instr_t		i;
    i.cc.b = cc;
    i.fs.b = fs;
    i.ft.b = ft;
    i.fm.b = fm;
    i.hc.b = MIPS_COP1;
    instr(i.op);
}

static void
_cmp_cond_fmt(jit_state_t *_jit, jit_int32_t fm, jit_int32_t fd,
	      jit_int32_t ft, jit_int32_t fs, jit_int32_t cn)
{
    jit_instr_t		i;
    i.op = 0;		/* must have bit 6 zero ed */
    i.cn.b = cn;
    i.ft.b = ft;
    i.fs.b = fs;
    i.fd.b = fd;
    i.fm.b = fm;
    i.hc.b = MIPS_COP1;
    instr(i.op);
}

#  define fpr_opi(name, type, size)					\
static void								\
_##name##i_##type(jit_state_t *_jit,					\
		  jit_int32_t r0, jit_int32_t r1,			\
		  jit_float##size##_t *i0)				\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    movi_##type(rn(reg), i0);						\
    name##r_##type(r0, r1, rn(reg));					\
    jit_unget_reg(reg);							\
}
#  define fpr_bopi(name, type, size)					\
static jit_word_t							\
_b##name##i_##type(jit_state_t *_jit,					\
		  jit_word_t i0, jit_int32_t r0,			\
		  jit_float##size##_t *i1)				\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    movi_##type(rn(reg), i1);						\
    word = b##name##r_##type(i0, r0, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define fopi(name)			fpr_opi(name, f, 32)
#  define fbopi(name)			fpr_bopi(name, f, 32)
#  define dopi(name)			fpr_opi(name, d, 64)
#  define dbopi(name)			fpr_bopi(name, d, 64)

fopi(add)
fopi(sub)
fopi(rsb)
fopi(mul)
fopi(div)

static void
_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_fpr);
#  if __WORDSIZE == 32
    MTC1(r1, rn(t0));
    CVT_S_W(r0, rn(t0));
#  else
    DMTC1(r1, rn(t0));
    CVT_S_L(r0, rn(t0));
#  endif
    jit_unget_reg(t0);
}

static void
_truncr_f_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_fpr);
    TRUNC_W_S(rn(t0), r1);
    MFC1(r0, rn(t0));
    jit_unget_reg(t0);
}

#  if __WORDSIZE == 64
static void
_truncr_f_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_fpr);
    TRUNC_L_S(rn(t0), r1);
    DMFC1(r0, rn(t0));
    jit_unget_reg(t0);
}
#  endif

static void
_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	LWC1(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_f(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	LWC1(r0, i0, r1);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_f(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_unldr_x(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0, r2;
#  if __WORDSIZE == 32
    jit_int32_t		t1, r3;
#  endif
    if (jit_unaligned_p()) {
	assert(i0 == 4 || i0 == 8);
	t0 = jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
	if (i0 == 4) {
	    unldr(r2, r1, 4);
	    movr_w_f(r0, r2);
	}
	else {
#  if __WORDSIZE == 32
	    t1 = jit_get_reg(jit_class_gpr);
	    r3 = rn(t1);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    unldr(r2, r1, 4);
	    addi(r3, r1, 4);
	    unldr(r3, r3, 4);
#    else
	    unldr(r3, r1, 4);
	    addi(r2, r1, 4);
	    unldr(r2, r2, 4);
#    endif
	    movr_ww_d(r0, r2, r3);
	    jit_unget_reg(t1);
#  else
	    unldr(r2, r1, 8);
	    movr_w_d(r0, r2);
#  endif
	}
	jit_unget_reg(t0);
    }
    else
	generic_unldr_x(r0, r1, i0);
}

static void
_unldi_x(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    jit_int32_t		t0, r2;
#  if __WORDSIZE == 32
    jit_int32_t		t1, r3;
#  endif
    if (jit_unaligned_p()) {
	assert(i1 == 4 || i1 == 8);
	t0 = jit_get_reg(jit_class_gpr);
	r2 = rn(t0);
	if (i1 == 4) {
	    unldi(r2, i0, 4);
	    movr_w_f(r0, r2);
	}
	else {
#  if __WORDSIZE == 32
	    t1 = jit_get_reg(jit_class_gpr);
	    r3 = rn(t1);
#    if __BYTE_ORDER == __LITTLE_ENDIAN
	    unldi(r2, i0, 4);
	    unldi(r3, i0 + 4, 4);
#    else
	    unldi(r3, i0, 4);
	    unldi(r2, i0 + 4, 4);
#    endif
	    movr_ww_d(r0, r3, r2);
	    jit_unget_reg(t1);
#  else
	    unldi(r2, i0, 8);
	    movr_w_d(r0, r2);
#  endif
	}
	jit_unget_reg(t0);
    }
    else
	generic_unldi_x(r0, i0, i1);
}

static void
_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SWC1(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_f(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_f(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SWC1(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_f(rn(reg), r1);
	jit_unget_reg(reg);
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
_fmar_f(jit_state_t *_jit,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MADDF_S(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_f(rn(t0), r3);
		MADDF_S(rn(t0), r2, r1);
		movr_f(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	}
	else
	    MADD_S(r0, r3, r2, r1);
    }
    else {
	if (r0 != r3) {
	    mulr_f(r0, r1, r2);
	    addr_f(r0, r0, r3);
	}
	else {
	    t0 = jit_get_reg(jit_class_fpr);
	    mulr_f(rn(t0), r1, r2);
	    addr_f(r0, rn(t0), r3);
	    jit_unget_reg(t0);
	}
    }
}

static void
_fmsr_f(jit_state_t *_jit,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MSUBF_S(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_f(rn(t0), r3);
		MSUBF_S(rn(t0), r2, r1);
		movr_f(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	    negr_f(r0, r0);
	}
	else
	    MSUB_S(r0, r3, r2, r1);
    }
    else {
	if (r0 != r3) {
	    mulr_f(r0, r1, r2);
	    subr_f(r0, r0, r3);
	}
	else {
	    t0 = jit_get_reg(jit_class_fpr);
	    mulr_f(rn(t0), r1, r2);
	    subr_f(r0, rn(t0), r3);
	    jit_unget_reg(t0);
	}
    }
}

static void
_fnmar_f(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MADDF_S(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_f(rn(t0), r3);
		MADDF_S(rn(t0), r2, r1);
		movr_f(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	    negr_f(r0, r0);
	}
	else
	    NMADD_S(r0, r3, r2, r1);
    }
    else {
	t0 = jit_get_reg(jit_class_fpr);
	negr_f(rn(t0), r1);
	mulr_f(rn(t0), rn(t0), r2);
	subr_f(r0, rn(t0), r3);
	jit_unget_reg(t0);
    }
}

static void
_fnmsr_f(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MSUBF_S(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_f(rn(t0), r3);
		MSUBF_S(rn(t0), r2, r1);
		movr_f(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	}
	else
	    NMSUB_S(r0, r3, r2, r1);
    }
    else {
	t0 = jit_get_reg(jit_class_fpr);
	negr_f(rn(t0), r1);
	mulr_f(rn(t0), rn(t0), r2);
	addr_f(r0, rn(t0), r3);
	jit_unget_reg(t0);
    }
}

static void
_movr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	MOV_S(r0, r1);
}

static void
_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t *i0)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } data;
    jit_int32_t		reg;

    data.f = *i0;
    if (data.i) {
	if (_jitc->no_data) {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), data.i);
	    MTC1(rn(reg), r0);
	    jit_unget_reg(reg);
	}
	else
	    ldi_f(r0, (jit_word_t)i0);
    }
    else
	MTC1(_ZERO_REGNO, r0);
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

dopi(add)
dopi(sub)
dopi(rsb)
dopi(mul)
dopi(div)

#if NEW_ABI
/* n32 abi requires 64 bit cpu */
static void
_movi64(jit_state_t *_jit, jit_int32_t r0, jit_int64_t i0)
{
    if (i0 == 0)
	OR(r0, _ZERO_REGNO, _ZERO_REGNO);
    else if (i0 >= -32678 && i0 <= 32767)
	DADDIU(r0, _ZERO_REGNO, i0);
    else if (i0 >= 0 && i0 <= 65535)
	ORI(r0, _ZERO_REGNO, i0);
    else {
	if (i0 >= 0 && i0 <= 0x7fffffffLL)
	    LUI(r0, i0 >> 16);
	else if (i0 >= 0 && i0 <= 0xffffffffLL) {
	    if (i0 & 0xffff0000LL) {
		ORI(r0, _ZERO_REGNO, (jit_word_t)(i0 >> 16));
		DSLL(r0, r0, 16);
	    }
	}
	else {
	    movi(r0, (jit_word_t)(i0 >> 32));
	    if (i0 & 0xffff0000LL) {
		DSLL(r0, r0, 16);
		ORI(r0, r0, (jit_word_t)(i0 >> 16));
		DSLL(r0, r0, 16);
	    }
	    else
		DSLL32(r0, r0, 0);
	}
	if ((jit_word_t)i0 & 0xffff)
	    ORI(r0, r0, (jit_word_t)i0 & 0xffff);
    }
}

static void
_movi_w_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi64(rn(reg), i0);
    movr_w_d(r0, rn(reg));
    jit_unget_reg(reg);
}

#else
static void
_movr_ww_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips6_p()) {
	MTC1(r1, r0);
	MTHC1(r2, r0);
    }
    else {
	MTC1(r1, r0 + BE_P);
	MTC1(r2, r0 + LE_P);
    }
}

static void
_movr_d_ww(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips6_p()) {
	MFC1(r0, r2);
	MFHC1(r1, r2);
    }
    else {
	MFC1(r0, r2 + BE_P);
	MFC1(r1, r2 + LE_P);
    }
}

static void
_movi_ww_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    movi(rn(t1), i1);
    MTC1(rn(t0), r0);
    MTC1(rn(t1), r0 + 1);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}
#endif

static void
_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_fpr);
#  if __WORDSIZE == 32
    MTC1(r1, rn(t0));
    CVT_D_W(r0, rn(t0));
#  else
    DMTC1(r1, rn(t0));
    CVT_D_L(r0, rn(t0));
#  endif
    jit_unget_reg(t0);
}

static void
_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_fpr);
    TRUNC_W_D(rn(t0), r1);
    MFC1(r0, rn(t0));
    jit_unget_reg(t0);
}

#  if __WORDSIZE == 64
static void
_truncr_d_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_fpr);
    TRUNC_L_D(rn(t0), r1);
    DMFC1(r0, rn(t0));
    jit_unget_reg(t0);
}
#  endif

static void
_ldr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips6_p() || __WORDSIZE == 64 || NEW_ABI)
	LDC1(r0, 0, r1);
    else {
	LWC1(r0 + BE_P, 0, r1);
	LWC1(r0 + LE_P, 4, r1);
    }
}

static void
_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_mips6_p() || __WORDSIZE == 64 || NEW_ABI) {
	if (can_sign_extend_short_p(i0))
	    LDC1(r0, i0, _ZERO_REGNO);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    LDC1(r0, 0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (can_sign_extend_short_p(i0) && can_sign_extend_short_p(i0 + 4)) {
	    LWC1(r0 + BE_P, i0, _ZERO_REGNO);
	    LWC1(r0 + LE_P, i0 + 4, _ZERO_REGNO);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    ldr_d(r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    ldr_d(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_mips6_p() || __WORDSIZE == 64 || NEW_ABI) {
	if (can_sign_extend_short_p(i0))
	    LDC1(r0, i0, r1);
	else
	    goto fallback;
    }
    else {
	if (can_sign_extend_short_p(i0) && can_sign_extend_short_p(i0 + 4)) {
	    LWC1(r0 + BE_P, i0, r1);
	    LWC1(r0 + LE_P, i0 + 4, r1);
	}
	else {
	fallback:
	    reg = jit_get_reg(jit_class_gpr);
	    addi(rn(reg), r1, i0);
	    ldr_d(r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_str_d(jit_state_t *_jit,jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips6_p() || __WORDSIZE == 64 || NEW_ABI)
	SDC1(r1, 0, r0);
    else {
	SWC1(r1 + BE_P, 0, r0);
	SWC1(r1 + LE_P, 4, r0);
    }
}

static void
_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (jit_mips6_p() ||  __WORDSIZE == 64 || NEW_ABI) {
	if (can_sign_extend_short_p(i0))
	    SDC1(r0, i0, _ZERO_REGNO);
	else
	    goto fallback;
    }
    else {
	if (can_sign_extend_short_p(i0) && can_sign_extend_short_p(i0 + 4)) {
	    SWC1(r0 + BE_P, i0, _ZERO_REGNO);
	    SWC1(r0 + LE_P, i0 + 4, _ZERO_REGNO);
	}
	else {
	fallback:
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    str_d(rn(reg), r0);
	    jit_unget_reg(reg);
	}
    }
}

static void
_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_d(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_mips6_p() || __WORDSIZE == 64 || NEW_ABI) {
	if (can_sign_extend_short_p(i0))
	    SDC1(r1, i0, r0);
	else
	    goto fallback;
    }
    else {
	if (can_sign_extend_short_p(i0) && can_sign_extend_short_p(i0 + 4)) {
	    SWC1(r1 + BE_P, i0, r0);
	    SWC1(r1 + LE_P, i0 + 4, r0);
	}
	else {
	fallback:
	    reg = jit_get_reg(jit_class_gpr);
	    addi(rn(reg), r0, i0);
	    str_d(rn(reg), r1);
	    jit_unget_reg(reg);
	}
    }
}

static void
_fmar_d(jit_state_t *_jit,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MADDF_D(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_d(rn(t0), r3);
		MADDF_D(rn(t0), r2, r1);
		movr_d(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	}
	else
	    MADD_D(r0, r3, r2, r1);
    }
    else {
	if (r0 != r3) {
	    mulr_d(r0, r1, r2);
	    addr_d(r0, r0, r3);
	}
	else {
	    t0 = jit_get_reg(jit_class_fpr);
	    mulr_d(rn(t0), r1, r2);
	    addr_d(r0, rn(t0), r3);
	    jit_unget_reg(t0);
	}
    }
}

static void
_fmsr_d(jit_state_t *_jit,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MSUBF_D(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_d(rn(t0), r3);
		MSUBF_D(rn(t0), r2, r1);
		movr_d(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	    negr_d(r0, r0);
	}
	else
	    MSUB_D(r0, r3, r2, r1);
    }
    else {
	if (r0 != r3) {
	    mulr_d(r0, r1, r2);
	    subr_d(r0, r0, r3);
	}
	else {
	    t0 = jit_get_reg(jit_class_fpr);
	    mulr_d(rn(t0), r1, r2);
	    subr_d(r0, rn(t0), r3);
	    jit_unget_reg(t0);
	}
    }
}

static void
_fnmar_d(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MADDF_D(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_d(rn(t0), r3);
		MADDF_D(rn(t0), r2, r1);
		movr_d(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	    negr_d(r0, r0);
	}
	else
	    NMADD_D(r0, r3, r2, r1);
    }
    else {
	t0 = jit_get_reg(jit_class_fpr);
	negr_d(rn(t0), r1);
	mulr_d(rn(t0), rn(t0), r2);
	subr_d(r0, rn(t0), r3);
	jit_unget_reg(t0);
    }
}

static void
_fnmsr_d(jit_state_t *_jit,
	 jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (jit_mips2_p()) {
	if (jit_mips6_p()) {
	    if (r0 == r3)
		MSUBF_D(r0, r2, r1);
	    else {
		t0 = jit_get_reg(jit_class_fpr);
		movr_d(rn(t0), r3);
		MSUBF_D(rn(t0), r2, r1);
		movr_d(r0, rn(t0));
		jit_unget_reg(t0);
	    }
	}
	else
	    NMSUB_D(r0, r3, r2, r1);
    }
    else {
	t0 = jit_get_reg(jit_class_fpr);
	negr_d(rn(t0), r1);
	mulr_d(rn(t0), rn(t0), r2);
	addr_d(r0, rn(t0), r3);
	jit_unget_reg(t0);
    }
}

static void
_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	MOV_D(r0, r1);
}

static void
_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t *i0)
{
    union {
	jit_int32_t	i[2];
	jit_int64_t	l;
	jit_float64_t	d;
    } data;
    jit_int32_t		reg;

    data.d = *i0;
#  if __WORDSIZE == 64 || NEW_ABI
    if (data.l) {
	if (_jitc->no_data) {
	    reg = jit_get_reg(jit_class_gpr);
	    movi64(rn(reg), data.l);
	    DMTC1(rn(reg), r0);
	    jit_unget_reg(reg);
	}
	else
	    ldi_d(r0, (jit_word_t)i0);
    }
    else
	DMTC1(_ZERO_REGNO, r0);
#  else
    if (jit_mips6_p()) {
	if (_jitc->no_data) {
	    reg = jit_get_reg(jit_class_gpr);
#  if __WORDSIZE == 64
	    movi(rn(reg), data.l);
	    DMTC1(rn(reg), r0);
#  else
	    movi(rn(reg), data.i[0 + BE_P]);
	    MTC1(rn(reg), r0);
	    movi(rn(reg), data.i[0 + LE_P]);
	    MTHC1(rn(reg), r0);
#  endif
	    jit_unget_reg(reg);
	}
	else
	    ldi_d(r0, (jit_word_t)i0);
    }
    else {
	if (_jitc->no_data)
	    reg = jit_get_reg(jit_class_gpr);
	if (data.i[0]) {
	    if (_jitc->no_data) {
		movi(rn(reg), data.i[0]);
		MTC1(rn(reg), r0 + BE_P);
	    }
	    else
		ldi_f(r0 + BE_P, (jit_word_t)i0);
	}
	else
	    MTC1(_ZERO_REGNO, r0 + BE_P);
	if (data.i[1]) {
	    if (_jitc->no_data) {
		movi(rn(reg), data.i[1]);
		MTC1(rn(reg), r0 + LE_P);
	    }
	    else
		ldi_f(r0 + LE_P, ((jit_word_t)i0) + 4);
	}
	else
	    MTC1(_ZERO_REGNO, r0 + LE_P);
	if (_jitc->no_data)
	    jit_unget_reg(reg);
    }
#  endif
}

static void
_ltr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LT_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_OLT_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(lt)

static void
_ler_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LE_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_OLE_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(le)

static void
_eqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_EQ_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_EQ_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(eq)

static void
_ger_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULT_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_ULT_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(ge)

static void
_gtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULE_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_ULE_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(gt)

static void
_ner_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_EQ_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_EQ_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(ne)

static void
_unltr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULT_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_ULT_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(unlt)

static void
_unler_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULE_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_ULE_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(unle)

static void
_uneqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UEQ_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_UEQ_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(uneq)

static void
_unger_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LT_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_OLT_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(unge)

static void
_ungtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LE_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_OLE_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(ungt)

static void
_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UEQ_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_UEQ_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(ltgt)

static void
_ordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UN_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_UN_S(r1, r2);
	flush();
	/* cannot optimize delay slot */
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(ord)

static void
_unordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UN_S(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_UN_S(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
fopi(unord)

static jit_word_t
_bltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LT_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLT_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(lt)

static jit_word_t
_bler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LE_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLE_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(le)

static jit_word_t
_beqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_EQ_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_EQ_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(eq)

static jit_word_t
_bger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULT_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULT_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ge)

static jit_word_t
_bgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULE_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULE_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(gt)

static jit_word_t
_bner_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_EQ_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_EQ_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ne)

static jit_word_t
_bunltr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULT_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULT_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unlt)

static jit_word_t
_bunler_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULE_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULE_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unle)

static jit_word_t
_buneqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UEQ_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UEQ_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(uneq)

static jit_word_t
_bunger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LT_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLT_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unge)

static jit_word_t
_bungtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LE_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLE_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ungt)

static jit_word_t
_bltgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UEQ_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UEQ_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ltgt)

static jit_word_t
_bordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UN_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UN_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(ord)

static jit_word_t
_bunordr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UN_S(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UN_S(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
fbopi(unord)

static void
_ltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LT_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_OLT_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(lt)

static void
_ler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LE_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_OLE_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(le)

static void
_eqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_EQ_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_EQ_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(eq)

static void
_ger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULT_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_ULT_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(ge)

static void
_gtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULE_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_ULE_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(gt)

static void
_ner_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_EQ_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_EQ_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(ne)

static void
_unltr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULT_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_ULT_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(unlt)

static void
_unler_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_ULE_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_ULE_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(unle)

static void
_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UEQ_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_UEQ_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(uneq)

static void
_unger_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LT_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_OLT_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(unge)

static void
_ungtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_LE_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_OLE_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(ungt)

static void
_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UEQ_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_UEQ_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(ltgt)

static void
_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UN_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	addi(r0, r0, 1);
    }
    else {
	C_UN_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1F(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(ord)

static void
_unordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_fpr);
	CMP_UN_D(rn(reg), r1, r2);
	MFC1(r0, rn(reg));
	jit_unget_reg(reg);
	andi(r0, r0, 1);
    }
    else {
	C_UN_D(r1, r2);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BC1T(0);
	/* delay slot */
	movi(r0, 1);
	movi(r0, 0);
	flush();
	patch_at(w, _jit->pc.w);
    }
}
dopi(unord)

static jit_word_t
_bltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LT_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLT_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(lt)

static jit_word_t
_bler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LE_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLE_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(le)

static jit_word_t
_beqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_EQ_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_EQ_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(eq)

static jit_word_t
_bger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULT_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULT_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(ge)

static jit_word_t
_bgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULE_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULE_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(gt)

static jit_word_t
_bner_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_EQ_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_EQ_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(ne)

static jit_word_t
_bunltr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULT_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULT_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(unlt)

static jit_word_t
_bunler_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_ULE_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_ULE_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(unle)

static jit_word_t
_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UEQ_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UEQ_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(uneq)

static jit_word_t
_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LT_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLT_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(unge)

static jit_word_t
_bungtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_LE_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_OLE_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(ungt)

static jit_word_t
_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UEQ_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UEQ_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(ltgt)

static jit_word_t
_bordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UN_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1EQZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UN_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1F(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(ord)

static jit_word_t
_bunordr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr, r1, r2);
	op = pending();
	CMP_UN_D(rn(reg), r1, r2);
	flush();
	w = _jit->pc.w;
	BC1NEZ(rn(reg), ((i0 - w) >> 2) - 1);
    }
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_fpr|jit_class_chk, r1, r2);
	op = pending();
	C_UN_D(r1, r2);
	flush();
	w = _jit->pc.w;
	BC1T(((i0 - w) >> 2) - 1);
    }
    delay(op);
    jit_unget_reg(reg);
    return (w);
}
dbopi(unord)

static void
_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if !NEW_ABI
    jit_int32_t		reg;
#endif
    assert(_jitc->function->self.call & jit_call_varargs);
#if !NEW_ABI
    /* Align, if required. */
    reg = jit_get_reg(jit_class_gpr);
    andi(rn(reg), r1, 7);
    addr(r1, r1, rn(reg));
    jit_unget_reg(reg);
#endif

    /* Load argument. */
    ldr_d(r0, r1);

    /* Update va_list. */
    addi(r1, r1, sizeof(jit_float64_t));
}

#  undef fopi
#  undef fbopi
#  undef dopi
#  undef dbopi
#  undef fpr_bopi
#  undef fpr_opi
#endif
