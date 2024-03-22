#include "../state.hpp"

#ifdef PARALLEL_INTEGRATION
#include "../rsp_1.1.h"
#include "m64p_plugin.h"
namespace RSP
{
extern RSP_INFO rsp;
} // namespace RSP
#endif

using namespace RSP;

extern "C"
{

#ifdef INTENSE_DEBUG
	void log_rsp_mem_parallel(void);
#endif

	int RSP_MFC0(RSP::CPUState *rsp, unsigned rt, unsigned rd)
	{
		rd &= 15;
		uint32_t res = *rsp->cp0.cr[rd];
		if (rt)
			rsp->sr[rt] = res;

#ifdef PARALLEL_INTEGRATION
		if (rd == CP0_REGISTER_SP_SEMAPHORE)
		{
			*rsp->cp0.cr[CP0_REGISTER_SP_SEMAPHORE] = 1;
			return MODE_EXIT;
		}
		// We don't return control to the CPU if the RDP FREEZE bit is set, doing so seems to cause flickering
		else if (rd == CP0_REGISTER_SP_STATUS && (*rsp->cp0.cr[CP0_REGISTER_CMD_STATUS] & DPC_STATUS_FREEZE) == 0)
		{
			return MODE_EXIT;
		}
#endif

		//if (rd == 4) // SP_STATUS_REG
		//   fprintf(stderr, "READING STATUS REG!\n");

		return MODE_CONTINUE;
	}

	static inline void rdp_status_write(RSP::CPUState *rsp, uint32_t rt)
	{
		uint32_t status = *rsp->cp0.cr[CP0_REGISTER_CMD_STATUS];
		if (rt & DPC_CLR_XBUS_DMEM_DMA)
			status &= ~DPC_STATUS_XBUS_DMEM_DMA;
		else if (rt & DPC_SET_XBUS_DMEM_DMA)
			status |= DPC_STATUS_XBUS_DMEM_DMA;

		if (rt & DPC_CLR_FREEZE)
			status &= ~DPC_STATUS_FREEZE;
		else if (rt & DPC_SET_FREEZE)
			status |= DPC_STATUS_FREEZE;

		if (rt & DPC_CLR_FLUSH)
			status &= ~DPC_STATUS_FLUSH;
		else if (rt & DPC_SET_FLUSH)
			status |= DPC_STATUS_FLUSH;

		if (rt & DPC_CLR_TMEM_CTR)
		{
			status &= ~DPC_STATUS_TMEM_BUSY;
			*rsp->cp0.cr[CP0_REGISTER_CMD_TMEM_BUSY] = 0;
		}
		if (rt & DPC_CLR_PIPE_CTR)
		{
			status &= ~DPC_STATUS_PIPE_BUSY;
			*rsp->cp0.cr[CP0_REGISTER_CMD_PIPE_BUSY] = 0;
		}
		if (rt & DPC_CLR_CMD_CTR)
		{
			status &= ~DPC_STATUS_CMD_BUSY;
			*rsp->cp0.cr[CP0_REGISTER_CMD_BUSY] = 0;
		}

		if (rt & DPC_CLR_CLOCK_CTR)
			*rsp->cp0.cr[CP0_REGISTER_CMD_CLOCK] = 0;

		*rsp->cp0.cr[CP0_REGISTER_CMD_STATUS] = status;
	}

	static inline int rsp_status_write(RSP::CPUState *rsp, uint32_t rt)
	{
		//fprintf(stderr, "Writing 0x%x to status reg!\n", rt);

		uint32_t status = *rsp->cp0.cr[CP0_REGISTER_SP_STATUS];

		if ((rt & SP_CLR_HALT) && !(rt & SP_SET_HALT))
			status &= ~SP_STATUS_HALT;
		if ((rt & SP_SET_HALT) && !(rt & SP_CLR_HALT))
			status |= SP_STATUS_HALT;

		if (rt & SP_CLR_BROKE)
			status &= ~SP_STATUS_BROKE;

		if ((rt & SP_CLR_INTR) && !(rt & SP_SET_INTR))
			*rsp->cp0.irq &= ~1;
		if ((rt & SP_SET_INTR) && !(rt & SP_CLR_INTR))
			*rsp->cp0.irq |= 1;

		if ((rt & SP_CLR_SSTEP) && !(rt & SP_SET_SSTEP))
			status &= ~SP_STATUS_SSTEP;
		if ((rt & SP_SET_SSTEP) && !(rt & SP_CLR_SSTEP))
			status |= SP_STATUS_SSTEP;

		if ((rt & SP_CLR_INTR_BREAK) && !(rt & SP_SET_INTR_BREAK))
			status &= ~SP_STATUS_INTR_BREAK;
		if ((rt & SP_SET_INTR_BREAK) && !(rt & SP_CLR_INTR_BREAK))
			status |= SP_STATUS_INTR_BREAK;

		if ((rt & SP_CLR_SIG0) && !(rt & SP_SET_SIG0))
			status &= ~SP_STATUS_SIG0;
		if ((rt & SP_SET_SIG0) && !(rt & SP_CLR_SIG0))
			status |= SP_STATUS_SIG0;

		if ((rt & SP_CLR_SIG1) && !(rt & SP_SET_SIG1))
			status &= ~SP_STATUS_SIG1;
		if ((rt & SP_SET_SIG1) && !(rt & SP_CLR_SIG1))
			status |= SP_STATUS_SIG1;

		if ((rt & SP_CLR_SIG2) && !(rt & SP_SET_SIG2))
			status &= ~SP_STATUS_SIG2;
		if ((rt & SP_SET_SIG2) && !(rt & SP_CLR_SIG2))
			status |= SP_STATUS_SIG2;

		if ((rt & SP_CLR_SIG3) && !(rt & SP_SET_SIG3))
			status &= ~SP_STATUS_SIG3;
		if ((rt & SP_SET_SIG3) && !(rt & SP_CLR_SIG3))
			status |= SP_STATUS_SIG3;

		if ((rt & SP_CLR_SIG4) && !(rt & SP_SET_SIG4))
			status &= ~SP_STATUS_SIG4;
		if ((rt & SP_SET_SIG4) && !(rt & SP_CLR_SIG4))
			status |= SP_STATUS_SIG4;

		if ((rt & SP_CLR_SIG5) && !(rt & SP_SET_SIG5))
			status &= ~SP_STATUS_SIG5;
		if ((rt & SP_SET_SIG5) && !(rt & SP_CLR_SIG5))
			status |= SP_STATUS_SIG5;

		if ((rt & SP_CLR_SIG6) && !(rt & SP_SET_SIG6))
			status &= ~SP_STATUS_SIG6;
		if ((rt & SP_SET_SIG6) && !(rt & SP_CLR_SIG6))
			status |= SP_STATUS_SIG6;

		if ((rt & SP_CLR_SIG7) && !(rt & SP_SET_SIG7))
			status &= ~SP_STATUS_SIG7;
		if ((rt & SP_SET_SIG7) && !(rt & SP_CLR_SIG7))
			status |= SP_STATUS_SIG7;

		*rsp->cp0.cr[CP0_REGISTER_SP_STATUS] = status;
		return ((*rsp->cp0.irq & 1) || (status & SP_STATUS_HALT)) ? MODE_CHECK_FLAGS : MODE_CONTINUE;
	}

#ifdef PARALLEL_INTEGRATION
	static int rsp_dma_read(RSP::CPUState *rsp)
	{
		uint32_t length_reg = *rsp->cp0.cr[CP0_REGISTER_DMA_READ_LENGTH];
		uint32_t length = ((length_reg & 0xFFF) | 7) + 1;
		uint32_t skip = (length_reg >> 20) & 0xFF8;
		unsigned count = ((length_reg >> 12) & 0xFF) + 1;

		// Check length.
		if (((*rsp->cp0.cr[CP0_REGISTER_DMA_CACHE] & 0xFFF) + length) > 0x1000)
			length = 0x1000 - (*rsp->cp0.cr[CP0_REGISTER_DMA_CACHE] & 0xFFF);

		unsigned i = 0;
		uint32_t source = *rsp->cp0.cr[CP0_REGISTER_DMA_DRAM];
		uint32_t dest = *rsp->cp0.cr[CP0_REGISTER_DMA_CACHE];

#ifdef INTENSE_DEBUG
		fprintf(stderr, "DMA READ: (0x%x <- 0x%x) len %u, count %u, skip %u\n", dest & 0x1ffc, source & 0x7ffffc,
		        length, count + 1, skip);
#endif

		do
		{
			unsigned j = 0;
			do
			{
				uint32_t source_addr = (source + j) & 0x7FFFFC;
				uint32_t dest_addr = (dest + j) & 0x1FFC;
				uint32_t word = rsp->rdram[source_addr >> 2];

				if (dest_addr & 0x1000)
				{
					// Invalidate IMEM.
					unsigned block = (dest_addr & 0xfff) / CODE_BLOCK_SIZE;
					rsp->dirty_blocks |= (0x3 << block) >> 1;
					//rsp->dirty_blocks = ~0u;
					rsp->imem[(dest_addr & 0xfff) >> 2] = word;
				}
				else
					rsp->dmem[dest_addr >> 2] = word;

				j += 4;
			} while (j < length);

			source += length + skip;
			dest += length;
		} while (++i < count);

		*rsp->cp0.cr[CP0_REGISTER_DMA_DRAM] = source;
		*rsp->cp0.cr[CP0_REGISTER_DMA_CACHE] = dest;
		*rsp->cp0.cr[CP0_REGISTER_DMA_READ_LENGTH] = 0xff8;

#ifdef INTENSE_DEBUG
		log_rsp_mem_parallel();
#endif
		return rsp->dirty_blocks ? MODE_CHECK_FLAGS : MODE_CONTINUE;
	}

	static void rsp_dma_write(RSP::CPUState *rsp)
	{
		uint32_t length_reg = *rsp->cp0.cr[CP0_REGISTER_DMA_WRITE_LENGTH];
		uint32_t length = ((length_reg & 0xFFF) | 7) + 1;
		uint32_t skip = (length_reg >> 20) & 0xFF8;
		unsigned count = ((length_reg >> 12) & 0xFF) + 1;

		// Check length.
		if (((*rsp->cp0.cr[CP0_REGISTER_DMA_CACHE] & 0xFFF) + length) > 0x1000)
			length = 0x1000 - (*rsp->cp0.cr[CP0_REGISTER_DMA_CACHE] & 0xFFF);

		uint32_t dest = *rsp->cp0.cr[CP0_REGISTER_DMA_DRAM];
		uint32_t source = *rsp->cp0.cr[CP0_REGISTER_DMA_CACHE];

#ifdef INTENSE_DEBUG
		fprintf(stderr, "DMA WRITE: (0x%x <- 0x%x) len %u, count %u, skip %u\n", dest & 0x7ffffc, source & 0x1ffc,
		        length, count + 1, skip);
#endif

		unsigned i = 0;
		do
		{
			unsigned j = 0;

			do
			{
				uint32_t source_addr = (source + j) & 0x1FFC;
				uint32_t dest_addr = (dest + j) & 0x7FFFFC;

				rsp->rdram[dest_addr >> 2] =
				    (source_addr & 0x1000) ? rsp->imem[(source_addr & 0xfff) >> 2] : rsp->dmem[source_addr >> 2];

				j += 4;
			} while (j < length);

			source += length;
			dest += length + skip;
		} while (++i < count);

		*rsp->cp0.cr[CP0_REGISTER_DMA_CACHE] = source;
		*rsp->cp0.cr[CP0_REGISTER_DMA_DRAM] = dest;
		*rsp->cp0.cr[CP0_REGISTER_DMA_WRITE_LENGTH] = 0xff8;
#ifdef INTENSE_DEBUG
		log_rsp_mem_parallel();
#endif
	}
#endif

	int RSP_MTC0(RSP::CPUState *rsp, unsigned rd, unsigned rt)
	{
		uint32_t val = rsp->sr[rt];

		switch (static_cast<CP0Registers>(rd & 15))
		{
		case CP0_REGISTER_DMA_CACHE:
			*rsp->cp0.cr[CP0_REGISTER_DMA_CACHE] = val & 0x1ff8;
			break;

		case CP0_REGISTER_DMA_DRAM:
			*rsp->cp0.cr[CP0_REGISTER_DMA_DRAM] = val & 0xfffff8;
			break;

		case CP0_REGISTER_DMA_READ_LENGTH:
			*rsp->cp0.cr[CP0_REGISTER_DMA_READ_LENGTH] = val;
#ifdef PARALLEL_INTEGRATION
			return rsp_dma_read(rsp);
#else
			return MODE_DMA_READ;
#endif

		case CP0_REGISTER_DMA_WRITE_LENGTH:
			*rsp->cp0.cr[CP0_REGISTER_DMA_WRITE_LENGTH] = val;
#ifdef PARALLEL_INTEGRATION
			rsp_dma_write(rsp);
#endif
			break;

		case CP0_REGISTER_SP_STATUS:
			return rsp_status_write(rsp, val);

		case CP0_REGISTER_SP_SEMAPHORE:
			*rsp->cp0.cr[CP0_REGISTER_SP_SEMAPHORE] = 0;
			break;

		case CP0_REGISTER_CMD_START:
#ifdef INTENSE_DEBUG
			fprintf(stderr, "CMD_START 0x%x\n", val & 0xfffff8u);
#endif
			if (!(*rsp->cp0.cr[CP0_REGISTER_CMD_STATUS] & DPC_STATUS_START_VALID))
			{
				*rsp->cp0.cr[CP0_REGISTER_CMD_START] = val & 0xfffff8u;
			}
			*rsp->cp0.cr[CP0_REGISTER_CMD_STATUS] |= DPC_STATUS_START_VALID;
			break;

		case CP0_REGISTER_CMD_END:
#ifdef INTENSE_DEBUG
			fprintf(stderr, "CMD_END 0x%x\n", val & 0xfffff8u);
#endif
			*rsp->cp0.cr[CP0_REGISTER_CMD_END] = val & 0xfffff8u;
			if (*rsp->cp0.cr[CP0_REGISTER_CMD_STATUS] & DPC_STATUS_START_VALID)
			{
				*rsp->cp0.cr[CP0_REGISTER_CMD_CURRENT] = *rsp->cp0.cr[CP0_REGISTER_CMD_START];
				*rsp->cp0.cr[CP0_REGISTER_CMD_STATUS] &= ~DPC_STATUS_START_VALID;
			}
#ifdef PARALLEL_INTEGRATION
			RSP::rsp.ProcessRdpList();
			if (*rsp->cp0.irq & 0x20)
				return MODE_EXIT;
#endif
			break;

		case CP0_REGISTER_CMD_CLOCK:
			*rsp->cp0.cr[CP0_REGISTER_CMD_CLOCK] = val;
			break;

		case CP0_REGISTER_CMD_STATUS:
			rdp_status_write(rsp, val);
			break;

		case CP0_REGISTER_CMD_CURRENT:
		case CP0_REGISTER_CMD_BUSY:
		case CP0_REGISTER_CMD_PIPE_BUSY:
		case CP0_REGISTER_CMD_TMEM_BUSY:
			break;

		default:
			*rsp->cp0.cr[rd & 15] = val;
			break;
		}

		return MODE_CONTINUE;
	}
}
