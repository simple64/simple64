/*
 * Copyright (C) 2023  Free Software Foundation, Inc.
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

#ifndef _jit_loongarch_h
#define _jit_loongarch_h

/*
 * Types
 */
#define JIT_FP			_FP
typedef enum {
#define jit_r_num()		9
#define jit_r(i)		(_T0 - (i))
#define JIT_R0			_T0
#define JIT_R1			_T1
#define JIT_R2			_T2
#define JIT_R3			_T3
#define JIT_R4			_T4
#define JIT_R5			_T5
#define JIT_R6			_T6
#define JIT_R7			_T7
#define JIT_R8			_T8
    _T8,	_T7,	_T6,	_T5,	_T4,	_T3,	_T2,	_T1,	_T0,
#define jit_v_num()		9
#define jit_v(i)		(_S0 - (i))
#define JIT_V0		_S0
#define JIT_V1		_S1
#define JIT_V2		_S2
#define JIT_V3		_S3
#define JIT_V4		_S4
#define JIT_V5		_S5
#define JIT_V6		_S6
#define JIT_V7		_S7
#define JIT_V8		_S8
    _S8,	_S7,	_S6,	_S5,	_S4,	_S3,	_S2,	_S1,	_S0,
    _A7,	_A6,	_A5,	_A4,	_A3,	_A2,	_A1,	_A0,
    _FP,
    _R21,
    _ZERO,
    _RA,
    _TP,
    _SP,
    _FT0,	_FT1,	_FT2,	_FT3,	_FT4,	_FT5,	_FT6,	_FT7,
    _FT8,	_FT9,	_FT10,	_FT11,	_FT12,	_FT13,	_FT14,	_FT15,
    _FA7,	_FA6,	_FA5,	_FA4,	FA3,	_FA2,	_FA1,	_FA0,
#define jit_f_num()	8
#define jit_f(i)	(_FS0 - (i))
#define JIT_F0		_FS0
#define JIT_F1		_FS1
#define JIT_F2		_FS2
#define JIT_F3		_FS3
#define JIT_F4		_FS4
#define JIT_F5		_FS5
#define JIT_F6		_FS6
#define JIT_F7		_FS7
    _FS7,	_FS6,	_FS5,	_FS4,	_FS3,	_FS2,	_FS1,	_FS0,
#define JIT_NOREG		_NOREG
    _NOREG,
} jit_reg_t;

typedef struct {
    /* generate special instructions for unaligned load/store? */
    /* It is not guaranteed unaligned memory access is supported. */
    jit_uint32_t unaligned	: 1;
} jit_cpu_t;

/*
 * Initialization
 */
extern jit_cpu_t		jit_cpu;

#endif /* _jit_loongarch_h */
