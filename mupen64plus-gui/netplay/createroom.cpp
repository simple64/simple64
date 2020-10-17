#include "mainwindow.h"
#include "createroom.h"
#include "waitroom.h"
#include "version.h"
#include "interface/core_commands.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

CreateRoom::CreateRoom(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    QLabel *nameLabel = new QLabel("Room Name", this);
    layout->addWidget(nameLabel, 0, 0);
    nameEdit = new QLineEdit(this);
    layout->addWidget(nameEdit, 0, 1);

    QLabel *passwordLabel = new QLabel("Password (Optional)", this);
    layout->addWidget(passwordLabel, 1, 0);
    passwordEdit = new QLineEdit(this);
    layout->addWidget(passwordEdit, 1, 1);

    QLabel *romLabel = new QLabel("ROM", this);
    layout->addWidget(romLabel, 2, 0);
    romButton = new QPushButton("ROM Path", this);
    connect(romButton, SIGNAL (released()), this, SLOT (handleRomButton()));
    layout->addWidget(romButton, 2, 1);

    QLabel *playerNameLabel = new QLabel("Player Name", this);
    layout->addWidget(playerNameLabel, 3, 0);
    playerNameEdit = new QLineEdit(this);
    if (w->getSettings()->contains("netplay_name"))
        playerNameEdit->setText(w->getSettings()->value("netplay_name").toString());
    layout->addWidget(playerNameEdit, 3, 1);

    QLabel *serverLabel = new QLabel("Server", this);
    layout->addWidget(serverLabel, 4, 0);
    serverChooser = new QComboBox(this);
    serverChooser->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    layout->addWidget(serverChooser, 4, 1);

    QFrame* lineH1 = new QFrame(this);
    lineH1->setFrameShape(QFrame::HLine);
    lineH1->setFrameShadow(QFrame::Sunken);
    layout->addWidget(lineH1, 5, 0, 1, 2);

    createButton = new QPushButton("Create Game", this);
    connect(createButton, SIGNAL (released()), this, SLOT (handleCreateButton()));
    layout->addWidget(createButton, 6, 0, 1, 2);

    setLayout(layout);

    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));

    connect(this, SIGNAL (finished(int)), this, SLOT (onFinished(int)));

    QNetworkRequest request(QUrl(QStringLiteral("https://m64p.s3.amazonaws.com/servers.json")));
    manager.get(request);

    broadcastSocket.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    connect(&broadcastSocket, &QUdpSocket::readyRead, this, &CreateRoom::processBroadcast);
    QByteArray multirequest;
    multirequest.append(1);
    broadcastSocket.writeDatagram(multirequest, QHostAddress::Broadcast, 45000);

    launched = 0;
}

void CreateRoom::processBroadcast()
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

void CreateRoom::onFinished(int)
{
    broadcastSocket.close();
    (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
    if (!launched && webSocket)
    {
        webSocket->close();
        webSocket->deleteLater();
    }
}

void CreateRoom::handleRomButton()
{
    filename = QFileDialog::getOpenFileName(this,
        tr("Open ROM"), w->getSettings()->value("ROMdir").toString(), tr("ROM Files (*.n64 *.N64 *.z64 *.Z64 *.v64 *.V64 *.zip *.ZIP *.7z)"));
    if (!filename.isNull())
    {
        romButton->setText(filename);
    }
}

void CreateRoom::handleCreateButton()
{
    QMessageBox msgBox;
    if (nameEdit->text().isEmpty())
    {
        msgBox.setText("Room name can not be empty");
        msgBox.exec();
        return;
    }
    if (playerNameEdit->text().isEmpty())
    {
        msgBox.setText("Player name can not be empty");
        msgBox.exec();
        return;
    }
    if (loadROM(romButton->text().toStdString()) == M64ERR_SUCCESS)
    {
        createButton->setEnabled(false);
        if (webSocket)
        {
            webSocket->close();
            webSocket->deleteLater();
        }
        webSocket = new QWebSocket;
        (*CoreDoCommand)(M64CMD_ROM_GET_SETTINGS, sizeof(rom_settings), &rom_settings);
        connect(webSocket, &QWebSocket::connected, this, &CreateRoom::onConnected);
        webSocket->open(QUrl(serverChooser->currentData().toString()));
    }
    else
    {
        msgBox.setText("Could not open ROM");
        msgBox.exec();
    }
}

void CreateRoom::onConnected()
{
    connect(webSocket, &QWebSocket::binaryMessageReceived,
            this, &CreateRoom::processBinaryMessage);

    QJsonObject json;
    json.insert("type", "create_room");
    json.insert("room_name", nameEdit->text());
    json.insert("player_name", playerNameEdit->text());
    json.insert("password", passwordEdit->text());
    json.insert("MD5", QString(rom_settings.MD5));
    json.insert("game_name", QString(rom_settings.goodname));
    json.insert("client_sha", QStringLiteral(GUI_VERSION));
    json.insert("netplay_version", NETPLAY_VER);
    QJsonDocument json_doc(json);
    webSocket->sendBinaryMessage(json_doc.toJson());
}

void CreateRoom::processBinaryMessage(QByteArray message)
{
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);
    QJsonDocument json_doc = QJsonDocument::fromJson(message);
    QJsonObject json = json_doc.object();
    if (json.value("type").toString() == "message")
    {
        msgBox.setText(json.value("message").toString());
        msgBox.exec();
    }
    else if (json.value("type").toString() == "send_room_create")
    {
        json.remove("type");
        launched = 1;
        WaitRoom *waitRoom = new WaitRoom(filename, json, webSocket, parentWidget());
        waitRoom->show();
        accept();
    }
}

void CreateRoom::downloadFinished(QNetworkReply *reply)
{
    if (!reply->error())
    {
        QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject json = json_doc.object();
        QStringList servers = json.keys();
        for (int i = 0; i < servers.size(); ++i)
            serverChooser->addItem(servers.at(i), json.value(servers.at(i)).toString());
    }

    reply->deleteLater();
}
