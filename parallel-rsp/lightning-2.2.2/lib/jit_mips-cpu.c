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
/* FIXME Should need qemu 7.2 -- apparently broken with qemu 7.0 */
#define PCREL_BROKEN		1
#define BALC_BROKEN		1

typedef union {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    struct {	jit_uint32_t _:26;	jit_uint32_t b :  6; } hc;
    struct {	jit_uint32_t _:21;	jit_uint32_t b :  5; } rs;
    struct {	jit_uint32_t _:21;	jit_uint32_t b :  5; } fm;
    struct {	jit_uint32_t _:21;	jit_uint32_t b :  5; } fr;
    struct {	jit_uint32_t _:18;	jit_uint32_t b :  3; } pD;
    struct {	jit_uint32_t _:19;	jit_uint32_t b :  2; } pW;
    struct {	jit_uint32_t _:16;	jit_uint32_t b :  5; } rt;
    struct {	jit_uint32_t _:16;	jit_uint32_t b :  5; } ft;
    struct {	jit_uint32_t _:11;	jit_uint32_t b :  5; } rd;
    struct {	jit_uint32_t _:11;	jit_uint32_t b :  5; } fs;
    struct {	jit_uint32_t _: 7;	jit_uint32_t b :  9; } i9;
    struct {	jit_uint32_t _: 6;	jit_uint32_t b :  5; } ic;
    struct {	jit_uint32_t _: 6;	jit_uint32_t b :  5; } fd;
    struct {				jit_uint32_t b :  6; } tc;
    struct {				jit_uint32_t b :  5; } cn;
    struct {				jit_uint32_t b : 11; } cc;
    struct {				jit_uint32_t b : 16; } is;
    struct {				jit_uint32_t b : 18; } iD;
    struct {				jit_uint32_t b : 19; } iW;
    struct {				jit_uint32_t b : 26; } ii;
#else
    struct {				jit_uint32_t b :  6; } hc;
    struct {	jit_uint32_t _: 6;	jit_uint32_t b :  5; } rs;
    struct {	jit_uint32_t _: 6;	jit_uint32_t b :  5; } fm;
    struct {	jit_uint32_t _: 6;	jit_uint32_t b :  5; } fr;
    struct {	jit_uint32_t _:11;	jit_uint32_t b :  3; } pD;
    struct {	jit_uint32_t _:11;	jit_uint32_t b :  2; } pW;
    struct {	jit_uint32_t _:11;	jit_uint32_t b :  5; } rt;
    struct {	jit_uint32_t _:11;	jit_uint32_t b :  5; } ft;
    struct {	jit_uint32_t _:16;	jit_uint32_t b :  5; } rd;
    struct {	jit_uint32_t _:16;	jit_uint32_t b :  5; } fs;
    struct {	jit_uint32_t _:16;	jit_uint32_t b :  9; } i9;
    struct {	jit_uint32_t _:21;	jit_uint32_t b :  5; } ic;
    struct {	jit_uint32_t _:21;	jit_uint32_t b :  5; } fd;
    struct {	jit_uint32_t _:26;	jit_uint32_t b :  6; } tc;
    struct {	jit_uint32_t _:27;	jit_uint32_t b :  5; } cn;
    struct {	jit_uint32_t _:21;	jit_uint32_t b : 11; } cc;
    struct {	jit_uint32_t _:16;	jit_uint32_t b : 16; } is;
    struct {	jit_uint32_t _:14;	jit_uint32_t b : 18; } iD;
    struct {	jit_uint32_t _:13;	jit_uint32_t b : 19; } iW;
    struct {	jit_uint32_t _: 6;	jit_uint32_t b : 26; } ii;
#endif
    int					op;
} jit_instr_t;
#define jit_mips2_p()			(jit_cpu.release >= 2)
#define jit_mips6_p()			(jit_cpu.release >= 6)
#define jit_unaligned_p()		(jit_cpu.unaligned)
#  define _ZERO_REGNO			0
#  define _T0_REGNO			0x08
#  define _T1_REGNO			0x09
#  define _T2_REGNO			0x0a
#  define _T3_REGNO			0x0b
#  define _T4_REGNO			0x0c
#  define _T5_REGNO			0x0d
#  define _T6_REGNO			0x0e
#  define _T7_REGNO			0x0f
#  define _S0_REGNO			0x10
#  define _S1_REGNO			0x11
#  define _S2_REGNO			0x12
#  define _S3_REGNO			0x13
#  define _S4_REGNO			0x14
#  define _S5_REGNO			0x15
#  define _S6_REGNO			0x16
#  define _S7_REGNO			0x17
#  define _T8_REGNO			0x18
#  define _T9_REGNO			0x19
#  define _SP_REGNO			0x1d
#  define _BP_REGNO			0x1e
#  define _RA_REGNO			0x1f
#  define _F16_REGNO			16
#  define _F18_REGNO			18
#  define _F20_REGNO			20
#  define _F22_REGNO			22
#  define _F24_REGNO			24
#  define _F26_REGNO			26
#  define _F28_REGNO			28
#  define _F30_REGNO			30
#  if __WORDSIZE == 32
#    define ldr(u,v)			ldr_i(u,v)
#    define ldi(u,v)			ldi_i(u,v)
#    define ldxr(u,v,w)			ldxr_i(u,v,w)
#    define ldxi(u,v,w)			ldxi_i(u,v,w)
#    define str(u,v)			str_i(u,v)
#    define sti(u,v)			sti_i(u,v)
#    define stxr(u,v,w)			stxr_i(u,v,w)
#    define stxi(u,v,w)			stxi_i(u,v,w)
#  else
#    define ldr(u,v)			ldr_l(u,v)
#    define ldi(u,v)			ldi_l(u,v)
#    define ldxr(u,v,w)			ldxr_l(u,v,w)
#    define ldxi(u,v,w)			ldxi_l(u,v,w)
#    define str(u,v)			str_l(u,v)
#    define sti(u,v)			sti_l(u,v)
#    define stxr(u,v,w)			stxr_l(u,v,w)
#    define stxi(u,v,w)			stxi_l(u,v,w)
#  endif
/* can_relative_jump_p(im) => can_sign_extend_short_p(im << 2) */
#  define can_relative_jump_p(im)	((im) >= -130712 && (im) <= 131068)
/* can_compact_jump_p(im) => can_sign_extend_i26_p(im << 2) */
#  define can_compact_jump_p(im)	((im) >= -268435456 && (im) <= 268435452)
#  define can_sign_extend_short_p(im)	((im) >= -32678 && (im) <= 32767)
#  define can_zero_extend_short_p(im)	((im) >= 0 && (im) <= 65535)
#  define can_sign_extend_i18_p(im)	((im) >= -262144 && (im) <= 262143)
#  define can_sign_extend_i19_p(im)	((im) >= -524288 && (im) <= 524287)
#  define can_sign_extend_i26_p(im)	((im) >= -67108864 && (im) <= 67108863)
#  define is_low_mask(im)		(((im) & 1) ? (__builtin_popcountl((im) + 1) <= 1) : 0)
#  define is_middle_mask(im)		((im) ? (__builtin_popcountl((im) + (1 << __builtin_ctzl(im))) <= 1) : 0)
#  define is_high_mask(im)		((im) ? (__builtin_popcountl((im) + (1 << __builtin_ctzl(im))) == 0) : 0)
#  define masked_bits_count(im)		__builtin_popcountl(im)
#  define unmasked_bits_count(im)	(__WORDSIZE - masked_bits_count(im))
#  if __WORDSIZE == 32
#    define can_sign_extend_int_p(im)	1
#    define can_zero_extend_int_p(im)	1
#  else
#    define can_sign_extend_int_p(im)					\
	(((im) >= 0 && (im) <=  0x7fffffffL) ||				\
	 ((im) <  0 && (im) >= -0x80000000L))
#    define can_zero_extend_int_p(im)	((im) >= 0 && (im) <= 0xffffffff)
#  endif
#  define MIPS_SPECIAL			0x00
#  define MIPS_REGIMM			0x01
#  define MIPS_J			0x02
#  define MIPS_SRL			0x02
#  define MIPS_JAL			0x03
#  define MIPS_SRA			0x03
#  define MIPS_BEQ			0x04
#  define MIPS_BNE			0x05
#  define MIPS_BLEZ			0x06
#  define MIPS_BGTZ			0x07
#  define MIPS_ADDI			0x08
#  define MIPS_ADDIU			0x09
#  define MIPS_SLTI			0x0a
#  define MIPS_SLTIU			0x0b
#  define MIPS_ANDI			0x0c
#  define MIPS_ORI			0x0d
#  define MIPS_XORI			0x0e
#  define MIPS_LUI			0x0f
#  define MIPS_COP0			0x10
#  define MIPS_COP1			0x11
#  define MIPS_COP2			0x12
#  define MIPS_COP1X			0x13
#  define MIPS_BEQL			0x14
#  define MIPS_BNEL			0x15
#  define MIPS_BLEZL			0x16
#  define MIPS_BGTZL			0x17
#  define MIPS_DADDI			0x18
#  define MIPS_DADDIU			0x19
#  define MIPS_LDL			0x1a
#  define MIPS_LDR			0x1b
#  define MIPS_SPECIAL2			0x1c
#  define MIPS_JALX			0x1d
#  define MIPS_DAUI			0x1d
#  define MIPS_SPECIAL3			0x1f
#  define MIPS_LB 			0x20
#  define MIPS_LH 			0x21
#  define MIPS_LWL			0x22
#  define MIPS_LW 			0x23
#  define MIPS_LBU			0x24
#  define MIPS_LHU			0x25
#  define MIPS_LWR			0x26
#  define MIPS_LWU			0x27
#  define MIPS_SB 			0x28
#  define MIPS_SH 			0x29
#  define MIPS_SWL			0x2a
#  define MIPS_SW 			0x2b
#  define MIPS_SDL			0x2c
#  define MIPS_SDR			0x2d
#  define MIPS_SWR			0x2e
#  define MIPS_CACHE			0x2f
#  define MIPS_LL 			0x30
#  define MIPS_LWC1			0x31
#  define MIPS_LWC2			0x32
#  define MIPS_PREF			0x33
#  define MIPS_LLD			0x34
#  define MIPS_LDC1			0x35
#  define MIPS_LDC2			0x36
#  define MIPS_LD 			0x37
#  define MIPS_SC 			0x38
#  define MIPS_BC_R6			0x32
#  define MIPS_BALC			0x3a
#  define MIPS_PCREL			0x3b
#  define MIPS_SCD			0x3c
#  define MIPS_SDC1			0x3d
#  define MIPS_SDC2			0x3e
#  define MIPS_SWC1			0x39
#  define MIPS_SWC2			0x3a
#  define MIPS_SD 			0x3f
#  define MIPS_MF			0x00
#  define MIPS_DMF			0x01
#  define MIPS_CF			0x02
#  define MIPS_MFH			0x03
#  define MIPS_MT			0x04
#  define MIPS_DMT			0x05
#  define MIPS_CT			0x06
#  define MIPS_MTH			0x07
#  define MIPS_BC			0x08
#  define MIPS_BC1EQZ			0x09	/* release 6 */
#  define MIPS_BC1NEZ			0x0d	/* release 6 */
#  define MIPS_WRPGPR			0x0e
#  define MIPS_BGZAL			0x11
#  define MIPS_MFMC0			0x11
#  define MIPS_BCF			0x00
#  define MIPS_BLTZ			0x00
#  define MIPS_BCT			0x01
#  define MIPS_BGEZ			0x01
#  define MIPS_BCFL			0x02
#  define MIPS_BLTZL			0x02
#  define MIPS_BCTL			0x03
#  define MIPS_BGEZL			0x03
#  define MIPS_TGEI			0x08
#  define MIPS_TGEIU			0x09
#  define MIPS_TLTI			0x0a
#  define MIPS_TLTIU			0x0b
#  define MIPS_TEQI			0x0c
#  define MIPS_TNEI			0x0e
#  define MIPS_BLTZAL			0x10
#  define MIPS_BGEZAL			0x11
#  define MIPS_BLTZALL			0x12
#  define MIPS_BGEZALL			0x13
#  define MIPS_SYNCI			0x1f
#  define MIPS_WSBH			0x02
#  define MIPS_DSBH			0x02
#  define MIPS_DSHD			0x05
#  define MIPS_SEB			0x10
#  define MIPS_SEH			0x18
#  define MIPS_MADD			0x00
#  define MIPS_SLL			0x00
#  define MIPS_EXT			0x00
#  define MIPS_DEXTM			0x01
#  define MIPS_MADDU			0x01
#  define MIPS_MOVFT			0x01
#  define MIPS_TLBR			0x01
#  define MIPS_MUL			0x02
#  define MIPS_DEXTU			0x02
#  define MIPS_TLBWI			0x02
#  define MIPS_DEXT			0x03
#  define MIPS_SLLV			0x04
#  define MIPS_INS			0x04
#  define MIPS_MSUB			0x04
#  define MIPS_DINSM			0x05
#  define MIPS_MSUBU			0x05
#  define MIPS_SRLV			0x06
#  define MIPS_DINSU			0x06
#  define MIPS_TLBWR			0x06
#  define MIPS_SRAV			0x07
#  define MIPS_DINS			0x07
#  define MIPS_JR			0x08
#  define MIPS_TLBP			0x08
#  define MIPS_JALR			0x09
#  define MIPS_MOVZ			0x0a
#  define MIPS_MOVN			0x0b
#  define MIPS_SYSCALL			0x0c
#  define MIPS_BREAK			0x0d
#  define MIPS_PREFX			0x0f
#  define MIPS_SYNC			0x0f
#  define MIPS_MFHI			0x10
#  define MIPS_MTHI			0x11
#  define MIPS_MFLO			0x12
#  define MIPS_MTLO			0x13
#  define MIPS_DSLLV			0x14
#  define MIPS_DSRLV			0x16
#  define MIPS_DSRAV			0x17
#  define MIPS_MULT			0x18
#  define MIPS_ERET			0x18
#  define MIPS_MULTU			0x19
#  define MIPS_DIV			0x1a
#  define MIPS_DIVU			0x1b
#  define MIPS_DMULT			0x1c
#  define MIPS_DMULTU			0x1d
#  define MIPS_DDIV			0x1e
#  define MIPS_DDIVU			0x1f
#  define MIPS_DERET			0x1f
#  define MIPS_ADD			0x20
#  define MIPS_CLZ			0x20
#  define MIPS_BSHFL			0x20
#  define MIPS_ADDU			0x21
#  define MIPS_CLO			0x21
#  define MIPS_SUB			0x22
#  define MIPS_SUBU			0x23
#  define MIPS_AND			0x24
#  define MIPS_DCLZ			0x24
#  define MIPS_DBSHFL			0x24
#  define MIPS_OR			0x25
#  define MIPS_DCLO			0x25
#  define MIPS_XOR			0x26
#  define MIPS_NOR			0x27
#  define MIPS_SLT			0x2a
#  define MIPS_SLTU			0x2b
#  define MIPS_DADD			0x2c
#  define MIPS_DADDU			0x2d
#  define MIPS_DSUB			0x2e
#  define MIPS_DSUBU			0x2f
#  define MIPS_TGE			0x30
#  define MIPS_TGEU			0x31
#  define MIPS_TLT			0x32
#  define MIPS_TLTU			0x33
#  define MIPS_TEQ			0x34
#  define MIPS_TNE			0x36
#  define MIPS_DSLL			0x38
#  define MIPS_DSRL			0x3a
#  define MIPS_DSRA			0x3b
#  define MIPS_DSLL32			0x3c
#  define MIPS_DSRL32			0x3e
#  define MIPS_DSRA32			0x3f
#  define MIPS_SDBPP			0x3f
#  define ii(i)				*_jit->pc.ui++ = i
#  define instr(op)			_instr(_jit, op)
static void _instr(jit_state_t*, jit_int32_t);
#  define flush()			_flush(_jit)
static void _flush(jit_state_t*);
#  define pending()			_pending(_jit)
static jit_int32_t _pending(jit_state_t*);
#  define delay(op)			_delay(_jit,op)
static void _delay(jit_state_t*,jit_int32_t);
#  define jit_get_reg_for_delay_slot(mask, r0,r1)			\
	_jit_get_reg_for_delay_slot(_jit,mask,r0,r1)
static jit_int32_t _jit_get_reg_for_delay_slot(jit_state_t*,jit_int32_t,
					       jit_int32_t, jit_int32_t);
#  define hrrrit(hc,rs,rt,rd,im,tc)	_hrrrit(_jit,hc,rs,rt,rd,im,tc)
static void
_hrrrit(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,
	jit_int32_t,jit_int32_t);
#  define hrrr_t(hc,rs,rt,rd,tc)	hrrrit(hc,rs,rt,rd,0,tc)
#  define rrr_t(rs,rt,rd,tc)		hrrr_t(0,rs,rt,rd,tc)
#  define hrri(hc,rs,rt,im)		_hrri(_jit,hc,rs,rt,im)
static void _hrri(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define hrri9(hc,rs,rt,i9,tc)		_hrri9(_jit,hc,rs,rt,i9,tc)
static void _hrri9(jit_state_t*,jit_int32_t,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define hriD(hc,rs,pD,iD)		_hriD(_jit,hc,rs,pD,iD)
static void _hriD(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define hriW(hc,rs,pW,iW)		_hriW(_jit,hc,rs,pW,iW)
static void _hriW(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define hi(hc,im)			_hi(_jit,hc,im)
static void _hi(jit_state_t*,jit_int32_t,jit_int32_t);
#  define NOP(i0)			instr(0)
#  define nop(i0)			_nop(_jit,i0)
static void _nop(jit_state_t*,jit_int32_t);
#  define h_ri(hc,rt,im)		_hrri(_jit,hc,0,rt,im)
#  define rrit(rt,rd,im,tc)		_hrrrit(_jit,0,0,rt,rd,im,tc)
#  define AUIPC(rs,im)			hrri(MIPS_PCREL,rs,30,im)
#  define ALUIPC(rs,im)			hrri(MIPS_PCREL,rs,31,im)
#  define ADDIUPC(rs,im)		hriW(MIPS_PCREL,rs,0,im)
#  define LUI(rt,im)			h_ri(MIPS_LUI,rt,im)
#  define AUI(rs,rt)			hrri(MIPS_LUI,rs,rt,im)	/* mips r6 */
#  define ADDU(rd,rs,rt)		rrr_t(rs,rt,rd,MIPS_ADDU)
#  define DADDU(rd,rs,rt)		rrr_t(rs,rt,rd,MIPS_DADDU)
#  define ADDIU(rt,rs,im)		hrri(MIPS_ADDIU,rs,rt,im)
#  define DADDIU(rt,rs,im)		hrri(MIPS_DADDIU,rs,rt,im)
#  define SUBU(rd,rs,rt)		rrr_t(rs,rt,rd,MIPS_SUBU)
#  define DSUBU(rd,rs,rt)		rrr_t(rs,rt,rd,MIPS_DSUBU)
#  define MUL(rd,rs,rt)			hrrr_t(MIPS_SPECIAL2,rs,rt,rd,MIPS_MUL)
#  define MULT(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_MULT)
#  define MUL_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 24)
#  define MUH_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 24)
#  define MULTU(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_MULTU)
#  define MULU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 25)
#  define MUHU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 25)
#  define DMULT(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_DMULT)
#  define DMUL_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 28)
#  define DMUH_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 28)
#  define DMULTU(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_DMULTU)
#  define DMULU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 29)
#  define DMUHU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 29)
#  define DIV(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_DIV)
#  define DIV_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 26)
#  define MOD_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 26)
#  define DIVU(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_DIVU)
#  define DIVU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 27)
#  define MODU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 27)
#  define DDIV(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_DDIV)
#  define DDIV_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 30)
#  define DMOD_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 30)
#  define DDIVU(rs,rt)			rrr_t(rs,rt,_ZERO_REGNO,MIPS_DDIVU)
#  define DDIVU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 2, 31)
#  define DMODU_R6(rd,rs,rt)		hrrrit(MIPS_SPECIAL, rs, rt, rd, 3, 31)
#  define SLLV(rd,rt,rs)		rrr_t(rs,rt,rd,MIPS_SLLV)
#  define SLL(rd,rt,sa)			rrit(rt,rd,sa,MIPS_SLL)
#  define DSLLV(rd,rt,rs)		rrr_t(rs,rt,rd,MIPS_DSLLV)
#  define DSLL(rd,rt,sa)		rrit(rt,rd,sa,MIPS_DSLL)
#  define DSLL32(rd,rt,sa)		rrit(rt,rd,sa,MIPS_DSLL32)
#  define SRAV(rd,rt,rs)		rrr_t(rs,rt,rd,MIPS_SRAV)
#  define SRA(rd,rt,sa)			rrit(rt,rd,sa,MIPS_SRA)
#  define SRLV(rd,rt,rs)		rrr_t(rs,rt,rd,MIPS_SRLV)
#  define SRL(rd,rt,sa)			rrit(rt,rd,sa,MIPS_SRL)
#  define DSRAV(rd,rt,rs)		rrr_t(rs,rt,rd,MIPS_DSRAV)
#  define DSRA(rd,rt,sa)		rrit(rt,rd,sa,MIPS_DSRA)
#  define DSRA32(rd,rt,sa)		rrit(rt,rd,sa,MIPS_DSRA32)
#  define DSRLV(rd,rt,rs)		rrr_t(rs,rt,rd,MIPS_DSRLV)
#  define DSRL(rd,rt,sa)		rrit(rt,rd,sa,MIPS_DSRL)
#  define DSRL32(rd,rt,sa)		rrit(rt,rd,sa,MIPS_DSRL32)
#  define INS(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,pos+size-1,pos,MIPS_INS)
#  define DINS(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,pos+size-1,pos,MIPS_DINS)
#  define DINSU(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,pos+size-32-1,pos-32,MIPS_DINSU)
#  define DINSM(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,pos+size-32-1,pos,MIPS_DINSM)
#  define EXT(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,size-1,pos,MIPS_EXT)
#  define DEXT(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,size-1,pos,MIPS_DEXT)
#  define DEXTU(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,size-1,pos-32,MIPS_DEXTU)
#  define DEXTM(rt,rs,pos,size)		hrrrit(MIPS_SPECIAL3,rs,rt,size-32-1,pos,MIPS_DEXTM)
#  define ROTR(rd,rt,sa)		hrrrit(MIPS_SPECIAL,1,rt,rd,sa,MIPS_SRL)
#  define ROTRV(rd,rt,rs)		hrrrit(MIPS_SPECIAL,rs,rt,rd,1,MIPS_SRLV)
#  define DROTR(rd,rt,sa)		hrrrit(MIPS_SPECIAL,1,rt,rd,sa,MIPS_DSRL)
#  define DROTR32(rd,rt,sa)		hrrrit(MIPS_SPECIAL,1,rt,rd,sa,MIPS_DSRL32)
#  define DROTRV(rd,rt,rs)		hrrrit(MIPS_SPECIAL,rs,rt,rd,1,MIPS_DSRLV)
#  define SYNC()			rrr_t(_ZERO_REGNO,_ZERO_REGNO,_ZERO_REGNO,MIPS_SYNC)
#  define MFHI(rd)			rrr_t(_ZERO_REGNO,_ZERO_REGNO,rd,MIPS_MFHI)
#  define MFLO(rd)			rrr_t(_ZERO_REGNO,_ZERO_REGNO,rd,MIPS_MFLO)
#  define MTHI(rs)			rrr_t(rs,_ZERO_REGNO,_ZERO_REGNO,MIPS_MTHI)
#  define MTLO(rs)			rrr_t(rs,_ZERO_REGNO,_ZERO_REGNO,MIPS_MTLO)
#  define AND(rd,rs,rt)			rrr_t(rs,rt,rd,MIPS_AND)
#  define ANDI(rt,rs,im)		hrri(MIPS_ANDI,rs,rt,im)
#  define OR(rd,rs,rt)			rrr_t(rs,rt,rd,MIPS_OR)
#  define ORI(rt,rs,im)			hrri(MIPS_ORI,rs,rt,im)
#  define NOR(rd,rs,rt)			rrr_t(rs,rt,rd,MIPS_NOR)
#  define XOR(rd,rs,rt)			rrr_t(rs,rt,rd,MIPS_XOR)
#  define XORI(rt,rs,im)		hrri(MIPS_XORI,rs,rt,im)
#  define LB(rt,of,rb)			hrri(MIPS_LB,rb,rt,of)
#  define LBU(rt,of,rb)			hrri(MIPS_LBU,rb,rt,of)
#  define LH(rt,of,rb)			hrri(MIPS_LH,rb,rt,of)
#  define LHU(rt,of,rb)			hrri(MIPS_LHU,rb,rt,of)
#  define LW(rt,of,rb)			hrri(MIPS_LW,rb,rt,of)
#  define LWPC(rs,im)			hriW(MIPS_PCREL,rs,1,im)
#  define LWU(rt,of,rb)			hrri(MIPS_LWU,rb,rt,of)
#  define LWUPC(rs,im)			hriW(MIPS_PCREL,rs,2,im)
#  define LWL(rt,of,rb)			hrri(MIPS_LWL,rb,rt,of)
#  define LWR(rt,of,rb)			hrri(MIPS_LWR,rb,rt,of)
#  define LD(rt,of,rb)			hrri(MIPS_LD,rb,rt,of)
#  define LDPC(rs,im)			hriD(MIPS_PCREL,rs,6,im)
#  define LL(rt,of,rb)			hrri(MIPS_LL,rb,rt,of)
#  define LL_R6(rt,of,rb)		hrri9(MIPS_SPECIAL3,rb,rt,of,54)
#  define LLD(rt,of,rb)			hrri(MIPS_LLD,rb,rt,of)
#  define LLD_R6(rt,of,rb)		hrri9(MIPS_SPECIAL3,rb,rt,of,55)
#  define LDL(rt,of,rb)			hrri(MIPS_LDL,rb,rt,of)
#  define LDR(rt,of,rb)			hrri(MIPS_LDR,rb,rt,of)
#  define SB(rt,of,rb)			hrri(MIPS_SB,rb,rt,of)
#  define SH(rt,of,rb)			hrri(MIPS_SH,rb,rt,of)
#  define SW(rt,of,rb)			hrri(MIPS_SW,rb,rt,of)
#  define SWL(rt,of,rb)			hrri(MIPS_SWL,rb,rt,of)
#  define SWR(rt,of,rb)			hrri(MIPS_SWR,rb,rt,of)
#  define SD(rt,of,rb)			hrri(MIPS_SD,rb,rt,of)
#  define SC(rt,of,rb)			hrri(MIPS_SC,rb,rt,of)
#  define SC_R6(rt,of,rb)		hrri9(MIPS_SPECIAL3,rb,rt,of,38)
#  define SCD(rt,of,rb)			hrri(MIPS_SCD,rb,rt,of)
#  define SCD_R6(rt,of,rb)		hrri9(MIPS_SPECIAL3,rb,rt,of,39)
#  define SDL(rt,of,rb)			hrri(MIPS_SDL,rb,rt,of)
#  define SDR(rt,of,rb)			hrri(MIPS_SDR,rb,rt,of)
#  define WSBH(rd,rt)			hrrrit(MIPS_SPECIAL3,0,rt,rd,MIPS_WSBH,MIPS_BSHFL)
#  define SEB(rd,rt)			hrrrit(MIPS_SPECIAL3,0,rt,rd,MIPS_SEB,MIPS_BSHFL)
#  define SEH(rd,rt)			hrrrit(MIPS_SPECIAL3,0,rt,rd,MIPS_SEH,MIPS_BSHFL)
#  define DSBH(rd,rt)			hrrrit(MIPS_SPECIAL3,0,rt,rd,MIPS_DSBH,MIPS_DBSHFL)
#  define DSHD(rd,rt)			hrrrit(MIPS_SPECIAL3,0,rt,rd,MIPS_DSHD,MIPS_DBSHFL)
#  define SLT(rd,rs,rt)			rrr_t(rs,rt,rd,MIPS_SLT)
#  define SLTU(rd,rs,rt)		rrr_t(rs,rt,rd,MIPS_SLTU)
#  define SLTI(rt,rs,im)		hrri(MIPS_SLTI,rs,rt,im)
#  define SLTIU(rt,rs,im)		hrri(MIPS_SLTIU,rs,rt,im)
#  define DAUI(rt,rs,im)		hrri(MIPS_DAUI,rs,rt,im)
#  define DAHI(rs,im)			hrri(MIPS_REGIMM,rs,6,im)
#  define DATI(rs,im)			hrri(MIPS_REGIMM,rs,30,im)
#  define BLTZ(rs,im)			hrri(MIPS_REGIMM,rs,MIPS_BLTZ,im)
#  define BLEZ(rs,im)			hrri(MIPS_BLEZ,rs,_ZERO_REGNO,im)
#  define BEQ(rs,rt,im)			hrri(MIPS_BEQ,rs,rt,im)
#  define BGEZ(rs,im)			hrri(MIPS_REGIMM,rs,MIPS_BGEZ,im)
#  define BGTZ(rs,im)			hrri(MIPS_BGTZ,rs,_ZERO_REGNO,im)
#  define BNE(rs,rt,im)			hrri(MIPS_BNE,rs,rt,im)
#  define BGEZAL(rs,im)			hrri(MIPS_REGIMM,rs,MIPS_BGEZAL,im)
#  define JALR(r0)			hrrrit(MIPS_SPECIAL,r0,0,_RA_REGNO,0,MIPS_JALR)
#  if 1		/* This should work for mips r6 or older */
#    define JR(r0)			hrrrit(MIPS_SPECIAL,r0,0,0,0,MIPS_JALR)
#  else		/* This should generate an illegal instruction in mips r6 */
#    define JR(r0)			hrrrit(MIPS_SPECIAL,r0,0,0,0,MIPS_JR)
#  endif
#  define CLO_R6(rd,rs)			hrrrit(MIPS_SPECIAL,rs,0,rd,1,0x11)
#  define DCLO_R6(rd,rs)		hrrrit(MIPS_SPECIAL,rs,0,rd,1,0x13)
#  define CLZ_R6(rd,rs)			hrrrit(MIPS_SPECIAL,rs,0,rd,1,0x10)
#  define DCLZ_R6(rd,rs)		hrrrit(MIPS_SPECIAL,rs,0,rd,1,0x12)
#  define BITSWAP(rd,rt)		hrrrit(MIPS_SPECIAL3,0,rt,rd,0,0x20)
#  define DBITSWAP(rd,rt)		hrrrit(MIPS_SPECIAL3,0,rt,rd,0,0x24)
#  define CLO(rd,rs)			hrrrit(MIPS_SPECIAL2,rs,rd,rd,0,MIPS_CLO)
#  define DCLO(rd,rs)			hrrrit(MIPS_SPECIAL2,rs,rd,rd,0,MIPS_DCLO)
#  define CLZ(rd,rs)			hrrrit(MIPS_SPECIAL2,rs,rd,rd,0,MIPS_CLZ)
#  define DCLZ(rd,rs)			hrrrit(MIPS_SPECIAL2,rs,rd,rd,0,MIPS_DCLZ)
#  define J(i0)				hi(MIPS_J,i0)
#  define JAL(i0)			hi(MIPS_JAL,i0)
#  define BC_R6(i0)			hi(MIPS_BC_R6,i0)
#  define BALC(i0)			hi(MIPS_BALC,i0)
#  define MOVN(rd,rs,rt)		hrrrit(0,rs,rt,rd,0,MIPS_MOVN)
#  define MOVZ(rd,rs,rt)		hrrrit(0,rs,rt,rd,0,MIPS_MOVZ)
#  define SELEQZ(rd,rs,rt)		hrrrit(0,rs,rt,rd,0,53)
#  define SELNEZ(rd,rs,rt)		hrrrit(0,rs,rt,rd,0,55)
#  define comr(r0,r1)			xori(r0,r1,-1)
#  define negr(r0,r1)			subr(r0,_ZERO_REGNO,r1)
#  define clor(r0, r1)			_clor(_jit, r0, r1)
static void _clor(jit_state_t*, jit_int32_t, jit_int32_t);
#  define clzr(r0, r1)			_clzr(_jit, r0, r1)
static void _clzr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ctor(r0, r1)			_ctor(_jit, r0, r1)
static void _ctor(jit_state_t*, jit_int32_t, jit_int32_t);
#  define ctzr(r0, r1)			_ctzr(_jit, r0, r1)
static void _ctzr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define rbitr(r0, r1)			_rbitr(_jit, r0, r1)
static void _rbitr(jit_state_t*, jit_int32_t, jit_int32_t);
#  if __WORDSIZE == 32
#    define addr(rd,rs,rt)		ADDU(rd,rs,rt)
#    define addiu(r0,r1,i0)		ADDIU(r0,r1,i0)
#    define subr(rd,rs,rt)		SUBU(rd,rs,rt)
#    define mult(rs,rt)			MULT(rs,rt)
#    define mul_r6(rd,rs,rt)		MUL_R6(rd,rs,rt)
#    define muh_r6(rd,rs,rt)		MUH_R6(rd,rs,rt)
#    define multu(rs,rt)		MULTU(rs,rt)
#    define mulu_r6(rd,rs,rt)		MULU_R6(rd,rs,rt)
#    define muhu_r6(rd,rs,rt)		MUHU_R6(rd,rs,rt)
#    define div(rs,rt)			DIV(rs,rt)
#    define divu(rs,rt)			DIVU(rs,rt)
#    define div_r6(rd,rs,rt)		DIV_R6(rd,rs,rt)
#    define divu_r6(rd,rs,rt)		DIVU_R6(rd,rs,rt)
#    define mod_r6(rd,rs,rt)		MOD_R6(rd,rs,rt)
#    define modu_r6(rd,rs,rt)		MODU_R6(rd,rs,rt)
#  else
#    define addr(rd,rs,rt)		DADDU(rd,rs,rt)
#    define addiu(r0,r1,i0)		DADDIU(r0,r1,i0)
#    define subr(rd,rs,rt)		DSUBU(rd,rs,rt)
#    define mult(rs,rt)			DMULT(rs,rt)
#    define mul_r6(rd,rs,rt)		DMUL_R6(rd,rs,rt)
#    define muh_r6(rd,rs,rt)		DMUH_R6(rd,rs,rt)
#    define multu(rs,rt)		DMULTU(rs,rt)
#    define mulu_r6(rd,rs,rt)		DMULU_R6(rd,rs,rt)
#    define muhu_r6(rd,rs,rt)		DMUHU_R6(rd,rs,rt)
#    define div(rs,rt)			DDIV(rs,rt)
#    define divu(rs,rt)			DDIVU(rs,rt)
#    define div_r6(rd,rs,rt)		DDIV_R6(rd,rs,rt)
#    define divu_r6(rd,rs,rt)		DDIVU_R6(rd,rs,rt)
#    define mod_r6(rd,rs,rt)		DMOD_R6(rd,rs,rt)
#    define modu_r6(rd,rs,rt)		DMODU_R6(rd,rs,rt)
#  endif
#  define mips_extr(rd,rt,lsb,nb)	_mips_extr(_jit,rd,rt,lsb,nb)
static void _mips_extr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define insr(rd,rt,lsb,nb)	_insr(_jit,rd,rt,lsb,nb)
static void _insr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addi(r0,r1,i0)		_addi(_jit,r0,r1,i0)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define addcr(r0,r1,r2)			_addcr(_jit,r0,r1,r2)
static void _addcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define addci(r0,r1,i0)			_addci(_jit,r0,r1,i0)
static void _addci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addxr(r0,r1,r2)		_addxr(_jit,r0,r1,r2)
static void _addxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addxi(r0,r1,i0)		_addxi(_jit,r0,r1,i0)
static void _addxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subi(r0,r1,i0)		_subi(_jit,r0,r1,i0)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subcr(r0,r1,r2)		_subcr(_jit,r0,r1,r2)
static void _subcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subci(r0,r1,i0)		_subci(_jit,r0,r1,i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subxr(r0,r1,r2)		_subxr(_jit,r0,r1,r2)
static void _subxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subxi(r0,r1,i0)		_subxi(_jit,r0,r1,i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define mulr(r0,r1,r2)		_mulr(_jit,r0,r1,r2)
static void _mulr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define muli(r0,r1,i0)		_muli(_jit,r0,r1,i0)
static void _muli(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,1)
#  define qmulr_u(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,0)
#  define iqmulr(r0,r1,r2,r3,cc)	_iqmulr(_jit,r0,r1,r2,r3,cc)
static void _iqmulr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#  define qmuli(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,1)
#  define qmuli_u(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,0)
#  define iqmuli(r0,r1,r2,i0,cc)	_iqmuli(_jit,r0,r1,r2,i0,cc)
static void _iqmuli(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#  define divr(r0,r1,r2)		_divr(_jit,r0,r1,r2)
static void _divr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi(r0,r1,i0)		_divi(_jit,r0,r1,i0)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define divr_u(r0,r1,r2)		_divr_u(_jit,r0,r1,r2)
static void _divr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi_u(r0,r1,i0)		_divi_u(_jit,r0,r1,i0)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qdivr(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,1)
#  define qdivr_u(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,0)
#  define iqdivr(r0,r1,r2,r3,cc)	_iqdivr(_jit,r0,r1,r2,r3,cc)
static void _iqdivr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#  define qdivi(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,1)
#  define qdivi_u(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,0)
#  define iqdivi(r0,r1,r2,i0,cc)	_iqdivi(_jit,r0,r1,r2,i0,cc)
static void _iqdivi(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#  define remr(r0,r1,r2)		_remr(_jit,r0,r1,r2)
static void _remr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi(r0,r1,i0)		_remi(_jit,r0,r1,i0)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define remr_u(r0,r1,r2)		_remr_u(_jit,r0,r1,r2)
static void _remr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi_u(r0,r1,i0)		_remi_u(_jit,r0,r1,i0)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 32
#    define lshr(r0,r1,r2)		SLLV(r0,r1,r2)
#    define lshi(r0,r1,i0)		SLL(r0,r1,i0)
#    define rshr(r0,r1,r2)		SRAV(r0,r1,r2)
#    define rshi(r0,r1,i0)		SRA(r0,r1,i0)
#    define rshr_u(r0,r1,r2)		SRLV(r0,r1,r2)
#    define rshi_u(r0,r1,i0)		SRL(r0,r1,i0)
#  else
#    define lshr(r0,r1,r2)		DSLLV(r0,r1,r2)
#    define lshi(r0,r1,i0)		_lshi(_jit,r0,r1,i0)
static void _lshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#    define rshr(r0,r1,r2)		DSRAV(r0,r1,r2)
#    define rshi(r0,r1,i0)		_rshi(_jit,r0,r1,i0)
static void _rshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#    define rshr_u(r0,r1,r2)		DSRLV(r0,r1,r2)
#    define rshi_u(r0,r1,i0)		_rshi_u(_jit,r0,r1,i0)
static void _rshi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  endif
#  define qlshr(r0,r1,r2,r3)		xlshr(1,r0,r1,r2,r3)
#  define qlshr_u(r0, r1, r2, r3)	xlshr(0, r0, r1, r2, r3)
#  define xlshr(s,r0,r1,r2,r3)		_xlshr(_jit,s,r0,r1,r2,r3)
static void
_xlshr(jit_state_t*,jit_bool_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define qlshi(r0, r1, r2, i0)		xlshi(1, r0, r1, r2, i0)
#  define qlshi_u(r0, r1, r2, i0)	xlshi(0, r0, r1, r2, i0)
#  define xlshi(s, r0, r1, r2, i0)	_xlshi(_jit, s, r0, r1, r2, i0)
static void
_xlshi(jit_state_t*,jit_bool_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define qrshr(r0, r1, r2, r3)		xrshr(1, r0, r1, r2, r3)
#  define qrshr_u(r0, r1, r2, r3)	xrshr(0, r0, r1, r2, r3)
#  define xrshr(s, r0, r1, r2, r3)	_xrshr(_jit, s, r0, r1, r2, r3)
static void
_xrshr(jit_state_t*,jit_bool_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define qrshi(r0, r1, r2, i0)		xrshi(1, r0, r1, r2, i0)
#  define qrshi_u(r0, r1, r2, i0)	xrshi(0, r0, r1, r2, i0)
#  define xrshi(s, r0, r1, r2, i0)	_xrshi(_jit, s, r0, r1, r2, i0)
static void
_xrshi(jit_state_t*,jit_bool_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#    define lrotr(r0,r1,r2)		_lrotr(_jit,r0,r1,r2)
static void _lrotr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define lroti(r0,r1,i0)		rroti(r0,r1,__WORDSIZE-i0)
#    define rrotr(r0,r1,r2)		_rrotr(_jit,r0,r1,r2)
static void _rrotr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define rroti(r0,r1,i0)		_rroti(_jit,r0,r1,i0)
static void _rroti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define andr(r0,r1,r2)		AND(r0,r1,r2)
#  define andi(r0,r1,i0)		_andi(_jit,r0,r1,i0)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define orr(r0,r1,r2)			OR(r0,r1,r2)
#  define ori(r0,r1,i0)			_ori(_jit,r0,r1,i0)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define xorr(r0,r1,r2)		XOR(r0,r1,r2)
#  define xori(r0,r1,i0)		_xori(_jit,r0,r1,i0)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define movr(r0,r1)			_movr(_jit,r0,r1)
static void _movr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi(r0,i0)			_movi(_jit,r0,i0)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#  define movi_p(r0,i0)			_movi_p(_jit,r0,i0)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#  define movnr(r0, r1, r2)		_movnr(_jit, r0, r1, r2)
static void _movnr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define movzr(r0, r1, r2)		_movzr(_jit, r0, r1, r2)
static void _movzr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define casx(r0, r1, r2, r3, i0)	_casx(_jit, r0, r1, r2, r3, i0)
static void _casx(jit_state_t *_jit,jit_int32_t,jit_int32_t,
		  jit_int32_t,jit_int32_t,jit_word_t);
#define casr(r0, r1, r2, r3)		casx(r0, r1, r2, r3, 0)
#define casi(r0, i0, r1, r2)		casx(r0, _NOREG, r1, r2, i0)
#  define ldr_c(r0,r1)			LB(r0,0,r1)
#  define ldi_c(r0,i0)			_ldi_c(_jit,r0,i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_uc(r0,r1)			LBU(r0,0,r1)
#  define ldi_uc(r0,i0)			_ldi_uc(_jit,r0,i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_s(r0,r1)			LH(r0,0,r1)
#  define ldi_s(r0,i0)			_ldi_s(_jit,r0,i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_us(r0,r1)			LHU(r0,0,r1)
#  define ldi_us(r0,i0)			_ldi_us(_jit,r0,i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_i(r0,r1)			LW(r0,0,r1)
#  define ldi_i(r0,i0)			_ldi_i(_jit,r0,i0)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 64
#    define ldr_ui(r0,r1)		LWU(r0,0,r1)
#    define ldi_ui(r0,i0)		_ldi_ui(_jit,r0,i0)
static void _ldi_ui(jit_state_t*,jit_int32_t,jit_word_t);
#    define ldr_l(r0,r1)		LD(r0,0,r1)
#    define ldi_l(r0,i0)		_ldi_l(_jit,r0,i0)
static void _ldi_l(jit_state_t*,jit_int32_t,jit_word_t);
#  endif
#  define ldxr_c(r0,r1,r2)		_ldxr_c(_jit,r0,r1,r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_c(r0,r1,i0)		_ldxi_c(_jit,r0,r1,i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_uc(r0,r1,r2)		_ldxr_uc(_jit,r0,r1,r2)
static void _ldxr_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_uc(r0,r1,i0)		_ldxi_uc(_jit,r0,r1,i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_s(r0,r1,r2)		_ldxr_s(_jit,r0,r1,r2)
static void _ldxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_s(r0,r1,i0)		_ldxi_s(_jit,r0,r1,i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_us(r0,r1,r2)		_ldxr_us(_jit,r0,r1,r2)
static void _ldxr_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_us(r0,r1,i0)		_ldxi_us(_jit,r0,r1,i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_i(r0,r1,r2)		_ldxr_i(_jit,r0,r1,r2)
static void _ldxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_i(r0,r1,i0)		_ldxi_i(_jit,r0,r1,i0)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  if __WORDSIZE == 64
#    define ldxr_ui(r0,r1,r2)		_ldxr_ui(_jit,r0,r1,r2)
static void _ldxr_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define ldxi_ui(r0,r1,i0)		_ldxi_ui(_jit,r0,r1,i0)
static void _ldxi_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#    define ldxr_l(r0,r1,r2)		_ldxr_l(_jit,r0,r1,r2)
static void _ldxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define ldxi_l(r0,r1,i0)		_ldxi_l(_jit,r0,r1,i0)
static void _ldxi_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  endif
#  define unldr(r0, r1, i0)		_unldr(_jit, r0, r1, i0)
static void _unldr(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define unldi(r0, i0, i1)		_unldi(_jit, r0, i0, i1)
static void _unldi(jit_state_t*, jit_int32_t, jit_word_t, jit_word_t);
#  define unldr_u(r0, r1, i0)		_unldr_u(_jit, r0, r1, i0)
static void _unldr_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define unldi_u(r0, i0, i1)		_unldi_u(_jit, r0, i0, i1)
static void _unldi_u(jit_state_t*, jit_int32_t, jit_word_t, jit_word_t);
#  define str_c(r0,r1)			SB(r1,0,r0)
#  define sti_c(i0,r0)			_sti_c(_jit,i0,r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_s(r0,r1)			SH(r1,0,r0)
#  define sti_s(i0,r0)			_sti_s(_jit,i0,r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_i(r0,r1)			SW(r1,0,r0)
#  define sti_i(i0,r0)			_sti_i(_jit,i0,r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#  if __WORDSIZE == 64
#    define str_l(r0,r1)		SD(r1,0,r0)
#    define sti_l(i0,r0)		_sti_l(_jit,i0,r0)
static void _sti_l(jit_state_t*,jit_word_t,jit_int32_t);
#  endif
#  define stxr_c(r0,r1,r2)		_stxr_c(_jit,r0,r1,r2)
static void _stxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_c(i0,r0,r1)		_stxi_c(_jit,i0,r0,r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_s(r0,r1,r2)		_stxr_s(_jit,r0,r1,r2)
static void _stxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_s(i0,r0,r1)		_stxi_s(_jit,i0,r0,r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_i(r0,r1,r2)		_stxr_i(_jit,r0,r1,r2)
static void _stxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_i(i0,r0,r1)		_stxi_i(_jit,i0,r0,r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  if __WORDSIZE == 64
#    define stxr_l(r0,r1,r2)		_stxr_l(_jit,r0,r1,r2)
static void _stxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#    define stxi_l(i0,r0,r1)		_stxi_l(_jit,i0,r0,r1)
static void _stxi_l(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  endif
#  define unstr(r0, r1, i0)		_unstr(_jit, r0, r1, i0)
static void _unstr(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define unsti(i0, r0, i1)		_unsti(_jit, i0, r0, i1)
static void _unsti(jit_state_t*, jit_word_t, jit_int32_t, jit_word_t);
#  define bswapr_us(r0,r1)		_bswapr_us(_jit,r0,r1)
static void _bswapr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define bswapr_ui(r0,r1)		_bswapr_ui(_jit,r0,r1)
static void _bswapr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  if __WORDSIZE == 64
#  define bswapr_ul(r0,r1)		_bswapr_ul(_jit,r0,r1)
static void _bswapr_ul(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#define extr(r0,r1,i0,i1)		_extr(_jit,r0,r1,i0,i1)
static void _extr(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t,jit_word_t);
#define extr_u(r0,r1,i0,i1)		_extr_u(_jit,r0,r1,i0,i1)
static void _extr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t,jit_word_t);
#define depr(r0,r1,i0,i1)		_depr(_jit,r0,r1,i0,i1)
static void _depr(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t,jit_word_t);
#  define extr_c(r0,r1)			_extr_c(_jit,r0,r1)
static void _extr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_uc(r0,r1)		ANDI(r0,r1,0xff)
#  define extr_s(r0,r1)			_extr_s(_jit,r0,r1)
static void _extr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_us(r0,r1)		ANDI(r0,r1,0xffff)
#  if __WORDSIZE == 64
#    define extr_i(r0,r1)		SLL(r0,r1,0)
#    define extr_ui(r0,r1)		_extr_ui(_jit,r0,r1)
static void _extr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  endif
#  define ltr(r0,r1,r2)			SLT(r0,r1,r2)
#  define lti(r0,r1,i0)			_lti(_jit,r0,r1,i0)
static void _lti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ltr_u(r0,r1,r2)		SLTU(r0,r1,r2)
#  define lti_u(r0,r1,i0)		_lti_u(_jit,r0,r1,i0)
static void _lti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ler(r0,r1,r2)			_ler(_jit,r0,r1,r2)
static void _ler(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lei(r0,r1,i0)			_lei(_jit,r0,r1,i0)
static void _lei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ler_u(r0,r1,r2)			_ler_u(_jit,r0,r1,r2)
static void _ler_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define lei_u(r0,r1,i0)			_lei_u(_jit,r0,r1,i0)
static void _lei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define eqr(r0,r1,r2)			_eqr(_jit,r0,r1,r2)
static void _eqr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define eqi(r0,r1,i0)			_eqi(_jit,r0,r1,i0)
static void _eqi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ger(r0,r1,r2)			_ger(_jit,r0,r1,r2)
static void _ger(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gei(r0,r1,i0)			_gei(_jit,r0,r1,i0)
static void _gei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ger_u(r0,r1,i0)			_ger_u(_jit,r0,r1,i0)
static void _ger_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define gei_u(r0,r1,i0)			_gei_u(_jit,r0,r1,i0)
static void _gei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define gtr(r0,r1,r2)			SLT(r0,r2,r1)
#define gti(r0,r1,i0)			_gti(_jit,r0,r1,i0)
static void _gti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define gtr_u(r0,r1,r2)		SLTU(r0,r2,r1)
#  define gti_u(r0,r1,i0)		_gti_u(_jit,r0,r1,i0)
static void _gti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define ner(r0,r1,r2)			_ner(_jit,r0,r1,r2)
static void _ner(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#define nei(r0,r1,i0)			_nei(_jit,r0,r1,i0)
static void _nei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#define bltr(i0,r0,r1)			bger(i0,r1,r0)
#define bltr_u(i0,r0,r1)		bger_u(i0,r1,r0)
#define blti(i0,r0,i1)			_bgei(_jit,i0,r0,i1,0,1)
#define blti_u(i0,r0,i1)		_bgei(_jit,i0,r0,i1,1,1)
#define bler(i0,r0,r1)			_bgtr(_jit,i0,r1,r0,0,1)
#define bler_u(i0,r0,r1)		_bgtr(_jit,i0,r1,r0,1,1)
#define blei(i0,r0,i1)			_bgti(_jit,i0,r0,i1,0,1)
#define blei_u(i0,r0,i1)		_bgti(_jit,i0,r0,i1,1,1)
#define beqr(i0,r0,r1)			_beqr(_jit,i0,r0,r1)
static jit_word_t _beqr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define beqi(i0,r0,i1)			_beqi(_jit,i0,r0,i1)
static jit_word_t _beqi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#define bger(i0,r0,r1)			_bger(_jit,i0,r0,r1,0)
#define bger_u(i0,r0,r1)		_bger(_jit,i0,r0,r1,1)
static jit_word_t _bger(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,
			jit_bool_t);
#define bgei(i0,r0,i1)			_bgei(_jit,i0,r0,i1,0,0)
#define bgei_u(i0,r0,i1)		_bgei(_jit,i0,r0,i1,1,0)
static jit_word_t _bgei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t,
			jit_bool_t,jit_bool_t);
#define bgtr(i0,r0,r1)			_bgtr(_jit,i0,r0,r1,0,0)
#define bgtr_u(i0,r0,r1)		_bgtr(_jit,i0,r0,r1,1,0)
static jit_word_t _bgtr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t,
			jit_bool_t,jit_bool_t);
#define bgti(i0,r0,i1)			_bgti(_jit,i0,r0,i1,0,0)
#define bgti_u(i0,r0,i1)		_bgti(_jit,i0,r0,i1,1,0)
static jit_word_t _bgti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t,
			jit_bool_t,jit_bool_t);
#define bner(i0,r0,r1)			_bner(_jit,i0,r0,r1)
static jit_word_t _bner(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#define bnei(i0,r0,i1)			_bnei(_jit,i0,r0,i1)
static jit_word_t _bnei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define jmpr(r0)			_jmpr(_jit,r0)
static void _jmpr(jit_state_t*,jit_int32_t);
#  define jmpi(i0,patch)		_jmpi(_jit,i0,patch)
static jit_word_t _jmpi(jit_state_t*,jit_word_t,jit_bool_t);
#  define jmpi_p(i0)			_jmpi_p(_jit,i0)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t);
#  define boaddr(i0,r0,r1)		_boaddr(_jit,i0,r0,r1)
static jit_word_t _boaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define boaddi(i0,r0,i1)		_boaddi(_jit,i0,r0,i1)
static jit_word_t _boaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr_u(i0,r0,r1)		_boaddr_u(_jit,i0,r0,r1)
static jit_word_t _boaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define boaddi_u(i0,r0,i1)		_boaddi_u(_jit,i0,r0,i1)
static jit_word_t _boaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxaddr(i0,r0,r1)		_bxaddr(_jit,i0,r0,r1)
static jit_word_t _bxaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxaddi(i0,r0,i1)		_bxaddi(_jit,i0,r0,i1)
static jit_word_t _bxaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxaddr_u(i0,r0,r1)		_bxaddr_u(_jit,i0,r0,r1)
static jit_word_t _bxaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxaddi_u(i0,r0,i1)		_bxaddi_u(_jit,i0,r0,i1)
static jit_word_t _bxaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr(i0,r0,r1)		_bosubr(_jit,i0,r0,r1)
static jit_word_t _bosubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bosubi(i0,r0,i1)		_bosubi(_jit,i0,r0,i1)
static jit_word_t _bosubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr_u(i0,r0,r1)		_bosubr_u(_jit,i0,r0,r1)
static jit_word_t _bosubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bosubi_u(i0,r0,i1)		_bosubi_u(_jit,i0,r0,i1)
static jit_word_t _bosubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxsubr(i0,r0,r1)		_bxsubr(_jit,i0,r0,r1)
static jit_word_t _bxsubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxsubi(i0,r0,i1)		_bxsubi(_jit,i0,r0,i1)
static jit_word_t _bxsubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxsubr_u(i0,r0,r1)		_bxsubr_u(_jit,i0,r0,r1)
static jit_word_t _bxsubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bxsubi_u(i0,r0,i1)		_bxsubi_u(_jit,i0,r0,i1)
static jit_word_t _bxsubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bmsr(i0,r0,r1)		_bmsr(_jit,i0,r0,r1)
static jit_word_t _bmsr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bmsi(i0,r0,i1)		_bmsi(_jit,i0,r0,i1)
static jit_word_t _bmsi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bmcr(i0,r0,r1)		_bmcr(_jit,i0,r0,r1)
static jit_word_t _bmcr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bmci(i0,r0,i1)		_bmci(_jit,i0,r0,i1)
static jit_word_t _bmci(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define callr(r0)			_callr(_jit,r0)
static void _callr(jit_state_t*,jit_int32_t);
#  define calli(i0,i1)			_calli(_jit,i0,i1)
static jit_word_t _calli(jit_state_t*,jit_word_t,jit_bool_t);
#  define calli_p(i0)			_calli_p(_jit,i0)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#  define prolog(node)			_prolog(_jit,node)
static void _prolog(jit_state_t*,jit_node_t*);
#  define epilog(node)			_epilog(_jit,node)
static void _epilog(jit_state_t*,jit_node_t*);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#define patch_abs(instr,label)		_patch_abs(_jit,instr,label)
static void _patch_abs(jit_state_t*,jit_word_t,jit_word_t);
#define patch_at(jump,label)		_patch_at(_jit,jump,label)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
/* definitions used by jit_get_reg_for_delay_slot() */
#include "jit_mips-fpu.c"
#endif

#if CODE
static void
_instr(jit_state_t *_jit, jit_int32_t op)
{
    if (_jitc->inst.pend)
	ii(_jitc->inst.op);
    else
	_jitc->inst.pend = 1;
    _jitc->inst.op = op;
}

static void
_flush(jit_state_t *_jit)
{
    if (_jitc->inst.pend) {
	ii(_jitc->inst.op);
	_jitc->inst.pend = 0;
    }
}

static jit_int32_t
_pending(jit_state_t *_jit)
{
    jit_int32_t		op;
    if (_jitc->inst.pend) {
	op = _jitc->inst.op;
	_jitc->inst.pend = 0;
    }
    else
	op = 0;
    return (op);
}

static void
_delay(jit_state_t *_jit, jit_int32_t op)
{
    assert(_jitc->inst.pend);
    ii(_jitc->inst.op);
    _jitc->inst.pend = 0;
    ii(op);
}

static jit_int32_t
_jit_get_reg_for_delay_slot(jit_state_t *_jit, jit_int32_t mask,
			    jit_int32_t reg0, jit_int32_t reg1)
{
    jit_instr_t		i;
    jit_int32_t		reg, r0, r1, r2/*, xreg*/, regs[3];
    /* If will emit a pending instruction */
    if (_jitc->inst.pend)
	i.op = _jitc->inst.op;
    /* Else if at least one instruction emited, check it */
    else if (_jit->pc.uc > _jit->code.ptr)
	i.op = _jit->pc.ui[-1];
    /* Else, a nop */
    else
	i.op = 0;
    regs[0] = regs[1] = regs[2]/* = xreg*/ = -1;
    switch (i.hc.b) {
	case MIPS_SPECIAL:		/* 00 */
	    switch (i.tc.b) {
		case MIPS_SLLV: 	/* 04 */
		case MIPS_SRLV: 	/* 06 */
		case MIPS_SRAV:		/* 07 */
		case MIPS_DSLLV:	/* 14 */
		case MIPS_DSRLV:	/* 16 */
		case MIPS_DSRAV:	/* 17 */
		case MIPS_ADDU: 	/* 21 */
		case MIPS_SUBU:		/* 23 */
		case MIPS_AND:		/* 24 */
		case MIPS_OR:		/* 25 */
		case MIPS_XOR:		/* 26 */
		case MIPS_NOR:		/* 27 */
		case MIPS_SLT:		/* 2a */
		case MIPS_SLTU:		/* 2b */
		case MIPS_DADDU:	/* 2d */
		case MIPS_DSUBU:	/* 2f */
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rt.b;
			regs[2] = i.rd.b;
		    }
		    break;
		    /* MUL MUH */
		case MIPS_MULT: 	/* 18 */
		    /* MULU MUHU */
		case MIPS_MULTU:	/* 19 */
		    /* DIV MOD */
		case MIPS_DIV:		/* 1a */
		    /* DIVU MODU */
		case MIPS_DIVU:		/* 1b */
		    /* DMUL DMUH */
		case MIPS_DMULT:	/* 1c */
		    /* DMULU DMUHU */
		case MIPS_DMULTU:	/* 1d */
		    /* DDIV DMOD */
		case MIPS_DDIV: 	/* 1e */
		    /* DDIVU DMODU */
		case MIPS_DDIVU:	/* 1f */
		    if (jit_mips6_p()) {
			assert(i.ic.b == 2 || i.ic.b == 3);
			if (mask & jit_class_gpr) {
			    regs[0] = i.rs.b;
			    regs[1] = i.rt.b;
			    regs[2] = i.rd.b;
			}
		    }
		    else {
			assert(i.rd.b == 0);
			if (mask & jit_class_gpr) {
			    regs[0] = i.rs.b;
			    regs[1] = i.rt.b;
			    regs[2] = 0;
			}
		    }
		    break;
		    /* CLZ */
		case MIPS_MFHI:		/* 10 */
		    /* CLO */
		case MIPS_MTHI:		/* 11 */
		    /* DCLZ */
		case MIPS_MFLO:		/* 12 */
		    /* DCLO */
		case MIPS_MTLO:		/* 13 */
		    if (mask & jit_class_gpr) {
			if (jit_mips6_p()) {
			    assert(i.ic.b == 1);
			    regs[1] = i.rd.b;
			}
			else {
			    assert(!i.rs.b && !i.rt.b);
			    regs[1] = 0;
			}
			regs[0] = i.rd.b;
			regs[1] = 0;
		    }
		    break;
		case MIPS_JR:		/* 08 */
		    assert(!jit_mips6_p());
		case MIPS_JALR:		/* 09 */
		    /* check for proper/known encondig */
		    assert(!i.ic.b);
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rt.b;
			regs[2] = i.rd.b;
		    }
		    break;
		case MIPS_SLL:		/* 00 */
		    /* If cannot have a shift in delay slot */
		    if (!jit_cpu.sll_delay)
			flush();
		case MIPS_SRL:		/* 02 */
		case MIPS_SRA:		/* 03 */
		case MIPS_DSLL:		/* 38 */
		case MIPS_DSRL:		/* 3a */
		case MIPS_DSRA:		/* 3b */
		case MIPS_DSLL32:	/* 3c */
		case MIPS_DSRA32:	/* 3f */
		case MIPS_DSRL32:	/* 3e */
		    /* shift (or rotate if i.rs.b == 1) */
		    assert(i.rs.b == 0 || i.rs.b == 1);
		    if (mask & jit_class_gpr) {
			regs[0] = i.rt.b;
			regs[1] = i.rd.b;
			regs[2] = 0;
		    }
		    break;
		case MIPS_SYNC:		/* 0f */
		    assert(i.rs.b == 0 && i.rt.b == 0 && i.rd.b == 0);
		    if (mask & jit_class_gpr)
			regs[0] = regs[1] = regs[1] = 0;
		    break;
		case MIPS_MOVZ:		/* 0a */
		case MIPS_MOVN:		/* 0b */
		    assert(!jit_mips6_p() && i.ic.b == 0);
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rt.b;
			regs[2] = i.rd.b;
		    }
		    break;
		/* SELEQZ */
		case 53:		/* 35 */
		/* SELNEZ */
		case 55:		/* 37 */
		    assert(jit_mips6_p() && i.ic.b == 0);
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rt.b;
			regs[2] = i.rd.b;
		    }
		    break;
		default:
		    abort();
	    }
	    break;
	case MIPS_REGIMM:		/* 01 */
	    switch (i.rt.b) {
		/* DAHI */
		case 6:			/* 06 */
		/* DATI */
		case 15:		/* 1e */
		    assert(jit_mips6_p());
		case MIPS_BLTZ:		/* 00 */
		case MIPS_BGEZ:		/* 01 */
		case MIPS_BGEZAL: 	/* 11 */
		    break;
		default:
		    abort();
	    }
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = regs[2] = 0;
	    }
	    break;
	case MIPS_BC_R6:		/* 32 */
	case MIPS_BALC:			/* 3a */
	    assert(jit_mips6_p());
	case MIPS_J:			/* 02 */
	case MIPS_JAL:			/* 03 */
	    if (mask & jit_class_gpr)
		regs[0] = regs[1] = regs[2] = 0;
	    break;
	case MIPS_LUI:			/* 0f */
	    assert(jit_mips6_p() || i.rs.b == 0);
	    if (mask & jit_class_gpr) {
		regs[0] = i.rt.b;
		regs[1] = i.rs.b;	/* AUI if non zero */
		regs[1] = 0;
	    }
	    break;
	case MIPS_SPECIAL2:		/* 1c */
	    switch (i.tc.b) {
		case MIPS_CLZ:		/* 20 */
		case MIPS_CLO:		/* 21 */
		case MIPS_DCLZ:		/* 24 */
		case MIPS_DCLO:		/* 25 */
		    assert(!jit_mips6_p() && i.rt.b == i.rd.b && i.ic.b == 0);
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rd.b;
			regs[2] = 0;
		    }
		    break;
		case MIPS_MUL:		/* 02 */
		    assert(jit_mips2_p() && i.ic.b == 0);
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rt.b;
			regs[2] = i.rd.b;
		    }
		    break;
		default:
		    abort();
	    }
	    break;
	case MIPS_SPECIAL3:		/* 1f */
	    switch (i.tc.b) {
		case MIPS_EXT:		/* 00 */
		case MIPS_DEXTM:	/* 01 */
		case MIPS_DEXTU:	/* 02 */
		case MIPS_DEXT:		/* 03 */
		case MIPS_INS:		/* 04 */
		case MIPS_DINSM:	/* 05 */
		case MIPS_DINSU:	/* 06 */
		case MIPS_DINS:		/* 07 */
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rt.b;
			regs[2] = 0;
		    }
		    break;
		/* BITSWAP */
		case MIPS_BSHFL:	/* 20 */
		/* DBITSWAP */
		case MIPS_DBSHFL:	/* 24 */
		    switch (i.ic.b) {
			/* DSBH */
			case MIPS_WSBH:	/* 02 */
			case MIPS_DSHD:	/* 05 */
			case MIPS_SEB:	/* 10 */
			case MIPS_SEH:	/* 18 */
			    if (mask & jit_class_gpr) {
				regs[0] = i.rt.b;
				regs[1] = i.rd.b;
				regs[2] = 0;
			    }
			    break;
			/* BITSWAP DBITSWAP */
			case 0:
			    assert(jit_mips6_p() && i.rt.b == 0);
			    if (mask & jit_class_gpr) {
				regs[0] = i.rs.b;
				regs[1] = i.rd.b;
				regs[2] = 0;
			    }
			    break;
			default:
			    abort();
		    }
		    break;
		/* SC */
		case 38:		/* 26 */
		/* SCD */
		case 39:		/* 27 */
		/* LD */
		case 54:		/* 36 */
		/* LLD */
		case 55:		/* 37 */
		    assert(jit_mips6_p());
		    if (mask & jit_class_gpr) {
			regs[0] = i.rs.b;
			regs[1] = i.rt.b;
			regs[2] = 0;
		    }
		    break;
		default:
		    abort();
	    }
	    break;
	case MIPS_COP1:			/* 11 */
	    switch (i.tc.b) {
		case MIPS_ADD_fmt:	/* 00 */
		    switch (i.rs.b) {
			case MIPS_MF:	/* 00 */
			case MIPS_DMF:	/* 01 */
			case MIPS_MFH:	/* 03 */
			case MIPS_MT:	/* 04 */
			case MIPS_DMT:	/* 05 */
			case MIPS_MTH:	/* 07 */
			    assert(i.ic.b == 0);
			    /* If these cop1 instructions in delay slot
			     * wont work  */
			    if (!jit_cpu.cop1_delay)
				flush();
			    if (mask & jit_class_gpr) {
				regs[0] = i.rt.b;
				regs[1] = regs[2] = 0;
			    }
			    else
				regs[0] = i.rd.b;
			    break;
			default:
			    goto three_fprs;
		    }
		    break;
		case MIPS_MADDF:	/* 18 */
		case MIPS_MSUBF:	/* 19 */
		    assert(jit_mips6_p());
		case MIPS_SUB_fmt:	/* 01 */
		case MIPS_MUL_fmt:	/* 02 */
		case MIPS_DIV_fmt:	/* 03 */
		three_fprs:
					/* 10 */
		    assert(i.rs.b == MIPS_fmt_S ||
					/* 11 */
			   i.rs.b == MIPS_fmt_D);
		    if (mask & jit_class_gpr)
			regs[0] = regs[1] = regs[2] = 0;
		    else {
			regs[0] = i.rt.b;
			regs[1] = i.rd.b;
			regs[2] = i.ic.b;
		    }
		    break;
		case MIPS_SQRT_fmt:	/* 04 */
		case MIPS_ABS_fmt:	/* 05 */
		case MIPS_MOV_fmt:	/* 06 */
		case MIPS_NEG_fmt:	/* 07 */
		    assert((i.rs.b == MIPS_fmt_S || i.rs.b == MIPS_fmt_D) &&
			   i.rt.b == 0);
		    if (mask & jit_class_gpr)
			regs[0] = regs[1] = regs[2] = 0;
		    else {
			regs[0] = i.rd.b;
			regs[1] = i.ic.b;
		    }
		    break;
		case MIPS_CVT_fmt_S:	/* 20 */
		case MIPS_CVT_fmt_D:	/* 21 */
		case MIPS_CVT_fmt_W:	/* 24 */
		case MIPS_CVT_fmt_L:	/* 25 */
		    switch (i.rs.b) {
			case MIPS_fmt_S:/* 10 */
			case MIPS_fmt_D:/* 11 */
			case MIPS_fmt_W:/* 14 */
			case MIPS_fmt_L:/* 15 */
			    break;
			default:
			    abort();
		    }
		    assert(i.rt.b == 0);
		    if (mask & jit_class_gpr)
			regs[0] = regs[1] = regs[2] = 0;
		    else {
			regs[0] = i.rd.b;
			regs[1] = i.ic.b;
		    }
		    break;
		case MIPS_cond_F:	/* 30 */
		case MIPS_cond_UN:	/* 31 */
		case MIPS_cond_EQ:	/* 32 */
		case MIPS_cond_UEQ:	/* 33 */
		case MIPS_cond_OLT:	/* 34 */
		case MIPS_cond_ULT:	/* 35 */
		case MIPS_cond_OLE:	/* 36 */
		case MIPS_cond_ULE:	/* 37 */
		case MIPS_cond_SF:	/* 38 */
		case MIPS_cond_NGLE:	/* 39 */
		case MIPS_cond_SEQ:	/* 3a */
		case MIPS_cond_NGL:	/* 3b */
		case MIPS_cond_LT:	/* 3c */
		case MIPS_cond_NGE:	/* 3d */
		case MIPS_cond_LE:	/* 3e */
		case MIPS_cond_UGT:	/* 3f */
		    assert(!jit_mips6_p() &&
					/* 10 */
			   (i.fm.b == MIPS_fmt_S ||
					/* 11 */
			    i.fm.b == MIPS_fmt_D));
		    if (mask & jit_class_gpr)
			regs[0] = regs[1] = regs[2] = 0;
		    else {
			regs[0] = i.ft.b;
			regs[1] = i.fs.b;
		    }
		    break;
		default:
		    switch (i.ic.b) {
			case MIPS_cmp_AF:  /* 00 */
			case MIPS_cmp_UN:  /* 01 */
			case MIPS_cmp_EQ:  /* 02 */
			case MIPS_cmp_UEQ: /* 03 */
			case MIPS_cmp_LT:  /* 04 */
			case MIPS_cmp_ULT: /* 05 */
			case MIPS_cmp_LE:  /* 06 */
			case MIPS_cmp_ULE: /* 07 */
			case MIPS_cmp_SAF: /* 08 */
			case MIPS_cmp_SUN: /* 09 */
			case MIPS_cmp_SEQ: /* 0a */
			case MIPS_cmp_SUEQ:/* 0b */
			case MIPS_cmp_SLT: /* 0c */
			case MIPS_cmp_SULT:/* 0d */
			case MIPS_cmp_SLE: /* 0e */
			case MIPS_cmp_SULE:/* 0f */
			    assert(jit_mips6_p() &&
					   /* 14 */
				   (i.rs.b == MIPS_condn_S ||
					   /* 15 */
				    i.rs.b == MIPS_condn_D));
			    if (mask & jit_class_gpr)
				regs[0] = regs[1] = regs[2] = 0;
			    else {
				regs[0] = i.ft.b;
				regs[1] = i.fs.b;
				regs[2] = i.fd.b;
			    }
			    goto done;
			default:
			    break;
		    }
		    switch (i.rt.b) {
			case MIPS_BC:	/* 08 */
			    assert(!jit_mips6_p() &&
					/* 00 */
				   (i.rs.b == MIPS_BCF ||
					/* 01 */
				    i.rs.b == MIPS_BCT));
			    if (mask & jit_class_gpr)
				regs[0] = regs[1] = regs[2] = 0;
			    else {
				regs[0] = i.rt.b;
				regs[1] = i.rd.b;
			    }
			    break;
			case MIPS_BC1EQZ:/* 09 */
			case MIPS_BC1NEZ:/* 0a */
			    assert(jit_mips6_p());
			    if (mask & jit_class_gpr)
				regs[0] = regs[1] = regs[2] = 0;
			    else
				regs[0] = i.rt.b;
			    break;
			default:
			    abort();
		    }
		    break;
	    }
	    break;
	case MIPS_COP1X:		/* 13 */
	    switch (i.tc.b) {
		case MIPS_MADD_fmt_S:
		case MIPS_MADD_fmt_D:
		case MIPS_MSUB_fmt_S:
		case MIPS_MSUB_fmt_D:
		case MIPS_NMADD_fmt_S:
		case MIPS_NMADD_fmt_D:
		case MIPS_NMSUB_fmt_S:
		case MIPS_NMSUB_fmt_D:
		    assert(!jit_mips6_p());
		    if (mask & jit_class_gpr)
			regs[0] = regs[1] = regs[2] = 0;
		    else {
			regs[0] = i.ft.b;
			regs[1] = i.fs.b;
			regs[2] = i.fd.b;
			/* FIXME No need to compute and check it.
			 * If asking for a tmeporary fpr, code will
			 * be flushed. */
			/* xreg = i.fr.b; */
		    }
		    break;
		default:
		    abort();
	    }
	    break;
	case MIPS_DAUI:	/* JALX */	/* 1d */
	    /* Do not generate JALX. No microMIPS64 or MIPS16e support */
	    assert(jit_mips6_p() && i.rs.b != 0);
	case MIPS_ADDIU:		/* 09 */
	case MIPS_SLTI: 		/* 0a */
	case MIPS_SLTIU:		/* 0b */
	case MIPS_ANDI:			/* 0c */
	case MIPS_ORI:			/* 0d */
	case MIPS_XORI: 		/* 0e */
	case MIPS_DADDIU:		/* 18 */
	case MIPS_LDL:			/* 1a */
	case MIPS_LDR:			/* 1b */
	case MIPS_LB:			/* 20 */
	case MIPS_LH:			/* 21 */
	case MIPS_LW:			/* 23 */
	case MIPS_LBU:			/* 24 */
	case MIPS_LHU:			/* 25 */
	case MIPS_LWU:			/* 27 */
	case MIPS_SB:			/* 28 */
	case MIPS_SH:			// 29 */
	case MIPS_SW:			/* 2b */
	case MIPS_LD:			/* 37 */
	case MIPS_SD:			/* 3f */
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = i.rt.b;
		regs[2] = 0;
	    }
	    break;
	case MIPS_LWL:			/* 22 */
	case MIPS_LWR:			/* 26 */
	    if (!jit_cpu.lwl_lwr_delay)
		flush();
	case MIPS_SWL:			/* 2a */
	case MIPS_SWR:			/* 2e */
	case MIPS_SDL:			/* 2c */
	case MIPS_SDR:			/* 2d */
	    assert(!(jit_mips6_p()));
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = i.rt.b;
		regs[2] = 0;
	    }
	    break;
	case MIPS_LL:			/* 30 */
	case MIPS_LLD:			/* 34 */
	case MIPS_SC:			/* 38 */
	case MIPS_SCD:			/* 3c */
	    assert(!jit_mips6_p() && i.ic.b == 0);
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = i.rt.b;
		regs[2] = 0;
	    }
	    break;
	case MIPS_BLEZ:			/* 06 */
	case MIPS_BGTZ:			/* 07 */
	    assert(i.rt.b == 0);
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = regs[2] = 0;
	    }
	    break;
	case MIPS_LWC1:			/* 31 */
	case MIPS_LDC1:			/* 35 */
	case MIPS_SWC1:			/* 39 */
	case MIPS_SDC1:			/* 3d */
	    /* If these cop1 instructions in delay wont not work  */
	    if (!jit_cpu.cop1_delay)
		flush();
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = i.rt.b;
		regs[2] = 0;
	    }
	    else
		regs[0] = i.rt.b;
	    break;
	case MIPS_BEQ:			/* 04 */
	case MIPS_BNE:			/* 05 */
	    assert(i.rt.b == 0);
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = i.rt.b;
		regs[2] = 0;
	    }
	    else
		regs[0] = i.rt.b;
	    break;
	case MIPS_PCREL:		/* 0x3b */
	    assert(jit_mips6_p());
	    switch (i.rt.b) {
		case 0x1e:		/* AUIPC */
		case 0x1f:		/* ALUIPC */
		    break;
		default:
		    assert(i.pD.b == 1 ||/* LDPC */
			   i.pW.b == 0 ||/* ADDIUPC */
			   i.pW.b == 1 ||/* LWPC */
			   i.pW.b == 2); /* LWUPC */
		    break;
	    }
	    if (mask & jit_class_gpr) {
		regs[0] = i.rs.b;
		regs[1] = regs[2] = 0;
	    }
	    break;
	default:
	    abort();
    }
done:
    /* If cannot move instruction do delay slot */
    if (_jitc->inst.pend &&
	(((mask & jit_class_fpr) || reg0) &&
	 (reg0 == regs[0] || reg0 == regs[1] || reg0 == regs[2])) ||
	(((mask & jit_class_fpr) || reg1) &&
	 (reg1 == regs[0] || reg1 == regs[1] || reg1 == regs[2]))) {
	flush();
    }
    /* Get a temporary register */
retry:
    reg = jit_get_reg(mask|jit_class_nospill);
    /* Make sure will not use a register in use by delay slot */
    if (_jitc->inst.pend) {
	if (rn(reg) == regs[0] ||
	    rn(reg) == regs[1] || rn(reg) == regs[2]) {
	    r0 = reg;
	    reg = jit_get_reg(mask|jit_class_nospill);
	    if (rn(reg) == regs[0] ||
		rn(reg) == regs[1] || rn(reg) == regs[2]) {
		r1 = reg;
		reg = jit_get_reg(mask|jit_class_nospill);
		if (rn(reg) == regs[0] ||
		    rn(reg) == regs[1] || rn(reg) == regs[2]) {
		    r2 = reg;
		    reg = jit_get_reg(mask|jit_class_nospill);
		    jit_unget_reg(r2);
		}
		jit_unget_reg(r1);
	    }
	    jit_unget_reg(r0);
	}
    }
    if (reg == JIT_NOREG) {
	/* Cannot get a register to optimize delay slot */
	flush();
	/* Must find a free register */
	if (!(mask & jit_class_chk))
	    goto retry;
    }
    assert(reg != JIT_NOREG || (mask & jit_class_chk));
    return (reg);
}

static void
_hrrrit(jit_state_t *_jit,jit_int32_t hc,
	jit_int32_t rs, jit_int32_t rt, jit_int32_t rd,
	jit_int32_t ic, jit_int32_t tc)
{
    jit_instr_t		i;
    i.tc.b = tc;
    i.ic.b = ic;
    i.rd.b = rd;
    i.rt.b = rt;
    i.rs.b = rs;
    i.hc.b = hc;
    instr(i.op);
}

static void
_hrri(jit_state_t *_jit, jit_int32_t hc,
      jit_int32_t rs, jit_int32_t rt, jit_int32_t im)
{
    jit_instr_t		i;
    i.op = 0;
    i.is.b = im;
    i.rt.b = rt;
    i.rs.b = rs;
    i.hc.b = hc;
    instr(i.op);
}

static void
_hrri9(jit_state_t *_jit, jit_int32_t hc,
      jit_int32_t rs, jit_int32_t rt, jit_int32_t i9, jit_int32_t tc)
{
    jit_instr_t		i;
    i.op = 0;
    i.tc.b = tc;
    i.i9.b = i9;
    i.rt.b = rt;
    i.rs.b = rs;
    i.hc.b = hc;
    instr(i.op);
}

static void
_hriD(jit_state_t *_jit, jit_int32_t hc,
       jit_int32_t rs, jit_int32_t pD, jit_int32_t iD)
{
    jit_instr_t		i;
    i.iD.b = iD;
    i.pD.b = pD;
    i.rs.b = rs;
    i.hc.b = hc;
    instr(i.op);
}

static void
_hriW(jit_state_t *_jit, jit_int32_t hc,
       jit_int32_t rs, jit_int32_t pW, jit_int32_t iW)
{
    jit_instr_t		i;
    i.iW.b = iW;
    i.pD.b = pW;
    i.rs.b = rs;
    i.hc.b = hc;
    instr(i.op);
}

static void
_hi(jit_state_t *_jit, jit_int32_t hc, jit_int32_t im)
{
    jit_instr_t		i;
    i.ii.b = im;
    i.hc.b = hc;
    instr(i.op);
}

static void
_nop(jit_state_t *_jit, jit_int32_t i0)
{
    for (; i0 > 0; i0 -= 4)
	NOP();
    assert(i0 == 0);
}

static void
_mips_extr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
      jit_int32_t pos, jit_int32_t size)
{
    assert(size > 0);

    if (__WORDSIZE == 32)
        EXT(r0, r1, pos, size);
    else if (pos >= 32)
        DEXTU(r0, r1, pos, size);
    else if (size > 32)
        DEXTM(r0, r1, pos, size);
    else
        DEXT(r0, r1, pos, size);
}

static void
_insr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
      jit_int32_t pos, jit_int32_t size)
{
    assert(size > 0);

    if (__WORDSIZE == 32)
        INS(r0, r1, pos, size);
    else if (pos >= 32)
        DINSU(r0, r1, pos, size);
    else if (size > 32)
        DINSM(r0, r1, pos, size);
    else
        DINS(r0, r1, pos, size);
}

static void
_clor(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if __WORDSIZE == 32
    if (jit_mips6_p())
	CLO_R6(r0, r1);
    else if (jit_mips2_p())
	CLO(r0, r1);
    else
	fallback_clo(r0, r1);
#else
    assert(jit_mips2_p());
    if (jit_mips6_p())
	DCLO_R6(r0, r1);
    else
	DCLO(r0, r1);
#endif
}

static void
_clzr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
#if __WORDSIZE == 32
    if (jit_mips6_p())
	CLZ_R6(r0, r1);
    else if (jit_mips2_p())
	CLZ(r0, r1);
    else
	fallback_clz(r0, r1);
#else
    assert(jit_mips2_p());
    if (jit_mips6_p())
	DCLZ_R6(r0, r1);
    else
	DCLZ(r0, r1);
#endif
}

static void
_ctor(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips6_p()) {
        rbitr(r0, r1);
        clor(r0, r0);
    }
    else {
        comr(r0, r1);
        ctzr(r0, r0);
    }
}

static void
_ctzr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips6_p()) {
        rbitr(r0, r1);
        clzr(r0, r0);
    }
    else {
        jit_int32_t     t0, t1;

        t0 = jit_get_reg(jit_class_gpr);
        t1 = jit_get_reg(jit_class_gpr);

        negr(rn(t0), r1);
        andr(rn(t0), rn(t0), r1);
        clzr(r0, rn(t0));
        xori(rn(t1), r0, __WORDSIZE - 1);
        movnr(r0, rn(t1), rn(t0));

        jit_unget_reg(t0);
        jit_unget_reg(t1);
    }
}

static void
_rbitr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips6_p()) {
#if __WORDSIZE == 32
	BITSWAP(r0, r1);
	bswapr_ui(r0, r0);
#else
	DBITSWAP(r0, r1);
	bswapr_ul(r0, r0);
#endif
    }
    else
	fallback_rbit(r0, r1);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 == 0)
	movr(r0, r1);
    else if (can_sign_extend_short_p(i0))
	addiu(r0, r1, i0);
    else {
	if (jit_mips6_p()) {
	    union {
		struct {
#  if __BYTE_ORDER == __LITTLE_ENDIAN
		    jit_word_t	_ :	16;
		    jit_word_t	aui :	16;
#   if __WORDSIZE == 64
		    jit_word_t	ahi :	16;
		    jit_word_t	ati :	16;
#   endif
#  else
#   if __WORDSIZE == 64
		    jit_word_t	ati :	16;
		    jit_word_t	ahi :	16;
#   endif
		    jit_word_t	aui :	16;
		    jit_word_t	_ :	16;
#  endif
		} b;
		jit_word_t		w;
	    } bits;
	    bits.w = i0;
	    if (r0 == r1 && ((jit_word_t)bits.b.aui << 16) == i0)
		/* FIXME It should not be required r0 == r1 per
		 * documentation, but this is now it works in qemu
		 * for DAUI. Assume AUI has the same restriction. */
		DAUI(r1, r0, bits.b.aui & 0xffff);
#if __WORDSIZE == 64
	    else if (r0 == r1 && ((jit_word_t)bits.b.ahi << 32) == i0)
		DAHI(r0, bits.b.ahi & 0xffff);
	    else if (r0 == r1 && ((jit_word_t)bits.b.ati << 48) == i0)
		DATI(r0, bits.b.ati & 0xffff);
#endif
	    else
		goto fallback;
	    goto done;
	}
    fallback:
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
done:;
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
	if (can_sign_extend_short_p(i0))
	    addiu(rn(t0), r1, i0);
	else {
	    movi(rn(t0), i0);
	    addr(rn(t0), r1, rn(t0));
	}
	SLTU(rn(jit_carry), rn(t0), r1);
	movr(r0, rn(t0));
    }
    else {
	if (can_sign_extend_short_p(i0))
	    addiu(r0, r1, i0);
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
    else if (can_sign_extend_short_p(i0) && (i0 & 0xffff) != 0x8000)
	addiu(r0, r1, -i0);
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
	if (can_sign_extend_short_p(i0) && (i0 & 0xffff) != 0x8000)
	    addiu(rn(t0), r1, -i0);
	else {
	    movi(rn(t0), i0);
	    subr(rn(t0), r1, rn(t0));
	}
	SLTU(rn(jit_carry), r1, rn(t0));
	movr(r0, rn(t0));
    }
    else {
	if (can_sign_extend_short_p(i0) && (i0 & 0xffff) != 0x8000)
	    addiu(r0, r1, -i0);
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
_mulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips6_p())
	mul_r6(r0, r1, r2);
    else {
	if (jit_mips2_p() && __WORDSIZE == 32)
	    MUL(r0, r1, r2);
	else {
	    multu(r1, r2);
	    MFLO(r0);
	}
    }
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
    if (jit_mips6_p()) {
	if (r0 == r2 || r0 == r3) {
	    t0 = jit_get_reg(jit_class_gpr);
	    if (sign)
		mul_r6(rn(t0), r2, r3);
	    else
		mulu_r6(rn(t0), r2, r3);
	}
	else {
	    if (sign)
		mul_r6(r0, r2, r3);
	    else
		mulu_r6(r0, r2, r3);
	}
	if (sign)
	    muh_r6(r1, r2, r3);
	else
	    muhu_r6(r1, r2, r3);
	if (r0 == r2 || r0 == r3) {
	    movr(r0, rn(t0));
	    jit_unget_reg(t0);
	}
    }
    else {
	if (sign)
	    mult(r2, r3);
	else
	    multu(r2, r3);
	MFLO(r0);
	MFHI(r1);
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
_divr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips6_p())
	div_r6(r0, r1, r2);
    else {
	div(r1, r2);
	MFLO(r0);
    }
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
_divr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips6_p())
	divu_r6(r0, r1, r2);
    else {
	divu(r1, r2);
	MFLO(r0);
    }
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
    if (jit_mips6_p()) {
	if (r0 == r2 || r0 == r3)
	    t0 = jit_get_reg(jit_class_gpr);
	else
	    t0 = _NOREG;
	if (sign) {
	    if (t0 == _NOREG)
		div_r6(r0, r2, r3);
	    else
		div_r6(rn(t0), r2, r3);
	    mod_r6(r1, r2, r3);
	}
	else {
	    if (t0 == _NOREG)
		divu_r6(r0, r2, r3);
	    else
		divu_r6(rn(t0), r2, r3);
	    modu_r6(r1, r2, r3);
	}
	if (t0 != _NOREG) {
	    movr(r0, rn(t0));
	    jit_unget_reg(t0);
	}
    }
    else {
	if (sign)
	    div(r2, r3);
	else
	    divu(r2, r3);
	MFLO(r0);
	MFHI(r1);
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
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips6_p())
	mod_r6(r0, r1, r2);
    else {
	div(r1, r2);
	MFHI(r0);
    }
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
_remr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips6_p())
	modu_r6(r0, r1, r2);
    else {
	divu(r1, r2);
	MFHI(r0);
    }
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

#if __WORDSIZE == 64
static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 <= 63);
    if (i0 < 32)
	DSLL(r0, r1, i0);
    else
	DSLL32(r0, r1, i0 - 32);
}

static void
_rshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 <= 63);
    if (i0 < 32)
	DSRA(r0, r1, i0);
    else
	DSRA32(r0, r1, i0 - 32);
}

static void
_rshi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 <= 63);
    if (i0 < 32)
	DSRL(r0, r1, i0);
    else
	DSRL32(r0, r1, i0 - 32);
}
#endif

static void
_xlshr(jit_state_t *_jit, jit_bool_t sign,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_bool_t		branch;
    jit_word_t		over, zero, done, done_over;
    jit_int32_t		t0, s0, t1, s1, t2, s2, t3, s3;
    s0 = jit_get_reg(jit_class_gpr);
    t0 = rn(s0);
    if (r0 == r2 || r1 == r2) {
	s2 = jit_get_reg(jit_class_gpr);
	t2 = rn(s2);
	movr(t2, r2);
    }
    else
	t2 = r2;
    if (r0 == r3 || r1 == r3) {
	s3 = jit_get_reg(jit_class_gpr);
	t3 = rn(s3);
	movr(t3, r3);
    }
    else
	t3 = r3;
    if ((s1 = jit_get_reg(jit_class_gpr|jit_class_nospill|jit_class_chk))) {
	t1 = rn(s1);
	branch = 0;
    }
    else
	branch = 1;
    rsbi(t0, t3, __WORDSIZE);
    lshr(r0, t2, t3);
    if (sign)
	rshr(r1, t2, t0);
    else
	rshr_u(r1, t2, t0);
    if (branch) {
	zero = beqi(_jit->pc.w, t3, 0);
	over = beqi(_jit->pc.w, t3, __WORDSIZE);
	done = jmpi(_jit->pc.w, 1);
	flush();
	patch_at(over, _jit->pc.w);
	/* overflow */
	movi(r0, 0);
	done_over = jmpi(_jit->pc.w, 1);
	/* zero */
	flush();
	patch_at(zero, _jit->pc.w);
	if (sign)
	    rshi(r1, t2, __WORDSIZE - 1);
	else
	    movi(r1, 0);
	flush();
	patch_at(done, _jit->pc.w);
	patch_at(done_over, _jit->pc.w);
    }
    else {
	if (sign)
	    rshi(t0, t2, __WORDSIZE - 1);
	else
	    movi(t0, 0);
	/* zero? */
	movzr(r1, t0, t3);
	/* Branchless but 4 bytes longer than branching fallback */
	if (sign)
	    movi(t0, 0);
	/* overflow? */
	eqi(t1, t3, __WORDSIZE);
	movnr(r0, t0, t1);
	jit_unget_reg(s1);
    }
    jit_unget_reg(s0);
    if (t2 != r2)
	jit_unget_reg(s2);
    if (t3 != r3)
	jit_unget_reg(s3);
}

static void
_xlshi(jit_state_t *_jit, jit_bool_t sign,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    if (i0 == 0) {
	movr(r0, r2);
	if (sign)
	    rshi(r1, r2, __WORDSIZE - 1);
	else
	    movi(r1, 0);
    }
    else if (i0 == __WORDSIZE) {
	movr(r1, r2);
	movi(r0, 0);
    }
    else {
	assert((jit_uword_t)i0 <= __WORDSIZE);
	if (sign)
	    rshi(r1, r2, __WORDSIZE - i0);
	else
	    rshi_u(r1, r2, __WORDSIZE - i0);
	lshi(r0, r2, i0);
    }
}

static void
_xrshr(jit_state_t *_jit, jit_bool_t sign,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_bool_t		branch;
    jit_word_t		over, zero, done, done_over;
    jit_int32_t		t0, s0, t1, s1, t2, s2, t3, s3;
    s0 = jit_get_reg(jit_class_gpr);
    t0 = rn(s0);
    if (r0 == r2 || r1 == r2) {
	s2 = jit_get_reg(jit_class_gpr);
	t2 = rn(s2);
	movr(t2, r2);
    }
    else
	t2 = r2;
    if (r0 == r3 || r1 == r3) {
	s3 = jit_get_reg(jit_class_gpr);
	t3 = rn(s3);
	movr(t3, r3);
    }
    else
	t3 = r3;
    if ((s1 = jit_get_reg(jit_class_gpr|jit_class_nospill|jit_class_chk))) {
	t1 = rn(s1);
	branch = 0;
    }
    else
	branch = 1;
    rsbi(t0, t3, __WORDSIZE);
    if (sign)
	rshr(r0, t2, t3);
    else
	rshr_u(r0, t2, t3);
    lshr(r1, t2, t0);
    if (branch) {
	zero = beqi(_jit->pc.w, t3, 0);
	over = beqi(_jit->pc.w, t3, __WORDSIZE);
	done = jmpi(_jit->pc.w, 1);
	flush();
	patch_at(over, _jit->pc.w);
	/* underflow */
	if (sign)
	    rshi(r0, t2, __WORDSIZE - 1);
	else
	    movi(r0, 0);
	done_over = jmpi(_jit->pc.w, 1);
	/* zero */
	flush();
	patch_at(zero, _jit->pc.w);
	if (sign)
	    rshi(r1, t2, __WORDSIZE - 1);
	else
	    movi(r1, 0);
	flush();
	patch_at(done, _jit->pc.w);
	patch_at(done_over, _jit->pc.w);
	jit_unget_reg(s1);
    }
    else {
	/* zero? */
	if (sign)
	    rshi(t0, t2, __WORDSIZE - 1);
	else
	    movi(t0, 0);
	movzr(r1, t0, t3);
	/* underflow? */
	eqi(t1, t3, __WORDSIZE);
	movnr(r0, t0, t1);
	jit_unget_reg(s1);
    }
    jit_unget_reg(s0);
    if (t2 != r2)
	jit_unget_reg(s2);
    if (t3 != r3)
	jit_unget_reg(s3);
}

static void
_xrshi(jit_state_t *_jit, jit_bool_t sign,
       jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    if (i0 == 0) {
	movr(r0, r2);
	if (sign)
	    rshi(r1, r2, __WORDSIZE - 1);
	else
	    movi(r1, 0);
    }
    else if (i0 == __WORDSIZE) {
	movr(r1, r2);
	if (sign)
	    rshi(r0, r2, __WORDSIZE - 1);
	else
	    movi(r0, 0);
    }
    else {
	assert((jit_uword_t)i0 <= __WORDSIZE);
	lshi(r1, r2, __WORDSIZE - i0);
	if (sign)
	    rshi(r0, r2, i0);
	else
	    rshi_u(r0, r2, i0);
    }
}

static void
_lrotr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_mips2_p()) {
	if (r0 != r1 && r0 != r2) {
	    rsbi(r0, r2, __WORDSIZE);
	    rrotr(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    rsbi(rn(reg), r2, __WORDSIZE);
	    rrotr(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else
	fallback_lrotr(r0, r1, r2);
}

static void
_rrotr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_mips2_p()) {
#if __WORDSIZE == 32
	ROTRV(r0, r1, r2);
#else
	DROTRV(r0, r1, r2);
#endif
    }
    else
	fallback_rrotr(r0, r1, r2);
}

static void
_rroti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 <= __WORDSIZE - 1);
    if (jit_mips2_p()) {
#if __WORDSIZE == 32
	ROTR(r0, r1, i0);
#else
	if (i0 < 32)
	    DROTR(r0, r1, i0);
	else
	    DROTR32(r0, r1, i0 - 32);
#endif
    }
    else
	fallback_rroti(r0, r1, i0);
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_zero_extend_short_p(i0))
        ANDI(r0, r1, i0);
    else if (is_low_mask(i0)) {
        if (jit_mips2_p())
            mips_extr(r0, r1, 0, masked_bits_count(i0));
        else {
            lshi(r0, r1, unmasked_bits_count(i0));
            rshi_u(r0, r0, unmasked_bits_count(i0));
        }
    } else if (is_high_mask(i0)) {
        if (jit_mips2_p() && r0 == r1)
            insr(r0, _ZERO_REGNO, 0, unmasked_bits_count(i0));
        else {
            rshi(r0, r1, unmasked_bits_count(i0));
            lshi(r0, r0, unmasked_bits_count(i0));
        }
    } else if (jit_mips2_p() && is_middle_mask(i0)) {
        mips_extr(r0, r1, __builtin_ctzl(i0), masked_bits_count(i0));
        lshi(r0, r0, __builtin_ctzl(i0));
    } else if (jit_mips2_p() && is_middle_mask(~i0)) {
        if (r0 != r1)
            movr(r0, r1);
        insr(r0, _ZERO_REGNO, __builtin_ctzl(~i0), masked_bits_count(~i0));
    } else {
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
    if (can_zero_extend_short_p(i0))
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
    if (can_zero_extend_short_p(i0))
	XORI(r0, r1, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	XOR(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	orr(r0, r1, _ZERO_REGNO);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    if (i0 == 0)
	OR(r0, _ZERO_REGNO, _ZERO_REGNO);
    else if (can_sign_extend_short_p(i0))
	addiu(r0, _ZERO_REGNO, i0);
    else if (can_zero_extend_short_p(i0))
	ORI(r0, _ZERO_REGNO, i0);
    else {
	/* Check if loading some constant reachable address */
	if (jit_mips6_p()) {
	    jit_word_t		w, d;
	    w = i0 - (_jit->pc.w + (_jitc->inst.pend ? 4 : 0));
#if !PCREL_BROKEN
	    if (!(i0 & 3)) {
		d = w >> 2;
		if (can_sign_extend_i19_p(d)) {
		    ADDIUPC(r0, d);
		    goto done;
		}
	    }
#endif
	    if (can_sign_extend_int_p(w)) {
		jit_int32_t	lo = (jit_int32_t)w << 16 >> 16;
		jit_int32_t	hi = w - lo;
		AUIPC(r0, hi >> 16);
		if (lo)
		    addiu(r0, r0, lo);
		goto done;
	    }
	}
	if (can_sign_extend_int_p(i0))
	    LUI(r0, i0 >> 16);
	else if (can_zero_extend_int_p(i0)) {
	    if (i0 & 0xffff0000) {
		ORI(r0, _ZERO_REGNO, i0 >> 16);
		lshi(r0, r0, 16);
	    }
	}
#  if __WORDSIZE == 64
	else {
	    movi(r0, (jit_uword_t)i0 >> 32);
	    if (i0 & 0xffff0000) {
		lshi(r0, r0, 16);
		ORI(r0, r0, i0 >> 16);
		lshi(r0, r0, 16);
	    }
	    else
		lshi(r0, r0, 32);
	}
#  endif
	if (i0 & 0xffff)
	    ORI(r0, r0, i0);
    }
done:;
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    flush();
    w = _jit->pc.w;
#  if __WORDSIZE == 32
    LUI(r0, i0 >> 16);
    ORI(r0, r0, i0);
#  else
    LUI(r0, i0 >> 48);
    ORI(r0, r0, i0 >> 32);
    lshi(r0, r0, 16);
    ORI(r0, r0, i0 >> 16);
    lshi(r0, r0, 16);
    ORI(r0, r0, i0);
#  endif

    return (w);
}

static void
_movnr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_gpr);
	SELNEZ(rn(reg), r1, r2);
	SELEQZ(r0, r0, r2);
	OR(r0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	MOVN(r0, r1, r2);
}

static void
_movzr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	reg = jit_get_reg(jit_class_gpr);
	SELEQZ(rn(reg), r1, r2);
	SELNEZ(r0, r0, r2);
	OR(r0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    else
	MOVZ(r0, r1, r2);
}

static void
_casx(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
      jit_int32_t r2, jit_int32_t r3, jit_word_t i0)
{
    jit_int32_t		r1_reg, iscasi;
    jit_word_t		retry, done, jump0, jump1;
    if ((iscasi = (r1 == _NOREG))) {
	r1_reg = jit_get_reg(jit_class_gpr);
	r1 = rn(r1_reg);
	movi(r1, i0);
    }
    SYNC();
    /* retry: */
    flush();
    retry = _jit->pc.w;
#  if __WORDSIZE == 32
    if (jit_mips6_p())	LL_R6(r0, 0, r1);
    else		LL(r0, 0, r1);
#  else
    if (jit_mips6_p())	LLD_R6(r0, 0, r1);
    else		LLD(r0, 0, r1);
#  endif
    flush();
    jump0 = _jit->pc.w;
    BNE(r0, r2, 1);				/* bne done r0 r2 */
    movi(r0, 0);				/* set to 0 in delay slot */
    flush();
    movr(r0, r3);				/* after jump and delay slot */
    /* store new value */
#  if __WORDSIZE == 32
    if (jit_mips6_p())	SC_R6(r0, 0, r1);
    else		SC(r0, 0, r1);
#  else
    if (jit_mips6_p())	SCD_R6(r0, 0, r1);
    else		SCD(r0, 0, r1);
#  endif
    flush();
    jump1 = _jit->pc.w;
    BEQ(r0, _ZERO_REGNO, 0);			/* beqi retry r0 0 */
    movi(r0, 1);				/* set to 1 in delay slot */
    flush();
    SYNC();
    /* done: */
    flush();
    done = _jit->pc.w;
    patch_at(jump0, done);
    patch_at(jump1, retry);
    if (iscasi)
	jit_unget_reg(r1_reg);
}

static void
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	LB(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_c(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	LBU(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_uc(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	LH(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_s(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	LHU(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_us(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#if !PCREL_BROKEN
    if (jit_mips6_p()) {
	jit_word_t	w;
	assert(!(i0 & 3));
	w = (i0 - (_jit->pc.w + (_jitc->inst.pend ? 4 : 0))) >> 2;
	if (can_sign_extend_i19_p(w)) {
	    LWPC(r0, w);
	    goto done;
	}
    }
#endif
    if (can_sign_extend_short_p(i0))
	LW(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_i(r0, rn(reg));
	jit_unget_reg(reg);
    }
#if !PCREL_BROKEN
done:;
#endif
}

#if __WORDSIZE == 64
static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
#if !PCREL_BROKEN
    if (jit_mips6_p()) {
	jit_word_t	w;
	assert(!(i0 & 3));
	w = (i0 - (_jit->pc.w + (_jitc->inst.pend ? 4 : 0))) >> 2;
	if (can_sign_extend_i19_p(w)) {
	    LWUPC(r0, w);
	    goto done;
	}
    }
#endif
    if (can_sign_extend_short_p(i0))
	LWU(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_ui(r0, rn(reg));
	jit_unget_reg(reg);
    }
#if !PCREL_BROKEN
done:;
#endif
}

static void
_ldi_l(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (jit_mips6_p()) {
	assert(!(i0 & 7));
	w = (i0 - (_jit->pc.w + (_jitc->inst.pend ? 4 : 0))) >> 3;
	if (can_sign_extend_i18_p(w)) {
	    LDPC(r0, w);
	    goto done;
	}
    }
    if (can_sign_extend_short_p(i0))
	LD(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_l(r0, rn(reg));
	jit_unget_reg(reg);
    }
done:;
}
#endif

static void
_ldxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    addr(r0, r1, r2);
    ldr_c(r0, r0);
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (can_sign_extend_short_p(i0))
	LB(r0, i0, r1);
    else {
	addi(r0, r1, i0);
	ldr_c(r0, r0);
    }
}

static void
_ldxr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    addr(r0, r1, r2);
    ldr_uc(r0, r0);
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (can_sign_extend_short_p(i0))
	LBU(r0, i0, r1);
    else {
	addi(r0, r1, i0);
	ldr_uc(r0, r0);
    }
}

static void
_ldxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    addr(r0, r1, r2);
    ldr_s(r0, r0);
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (can_sign_extend_short_p(i0))
	LH(r0, i0, r1);
    else {
	addi(r0, r1, i0);
	ldr_s(r0, r0);
    }
}

static void
_ldxr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    addr(r0, r1, r2);
    ldr_us(r0, r0);
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (can_sign_extend_short_p(i0))
	LHU(r0, i0, r1);
    else {
	addi(r0, r1, i0);
	ldr_us(r0, r0);
    }
}

static void
_ldxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    addr(r0, r1, r2);
    ldr_i(r0, r0);
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (can_sign_extend_short_p(i0))
	LW(r0, i0, r1);
    else {
	addi(r0, r1, i0);
	ldr_i(r0, r0);
    }
}

#if __WORDSIZE == 64
static void
_ldxr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    addr(r0, r1, r2);
    ldr_ui(r0, r0);
}

static void
_ldxi_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (can_sign_extend_short_p(i0))
	LWU(r0, i0, r1);
    else {
	addi(r0, r1, i0);
	ldr_ui(r0, r0);
    }
}

static void
_ldxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    addr(r0, r1, r2);
    ldr_l(r0, r0);
}

static void
_ldxi_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (can_sign_extend_short_p(i0))
	LD(r0, i0, r1);
    else {
	addi(r0, r1, i0);
	ldr_l(r0, r0);
    }
}
#endif

#if __WORDSIZE == 32
#  define LOAD_LEFT			LWL
#  define LOAD_RIGHT			LWR
#else
#  define LOAD_LEFT			LDL
#  define LOAD_RIGHT			LDR
#endif
static void
_unldr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0, r2;
    if (jit_unaligned_p()) {
	assert(i0 >= 1 && i0 <= sizeof(jit_word_t));
	if (i0 == 1)
	    ldr_c(r0, r1);
	else {
	    if (r0 == r1) {
		t0 = jit_get_reg(jit_class_gpr);
		r2 = rn(t0);
		movr(r2, r1);
	    }
	    else
		r2 = r1;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    LOAD_LEFT(r0, sizeof(jit_word_t) - 1, r2);
	    LOAD_RIGHT(r0, 0, r2);
#else
	    LOAD_LEFT(r0, 0, r2);
	    LOAD_RIGHT(r0, sizeof(jit_word_t) - 1, r2);
#endif
	    if (r0 == r1)
		jit_unget_reg(t0);
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
    }
    else
	generic_unldr(r0, r1, i0);
}

static void
_unldi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    jit_int32_t		t0;
    if (jit_unaligned_p()) {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	unldr(r0, rn(t0), i1);
	jit_unget_reg(t0);
    }
    else
	generic_unldi(r0, i0, i1);
}

static void
_unldr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0, r2;
    if (jit_unaligned_p()) {
	assert(i0 >= 1 && i0 <= sizeof(jit_word_t));
	if (i0 == 1)
	    ldr_uc(r0, r1);
	else {
	    if (r0 == r1) {
		t0 = jit_get_reg(jit_class_gpr);
		r2 = rn(t0);
		movr(r2, r1);
	    }
	    else
		r2 = r1;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	    LOAD_LEFT(r0, sizeof(jit_word_t) - 1, r2);
	    LOAD_RIGHT(r0, 0, r2);
#else
	    LOAD_LEFT(r0, 0, r2);
	    LOAD_RIGHT(r0, sizeof(jit_word_t) - 1, r2);
#endif
	    if (r0 == r1)
		jit_unget_reg(t0);
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
    }
    else
	generic_unldr_u(r0, r1, i0);
}
#undef LOAD_LEFT
#undef LOAD_RIGHT

static void
_unldi_u(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0, jit_word_t i1)
{
    jit_int32_t		t0;
    if (jit_unaligned_p()) {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	unldr_u(r0, rn(t0), i1);
	jit_unget_reg(t0);
    }
    else
	generic_unldi_u(r0, i0, i1);
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SB(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_c(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SH(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_s(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SW(r0, i0, _ZERO_REGNO);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_i(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

#if __WORDSIZE == 64
static void
_sti_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SD(r0, i0, _ZERO_REGNO);
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
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_c(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SB(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_c(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_s(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SH(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_s(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_i(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SW(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_i(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

#if __WORDSIZE == 64
static void
_stxr_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1 ,jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    str_l(rn(reg), r2);
    jit_unget_reg(reg);
}

static void
_stxi_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (can_sign_extend_short_p(i0))
	SD(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_l(rn(reg), r1);
	jit_unget_reg(reg);
    }
}
#endif

static void
_unstr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 > 0 && i0 <= sizeof(jit_word_t));
    if (jit_unaligned_p()) {
	switch (i0) {
	    case 4:
#if __BYTE_ORDER == __LITTLE_ENDIAN
		SWL(r1, 3, r0);
		SWR(r1, 0, r0);
#else
		SWL(r1, 0, r0);
		SWR(r1, 3, r0);
#endif
		break;
#if __WORDSIZE == 64
	    case 8:
#  if __BYTE_ORDER == __LITTLE_ENDIAN
		SDL(r1, 7, r0);
		SDR(r1, 0, r0);
#  else
		SDL(r1, 0, r0);
		SDR(r1, 7, r0);
#  endif
		break;
#endif
	    default:
		/* FIXME Cost of loading memory contents, creating masks,
		 * and'ing, and or'ing values to use SW* or SD* might
		 * larger than using fallback. */
		 /* FIXME Probably not, and would be without branches. */
		fallback_unstr(r0, r1, i0);
		break;
	}
    }
    else
	generic_unstr(r0, r1, i0);
}

static void
_unsti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    if (jit_unaligned_p()) {
	if (i1 == 4 || i1 == 8) {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    unstr(rn(reg), r0, i1);
	    jit_unget_reg(reg);
	}
	else
	    fallback_unsti(i0, r0, i1);
    }
    else
	generic_unsti(i0, r0, i1);
}

static void
_bswapr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips2_p()) {
        extr_us(r0, r1);
        WSBH(r0, r0);
    } else {
        generic_bswapr_us(_jit, r0, r1);
    }
}

static void
_bswapr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips2_p()) {
        if (__WORDSIZE == 64) {
            SLL(r0, r1, 0);
            WSBH(r0, r0);
            ROTR(r0, r0, 16);
            mips_extr(r0, r0, 0, 32);
        } else {
            WSBH(r0, r1);
            ROTR(r0, r0, 16);
        }
    } else {
        generic_bswapr_ui(_jit, r0, r1);
    }
}

#if __WORDSIZE == 64
static void
_bswapr_ul(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips2_p()) {
	DSBH(r0, r1);
	DSHD(r0, r0);
    }
    else
	generic_bswapr_ul(_jit, r0, r1);
}
#endif

static void
_extr(jit_state_t *_jit,
      jit_int32_t r0, jit_int32_t r1, jit_word_t i0, jit_word_t i1)
{
    assert(i0 >= 0 && i1 >= 1 && i0 + i1 <= __WORDSIZE);
    if ( i1 == __WORDSIZE)
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
_extr_u(jit_state_t *_jit,
	jit_int32_t r0, jit_int32_t r1, jit_word_t i0, jit_word_t i1)
{
    assert(i0 >= 0 && i1 >= 1 && i0 + i1 <= __WORDSIZE);
    if (jit_mips2_p()) {
	if (i1 == __WORDSIZE)
	    movr(r0, r1);
	else {
#  if __BYTE_ORDER == __BIG_ENDIAN
	    i0 = __WORDSIZE - (i0 + i1);
#  endif
#if __WORDSIZE == 32
	    EXT(r0, r1, i0, i1);
#else
	    if (i0 < 32 && i1 <= 32)
		DEXT(r0, r1, i0, i1);
	    else if (i0 < 32 && i1 > 32)
		DEXTM(r0, r1, i0, i1);
	    else {
		assert(i0 >= 32 && i1 <= 32);
		DEXTU(r0, r1, i0, i1);
	    }
#endif
	}
    }
    else
	fallback_ext_u(r0, r1, i0, i1);
}

static void
_depr(jit_state_t *_jit,
      jit_int32_t r0, jit_int32_t r1, jit_word_t i0, jit_word_t i1)
{
    assert(i0 >= 0 && i1 >= 1 && i0 + i1 <= __WORDSIZE);
    if (jit_mips2_p()) {
	if (i1 == __WORDSIZE)
	    movr(r0, r1);
	else {
#  if __BYTE_ORDER == __BIG_ENDIAN
	    i0 = __WORDSIZE - (i0 + i1);
#  endif
#if __WORDSIZE == 32
	    INS(r0, r1, i0, i1);
#else
	    if (i0 < 32 && i1 <= 32 && (i0 + i1) <= 32)
		DINS(r0, r1, i0, i1);
	    else if (i0 < 32 && i1 >= 2 && (i0 + i1) > 32)
		DINSM(r0, r1, i0, i1);
	    else {
		assert(i0 >= 32 && i1 >= 1 && i1 <= 32 && (i0 + i1) > 32);
		DINSU(r0, r1, i0, i1);
	    }
#endif
	}
    }
    else
	fallback_dep(r0, r1, i0, i1);
}

static void
_extr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips2_p())
	SEB(r0, r1);
    else {
	lshi(r0, r1, __WORDSIZE - 8);
	rshi(r0, r0, __WORDSIZE - 8);
    }
}

static void
_extr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips2_p())
	SEH(r0, r1);
    else {
	lshi(r0, r1, __WORDSIZE - 16);
	rshi(r0, r0, __WORDSIZE - 16);
    }
}

#  if __WORDSIZE == 64
static void
_extr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_mips2_p())
        DEXT(r0, r1, 0, 32);
    else {
        lshi(r0, r1, 32);
        rshi_u(r0, r0, 32);
    }
}
#  endif

static void
_lti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;

    if (can_sign_extend_short_p(i0))
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

    if (can_sign_extend_short_p(i0))
	SLTIU(r0, r1, i0);
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

    if (can_sign_extend_short_p(i0 + 1))
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

    if (can_sign_extend_short_p(i0 + 1))
        SLTIU(r0, r1, i0 + 1);
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
    SLTIU(r0, r0, 1);
}

static void
_eqi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0) {
	subi(r0, r1, i0);
	SLTIU(r0, r0, 1);
    } else {
	SLTIU(r0, r1, 1);
    }
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

    if (can_sign_extend_short_p(i0)) {
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

    if (can_sign_extend_short_p(i0)) {
        SLTIU(r0, r1, i0);
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
_beqr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    /* Just to not move incorrectly instruction to delay slot */
    reg = jit_get_reg_for_delay_slot(jit_class_gpr|jit_class_chk, r0, r1);
    op = pending();
    /* implicit flush() */
    w = _jit->pc.w;
    BEQ(r0, r1, ((i0 - w) >> 2) - 1);
    delay(op);
    if (reg != JIT_NOREG)
	jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_beqi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (i1 == 0)
	w = beqr(i0, r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_gpr, r0, _ZERO_REGNO);
	op = pending();
	movi(rn(reg), i1);
	flush();
	w = _jit->pc.w;
	BEQ(r0, rn(reg), ((i0 - w) >> 2) - 1);
	delay(op);
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bger(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1,
      jit_bool_t sltu)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    reg = jit_get_reg_for_delay_slot(jit_class_gpr, r0, r1);
    op = pending();
    if (sltu)
	SLTU(rn(reg), r0, r1);
    else
	SLT(rn(reg), r0, r1);
    flush();
    w = _jit->pc.w;
    BEQ(rn(reg), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    delay(op);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
      jit_bool_t sltiu, jit_bool_t bne)
{
    jit_word_t		w;
    jit_word_t		d;
    jit_bool_t		zero_p;
    jit_int32_t		op, t0, mask;
    zero_p = !sltiu && i1 == 0;
    /* Even if zero_p allocate one as a mean to avoid incorrect delay slot */
    mask = jit_class_gpr;
    if (zero_p)
	mask |= jit_class_chk;
    t0 = jit_get_reg_for_delay_slot(mask, r0, _ZERO_REGNO);
    if (can_sign_extend_short_p(i1)) {
	op = pending();
	if (!zero_p) {
	    if (sltiu)
		SLTIU(rn(t0), r0, i1);
	    else
		SLTI(rn(t0), r0, i1);
        }
	flush();
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 1;
	if (bne) {
	    if (!zero_p)
		BNE(rn(t0), _ZERO_REGNO, d);
	    else
		BLTZ(r0, d);
	}
	else {
	    if (!zero_p)
		BEQ(rn(t0), _ZERO_REGNO, d);
	    else
		BGEZ(r0, d);
	}
    }
    else {
	op = pending();
	movi(rn(t0), i1);
	if (sltiu)
	    SLTU(rn(t0), r0, rn(t0));
        else
	    SLT(rn(t0), r0, rn(t0));
	flush();
	w = _jit->pc.w;
	if (bne)
	    BNE(rn(t0), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	else
	    BEQ(rn(t0), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    }
    delay(op);
    if (t0 != JIT_NOREG)
	jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bgtr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1,
      jit_bool_t sltu, jit_bool_t inv)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    reg = jit_get_reg_for_delay_slot(jit_class_gpr, r0, r1);
    op = pending();
    if (sltu)
	SLTU(rn(reg), r1, r0);
    else
	SLT(rn(reg), r1, r0);
    flush();
    w = _jit->pc.w;
    if (inv)
	BEQ(rn(reg), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    else
	BNE(rn(reg), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    delay(op);
    jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bgti(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1,
      jit_bool_t sltiu, jit_bool_t inv)
{
    jit_word_t		w;
    jit_int32_t		op, t0, mask;
    mask = jit_class_gpr;
    if (i0 == 0)
	mask |= jit_class_chk;
    /* Allocate even if i0 == 0 as a way to avoid incorrect delay slot */
    t0 = jit_get_reg_for_delay_slot(mask, r0, _ZERO_REGNO);
    if (i1 == 0) {
	op = pending();
	/* implicit flush() */
	w = _jit->pc.w;
	if (inv) {
	    if (sltiu)
		BEQ(r0, _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	    else
		BLEZ(r0, ((i0 - w) >> 2) - 1);
	}
	else {
	    if (sltiu)
		BNE(r0, _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	    else
		BGTZ(r0, ((i0 - w) >> 2) - 1);
	}
    }
    else {
	op = pending();
	movi(rn(t0), i1);
	if (sltiu)
	    SLTU(rn(t0), rn(t0), r0);
	else
	    SLT(rn(t0), rn(t0), r0);
	flush();
	w = _jit->pc.w;
	if (inv)
	    BEQ(rn(t0), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	else
	    BNE(rn(t0), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    }
    delay(op);
    if (t0 != JIT_NOREG)
	jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bner(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    /* Just to not move incorrectly instruction to delay slot */
    reg = jit_get_reg_for_delay_slot(jit_class_gpr|jit_class_chk, r0, r1);
    op = pending();
    /* implicit flush() */
    w = _jit->pc.w;
    BNE(r0, r1, ((i0 - w) >> 2) - 1);
    delay(op);
    if (reg != JIT_NOREG)
	jit_unget_reg(reg);
    return (w);
}

static jit_word_t
_bnei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		op, reg;
    if (i1 == 0)
	w = bner(i0, r0, _ZERO_REGNO);
    else {
	reg = jit_get_reg_for_delay_slot(jit_class_gpr, r0, _ZERO_REGNO);
	op = pending();
	movi(rn(reg), i1);
	flush();
	w = _jit->pc.w;
	BNE(r0, rn(reg), ((i0 - w) >> 2) - 1);
	delay(op);
	jit_unget_reg(reg);
    }
    return (w);
}

static void
_jmpr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		op, t0;
    /* make sure delay slot does not use r0 */
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr|jit_class_chk,
				    r0, _ZERO_REGNO);
    op = pending();
    JR(r0);
    delay(op);
    if (t0 != JIT_NOREG)
	jit_unget_reg(t0);
}

static jit_word_t
_jmpi(jit_state_t *_jit, jit_word_t i0, jit_bool_t patch)
{
    jit_int32_t		op, t0;
    jit_word_t		w, disp;
#if !BALC_BROKEN
    if (jit_mips6_p() && !(i0 & 3)) {
	disp = ((i0 - (_jit->pc.w + (_jitc->inst.pend ? 4 : 0))) >> 2) - 1;
	if (patch || can_sign_extend_i26_p(disp)) {
	    flush();
	    w = _jit->pc.w;
	    /* Compact branch instructions do not have a delay slot */
	    BC_R6(disp);
	    goto done_without_delay;
	}
    }
#endif
    /* try to get a pending instruction before the jump */
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr, _ZERO_REGNO, _ZERO_REGNO);
    op = pending();
    /* implicit flush() */
    w = _jit->pc.w;
    if (jit_mips2_p()) {
	disp = ((i0 - w) >> 2) - 1;
	if (patch || can_sign_extend_short_p(disp)) {
	    BEQ(_ZERO_REGNO, _ZERO_REGNO, disp);
	    goto done;
	}
    }
    if (((w + sizeof(jit_int32_t)) & 0xf0000000) == (i0 & 0xf0000000))
	J((i0 & ~0xf0000000) >> 2);
    else {
	if (patch)
	    w = movi_p(rn(t0), i0);
	else
	    movi(rn(t0), i0);
	JR(rn(t0));
    }
done:
    delay(op);
    jit_unget_reg(t0);
#if !BALC_BROKEN
done_without_delay:
#endif
    return (w);
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		op, t0;
    /* Get a register without side effects in delay slot */
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr, _ZERO_REGNO, _ZERO_REGNO);
    /* Check for a instruction that can be executed in the delay slot */
    op = pending();
    /* implicit flush() */
    w = _jit->pc.w;
    movi_p(rn(t0), i0);
    flush();			/* movi_p will be patched */
    JR(rn(t0));
    delay(op);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    /* t1 = r0 + r1;	overflow = r1 < 0 ? r0 < t1 : t1 < r0 */
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLT(rn(t0), r1, _ZERO_REGNO);	/* t0 = r1 < 0 */
    addr(rn(t1), r0, r1);		/* t1 = r0 + r1 */
    SLT(rn(t2), rn(t1), r0);		/* t2 = t1 < r0 */
    SLT(rn(t1), r0, rn(t1));		/* t1 = r0 < t1 */
    movzr(rn(t1), rn(t2), rn(t0));	/* if (r0 == 0) t1 = t2 */
    /* cannot optimize delay slot */
    flush();
    w = _jit->pc.w;
    BNE(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    /* delay slot */
    addr(r0, r0, r1);
    flush();
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);

    return (w);
}

static jit_word_t
_boaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    if (can_sign_extend_short_p(i1)) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	SLTI(rn(t0), _ZERO_REGNO, i1);
	addiu(rn(t1), r0, i1);
	SLT(rn(t2), r0, rn(t1));
	SLT(rn(t1), rn(t1), r0);
	movzr(rn(t1), rn(t2), rn(t0));
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BNE(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	/* delay slot */
	addiu(r0, r0, i1);
	flush();
	jit_unget_reg(t2);
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i1);
	w = boaddr(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_boaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    SLTU(rn(t1), rn(t0), r0);
    flush();
    /* cannot optimize delay slot */
    w = _jit->pc.w;
    BNE(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
    /* delay slot */
    movr(r0, rn(t0));
    flush();
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    if (can_sign_extend_short_p(i0)) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	addiu(rn(t0), r0, i1);
	SLTU(rn(t1), rn(t0), r0);
	flush();
	/* cannot optimize delay slot */
	w = _jit->pc.w;
	BNE(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
	/* delay slot */
	movr(r0, rn(t0));
	flush();
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i1);
	w = boaddr_u(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_bxaddr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    /* t1 = r0 + r1;	overflow = r1 < 0 ? r0 < t1 : t1 < r0 */
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLT(rn(t0), r1, _ZERO_REGNO);	/* t0 = r1 < 0 */
    addr(rn(t1), r0, r1);		/* t1 = r0 + r1 */
    SLT(rn(t2), rn(t1), r0);		/* t2 = t1 < r0 */
    SLT(rn(t1), r0, rn(t1));		/* t1 = r0 < t1 */
    movzr(rn(t1), rn(t2), rn(t0));	/* if (r0 == 0) t1 = t2 */
    /* cannot optimize delay slot */
    flush();
    w = _jit->pc.w;
    BEQ(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    /* delay slot */
    addr(r0, r0, r1);
    flush();
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);

    return (w);
}

static jit_word_t
_bxaddi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    if (can_sign_extend_short_p(i1)) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	SLTI(rn(t0), _ZERO_REGNO, i1);
	addiu(rn(t1), r0, i1);
	SLT(rn(t2), r0, rn(t1));
	SLT(rn(t1), rn(t1), r0);
	movzr(rn(t1), rn(t2), rn(t0));
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BEQ(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	/* delay slot */
	addiu(r0, r0, i1);
	flush();
	jit_unget_reg(t2);
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i1);
	w = bxaddr(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_bxaddr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    SLTU(rn(t1), rn(t0), r0);
    /* cannot optimize delay slot */
    flush();
    w = _jit->pc.w;
    BEQ(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
    /* delay slot */
    movr(r0, rn(t0));
    flush();
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    if (can_sign_extend_short_p(i0)) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	addiu(rn(t0), r0, i1);
	SLTU(rn(t1), rn(t0), r0);
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BEQ(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
	/* delay slot */
	movr(r0, rn(t0));
	flush();
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i1);
	w = bxaddr_u(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_bosubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    /* t1 = r0 - r1;	overflow = 0 < r1 ? r0 < t1 : t1 < r0 */
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLT(rn(t0), _ZERO_REGNO, r1);	/* t0 = 0 < r1 */
    subr(rn(t1), r0, r1);		/* t1 = r0 - r1 */
    SLT(rn(t2), rn(t1), r0);		/* t2 = t1 < r0 */
    SLT(rn(t1), r0, rn(t1));		/* t1 = r0 < t1 */
    movzr(rn(t1), rn(t2), rn(t0));	/* if (r0 == 0) t1 = t2 */
    flush();
    w = _jit->pc.w;
    BNE(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    /* delay slot */
    subr(r0, r0, r1);
    flush();
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);

    return (w);
}

static jit_word_t
_bosubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    if (can_sign_extend_short_p(i1) && (i1 & 0xffff) != 0x8000) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	SLTI(rn(t0), _ZERO_REGNO, i1);
	addiu(rn(t1), r0, -i1);
	SLT(rn(t2), rn(t1), r0);
	SLT(rn(t1), r0, rn(t1));
	movzr(rn(t1), rn(t2), rn(t0));
	flush();
	w = _jit->pc.w;
	BNE(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	/* delay slot */
	addiu(r0, r0, -i1);
	flush();
	jit_unget_reg(t2);
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i1);
	w = bosubr(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_bosubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    SLTU(rn(t1), r0, rn(t0));
    /* cannot optimize delay slot */
    flush();
    w = _jit->pc.w;
    BNE(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
    /* delay slot */
    movr(r0, rn(t0));
    flush();
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    if (can_sign_extend_short_p(i0) && (i0 & 0xffff) != 0x8000) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	addiu(rn(t0), r0, -i1);
	SLTU(rn(t1), r0, rn(t0));
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BNE(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
	/* delay slot */
	movr(r0, rn(t0));
	flush();
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i1);
	w = bosubr_u(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_bxsubr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    /* t1 = r0 - r1;	overflow = 0 < r1 ? r0 < t1 : t1 < r0 */
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    SLT(rn(t0), _ZERO_REGNO, r1);	/* t0 = 0 < r1 */
    subr(rn(t1), r0, r1);		/* t1 = r0 - r1 */
    SLT(rn(t2), rn(t1), r0);		/* t2 = t1 < r0 */
    SLT(rn(t1), r0, rn(t1));		/* t1 = r0 < t1 */
    movzr(rn(t1), rn(t2), rn(t0));	/* if (t0 == 0) t1 = t2 */
    /* cannot optimize delay slot */
    flush();
    w = _jit->pc.w;
    BEQ(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
    /* delay slot */
    subr(r0, r0, r1);
    flush();
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);

    return (w);
}

static jit_word_t
_bxsubi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;

    if (can_sign_extend_short_p(i1) && (i1 & 0xffff) != 0x8000) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t2 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	SLTI(rn(t0), _ZERO_REGNO, i1);
	addiu(rn(t1), r0, -i1);
	SLT(rn(t2), rn(t1), r0);
	SLT(rn(t1), r0, rn(t1));
	movzr(rn(t1), rn(t2), rn(t0));
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BEQ(rn(t1), _ZERO_REGNO, ((i0 - w) >> 2) - 1);
	/* delay slot */
	addiu(r0, r0, -i1);
	flush();
	jit_unget_reg(t2);
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i1);
	w = bxsubr(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_bxsubr_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    SLTU(rn(t1), r0, rn(t0));
    /* cannot optimize delay slot */
    flush();
    w = _jit->pc.w;
    BEQ(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
    /* delay slot */
    movr(r0, rn(t0));
    flush();
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubi_u(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		t1;

    if (can_sign_extend_short_p(i0) && (i0 & 0xffff) != 0x8000) {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	addiu(rn(t0), r0, -i1);
	SLTU(rn(t1), r0, rn(t0));
	/* cannot optimize delay slot */
	flush();
	w = _jit->pc.w;
	BEQ(_ZERO_REGNO, rn(t1), ((i0 - w) >> 2) - 1);
	/* delay slot */
	movr(r0, rn(t0));
	flush();
	jit_unget_reg(t1);
	jit_unget_reg(t0);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i1);
	w = bxsubr_u(i0, r0, rn(t0));
	jit_unget_reg(t0);
    }
    return (w);
}

static jit_word_t
_bmsr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		op, t0;
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr, r0, r1);
    op = pending();
    AND(rn(t0), r0, r1);
    flush();
    w = _jit->pc.w;
    BNE(_ZERO_REGNO, rn(t0), ((i0 - w) >> 2) - 1);
    delay(op);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmsi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		op, t0;
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr, r0, _ZERO_REGNO);
    op = pending();
    andi(rn(t0), r0, i1);
    flush();
    w = _jit->pc.w;
    BNE(_ZERO_REGNO, rn(t0), ((i0 - w) >> 2) - 1);
    delay(op);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmcr(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		op, t0;
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr, r0, r1);
    op = pending();
    AND(rn(t0), r0, r1);
    flush();
    w = _jit->pc.w;
    BEQ(_ZERO_REGNO, rn(t0), ((i0 - w) >> 2) - 1);
    delay(op);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmci(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		op, t0;
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr, r0, _ZERO_REGNO);
    op = pending();
    andi(rn(t0), r0, i1);
    flush();
    w = _jit->pc.w;
    BEQ(_ZERO_REGNO, rn(t0), ((i0 - w) >> 2) - 1);
    delay(op);
    jit_unget_reg(t0);
    return (w);
}

static void
_callr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		op, t0;
    if (r0 != _T9_REGNO) {
	JALR(r0);
	/* delay slot */
        movr(_T9_REGNO, r0);
	flush();
    }
    else {
	/* make sure delay slot does not use r0 */
	t0 = jit_get_reg_for_delay_slot(jit_class_gpr|jit_class_chk,
					r0, _ZERO_REGNO);
	op = pending();
	JALR(r0);
	delay(op);
	if (t0 != JIT_NOREG)
	    jit_unget_reg(t0);
    }
}

static jit_word_t
_calli(jit_state_t *_jit, jit_word_t i0, jit_bool_t patch)
{
    jit_int32_t		op, t0;
    jit_word_t		w, disp;
    w = _jit->pc.w;
#if !BALC_BROKEN
    if (jit_mips6_p() && !(i0 & 3)) {
	disp = ((i0 - (w + (_jitc->inst.pend ? 4 : 0))) >> 2) - 1;
	if (patch || can_sign_extend_i26_p(disp)) {
	    flush();
	    w = _jit->pc.w;
	    /* Compact branch instructions do not have a delay slot */
	    BALC(disp);
	    goto done;
	}
    }
#endif
    if (jit_mips2_p()) {
	disp = ((i0 - (w + _jitc->inst.pend ? 4 : 0)) >> 2) - 1;
	if (patch || can_sign_extend_short_p(disp)) {
	    op = pending();
	    BGEZAL(_ZERO_REGNO, disp);	/* Renamed to BAL in mips release 6 */
	    delay(op);
	    goto done;
	}
    }
    assert(!patch);
    flush();
    if (((w + sizeof(jit_int32_t)) & 0xf0000000) == (i0 & 0xf0000000)) {
	if (can_sign_extend_short_p(i0)) {
	    JAL((i0 & ~0xf0000000) >> 2);
	    /* delay slot */
	    addiu(_T9_REGNO, _ZERO_REGNO, i0);
	}
	else if (can_zero_extend_short_p(i0)) {
	    JAL((i0 & ~0xf0000000) >> 2);
	    /* delay slot */
	    ORI(_T9_REGNO, _ZERO_REGNO, i0);
        }
	else if (can_sign_extend_int_p(i0)) {
	    if (i0 & 0xffff) {
		LUI(_T9_REGNO, i0 >> 16);
		JAL((i0 & ~0xf0000000) >> 2);
		/* delay slot */
		ORI(_T9_REGNO, _T9_REGNO, i0);
            }
	    else {
		JAL((i0 & ~0xf0000000) >> 2);
		/* delay slot */
		LUI(_T9_REGNO, i0 >> 16);
	    }
        }
	else
	    goto fallback;
    }
    else {
    fallback:
	/* make sure delay slot does not use _T9_REGNO */
	t0 = jit_get_reg_for_delay_slot(jit_class_gpr|jit_class_chk,
					_T9_REGNO, _ZERO_REGNO);
	/* try to get an instruction before the call */
	op = pending();
	movi(_T9_REGNO, i0);
	JALR(_T9_REGNO);
	delay(op);
	if (t0 != JIT_NOREG)
	    jit_unget_reg(t0);
    }
    done:
    return (w);
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		word;
    jit_int32_t		op, t0;
    /* make sure delay slot does not use _T9_REGNO */
    t0 = jit_get_reg_for_delay_slot(jit_class_gpr|jit_class_chk,
				    _T9_REGNO, _ZERO_REGNO);
    op = pending();
    /* implicit flush() */
    word = _jit->pc.w;
    movi_p(_T9_REGNO, i0);
    JALR(_T9_REGNO);
    delay(op);
    if (t0 != JIT_NOREG)
	jit_unget_reg(t0);
    return (word);
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
	_jitc->function->self.aoff &= -8;
#if NEW_ABI
    _jitc->function->stack = ((_jitc->function->self.alen -
			      /* align stack at 16 bytes */
			       _jitc->function->self.aoff) + 15) & -16;
#else
    _jitc->function->stack = ((/* first 16 bytes must be allocated */
			      (_jitc->function->self.alen > 16 ?
			       _jitc->function->self.alen : 16) -
			      /* align stack at 8 bytes */
			      _jitc->function->self.aoff) + 7) & -8;
#endif

#if NEW_ABI
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
#else
    /* Need always a frame due to the need to always allocate 16 bytes */
    jit_check_frame();
#endif

    if (_jitc->function->need_frame || _jitc->function->need_stack)
	subi(_SP_REGNO, _SP_REGNO, jit_framesize());
    if (_jitc->function->need_frame) {
	stxi(0, _SP_REGNO, _RA_REGNO);
	stxi(STACK_SLOT, _SP_REGNO, _BP_REGNO);
    }
    /* callee save registers */
    for (reg = 0, offs = STACK_SLOT << 1; reg < jit_size(iregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, iregs[reg])) {
	    stxi(offs, _SP_REGNO, rn(iregs[reg]));
	    offs += STACK_SLOT;
	}
    }
    for (reg = 0; reg < jit_size(fregs); reg++) {
	if (jit_regset_tstbit(&_jitc->function->regset, fregs[reg])) {
	    stxi_d(offs, _SP_REGNO, rn(fregs[reg]));
	    offs += sizeof(jit_float64_t);
	}
    }

    if (_jitc->function->need_frame)
	movr(_BP_REGNO, _SP_REGNO);

    /* alloca */
    if (_jitc->function->stack)
	subi(_SP_REGNO, _SP_REGNO, _jitc->function->stack);
    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _BP_REGNO, rn(reg));
	jit_unget_reg(reg);
    }

    if (_jitc->function->self.call & jit_call_varargs) {
	for (reg = _jitc->function->vagp; jit_arg_reg_p(reg); ++reg) {
	    offs = jit_framesize() - ((NUM_WORD_ARGS - reg) * STACK_SLOT);
#if NEW_ABI
	    SD(rn(_A0 - reg), offs, _BP_REGNO);
#else
	    offs += 16 + WORD_ADJUST;
	    stxi(offs, _BP_REGNO, rn(_A0 - reg));
#endif
	}
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg, offs;
    if (_jitc->function->assume_frame)
	return;

    if (_jitc->function->need_frame) {
	movr(_SP_REGNO, _BP_REGNO);
	ldxi(_RA_REGNO, _SP_REGNO, 0);
	ldxi(_BP_REGNO, _SP_REGNO, STACK_SLOT);
    }

    /* callee save registers */
    for (reg = 0, offs = STACK_SLOT << 1; reg < jit_size(iregs); reg++) {
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
    JR(_RA_REGNO);
    /* delay slot */
    if (_jitc->function->need_frame || _jitc->function->need_stack)
	addi(_SP_REGNO, _SP_REGNO, jit_framesize());
    else
	NOP(1);
    flush();
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    assert(_jitc->function->self.call & jit_call_varargs);
#if NEW_ABI
    /* Initialize va_list to the first stack argument. */
    if (jit_arg_reg_p(_jitc->function->vagp))
	addi(r0, _BP_REGNO,
	     jit_framesize() -
	     ((NUM_WORD_ARGS - _jitc->function->vagp) * STACK_SLOT));
    else
#endif
	addi(r0, _BP_REGNO, jit_selfsize());
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    /* Load argument. */
#if WORD_ADJUST
    ldxi(r0, r1, WORD_ADJUST);
#else
    ldr(r0, r1);
#endif

    /* Update va_list. */
    addi(r1, r1, STACK_SLOT);
}

static void
_patch_abs(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    jit_instr_t		i;
    union {
	jit_int32_t	*i;
	jit_word_t	 w;
    } u;

    u.w = instr;
#if __WORDSIZE == 32
    i.op = u.i[0];
    assert(i.hc.b == MIPS_LUI);
    i.is.b = label >> 16;
    u.i[0] = i.op;
    i.op = u.i[1];
    assert(i.hc.b == MIPS_ORI);
    i.is.b = label;
    u.i[1] = i.op;
#else
    i.op = u.i[0];
    assert(i.hc.b == MIPS_LUI);
    i.is.b = label >> 48;
    u.i[0] = i.op;
    i.op = u.i[1];
    assert(i.hc.b == MIPS_ORI);
    i.is.b = label >> 32;
    u.i[1] = i.op;
    /* lshi */
    i.op = u.i[3];
    assert(i.hc.b == MIPS_ORI);
    i.is.b = label >> 16;
    u.i[3] = i.op;
    /* lshi */
    i.op = u.i[5];
    assert(i.hc.b == MIPS_ORI);
    i.is.b = label;
    u.i[5] = i.op;
#endif
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    jit_instr_t		i;
    union {
	jit_int32_t	*i;
	jit_word_t	 w;
    } u;

    u.w = instr;
    i.op = u.i[0];
    switch (i.hc.b) {
	/* 16 bit immediate opcodes */
	case MIPS_REGIMM:
	    switch (i.rt.b) {
		case MIPS_BLTZ:		case MIPS_BLTZL:
		case MIPS_BLTZAL:	case MIPS_BLTZALL:
		case MIPS_BGEZ:		case MIPS_BGEZAL:
		case MIPS_BGEZALL:	case MIPS_BGEZL:
		case MIPS_TEQI:		case MIPS_TGEI:
		case MIPS_TGEIU:	case MIPS_TLTI:
		case MIPS_TLTIU:	case MIPS_TNEI:
		    i.is.b = ((label - instr) >> 2) - 1;
		    u.i[0] = i.op;
		    break;
		default:
		    assert(!"unhandled branch opcode");
		    break;
	    }
	    break;

	case MIPS_COP1:			case MIPS_COP2:
	    if (jit_mips6_p()) {
		switch (i.rs.b) {
		    case MIPS_BC1EQZ:	case MIPS_BC1NEZ:
			assert(jit_mips6_p());
			i.is.b = ((label - instr) >> 2) - 1;
			u.i[0] = i.op;
			break;
		    default:
			assert(!"unhandled branch opcode");
			break;
		}
	    }
	    else {
		assert(i.rs.b == MIPS_BC);
		switch (i.rt.b) {
		    case MIPS_BCF:		case MIPS_BCFL:
		    case MIPS_BCT:		case MIPS_BCTL:
			assert(!jit_mips6_p());
			i.is.b = ((label - instr) >> 2) - 1;
			u.i[0] = i.op;
			break;
		    default:
			assert(!"unhandled branch opcode");
			break;
		}
	    }
	    break;

	case MIPS_BLEZ:			case MIPS_BLEZL:
	case MIPS_BEQ:			case MIPS_BEQL:
	case MIPS_BGTZ:			case MIPS_BGTZL:
	case MIPS_BNE:			case MIPS_BNEL:
	    i.is.b = ((label - instr) >> 2) - 1;
	    u.i[0] = i.op;
	    break;

	case MIPS_LUI:
	    patch_abs(instr, label);
	    break;

	case MIPS_J:			case MIPS_JAL:
	case MIPS_JALX:
	    assert(((instr + sizeof(jit_int32_t)) & 0xf0000000) ==
		   (label & 0xf0000000));
	    i.ii.b = (label & ~0xf0000000) >> 2;
	    u.i[0] = i.op;
	    break;

	case MIPS_BALC:			case MIPS_BC_R6:
	    assert(jit_mips6_p());
	    i.ii.b = ((label - instr) >> 2) - 1;
	    u.i[0] = i.op;
	    break;

	default:
	    assert(!"unhandled branch opcode");
	    break;
    }
}
#endif
