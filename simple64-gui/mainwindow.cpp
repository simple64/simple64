#include <QString>
#include <QFileDialog>
#include <QCloseEvent>
#include <QActionGroup>
#include <QDesktopServices>
#include "settingsdialog.h"
#include "plugindialog.h"
#include "hotkeydialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "interface/common.h"
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

m64p_video_extension_functions vidExtFunctions = {16,
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
                                                 qtVidExtFuncGLGetDefaultFramebuffer,
                                                 qtVidExtFuncGetVkSurface,
                                                 qtVidExtFuncGetVkInstExtensions};

void MainWindow::updatePlugins()
{
#ifdef PLUGIN_DIR_PATH
    QString pluginPath = PLUGIN_DIR_PATH;
#else
    QString pluginPath = QCoreApplication::applicationDirPath();
#endif
    QDir PluginDir(pluginPath);
    PluginDir.setFilter(QDir::Files);
    QStringList Filter;
    Filter.append("");
    QStringList current;
    QString default_value;

    if (!settings->contains("inputPlugin")) {
        Filter.replace(0,"*-input-*");
        current = PluginDir.entryList(Filter);
        default_value = "simple64-input-qt";
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
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb *.gbc)"));
        if (!filename.isNull()) {
            settings->setValue("Player1GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 1 ROM: " + current);
        }
    });

    QAction *fileSelect2 = new QAction(this);
    current = settings->value("Player1GBRAM").toString();
    fileSelect2->setText("Player 1 SAV: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB SAV Files (*.sav *.ram)"));
        if (!filename.isNull()) {
            settings->setValue("Player1GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 1 SAV: " + current);
        }
    });

    QAction *clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 1 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player1GBROM");
        settings->remove("Player1GBRAM");
        fileSelect->setText("Player 1 ROM: ");
        fileSelect2->setText("Player 1 SAV: ");
    });
    GB->addSeparator();

    fileSelect = new QAction(this);
    current = settings->value("Player2GBROM").toString();
    fileSelect->setText("Player 2 ROM: " + current);
    GB->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb *.gbc)"));
        if (!filename.isNull()) {
            settings->setValue("Player2GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 2 ROM: " + current);
        }
    });

    fileSelect2 = new QAction(this);
    current = settings->value("Player2GBRAM").toString();
    fileSelect2->setText("Player 2 SAV: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB SAV Files (*.sav *.ram)"));
        if (!filename.isNull()) {
            settings->setValue("Player2GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 2 SAV: " + current);
        }
    });

    clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 2 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player2GBROM");
        settings->remove("Player2GBRAM");
        fileSelect->setText("Player 2 ROM: ");
        fileSelect2->setText("Player 2 SAV: ");
    });
    GB->addSeparator();

    fileSelect = new QAction(this);
    current = settings->value("Player3GBROM").toString();
    fileSelect->setText("Player 3 ROM: " + current);
    GB->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb *.gbc)"));
        if (!filename.isNull()) {
            settings->setValue("Player3GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 3 ROM: " + current);
        }
    });

    fileSelect2 = new QAction(this);
    current = settings->value("Player3GBRAM").toString();
    fileSelect2->setText("Player 3 SAV: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB SAV Files (*.sav *.ram)"));
        if (!filename.isNull()) {
            settings->setValue("Player3GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 3 SAV: " + current);
        }
    });

    clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 3 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player3GBROM");
        settings->remove("Player3GBRAM");
        fileSelect->setText("Player 3 ROM: ");
        fileSelect2->setText("Player 3 SAV: ");
    });
    GB->addSeparator();

    fileSelect = new QAction(this);
    current = settings->value("Player4GBROM").toString();
    fileSelect->setText("Player 4 ROM: " + current);
    GB->addAction(fileSelect);
    connect(fileSelect, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB ROM File"), NULL, tr("GB ROM Files (*.gb *.gbc)"));
        if (!filename.isNull()) {
            settings->setValue("Player4GBROM", filename);
            QString current = filename;
            fileSelect->setText("Player 4 ROM: " + current);
        }
    });

    fileSelect2 = new QAction(this);
    current = settings->value("Player4GBRAM").toString();
    fileSelect2->setText("Player 4 SAV: " + current);
    GB->addAction(fileSelect2);
    connect(fileSelect2, &QAction::triggered,[=](){
        QString filename = QFileDialog::getOpenFileName(this,
            tr("GB RAM File"), NULL, tr("GB SAV Files (*.sav *.ram)"));
        if (!filename.isNull()) {
            settings->setValue("Player4GBRAM", filename);
            QString current = filename;
            fileSelect2->setText("Player 4 SAV: " + current);
        }
    });

    clearSelect = new QAction(this);
    clearSelect->setText("Clear Player 4 Selections");
    GB->addAction(clearSelect);
    connect(clearSelect, &QAction::triggered,[=](){
        settings->remove("Player4GBROM");
        settings->remove("Player4GBRAM");
        fileSelect->setText("Player 4 ROM: ");
        fileSelect2->setText("Player 4 SAV: ");
    });
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    verbose = 0;
    nogui = 0;
    run_test = 0;
    ui->setupUi(this);
    this->setWindowTitle("simple64    build: " + QStringLiteral(GUI_VERSION).mid(0,7));

    QString ini_path = QDir(QCoreApplication::applicationDirPath()).filePath("simple64-gui.ini");
    settings = new QSettings(ini_path, QSettings::IniFormat, this);

    if (!settings->isWritable())
        settings = new QSettings("simple64", "gui", this);

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

    // migrate from old $CONFIG_PATH$ default
    if (settings->value("configDirPath").toString().startsWith("$"))
        settings->remove("configDirPath");

#ifdef CONFIG_DIR_PATH
    settings->setValue("configDirPath", CONFIG_DIR_PATH);
#endif

    updatePlugins();

    if (!settings->contains("volume"))
        settings->setValue("volume", 100);
    VolumeAction * volumeAction = new VolumeAction(tr("Volume"), this);
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
    FPSLabel = new QLabel(this);
    statusBar()->addPermanentWidget(FPSLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateApp()
{
#ifdef _AUTOUPDATE
    QString disable_update = qEnvironmentVariable("SIMPLE64_DISABLE_UPDATER");
    if (!disable_update.isEmpty())
        return;
#ifndef __APPLE__
    QNetworkAccessManager *updateManager = new QNetworkAccessManager(this);
    connect(updateManager, &QNetworkAccessManager::finished,
        this, &MainWindow::updateReplyFinished);

    updateManager->get(QNetworkRequest(QUrl("https://api.github.com/repos/simple64/simple64/releases/latest")));
#endif
#endif
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
            download_message->done(QDialog::Accepted);
            QString fullpath = dir.filePath(reply->url().fileName());
            QFile file(fullpath);
            file.open(QIODevice::WriteOnly);
            file.write(reply->readAll());
            file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner);
            file.close();
            QProcess process;
            process.setProgram(fullpath);
            QStringList arguments = { QCoreApplication::applicationDirPath() };
            process.setArguments(arguments);
            process.startDetached();
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
                QNetworkRequest req(QUrl("https://github.com/simple64/simple64-updater/releases/latest/download/simple64-updater.exe"));
#else
                QNetworkRequest req(QUrl("https://github.com/simple64/simple64-updater/releases/latest/download/simple64-updater"));
#endif
                req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
                updateManager->get(req);
                download_message = new QMessageBox(this);
                download_message->setStandardButtons(QMessageBox::NoButton);
                download_message->setText("Downloading updater");
                download_message->show();
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

int MainWindow::getNoGUI()
{
    return nogui;
}

void MainWindow::setTest(int value)
{
    run_test = value;
}

int MainWindow::getTest()
{
    return run_test;
}

int MainWindow::getVerbose()
{
    return verbose;
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
    QStringList list = settings->value("RecentROMs2").toStringList();
    if (list.isEmpty()) {
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
                    openROM(temp_recent->text(), "", 0, 0);
                });
    }
    OpenRecent->addSeparator();

    QAction *clearRecent = new QAction(this);
    clearRecent->setText("Clear List");
    OpenRecent->addAction(clearRecent);
    connect(clearRecent, &QAction::triggered,[=](){
        settings->remove("RecentROMs2");
        updateOpenRecent();
    });
}

void MainWindow::showMessage(QString message)
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText(message);
    msgBox->show();
}

void MainWindow::createVkWindow(QVulkanInstance* vulkan_inst)
{
    my_window = new VkWindow;
    my_window->setVulkanInstance(vulkan_inst);

    QWidget *container = QWidget::createWindowContainer(my_window, this);
    container->setFocusPolicy(Qt::StrongFocus);

    my_window->setCursor(Qt::BlankCursor);

    setCentralWidget(container);

    my_window->installEventFilter(&keyPressFilter);
    this->installEventFilter(&keyPressFilter);
    frame_timer = new QTimer(this);
    connect(frame_timer, &QTimer::timeout, this, &MainWindow::updateFrameCount);
    frame_timer->start(1000);
    frame_count = 0;
}

void MainWindow::deleteVkWindow()
{
    QWidget *container = new QWidget(this);
    setCentralWidget(container);
    delete my_window;
    frame_timer->stop();
    frame_timer->deleteLater();
    FPSLabel->clear();
}

void MainWindow::killThread()
{
    printf("Application hung, exiting\n");
    exit(1); // Force kill the application, it's stuck
}

void MainWindow::stopGame()
{
    if (!coreLib) return;

    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
    if (response != M64EMU_STOPPED)
    {
        (*CoreDoCommand)(M64CMD_STOP, 0, NULL);
        kill_timer = new QTimer(this);
        kill_timer->setInterval(5000);
        kill_timer->setSingleShot(true);
        connect(kill_timer, &QTimer::timeout, this, &MainWindow::killThread);
        kill_timer->start();
        while (workerThread->isRunning())
            QCoreApplication::processEvents();
        kill_timer->stop();
        kill_timer->deleteLater();
    }
}

void MainWindow::resetCore()
{
    closePlugins();
    closeCoreLib();
    loadCoreLib();
    loadPlugins();
}

void MainWindow::openROM(QString filename, QString netplay_ip, int netplay_port, int netplay_player)
{
    stopGame();

    logViewer.clearLog();

    resetCore();

    workerThread = new WorkerThread(netplay_ip, netplay_port, netplay_player, this);
    workerThread->setFileName(filename);

    QStringList list;
    if (settings->contains("RecentROMs2"))
        list = settings->value("RecentROMs2").toStringList();
    list.removeAll(filename);
    list.prepend(filename);
    if (list.size() > RECENT_SIZE)
        list.removeLast();
    settings->setValue("RecentROMs2",list);
    updateOpenRecent();

    workerThread->start();
}

void MainWindow::on_actionOpen_ROM_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open ROM"), settings->value("ROMdir").toString(), tr("ROM Files (*.n64 *.N64 *.z64 *.Z64 *.v64 *.V64 *.rom *.ROM *.zip *.ZIP *.7z)"));
    if (!filename.isNull()) {
        QFileInfo info(filename);
        settings->setValue("ROMdir", info.absoluteDir().absolutePath());
        openROM(filename, "", 0, 0);
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
    int response;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &response);
    if (response == M64EMU_RUNNING)
        (*CoreDoCommand)(M64CMD_PAUSE, 0, NULL);
    else if (response == M64EMU_PAUSED)
        (*CoreDoCommand)(M64CMD_RESUME, 0, NULL);
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

void MainWindow::on_actionHotkey_Configuration_triggered()
{
    HotkeyDialog *settings = new HotkeyDialog(this);
    settings->show();
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
    QDesktopServices::openUrl(QUrl("https://www.patreon.com/loganmc10"));
}

void MainWindow::on_actionSupport_on_GithubSponser_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/sponsors/loganmc10"));
}

void MainWindow::on_actionOpen_Discord_Channel_triggered()
{
    QDesktopServices::openUrl(QUrl("https://discord.gg/tsR3RtYynZ"));
}

WorkerThread* MainWindow::getWorkerThread()
{
    return workerThread;
}

VkWindow* MainWindow::getVkWindow()
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
#ifdef CORE_LIBRARY_PATH
    QString corePath = CORE_LIBRARY_PATH;
#else
    QString corePath = QCoreApplication::applicationDirPath();
#endif
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
    ConfigGetParameterHelp =      (ptr_ConfigGetParameterHelp) osal_dynlib_getproc(coreLib, "ConfigGetParameterHelp");
    ConfigGetParamInt =           (ptr_ConfigGetParamInt) osal_dynlib_getproc(coreLib, "ConfigGetParamInt");
    ConfigGetParamFloat =         (ptr_ConfigGetParamFloat) osal_dynlib_getproc(coreLib, "ConfigGetParamFloat");
    ConfigGetParamBool =          (ptr_ConfigGetParamBool) osal_dynlib_getproc(coreLib, "ConfigGetParamBool");
    ConfigGetParamString =        (ptr_ConfigGetParamString) osal_dynlib_getproc(coreLib, "ConfigGetParamString");
    ConfigSetParameter =          (ptr_ConfigSetParameter) osal_dynlib_getproc(coreLib, "ConfigSetParameter");
    ConfigDeleteSection =         (ptr_ConfigDeleteSection) osal_dynlib_getproc(coreLib, "ConfigDeleteSection");
    ConfigOpenSection =           (ptr_ConfigOpenSection) osal_dynlib_getproc(coreLib, "ConfigOpenSection");
    ConfigSaveSection =           (ptr_ConfigSaveSection) osal_dynlib_getproc(coreLib, "ConfigSaveSection");
    ConfigListParameters =        (ptr_ConfigListParameters) osal_dynlib_getproc(coreLib, "ConfigListParameters");
    ConfigGetSharedDataFilepath = (ptr_ConfigGetSharedDataFilepath) osal_dynlib_getproc(coreLib, "ConfigGetSharedDataFilepath");

    QString qtConfigDir = settings->value("configDirPath").toString();

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

typedef void (*ptr_VolumeSetLevel)(int level);
void MainWindow::loadPlugins()
{
    if (coreLib == nullptr)
        return;

    ptr_VolumeSetLevel VolumeSetLevel;
    ptr_PluginStartup PluginStartup;
    ptr_PluginGetVersion PluginGetVersion;
    m64p_error res;
    QMessageBox msgBox;
#ifdef PLUGIN_DIR_PATH
    QString pluginPath = PLUGIN_DIR_PATH;
#else
    QString pluginPath = QCoreApplication::applicationDirPath();
#endif
    QString file_path;
    QString plugin_path;

    file_path = "simple64-video-parallel";
    file_path += OSAL_DLL_EXTENSION;
    plugin_path = QDir(pluginPath).filePath(file_path);

    res = osal_dynlib_open(&gfxPlugin, plugin_path.toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load video plugin");
        msgBox.exec();
        return;
    }

    PluginGetVersion = (ptr_PluginGetVersion) osal_dynlib_getproc(gfxPlugin, "PluginGetVersion");
    const char* pluginName;
    (*PluginGetVersion)(NULL, NULL, NULL, &pluginName, NULL);
    if (strcmp("parallel", pluginName))
    {
        osal_dynlib_close(gfxPlugin);
        gfxPlugin = nullptr;
        msgBox.setText("Incorrect GFX plugin");
        msgBox.exec();
        return;
    }

    PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(gfxPlugin, "PluginStartup");
    (*PluginStartup)(coreLib, (char*)"Video", DebugCallback);

    file_path = "simple64-audio-sdl2";
    file_path += OSAL_DLL_EXTENSION;
    plugin_path = QDir(pluginPath).filePath(file_path);

    res = osal_dynlib_open(&audioPlugin, plugin_path.toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load audio plugin");
        msgBox.exec();
        return;
    }

    PluginGetVersion = (ptr_PluginGetVersion) osal_dynlib_getproc(audioPlugin, "PluginGetVersion");
    (*PluginGetVersion)(NULL, NULL, NULL, &pluginName, NULL);
    if (strcmp("simple64 SDL2 Audio Plugin", pluginName))
    {
        osal_dynlib_close(audioPlugin);
        audioPlugin = nullptr;
        msgBox.setText("Incorrect Audio plugin");
        msgBox.exec();
        return;
    }

    PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(audioPlugin, "PluginStartup");
    (*PluginStartup)(coreLib, (char*)"Audio", DebugCallback);
    int volume = 100;
    if (settings->contains("volume"))
        volume = settings->value("volume").toInt();
    VolumeSetLevel = (ptr_VolumeSetLevel) osal_dynlib_getproc(audioPlugin, "VolumeSetLevel");
    (*VolumeSetLevel)(volume);

    res = osal_dynlib_open(&inputPlugin, QDir(pluginPath).filePath(settings->value("inputPlugin").toString()).toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load input plugin");
        msgBox.exec();
        return;
    }
    PluginStartup = (ptr_PluginStartup) osal_dynlib_getproc(inputPlugin, "PluginStartup");
    if (settings->value("inputPlugin").toString().contains("-qt"))
        (*PluginStartup)(coreLib, this, DebugCallback);
    else
        (*PluginStartup)(coreLib, (char*)"Input", DebugCallback);


    file_path = "simple64-rsp-parallel";
    file_path += OSAL_DLL_EXTENSION;
    plugin_path = QDir(pluginPath).filePath(file_path);

    res = osal_dynlib_open(&rspPlugin, plugin_path.toUtf8().constData());
    if (res != M64ERR_SUCCESS)
    {
        msgBox.setText("Failed to load rsp plugin");
        msgBox.exec();
        return;
    }

    PluginGetVersion = (ptr_PluginGetVersion) osal_dynlib_getproc(rspPlugin, "PluginGetVersion");
    (*PluginGetVersion)(NULL, NULL, NULL, &pluginName, NULL);
    if (strcmp("ParaLLEl RSP", pluginName))
    {
        osal_dynlib_close(rspPlugin);
        rspPlugin = nullptr;
        msgBox.setText("Incorrect RSP plugin");
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

void MainWindow::addFrameCount()
{
    ++frame_count;
}

void MainWindow::simulateInput()
{
    int keyValue = 225; // Shift/A
    if (run_test % 2)
        (*CoreDoCommand)(M64CMD_SEND_SDL_KEYDOWN, keyValue, NULL);
    else
    {
        (*CoreDoCommand)(M64CMD_SEND_SDL_KEYUP, keyValue, NULL);
        if (keyValue = 225)
            keyValue = 40; // Enter/Start
        else
            keyValue = 225;
    }
    printf("%u\n", frame_count);
    if (run_test == 1)
    {
        if (frame_count == 0)
            abort();
        else
            (*CoreDoCommand)(M64CMD_STOP, 0, NULL);
    }
    --run_test;
}

void MainWindow::updateFrameCount()
{
    if (run_test)
        simulateInput();
    QString FPS = QString("%1 FPS").arg(frame_count);
    FPSLabel->setText(FPS);
    frame_count = 0;
}
