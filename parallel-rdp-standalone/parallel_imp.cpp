// Adapted from https://github.com/Themaister/parallel-rdp/blob/master/integration_example.cpp
#include "parallel_imp.h"
#include "vkguts.h"
#include <memory>
#include <vector>
#include "rdp_device.hpp"
#include "wsi.hpp"
#include "wsi_platform.h"

using namespace Vulkan;
using namespace std;

static int cmd_cur;
static int cmd_ptr;
static uint32_t cmd_data[0x00040000 >> 2];

static RDP::CommandProcessor* processor;
static WSI* wsi;
static QT_WSIPlatform* wsi_platform;

int32_t vk_rescaling;
bool vk_ssreadbacks;
bool vk_ssdither;
unsigned vk_overscan;
unsigned vk_vertical_stretch;
unsigned vk_downscaling_steps;
bool vk_native_texture_lod;
bool vk_native_tex_rect;
bool vk_synchronous, vk_divot_filter, vk_gamma_dither;
bool vk_vi_aa, vk_vi_scale, vk_dither_filter;
bool vk_interlacing;

static uint64_t rdp_sync_signal;
static uint64_t last_frame_counter;

uint64_t frame_counter;

static const unsigned cmd_len_lut[64] = {
	1, 1, 1, 1, 1, 1, 1, 1, 4, 6, 12, 14, 12, 14, 20, 22,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1,  1,  1,
};

static const uint32_t vertex_spirv[] =
		{0x07230203,0x00010000,0x000d000a,0x00000034,
		 0x00000000,0x00020011,0x00000001,0x0006000b,
		 0x00000001,0x4c534c47,0x6474732e,0x3035342e,
		 0x00000000,0x0003000e,0x00000000,0x00000001,
		 0x0008000f,0x00000000,0x00000004,0x6e69616d,
		 0x00000000,0x00000008,0x00000016,0x0000002b,
		 0x00040047,0x00000008,0x0000000b,0x0000002a,
		 0x00050048,0x00000014,0x00000000,0x0000000b,
		 0x00000000,0x00050048,0x00000014,0x00000001,
		 0x0000000b,0x00000001,0x00050048,0x00000014,
		 0x00000002,0x0000000b,0x00000003,0x00050048,
		 0x00000014,0x00000003,0x0000000b,0x00000004,
		 0x00030047,0x00000014,0x00000002,0x00040047,
		 0x0000002b,0x0000001e,0x00000000,0x00020013,
		 0x00000002,0x00030021,0x00000003,0x00000002,
		 0x00040015,0x00000006,0x00000020,0x00000001,
		 0x00040020,0x00000007,0x00000001,0x00000006,
		 0x0004003b,0x00000007,0x00000008,0x00000001,
		 0x0004002b,0x00000006,0x0000000a,0x00000000,
		 0x00020014,0x0000000b,0x00030016,0x0000000f,
		 0x00000020,0x00040017,0x00000010,0x0000000f,
		 0x00000004,0x00040015,0x00000011,0x00000020,
		 0x00000000,0x0004002b,0x00000011,0x00000012,
		 0x00000001,0x0004001c,0x00000013,0x0000000f,
		 0x00000012,0x0006001e,0x00000014,0x00000010,
		 0x0000000f,0x00000013,0x00000013,0x00040020,
		 0x00000015,0x00000003,0x00000014,0x0004003b,
		 0x00000015,0x00000016,0x00000003,0x0004002b,
		 0x0000000f,0x00000017,0xbf800000,0x0004002b,
		 0x0000000f,0x00000018,0x00000000,0x0004002b,
		 0x0000000f,0x00000019,0x3f800000,0x0007002c,
		 0x00000010,0x0000001a,0x00000017,0x00000017,
		 0x00000018,0x00000019,0x00040020,0x0000001b,
		 0x00000003,0x00000010,0x0004002b,0x00000006,
		 0x0000001f,0x00000001,0x0004002b,0x0000000f,
		 0x00000023,0x40400000,0x0007002c,0x00000010,
		 0x00000024,0x00000017,0x00000023,0x00000018,
		 0x00000019,0x0007002c,0x00000010,0x00000027,
		 0x00000023,0x00000017,0x00000018,0x00000019,
		 0x00040017,0x00000029,0x0000000f,0x00000002,
		 0x00040020,0x0000002a,0x00000003,0x00000029,
		 0x0004003b,0x0000002a,0x0000002b,0x00000003,
		 0x0004002b,0x0000000f,0x0000002f,0x3f000000,
		 0x0005002c,0x00000029,0x00000033,0x0000002f,
		 0x0000002f,0x00050036,0x00000002,0x00000004,
		 0x00000000,0x00000003,0x000200f8,0x00000005,
		 0x0004003d,0x00000006,0x00000009,0x00000008,
		 0x000500aa,0x0000000b,0x0000000c,0x00000009,
		 0x0000000a,0x000300f7,0x0000000e,0x00000000,
		 0x000400fa,0x0000000c,0x0000000d,0x0000001d,
		 0x000200f8,0x0000000d,0x00050041,0x0000001b,
		 0x0000001c,0x00000016,0x0000000a,0x0003003e,
		 0x0000001c,0x0000001a,0x000200f9,0x0000000e,
		 0x000200f8,0x0000001d,0x000500aa,0x0000000b,
		 0x00000020,0x00000009,0x0000001f,0x000300f7,
		 0x00000022,0x00000000,0x000400fa,0x00000020,
		 0x00000021,0x00000026,0x000200f8,0x00000021,
		 0x00050041,0x0000001b,0x00000025,0x00000016,
		 0x0000000a,0x0003003e,0x00000025,0x00000024,
		 0x000200f9,0x00000022,0x000200f8,0x00000026,
		 0x00050041,0x0000001b,0x00000028,0x00000016,
		 0x0000000a,0x0003003e,0x00000028,0x00000027,
		 0x000200f9,0x00000022,0x000200f8,0x00000022,
		 0x000200f9,0x0000000e,0x000200f8,0x0000000e,
		 0x00050041,0x0000001b,0x0000002c,0x00000016,
		 0x0000000a,0x0004003d,0x00000010,0x0000002d,
		 0x0000002c,0x0007004f,0x00000029,0x0000002e,
		 0x0000002d,0x0000002d,0x00000000,0x00000001,
		 0x0005008e,0x00000029,0x00000030,0x0000002e,
		 0x0000002f,0x00050081,0x00000029,0x00000032,
		 0x00000030,0x00000033,0x0003003e,0x0000002b,
		 0x00000032,0x000100fd,0x00010038};

static const uint32_t fragment_spirv[] =
		{0x07230203,0x00010000,0x000d000a,0x00000015,
		 0x00000000,0x00020011,0x00000001,0x0006000b,
		 0x00000001,0x4c534c47,0x6474732e,0x3035342e,
		 0x00000000,0x0003000e,0x00000000,0x00000001,
		 0x0007000f,0x00000004,0x00000004,0x6e69616d,
		 0x00000000,0x00000009,0x00000011,0x00030010,
		 0x00000004,0x00000007,0x00040047,0x00000009,
		 0x0000001e,0x00000000,0x00040047,0x0000000d,
		 0x00000022,0x00000000,0x00040047,0x0000000d,
		 0x00000021,0x00000000,0x00040047,0x00000011,
		 0x0000001e,0x00000000,0x00020013,0x00000002,
		 0x00030021,0x00000003,0x00000002,0x00030016,
		 0x00000006,0x00000020,0x00040017,0x00000007,
		 0x00000006,0x00000004,0x00040020,0x00000008,
		 0x00000003,0x00000007,0x0004003b,0x00000008,
		 0x00000009,0x00000003,0x00090019,0x0000000a,
		 0x00000006,0x00000001,0x00000000,0x00000000,
		 0x00000000,0x00000001,0x00000000,0x0003001b,
		 0x0000000b,0x0000000a,0x00040020,0x0000000c,
		 0x00000000,0x0000000b,0x0004003b,0x0000000c,
		 0x0000000d,0x00000000,0x00040017,0x0000000f,
		 0x00000006,0x00000002,0x00040020,0x00000010,
		 0x00000001,0x0000000f,0x0004003b,0x00000010,
		 0x00000011,0x00000001,0x0004002b,0x00000006,
		 0x00000013,0x00000000,0x00050036,0x00000002,
		 0x00000004,0x00000000,0x00000003,0x000200f8,
		 0x00000005,0x0004003d,0x0000000b,0x0000000e,
		 0x0000000d,0x0004003d,0x0000000f,0x00000012,
		 0x00000011,0x00070058,0x00000007,0x00000014,
		 0x0000000e,0x00000012,0x00000002,0x00000013,
		 0x0003003e,0x00000009,0x00000014,0x000100fd,
		 0x00010038};

static void calculate_viewport(float* x, float* y, float* width, float* height)
{
    int32_t display_width = (window_widescreen ? 854 : 640) * vk_rescaling;
    int32_t display_height = 480 * vk_rescaling;

    *width = window_width;
    *height = window_height;
    *x = 0;
    *y = 0;
    int32_t hw = display_height * *width;
    int32_t wh = display_width * *height;

    // add letterboxes or pillarboxes if the window has a different aspect ratio
    // than the current display mode
    if (hw > wh) {
        int32_t w_max = wh / display_height;
        *x += (*width - w_max) / 2;
        *width = w_max;
    } else if (hw < wh) {
        int32_t h_max = hw / display_width;
        *y += (*height - h_max) / 2;
        *height = h_max;
    }
}

static void render_frame(Vulkan::Device &device)
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
	if (vk_vertical_stretch) {
		opts.crop_rect.top = vk_vertical_stretch;
		opts.crop_rect.bottom = vk_vertical_stretch;
		opts.crop_rect.enable = true;
	}
	Vulkan::ImageHandle image = processor->scanout(opts);

	// Normally reflection is automated.
	Vulkan::ResourceLayout vertex_layout = {};
	Vulkan::ResourceLayout fragment_layout = {};
	fragment_layout.output_mask = 1 << 0;
	fragment_layout.sets[0].sampled_image_mask = 1 << 0;

	// This request is cached.
	auto *program = device.request_program(vertex_spirv, sizeof(vertex_spirv),
	                                       fragment_spirv, sizeof(fragment_spirv),
	                                       &vertex_layout,
	                                       &fragment_layout);

	// Blit image on screen.
	auto cmd = device.request_command_buffer();
	{
		auto rp = device.get_swapchain_render_pass(Vulkan::SwapchainRenderPass::ColorOnly);
		cmd->begin_render_pass(rp);
		if (image.get() != NULL)
		{
			VkViewport vp = cmd->get_viewport();
			calculate_viewport(&vp.x, &vp.y, &vp.width, &vp.height);

			cmd->set_program(program);

			// Basic default render state.
			cmd->set_opaque_state();
			cmd->set_depth_test(false, false);
			cmd->set_cull_mode(VK_CULL_MODE_NONE);

			cmd->set_texture(0, 0, image->get_view(), Vulkan::StockSampler::NearestClamp);
			cmd->set_viewport(vp);

			// The vertices are constants in the shader.
			// Draws fullscreen quad using oversized triangle.
			cmd->draw(3);
		}
		cmd->end_render_pass();
	}
	device.submit(cmd);
}

void vk_rasterize()
{
	processor->set_vi_register(RDP::VIRegister::Control, *GET_GFX_INFO(VI_STATUS_REG));
	processor->set_vi_register(RDP::VIRegister::Origin, *GET_GFX_INFO(VI_ORIGIN_REG));
	processor->set_vi_register(RDP::VIRegister::Width, *GET_GFX_INFO(VI_WIDTH_REG));
	processor->set_vi_register(RDP::VIRegister::Intr, *GET_GFX_INFO(VI_INTR_REG));
	processor->set_vi_register(RDP::VIRegister::VCurrentLine, *GET_GFX_INFO(VI_V_CURRENT_LINE_REG));
	processor->set_vi_register(RDP::VIRegister::Timing, *GET_GFX_INFO(VI_V_BURST_REG));
	processor->set_vi_register(RDP::VIRegister::VSync, *GET_GFX_INFO(VI_V_SYNC_REG));
	processor->set_vi_register(RDP::VIRegister::HSync, *GET_GFX_INFO(VI_H_SYNC_REG));
	processor->set_vi_register(RDP::VIRegister::Leap, *GET_GFX_INFO(VI_LEAP_REG));
	processor->set_vi_register(RDP::VIRegister::HStart, *GET_GFX_INFO(VI_H_START_REG));
	processor->set_vi_register(RDP::VIRegister::VStart, *GET_GFX_INFO(VI_V_START_REG));
	processor->set_vi_register(RDP::VIRegister::VBurst, *GET_GFX_INFO(VI_V_BURST_REG));
	processor->set_vi_register(RDP::VIRegister::XScale, *GET_GFX_INFO(VI_X_SCALE_REG));
	processor->set_vi_register(RDP::VIRegister::YScale, *GET_GFX_INFO(VI_Y_SCALE_REG));

	RDP::Quirks quirks;
	quirks.set_native_texture_lod(vk_native_texture_lod);
	quirks.set_native_resolution_tex_rect(vk_native_tex_rect);
	processor->set_quirks(quirks);

	auto &device = wsi->get_device();
	render_frame(device);
	(*render_callback)(1);
	wsi->end_frame();
	screen_frame_count();
	wsi->begin_frame();
}

void vk_read_screen(unsigned char* dest)
{
	Vulkan::Fence fence;
	Vulkan::BufferHandle buffer;
	Vulkan::BufferCreateInfo info = {};
	Vulkan::ImageView* image = &wsi->get_device().get_swapchain_view();
	info.size = window_width * window_height * sizeof(uint32_t);
	info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	info.domain = Vulkan::BufferDomain::CachedHost;
	if (!buffer || buffer->get_create_info().size < info.size)
		buffer = wsi->get_device().create_buffer(info);

	auto cmd = wsi->get_device().request_command_buffer();
	cmd->copy_image_to_buffer(*buffer, image->get_image(), 0, {}, { window_width, window_height, 1 }, 0, 0, { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 });
	cmd->barrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_ACCESS_TRANSFER_WRITE_BIT,
	             VK_PIPELINE_STAGE_HOST_BIT, VK_ACCESS_HOST_READ_BIT);

	fence.reset();
	wsi->get_device().submit(cmd, &fence);

	fence->wait();
	unsigned char *image_buffer = (unsigned char*)wsi->get_device().map_host_buffer(*buffer, Vulkan::MEMORY_ACCESS_READ_BIT);

	image_buffer = image_buffer + info.size;
	for (int i = 0; i < window_height; ++i)
	{
		image_buffer -= window_width * 4;
		for (int j = 0; j < window_width; ++j)
		{
			switch (image->get_format()) {
				case VK_FORMAT_B8G8R8A8_UNORM:
					dest[0] = image_buffer[2];
					dest[1] = image_buffer[1];
					dest[2] = image_buffer[0];
					break;
				case VK_FORMAT_R8G8B8A8_UNORM:
					dest[0] = image_buffer[0];
					dest[1] = image_buffer[1];
					dest[2] = image_buffer[2];
					break;
				case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
					dest[0] = image_buffer[3];
					dest[1] = image_buffer[2];
					dest[2] = image_buffer[1];
					break;
				default:
					wsi->get_device().unmap_host_buffer(*buffer, Vulkan::MEMORY_ACCESS_READ_BIT);
					return;
			}
			dest += 3;
			image_buffer += 4;
		}
		image_buffer -= window_width * 4;
	}
	wsi->get_device().unmap_host_buffer(*buffer, Vulkan::MEMORY_ACCESS_READ_BIT);
}

void vk_process_commands()
{
	const uint32_t DP_CURRENT = *GET_GFX_INFO(DPC_CURRENT_REG) & 0x00FFFFF8;
	const uint32_t DP_END = *GET_GFX_INFO(DPC_END_REG) & 0x00FFFFF8;

	int length = DP_END - DP_CURRENT;
	if (length <= 0)
		return;

	length = unsigned(length) >> 3;
	if ((cmd_ptr + length) & ~(0x0003FFFF >> 3))
		return;

	*GET_GFX_INFO(DPC_STATUS_REG) |= DP_STATUS_PIPE_BUSY | DP_STATUS_START_GCLK;

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

		if (command >= 8)
			processor->enqueue_command(cmd_length * 2, &cmd_data[2 * cmd_cur]);

		if (RDP::Op(command) == RDP::Op::SyncFull)
		{
			// For synchronous RDP:
			if (vk_synchronous)
			{
				if (frame_counter != last_frame_counter) // Only sync once per frame
				{
					rdp_sync_signal = processor->signal_timeline();
					last_frame_counter = frame_counter;
				}
				else
				{
					rdp_sync_signal = 0;
				}
			}
			*gfx.MI_INTR_REG |= DP_INTERRUPT;
			*GET_GFX_INFO(DPC_STATUS_REG) &= ~(DP_STATUS_PIPE_BUSY | DP_STATUS_START_GCLK);
			gfx.CheckInterrupts();
		}

		cmd_cur += cmd_length;
	}

	cmd_ptr = 0;
	cmd_cur = 0;
	*GET_GFX_INFO(DPC_CURRENT_REG) = *GET_GFX_INFO(DPC_END_REG);
	*GET_GFX_INFO(DPC_STATUS_REG) |= DP_STATUS_CBUF_READY;
}

void vk_full_sync()
{
	if (vk_synchronous && rdp_sync_signal)
		processor->wait_for_timeline(rdp_sync_signal);
}

void vk_resize()
{
	QT_WSIPlatform* platform = (QT_WSIPlatform*)&wsi->get_platform();
	platform->do_resize();
}

void vk_destroy()
{
	wsi->end_frame();
	screen_close();
	if (processor)
	{
		delete processor;
		processor = nullptr;
	}
	if (wsi)
	{
		delete wsi;
		wsi = nullptr;
	}
	if (wsi_platform)
	{
		delete wsi_platform;
		wsi_platform = nullptr;
	}
}

bool vk_init()
{
	screen_init();
	wsi = new WSI;
	wsi_platform = new QT_WSIPlatform;
	wsi->set_platform(wsi_platform);
	wsi->set_present_mode(window_vsync ? PresentMode::SyncToVBlank: PresentMode::UnlockedMaybeTear);
	wsi->set_backbuffer_srgb(false);
	Context::SystemHandles handles = {};
	if (!::Vulkan::Context::init_loader(nullptr))
		return false;
	if (!wsi->init_simple(1, handles))
		return false;

	uintptr_t aligned_rdram = reinterpret_cast<uintptr_t>(gfx.RDRAM);
	uintptr_t offset = 0;

	if (wsi->get_device().get_device_features().supports_external_memory_host)
	{
		size_t align = wsi->get_device().get_device_features().host_memory_properties.minImportedHostPointerAlignment;
		offset = aligned_rdram & (align - 1);

		if (offset)
		{
			return false;
		}
		aligned_rdram -= offset;
	}

	RDP::CommandProcessorFlags flags = 0;

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
	default:
		break;
	}
	if (vk_rescaling > 1 && vk_ssreadbacks)
		flags |= RDP::COMMAND_PROCESSOR_FLAG_SUPER_SAMPLED_READ_BACK_BIT;
	if (vk_ssdither)
		flags |= RDP::COMMAND_PROCESSOR_FLAG_SUPER_SAMPLED_DITHER_BIT;

	processor = new RDP::CommandProcessor(wsi->get_device(), reinterpret_cast<void *>(aligned_rdram),
											 offset, rdram_size, rdram_size / 2, flags);
	if (!processor->device_is_supported())
	{
		delete processor;
		processor = nullptr;
		return false;
	}

	RDP::Quirks quirks;
	quirks.set_native_texture_lod(vk_native_texture_lod);
	quirks.set_native_resolution_tex_rect(vk_native_tex_rect);
	processor->set_quirks(quirks);

	screen_set_mode(window_fullscreen);
	wsi->begin_frame();

	last_frame_counter = 0;
	frame_counter = 0;
	return true;
}
