#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QApplication>
#include <QString>
#include <QVulkanInstance>
#include <QJsonObject>
#include "interface/common.h"
#include "discord/discord_game_sdk.h"

class WorkerThread
    : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;

public:
    explicit WorkerThread(QString _netplay_ip, int _netplay_port, int _netplay_player, QJsonObject _cheats, QObject *parent = 0);
    void setFileName(QString filename);
signals:
    void resizeMainWindow(int Width, int Height);
    void toggleFS(int force);
    void createVkWindow(QVulkanInstance *instance);
    void deleteVkWindow();
    void showMessage(QString message);
    void updateDiscordActivity(struct DiscordActivity activity);
    void clearDiscordActivity();
    void addLog(QString text);
    void addFrameCount();
    void setCheats(QJsonObject cheatsData, bool netplay);

private:
    QString m_fileName;
    QString netplay_ip;
    QJsonObject cheats;
    int netplay_port;
    int netplay_player;
};

#endif // WORKERTHREAD_H
