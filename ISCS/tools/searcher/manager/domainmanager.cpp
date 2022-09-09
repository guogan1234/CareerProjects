#include "domainmanager.h"

DomainManager::DomainManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<Domain *> *DomainManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

void DomainManager::init()
{
    initParam();
}

void DomainManager::initParam()
{
    data = new DomainData();
}
