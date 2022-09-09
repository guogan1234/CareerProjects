#include "configmanager.h"

ConfigManager::ConfigManager(QObject *parent):QObject(parent)
{
    init();
}

ConfigManager::~ConfigManager()
{
    delete data;
}

int ConfigManager::initConf()
{
    return data->initConf();
}

QList<KeyValue *> *ConfigManager::loadConf()
{
    return data->loadConf();
}

int ConfigManager::saveConf(QList<KeyValue *> *keyValues)
{
    return data->saveConf(keyValues);
}

const char *ConfigManager::getValueByKey(const char *key)
{
    return data->getValueByKey(key);
}

int ConfigManager::getValueByKeyToInt(const char *key)
{
    const char* value = data->getValueByKey(key);
    if(!value) return -1;
    int temp;
    sscanf(value,"%d",&temp);
    return temp;
}

int ConfigManager::getCollectPeriodValue(const char *key)
{
    const char* value = NULL;
    value = data->getValueByKey(key);
    if(!value) return -1;
    int collectPeriod;
    sscanf(value,"%d",&collectPeriod);
    return collectPeriod;
}

int ConfigManager::getConnectPort(const char *key)
{
    const char* value = NULL;
    value = data->getValueByKey(key);
    if(!value) return -1;
    int connectPort;
    sscanf(value,"%d",&connectPort);
    return connectPort;
}

int ConfigManager::getServicePort(const char *key)
{
    const char* value = NULL;
    value = data->getValueByKey(key);
    if(!value) return -1;
    int servicePort;
    sscanf(value,"%d",&servicePort);
    return servicePort;
}

int ConfigManager::getSustainTime(const char *key)
{
    const char* value = NULL;
    value = data->getValueByKey(key);
    if(!value) return -1;
    int sustainTime;
    sscanf(value,"%d",&sustainTime);
    return sustainTime;
}

int ConfigManager::getCpuLimit(const char *key)
{
    return getValueByKeyToInt(key);
}

int ConfigManager::getMemLimit(const char *key)
{
    return getValueByKeyToInt(key);
}

int ConfigManager::getDiskLimit(const char *key)
{
    return getValueByKeyToInt(key);
}

AlarmConfig *ConfigManager::getAlarmConfig()
{
    AlarmConfig* ret = new AlarmConfig();
    ret->collectPeriod = getCollectPeriodValue("collect_period");
    ret->sustainTime = getSustainTime("sustain_time");
    ret->cpuLimit = getCpuLimit("cpu_limit");
    ret->memLimit = getMemLimit("mem_limit");
    ret->diskLimit = getDiskLimit("disk_limit");
    return ret;
}

void ConfigManager::init()
{
    initParam();
}

void ConfigManager::initParam()
{
    data = new ConfigData();
}
