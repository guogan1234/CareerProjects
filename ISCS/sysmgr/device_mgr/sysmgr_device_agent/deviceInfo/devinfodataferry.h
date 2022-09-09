#ifndef DEVINFODATAFERRY_H
#define DEVINFODATAFERRY_H

#include "devinfodata.h"

class DevInfoDataFerry
{
    public:
        DevInfoDataFerry();

        void initData();      
        int addDevInfo(DevInfo* info);
        DevInfo* getDevInfo();
    private:
        void init();
        void initParam();
        void takeInitData();

        DevInfoData* data;

        int takeAddDevInfo(DevInfo *info);
        DevInfo* takeGetDevInfo();
};

#endif // DEVINFODATAFERRY_H
