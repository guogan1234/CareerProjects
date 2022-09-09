#include "communicatethread.h"

CommunicateThread::CommunicateThread(QObject *parent) : QThread(parent)
{
    init();
}

void CommunicateThread::setConnServer(QList<ConnClient *> *server)
{
    connServer = server;
}

void CommunicateThread::setRecvDevInfo(DevInfo *info)
{
    recvDataWorker->setDevInfo(info);
}

void CommunicateThread::setFullDevInfo(FullDevInfo *info)
{
    recvDataWorker->setFullDevInfo(info);
}

void CommunicateThread::run()
{
    qDebug("CommunicateThread::run");
    worker = new CommunicateWorkerImpl();
    worker->setConnServer(connServer);
    connect(this,SIGNAL(devInfoRequestSig(QStandardItem*)),worker,SLOT(devInfoRequestSlot(QStandardItem*)));
    connect(worker,SIGNAL(recvDataSignal(QByteArray)),recvDataWorker,SLOT(recvDataSlot(QByteArray)));
    connect(this,SIGNAL(updateCollectPeriod(int)),worker,SLOT(updateCollectPeriodSlot(int)));
    exec();
}

void CommunicateThread::init()
{
    initParam();
}

void CommunicateThread::initParam()
{
    recvDataWorker = new RecvDataWorkerImpl();
    connect(recvDataWorker,SIGNAL(gettedDevInfoSignal()),this,SIGNAL(gettedDevInfoSignal()));
}
