#ifndef DEVINFO_H
#define DEVINFO_H

#include <QString>

struct Process
{
    QString name;
    double cpuRate;
//    QString memUsed;//1799KB
    long long memUsed;
    int bRun;//0 - 未运行，1 - 正在运行
};
typedef struct Process Process;

struct Cpu
{
    QString name;
    double usedRate;
};
typedef struct Cpu Cpu;

struct Memory{
//    QString total;
//    QString free;
//    QString used;
    long long total;
    long long free;
    long long used;
};
typedef struct Memory Memory;

struct Disk
{
    QString name;
//    QString total;
//    QString used;
//    QString free;
    long long total;
    long long used;
    long long free;
    double usedRate;
};
typedef struct Disk Disk;

struct DevInfo
{
    long long seq;
    QList<Process*> * procList;
    QList<Cpu*> * cpuList;
    Memory* memory;
    QList<Disk*> * diskList;
    long long timestamp;
};
typedef struct DevInfo DevInfo;

#endif // DEVINFO_H
