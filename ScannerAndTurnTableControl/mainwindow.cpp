#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "udphandledefines.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initObject();
    params = new RadarBinParams();
}

MainWindow::~MainWindow()
{
    delete params;
    mainThread.quit();
    mainThread.wait();
    delete ui;
}


void MainWindow::on_pushButton_9_clicked()
{
    emit startWork();
    ui->pushButton_9->setEnabled(false);
}

void MainWindow::slotRefreshStatus(QString msg)
{
    qDebug("MainWindow::slotRefreshStatus");
    ui->label_status->setText(msg);
}

void MainWindow::slotRefreshAngles(const TurnTableAngle *angles)
{
    qDebug()<<"refesh angles";

    ui->lineEdit_azimuth->setText(QString::number(angles->azimuth));
    ui->lineEdit_7->setText(QString::number(angles->elevation));
}

void MainWindow::initObject()
{
    initThreads();
    initConnects();
}

void MainWindow::initThreads()
{
    mainThread.start();
    mainWorker.moveToThread(&mainThread);
}

void MainWindow::initConnects()
{
    connect(this,&MainWindow::startWork,&mainWorker,&MainWorker::mainw_startWork);
    connect(this,&MainWindow::suspendWork,&mainWorker,&MainWorker::mainw_suspendWork);
    connect(this,&MainWindow::closeWork,&mainWorker,&MainWorker::mainw_closeWork);
    connect(this,&MainWindow::send_sampling_params_toMainWorker,&mainWorker,&MainWorker::mainw_sampling_params);
    connect(this,&MainWindow::send_filepath_msg_toMainWorker,&mainWorker,&MainWorker::receive_save_filepath_msg);
    connect(&mainWorker,&MainWorker::signalRefreshMsg,this,&MainWindow::slotRefreshStatus,Qt::BlockingQueuedConnection);
    connect(&mainWorker,&MainWorker::sendAnglesMsg,this,&MainWindow::slotRefreshAngles,Qt::BlockingQueuedConnection);
}

void MainWindow::initFolderSys()
{
    QDir qDir_t;
    QString qApp_path=QCoreApplication::applicationDirPath();
    QString qString_dir_t;
    qString_dir_t = qApp_path + "/Data";
    if (!qDir_t.exists(qString_dir_t))
    {
        qDir_t.mkpath(qString_dir_t);
    }
}

void MainWindow::on_pushButton_scan_stop_clicked()
{
    emit suspendWork();
    ui->pushButton_9->setEnabled(true);
    qDebug()<<"MainWindow's suspend is working!!";
}

void MainWindow::on_pushButton_device_open_clicked()
{
    //    emit startWork();
    ui->pushButton_device_open->setEnabled(false);
}

void MainWindow::on_pushButton_device_close_clicked()
{
    emit closeWork();
    ui->pushButton_device_open->setEnabled(true);
}

void MainWindow::on_pushButton_ok_clicked()
{
    params->samples = ui->lineEdit_sample_num->text().toInt();
    params->chirpNum = ui->lineEdit_chirp_num->text().toInt();
    params->speed = ui->lineEdit_speed->text().toInt();
    qDebug()<<"params"<<params->samples<<params->chirpNum;
    emit send_sampling_params_toMainWorker(params);
}

void MainWindow::on_pushButton_4_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("Select Content"),
                                                        "./Data",
                                                        QFileDialog::ShowDirsOnly);
    emit send_filepath_msg_toMainWorker(dirpath);
    ui->lineEdit_6->setText(dirpath);
    qDebug()<<"file_path"<<dirpath;
}
