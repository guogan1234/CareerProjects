#include "devinfodata.h"

DevInfoData* DevInfoData::data = NULL;

int DevInfoData::addDevInfo(DevInfo *info)
{
    return takeAddDevInfo(info);
}

DevInfo *DevInfoData::getDevInfo()
{
    return takeGetDevInfo();
}

int DevInfoData::checkAndEmptyList()
{
    if(dataList->size() >= 1000){
        qDebug("start empty max list!\n");
        emptyDevInfoList();
        qDebug("end empty max list!\n");
    }
    return 0;
}

DevInfoData::DevInfoData()
{
    init();
}

void DevInfoData::init()
{
    initParam();
}

void DevInfoData::initParam()
{
    dataList = new QList<DevInfo*>();
    locker = new QReadWriteLock();
}

int DevInfoData::takeAddDevInfo(DevInfo *info)
{
    if(!dataList || !locker) return -1;   
    locker->lockForWrite();
    //empty max cache
    checkAndEmptyList();
    //add
    dataList->append(info);
    qDebug("add:devInfoList len - %d",dataList->size());
    locker->unlock();
    return 0;
}

DevInfo *DevInfoData::takeGetDevInfo()
{
    if(!dataList || !locker) return NULL;
    locker->lockForWrite();
    qDebug("get:devInfoList len - %d",dataList->size());
    DevInfo* info = NULL;
    if(!dataList->isEmpty()){
        info = dataList->takeLast();
    }
    locker->unlock();
    return info;
}

void DevInfoData::emptyDevInfoList()
{
    foreach (DevInfo* temp, *dataList) {
        deleteDevInfo(temp);
        delete temp;
    }
    dataList->clear();
}

void DevInfoData::deleteDevInfo(DevInfo *info)
{
    //delete process
    foreach (Process* temp, *info->procList) {
        delete temp;
    }
    //delete cpu
    foreach (Cpu* temp, *info->cpuList) {
        delete temp;
    }
    //delete disk
    foreach (Disk* temp, *info->diskList) {
        delete temp;
    }
    //delete memory
    delete info->memory;
}
