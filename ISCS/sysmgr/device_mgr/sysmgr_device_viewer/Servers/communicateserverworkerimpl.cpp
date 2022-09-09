#include "communicateserverworkerimpl.h"

CommunicateServerWorkerImpl::CommunicateServerWorkerImpl(QObject *parent) : CommunicateServerWorker(parent)
{
    init();
}

void CommunicateServerWorkerImpl::start(QList<ConnClient *> *server)
{
    startThread(server);
}

void CommunicateServerWorkerImpl::setDevInfo(DevInfo *info)
{
    thread->setRecvDevInfo(info);
}

void CommunicateServerWorkerImpl::setFullDevInfo(FullDevInfo *info)
{
    thread->setFullDevInfo(info);
}

void CommunicateServerWorkerImpl::startThread(QList<ConnClient *> *server)
{
    thread->setConnServer(server);
    thread->start();
}

void CommunicateServerWorkerImpl::init()
{
    initParam();
}

void CommunicateServerWorkerImpl::initParam()
{
    thread = new CommunicateThread();
    connect(this,SIGNAL(devInfoRequestSig(QStandardItem*)),thread,SIGNAL(devInfoRequestSig(QStandardItem*)));
    connect(thread,SIGNAL(gettedDevInfoSignal()),this,SIGNAL(gettedDevInfoSignal()));
    connect(this,SIGNAL(updateCollectPeriod(int)),thread,SIGNAL(updateCollectPeriod(int)));
}
