#include "accmanager.h"

AccManager::AccManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<Acc *> *AccManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

char **AccManager::getRecordHeaderByCFDB()
{
    return data->getRecordHeaderByCFDB();
}

int AccManager::getRecordHeaderSizeByCFDB()
{
    return data->getRecordHeaderSizeByCFDB();
}

void AccManager::init()
{
    initParam();
}

void AccManager::initParam()
{
    data = new AccData();
}
