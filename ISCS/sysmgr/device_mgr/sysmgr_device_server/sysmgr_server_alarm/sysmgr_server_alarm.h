#ifndef SYSMGR_SERVER_ALARM_H
#define SYSMGR_SERVER_ALARM_H

#include "sysmgr_server_alarm_global.h"
#include "Data/alarmconfig.h"
#include "Data/devinfo.h"
#include <QList>

class SYSMGR_SERVER_ALARMSHARED_EXPORT Sysmgr_server_alarm
{

public:
    Sysmgr_server_alarm();

    int alarmHandle(AlarmConfig* config,AlarmExtra* extra,DevInfo* devInfo);

    int checkCaller(int limit,const char* key);
private:
    int checkCpu(int limit,AlarmExtra* extra,QList<Cpu*> * cpuList);
    int checkMem(int limit,AlarmExtra* extra,Memory* memory);
    int checkDisk(int limit,AlarmExtra* extra,QList<Disk*> * diskList);

    int limitCount;
};

#endif // SYSMGR_SERVER_ALARM_H
