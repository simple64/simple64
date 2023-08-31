#include "netplay_common.h"
#include <QJsonDocument>

void sendNetplayMessage(QWebSocket* webSocket, QJsonObject json)
{
    QCryptographicHash hash = QCryptographicHash(QCryptographicHash::Sha256);
    QByteArray currentTime = QByteArray::number(QDateTime::currentMSecsSinceEpoch());

    hash.addData(currentTime);
    // this auth code is emulator specific, and is only checked when the server is running with the --enable-auth option
    hash.addData(QStringLiteral(NETPLAY_AUTH_CODE).toUtf8());

    json.insert("auth", QString(hash.result().toHex()));
    json.insert("authTime", QString(currentTime));

    QJsonDocument json_doc(json);
    webSocket->sendTextMessage(json_doc.toJson());
}
