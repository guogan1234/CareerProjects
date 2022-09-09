#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <QMutex>
#include <QList>
#include "Data/fulldevinfo.h"

class CacheManager
{
public:
    static CacheManager* getInstance();
protected:
    CacheManager();

private:
    static CacheManager* manager;

    void init();
    void initParam();

    QList<FullDevInfo*> * dataList;
};

#endif // CACHEMANAGER_H
