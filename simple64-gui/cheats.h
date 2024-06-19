#ifndef CHEATS_H
#define CHEATS_H

#include <QDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QJsonObject>
#include <QButtonGroup>
#include <QPlainTextEdit>

class CheatsTextEdit : public QPlainTextEdit
{
public:
    explicit CheatsTextEdit(QString _game, QWidget *parent = 0);

private:
    QString m_game = "";
};

class CheatsCheckBox : public QCheckBox
{
public:
    explicit CheatsCheckBox(QString _game, QString _cheat, QWidget *parent = 0);
    void loadState();
    void setGroup(QButtonGroup *group)
    {
        m_group = group;
    }
    void setOptionName(QString name)
    {
        m_optionName = name;
    }

private:
    QButtonGroup *m_group = nullptr;
    QString m_cheatName = "";
    QString m_optionName = "";
    QString m_game = "";
};

class CheatsDialog : public QDialog
{
    Q_OBJECT
public:
    CheatsDialog(QString gameName, QWidget *parent = nullptr);

private:
    QGridLayout *m_layout;
};

QJsonObject getCheatsFromSettings(QString gameName, QJsonObject gameData);
bool loadCheats(QJsonObject cheatsData);
QString getCheatGameName();
QJsonObject loadCheatData(QString gameName);
#endif
