#ifndef ALARMCONFIG_H
#define ALARMCONFIG_H

struct AlarmConfig{
    int collectPeriod;
    int sustainTime;
    int cpuLimit;
    int memLimit;
    int diskLimit;
};
typedef struct AlarmConfig AlarmConfig;

struct AlarmExtra{
    int stationId;
    int domainId;
    char* nodeName;
};
typedef struct AlarmExtra AlarmExtra;

#endif // ALARMCONFIG_H
