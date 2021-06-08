#include "parallel_imp.h"
#include <memory>
#include <vector>
#include "rdp_device.hpp"
#include "context.hpp"
#include "device.hpp"

using namespace Vulkan;
using namespace std;

static int cmd_cur;
static int cmd_ptr;
static uint32_t cmd_data[0x00040000 >> 2];

static unique_ptr<RDP::CommandProcessor> frontend;
static unique_ptr<Device> device;
static unique_ptr<Context> context;

int32_t vk_rescaling;
bool vk_ssreadbacks;
bool vk_ssdither;
bool running = false;
unsigned width, height;
unsigned vk_overscan;
unsigned vk_downscaling_steps;
bool vk_native_texture_lod;
bool vk_native_tex_rect;
bool vk_synchronous, vk_divot_filter, vk_gamma_dither;
bool vk_vi_aa, vk_vi_scale, vk_dither_filter;
bool vk_interlacing;

static const unsigned cmd_len_lut[64] = {
	1, 1, 1, 1, 1, 1, 1, 1, 4, 6, 12, 14, 12, 14, 20, 22,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
};

void vk_blit(unsigned &width, unsigned &height)
{
	if (running)
	{

		RDP::ScanoutOptions opts = {};
		opts.persist_frame_on_invalid_input = true;
		opts.vi.aa = vk_vi_aa;
		opts.vi.scale = vk_vi_scale;
		opts.vi.dither_filter = vk_dither_filter;
		opts.vi.divot_filter = vk_divot_filter;
		opts.vi.gamma_dither = vk_gamma_dither;
		opts.blend_previous_frame = vk_interlacing;
		opts.upscale_deinterlacing = !vk_interlacing;
		opts.downscale_steps = vk_downscaling_steps;
		opts.crop_overscan_pixels = vk_overscan;

		RDP::VIScanoutBuffer scanout;
		frontend->scanout_async_buffer(scanout, opts);

		uint8_t* color_data = screen_get_texture_data();
		if (!scanout.width || !scanout.height)
		{
			width = 0;
			height = 0;
			memset(color_data, 0, width * height * sizeof(uint32_t));
			return;
		}

		width = scanout.width;
		height = scanout.height;

		scanout.fence->wait();
		memcpy(color_data, device->map_host_buffer(*scanout.buffer, Vulkan::MEMORY_ACCESS_READ_BIT),
			   width * height * sizeof(uint32_t));
		device->unmap_host_buffer(*scanout.buffer, Vulkan::MEMORY_ACCESS_READ_BIT);
	}
}

void vk_rasterize()
{

	if (frontend && running)
	{
		frontend->set_vi_register(RDP::VIRegister::Control, *GET_GFX_INFO(VI_STATUS_REG));
		frontend->set_vi_register(RDP::VIRegister::Origin, *GET_GFX_INFO(VI_ORIGIN_REG));
		frontend->set_vi_register(RDP::VIRegister::Width, *GET_GFX_INFO(VI_WIDTH_REG));
		frontend->set_vi_register(RDP::VIRegister::Intr, *GET_GFX_INFO(VI_INTR_REG));
		frontend->set_vi_register(RDP::VIRegister::VCurrentLine, *GET_GFX_INFO(VI_V_CURRENT_LINE_REG));
		frontend->set_vi_register(RDP::VIRegister::Timing, *GET_GFX_INFO(VI_V_BURST_REG));
		frontend->set_vi_register(RDP::VIRegister::VSync, *GET_GFX_INFO(VI_V_SYNC_REG));
		frontend->set_vi_register(RDP::VIRegister::HSync, *GET_GFX_INFO(VI_H_SYNC_REG));
		frontend->set_vi_register(RDP::VIRegister::Leap, *GET_GFX_INFO(VI_LEAP_REG));
		frontend->set_vi_register(RDP::VIRegister::HStart, *GET_GFX_INFO(VI_H_START_REG));
		frontend->set_vi_register(RDP::VIRegister::VStart, *GET_GFX_INFO(VI_V_START_REG));
		frontend->set_vi_register(RDP::VIRegister::VBurst, *GET_GFX_INFO(VI_V_BURST_REG));
		frontend->set_vi_register(RDP::VIRegister::XScale, *GET_GFX_INFO(VI_X_SCALE_REG));
		frontend->set_vi_register(RDP::VIRegister::YScale, *GET_GFX_INFO(VI_Y_SCALE_REG));

		RDP::Quirks quirks;
		quirks.set_native_texture_lod(vk_native_texture_lod);
		quirks.set_native_resolution_tex_rect(vk_native_tex_rect);
		frontend->set_quirks(quirks);

		frontend->begin_frame_context();

		unsigned width = 0;
		unsigned height = 0;
		vk_blit(width, height);

		if (width == 0 || height == 0)
		{
			screen_swap(true);
			return;
		}

		struct frame_buffer buf = {0};
		buf.valid = true;
		buf.height = height;
		buf.width = width;
		buf.pitch = width;
		screen_write(&buf);
		screen_swap(false);
	}
}

void vk_process_commands()
{
	if (running)
	{

		const uint32_t DP_CURRENT = *GET_GFX_INFO(DPC_CURRENT_REG) & 0x00FFFFF8;
		const uint32_t DP_END = *GET_GFX_INFO(DPC_END_REG) & 0x00FFFFF8;

		int length = DP_END - DP_CURRENT;
		if (length <= 0)
			return;

		length = unsigned(length) >> 3;
		if ((cmd_ptr + length) & ~(0x0003FFFF >> 3))
			return;

		uint32_t offset = DP_CURRENT;
		if (*GET_GFX_INFO(DPC_STATUS_REG) & DP_STATUS_XBUS_DMA)
		{
			do
			{
				offset &= 0xFF8;
				cmd_data[2 * cmd_ptr + 0] = *reinterpret_cast<const uint32_t *>(SP_DMEM + offset);
				cmd_data[2 * cmd_ptr + 1] = *reinterpret_cast<const uint32_t *>(SP_DMEM + offset + 4);
				offset += sizeof(uint64_t);
				cmd_ptr++;
			} while (--length > 0);
		}
		else
		{
			if (DP_END > 0x7ffffff || DP_CURRENT > 0x7ffffff)
			{
				return;
			}
			else
			{
				do
				{
					offset &= 0xFFFFF8;
					cmd_data[2 * cmd_ptr + 0] = *reinterpret_cast<const uint32_t *>(DRAM + offset);
					cmd_data[2 * cmd_ptr + 1] = *reinterpret_cast<const uint32_t *>(DRAM + offset + 4);
					offset += sizeof(uint64_t);
					cmd_ptr++;
				} while (--length > 0);
			}
		}

		while (cmd_cur - cmd_ptr < 0)
		{
			uint32_t w1 = cmd_data[2 * cmd_cur];
			uint32_t command = (w1 >> 24) & 63;
			int cmd_length = cmd_len_lut[command];

			if (cmd_ptr - cmd_cur - cmd_length < 0)
			{
				*GET_GFX_INFO(DPC_START_REG) = *GET_GFX_INFO(DPC_CURRENT_REG) = *GET_GFX_INFO(DPC_END_REG);
				return;
			}

			if (command >= 8 && frontend)
				frontend->enqueue_command(cmd_length * 2, &cmd_data[2 * cmd_cur]);

			if (RDP::Op(command) == RDP::Op::SyncFull)
			{
				// For synchronous RDP:
				if (vk_synchronous && frontend)
					frontend->wait_for_timeline(frontend->signal_timeline());
				*gfx.MI_INTR_REG |= DP_INTERRUPT;
				gfx.CheckInterrupts();
			}

			cmd_cur += cmd_length;
		}

		cmd_ptr = 0;
		cmd_cur = 0;
		*GET_GFX_INFO(DPC_START_REG) = *GET_GFX_INFO(DPC_CURRENT_REG) = *GET_GFX_INFO(DPC_END_REG);
	}
}

void vk_destroy()
{
	running = false;
	frontend.reset();
	device.reset();
	context.reset();

	screen_close();
}

bool vk_init()
{
	running = false;
	screen_init();
	context.reset(new Context);
	device.reset(new Device);
	frontend.reset();

	if (!::Vulkan::Context::init_loader(nullptr))
		return false;
	if (!context->init_instance_and_device(nullptr, 0, nullptr, 0, ::Vulkan::CONTEXT_CREATION_DISABLE_BINDLESS_BIT))
		return false;

	uintptr_t aligned_rdram = reinterpret_cast<uintptr_t>(gfx.RDRAM);
	uintptr_t offset = 0;


	if (device->get_device_features().supports_external_memory_host)
	{
		size_t align = device->get_device_features().host_memory_properties.minImportedHostPointerAlignment;
		offset = aligned_rdram & (align - 1);

		if (offset)
		{
			return false;
		}
		aligned_rdram -= offset;
	}

	device->set_context(*context);
	device->init_frame_contexts(3);
	::RDP::CommandProcessorFlags flags = 0;

	switch (vk_rescaling)
	{
	case 1:
		break;
	case 2:
		flags |= RDP::COMMAND_PROCESSOR_FLAG_UPSCALING_2X_BIT;
		break;
	case 4:
		flags |= RDP::COMMAND_PROCESSOR_FLAG_UPSCALING_4X_BIT;
		break;
	case 8:
		flags |= RDP::COMMAND_PROCESSOR_FLAG_UPSCALING_8X_BIT;
		break;

	default:
		break;
	}
	if (vk_rescaling >1 && vk_ssreadbacks)
		flags |= RDP::COMMAND_PROCESSOR_FLAG_SUPER_SAMPLED_READ_BACK_BIT;
	if (vk_ssdither)
		flags |= RDP::COMMAND_PROCESSOR_FLAG_SUPER_SAMPLED_DITHER_BIT;

	frontend.reset(new RDP::CommandProcessor(*device, reinterpret_cast<void *>(aligned_rdram),
											 offset, rdram_size, rdram_size / 2, flags));
	if (!frontend->device_is_supported())
	{
		frontend.reset();
		return false;
	}

	RDP::Quirks quirks;
	quirks.set_native_texture_lod(vk_native_texture_lod);
	quirks.set_native_resolution_tex_rect(vk_native_tex_rect);
	frontend->set_quirks(quirks);

	running = true;
	return true;
}
