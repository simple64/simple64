#ifndef VKWINDOW_H
#define VKWINDOW_H

#include <QCloseEvent>
#include <QWindow>
#include "interface/common.h"

class VkWindow : public QWindow
{
    Q_OBJECT
public:
    explicit VkWindow(QWindow *parent = nullptr);
    ~VkWindow();
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *te) Q_DECL_OVERRIDE;
private:
    int m_width;
    int m_height;
    int orig_width;
    int orig_height;
    int timerId = 0;
};
#endif // VKWINDOW_H
