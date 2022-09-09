#include "collectserverworkerimpl.h"
#include <QDateTime>
#include <QDebug>

CollectServerWorkerImpl::CollectServerWorkerImpl(QObject *parent) : CollectServerWorker(parent)
{
    init();
}

void CollectServerWorkerImpl::start()
{
    startThread();
    collectInfo();
}

void CollectServerWorkerImpl::setInfoManager(DevInfoManager *manager)
{
    takeSetInfoManager(manager);
}

void CollectServerWorkerImpl::setConfManager(ConfManager *manager)
{
    confManager = manager;
}

void CollectServerWorkerImpl::timeoutSlot()
{
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    emit timeoutSignal(timestamp);
}

void CollectServerWorkerImpl::init()
{
    initParam();
}

void CollectServerWorkerImpl::initParam()
{
    timer = new QTimer(this);
    thread = new CollectThread(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutSlot()));
    connect(this,SIGNAL(timeoutSignal(qint64)),thread,SIGNAL(timeoutSignal(qint64)));
}

void CollectServerWorkerImpl::startThread()
{
    if(thread){
        thread->start();
    }
}

void CollectServerWorkerImpl::collectInfo()
{
    if(timer){
        int collectPeriod = confManager->getCollectPeriodValue("collect_period");
        qDebug()<<"collectPeriod - "<<collectPeriod;
        timer->start(collectPeriod);
//        timer->start(1000);
    }
}

void CollectServerWorkerImpl::takeSetInfoManager(DevInfoManager *manager)
{
    thread->setInfoManager(manager);
}
