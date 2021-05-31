#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QActionGroup>
#include <QDesktopServices>
#include "settingsdialog.h"
#include "plugindialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "vidext.h"
#include "netplay/createroom.h"
#include "netplay/joinroom.h"

#include "osal/osal_preproc.h"
#include "interface/core_commands.h"
#include "interface/version.h"
#include "m64p_common.h"
#include "version.h"

#define RECENT_SIZE 10

#define SETTINGS_VER 2

m64p_video_extension_functions vidExtFunctions = {14,
                                                 qtVidExtFuncInit,
                                                 qtVidExtFuncQuit,
                                                 qtVidExtFuncListModes,
                                                 qtVidExtFuncListRates,
                                                 qtVidExtFuncSetMode,
                                                 qtVidExtFuncSetModeWithRate,
                                                 qtVidExtFuncGLGetProc,
                                                 qtVidExtFuncGLSetAttr,
                                                 qtVidExtFuncGLGetAttr,
                                                 qtVidExtFuncGLSwapBuf,
                                                 qtVidExtFuncSetCaption,
                                                 qtVidExtFuncToggleFS,
                                                 qtVidExtFuncResizeWindow,
                                                 qtVidExtFuncGLGetDefaultFramebuffer};

void MainWindow::updatePlugins()
{
    QString pluginPath = settings->value("pluginDirPath").toString();
    pluginPath.replace("$APP_PATH$", QCoreApplication::applicationDirPath());
    QDir PluginDir(pluginPath);
    PluginDir.setFilter(QDir::Files);
    QStringList Filter;
    Filter.append("");
    QStringList current;
    QString default_value;

    if (!settings->contains("videoPlugin")) {
        Filter.replace(0,"mupen64plus-video*");
        current = PluginDir.entryList(Filter);
        default_value = "mupen64plus-video-GLideN64";
        default_value += OSAL_DLL_EXTENSION;
        if (current.isEmpty())
            settings->setValue("videoPlugin", "dummy");
        else if (current.indexOf(default_value) != -1)
            settings->setValue("videoPlugin", default_value);
        else
            settings->setValue("videoPlugin", current.at(0));
    }
    if (!settings->contains("audioPlugin")) {
        Filter.replace(0,"mupen64plus-audio*");
        current = PluginDir.entryList(Filter);
        default_value = "mupen64plus-audio-sdl2";
        default_value += OSAL_DLL_EXTENSION;
        if (current.isEmpty())
            settings->setValue("audioPlugin", "dummy");
        else if (current.indexOf(default_value) != -1)
            settings->setValue("audioPlugin", default_value);
        else
            settings->setValue("audioPlugin", current.at(0));
    }
    if (!settings->contains("rspPlugin")) {
        Filter.replace(0,"mupen64plus-rsp*");
        current = PluginDir.entryList(Filter);
        default_value = "mupen64plus-rsp-hle";
        default_value += OSAL_DLL_EXTENSION;
        if (current.isEmpty())
            settings->setValue("rspPlugin", "dummy");
        else if (current.indexOf(default_value) != -1)
            settings->setValue("rspPlugin", default_value);
        else
            settings->setValue("rspPlugin", current.at(0));
    }
    if (!settings->contains("inputPlugin")) {
        Filter.replace(0,"mupen64plus-input*");
        current = PluginDir.entryList(Filter);
        default_value = "mupen64plus-input-qt";
        default_value += OSAL_DLL_EXTENSION;
        if (current.isEmpty())
            settings->setValue("inputPlugin", "dummy");
        else if (current.indexOf(default_value) != -1)
            settings->setValue("inputPlugin", default_value);
        else
            settings->setValue("inputPlugin", current.at(0));
    }

    ui->actionController_Configuration->setEnabled(settings->value("inputPlugin").toString().contains("-qt"));
}

void MainWindow::updatePIF(Ui::MainWindow *ui)
{
    QMenu *PIF = new QMenu(this);
    PIF->setTitle("PIF ROM");
    ui->menuFile->insertMenu(ui->actionTake_Screenshot, PIF);
    ui->menuFile->insertSeparator(ui->actionTake_Screenshot);

    QAction *fileSelect = new QAction(this);
    QString current = settings->value("PIF_ROM").toString();
    fileSelect->setText("PIF ROM: " + current);
    PIF->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered, [=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("PIF ROM"), NULL, tr("PIF ROM File (*.bin)"));
        if (!filename.isNull()) {
            settings->setValue("PIF_ROM", filename);
            QString current = filename;
            fileSelect->setText("PIF ROM: " + current);
        }
    });

    QAction *clearSelect = new QAction(this);
    clearSelect->setText("Clear PIF ROM Selection");
    PIF->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("PIF_ROM");
        fileSelect->setText("PIF ROM: ");
    });
}

void MainWindow::updateDD(Ui::MainWindow *ui)
{
    QMenu *DD = new QMenu(this);
    DD->setTitle("64DD");
    ui->menuFile->insertMenu(ui->actionTake_Screenshot, DD);

    QAction *fileSelect = new QAction(this);
    QString current = settings->value("DD_ROM").toString();
    fileSelect->setText("64DD IPL ROM: " + current);
    DD->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered, [=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("64DD IPL ROM"), NULL, tr("64DD ROM File (*.n64)"));
        if (!filename.isNull()) {
            settings->setValue("DD_ROM", filename);
            QString current = filename;
            fileSelect->setText("64DD IPL ROM: " + current);
        }
    });

    QAction *fileSelect2 = new QAction(this);
    current = settings->value("DD_DISK").toString();
    fileSelect2->setText("64DD Disk: " + current);
    DD->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("64DD Disk File"), NULL, tr("64DD Disk File (*.ndd)"));
        if (!filename.isNull()) {
            settings->setValue("DD_DISK", filename);
            QString current = filename;
            fileSelect2->setText("64DD Disk: " + current);
        }
    });

    QAction *clearSelect = new QAction(this);
    clearSelect->setText("Clear 64DD Selections");
    DD->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("DD_ROM");
        settings->remove("DD_DISK");
        fileSelect->setText("64DD IPL ROM: ");
        fileSelect2->setText("64DD Disk: ");
    });

    //DD->addSeparator();

    //QAction *startGame = new QAction(this);
    //startGame->setText("Start 64DD");
    //DD->addAction(startGame);
    //connect(startGame, &QAction::triggered,[=](){
    //    openROM("");
    //});
}

void MainWindow::updateGB(Ui::MainWindow *ui)
{
    QMenu *GB = new QMenu(this);
    GB->setTitle("Game Boy Cartridges");
    ui->menuFile->insertMenu(ui->actionTake_Screenshot, GB);

    QAction *fileSelect = new QAction(this);
    QString current = settings->value("Player1GBROM").toString();
    fileSelect->setText("Player 1 ROM: " + current);
    GB->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb)"));
        if (!filename.isNull()) {
            settings->setValue("Player1GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 1 ROM: " + current);
        }
    });

    QAction *fileSelect2 = new QAction(this);
    current = settings->value("Player1GBRAM").toString();
    fileSelect2->setText("Player 1 RAM: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB RAM Files (*.sav)"));
        if (!filename.isNull()) {
            settings->setValue("Player1GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 1 RAM: " + current);
        }
    });

    QAction *clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 1 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player1GBROM");
        settings->remove("Player1GBRAM");
        fileSelect->setText("Player 1 ROM: ");
        fileSelect2->setText("Player 1 RAM: ");
    });
    GB->addSeparator();

    fileSelect = new QAction(this);
    current = settings->value("Player2GBROM").toString();
    fileSelect->setText("Player 2 ROM: " + current);
    GB->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb)"));
        if (!filename.isNull()) {
            settings->setValue("Player2GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 2 ROM: " + current);
        }
    });

    fileSelect2 = new QAction(this);
    current = settings->value("Player2GBRAM").toString();
    fileSelect2->setText("Player 2 RAM: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB RAM Files (*.sav)"));
        if (!filename.isNull()) {
            settings->setValue("Player2GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 2 RAM: " + current);
        }
    });

    clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 2 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player2GBROM");
        settings->remove("Player2GBRAM");
        fileSelect->setText("Player 2 ROM: ");
        fileSelect2->setText("Player 2 RAM: ");
    });
    GB->addSeparator();

    fileSelect = new QAction(this);
    current = settings->value("Player3GBROM").toString();
    fileSelect->setText("Player 3 ROM: " + current);
    GB->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb)"));
        if (!filename.isNull()) {
            settings->setValue("Player3GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 3 ROM: " + current);
        }
    });

    fileSelect2 = new QAction(this);
    current = settings->value("Player3GBRAM").toString();
    fileSelect2->setText("Player 3 RAM: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB RAM Files (*.sav)"));
        if (!filename.isNull()) {
            settings->setValue("Player3GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 3 RAM: " + current);
        }
    });

    clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 3 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player3GBROM");
        settings->remove("Player3GBRAM");
        fileSelect->setText("Player 3 ROM: ");
        fileSelect2->setText("Player 3 RAM: ");
    });
    GB->addSeparator();

    fileSelect = new QAction(this);
    current = settings->value("Player4GBROM").toString();
    fileSelect->setText("Player 4 ROM: " + current);
    GB->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb)"));
        if (!filename.isNull()) {
            settings->setValue("Player4GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 4 ROM: " + current);
        }
    });

    fileSelect2 = new QAction(this);
    current = settings->value("Player4GBRAM").toString();
    fileSelect2->setText("Player 4 RAM: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB RAM Files (*.sav)"));
        if (!filename.isNull()) {
            settings->setValue("Player4GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 4 RAM: " + current);
        }
    });

    clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 4 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player4GBROM");
        settings->remove("Player4GBRAM");
        fileSelect->setText("Player 4 ROM: ");
        fileSelect2->setText("Player 4 RAM: ");
    });
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    verbose = 0;
    nogui = 0;
    gles = 0;
    ui->setupUi(this);

    m_title = "mupen64plus-gui    Build Date: ";
    m_title += __DATE__;
    this->setWindowTitle(m_title);

    QString ini_path = QDir(QCoreApplication::applicationDirPath()).filePath("mupen64plus-gui.ini");
    settings = new QSettings(ini_path, QSettings::IniFormat, this);

    if (!settings->isWritable())
        settings = new QSettings("mupen64plus", "gui", this);

    if (!settings->contains("version") || settings->value("version").toInt() != SETTINGS_VER)
    {
        settings->clear();
        settings->setValue("version", SETTINGS_VER);
    }

    restoreGeometry(settings->value("geometry").toByteArray());
    restoreState(settings->value("windowState").toByteArray());

    QActionGroup *my_slots_group = new QActionGroup(this);
    QAction *my_slots[10];
    OpenRecent = new QMenu(this);
    QMenu * SaveSlot = new QMenu(this);
    OpenRecent->setTitle("Open Recent");
    SaveSlot->setTitle("Change Save Slot");
    ui->menuFile->insertMenu(ui->actionSave_State, OpenRecent);
    ui->menuFile->insertSeparator(ui->actionSave_State);
    ui->menuFile->insertMenu(ui->actionSave_State_To, SaveSlot);
    ui->menuFile->insertSeparator(ui->actionSave_State_To);
    for (int i = 0; i < 10; ++i) {
        my_slots[i] = new QAction(this);
        my_slots[i]->setCheckable(true);
        my_slots[i]->setText("Slot " + QString::number(i));
        my_slots[i]->setActionGroup(my_slots_group);
        SaveSlot->addAction(my_slots[i]);
        QAction *temp_slot = my_slots[i];
        connect(temp_slot, &QAction::triggered,[=](bool checked){
            if (checked) {
                int slot = temp_slot->text().remove("Slot ").toInt();
                (*CoreDoCommand)(M64CMD_STATE_SET_SLOT, slot, NULL);
            }
        });
    }

    updateOpenRecent();
    updateGB(ui);
    updateDD(ui);
    updatePIF(ui);

    if (!settings->contains("coreLibPath"))
        settings->setValue("coreLibPath", "$APP_PATH$");
    if (!settings->contains("pluginDirPath"))
        settings->setValue("pluginDirPath", "$APP_PATH$");
    if (!settings->contains("configDirPath"))
        settings->setValue("configDirPath", "$CONFIG_PATH$");

    updatePlugins();

    if (!settings->contains("volume"))
        settings->setValue("volume", 100);
    VolumeAction * volumeAction = new VolumeAction(tr("Volume"));
    connect(volumeAction->slider(), SIGNAL(valueChanged(int)), this, SLOT(volumeValueChanged(int)));
    volumeAction->slider()->setValue(settings->value("volume").toInt());
    ui->menuEmulation->insertAction(ui->actionMute, volumeAction);

    coreLib = nullptr;
    gfxPlugin = nullptr;
    rspPlugin = nullptr;
    audioPlugin = nullptr;
    inputPlugin = nullptr;
    loadCoreLib();
    loadPlugins();

    setupLLE();

    if (coreLib)
    {
        m64p_handle coreConfigHandle;
        m64p_error res = (*ConfigOpenSection)("Core", &coreConfigHandle);
        if (res == M64ERR_SUCCESS)
        {
            int current_slot = (*ConfigGetParamInt)(coreConfigHandle, "CurrentStateSlot");
            my_slots[current_slot]->setChecked(true);
        }
    }

    setupDiscord();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateApp()
{
#ifdef _AUTOUPDATE
#ifndef __APPLE__
    QNetworkAccessManager *updateManager = new QNetworkAccessManager(this);
    connect(updateManager, &QNetworkAccessManager::finished,
        this, &MainWindow::updateReplyFinished);

    updateManager->get(QNetworkRequest(QUrl("https://api.github.com/repos/loganmc10/m64p/releases/latest")));
#endif
#endif
}

void MainWindow::setupLLE()
{
    if (!settings->contains("LLE"))
    {
        settings->setValue("LLE", 0);
    }

    ui->actionVideo_Settings->setEnabled(!settings->value("LLE").toInt());
    ui->actionLLE_Graphics->setChecked(settings->value("LLE").toInt());

    if (coreLib)
    {
        m64p_handle lionConfigHandle;
        m64p_error res = (*ConfigOpenSection)("Video-Angrylion-Plus", &lionConfigHandle);
        if (res == M64ERR_SUCCESS)
        {
            int vimode = (*ConfigGetParamInt)(lionConfigHandle, "ViMode");
            ui->actionVI_Filter->setChecked(!vimode);
        }
    }
    ui->actionVI_Filter->setEnabled(settings->value("LLE").toInt());

    connect(ui->actionLLE_Graphics, &QAction::toggled,
    [=]( bool checked ) {
        settings->setValue("LLE", checked ? 1 : 0);
        ui->actionVI_Filter->setEnabled(checked);
        ui->actionVideo_Settings->setEnabled(!checked);
        if (coreLib)
        {
            int response;
            (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
            if (response == M64EMU_STOPPED)
                resetCore();
        }
    });

    connect(ui->actionVI_Filter, &QAction::toggled,
    [=]( bool checked ) {
        m64p_handle lionConfigHandle;
        m64p_error res = (*ConfigOpenSection)("Video-Angrylion-Plus", &lionConfigHandle);
        if (res == M64ERR_SUCCESS)
        {
            int vimode = checked == true ? 0 : 1;
            (*ConfigSetParameter)(lionConfigHandle, "ViMode", M64TYPE_INT, &vimode);
        }
    });

}

void MainWindow::setupDiscord()
{
    QLibrary *discordLib = new QLibrary((QDir(QCoreApplication::applicationDirPath()).filePath("discord_game_sdk")), this);

    memset(&discord_app, 0, sizeof(discord_app));

    DiscordCreateParams params;
    DiscordCreateParamsSetDefault(&params);
    params.client_id = 770838334015930398LL;
    params.flags = DiscordCreateFlags_NoRequireDiscord;
    params.event_data = &discord_app;

    typedef EDiscordResult (*CreatePrototype)(DiscordVersion version, struct DiscordCreateParams* params, struct IDiscordCore** result);
    CreatePrototype createFunction = (CreatePrototype) discordLib->resolve("DiscordCreate");
    if (createFunction)
        createFunction(DISCORD_VERSION, &params, &discord_app.core);

    if (discord_app.core)
    {
        discord_app.activities = discord_app.core->get_activity_manager(discord_app.core);
        discord_app.lobbies = discord_app.core->get_lobby_manager(discord_app.core);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::discordCallback);
        timer->start(1000);
    }
}

struct Discord_Application* MainWindow::getDiscordApp()
{
    return &discord_app;
}

void MainWindow::updateDiscordActivity(struct DiscordActivity activity)
{
    if (discord_app.activities)
        discord_app.activities->update_activity(discord_app.activities, &activity, &discord_app, nullptr);
}

void MainWindow::clearDiscordActivity()
{
    if (discord_app.activities)
        discord_app.activities->clear_activity(discord_app.activities, &discord_app, nullptr);
}


void MainWindow::discordCallback()
{
    if (discord_app.core)
        discord_app.core->run_callbacks(discord_app.core);
}

void MainWindow::updateDownloadFinished(QNetworkReply *reply)
{
    if (!reply->error())
    {
        QTemporaryDir dir;
        dir.setAutoRemove(false);
        if (dir.isValid())
        {
            QString fullpath = dir.filePath(reply->url().fileName());
            QFile file(fullpath);
            file.open(QIODevice::WriteOnly);
            file.write(reply->readAll());
            file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
            file.close();
            QProcess process;
            QString command = fullpath + " \"";
            command += QCoreApplication::applicationDirPath();
            command += "\"";
            process.startDetached(command);
            reply->deleteLater();
            QCoreApplication::quit();
        }
    }
    reply->deleteLater();
}

void MainWindow::updateReplyFinished(QNetworkReply *reply)
{
    if (!reply->error())
    {
        QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject json = json_doc.object();
        if (json.value("target_commitish").toString() != QString(GUI_VERSION))
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Update Available", "A newer version is available, update?", QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                QNetworkAccessManager *updateManager = new QNetworkAccessManager(this);
                connect(updateManager, &QNetworkAccessManager::finished,
                    this, &MainWindow::updateDownloadFinished);
#ifdef _WIN32
                QNetworkRequest req(QUrl("https://github.com/m64p/m64p-updater/releases/latest/download/m64p-updater.exe"));
#else
                QNetworkRequest req(QUrl("https://github.com/m64p/m64p-updater/releases/latest/download/m64p-updater"));
#endif
                req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
                updateManager->get(req);
                QMessageBox *message = new QMessageBox(this);
                message->setStandardButtons(0);
                message->setText("Downloading updater");
                message->show();
            }
        }
    }
    reply->deleteLater();
}

void MainWindow::volumeValueChanged(int value)
{
    if (value != settings->value("volume").toInt())
    {
        settings->setValue("volume", value);
        settings->sync();
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_AUDIO_VOLUME, &value);
    }
}

void MainWindow::setVerbose()
{
    verbose = 1;
}

void MainWindow::setNoGUI()
{
    nogui = 1;
    if (!menuBar()->isNativeMenuBar())
        menuBar()->hide();
    statusBar()->hide();
}

void MainWindow::setGLES()
{
    gles = 1;
}

int MainWindow::getNoGUI()
{
    return nogui;
}

int MainWindow::getVerbose()
{
    return verbose;
}

int MainWindow::getGLES()
{
    return gles;
}

void MainWindow::resizeMainWindow(int Width, int Height)
{
    QSize size = this->size();
    Height += (menuBar()->isNativeMenuBar() ? 0 : ui->menuBar->height()) + ui->statusBar->height();
    if (size.width() != Width || size.height() != Height)
        resize(Width, Height);
}

void MainWindow::toggleFS(int force)
{
    if (coreLib == nullptr) return;

    int response = M64VIDEO_NONE;
    if (force == M64VIDEO_NONE)
        (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_MODE, &response);
    if (response == M64VIDEO_WINDOWED || force == M64VIDEO_FULLSCREEN) {
        if (!menuBar()->isNativeMenuBar())
            menuBar()->hide();
        statusBar()->hide();
        showFullScreen();
    } else if (response == M64VIDEO_FULLSCREEN || force == M64VIDEO_WINDOWED) {
        if (!nogui) {
            if (!menuBar()->isNativeMenuBar())
                menuBar()->show();
            statusBar()->show();
        }
        showNormal();
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
#ifdef SINGLE_THREAD
    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
    if (response != M64EMU_STOPPED)
    {
        QMessageBox msgBox;
        msgBox.setText("Stop current game first");
        msgBox.exec();
        event->ignore();
        return;
    }
#endif

    stopGame();

    closePlugins();
    closeCoreLib();

    if (discord_app.activities)
        discord_app.activities->clear_activity(discord_app.activities, &discord_app, nullptr);
    if (discord_app.core)
    {
        discord_app.core->run_callbacks(discord_app.core);
        discord_app.core->destroy(discord_app.core);
    }

    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());

    event->accept();
}

void MainWindow::updateOpenRecent()
{
    OpenRecent->clear();
    QAction *recent[RECENT_SIZE];
    QStringList list = settings->value("RecentROMs").toString().split(";");
    if (list.first() == "") {
        OpenRecent->setEnabled(false);
        return;
    }

    OpenRecent->setEnabled(true);
    for (int i = 0; i < list.size() && i < RECENT_SIZE; ++i) {
        recent[i] = new QAction(this);
        recent[i]->setText(list.at(i));
        OpenRecent->addAction(recent[i]);
        QAction *temp_recent = recent[i];
        connect(temp_recent, &QAction::triggered,[=](){
#ifndef SINGLE_THREAD
                    openROM(temp_recent->text(), "", 0, 0);
#else
                    singleThreadLaunch(temp_recent->text(), "", 0, 0);
#endif
                });
    }
    OpenRecent->addSeparator();

    QAction *clearRecent = new QAction(this);
    clearRecent->setText("Clear List");
    OpenRecent->addAction(clearRecent);
    connect(clearRecent, &QAction::triggered,[=](){
        settings->remove("RecentROMs");
        updateOpenRecent();
    });
}

void MainWindow::showMessage(QString message)
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText(message);
    msgBox->show();
}

void MainWindow::createOGLWindow(QSurfaceFormat* format)
{
    my_window = new OGLWindow();
    QWidget *container = QWidget::createWindowContainer(my_window, this);
    container->setFocusPolicy(Qt::StrongFocus);

    my_window->setCursor(Qt::BlankCursor);
    my_window->setFormat(*format);

    setCentralWidget(container);

    my_window->installEventFilter(&keyPressFilter);
    this->installEventFilter(&keyPressFilter);
}

void MainWindow::deleteOGLWindow()
{
    QWidget *container = new QWidget(this);
    my_window->doneCurrent();
    setCentralWidget(container);
}

void MainWindow::stopGame()
{
    if (!coreLib) return;

    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
    if (response != M64EMU_STOPPED)
    {
        (*CoreDoCommand)(M64CMD_STOP, 0, NULL);
#ifndef SINGLE_THREAD
        while (workerThread->isRunning())
            QCoreApplication::processEvents();
#endif
    }
}

void MainWindow::resetCore()
{
    closePlugins();
    closeCoreLib();
    loadCoreLib();
    loadPlugins();
}

#ifdef SINGLE_THREAD
void MainWindow::singleThreadLaunch(QString filename, QString netplay_ip, int netplay_port, int netplay_player)
{
    QTimer::singleShot(1000, [=]() { openROM(filename, netplay_ip, netplay_port, netplay_player); } );
}
#endif

void MainWindow::openROM(QString filename, QString netplay_ip, int netplay_port, int netplay_player)
{
#ifdef SINGLE_THREAD
    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
    if (response != M64EMU_STOPPED)
    {
        QMessageBox msgBox;
        msgBox.setText("Stop current game first");
        msgBox.exec();
        return;
    }
#endif

    stopGame();

    logViewer.clearLog();

    resetCore();

    workerThread = new WorkerThread(netplay_ip, netplay_port, netplay_player, this);
    workerThread->setFileName(filename);

    QStringList list;
    if (settings->contains("RecentROMs"))
        list = settings->value("RecentROMs").toString().split(";");
    list.removeAll(filename);
    list.prepend(filename);
    if (list.size() > RECENT_SIZE)
        list.removeLast();
    settings->setValue("RecentROMs",list.join(";"));
    updateOpenRecent();

    workerThread->start();
}

void MainWindow::on_actionOpen_ROM_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open ROM"), settings->value("ROMdir").toString(), tr("ROM Files (*.n64 *.N64 *.z64 *.Z64 *.v64 *.V64 *.zip *.ZIP *.7z)"));
    if (!filename.isNull()) {
        QFileInfo info(filename);
        settings->setValue("ROMdir", info.absoluteDir().absolutePath());
#ifndef SINGLE_THREAD
        openROM(filename, "", 0, 0);
#else
        singleThreadLaunch(filename, "", 0, 0);
#endif
    }
}

void MainWindow::on_actionPlugin_Paths_triggered()
{
    SettingsDialog *settings = new SettingsDialog(this);
    settings->show();
}

void MainWindow::on_actionStop_Game_triggered()
{
    stopGame();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionPlugin_Settings_triggered()
{
    PluginDialog *settings = new PluginDialog(this);
    settings->show();
}

void MainWindow::on_actionPause_Game_triggered()
{
#ifndef SINGLE_THREAD
    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
    if (response == M64EMU_RUNNING)
        (*CoreDoCommand)(M64CMD_PAUSE, 0, NULL);
    else if (response == M64EMU_PAUSED)
        (*CoreDoCommand)(M64CMD_RESUME, 0, NULL);
#else
    QMessageBox msgBox;
    msgBox.setText("Paused");
    msgBox.exec();
#endif
}

void MainWindow::on_actionMute_triggered()
{
    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_AUDIO_MUTE, &response);
    if (response == 0) {
        response = 1;
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_AUDIO_MUTE, &response);
    } else if (response == 1) {
        response = 0;
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_AUDIO_MUTE, &response);
    }
}

void MainWindow::on_actionHard_Reset_triggered()
{
    (*CoreDoCommand)(M64CMD_RESET, 1, NULL);
}

void MainWindow::on_actionSoft_Reset_triggered()
{
    (*CoreDoCommand)(M64CMD_RESET, 0, NULL);
}

void MainWindow::on_actionTake_Screenshot_triggered()
{
    (*CoreDoCommand)(M64CMD_TAKE_NEXT_SCREENSHOT, 0, NULL);
}

void MainWindow::on_actionSave_State_triggered()
{
    (*CoreDoCommand)(M64CMD_STATE_SAVE, 1, NULL);
}

void MainWindow::on_actionLoad_State_triggered()
{
    (*CoreDoCommand)(M64CMD_STATE_LOAD, 1, NULL);
}

void MainWindow::on_actionToggle_Fullscreen_triggered()
{
    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_VIDEO_MODE, &response);
    if (response == M64VIDEO_WINDOWED) {
        response = M64VIDEO_FULLSCREEN;
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_MODE, &response);
    } else if (response == M64VIDEO_FULLSCREEN) {
        response = M64VIDEO_WINDOWED;
        (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_MODE, &response);
    }
}

void MainWindow::on_actionSave_State_To_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save State File"), NULL, NULL);
    if (!filename.isNull()) {
        if (!filename.contains(".st"))
            filename.append(".state");
        (*CoreDoCommand)(M64CMD_STATE_SAVE, 1, filename.toUtf8().data());
    }
}

void MainWindow::on_actionLoad_State_From_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Save State"), NULL, tr("State Files (*.st* *.pj*)"));
    if (!filename.isNull()) {
        (*CoreDoCommand)(M64CMD_STATE_LOAD, 1, filename.toUtf8().data());
    }
}

void MainWindow::on_actionController_Configuration_triggered()
{
    if (!coreLib) return;

    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
    if (response == M64EMU_STOPPED)
        resetCore();

    typedef void (*Config_Func)();
    Config_Func PluginConfig = (Config_Func) osal_dynlib_getproc(inputPlugin, "PluginConfig");
    if (PluginConfig)
        PluginConfig();
}

void MainWindow::on_actionToggle_Speed_Limiter_triggered()
{
    int value;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_SPEED_LIMITER, &value);
    value = !value;
    (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_SPEED_LIMITER, &value);
}

void MainWindow::on_actionView_Log_triggered()
{
    logViewer.show();
}

void MainWindow::on_actionVideo_Settings_triggered()
{
    typedef void (*Config_Func)();
    Config_Func PluginConfig = (Config_Func) osal_dynlib_getproc(gfxPlugin, "PluginConfig");
    if (PluginConfig)
        PluginConfig();
}

void MainWindow::on_actionCreate_Room_triggered()
{
    CreateRoom *createRoom = new CreateRoom(this);
    createRoom->show();
}

void MainWindow::on_actionJoin_Room_triggered()
{
    JoinRoom *joinRoom = new JoinRoom(this);
    joinRoom->show();
}

void MainWindow::on_actionSupport_on_Patreon_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.patreon.com/m64p"));
}

void MainWindow::on_actionOpen_Discord_Channel_triggered()
{
    QDesktopServices::openUrl(QUrl("https://discord.gg/B5svWhR"));
}

WorkerThread* MainWindow::getWorkerThread()
{
    return workerThread;
}

OGLWindow* MainWindow::getOGLWindow()
{
    return my_window;
}

QSettings* MainWindow::getSettings()
{
    return settings;
}

LogViewer* MainWindow::getLogViewer()
{
    return &logViewer;
}

m64p_dynlib_handle MainWindow::getAudioPlugin()
{
    return audioPlugin;
}

m64p_dynlib_handle MainWindow::getRspPlugin()
{
    return rspPlugin;
}

m64p_dynlib_handle MainWindow::getInputPlugin()
{
    return inputPlugin;
}

m64p_dynlib_handle MainWindow::getGfxPlugin()
{
    return gfxPlugin;
}

void MainWindow::closeCoreLib()
{
    if (coreLib != nullptr)
    {
        (*ConfigSaveFile)();
        (*CoreShutdown)();
        osal_dynlib_close(coreLib);
        coreLib = nullptr;
    }
}

void MainWindow::loadCoreLib()
{
    QString corePath = settings->value("coreLibPath").toString();
    corePath.replace("$APP_PATH$", QCoreApplication::applicationDirPath());

    m64p_error res = osal_dynlib_open(&coreLib, QDir(corePath).filePath(OSAL_DEFAULT_DYNLIB_FILENAME).toUtf8().constData());

    if (res != M64ERR_SUCCESS)
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to load core library");
        msgBox.exec();
        return;
    }

    CoreStartup =                 (ptr_CoreStartup) osal_dynlib_getproc(coreLib, "CoreStartup");
    CoreShutdown =                (ptr_CoreShutdown) osal_dynlib_getproc(coreLib, "CoreShutdown");
    CoreDoCommand =               (ptr_CoreDoCommand) osal_dynlib_getproc(coreLib, "CoreDoCommand");
    CoreAttachPlugin =            (ptr_CoreAttachPlugin) osal_dynlib_getproc(coreLib, "CoreAttachPlugin");
    CoreDetachPlugin =            (ptr_CoreDetachPlugin) osal_dynlib_getproc(coreLib, "CoreDetachPlugin");
    CoreOverrideVidExt =          (ptr_CoreOverrideVidExt) osal_dynlib_getproc(coreLib, "CoreOverrideVidExt");

    ConfigGetUserConfigPath =     (ptr_ConfigGetUserConfigPath) osal_dynlib_getproc(coreLib, "ConfigGetUserConfigPath");
    ConfigSaveFile =              (ptr_ConfigSaveFile) osal_dynlib_getproc(coreLib, "ConfigSaveFile");
    ConfigGetParameterHelp =      (ptr_ConfigGetParameterHelp) osal_dynlib_getproc(coreLib, "ConfigSaveFile");
    ConfigGetParamInt =           (ptr_ConfigGetParamInt) osal_dynlib_getproc(coreLib, "ConfigGetParamInt");
    ConfigGetParamFloat =         (ptr_ConfigGetParamFloat) osal_dynlib_getproc(coreLib, "ConfigGetParamFloat");
    ConfigGetParamBool =          (ptr_ConfigGetParamBool) osal_dynlib_getproc(coreLib, "ConfigGetParamBool");
    ConfigGetParamString =        (ptr_ConfigGetParamString) osal_dynlib_getproc(coreLib, "ConfigGetParamString");
    ConfigSetParameter =          (ptr_ConfigSetParameter) osal_dynlib_getproc(coreLib, "ConfigSetParameter");
    ConfigDeleteSection =         (ptr_ConfigDeleteSection) osal_dynlib_getproc(coreLib, "ConfigDeleteSection");
    ConfigOpenSection =           (ptr_ConfigOpenSection) osal_dynlib_getproc(coreLib, "ConfigOpenSection");
    ConfigListParameters =        (ptr_ConfigListParameters) osal_dynlib_getproc(coreLib, "ConfigListParameters");
    ConfigGetSharedDataFilepath = (ptr_ConfigGetSharedDataFilepath) osal_dynlib_getproc(coreLib, "ConfigGetSharedDataFilepath");

    QString qtConfigDir = settings->value("configDirPath").toString();
    qtConfigDir.replace("$APP_PATH$", QCoreApplication::applicationDirPath());
    qtConfigDir.replace("$CONFIG_PATH$", ConfigGetUserConfigPath());

    if (!qtConfigDir.isEmpty())
        (*CoreStartup)(CORE_API_VERSION, qtConfigDir.toUtf8().constData() /*Config dir*/, QCoreApplication::applicationDirPath().toUtf8().constData(), (char*)"Core", DebugCallback, NULL, NULL);
    else
        (*CoreStartup)(CORE_API_VERSION, NULL /*Config dir*/, QCoreApplication::applicationDirPath().toUtf8().constData(), (char*)"Core", DebugCallback, NULL, NULL);

    CoreOverrideVidExt(&vidExtFunctions);
}

void MainWindow::closePlugins()
{
    ptr_PluginShutdown PluginShutdown;

    if (gfxPlugin != nullptr)
    {
        PluginShutdown = (ptr_PluginShutdown) osal_dynlib_getproc(gfxPlugin, "PluginShutdown");
        (*PluginShutdown)();
        osal_dynlib_close(gfxPlugin);
        gfxPlugin = nullptr;
    }
    if (audioPlugin != nullptr)
    {
        PluginShutdown = (ptr_PluginShutdown) osal_dynlib_getproc(audioPlugin, "PluginShutdown");
        (*PluginShutdown)();
        osal_dynlib_close(audioPlugin);
        audioPlugin = nullptr;
    }
    if (inputPlugin != nullptr)
    {
        PluginShutdown = (ptr_PluginShutdown) osal_dynlib_getproc(inputPlugin, "PluginShutdown");
        (*PluginShutdown)();
        osal_dynlib_close(inputPlugin);
        inputPlugin = nullptr;
    }
    if (rspPlugin != nullptr)
    {
        PluginShutdown = (ptr_PluginShutdown) osal_dynlib_getproc(rspPlugin, "PluginShutdown");
        (*PluginShutdown)();
        osal_dynlib_close(rspPlugin);
        rspPlugin = nullptr;
    }
}

void MainWindow::loadPlugins()
{
    if (coreLib == nullptr)
        return;

    ptr_PluginStartup PluginStartup;
    m64p_error res;
    QMessageBox msgBox;

    QString pluginPath = settings->value("pluginDirPath").toString();
    pluginPath.replace("$APP_PATH$", QCoreApplication::applicationDirPath());

    QString lle_path;
    QString plugin_path;
    if (settings->value("LLE").toInt())
    {
        lle_path = "mupen64plus-video-angrylion-plus";
        lle_path += OSAL_DLL_EXTENSION;
        plugin_path = QDir(pluginPath).filePath(lle_path);
    }
    else
        plugin_path = QDir(pluginPath).filePath(settings->value("videoPlugin").toString());
    res = osal_dynlib_open(&gfxPlugin, plugin_path.toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load video plugin");
        msgBox.exec();
        return;
    }
    PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(gfxPlugin, "PluginStartup");
    (*PluginStartup)(coreLib, (char*)"Video", DebugCallback);
    res = osal_dynlib_open(&audioPlugin, QDir(pluginPath).filePath(settings->value("audioPlugin").toString()).toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load audio plugin");
        msgBox.exec();
        return;
    }
    PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(audioPlugin, "PluginStartup");
    (*PluginStartup)(coreLib, (char*)"Audio", DebugCallback);
    res = osal_dynlib_open(&inputPlugin, QDir(pluginPath).filePath(settings->value("inputPlugin").toString()).toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load input plugin");
        msgBox.exec();
        return;
    }
    PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(inputPlugin, "PluginStartup");
    if (settings->value("inputPlugin").toString().contains("-qt"))
        (*PluginStartup)(coreLib, this, nullptr);
    else
        (*PluginStartup)(coreLib, (char*)"Input", DebugCallback);

    if (settings->value("LLE").toInt())
    {
        lle_path = "mupen64plus-rsp-parallel";
        lle_path += OSAL_DLL_EXTENSION;
        plugin_path = QDir(pluginPath).filePath(lle_path);
    }
    else
        plugin_path = QDir(pluginPath).filePath(settings->value("rspPlugin").toString());
    res = osal_dynlib_open(&rspPlugin, plugin_path.toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load rsp plugin");
        msgBox.exec();
        return;
    }
    PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(rspPlugin, "PluginStartup");
    (*PluginStartup)(coreLib, (char*)"RSP", DebugCallback);
}

m64p_dynlib_handle MainWindow::getCoreLib()
{
    return coreLib;
}

QThread* MainWindow::getRenderingThread()
{
    return rendering_thread;
}

void MainWindow::setRenderingThread(QThread* thread)
{
    rendering_thread = thread;
}
