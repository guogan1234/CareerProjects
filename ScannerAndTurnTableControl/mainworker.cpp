#include "mainworker.h"
#include <QDebug>
/*mainworker 的工作内容 */
MainWorker::MainWorker(QObject *parent) : QObject(parent)
{
    initObject();
}

MainWorker::~MainWorker()
{
    udpHandler->closeUdpThread();
    collectThread.quit();
    collectThread.wait();
    writerThread.quit();
    writerThread.wait();
    turnThread.quit();
    turnThread.wait();
}

void MainWorker::connectSlot()
{
    connectNetwork();
    getOneRadarFrame();
}

void MainWorker::mainw_startWork()
{
    connectNetwork();
    turningWork();
}

void MainWorker::mainw_suspendWork()
{
    suspendDevice();
}

void MainWorker::mainw_closeWork()
{
    closeDevice();
}

void MainWorker::turningWork()
{
    openDevice();
    workProcess();
}

void MainWorker::getOneRadarFrameSlot()
{
    udpHandler->GetOneRadarFrame();
}

void MainWorker::slotRefreshMsg(QString msg)
{
//    qDebug("MainWorker::slotRefreshMsg");
    emit signalRefreshMsg(msg);
}

void MainWorker::receAnglesMsg(const TurnTableAngle *angles)
{
    emit sendAnglesMsg(angles);
}

void MainWorker::mainw_sampling_params(RadarBinParams *params)
{
    emit sendSamplingMsg(params);
}

void MainWorker::receive_save_filepath_msg(QString filepath)
{
    emit send_filepath_msg_to_writer(filepath);
}

void MainWorker::initObject()
{
    initParams();
    initThreads();
    initConnects();
}

void MainWorker::initParams()
{
    pcapThread = new PcapCollectorThread("udp and src host 192.168.1.200",4,"AntData",this);
//    pcapThread = new PcapCollectorThread("udp and src host 127.0.0.1",5,"AntData",this);
    udpHandler = new UdpHandleThread(&workerQueue,"AntData");

    xStartAngle = -75.0;
    yStartAngle = -15.0;
    xAdd = 1.0;
    yAdd = 1.0;
}

void MainWorker::initThreads()
{
    collectThread.start(QThread::TimeCriticalPriority);
//    collector.moveToThread(&collectThread);
    writerThread.start();
    binWriter.moveToThread(&writerThread);
//    ewsComThread.start();
//    ewsCom.moveToThread(&ewsComThread);
    turnThread.start();
    turnWorker.moveToThread(&turnThread);

    udpHandler->start();
}

void MainWorker::initConnects()
{
//    connect(this,&MainWorker::startNetwork,&collector,&Collector::startNetwork);
    connect(udpHandler,&UdpHandleThread::oneFrameDataReady,&binWriter,&RadarBinWriter::oneFrameDataSlot);

    connect(this,&MainWorker::openDeviceSignal,&turnWorker,&TurnWorker::openDeviceSlot,Qt::BlockingQueuedConnection);
    connect(this,&MainWorker::suspendDeviceSignal,&turnWorker,&TurnWorker::suspendDeviceSlot,Qt::BlockingQueuedConnection);
    connect(this,&MainWorker::closeDeviceSignal,&turnWorker,&TurnWorker::closeDeviceSlot,Qt::BlockingQueuedConnection);

    connect(this,&MainWorker::workProcessSignal,&turnWorker,&TurnWorker::workProcessSlot);
    connect(&turnWorker,&TurnWorker::writeRadarBinParams,&binWriter,&RadarBinWriter::getWriteParamsSlot,Qt::BlockingQueuedConnection);
    connect(&turnWorker,&TurnWorker::getOneRadarFrameSignal,this,&MainWorker::getOneRadarFrameSlot,Qt::BlockingQueuedConnection);
    connect(&binWriter,&RadarBinWriter::endWriteRadarBin,&turnWorker,&TurnWorker::endWriteRadarBinSlot);

    connect(&turnWorker,&TurnWorker::signalRefreshOpenStatus,this,&MainWorker::slotRefreshMsg);//
    connect(&turnWorker,&TurnWorker::signalRefreshStatus,this,&MainWorker::slotRefreshMsg,Qt::BlockingQueuedConnection);
    connect(&binWriter,&RadarBinWriter::signalRefreshStatus,this,&MainWorker::slotRefreshMsg,Qt::BlockingQueuedConnection);

    connect(&turnWorker,&TurnWorker::tansAnglesMsg,this,&MainWorker::receAnglesMsg,Qt::BlockingQueuedConnection);

    connect(this,&MainWorker::sendSamplingMsg,&turnWorker,&TurnWorker::recvSamplingMsg);

    connect(this,&MainWorker::send_filepath_msg_to_writer,&binWriter,&RadarBinWriter::receive_filepath_msg);
//    connect(&turnWorker,&TurnWorker::signalRefreshStatus,this,&MainWorker::slotRefreshMsg);
//    connect(&binWriter,&RadarBinWriter::signalRefreshStatus,this,&MainWorker::slotRefreshMsg);
}

void MainWorker::connectNetwork()
{
//    emit startNetwork();
    //开启网络连接
    pcapThread->startNetwork();
}

void MainWorker::getOneRadarFrame()
{
    udpHandler->GetOneRadarFrame();
}

void MainWorker::connectNetworkByPcap()
{

}

void MainWorker::openDevice()
{
    emit openDeviceSignal();
}

void MainWorker::suspendDevice()
{
    emit suspendDeviceSignal();
    qDebug()<<"suspendDevice"<<turnThread.isFinished()<<writerThread.isFinished()
           <<collectThread.isFinished();
    collectThread.quit();
    collectThread.wait();
    writerThread.quit();
    writerThread.wait();
    turnThread.quit();
    turnThread.wait();
    udpHandler->closeUdpThread();
}

void MainWorker::closeDevice()
{
    emit closeDeviceSignal();
}

void MainWorker::workProcess()
{
    emit workProcessSignal();
}
