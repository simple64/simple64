#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QWebSocket>
#include <QComboBox>
#include <QtNetwork>
#include "interface/common.h"

class CreateRoom : public QDialog
{
    Q_OBJECT
public:
    CreateRoom(QWidget *parent = nullptr);
private slots:
    void handleRomButton();
    void handleCreateButton();
    void onConnected();
    void downloadFinished(QNetworkReply *reply);
    void processBinaryMessage(QByteArray message);
    void onFinished(int result);
    void processBroadcast();
    void handleUseInputDelay(bool useInputDelay);
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
    QCheckBox *useInputDelay;
    QLineEdit *inputDelay;
    int launched;
    QString filename;
    QUdpSocket broadcastSocket;
};

#endif // CREATEROOM_H
