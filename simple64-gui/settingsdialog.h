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

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void handleConfigButton();
    void handleAppPathButton();
    void handleClearConfigButton();
    void handleConfigEdit();

private:
    void initStuff();
    QLineEdit *configPath;
    QGridLayout *layout = nullptr;
};

#endif // SETTINGSDIALOG_H
