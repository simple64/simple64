#ifndef JOINROOM_H
#define JOINROOM_H

#include <QDialog>
#include <QComboBox>
#include <QtNetwork>
#include <QTableWidget>
#include <QWebSocket>
#include <QLineEdit>
#include <QPushButton>

class JoinRoom : public QDialog
{
    Q_OBJECT
public:
    JoinRoom(QWidget *parent = nullptr);
private slots:
    void downloadFinished(QNetworkReply *reply);
    void serverChanged(QString server);
    void onConnected();
    void processBinaryMessage(QByteArray message);
    void refresh();
    void joinGame();
    void onFinished(int result);
    void processBroadcast();
private:
    void resetList();
    QComboBox *serverChooser;
    QNetworkAccessManager manager;
    QTableWidget *listWidget;
    QWebSocket *webSocket = nullptr;
    QLineEdit *playerName;
    QLineEdit *passwordEdit;
    QPushButton *joinButton;
    QPushButton *refreshButton;
    QList<QJsonObject> rooms;
    int row = 0;
    int launched;
    QString filename;
    QUdpSocket broadcastSocket;
};

#endif // JOINROOM_H
