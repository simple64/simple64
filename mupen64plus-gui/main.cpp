#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

MainWindow *w = nullptr;
int main(int argc, char *argv[])
{
    srand (time(NULL));

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/mupen64plus.svg"));

    QCoreApplication::setApplicationName("mupen64plus-gui");

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption verboseOption({"v", "verbose"}, "Verbose mode. Prints out more information to log.");
    QCommandLineOption noGUIOption("nogui", "Disable GUI elements.");
    QCommandLineOption GLESOption("gles", "Request an OpenGL ES Context.");
    parser.addOption(verboseOption);
    parser.addOption(noGUIOption);
    parser.addOption(GLESOption);
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
    if (parser.isSet(GLESOption))
        w->setGLES();
    if (args.size() > 0)
#ifndef SINGLE_THREAD
        w->openROM(args.at(0), "", 0, 0);
#else
        w->singleThreadLaunch(args.at(0), "", 0, 0);
#endif
    else
        w->updateApp();

    return a.exec();
}
