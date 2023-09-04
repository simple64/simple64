#ifndef CHEATS_H
#define CHEATS_H

#include <QDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QJsonObject>
#include <QButtonGroup>

class CheatsCheckBox : public QCheckBox
{
public:
    explicit CheatsCheckBox(QWidget *parent = 0);
    void loadState();
    void setGroup(QButtonGroup* group) {
        m_group = group;
    }
    void setCheatName(QString name) {
        m_cheatName = name;
    }
    void setOptionName(QString name) {
        m_optionName = name;
    }
    void setGame(QString name) {
        m_game = name;
    }
private:
    QButtonGroup* m_group = nullptr;
    QString m_cheatName = "";
    QString m_optionName = "";
    QString m_game = "";
};

class CheatsDialog : public QDialog
{
    Q_OBJECT
public:
    CheatsDialog(QWidget *parent = nullptr);
private:
    QGridLayout *m_layout;
};

QJsonObject getCheatsFromSettings(QString gameName, QJsonObject gameData);
bool loadCheats(QJsonObject cheatsData);
QString getCheatGameName();
QJsonObject loadCheatData(QString gameName);
#endif
