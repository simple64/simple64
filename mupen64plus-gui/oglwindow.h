#ifndef OGLWINDOW_H
#define OGLWINDOW_H

#include <QCloseEvent>
#include <QWindow>
#include <QOpenGLContext>
#include "common.h"

class OGLWindow : public QWindow
{
    Q_OBJECT
public:
    explicit OGLWindow(QSurfaceFormat _format, QWindow *parent = nullptr);
    QOpenGLContext* context();
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *te) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent *event) override;
private:
    int ogl_init;
    int m_width;
    int m_height;
    int timerId = 0;
    QOpenGLContext m_context;
};
#endif // OGLWINDOW_H
