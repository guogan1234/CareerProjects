#ifndef CPUUSAGE_H
#define CPUUSAGE_H

#include <QtGlobal>
#ifdef Q_OS_WIN
#include <windows.h>

typedef	struct SYSTEM_BASIC
{
    DWORD	dwUnknown1;
    ULONG	uKeMaximumIncrement;
    ULONG	uPageSize;
    ULONG	uMmNumberOfPhysicalPages;
    ULONG	uMmLowestPhysicalPage;
    ULONG   uMmHighestPhysicalPage;
    ULONG   uAllocationGranularity;
    PVOID   pLowestUserAddress;
    PVOID   pMmHighestUserAddress;
    ULONG   uKeActiveProcessors;
    BYTE	bKeNumberProcessors;
    BYTE    bUnknown2;
    WORD    wUnknown3;
}SYSTEM_BASIC_INFORMATION;

typedef struct SYSTEM_PROCESSOR_PERFORMANCE
{
    __int64 i64IdleTime;
    __int64 i64KernelTime;
    __int64 i64UserTime;
    __int64 i64DpcTime;
    __int64 i64InterruptTime;
    long lInterruptCount;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

class CpuUsage
{
public:
    CpuUsage();
    ~CpuUsage();
    int Init();
    int GetNumberProcessors();		//获得CPU核心数量
    int GetUseage(const int);		//获得CPU使用率的值
    void CalProcessorsUses();		//计算CPU使用率

protected:

private:
    SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION *m_ProcessInfo;

    int m_nUses[64];
    int m_nNumberProcessors;

    __int64 m_nOidleTime[5];	//旧的CPU时间，用于计算CPU使用率
    __int64 m_nOkerenelTime[5];
    __int64 m_nOuserTime[5];
    __int64 m_nOdpctTime[5];
    __int64 m_nOinterruptTime[5];
};
#endif

#endif // CPUUSAGE_H
