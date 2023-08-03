#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QWebSocket>
#include <QComboBox>
#include <QtNetwork>
#include "../interface/common.h"
#include <QLabel>

class CreateRoom : public QDialog
{
    Q_OBJECT
public:
    CreateRoom(QWidget *parent = nullptr);
private slots:
    void handleRomButton();
    void handleCreateButton();
    void createRoom();
    void downloadFinished(QNetworkReply *reply);
    void processBinaryMessage(QByteArray message);
    void onFinished(int result);
    void processBroadcast();
    void handleServerChanged(int index);
    void connectionFailed();
    void sendPing();
    void updatePing(quint64 elapsedTime, const QByteArray&);
private:
    QPushButton *romButton;
    QPushButton *createButton;
    QWebSocket *webSocket = nullptr;
    QNetworkAccessManager manager;
    QComboBox *serverChooser;
    m64p_rom_settings rom_settings;
    QLineEdit *nameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *playerNameEdit;
    QLabel *pingLabel;
    QLabel *pingValue;
    int launched;
    QString filename;
    QString playerName;
    QUdpSocket broadcastSocket;
    QString customServerHost;
    QTimer *connectionTimer;
};

#endif // CREATEROOM_H
