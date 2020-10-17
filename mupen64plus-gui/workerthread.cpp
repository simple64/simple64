
#include "workerthread.h"
#include "vidext.h"
#include "mainwindow.h"
#include "interface/core_commands.h"
#ifndef _WIN32
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusInterface>
#endif

WorkerThread::WorkerThread(QString _netplay_ip, int _netplay_port, int _netplay_player, QObject *parent)
    : QThread(parent)
{
    netplay_ip = _netplay_ip;
    netplay_port = _netplay_port;
    netplay_player = _netplay_player;
}

void WorkerThread::run()
{
    connect(this, SIGNAL(resizeMainWindow(int,int)), w, SLOT(resizeMainWindow(int, int)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(toggleFS(int)), w, SLOT(toggleFS(int)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(createOGLWindow(QSurfaceFormat*)), w, SLOT(createOGLWindow(QSurfaceFormat*)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(deleteOGLWindow()), w, SLOT(deleteOGLWindow()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(setTitle(std::string)), w, SLOT(setTitle(std::string)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(pluginWarning(QString)), w, SLOT(pluginWarning(QString)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(showMessage(QString)), w, SLOT(showMessage(QString)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(setCoreStarted(int)), w, SLOT(setCoreStarted(int)), Qt::BlockingQueuedConnection);
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

    m64p_error res = loadROM(m_fileName.toStdString());
    if (res == M64ERR_SUCCESS)
    {
        emit setCoreStarted(1);
        res = launchGame(netplay_ip, netplay_port, netplay_player);
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
    else
        emit setCoreStarted(0);
}

void WorkerThread::setFileName(QString filename)
{
    m_fileName = filename;
}
