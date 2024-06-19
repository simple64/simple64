
#include "vkwindow.h"
#include "mainwindow.h"
#include "interface/core_commands.h"

VkWindow::VkWindow(QWindow *parent)
    : QWindow(parent)
{
    setSurfaceType(QWindow::VulkanSurface);
}

VkWindow::~VkWindow()
{
    if (w->getCoreLib())
    {
        m64p_handle configVideoParallel = NULL;
        (*ConfigOpenSection)("Video-Parallel", &configVideoParallel);
        (*ConfigSetParameter)(configVideoParallel, "ScreenWidth", M64TYPE_INT, &orig_width);
        (*ConfigSetParameter)(configVideoParallel, "ScreenHeight", M64TYPE_INT, &orig_height);
        (*ConfigSaveSection)("Video-Parallel");
    }
}

void VkWindow::resizeEvent(QResizeEvent *event)
{
    QWindow::resizeEvent(event);
    if (timerId)
    {
        killTimer(timerId);
        timerId = 0;
    }
    timerId = startTimer(500);
    orig_width = event->size().width();
    orig_height = event->size().height();
    m_width = orig_width * devicePixelRatio();
    m_height = orig_height * devicePixelRatio();
}

void VkWindow::timerEvent(QTimerEvent *te)
{
    int size = (m_width << 16) + m_height;
    int current_size = 0;

    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_SIZE, &current_size);
    if (current_size != size)
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_SIZE, &size);

    killTimer(te->timerId());
    timerId = 0;
    requestActivate();
}
