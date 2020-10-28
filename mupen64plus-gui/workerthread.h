#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QApplication>
#include <QString>
#include <QSurfaceFormat>
#include "common.h"
#include "discord/discord_game_sdk.h"

class WorkerThread
#ifndef SINGLE_THREAD
 : public QThread
#else
 : public QObject
#endif
{
    Q_OBJECT
#ifndef SINGLE_THREAD
    void run() Q_DECL_OVERRIDE;
#endif
public:
    explicit WorkerThread(QString _netplay_ip, int _netplay_port, int _netplay_player, QObject *parent = 0);
    void setFileName(QString filename);
#ifdef SINGLE_THREAD
    void start();
#endif
signals:
    void resizeMainWindow(int Width, int Height);
    void toggleFS(int force);
    void createOGLWindow(QSurfaceFormat* format);
    void deleteOGLWindow();
    void showMessage(QString message);
    void updateDiscordActivity(struct DiscordActivity activity);
    void clearDiscordActivity();

private:
    QString m_fileName;
    QString netplay_ip;
    int netplay_port;
    int netplay_player;
};

#endif // WORKERTHREAD_H
