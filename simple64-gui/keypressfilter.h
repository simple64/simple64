#ifndef KEYPRESSFILTER_H
#define KEYPRESSFILTER_H

#include <QObject>

class KeyPressFilter : public QObject
{
    Q_OBJECT
public:
    explicit KeyPressFilter(QObject *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif
