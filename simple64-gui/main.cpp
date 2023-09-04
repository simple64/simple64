#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

MainWindow *w = nullptr;
int main(int argc, char *argv[])
{
    srand (time(NULL));

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/simple64.svg"));

    QCoreApplication::setApplicationName("simple64-gui");

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption verboseOption({"v", "verbose"}, "Verbose mode. Prints out more information to log.");
    QCommandLineOption noGUIOption("nogui", "Disable GUI elements.");
    QCommandLineOption testOption("test", "Run debug tests.", "number_of_frames");
    parser.addOption(verboseOption);
    parser.addOption(noGUIOption);
    parser.addOption(testOption);
    parser.addPositionalArgument("ROM", QCoreApplication::translate("main", "ROM to open."));
    parser.process(a);
    const QStringList args = parser.positionalArguments();

    MainWindow mainWin;
    w = &mainWin;
    w->show();
    if (parser.isSet(verboseOption))
        w->setVerbose();
    if (parser.isSet(noGUIOption))
        w->setNoGUI();
    if (parser.isSet(testOption))
        w->setTest(parser.value(testOption).toInt());
    if (args.size() > 0)
        w->openROM(args.at(0), "", 0, 0, QJsonObject());
    else
        w->updateApp();

    return a.exec();
}
