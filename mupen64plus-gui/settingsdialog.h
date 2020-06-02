#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);
private slots:
    void handleCoreButton();
    void handlePluginButton();
    void handleConfigButton();
    void handleClearConfigButton();
    void handleCoreEdit();
    void handlePluginEdit();
    void handleConfigEdit();
private:
    void initStuff();
    QLineEdit *corePath;
    QLineEdit *pluginPath;
    QLineEdit *configPath;
    QGridLayout *layout = nullptr;
};

#endif // SETTINGSDIALOG_H
