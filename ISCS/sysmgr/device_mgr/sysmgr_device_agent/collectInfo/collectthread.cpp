#include "collectthread.h"

CollectThread::CollectThread(QObject *parent) : QThread(parent)
{

}

void CollectThread::run()
{
#ifdef Q_OS_WIN
    collectWorker = new CollectWorkerImplWin();
#elif defined(Q_OS_UNIX)
    collectWorker = new CollectWorkerImplUnix();
#endif
    collectWorker->setInfoManager(infoManager);
    connect(this,SIGNAL(timeoutSignal(qint64)),collectWorker,SLOT(timeoutSlot(qint64)));
    exec();
}

void CollectThread::setInfoManager(DevInfoManager *manager)
{
    takeSetInfoManager(manager);
}

void CollectThread::takeSetInfoManager(DevInfoManager *manager)
{
    infoManager = manager;
}
