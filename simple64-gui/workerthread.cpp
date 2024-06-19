
#include "workerthread.h"
#include "vidext.h"
#include "mainwindow.h"
#include "interface/core_commands.h"
#ifndef _WIN32
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusInterface>
#endif

WorkerThread::WorkerThread(QString _netplay_ip, int _netplay_port, int _netplay_player, QJsonObject _cheats, QObject *parent)
    : QThread(parent)
{
#ifdef _WIN32
    setStackSize(4194304);
#endif
    netplay_ip = _netplay_ip;
    netplay_port = _netplay_port;
    netplay_player = _netplay_player;
    cheats = _cheats;
}

void WorkerThread::run()
{
    connect(this, &WorkerThread::resizeMainWindow, w, &MainWindow::resizeMainWindow, Qt::BlockingQueuedConnection);
    connect(this, &WorkerThread::toggleFS, w, &MainWindow::toggleFS, Qt::BlockingQueuedConnection);
    connect(this, &WorkerThread::createVkWindow, w, &MainWindow::createVkWindow, Qt::BlockingQueuedConnection);
    connect(this, &WorkerThread::deleteVkWindow, w, &MainWindow::deleteVkWindow, Qt::BlockingQueuedConnection);
    connect(this, &WorkerThread::showMessage, w, &MainWindow::showMessage, Qt::QueuedConnection);
    connect(this, &WorkerThread::updateDiscordActivity, w, &MainWindow::updateDiscordActivity, Qt::BlockingQueuedConnection);
    connect(this, &WorkerThread::clearDiscordActivity, w, &MainWindow::clearDiscordActivity, Qt::BlockingQueuedConnection);
    connect(this, &WorkerThread::setCheats, w, &MainWindow::setCheats, Qt::BlockingQueuedConnection);
    connect(this, &WorkerThread::addLog, w->getLogViewer(), &LogViewer::addLog, Qt::QueuedConnection);
    connect(this, &WorkerThread::addFrameCount, w, &MainWindow::addFrameCount, Qt::QueuedConnection);
#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
#else
    uint32_t cookieID = 0;
    QDBusInterface screenSaverInterface("org.freedesktop.ScreenSaver", "/org/freedesktop/ScreenSaver", "org.freedesktop.ScreenSaver");
    if (screenSaverInterface.isValid())
    {
        QDBusReply<uint32_t> reply = screenSaverInterface.call("Inhibit", "simple64-gui", "game");
        if (reply.isValid())
            cookieID = reply.value();
    }
#endif

    m64p_error res = loadROM(m_fileName);
    if (res == M64ERR_SUCCESS)
    {
        m64p_rom_settings rom_settings;
        (*CoreDoCommand)(M64CMD_ROM_GET_SETTINGS, sizeof(rom_settings), &rom_settings);
        struct DiscordActivity activity;
        struct DiscordActivityAssets assets;
        struct DiscordActivityTimestamps timestamps;
        memset(&activity, 0, sizeof(activity));
        memset(&assets, 0, sizeof(assets));
        memset(&timestamps, 0, sizeof(timestamps));
        QDateTime current = QDateTime::currentDateTimeUtc();
        timestamps.start = current.currentSecsSinceEpoch();
        strcpy(assets.large_image, "6205049");
        strcpy(assets.large_text, "https://simple64.github.io");
        activity.assets = assets;
        activity.timestamps = timestamps;
        strncpy(activity.details, rom_settings.goodname, 128);
        emit updateDiscordActivity(activity);

        emit setCheats(cheats, netplay_port > 0);

        res = launchGame(netplay_ip, netplay_port, netplay_player);

        emit clearDiscordActivity();
    }

#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS);
#else
    if (cookieID)
        screenSaverInterface.call("UnInhibit", cookieID);
#endif

    if (res == M64ERR_SUCCESS)
        (*ConfigSaveFile)();

    if (w->getNoGUI())
        QApplication::quit();
}

void WorkerThread::setFileName(QString filename)
{
    m_fileName = filename;
}
