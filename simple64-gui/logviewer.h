#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QVBoxLayout>

class LogViewer : public QDialog
{
    Q_OBJECT
public:
    explicit LogViewer(QWidget *parent = 0);
    void clearLog();
public slots:
    void addLog(QString text);

private:
    QPlainTextEdit *textArea = nullptr;
};

#endif // LOGVIEWER_H
