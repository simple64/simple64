#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QDialog>
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
    void processMulticast();
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
    int launched;
    QString filename;
    QUdpSocket multicastSocket;
};

#endif // CREATEROOM_H
