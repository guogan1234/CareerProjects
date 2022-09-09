#include "devtypemanager.h"

DevTypeManager::DevTypeManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<DevType *> *DevTypeManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

void DevTypeManager::init()
{
    initParam();
}

void DevTypeManager::initParam()
{
    data = new DevTypeData();
}
