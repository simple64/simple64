
#include "vkwindow.h"
#include "mainwindow.h"
#include "interface/core_commands.h"

VkWindow::VkWindow(QWindow *parent)
    : QWindow(parent)
{
    setSurfaceType(QWindow::VulkanSurface);
}

void VkWindow::resizeEvent(QResizeEvent *event) {
    QWindow::resizeEvent(event);
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }
    timerId = startTimer(500);
    m_width = event->size().width() * devicePixelRatio();
    m_height = event->size().height() * devicePixelRatio();
}

void VkWindow::timerEvent(QTimerEvent *te) {
    int size = (m_width << 16) + m_height;
    int current_size = 0;

    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_SIZE, &current_size);
    if (current_size != size)
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_SIZE, &size);

    killTimer(te->timerId());
    timerId = 0;
    requestActivate();
}
