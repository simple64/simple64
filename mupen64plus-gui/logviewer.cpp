#include "logviewer.h"

LogViewer::LogViewer(QWidget *parent)
    : QDialog(parent)
{
    this->resize(640,480);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    textArea = new QPlainTextEdit(this);
    textArea->setReadOnly(1);
    mainLayout->addWidget(textArea);
    setLayout(mainLayout);
}

void LogViewer::addLog(QString text)
{
    if (textArea)
        textArea->appendPlainText(text);
}

void LogViewer::clearLog()
{
    textArea->clear();
}
