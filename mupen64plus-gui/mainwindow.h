#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "oglwindow.h"
#include "workerthread.h"
#include "logviewer.h"
#include "keypressfilter.h"

#include <QMainWindow>
#include <QSettings>
#include <QSurfaceFormat>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    WorkerThread* getWorkerThread();
    OGLWindow* getOGLWindow();
    QSettings* getSettings();
    LogViewer* getLogViewer();

    void openROM(QString filename, QString netplay_ip, int netplay_port, int netplay_player);
    void resetTitle();
    void setVerbose();
    int getVerbose();
    void setNoGUI();
    int getNoGUI();
    void setGLES();
    int getGLES();
    void updatePlugins();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void resizeMainWindow(int Width, int Height);
    void toggleFS(int force);
    void createOGLWindow(QSurfaceFormat* format);
    void deleteOGLWindow();
    void setTitle(std::string title);
    void pluginWarning(QString name);
    void showMessage(QString message);

private slots:
    void on_actionOpen_ROM_triggered();

    void on_actionPlugin_Paths_triggered();

    void on_actionStop_Game_triggered();

    void on_actionExit_triggered();

    void on_actionPlugin_Settings_triggered();

    void on_actionPause_Game_triggered();

    void on_actionMute_triggered();

    void on_actionHard_Reset_triggered();

    void on_actionSoft_Reset_triggered();

    void on_actionTake_Screenshot_triggered();

    void on_actionSave_State_triggered();

    void on_actionLoad_State_triggered();

    void on_actionToggle_Fullscreen_triggered();

    void on_actionSave_State_To_triggered();

    void on_actionLoad_State_From_triggered();

    void on_actionCheats_triggered();

    void on_actionController_Configuration_triggered();

    void on_actionToggle_Speed_Limiter_triggered();

    void on_actionView_Log_triggered();

    void on_actionVideo_Settings_triggered();

    void on_actionCreate_Room_triggered();

    void on_actionJoin_Room_triggered();

    void on_actionSupport_on_Patreon_triggered();

    void on_actionOpen_Discord_Channel_triggered();

private:
    void stopGame();
    void updateOpenRecent();
    void updateGB(Ui::MainWindow *ui);
    void updateDD(Ui::MainWindow *ui);
    void updatePIF(Ui::MainWindow *ui);
    void findRecursion(const QString &path, const QString &pattern, QStringList *result);
    Ui::MainWindow *ui;
    QMenu * OpenRecent;
    int verbose;
    int nogui;
    int gles;
    QString m_title;

    OGLWindow *my_window = nullptr;
    WorkerThread *workerThread = nullptr;
    LogViewer logViewer;
    QSettings *settings = nullptr;
    KeyPressFilter keyPressFilter;
};

extern MainWindow *w;

#endif // MAINWINDOW_H
