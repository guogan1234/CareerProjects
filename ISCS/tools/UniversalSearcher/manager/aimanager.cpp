#include "aimanager.h"

AiManager::AiManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<Ai *> *AiManager::loadDataByCFDB(QString sql)
{
    return data->loadDataByCFDB(sql);
}

char **AiManager::getRecordHeaderByCFDB()
{
    return data->getRecordHeaderByCFDB();
}

int AiManager::getRecordHeaderSizeByCFDB()
{
    return data->getRecordHeaderSizeByCFDB();
}

void AiManager::init()
{
    initParam();
}

void AiManager::initParam()
{
    data = new AiData();
}
