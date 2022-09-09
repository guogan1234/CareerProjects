#include "devinfodataferry.h"

DevInfoDataFerry::DevInfoDataFerry()
{
    init();
}

void DevInfoDataFerry::initData()
{
    takeInitData();
}

int DevInfoDataFerry::addDevInfo(DevInfo *info)
{
    return takeAddDevInfo(info);
}

DevInfo *DevInfoDataFerry::getDevInfo()
{
    return takeGetDevInfo();
}

void DevInfoDataFerry::init()
{
    initParam();
    initData();
}

void DevInfoDataFerry::initParam()
{

}

void DevInfoDataFerry::takeInitData()
{
    data = DevInfoData::getClass();
    qDebug("data - %p\n",data);
}

int DevInfoDataFerry::takeAddDevInfo(DevInfo *info)
{
    return data->addDevInfo(info);
}

DevInfo *DevInfoDataFerry::takeGetDevInfo()
{
    return data->getDevInfo();
}
