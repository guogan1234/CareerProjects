#include "cachemanager.h"

CacheManager* CacheManager::manager = new CacheManager();

CacheManager *CacheManager::getInstance()
{
    return manager;
}

CacheManager::CacheManager()
{
    init();
}

void CacheManager::init()
{
    initParam();
}

void CacheManager::initParam()
{
    dataList = new QList<FullDevInfo*>();
}
