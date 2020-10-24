
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
        res = launchGame(netplay_ip, netplay_port, netplay_player);

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
