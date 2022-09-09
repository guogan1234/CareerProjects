#ifndef DEVINFODATA_H
#define DEVINFODATA_H

#include <stdio.h>
#include <QReadWriteLock>
#include "data/devinfo.h"

class DevInfoData
{
    public:
        static DevInfoData* getClass(){
            if(NULL == data){
                data = new DevInfoData();
            }
            return data;
        }

        int addDevInfo(DevInfo* info);
        DevInfo* getDevInfo();
        int checkAndEmptyList();
    private:
        DevInfoData();

        static DevInfoData* data;

        void init();
        void initParam();
        QList<DevInfo*> * dataList;
        QReadWriteLock* locker;

        int takeAddDevInfo(DevInfo *info);
        DevInfo* takeGetDevInfo();
        void emptyDevInfoList();

        void deleteDevInfo(DevInfo* info);
};

#endif // DEVINFODATA_H
