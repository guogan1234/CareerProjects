#include "netinfomanager.h"
#include <QDebug>

NetInfoManager::NetInfoManager(QObject *parent) : QObject(parent)
{
    init();
}

void NetInfoManager::setNetInfoMap(QMap<QString, NetInfo *> *map, QReadWriteLock *locker)
{
    takeSetNetInfoMap(map,locker);
}

int NetInfoManager::addNetInfo(qint64 timestamp, NetInfo *data)
{
    return takeAddNetInfo(timestamp,data);
}

void NetInfoManager::init()
{
    initParam();
}

void NetInfoManager::initParam()
{

}

void NetInfoManager::takeSetNetInfoMap(QMap<QString, NetInfo *> *map, QReadWriteLock *locker)
{
    infoMap = map;
    mapLocker = locker;
}

int NetInfoManager::takeAddNetInfo(qint64 timestamp, NetInfo *data)
{
    if(!data || !infoMap) return -1;

    mapLocker->lockForWrite();
//    qDebug()<<"add - "<<data->bConn<<timestamp;
    infoMap->insertMulti(data->name,data);
    mapLocker->unlock();
    return 0;
}
