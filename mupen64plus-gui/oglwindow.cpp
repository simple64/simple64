
#include "oglwindow.h"
#include "mainwindow.h"
#include "interface/core_commands.h"

OGLWindow::OGLWindow(QSurfaceFormat _format, QWindow *parent)
    : QWindow(parent)
{
    ogl_init = 0;
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(_format);
    m_context.setFormat(_format);
    m_context.create();
}

void OGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);
    if (ogl_init == 0)
    {
        m_context.doneCurrent();
        m_context.moveToThread(w->getRenderingThread());
        ogl_init = 1;
    }
}

QOpenGLContext* OGLWindow::context()
{
    return &m_context;
}

void OGLWindow::resizeEvent(QResizeEvent *event) {
    QWindow::resizeEvent(event);
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }
    timerId = startTimer(500);
    m_width = event->size().width() * devicePixelRatio();
    m_height = event->size().height() * devicePixelRatio();
}

void OGLWindow::timerEvent(QTimerEvent *te) {
    int size = (m_width << 16) + m_height;
    int current_size = 0;

    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_SIZE, &current_size);
    if (current_size != size)
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_SIZE, &size);

    killTimer(te->timerId());
    timerId = 0;
    requestActivate();
}
