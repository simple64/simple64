#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QApplication>
#include <QString>
#include <QSurfaceFormat>
#include "common.h"

class WorkerThread : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE;
public:
    explicit WorkerThread(QString _netplay_ip, int _netplay_port, int _netplay_player, QObject *parent = 0);
    void setFileName(QString filename);

signals:
    void resizeMainWindow(int Width, int Height);
    void toggleFS(int force);
    void createOGLWindow(QSurfaceFormat* format);
    void deleteOGLWindow();
    void pluginWarning(QString name);
    void showMessage(QString message);

private:
    QString m_fileName;
    QString netplay_ip;
    int netplay_port;
    int netplay_player;
};

#endif // WORKERTHREAD_H
