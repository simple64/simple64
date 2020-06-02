#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QTemporaryFile>
#include <QTextStream>

class LogViewer : public QDialog
{
    Q_OBJECT
public:
    explicit LogViewer(QWidget *parent = 0);
    ~LogViewer();
    void addLog(QString text);
    void clearLog();
protected:
    void showEvent(QShowEvent *event);
private:
    QTemporaryFile file;
    QPlainTextEdit *textArea = nullptr;
};

#endif // LOGVIEWER_H
