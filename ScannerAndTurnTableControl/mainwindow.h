#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "mainworker.h"
#include "turntablemessage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startWork();
    void suspendWork();
    void closeWork();
    void send_sampling_params_toMainWorker(RadarBinParams* params);
    void send_filepath_msg_toMainWorker(QString filepath);
private slots:
    void on_pushButton_9_clicked();

    void slotRefreshStatus(QString msg);

    void slotRefreshAngles(const TurnTableAngle* angles);

    void on_pushButton_scan_stop_clicked();

    void on_pushButton_device_open_clicked();

    void on_pushButton_device_close_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;

    MainWorker mainWorker;
    QThread mainThread;

    void initObject();
    void initThreads();
    void initConnects();
    void initFolderSys();
public:
    RadarBinParams* params;
};
#endif // MAINWINDOW_H
