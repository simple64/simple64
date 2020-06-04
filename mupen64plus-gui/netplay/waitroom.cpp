#include "waitroom.h"
#include "mainwindow.h"
#include <QGridLayout>
#include <QMessageBox>

WaitRoom::WaitRoom(QString filename, QJsonObject room, QWebSocket *socket, QWidget *parent)
    : QDialog(parent)
{
    this->resize(640,480);

    player_name = room.value("player_name").toString();
    room_port = room.value("port").toInt();
    room_name = room.value("room_name").toString();
    file_name = filename;

    w->getSettings()->setValue("netplay_name", player_name);

    webSocket = socket;
    connect(webSocket, &QWebSocket::binaryMessageReceived,
            this, &WaitRoom::processBinaryMessage);

    connect(webSocket, &QWebSocket::pong, this, &WaitRoom::updatePing);

    QGridLayout *layout = new QGridLayout(this);
    QLabel *nameLabel = new QLabel("Room Name:", this);
    layout->addWidget(nameLabel, 0, 0);
    QLabel *roomName = new QLabel(room_name, this);
    layout->addWidget(roomName, 0, 1);

    QLabel *gameLabel = new QLabel("Game Name:", this);
    layout->addWidget(gameLabel, 1, 0);
    QLabel *gameName = new QLabel(room.value("game_name").toString(), this);
    layout->addWidget(gameName, 1, 1);

    QLabel *pingLabel = new QLabel("Your ping:", this);
    layout->addWidget(pingLabel, 2, 0);
    pingValue = new QLabel(this);
    layout->addWidget(pingValue, 2, 1);

    QLabel *p1Label = new QLabel("Player 1:", this);
    layout->addWidget(p1Label, 3, 0);

    QLabel *p2Label = new QLabel("Player 2:", this);
    layout->addWidget(p2Label, 4, 0);

    QLabel *p3Label = new QLabel("Player 3:", this);
    layout->addWidget(p3Label, 5, 0);

    QLabel *p4Label = new QLabel("Player 4:", this);
    layout->addWidget(p4Label, 6, 0);

    for (int i = 0; i < 4; ++i)
    {
        pName[i] = new QLabel(this);
        layout->addWidget(pName[i], i+3, 1);
    }

    chatWindow = new QPlainTextEdit(this);
    chatWindow->setReadOnly(1);
    layout->addWidget(chatWindow, 7, 0, 3, 2);

    chatEdit = new QLineEdit(this);
    chatEdit->setPlaceholderText("Enter chat message here");
    connect(chatEdit, &QLineEdit::returnPressed, this, &WaitRoom::sendChat);
    layout->addWidget(chatEdit, 10, 0, 1, 2);

    startGameButton = new QPushButton(this);
    startGameButton->setText("Start Game");
    startGameButton->setAutoDefault(0);
    connect(startGameButton, &QPushButton::released, this, &WaitRoom::startGame);
    layout->addWidget(startGameButton, 11, 0, 1, 2);

    discordLink = new QLabel(this);
    discordLink->setTextFormat(Qt::RichText);
    discordLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    discordLink->setOpenExternalLinks(true);
    discordLink->setAlignment(Qt::AlignCenter);
    layout->addWidget(discordLink, 12, 0, 1, 2);

    setLayout(layout);

    connect(this, SIGNAL (finished(int)), this, SLOT (onFinished(int)));

    QJsonObject json;
    json.insert("type", "request_players");
    json.insert("port", room_port);
    QJsonDocument json_doc = QJsonDocument(json);
    webSocket->sendBinaryMessage(json_doc.toBinaryData());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WaitRoom::sendPing);
    timer->start(5000);
}

void WaitRoom::sendPing()
{
    if (discordLink->text().isEmpty())
    {
        QJsonObject json;
        json.insert("type", "get_discord");
        json.insert("room_name", room_name);
        QJsonDocument json_doc = QJsonDocument(json);
        webSocket->sendBinaryMessage(json_doc.toBinaryData());
    }

    webSocket->ping();
}

void WaitRoom::updatePing(quint64 elapsedTime, const QByteArray&)
{
    pingValue->setText(QString::number(elapsedTime) + " ms");
}

void WaitRoom::startGame()
{
    if (player_name == pName[0]->text())
    {
        QJsonObject json;
        json.insert("type", "start_game");
        json.insert("port", room_port);
        QJsonDocument json_doc = QJsonDocument(json);
        webSocket->sendBinaryMessage(json_doc.toBinaryData());
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Only player 1 can start the game");
        msgBox.exec();
    }
}

void WaitRoom::sendChat()
{
    if (!chatEdit->text().isEmpty())
    {
        QJsonObject json;
        json.insert("type", "chat_message");
        json.insert("port", room_port);
        json.insert("player_name", player_name);
        json.insert("message", chatEdit->text());
        QJsonDocument json_doc = QJsonDocument(json);
        webSocket->sendBinaryMessage(json_doc.toBinaryData());
        chatEdit->clear();
    }
}

void WaitRoom::onFinished(int)
{
    timer->stop();
    webSocket->close();
    webSocket->deleteLater();
}

void WaitRoom::processBinaryMessage(QByteArray message)
{
    QJsonDocument json_doc = QJsonDocument::fromBinaryData(message);
    QJsonObject json = json_doc.object();
    if (json.value("type").toString() == "room_players")
    {
        for (int i = 0; i < 4; ++i)
        {
            if (json.contains(QString::number(i)))
            {
                pName[i]->setText(json.value(QString::number(i)).toString());
                if (pName[i]->text() == player_name)
                    player_number = i + 1;
            }
            else
                pName[i]->clear();
        }
    }
    else if (json.value("type").toString() == "chat_update")
    {
        chatWindow->appendPlainText(json.value("message").toString());
    }
    else if (json.value("type").toString() == "begin_game")
    {
        w->openROM(file_name, webSocket->peerAddress().toString(), room_port, player_number);
        accept();
    }
    else if (json.value("type").toString() == "discord_link")
    {
        QString link = json.value("link").toString();
        discordLink->setText("Discord Voice Chat <a href=\"discord://" + link + "\">App Link</a> - <a href=\"http://" + link + "\">Web Link</a>");
    }
}
