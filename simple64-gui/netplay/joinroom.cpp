#include "joinroom.h"
#include "waitroom.h"
#include "../mainwindow.h"
#include "../interface/core_commands.h"
#include "../version.h"
#include <QGridLayout>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>

JoinRoom::JoinRoom(QWidget *parent)
    : QDialog(parent)
{
    setMinimumWidth(1000);
    setMinimumHeight(500);
    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnMinimumWidth(1, 500);

    QRegularExpression rx("[a-zA-Z0-9]+");
    QValidator *validator = new QRegularExpressionValidator(rx, this);

    playerNameEdit = new QLineEdit(this);
    playerNameEdit->setValidator(validator);
    playerNameEdit->setMaxLength(30);
    if (w->getSettings()->contains("netplay_name"))
        playerNameEdit->setText(w->getSettings()->value("netplay_name").toString());
    playerNameEdit->setPlaceholderText("Player Name");
    layout->addWidget(playerNameEdit, 0, 0);

    pingLabel = new QLabel("Ping: (Calculating)", this);
    layout->addWidget(pingLabel, 0, 2);

    serverChooser = new QComboBox(this);
    serverChooser->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(serverChooser, SIGNAL(currentIndexChanged(int)), this, SLOT(serverChanged(int)));

    layout->addWidget(serverChooser, 0, 3);

    refreshButton = new QPushButton(this);
    refreshButton->setText("Refresh");
    layout->addWidget(refreshButton, 0, 4);
    connect(refreshButton, &QPushButton::released, this, &JoinRoom::refresh);

    listWidget = new QTableWidget(this);
    listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    resetList();

    layout->addWidget(listWidget, 1, 0, 1, 5);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password (if required)");
    layout->addWidget(passwordEdit, 2, 0);

    joinButton = new QPushButton(this);
    connect(joinButton, &QPushButton::released, this, &JoinRoom::joinGame);
    joinButton->setText("Join Game");
    layout->addWidget(joinButton, 2, 2, 1, 2);

    setLayout(layout);

    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));

    connect(this, SIGNAL (finished(int)), this, SLOT (onFinished(int)));

    QNetworkRequest request(QUrl(QStringLiteral("https://m64p.s3.amazonaws.com/servers.json")));
    manager.get(request);

    broadcastSocket.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    connect(&broadcastSocket, &QUdpSocket::readyRead, this, &JoinRoom::processBroadcast);
    QByteArray multirequest;
    multirequest.append(1);
    broadcastSocket.writeDatagram(multirequest, QHostAddress::Broadcast, 45000);

    launched = 0;
}

void JoinRoom::processBroadcast()
{
    while (broadcastSocket.hasPendingDatagrams())
    {
        QNetworkDatagram datagram = broadcastSocket.receiveDatagram();
        QByteArray incomingData = datagram.data();
        QJsonDocument json_doc = QJsonDocument::fromJson(incomingData);
        QJsonObject json = json_doc.object();
        QStringList servers = json.keys();
        for (int i = 0; i < servers.size(); ++i)
            serverChooser->addItem(servers.at(i), json.value(servers.at(i)).toString());
    }
}

void JoinRoom::onFinished(int)
{
    broadcastSocket.close();
    (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
    if (!launched && webSocket)
    {
        webSocket->close();
        webSocket->deleteLater();
    }
}

void JoinRoom::resetList()
{
    row = 0;
    rooms.clear();
    listWidget->clear();
    listWidget->setColumnCount(4);
    listWidget->setRowCount(row);
    QStringList headers;
    headers.append("Room Name");
    headers.append("Game Name");
    headers.append("Game MD5");
    headers.append("Password Protected");
    //headers.append("Fixed Input Delay");
    listWidget->setHorizontalHeaderLabels(headers);
    listWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void JoinRoom::downloadFinished(QNetworkReply *reply)
{
    if (!reply->error())
    {
        QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject json = json_doc.object();
        QStringList servers = json.keys();
        for (int i = 0; i < servers.size(); ++i)
            serverChooser->addItem(servers.at(i), json.value(servers.at(i)).toString());
        serverChooser->addItem(QString("Custom"), QString("Custom"));
    }

    reply->deleteLater();
}

void JoinRoom::refresh()
{
    serverChanged(serverChooser->currentIndex());
}

void JoinRoom::joinGame()
{
    QMessageBox msgBox;
    if (webSocket && webSocket->state() != QAbstractSocket::ConnectedState)
    {
        msgBox.setText("Could not connect to server");
        msgBox.exec();
        return;
    }
    if (listWidget->currentRow() < 0)
    {
        msgBox.setText("You haven't selected a game to join");
        msgBox.exec();
        return;
    }

    filename = QFileDialog::getOpenFileName(this,
    tr("Open ROM"), w->getSettings()->value("ROMdir").toString(), tr("ROM Files (*.n64 *.N64 *.z64 *.Z64 *.v64 *.V64 *.rom *.ROM *.zip *.ZIP *.7z)"));
    if (!filename.isNull())
    {
        if (loadROM(filename) == M64ERR_SUCCESS)
        {
            int room_port = rooms.at(listWidget->currentRow()).value("port").toInt();
            m64p_rom_settings rom_settings;
            (*CoreDoCommand)(M64CMD_ROM_GET_SETTINGS, sizeof(rom_settings), &rom_settings);

            joinButton->setEnabled(false);
            QJsonObject json;
            json.insert("type", "request_join_room");
            json.insert("player_name", playerNameEdit->text());
            json.insert("password", passwordEdit->text());
            json.insert("client_sha", QStringLiteral(GUI_VERSION));
            json.insert("MD5", QString(rom_settings.MD5));
            json.insert("port", room_port);

            QJsonDocument json_doc(json);
            webSocket->sendBinaryMessage(json_doc.toJson());
        }
        else
        {
            msgBox.setText("Could not open ROM");
            msgBox.exec();
        }
    }
}

void JoinRoom::serverChanged(int index)
{
    QString serverName = serverChooser->itemData(index).toString();
    if (webSocket)
    {
        webSocket->close();
        webSocket->deleteLater();
        webSocket = NULL;
    }

    if (serverName == "Custom")
    {
        bool ok;
        customServerAddress = QInputDialog::getText(this, "Custom Netplay Server", "IP Address / Host:", QLineEdit::Normal, "", &ok);

        if (!ok || customServerAddress.isEmpty())
        {
            customServerAddress.clear();
            resetList();
            return;
        }
        else
        {
            customServerAddress.prepend("ws://");
        }
    }
    else
    {
        customServerAddress.clear();
    }

    pingLabel->setText("Ping: (Calculating)");

    resetList();
    webSocket = new QWebSocket();
    connect(webSocket, &QWebSocket::connected, this, &JoinRoom::onConnected);
    connectionTimer = new QTimer(this);
    connectionTimer->setSingleShot(true);
    connectionTimer->start(5000);
    connect(webSocket, &QWebSocket::disconnected, connectionTimer, &QTimer::stop);
    connect(webSocket, &QObject::destroyed, connectionTimer, &QTimer::stop);

    connect(webSocket, &QWebSocket::binaryMessageReceived, this, &JoinRoom::processBinaryMessage);

    QTimer *pingTimer = new QTimer(this);
    connect(webSocket, &QWebSocket::pong, this, &JoinRoom::updatePing);
    connect(pingTimer, &QTimer::timeout, this, &JoinRoom::sendPing);
    connect(webSocket, &QWebSocket::disconnected, pingTimer, &QTimer::stop);
    connect(webSocket, &QObject::destroyed, pingTimer, &QTimer::stop);
    pingTimer->start(2500);
    webSocket->ping();

    QString serverUrlStr = customServerAddress.isEmpty() ? serverChooser->currentData().toString() : customServerAddress;
    QUrl serverUrl = QUrl(serverUrlStr);
    if (!customServerAddress.isEmpty() && serverUrl.port() < 0)
        // Be forgiving of custom server addresses that forget the port
        serverUrl.setPort(45000);

    webSocket->open(serverUrl);
}


void JoinRoom::onConnected()
{
    connectionTimer->stop();

    QJsonObject json;
    json.insert("type", "request_get_rooms");
    json.insert("netplay_version", NETPLAY_VER);
    json.insert("emulator", "simple64");
    QJsonDocument json_doc(json);
    webSocket->sendBinaryMessage(json_doc.toJson());
}

void JoinRoom::processBinaryMessage(QByteArray message)
{
    QJsonDocument json_doc = QJsonDocument::fromJson(message);
    QJsonObject json = json_doc.object();
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);

    if (json.value("type").toString() == "reply_get_rooms")
    {
        if (json.value("accept").toInt() == 0)
        {
            json.remove("type");
            rooms << json;

            listWidget->insertRow(row);
            QTableWidgetItem *newItem = new QTableWidgetItem(json.value("room_name").toString());
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
            listWidget->setItem(row, 0, newItem);
            newItem = new QTableWidgetItem(json.value("game_name").toString());
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
            listWidget->setItem(row, 1, newItem);
            newItem = new QTableWidgetItem(json.value("MD5").toString());
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
            listWidget->setItem(row, 2, newItem);
            newItem = new QTableWidgetItem(json.value("protected").toString());
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
            listWidget->setItem(row, 3, newItem);

            ++row;
        }
        else
        {
            msgBox.setText(json.value("message").toString());
            msgBox.exec();
        }
    }
    else if (json.value("type").toString() == "reply_join_room")
    {
        if (json.value("accept").toInt() == 0)
        {
            json.remove("type");
            json.remove("accept");
            launched = 1;
            WaitRoom *waitRoom = new WaitRoom(filename, json, webSocket, parentWidget());
            waitRoom->show();
            accept();
            return;
        }
        else
        {
            (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
            msgBox.setText(json.value("message").toString());
            msgBox.exec();
        }
        joinButton->setEnabled(true);
    }
}

void JoinRoom::updatePing(quint64 elapsedTime, const QByteArray&)
{
    pingLabel->setText("Ping: " + QString::number(elapsedTime) + " ms");
}

void JoinRoom::sendPing()
{
    webSocket->ping();
}
