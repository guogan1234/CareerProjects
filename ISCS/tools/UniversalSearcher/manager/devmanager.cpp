#include "devmanager.h"

DevManager::DevManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<Dev *> *DevManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

void DevManager::init()
{
    initParam();
}

void DevManager::initParam()
{
    data = new DevData();
}
