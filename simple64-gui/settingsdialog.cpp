#include "settingsdialog.h"
#include "mainwindow.h"
#include "interface/core_commands.h"

#include <QPushButton>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>

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

void SettingsDialog::handleConfigEdit()
{
    w->getSettings()->setValue("configDirPath", configPath->text());
}

void SettingsDialog::initStuff()
{
    layout = new QGridLayout(this);

    QLabel *note = new QLabel("Note: If you change the Config Path, you need to close and re-open simple64-gui before it will take effect.", this);
    QLabel *configLabel = new QLabel("Config Dir Path", this);
    configPath = new QLineEdit(this);
    configPath->setText(w->getSettings()->value("configDirPath").toString());
    QPushButton *configButton = new QPushButton("Set Path", this);
    connect(configButton, SIGNAL (released()),this, SLOT (handleConfigButton()));
    connect(configPath, SIGNAL (editingFinished()),this, SLOT (handleConfigEdit()));
    QPushButton *clearConfigButton = new QPushButton("Clear", this);
    connect(clearConfigButton, SIGNAL (released()),this, SLOT (handleClearConfigButton()));
    configPath->setStyleSheet("border: 1px solid; padding: 10px");
    layout->addWidget(note,0,0,1,-1);
    layout->addWidget(configLabel,1,0);
    layout->addWidget(configPath,1,1);
    layout->addWidget(configButton,1,2);
    layout->addWidget(clearConfigButton,1,3);

#ifdef CONFIG_DIR_PATH
    configPath->setEnabled(false);
    configButton->setEnabled(false);
    clearConfigButton->setEnabled(false);
#endif
#ifdef PLUGIN_DIR_PATH
    QString pluginPath = PLUGIN_DIR_PATH;
#else
    QString pluginPath = QCoreApplication::applicationDirPath();
#endif
    QDir PluginDir(pluginPath);
    QStringList Filter;
    Filter.append("");
    QStringList current;

    QLabel *inputLabel = new QLabel("Input Plugin", this);
    layout->addWidget(inputLabel,5,0);
    QComboBox *inputChoice = new QComboBox(this);
    Filter.replace(0,"*-input-*");
    current = PluginDir.entryList(Filter);
    inputChoice->addItems(current);
    QString qtInputPlugin = w->getSettings()->value("inputPlugin").toString();
    int my_index = inputChoice->findText(qtInputPlugin);
    if (my_index == -1) {
        inputChoice->addItem(qtInputPlugin);
        my_index = inputChoice->findText(qtInputPlugin);
    }
    inputChoice->setCurrentIndex(my_index);
    connect(inputChoice, QOverload<int>::of(&QComboBox::activated),
        [=](int index){
            w->getSettings()->setValue("inputPlugin", inputChoice->itemText(index));
            w->updatePlugins();
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
    w->getSettings()->sync();
    int value;
    if (w->getCoreLib())
    {
        (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &value);
        if (value == M64EMU_STOPPED)
            w->resetCore();
    }
    else
        w->resetCore();

    event->accept();
}
