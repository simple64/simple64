
#include "workerthread.h"
#include "vidext.h"
#include "mainwindow.h"
#include "interface/core_commands.h"
#ifndef _WIN32
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusInterface>
#endif

#ifdef SINGLE_THREAD
#define CONNECTION_TYPE Qt::AutoConnection
#else
#define CONNECTION_TYPE Qt::BlockingQueuedConnection
#endif

WorkerThread::WorkerThread(QString _netplay_ip, int _netplay_port, int _netplay_player, QObject *parent)
#ifdef SINGLE_THREAD
    : QObject(parent)
#else
    : QThread(parent)
#endif
{
    netplay_ip = _netplay_ip;
    netplay_port = _netplay_port;
    netplay_player = _netplay_player;
}

#ifdef SINGLE_THREAD
void WorkerThread::start()
#else
void WorkerThread::run()
#endif
{
    connect(this, SIGNAL(resizeMainWindow(int,int)), w, SLOT(resizeMainWindow(int, int)), CONNECTION_TYPE);
    connect(this, SIGNAL(toggleFS(int)), w, SLOT(toggleFS(int)), CONNECTION_TYPE);
    connect(this, SIGNAL(createOGLWindow(QSurfaceFormat*)), w, SLOT(createOGLWindow(QSurfaceFormat*)), CONNECTION_TYPE);
    connect(this, SIGNAL(deleteOGLWindow()), w, SLOT(deleteOGLWindow()), CONNECTION_TYPE);
    connect(this, SIGNAL(showMessage(QString)), w, SLOT(showMessage(QString)), CONNECTION_TYPE);
    connect(this, SIGNAL(updateDiscordActivity(struct DiscordActivity)), w, SLOT(updateDiscordActivity(struct DiscordActivity)), CONNECTION_TYPE);
    connect(this, SIGNAL(clearDiscordActivity()), w, SLOT(clearDiscordActivity()), CONNECTION_TYPE);
    connect(this, SIGNAL(addLog(QString)), w->getLogViewer(), SLOT(addLog(QString)), CONNECTION_TYPE);
#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED);
#else
    uint32_t cookieID = 0;
    QDBusInterface screenSaverInterface("org.freedesktop.ScreenSaver", "/org/freedesktop/ScreenSaver", "org.freedesktop.ScreenSaver");
    if (screenSaverInterface.isValid()) {
        QDBusReply<uint32_t> reply = screenSaverInterface.call("Inhibit", "mupen64plus-gui", "game");
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
        strcpy(assets.large_text, "https://m64p.github.io");
        activity.assets = assets;
        activity.timestamps = timestamps;
        strncpy(activity.details, rom_settings.goodname, 128);
        emit updateDiscordActivity(activity);

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
