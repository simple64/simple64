#include "joinroom.h"
#include "waitroom.h"
#include "mainwindow.h"
#include "version.h"
#include <QGridLayout>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>

JoinRoom::JoinRoom(QWidget *parent)
    : QDialog(parent)
{
    setMinimumWidth(1000);
    setMinimumHeight(500);
    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnMinimumWidth(1, 500);

    playerName = new QLineEdit(this);
    playerName->setPlaceholderText("Player Name");
    layout->addWidget(playerName, 0, 0);

    QLabel *serverLabel = new QLabel("Server", this);
    serverLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(serverLabel, 0, 1);
    serverChooser = new QComboBox(this);
    serverChooser->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(serverChooser, SIGNAL(currentTextChanged(QString)), this, SLOT(serverChanged(QString)));

    layout->addWidget(serverChooser, 0, 2);

    refreshButton = new QPushButton(this);
    refreshButton->setText("Refresh");
    layout->addWidget(refreshButton, 0, 3);
    connect(refreshButton, &QPushButton::released, this, &JoinRoom::refresh);

    listWidget = new QTableWidget(this);
    listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    resetList();

    layout->addWidget(listWidget, 1, 0, 1, 4);

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

    launched = 0;
}

void JoinRoom::onFinished(int)
{
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
    }

    reply->deleteLater();
}

void JoinRoom::refresh()
{
    serverChanged("");
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
    if (playerName->text().isEmpty())
    {
        msgBox.setText("Player name can not be empty");
        msgBox.exec();
        return;
    }

    filename = QFileDialog::getOpenFileName(this,
    tr("Open ROM"), w->getSettings()->value("ROMdir").toString(), tr("ROM Files (*.n64 *.N64 *.z64 *.Z64 *.v64 *.V64 *.zip *.ZIP *.7z)"));
    if (!filename.isNull())
    {
        if (loadROM(filename.toStdString()) == M64ERR_SUCCESS)
        {
            QJsonObject json = rooms.at(listWidget->currentRow());
            m64p_rom_settings rom_settings;
            (*CoreDoCommand)(M64CMD_ROM_GET_SETTINGS, sizeof(rom_settings), &rom_settings);
            if (QString(rom_settings.MD5) == json.value("MD5").toString())
            {
                json.insert("type", "join_room");
                json.insert("player_name", playerName->text());
                json.insert("password", passwordEdit->text());
                json.insert("client_sha", QStringLiteral(GUI_VERSION));
                QJsonDocument json_doc(json);
                webSocket->sendBinaryMessage(json_doc.toBinaryData());
            }
            else
            {
                (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
                msgBox.setText("ROM does not match room ROM");
                msgBox.exec();
            }
        }
        else
        {
            msgBox.setText("Could not open ROM");
            msgBox.exec();
        }
    }
}

void JoinRoom::serverChanged(QString)
{
    if (webSocket)
    {
        webSocket->close();
        webSocket->deleteLater();
    }

    resetList();
    webSocket = new QWebSocket();
    connect(webSocket, &QWebSocket::connected, this, &JoinRoom::onConnected);
    webSocket->open(QUrl(serverChooser->currentData().toString()));
}

void JoinRoom::onConnected()
{
    connect(webSocket, &QWebSocket::binaryMessageReceived,
            this, &JoinRoom::processBinaryMessage);

    QJsonObject json;
    json.insert("type", "get_rooms");
    json.insert("netplay_version", NETPLAY_VER);
    QJsonDocument json_doc(json);
    webSocket->sendBinaryMessage(json_doc.toBinaryData());
}

void JoinRoom::processBinaryMessage(QByteArray message)
{
    QJsonDocument json_doc = QJsonDocument::fromBinaryData(message);
    QJsonObject json = json_doc.object();
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setTextInteractionFlags(Qt::TextBrowserInteraction);

    if (json.value("type").toString() == "message")
    {
        msgBox.setText(json.value("message").toString());
        msgBox.exec();
    }
    else if (json.value("type").toString() == "send_room")
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
    else if (json.value("type").toString() == "accept_join")
    {
        if (json.value("accept").toInt() == 0)
        {
            json.remove("type");
            json.remove("accept");
            launched = 1;
            WaitRoom *waitRoom = new WaitRoom(filename, json, webSocket, parentWidget());
            waitRoom->show();
            accept();
        }
        else if (json.value("accept").toInt() == 1)
        {
            (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
            msgBox.setText("Bad password");
            msgBox.exec();
        }
        else if (json.value("accept").toInt() == 2)
        {
            (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
            msgBox.setText("client versions do not match");
            msgBox.exec();
        }
        else
        {
           (*CoreDoCommand)(M64CMD_ROM_CLOSE, 0, NULL);
            msgBox.setText("Could not join room");
            msgBox.exec();
        }
    }
}
