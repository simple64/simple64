#ifndef JOINROOM_H
#define JOINROOM_H

#include <QDialog>
#include <QComboBox>
#include <QtNetwork>
#include <QTableWidget>
#include <QWebSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class JoinRoom : public QDialog
{
    Q_OBJECT
public:
    JoinRoom(QWidget *parent = nullptr);
private slots:
    void downloadFinished(QNetworkReply *reply);
    void serverChanged(int index);
    void onConnected();
    void processBinaryMessage(QByteArray message);
    void refresh();
    void getIP();
    void joinGame();
    void ipReplyFinished(QNetworkReply *reply);
    void onFinished(int result);
    void processBroadcast();
    void sendPing();
    void updatePing(quint64 elapsedTime, const QByteArray&);
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
    //QLineEdit *inputDelay;
    QLabel *pingLabel;
    QList<QJsonObject> rooms;
    int row = 0;
    int launched;
    QString filename;
    QString clientIP;
    QUdpSocket broadcastSocket;
    QTimer *connectionTimer;
    QString customServerAddress;
};

#endif // JOINROOM_H
