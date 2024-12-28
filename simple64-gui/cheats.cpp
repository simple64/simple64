#include "cheats.h"
#include "mainwindow.h"
#include "interface/core_commands.h"
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QtEndian>
#include <QScrollArea>
#include <QLabel>
#include <QTextStream>
#include <QJsonArray>
#include <cinttypes>

CheatsDialog::CheatsDialog(QString gameName, QWidget *parent)
    : QDialog(parent)
{
    QJsonObject gameData = loadCheatData(gameName);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *cheatsSettings = new QWidget(this);
    m_layout = new QGridLayout(cheatsSettings);
    QScrollArea *cheatsScroll = new QScrollArea(this);

    QStringList keys = gameData.keys();
    int row = 0;
    for (int i = 0; i < keys.size(); ++i)
    {
        QJsonObject cheats = gameData.value(keys.at(i)).toObject();
        QLabel *name = new QLabel(keys.at(i), this);
        QString helper = cheats.value("note").toString();
        if (!helper.isEmpty())
        {
            helper.prepend("<span style=\"color:black;\">");
            helper.append("</span>");
            name->setToolTip(helper);
        }
        name->setStyleSheet("padding: 10px");
        m_layout->addWidget(name, row, 0);
        bool hasOptions = cheats.value("hasOptions").toBool();
        if (!hasOptions)
        {
            CheatsCheckBox *box = new CheatsCheckBox(gameName, keys.at(i), this);
            box->loadState();
            m_layout->addWidget(box, row++, 1);
        }
        else
        {
            row++;
            QJsonObject options = cheats.value("options").toObject();
            QButtonGroup *optionButtons = new QButtonGroup(this);
            optionButtons->setExclusive(true);
            QStringList optionsKeys = options.keys();
            for (int j = 0; j < optionsKeys.size(); ++j)
            {
                QString optionName = optionsKeys.at(j);
                QLabel *optionLabel = new QLabel(" -- " + optionName, this);
                if (!helper.isEmpty())
                    optionLabel->setToolTip(helper);
                optionLabel->setStyleSheet("padding: 10px");
                m_layout->addWidget(optionLabel, row, 0);
                CheatsCheckBox *box = new CheatsCheckBox(gameName, keys.at(i), this);
                box->setOptionName(optionName);
                box->setGroup(optionButtons);
                box->loadState();
                optionButtons->addButton(box);
                m_layout->addWidget(box, row++, 1);
            }
        }
    }
    cheatsSettings->setLayout(m_layout);
    cheatsScroll->setMinimumWidth(cheatsSettings->sizeHint().width() + 20);
    cheatsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cheatsScroll->setWidget(cheatsSettings);
    QLabel *myLabel = new QLabel("Restart game for cheats to take effect.\n\nHover your mouse over a cheat for a description (if one exists).\n", this);
    myLabel->setStyleSheet("font-weight: bold");
    mainLayout->addWidget(myLabel);
    mainLayout->addWidget(cheatsScroll);

    QLabel *customLabel = new QLabel("Custom Codes:", this);
    CheatsTextEdit *customCodeEdit = new CheatsTextEdit(gameName, this);
    mainLayout->addWidget(customLabel);
    mainLayout->addWidget(customCodeEdit);
    setLayout(mainLayout);
}

CheatsTextEdit::CheatsTextEdit(QString _game, QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_game = _game;
    QString prefix = "Cheats/" + m_game + "/custom/";

    setPlainText(w->getSettings()->value(prefix + "cheat").toString());

    connect(this, &QPlainTextEdit::textChanged, [=]
            { w->getSettings()->setValue(prefix + "cheat", toPlainText()); });
}

CheatsCheckBox::CheatsCheckBox(QString _game, QString _cheat, QWidget *parent)
    : QCheckBox(parent)
{
    m_game = _game;
    m_cheatName = _cheat;

    connect(this, &QAbstractButton::pressed, [=]
            {
        if (m_group != nullptr && checkState() == Qt::Checked)
        {
            m_group->setExclusive(false);
        } });

    connect(this, &QAbstractButton::released, [=]
            {
        if (m_group != nullptr)
        {
            m_group->setExclusive(true);
        } });

    connect(this, &QCheckBox::checkStateChanged, [=](Qt::CheckState state)
            {
        QString prefix = "Cheats/" + m_game + "/" + m_cheatName + "/";
        if (m_optionName != "")
        {
            w->getSettings()->setValue(prefix + "option", m_optionName);
        }
        w->getSettings()->setValue(prefix + "enabled", state == Qt::Checked ? true : false); });
}

void CheatsCheckBox::loadState()
{
    QString prefix = "Cheats/" + m_game + "/" + m_cheatName + "/";

    if (w->getSettings()->value(prefix + "option").toString() == m_optionName)
    {
        if (w->getSettings()->value(prefix + "enabled").toBool())
            setCheckState(Qt::Checked);
    }
}

QJsonObject getCheatsFromSettings(QString gameName, QJsonObject gameData)
{
    QJsonObject data;
    w->getSettings()->beginGroup("Cheats");
    w->getSettings()->beginGroup(gameName);
    QStringList childGroups = w->getSettings()->childGroups();
    for (int i = 0; i < childGroups.size(); ++i)
    {
        w->getSettings()->beginGroup(childGroups.at(i));
        if (childGroups.at(i) == "custom")
        {
            QString customCheatsString = w->getSettings()->value("cheat").toString();
            QTextStream customCheatStream(&customCheatsString);
            QJsonArray custom_codes;
            while (!customCheatStream.atEnd())
            {
                QString line = customCheatStream.readLine();
                if (!line.isEmpty())
                {
                    custom_codes.append(line);
                }
            }
            if (!custom_codes.isEmpty())
            {
                data.insert("custom", custom_codes);
            }
        }
        else if (w->getSettings()->value("enabled").toBool())
        {
            QJsonArray cheat_codes = gameData.value(childGroups.at(i)).toObject().value("data").toArray();
            if (w->getSettings()->contains("option"))
            {
                for (int j = 0; j < cheat_codes.size(); ++j)
                {
                    if (cheat_codes.at(j).toString().contains("?"))
                    {
                        QString replacement = gameData.value(childGroups.at(i)).toObject().value("options").toObject().value(w->getSettings()->value("option").toString()).toString();
                        QString code = cheat_codes.at(j).toString().replace(cheat_codes.at(j).toString().indexOf("?"), replacement.size(), replacement);
                        cheat_codes.replace(j, code);
                    }
                }
            }
            data.insert(childGroups.at(i), cheat_codes);
        }
        w->getSettings()->endGroup();
    }
    w->getSettings()->endGroup();
    w->getSettings()->endGroup();
    return data;
}

bool loadCheats(QJsonObject cheatsData)
{
    bool loaded = false;

    for (int i = 0; i < cheatsData.size(); ++i)
    {
        QJsonArray cheat_codes = cheatsData.value(cheatsData.keys().at(i)).toArray();
        QList<m64p_cheat_code> codes;
        for (int j = 0; j < cheat_codes.size(); ++j)
        {
            QStringList data = cheat_codes.at(j).toString().split(" ");
            m64p_cheat_code code;
            bool ok;
            code.address = data[0].toUInt(&ok, 16);
            code.value = data[1].toInt(&ok, 16);
            codes.append(code);
        }
        m64p_error success = (*CoreAddCheat)(cheatsData.keys().at(i).toUtf8().constData(), codes.data(), codes.size());
        if (success != M64ERR_SUCCESS)
            DebugMessage(M64MSG_WARNING, "could not load cheat %s", cheatsData.keys().at(i).toUtf8().constData());
        else
        {
            loaded = true;
            DebugMessage(M64MSG_INFO, "loaded cheat %s", cheatsData.keys().at(i).toUtf8().constData());
        }
    }

    return loaded;
}

QString getCheatGameName()
{
    m64p_rom_header rom_header;
    m64p_error success = (*CoreDoCommand)(M64CMD_ROM_GET_HEADER, sizeof(rom_header), &rom_header);
    if (success != M64ERR_SUCCESS)
        return "";
    return QString("%1-%2-C:%3").arg(qFromBigEndian(rom_header.CRC1), 8, 16, QLatin1Char('0')).arg(qFromBigEndian(rom_header.CRC2), 8, 16, QLatin1Char('0')).arg(rom_header.Country_code, 2, 16).toUpper();
}

QJsonObject loadCheatData(QString gameName)
{
    QString cheats_path = QDir(QCoreApplication::applicationDirPath()).filePath("cheats.json");
    QFile file(cheats_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonObject data = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    return data.value(gameName).toObject();
}
