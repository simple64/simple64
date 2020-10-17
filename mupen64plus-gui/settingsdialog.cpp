#include "settingsdialog.h"
#include "mainwindow.h"
#include "interface/core_commands.h"

#include <QPushButton>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

void SettingsDialog::handleCoreButton()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Locate Core Library"), NULL, tr("Shared Libraries (*.dylib *.so* *.dll)"));
    if (!fileName.isNull()) {
        corePath->setText(fileName);
        w->getSettings()->setValue("coreLibPath", fileName);
    }
}

void SettingsDialog::handlePluginButton()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Locate Plugin Directory"),
                                                         NULL,
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if (!fileName.isNull()) {
        pluginPath->setText(fileName);
        w->getSettings()->setValue("pluginDirPath", fileName);

        w->getSettings()->remove("inputPlugin");
        w->getSettings()->remove("videoPlugin");
        w->getSettings()->remove("audioPlugin");
        w->getSettings()->remove("rspPlugin");
        w->updatePlugins();
        initStuff();
    }
}

void SettingsDialog::handleConfigButton()
{
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Set Config Directory"),
                                                         NULL,
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if (!fileName.isNull()) {
        configPath->setText(fileName);
        w->getSettings()->setValue("configDirPath", fileName);
    }
}

void SettingsDialog::handleClearConfigButton()
{
    configPath->setText("");
    w->getSettings()->remove("configDirPath");
}

void SettingsDialog::handleCoreEdit()
{
    w->getSettings()->setValue("coreLibPath", corePath->text());
}

void SettingsDialog::handlePluginEdit()
{
    w->getSettings()->setValue("pluginDirPath", pluginPath->text());

    w->getSettings()->remove("inputPlugin");
    w->getSettings()->remove("videoPlugin");
    w->getSettings()->remove("audioPlugin");
    w->getSettings()->remove("rspPlugin");
    w->updatePlugins();
    initStuff();
}

void SettingsDialog::handleConfigEdit()
{
    w->getSettings()->setValue("configDirPath", configPath->text());
}

void SettingsDialog::initStuff()
{
    layout = new QGridLayout(this);

    QLabel *coreLabel = new QLabel("Core Library Path", this);
    corePath = new QLineEdit(this);
    corePath->setText(w->getSettings()->value("coreLibPath").toString());
    QPushButton *coreButton = new QPushButton("Set Path", this);
    connect(coreButton, SIGNAL (released()),this, SLOT (handleCoreButton()));
    connect(corePath, SIGNAL (editingFinished()),this, SLOT (handleCoreEdit()));
    corePath->setStyleSheet("border: 1px solid; padding: 10px");
    layout->addWidget(coreLabel,0,0);
    layout->addWidget(corePath,0,1);
    layout->addWidget(coreButton,0,2);

    QLabel *pluginLabel = new QLabel("Plugin Dir Path", this);
    pluginPath = new QLineEdit(this);
    pluginPath->setText(w->getSettings()->value("pluginDirPath").toString());
    QPushButton *pluginButton = new QPushButton("Set Path", this);
    connect(pluginButton, SIGNAL (released()),this, SLOT (handlePluginButton()));
    connect(pluginPath, SIGNAL (editingFinished()),this, SLOT (handlePluginEdit()));
    pluginPath->setStyleSheet("border: 1px solid; padding: 10px");
    layout->addWidget(pluginLabel,1,0);
    layout->addWidget(pluginPath,1,1);
    layout->addWidget(pluginButton,1,2);

    QLabel *note = new QLabel("Note: If you change the Config Path, you need to close and re-open mupen64plus-gui before it will take effect.", this);
    QLabel *configLabel = new QLabel("Config Dir Path", this);
    configPath = new QLineEdit(this);
    QString configDirPath = w->getSettings()->value("configDirPath").toString();
    if (!configDirPath.isEmpty())
        configPath->setText(configDirPath);
    else
        configPath->setText(ConfigGetUserConfigPath());
    QPushButton *configButton = new QPushButton("Set Path", this);
    connect(configButton, SIGNAL (released()),this, SLOT (handleConfigButton()));
    connect(configPath, SIGNAL (editingFinished()),this, SLOT (handleConfigEdit()));
    QPushButton *clearConfigButton = new QPushButton("Clear", this);
    connect(clearConfigButton, SIGNAL (released()),this, SLOT (handleClearConfigButton()));
    configPath->setStyleSheet("border: 1px solid; padding: 10px");
    layout->addWidget(note,2,0,1,-1);
    layout->addWidget(configLabel,3,0);
    layout->addWidget(configPath,3,1);
    layout->addWidget(configButton,3,2);
    layout->addWidget(clearConfigButton,3,3);

    QDir PluginDir(w->getSettings()->value("pluginDirPath").toString());
    QStringList Filter;
    Filter.append("");
    QStringList current;

    QLabel *inputLabel = new QLabel("Input Plugin", this);
    layout->addWidget(inputLabel,5,0);
    QComboBox *inputChoice = new QComboBox(this);
    Filter.replace(0,"mupen64plus-input*");
    current = PluginDir.entryList(Filter);
    inputChoice->addItems(current);
    QString qtInputPlugin = w->getSettings()->value("inputPlugin").toString();
    int my_index = inputChoice->findText(qtInputPlugin);
    if (my_index == -1) {
        inputChoice->addItem(qtInputPlugin);
        my_index = inputChoice->findText(qtInputPlugin);
    }
    inputChoice->setCurrentIndex(my_index);
    connect(inputChoice, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated),
        [=](const QString &text) {
            w->getSettings()->setValue("inputPlugin", text);
    });
    layout->addWidget(inputChoice,5,1);

    setLayout(layout);
}

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    initStuff();
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    if (w->getCoreStarted() == 0)
    {
        w->loadCoreLib();
        w->loadPlugins();
    }
    event->accept();
}
