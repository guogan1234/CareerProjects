#include "netthreadimpl.h"

NetThreadImpl::NetThreadImpl()
{

}

void NetThreadImpl::run()
{
    netWorker = new NetWorkerImpl();
    netWorker->setConfig(connConfigs);
    netInfoManager = new NetInfoManager();
    netInfoManager->setNetInfoMap(netInfoMap,netMapLocker);
    netWorker->setManager(netInfoManager);
    connect(this,SIGNAL(dataTimeout(qint64)),netWorker,SLOT(dataTimeoutSlot(qint64)));

    exec();
}

void NetThreadImpl::setNetInfoMap(QMap<QString, NetInfo *> *map, QReadWriteLock *locker)
{
    takeSetNetInfoMap(map,locker);
}

void NetThreadImpl::setConnConfigs(QList<ConnClient *> *config)
{
    takeSetConnConfigs(config);
}

void NetThreadImpl::takeSetNetInfoMap(QMap<QString, NetInfo *> *map, QReadWriteLock *locker)
{
    netInfoMap = map;
    netMapLocker = locker;
}

void NetThreadImpl::takeSetConnConfigs(QList<ConnClient *> *config)
{
    connConfigs = config;
}
