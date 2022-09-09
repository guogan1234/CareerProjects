#include "dimanager.h"

DiManager::DiManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<Di *> *DiManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

char **DiManager::getRecordHeaderByCFDB()
{
    return data->getRecordHeaderByCFDB();
}

int DiManager::getRecordHeaderSizeByCFDB()
{
    return data->getRecordHeaderSizeByCFDB();
}

void DiManager::init()
{
    initParam();
}

void DiManager::initParam()
{
    data = new DiData();
}
