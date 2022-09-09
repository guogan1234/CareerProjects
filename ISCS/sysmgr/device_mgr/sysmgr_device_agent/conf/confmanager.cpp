#include "confmanager.h"

ConfManager::ConfManager(QObject *parent) : QObject(parent)
{
    init();
}

ConfManager::~ConfManager()
{
    delete data;
}

int ConfManager::initConf()
{
    return data->initConf();
}

QList<KeyValue *> *ConfManager::loadConf()
{
    return data->loadConf();
}

int ConfManager::saveConf(QList<KeyValue *> *keyValues)
{
    return data->saveConf(keyValues);
}

const char *ConfManager::getValueByKey(const char *key)
{
    return data->getValueByKey(key);
}

int ConfManager::getCollectPeriodValue(const char *key)
{
    const char* value = NULL;
    value = data->getValueByKey(key);
    if(!value) return -1;
    int collectPeriod;
    sscanf(value,"%d",&collectPeriod);
    return collectPeriod;
}

int ConfManager::getCollectorListenPort(const char *key)
{
    const char* value = NULL;
    value = data->getValueByKey(key);
    if(!value) return -1;
    int collectorPort;
    sscanf(value,"%d",&collectorPort);
    return collectorPort;
}

void ConfManager::init()
{
    initParam();
}

void ConfManager::initParam()
{
    data = new ConfData();
}
