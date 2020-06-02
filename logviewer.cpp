#include "logviewer.h"

LogViewer::LogViewer(QWidget *parent)
    : QDialog(parent)
{
    file.open();

    this->resize(640,480);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    textArea = new QPlainTextEdit(this);
    textArea->setReadOnly(1);
    mainLayout->addWidget(textArea);
    setLayout(mainLayout);
}

LogViewer::~LogViewer()
{
    file.close();
}

void LogViewer::showEvent(QShowEvent *event)
{
    file.flush();

    qint64 pos = file.pos();
    file.seek(0);
    QTextStream in(&file);
    QString data;
    while (!in.atEnd()) {
        data += in.readLine();
        data += "\n";
    }
    file.seek(pos);
    textArea->setPlainText(data);
    QWidget::showEvent( event );
}

void LogViewer::addLog(QString text)
{
    QTextStream out(&file);
    out << text;
}

void LogViewer::clearLog()
{
    file.seek(0);
    file.resize(0);
    file.flush();
}
