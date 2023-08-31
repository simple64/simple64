#ifndef NETPLAY_COMMON_H
#define NETPLAY_COMMON_H

#include <QWebSocket>
#include <QJsonObject>

void sendNetplayMessage(QWebSocket* webSocket, QJsonObject json);

#endif
