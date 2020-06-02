#include "Language.h"
#include "util.h"
#include <windows.h>
#include <algorithm>
#include <map>
#include "../Config.h"

typedef std::map<int32_t, std::string> LANG_STRINGS;
typedef LANG_STRINGS::value_type LANG_STR;

bool g_debugLang = false;
LANG_STRINGS g_currentStrings, g_defaultStrings;

void loadDefaultStrings(void)
{
	//Config Dialog
	g_defaultStrings.insert(LANG_STRINGS::value_type(TAB_VIDEO, "Video"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TAB_EMULATION, "Emulation"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TAB_FRAME_BUFFER, "Frame buffer"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TAB_TEXTURE_ENHANCEMENT, "Texture enhancement"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TAB_OSD, "OSD"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TAB_DEBUG, "Debug"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(CFG_SAVE_SETTINGS_FOR, "Save settings for:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(CFG_SETTINGS_PROFILE, "Settings profile:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(CFG_REMOVE, "Remove"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(CFG_RESTORE_DEFAULTS, "Restore Defaults"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(CFG_SAVE_AND_CLOSE, "Save and Close"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(CFG_SAVE, "Save"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(CFG_CLOSE, "Close"));
	
	//Video Tab
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_GROUP, "Video"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_FULL_SCREEN_RES, "Full screen resolution:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_FULL_SCREEN_RES_TOOLTIP, "All the resolutions that your video card/monitor supports should be displayed.\n\n[Recommended: Maximum resolution for your monitor unless performance becomes an issue]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_REFRESH_RATE, "Refresh rate:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_REFRESH_RATE_TOOLTIP, ""));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_WINDOWED_RESOLUTION, "Windowed resolution:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_WINDOWED_RESOLUTION_TOOLTIP, "This option selects the resolution for windowed mode. You also may select Custom and enter your own window size.\n\n[Recommended: 640 x 480, 800 x 600, 1024 x 768, 1280 x 960]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ASPECT_RATIO, "Aspect ratio:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ASPECT_RATIO_TOOLTIP, "This setting adjusts the aspect ratio of the video output. All N64 games support 4:3. Some games support 16:9 within game settings. Use Stretch to fill the screen without pillar or letterboxing.\nTry to adjust game to fit tries to adjust the viewing space to fit without stretching. Many games work well adjusted, but some don't."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ASPECT_4_3, "4:3 (recommended)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ASPECT_16_19, "16:9"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ASPECT_STRETCH, "Stretch"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ASPECT_ADJUST, "Try to adjust game to fit"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_VSYNC, "Enable VSync"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_VSYNC_TOOLTIP, "Vertical sync, or VSync, can improve the image by syncing the game's frame rate to your monitor's refresh rate.This prevents image tearing, but may cause performance problems.\n[Recommended:Usually off, on if you have image tearing problems]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_THREADED_VIDEO, "Enable threaded video"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_THREADED_VIDEO_TOOLTIP, "Threaded video can improve performance with poor OpenGL drivers at the cost of very marginal input lag, usually less than half a frame.\n[Recommended: Usually off, unless there are performance issues]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_OVERSCAN, "Overscan"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_OVERSCAN_TOOLTIP, "When enabled, the image is cropped by values specified in N64 pixels. Useful to remove black borders in some games."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_NTSC, "NTSC"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_PAL, "PAL"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ANTI_ALIASING, "Anti-aliasing"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_NO_ANTI_ALIASING, "No anti-aliasing"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_FAST_ANTI_ALIASING, "Fast approximate anti-aliasing (FXAA)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_MULTISAMPLE_ANTI_ALIASING, "Multisample anti-aliasing (MSAA):"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_AA_OFF, "Off"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_AA_HIGH, "High"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_AA_TOOLTIP, "GLideN64 offers two methods to smooth jagged polygons:\nFast approximate anti-aliasing (FXAA): FXAA is a post-processing filter that can provide a decent result, but as good as MSAA. The main reason to use FXAA is to use with N64-style depth compare. FXAA adds some blurriness to the output image, causing some textures like text to possibly look worse.\nMultisample anti-aliasing (MSAA): MSAA is a standard anti-aliasing technique used in computer graphics to improve image quality. Most modern GPUs support 2, 4, 8, and 16 samples. More samples mean better quality, but are slower. There are two downsides: it's incompatible with N64-style depth compare and may cause minor glitches in some games.\nRecommendation: [Usually 16x MSAA, or FXAA with N64-style depth compare]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_AA_INFO, "Multisample anti-aliasing is not compatible with N64-style depth compare."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_FILTERING_GROUP, "Filtering"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ANISOTROPIC, "Anisotropic filtering:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ANISOTROPIC_OFF, "Off"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_ANISOTROPIC_HIGH, "High"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_BILINEAR, "Bilinear filtering:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_BILINEAR_STANDARD, "Standard"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_BILINEAR_3POINT, "N64-style 3 point"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_BILINEAR_TOOLTIP, "Bilinear filtering: Textures will use standard PC-style bilinear filtering.\nN64-style 3 point: Textures will be filtered more like the N64. The result is less smooth but more accurate."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_GROUP, "Dithering"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_PATTERN, "Pattern (RDRAM):"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_APPLY_TO_OUTPUT, "Apply to final output"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_APPLY_TO_OUTPUT_TOOLTIP, "This setting enables game controlled ordered grid dithering. Enable it for accurate representation. Default = disabled."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_5BIT_QUANTIZATION, "Enable 5-bit quantization"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_5BIT_QUANTIZATION_TOOLTIP, "Like real hardware this setting reduces the number of colors if dithering is used. Removes undesired dithering fragments. Default = enabled."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_HIRES_NOISE, "High resolution noise"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_HIRES_NOISE_TOOLTIP, "This setting doubles noise resolution to get a finer noise effect at higher resolutions. Default = disabled."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_MODE_TOOLTIP, "RDRAM dithering prevents color banding in games with framebuffer effects.\nSettings: Disabled, Bayer ordered grid dithering, Magic Square ordered grid dithering or blue noise dithering. Blue noise dithering produces unobtrusive results. Default = blue noise dithering."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_DISABLE, "disable"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_BAYER, "Bayer"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_MAGIC_SQUARE, "Magic square"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_DITHERING_BLUE_NOISE, "Blue noise"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(VIDEO_LANGUAGE, "Language:"));

	//Emulation Tab
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_USE_PER_GAME, "Use per-game settings"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_USE_PER_GAME_TOOLTIP, "When enabled, all non-default values of settings are stored individually for each game.\nWhen a game is running, settings are displayed and saved for the currently running game.\nNote: GLideN64 already contains settings for the optimal performance of most games. Be careful when altering options on 'Emulation' and 'Frame buffer' tab.\n[Recommended: Checked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_N64_STYLE_MIP_MAPPING, "Enable N64-style mip-mapping"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_N64_STYLE_MIP_MAPPING_TOOLTIP, "The N64 uses a unique method of mip-mapping that's difficult to reproduce correctly on PCs. When checked, this option emulates N64-accurate mip-mapping. When unchecked, some games have sharper distant textures.\n[Recommended: Checked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_HWLIGHTING, "Enable per-pixel lighting (better quality, HLE only)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_HWLIGHTING_TOOLTIP, "In N64 games lighting is calculated per vertex. This option enables Phong shading, which provides smoother and more realistic lighting.\n\n[Recommended: Your preference]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_SHADERS_STORAGE, "Store compiled shaders for performance (recommended)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_SHADERS_STORAGE_TOOLTIP, "Use persistent storage for compiled shader programs.\nEach game uses a set of combiners. A combiner is an equation that defines how to build output color from various color inputs. GLideN64 translates shaders, and compiles shader programs on the fly. Shaders are large and complex. If the game uses several new combiners, compiling new shaders will take time and result in stuttering. When this option is checked, these shaders are saved so they're not recompiled the next time you run the game.\n[Recommended: Checked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_INTERNAL_RES, "Internal resolution"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_INTERNAL_RES_TOOLTIP, "This option sets the output render buffer size. By default, the render buffer is set to the Same as output resolution, but you can set it to the Original N64 resolution or a Multiple of N64 resolution."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_FACTOR0X, "Same as output resolution"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_FACTOR1X, "Original N64 resolution (most accurate)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_FACTORXX, "Multiple of N64 resolution:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_GAMMA, "Gamma"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_GAMMA_TOOLTIP, "Gamma correction.\nSome N64 games use gamma correction. Gamma correction makes image brighter. N64 applies gamma correction in its Video Interface. \nGLideN64 emulates gamma correction as post-processing effect. That is, it works only when frame buffer emulation enabled. Gamma correction enabled automatically for games, which use it on real N64. You may force gamma correction for all games. Default level of gamma correction is 2, as on N64.\n[Recommended: use defaults]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_GAMMA_CORRECTION, "Custom gamma correction level:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_GAMMA_INFO, "Selecting this option overrides gamma correction specified by the game."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_2D_ELEMENTS, "2D elements"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_RENDER_2D_ELEMENTS, "Render 2D elements in N64 resolution (best quality, can be slow):"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_RENDER_2D_TOOLTIP, "When checked, 2D elements are rendered at the N64s resolution before copying them to output. This usually eliminates display issues with 2D elements, but it can be slow. This option uses heuristics to detect adjacent 2D elements that doesn't work for every game.\n[Recommended: Checked, unless you have performance problems]"));	
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_RENDER_DISABLE, "Disable"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_RENDER_ENABLE_OPTIMIZED, "Enable optimized"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_RENDER_ENABLE_UNOPTIMIZED, "Enable unoptimized"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_HALOS_REMOVAL, "Try to remove colored halos around transparent objects"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_FIX_BLACK_LINES, "Fix black lines between 2D elements:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_FIX_BLACK_LINES_TOOLTIP, "In some games the coordinates for parts of 2D elements are not aligned: there is a half-pixel split between adjacent elements. When rendering at the N64's original resolution it isn't visible, but when the image is scaled up it results in black lines. This option attempts to connect these 2D elements.\nFor adjacent 2D elements: Apply the correction only for polygons that need it. Select this option for games that have issues with black lines.\nAlways: Apply the correction for every 2D element. Select this option when For adjacent 2D elements doesn't help.\nNever: Don't attempt to correct black lines between 2D elements.\n[Recommended: Game dependent, mostly Never]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_ADJACENT_2D_ELEMENTS, "For adjacent 2D elements"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_ALWAYS, "Always"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_NEVER, "Never"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_BACKGROUND, "Background rendering mode (HLE only):"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_BACKGROUND_TOOLTIP, "Background is a complex macro command used to render large (normally full screen) images. Since background images usually don't fit texture memory, the microcode splits them on narrow strips and renders them one by one. HLE code has two modes to emulate background commands:\nOne piece: The whole background image rendred as one textured rectangle. This method is normally much faster, but the result is not always correct.\nStripped: This method emulates background commands as close as possible to actual microcode implementation. It's slower but more precise. Another problem: some games may have gaps between rendered strips in high resolution. Use \"Render 2D elements in N64 resolution\" option to remove the gaps.\n[Recommended: Game dependent, mostly Stripped]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_ONE_PIECE, "One piece"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(EMULATION_STRIPPED, "Stripped"));

	//Frame Buffer Tab
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_ENABLE, "Emulate frame buffer"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_ENABLE_INFO, "Unchecking this option disables many effects including cropping, aspect ratio, N64 resolution factor, N64-style depth compare and more. Don't uncheck this option unless you have performance issues."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_AUX_BUFFERS, "Copy auxiliary color buffers to N64 memory (sometimes slow, fewer game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_AUX_BUFFERS_TOOLTIP, "When checked, the auxiliary color buffer is copied to N64 memory right after rendering to it is finished. This option helps to correctly emulate frame buffer effects in some games. This option may noticeably reduce performance.\n[Recommended: Usually unchecked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_SWAP, "Frame buffer swap on:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_SWAP_TOOLTIP, "This option controls how often GLideN64 updates the main frame buffer.\nVertical interrupt : Update the buffer as quickly as possible, every vertical interrupt per second(50 for PAL and 60 for NTSC games).This option is the most compatible.Use this mode unless you have performance problems.\nVI origin change : The VI origin changes corresponding to the game's actual FPS. N64 games typically run between 20 to 30 FPS so the buffers swap less often than the first mode. This mode does not work for games that use single buffering, which is rare.\nColor buffer change: This option checks to see if the frame buffer has been changed. There are some games where this doesn't work."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_VERTICAL_INTERRUPT, "Vertical interrupt (recommended, fewest game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_VI_ORIGIN_CHANGE, "VI origin change (faster, few game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COLOR_BUFFER_CHANGE, "Color buffer change (fastest, some game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_INFO_ENABLE, "Use emulator help to read/write frame buffers"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_INFO_ENABLE_TOOLTIP, "Some emulators do a poor job of detecting when to read/write frame buffers. You can disable emulator help to let GLideN64 read/write frame buffers itself.\n[Recommended: Usually unchecked, but for some games/emulators it may be faster checked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_READ_COLOR_CHUNK, "Read color buffer in 4 KB chunks"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_READ_COLOR_CHUNK_TOOLTIP, "When checked, the color buffer will be read in chunks. Otherwise, the buffer will be read in its entirety. If a game needs to read the entire buffer, selecting this option may be slow.\n[Recommended: Usually unchecked, because the color buffer is usually read entirely]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_READ_DEPTH_CHUNK, "Read depth buffer in 4 KB chunks"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_READ_DEPTH_CHUNK_TOOLTIP, "When checked, the depth buffer will be read in chunks. Otherwise the buffer will be read in its entirety. If a game needs to read the entire buffer, selecting this option may be slow.\n[Recommended: Checked, because the depth buffer is not often read entirely]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_COLOR_BUFFER, "Copy video card frame buffer to N64 memory:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_COLOR_BUFFER_TOOLTIP, "In some games GLideN64 can't detect when the game uses the frame buffer. With these options, you can have GLideN64 copy each frame of your video card's frame buffer to N64 memory.\nNever: Disable copying buffers from video card.\nSynchronous: Effects are detected for all games, but it can be slow. Use for games where Asynchronous doesn't work.\nAsynchronous: Effects are detected for most games. [Recommended: Usually Asynchronous]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_NEVER, "Never (fastest, many game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_SYNCHRONOUS, "Synchronous (slowest, fewest game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_ASYNCHRONOUS, "Asynchronous (fast, few game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_DEPTH_BUFFER, "Copy video card depth buffer to N64 memory:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_DEPTH_BUFFER_TOOLTIP, "The depth buffer is used to emulate some effects (e.g. coronas):\nNever: Depth buffer is disabled.\nFrom VRAM: Your video card's depth buffer is copied to N64 memory each frame, which can be slow on some games.\nIn software: Generally faster than copying from VRAM, but the result can be imperfect.\n[Recommended: In software]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_DEPTH_NEVER, "Never (fastest, most game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_DEPTH_VRAM, "From VRAM (slow, some game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_DEPTH_SOFTWARE, "In software (fast, fewest game issues)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_N64_DEPTH_COMPARE, "N64-style depth compare (experimental, disables MSAA)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_N64_DEPTH_COMPARE_TOOLTIP, "The N64 uses a unique method of calculating depth to the camera. When enabled, GlideN64 uses shaders to try to emulate these calculations correctly. Not compatible with anti-aliasing. Experimental!\nFast mode requires OpenGL 4.2 and fragment shader interlock extensions.\nCompatible mode requires only core OpenGL 4.2 Can be slow!\n[Recommended: Sometimes checked, for several games]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_N64_DEPTH_DISABLE, "Disable"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_N64_DEPTH_FAST, "Fast"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_N64_DEPTH_COMPATIBLE, "Compatible"));   
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_FORCE_DEPTH_BUFFER_CLEAR, "Force depth buffer clear"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_FORCE_DEPTH_BUFFER_CLEAR_TOOLTIP, "Enable force depth buffer clear. A hack. Necessary for Eikou no Saint Andrews.\n[Recommended: except for Eikou no Saint Andrews]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_RENDER_FRAMEBUFFER, "Render N64 frame buffer to output"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_RENDER_FRAMEBUFFER_TOOLTIP, "When this option is checked, the frame buffer is rendered directly to the screen. This prevents some graphic problems but may cause slowdowns or visual quality problems.\n[Recommended: Usually unchecked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_DEPTH_TO_MAIN, "Copy depth to main depth buffer"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(FRAMEBUFFER_COPY_DEPTH_TO_MAIN_TOOLTIP, "This option is required for some Reshade depth dependant effects, such as SSAO and depth of field.\n[Recommended: only if you use Reshade and need depth dependant effects]"));

	//Texture enhancement
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_TEXTURES_GROUP, "N64 textures"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER, "Filter:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_TOOLTIP, "This filter smooths or sharpens textures. There are four smoothing filters and two sharpening filters. The higher the number, the stronger the effect. Performance may be affected depending on the game and/or your PC.\n[Recommended: Your preference]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_NONE, "None"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_SMOOTH1, "Smooth filtering 1"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_SMOOTH2, "Smooth filtering 2"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_SMOOTH3, "Smooth filtering 3"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_SMOOTH4, "Smooth filtering 4"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_SHARP1, "Sharp filtering 1"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_N64_FILTER_SHARP2, "Sharp filtering 2"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_ENHANCEMENT, "Enhancement:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_ENHANCEMENT_TOOLTIP, "There are 12 distinct filters to select. Depending on which filter, they may cause performance problems.\nWhen Store is selected, textures are saved to the cache as-is. This improves performance in games that load many textures. Uncheck Disable for backgrounds for the best performance.\n[Recommended: Your preference]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_ENHANCEMENT_NONE, "None"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_ENHANCEMENT_STORE, "Store"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_DECREASE_COLOR, "Decrease color banding (recommended for xBRZ enhancement)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_DECREASE_COLOR_TOOLTIP, "This option enables a pre-processing step that reduces posterization issues on enhanced textures.\n[Recommended: Checked for xBRZ]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_IGNORE_BACKGROUNDS, "Disable for backgrounds"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_IGNORE_BACKGROUNDS_TOOLTIP, "This option skips texture enhancements for long, narrow textures that are usually used for backgrounds. This may save texture memory and improve performance.\n[Recommended: Checked, unless Enhancement is set to Store]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_USE_FILE_STORAGE, "Use file storage instead of memory cache"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_USE_TEXTURE_PACK, "Use texture pack"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_TEXTURE_PACK, "Texture pack path:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_TEXTURE_PACK_TOOLTIP, "Select path to the folder with texture packs.\nDefault: Plugin / hires_texture"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_CACHE_PATH, "Texture cache path:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_DUMP_PATH, "Texture dump path:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_USE_FULL_TRANSPARENCIES, "Use full transparencies (recommended for newer texture packs)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_USE_FULL_TRANSPARENCIES_TOOLTIP, "When this option is cleared, textures will be loaded as they are when using Rice Video: transparencies either on or off. When this option is selected, GlideN64 will check how the texture's alpha channel was designed and will select the most appropriate format. This gives texture pack designers freedom to use semi-transparent textures.\nClear this option for older or poorly designed texture packs.\n[Recommended: Texture pack dependent]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_ALTERNATIVE_CRC, "Alternative CRC calculation (for old Rice Video packs)"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_ALTERNATIVE_CRC_TOOLTIP, "This option emulates a palette CRC calculation bug in Rice Video. If you have problems loading textures, try checking or unchecking this option.\n[Recommended: Mostly unchecked, checked for old texture packs]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_FILE_STORAGE, "Use file storage instead of memory cache"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_DUMP_EDIT, "Dump/edit textures"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_DUMP_EDIT_TOOLTIP, "This option dumps textures on screen to a texture pack folder. You can also reload textures while the game is running to see how they look instantly—big time saver!\nHotkeys:\nUse R to reload textures from the texture pack\nUse D to toggle texture dumping on or off"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_SIZE_OF_MEMORY_CACHE, "Size of memory cache for enhanced textures:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_SIZE_OF_MEMORY_CACHE_TOOLTIP, "Enhanced and filtered textures can be cached to improve performance. This option adjusts how much memory is dedicated to the texture cache. This can improve performance if there are many requests for the same texture, which is usually the case. Normally 128 MB should be more than enough, but the best option is different for each game. Super Mario 64 may not need more than 32 MB, but Conker's Bad Fur Day can take advantage of 256 MB+. Adjust accordingly if you are having performance problems. Setting this option to 0 disables the cache.\n[Recommended: PC and game dependent]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_SAVE_ENHANCED, "Save enhanced texture cache to hard disk"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_SAVE_ENHANCED_TOOLTIP, "This option saves all previously loaded and enhanced textures to your PC. When the game is next launched, textures don't have to be recreated, causing smoother performance.\nWhen using texture packs, loading packs will take only a few seconds when the game is launched as opposed to the 5-60 seconds that loading usually takes. However, if you change the texture pack you'll have to manually delete the texture cache. Saved cache files are saved to a folder called Cache within the plugins folder.\n[Recommended: Checked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_COMPRESS_CACHE, "Compress texture cache"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_COMPRESS_CACHE_TOOLTIP, "Textures will be compressed so more textures can be held in the cache. The compression ratio varies per texture, but the compression is typically 1/5 of the original size.\n[Recommended: Checked]"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_CONVERT_16BPP, "Convert textures to 16 bpp"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(TEXTURE_CONVERT_16BPP_TOOLTIP, "This option halves the space used by textures in the texture cache and video card memory to improve performance. When reducing the color, GLideN64 tries to perserve the original quality as much as possible. On most textures it's hardly noticeable, but some textures, like skies, can look noticeably worse.\n[Recommended: Unchecked]"));

	//On-screen display
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_POSITION, "Position"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_DISPLAY_FPS, "Display FPS"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_DISPLAY_VI, "Display VI/s"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_DISPLAY_PERCENTAGE, "Display percentage"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_DISPLAY_INTERNAL_RESOLUTION, "Display internal resolution"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_DISPLAY_RENDERING_RESOLUTION, "Display rendering resolution"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_FONT, "Font"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_SIZE, "Size:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_PX, "px"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_COLOR, "Color:"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(OSD_PREVIEW, "Preview"));

	//Debug tab
	g_defaultStrings.insert(LANG_STRINGS::value_type(DEBUG_DUMP_LOWLEVEL_INFO, "Dump low-level info"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(DEBUG_DUMP_DISPLAY_LIST, "Dump display list commands"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(DEBUG_DUMP_DETAILED_INFO, "Dump detailed info (matrices, vertices, etc.)"));

	//About
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_ABOUT_TITLE, "About GLideN64"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_TAB_ABOUT, "About"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_TAB_CONTRIBUTORS, "Contributors"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_TAB_FUNDERS, "Funders"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_TAB_CREDITS, "Credits"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_NEXT_GENERATION, "The next generation open source graphics plugin for N64 emulators."));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_AUTHORS, "Authors"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_DEVELOPER, "Developer"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_BETA_TESTER, "Beta tester"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_GUI_DESIGNER, "GUI designer"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_AND_MORE, "... and more"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_AUTHOR_GLN64, "Author of the original glN64 graphics plugin"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_AUTHOR_GLES2N64, "Author of gles2n64, a port of glN64 to GL ES 2.0"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_AUTHOR_GLIDEHQ, "Author of the GlideHQ texture library"));
	g_defaultStrings.insert(LANG_STRINGS::value_type(ABOUT_AUTHOR_Z64, "Author of z64, an OpenGL LLE plugin"));
}

LANG_STR GetNextLangString(FILE * file)
{
	if (feof(file))
		return LANG_STR(0, "");

	char token = 0;
	while (token != '#' && !feof(file))
		fread(&token, 1, 1, file);

	if (feof(file))
		return LANG_STR(0, "");

	int32_t StringID;
	fscanf_s(file, "%d", &StringID);

	token = 0;
	while (token != '#' && !feof(file))
		fread(&token, 1, 1, file);

	if (feof(file))
		return LANG_STR(0, "");

	while (token != '"' && !feof(file))
		fread(&token, 1, 1, file);

	if (feof(file))
		return LANG_STR(0, "");

	fread(&token, 1, 1, file);
	std::string text;
	while (token != '"' && !feof(file)) {
		text += token;
		fread(&token, 1, 1, file);
	}

	std::string::size_type pos = text.find("\\n");
	while (pos != std::string::npos) {
		text.replace(pos, 2, "\n");
		pos = text.find("\\n", pos + 1);
	}
	return LANG_STR(StringID, text);
}

bool LoadLanguageFile(const std::string & langFile)
{
	if (g_defaultStrings.size() == 0)
		loadDefaultStrings();

	g_currentStrings.clear();
	if (g_debugLang)
		return true;

	FILE *file = NULL;
	if (fopen_s(&file, langFile.c_str(), "rb") != 0 || file == NULL)
		return false;

	uint8_t utf_bom[3];
	if (fread(&utf_bom, sizeof(utf_bom), 1, file) != 1 || utf_bom[0] != 0xEF || utf_bom[1] != 0xBB || utf_bom[2] != 0xBF) {
		fclose(file);
		return false;
	}

	while (!feof(file))
		g_currentStrings.insert(GetNextLangString(file));

	fclose(file);
	return true;
}

void LoadCurrentStrings(const char * path, const std::string & lang)
{
	std::string translationsFolder(path);
	std::replace(translationsFolder.begin(), translationsFolder.end(), '/', '\\');
	if (translationsFolder[translationsFolder.length() - 1] != '\\')
	{
		translationsFolder += '\\';
	}
	translationsFolder += "translations\\";
	if (lang.length() > 0)
	{
		std::string langFile = translationsFolder + lang;
		if (LoadLanguageFile(langFile))
		{
			return;
		}
	} // default: attempt to use gliden64_en.Lang
	std::string langFile = translationsFolder + "gliden64_en.Lang";
	LoadLanguageFile(langFile);
}


std::string getLangString(const char * FileName, languageStringID ID)
{
	FILE *file = NULL;
	if (fopen_s(&file, FileName, "rb") != 0 || file == NULL)
		return "";

	uint8_t utf_bom[3];
	if (fread(&utf_bom, sizeof(utf_bom), 1, file) != 1 ||
		utf_bom[0] != 0xEF ||
		utf_bom[1] != 0xBB ||
		utf_bom[2] != 0xBF)
	{
		fclose(file);
		return "";
	}

	while (!feof(file)) {
		LANG_STR String = GetNextLangString(file);
		if (String.first == ID) {
			fclose(file);
			return String.second;
		}
	}
	fclose(file);
	return "";
}

LanguageList GetLanguageList(const char * path)
{
	std::string pluginFolder(path);
	std::replace(pluginFolder.begin(), pluginFolder.end(), '/', '\\');
	if (pluginFolder[pluginFolder.length() - 1] != '\\')
	{
		pluginFolder += '\\';
	}
	pluginFolder += "translations\\";
	std::string nameFilters = pluginFolder + "gliden64_*.Lang";

	WIN32_FIND_DATAA FindData;
	HANDLE hFindFile = FindFirstFileA(nameFilters.c_str(), &FindData); // Find anything
	LanguageList languages;
	if (hFindFile != INVALID_HANDLE_VALUE) {
		do {
			std::string langFile = pluginFolder + FindData.cFileName;
			LanguageFile file; 

			file.Filename = FindData.cFileName;
			file.LanguageName = getLangString(langFile.c_str(), LANGUAGE_NAME);

			if (file.LanguageName.length() == 0)
				continue;

			languages.push_back(file);
		} while (FindNextFileA(hFindFile, &FindData));
		FindClose(hFindFile);
	}
	return languages;
}

const std::string & getString(languageStringID stringID)
{
	LANG_STRINGS::iterator currentString = g_currentStrings.find(stringID);
	if (currentString != g_currentStrings.end())
		return currentString->second;

	if (g_debugLang) {
		char debugStr[300];
		sprintf_s(debugStr, "#%d#", stringID);
		std::pair<LANG_STRINGS::iterator, bool> ret = g_currentStrings.insert(LANG_STRINGS::value_type(stringID, debugStr));
		if (ret.second)
			return ret.first->second;
	}

	LANG_STRINGS::iterator DefString = g_defaultStrings.find(stringID);
	if (DefString != g_defaultStrings.end())
	{
		return DefString->second;
	}
	static std::string emptyString;
	return emptyString;
}

std::wstring wGS(languageStringID StringID)
{
	return ToUTF16(getString(StringID).c_str());
}