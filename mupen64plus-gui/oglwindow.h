#ifndef OGLWINDOW_H
#define OGLWINDOW_H

#include <QCloseEvent>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include "common.h"

class OGLWindow : public QOpenGLWindow
{
protected:
    void exposeEvent(QExposeEvent *) Q_DECL_OVERRIDE {}

    void initializeGL() Q_DECL_OVERRIDE;

    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *te) Q_DECL_OVERRIDE;
private:
    int m_width;
    int m_height;
    int timerId = 0;
};
#endif // OGLWINDOW_H
