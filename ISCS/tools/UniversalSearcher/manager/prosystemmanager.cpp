#include "prosystemmanager.h"

ProSystemManager::ProSystemManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<ProSystem *> *ProSystemManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

void ProSystemManager::init()
{
    initParam();
}

void ProSystemManager::initParam()
{
    data = new ProSystemData();
}
