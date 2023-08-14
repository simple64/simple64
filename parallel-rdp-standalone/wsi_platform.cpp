#include "wsi_platform.h"
#include "vkguts.h"
#include "gfx_simple64.h"

VkSurfaceKHR QT_WSIPlatform::create_surface(VkInstance instance, VkPhysicalDevice gpu)
{
    surface = nullptr;
    screen_get_surface(&surface, instance);
    return surface;
}

void QT_WSIPlatform::destroy_surface(VkInstance instance, VkSurfaceKHR surface)
{
    screen_close();
}

std::vector<const char *> QT_WSIPlatform::get_instance_extensions()
{
    const char** ext;
    uint32_t ext_num = 0;
    screen_get_instance_extensions(&ext, &ext_num);
    std::vector<const char*> return_value;
    for (int i=0; i< ext_num; ++i)
        return_value.push_back(ext[i]);
    return return_value;
}

uint32_t QT_WSIPlatform::get_surface_width()
{
    return window_width;
}

uint32_t QT_WSIPlatform::get_surface_height()
{
    return window_height;
}

bool QT_WSIPlatform::alive(Vulkan::WSI &wsi)
{
    return true;
}

void QT_WSIPlatform::poll_input()
{
    
}

void QT_WSIPlatform::do_resize()
{
    resize = true;
}
