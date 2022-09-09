#include "stationmanager.h"

StationManager::StationManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<Station *> *StationManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

void StationManager::init()
{
    initParam();
}

void StationManager::initParam()
{
    data = new StationData();
}
