#include "devinfomanager.h"

DevInfoManager::DevInfoManager(QObject *parent) : QObject(parent)
{
    init();
}

int DevInfoManager::addDevInfo(DevInfo *info)
{
    return takeAddDevInfo(info);
}

DevInfo *DevInfoManager::getDevInfo()
{
    return takeGetDevInfo();
}

void DevInfoManager::init()
{
    initParam();
}

void DevInfoManager::initParam()
{
    ferry = new DevInfoDataFerry();
}

int DevInfoManager::takeAddDevInfo(DevInfo *info)
{
    if(!ferry) return -1;
    return ferry->addDevInfo(info);
}

DevInfo *DevInfoManager::takeGetDevInfo()
{
    if(!ferry) return NULL;
    return ferry->getDevInfo();
}
