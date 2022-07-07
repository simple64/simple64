#include "vidext.h"
#include "common.h"
#include "workerthread.h"
#include "mainwindow.h"
#include "interface/core_commands.h"
#include <stdio.h>
#include <QScreen>

static int init;

static QThread* rendering_thread;
static QVulkanInstance vulkan_inst;
static QVulkanInfoVector<QVulkanExtension> extensions;
static QVector<const char*> extension_list;

m64p_error qtVidExtFuncInit(void)
{
    init = 0;
    rendering_thread = QThread::currentThread();
    return M64ERR_SUCCESS;
}

m64p_error qtVidExtFuncQuit(void)
{
    init = 0;
    w->getWorkerThread()->toggleFS(M64VIDEO_WINDOWED);
    w->getWorkerThread()->deleteVkWindow();
    if (vulkan_inst.isValid())
        vulkan_inst.destroy();
    return M64ERR_SUCCESS;
}

m64p_error qtVidExtFuncListModes(m64p_2d_size *SizeArray, int *NumSizes)
{
    QList<QScreen *> screens = QGuiApplication::screens();
    QRect size = screens.first()->geometry();
    SizeArray[0].uiWidth = size.width();
    SizeArray[0].uiHeight = size.height();
    *NumSizes = 1;
    return M64ERR_SUCCESS;
}

m64p_error qtVidExtFuncListRates(m64p_2d_size, int*, int*)
{
    return M64ERR_UNSUPPORTED;
}

m64p_error qtVidExtFuncSetMode(int Width, int Height, int, int WindowMode, int)
{
    if (WindowMode == M64VIDEO_FULLSCREEN)
        w->getWorkerThread()->toggleFS(M64VIDEO_FULLSCREEN);
    else
        w->getWorkerThread()->resizeMainWindow(Width, Height);
    return M64ERR_SUCCESS;
}

m64p_error qtVidExtFuncSetModeWithRate(int, int, int, int, int, int)
{
    return M64ERR_UNSUPPORTED;
}

m64p_function qtVidExtFuncGLGetProc(const char*)
{
    return NULL;
}

m64p_error qtVidExtFuncGLSetAttr(m64p_GLattr, int)
{
    return M64ERR_UNSUPPORTED;
}

m64p_error qtVidExtFuncGLGetAttr(m64p_GLattr, int*)
{
    if (!init) return M64ERR_NOT_INIT;
    return M64ERR_UNSUPPORTED;
}

m64p_error qtVidExtFuncGLSwapBuf(void)
{
    return M64ERR_UNSUPPORTED;
}

m64p_error qtVidExtFuncSetCaption(const char *)
{
    return M64ERR_SUCCESS;
}

m64p_error qtVidExtFuncToggleFS(void)
{
    if (QThread::currentThread() == rendering_thread)
        w->getWorkerThread()->toggleFS(M64VIDEO_NONE);
    else
        w->toggleFS(M64VIDEO_NONE);

    return M64ERR_SUCCESS;
}

m64p_error qtVidExtFuncResizeWindow(int width, int height)
{
    int response = M64VIDEO_NONE;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_MODE, &response);
    if (response == M64VIDEO_WINDOWED)
    {
        int size = (width << 16) + height;
        int current_size = 0;
        (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_SIZE, &current_size);
        if (current_size != size)
            w->getWorkerThread()->resizeMainWindow(width, height);
    }
    return M64ERR_SUCCESS;
}

uint32_t qtVidExtFuncGLGetDefaultFramebuffer(void)
{
    return 0;
}

void* qtVidExtFuncGetVkSurface(void* instance)
{
    vulkan_inst.setVkInstance((VkInstance)instance);
    vulkan_inst.create();
    w->getWorkerThread()->createVkWindow(&vulkan_inst);
    VkSurfaceKHR surface = nullptr;
    while (surface == nullptr)
        surface = QVulkanInstance::surfaceForWindow(w->getVkWindow());
    init = 1;
    return surface;
}

m64p_error qtVidExtFuncGetVkInstExtensions(const char** ext[], uint32_t* ext_num)
{
    extensions = vulkan_inst.supportedExtensions();
    *ext_num = extensions.size();
    extension_list.clear();
    for (int i=0; i<extensions.size(); ++i)
    {
        extension_list.append(extensions[i].name.data());
    }
    *ext = extension_list.data();
    return M64ERR_SUCCESS;
}
