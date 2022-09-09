#include "datafinder.h"
#include "Parser/devinfoparser.h"

DataFinder::DataFinder(QObject *parent) : QObject(parent)
{

}

void DataFinder::setRawInfoList(QList<QByteArray> *list, QMutex *mutex)
{
    rawInfoList = list;
    rawListMetux = mutex;
}

void DataFinder::setRawInfoHash(QHash<QString, QByteArray> *hash)
{
    rawInfoHash = hash;
}

void DataFinder::setNetInfoMap(QMap<QString, NetInfo *> *map, QReadWriteLock *lock)
{
    netInfoMap = map;
    netMapLock = lock;
}

FullDevInfo *DataFinder::getMatchedData(DevInfoRequest *request)
{
    QList<QString> * ips = request->ips;
    QByteArray devInfoArray;
    for(int i = 0;i < ips->size(); ++i){
        QString ip = ips->at(i);
        if(rawInfoHash->contains(ip)){
            devInfoArray = rawInfoHash->value(ip);
            break;
        }
    }
    DevInfo* devInfo = NULL;
    devInfo = DevInfoParser::jsonToClass(devInfoArray);
    QString name = request->name;
    NetInfo* netInfo = NULL;
    netInfo = netInfoMap->value(name);
    FullDevInfo* fullDevInfo = new FullDevInfo();
    fullDevInfo->devInfo = devInfo;
    fullDevInfo->netInfo = netInfo;
    return fullDevInfo;
}
