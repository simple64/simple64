#include "../state.hpp"

#ifdef TRACE_COP2
#include <stdio.h>
#define TRACE_LS(op) printf(#op " v%u, %u, %d(r%u)\n", rt, e, offset, base)
#else
#define TRACE_LS(op) ((void)0)
#endif

extern "C"
{
	// Using mostly Ares' implementation as a base here

	static inline uint8_t byteFromHalfWords(const uint16_t *arr, unsigned i)
	{
		return (i & 1) ?
			(uint8_t)(arr[i >> 1] & 0xff) :
			(uint8_t)(arr[i >> 1] >> 8);
	}
	
	static inline void writeByteToHalfWords(uint16_t *arr, unsigned i, uint8_t b)
	{
		const unsigned n = i >> 1;
		if (i & 1)
			arr[n] = (arr[n] & 0xff00) | (uint16_t)b;
		else
			arr[n] = (arr[n] & 0xff) | ((uint16_t)b << 8);
	}
	
	// Load 8-bit
	void RSP_LBV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LBV);
		unsigned addr = (rsp->sr[base] + offset * 1) & 0xfff;
		reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES(e)] = READ_MEM_U8(rsp->dmem, addr);
	}

	// Store 8-bit
	void RSP_SBV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SBV);
		unsigned addr = (rsp->sr[base] + offset * 1) & 0xfff;
		uint8_t v = reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES(e)];

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SBV: 0x%x (0x%x)\n", addr, v);
#endif

		WRITE_MEM_U8(rsp->dmem, addr, v);
	}

	// Load 16-bit
	void RSP_LSV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LSV);
		unsigned addr = rsp->sr[base] + offset * 2;
		const unsigned end = (e > 14) ? 16 : (e + 2);
		for (unsigned i = e; i < end; i++)
			writeByteToHalfWords(rsp->cp2.regs[rt].e, i & 0xf, READ_MEM_U8(rsp->dmem, addr++ & 0xfff));
	}

	// Store 16-bit
	void RSP_SSV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SSV);
		unsigned addr = (rsp->sr[base] + offset * 2) & 0xfff;
		uint8_t v0 = reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES(e)];
		uint8_t v1 = reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES((e + 1) & 0xf)];

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SSV: 0x%x (0x%x, 0x%x)\n", addr, v0, v1);
#endif

		WRITE_MEM_U8(rsp->dmem, addr, v0);
		WRITE_MEM_U8(rsp->dmem, (addr + 1) & 0xfff, v1);
	}

	// Load 32-bit
	void RSP_LLV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LLV);
		unsigned addr = rsp->sr[base] + offset * 4;
		const unsigned end = (e > 12) ? 16 : (e + 4);
		for (unsigned i = e; i < end; i++)
			writeByteToHalfWords(rsp->cp2.regs[rt].e, i & 0xf, READ_MEM_U8(rsp->dmem, addr++ & 0xfff));
	}

	// Store 32-bit
	void RSP_SLV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SLV);
		unsigned addr = (rsp->sr[base] + offset * 4) & 0xfff;

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SLV 0x%x, e = %u\n", addr, e);
#endif

		for (unsigned i = e; i < e + 4; i++)
			WRITE_MEM_U8(rsp->dmem, addr++, byteFromHalfWords(rsp->cp2.regs[rt].e, i & 0xf));
	}

	// Load 64-bit
	void RSP_LDV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LDV);
		unsigned addr = rsp->sr[base] + offset * 8;
		const unsigned end = (e > 8) ? 16 : (e + 8);
		for (unsigned i = e; i < end; i++)
			writeByteToHalfWords(rsp->cp2.regs[rt].e, i & 0xf, READ_MEM_U8(rsp->dmem, addr++ & 0xfff));
	}

	// Store 64-bit
	void RSP_SDV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SDV);
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SDV 0x%x, e = %u\n", addr, e);
#endif

		// Handle illegal scenario.
		if ((e > 8) || (e & 1) || (addr & 1))
		{
			for (unsigned i = 0; i < 8; i++)
			{
				WRITE_MEM_U8(rsp->dmem, (addr + i) & 0xfff,
				             reinterpret_cast<const uint8_t *>(rsp->cp2.regs[rt].e)[MES((e + i) & 0xf)]);
			}
		}
		else
		{
			e >>= 1;
			for (unsigned i = 0; i < 4; i++)
			{
				WRITE_MEM_U16(rsp->dmem, (addr + 2 * i) & 0xfff, rsp->cp2.regs[rt].e[e + i]);
			}
		}
	}

	// Load 8x8-bit into high bits.
	void RSP_LPV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LPV);
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		const unsigned index = (addr & 7) - e;
		addr &= ~7;

		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			reg[i] = READ_MEM_U8(rsp->dmem, (addr + (i + index & 0xf)) & 0xfff) << 8;
	}

	void RSP_SPV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SPV);
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;

		for (unsigned i = e; i < e + 8; i++) {
			const unsigned shift = ((i & 0xf) < 8) ? 8 : 7;
			WRITE_MEM_U8(rsp->dmem, addr++ & 0xfff, int16_t(reg[i & 7]) >> shift);
		}
	}

	// Load 8x8-bit into high bits, but shift by 7 instead of 8.
	// Was probably used for certain fixed point algorithms to get more headroom without
	// saturation, but weird nonetheless.
	void RSP_LUV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LUV);
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		const unsigned index = (addr & 7) - e;
		addr &= ~7;

		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			reg[i] = READ_MEM_U8(rsp->dmem, (addr + (i + index & 0xf)) & 0xfff) << 7;
	}

	void RSP_SUV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SUV);
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;

		for (unsigned i = e; i < e + 8; i++) {
			const unsigned shift = ((i & 0xf) < 8) ? 7 : 8;
			WRITE_MEM_U8(rsp->dmem, addr++ & 0xfff, int16_t(reg[i & 7]) >> shift);
		}
	}

	// Load 8x8-bits into high bits, but shift by 7 instead of 8.
	// Seems to differ from LUV in that it loads every other byte instead of packed bytes.
	void RSP_LHV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LHV);
		unsigned addr = rsp->sr[base] + offset * 16;
		const unsigned index = (addr & 7) - e;
		addr &= ~7;

		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			reg[i] = (uint16_t)READ_MEM_U8(rsp->dmem, (addr + (index + i * 2 & 0xf)) & 0xfff) << 7;
	}

	void RSP_SHV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SHV);
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		const unsigned index = addr & 7;
		addr &= ~7;

		const auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
		{
			const unsigned b = e + (i << 1);
			const uint8_t byte = byteFromHalfWords(reg, b & 0xf) << 1 | byteFromHalfWords(reg, b + 1 & 0xf) >> 7;
			WRITE_MEM_U8(rsp->dmem, addr + (index + i * 2 & 0xf), byte);
		}
	}

	void RSP_LFV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LFV);
		uint16_t temp[8];

		unsigned addr = rsp->sr[base] + offset * 16;
		const unsigned index = (addr & 7) - e;
		const unsigned end = (e > 8) ? 16 : (e + 8);
		addr &= ~7;

		for (unsigned i = 0; i < 4; i++)
		{
			temp[i] = (uint16_t)READ_MEM_U8(rsp->dmem, (addr + (index + i * 4 & 0xf)) & 0xfff) << 7;
			temp[i+4] = (uint16_t)READ_MEM_U8(rsp->dmem, (addr + (index + i * 4 + 8 & 0xf)) & 0xfff) << 7;
		}

		for (unsigned i = e; i < end; i++)
			writeByteToHalfWords(rsp->cp2.regs[rt].e, i, byteFromHalfWords(temp, i));
	}

#define RSP_SFV_CASE(a,b,c,d) \
	WRITE_MEM_U8(rsp->dmem, addr + base, int16_t(reg[a]) >> 7); \
	WRITE_MEM_U8(rsp->dmem, addr + 4 + base, int16_t(reg[b]) >> 7); \
	WRITE_MEM_U8(rsp->dmem, addr + (8 + base & 0xf), int16_t(reg[c]) >> 7); \
	WRITE_MEM_U8(rsp->dmem, addr + (12 + base & 0xf), int16_t(reg[d]) >> 7);

	void RSP_SFV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SFV);
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		base = addr & 7;
		addr &= ~7;

		auto *reg = rsp->cp2.regs[rt].e;
		switch (e)
		{
		case 0:
		case 15:
			RSP_SFV_CASE(0,1,2,3)
			break;
		case 1:
			RSP_SFV_CASE(6,7,4,5)
			break;
		case 4:
			RSP_SFV_CASE(1,2,3,0)
			break;
		case 5:
			RSP_SFV_CASE(7,4,5,6)
			break;
		case 8:
			RSP_SFV_CASE(4,5,6,7)
			break;
		case 11:
			RSP_SFV_CASE(3,0,1,2)
			break;
		case 12:
			RSP_SFV_CASE(5,6,7,4)
			break;
		default:
			WRITE_MEM_U8(rsp->dmem, addr + base, 0);
			WRITE_MEM_U8(rsp->dmem, addr + 4 + base, 0);
			WRITE_MEM_U8(rsp->dmem, addr + (8 + base & 0xf), 0);
			WRITE_MEM_U8(rsp->dmem, addr + (12 + base & 0xf), 0);
			break;
		}
	}

	void RSP_LWV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LWV);
		unsigned addr = rsp->sr[base] + offset * 16;
		for (unsigned i = 16 - e; i < 16 + e; i++)
		{
			writeByteToHalfWords(rsp->cp2.regs[rt].e, i & 0xf, READ_MEM_U8(rsp->dmem, addr & 0xfff));
			addr += 4;
		}
	}

	void RSP_SWV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SWV);

		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		base = addr & 7;
		addr &= ~7;

		for (unsigned i = e; i < e + 16; i++)
			WRITE_MEM_U8(rsp->dmem, addr + (base++ & 0xf), byteFromHalfWords(rsp->cp2.regs[rt].e, i & 0xf));
	}

	void RSP_LQV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LQV);
		unsigned addr = rsp->sr[base] + offset * 16;
		unsigned end = 16 + e - (addr & 0xf);
		if (end > 16) end = 16;

		for (unsigned i = e; i < end; i++)
			writeByteToHalfWords(rsp->cp2.regs[rt].e, i & 0xf, READ_MEM_U8(rsp->dmem, addr++ & 0xfff));
	}

	void RSP_SQV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SQV);
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		
		const unsigned end = e + (16 - (addr & 15));
		for (unsigned i = e; i < end; i++)
			WRITE_MEM_U8(rsp->dmem, addr++, byteFromHalfWords(rsp->cp2.regs[rt].e, i & 15));
	}

	void RSP_LRV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LRV);
		unsigned addr = rsp->sr[base] + offset * 16;
		const unsigned start = 16 - ((addr & 0xf) - e);
		addr &= ~0xf;

		for (unsigned i = start; i < 16; i++)
			writeByteToHalfWords(rsp->cp2.regs[rt].e, i & 0xf, READ_MEM_U8(rsp->dmem, addr++ & 0xfff));
	}

	void RSP_SRV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SRV);
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		const unsigned end = e + (addr & 0xf);
		base = 16 - (addr & 0xf);
		addr &= ~0xf;

		for (unsigned i = e; i < end; i++)
			WRITE_MEM_U8(rsp->dmem, addr++, byteFromHalfWords(rsp->cp2.regs[rt].e, i + base & 0xf));
	}

	void RSP_LTV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LTV);
		unsigned addr = rsp->sr[base] + offset * 16;
		const unsigned start = addr & ~7;
		const unsigned vt0 = rt & ~7;
		addr = start + ((e + (addr & 8)) & 0xf);
		unsigned j = e >> 1;

		for (unsigned i = 0; i < 16; j++)
		{
			j &= 7;
			writeByteToHalfWords(rsp->cp2.regs[vt0+j].e, i++, READ_MEM_U8(rsp->dmem, addr++ & 0xfff));
			if (addr == start + 16) addr = start;
			writeByteToHalfWords(rsp->cp2.regs[vt0+j].e, i++, READ_MEM_U8(rsp->dmem, addr++ & 0xfff));
			if (addr == start + 16) addr = start;
		}
	}

	void RSP_STV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(STV);
		e &= ~1;
		rt &= ~7;

		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		unsigned element = 16 - e;
		base = (addr & 7) - e;
		addr &= ~7;

		for (unsigned i = rt; i < rt + 8; i++ )
		{
			WRITE_MEM_U8(rsp->dmem, addr + (base++ & 0xf), byteFromHalfWords(rsp->cp2.regs[i].e, element++ & 0xf));
			WRITE_MEM_U8(rsp->dmem, addr + (base++ & 0xf), byteFromHalfWords(rsp->cp2.regs[i].e, element++ & 0xf));
		}
	}
}
