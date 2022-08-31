#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>

class PluginDialog : public QDialog
{
    Q_OBJECT
public:
    PluginDialog(QWidget *parent = nullptr);
private slots:
    void handleResetButton();
private:
    QString AudioName;
};

#endif // PLUGINDIALOG_H
