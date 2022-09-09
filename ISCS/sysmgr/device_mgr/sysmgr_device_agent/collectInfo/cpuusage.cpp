#include "cpuusage.h"
#include <stdio.h>

#ifdef Q_OS_WIN
#include <tchar.h>
//#include <vector>

#define	SystemBasicInformation	0
#define	SystemPerformanceInformation	2
#define	SystemTimeInformation	3
#define SystemProcessorPerformanceInformation	8

typedef   LONG   (WINAPI   *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);

CpuUsage::CpuUsage()
{

}

CpuUsage::~CpuUsage()
{
    delete m_ProcessInfo;
}

//类初始化
int CpuUsage::Init()
{
    m_nNumberProcessors=GetNumberProcessors();
    if (m_nNumberProcessors==-1)
        return -1;		//获取CPU核心数失败，返回-1

    memset(m_nUses,0,sizeof(m_nUses));
    memset(m_nOidleTime,0,sizeof(m_nOidleTime));
    memset(m_nOkerenelTime,0,sizeof(m_nOidleTime));
    memset(m_nOuserTime,0,sizeof(m_nOidleTime));
    memset(m_nOdpctTime,0,sizeof(m_nOidleTime));

    m_ProcessInfo =new SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION[m_nNumberProcessors];
    return 1;
}

//获得处理器核心数量
int CpuUsage::GetNumberProcessors()
{
    SYSTEM_BASIC_INFORMATION	m_SystemBaseInfo;
    PROCNTQSI	NtQuerySystemInformation;

    LPCWSTR param = L"ntdll";
    NtQuerySystemInformation=(PROCNTQSI)GetProcAddress(GetModuleHandle(param),"NtQuerySystemInformation");
//    NtQuerySystemInformation=(PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"),"NtQuerySystemInformation");  //利用ntdll内置的API获取CPU
    if(!NtQuerySystemInformation){
        printf("NtQuerySystemInformation is a NULL point!\n");
        return -1;
    }
    int status = NtQuerySystemInformation(SystemBasicInformation,&m_SystemBaseInfo,sizeof(m_SystemBaseInfo),NULL);
    if(status != NO_ERROR)
            return -1;
    return m_SystemBaseInfo.bKeNumberProcessors;
}

//获得CPU使用率的值
int CpuUsage::GetUseage(const int nNumber)
{
    return m_nUses[nNumber];
}

//获得CPU使用率
void CpuUsage::CalProcessorsUses()
{
    PROCNTQSI	NtQuerySystemInformation;
    __int64	nIdleTime=0,nKernelTime=0,nUserTime=0,nDpcTime=0,nInterruptTime=0,nTotalTime=0;

    LPCWSTR param = L"ntdll";
    NtQuerySystemInformation=(PROCNTQSI)GetProcAddress(GetModuleHandle(param),"NtQuerySystemInformation");  //利用ntdll内置的API获取CPU
//    NtQuerySystemInformation=(PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"),"NtQuerySystemInformation");  //利用ntdll内置的API获取CPU
//    int status=NtQuerySystemInformation(SystemProcessorPerformanceInformation, m_ProcessInfo, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * m_nNumberProcessors, 0);
    NtQuerySystemInformation(SystemProcessorPerformanceInformation, m_ProcessInfo, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * m_nNumberProcessors, 0);
    for (int i = 0; i < m_nNumberProcessors; i++)
    {
        nIdleTime = m_ProcessInfo[i].i64IdleTime - m_nOidleTime[i];
        nKernelTime = m_ProcessInfo[i].i64KernelTime - m_nOkerenelTime[i];
        nUserTime = m_ProcessInfo[i].i64UserTime - m_nOuserTime[i];
        nDpcTime = m_ProcessInfo[i].i64DpcTime -m_nOdpctTime[i];
        nInterruptTime = m_ProcessInfo[i].i64InterruptTime - m_nOinterruptTime[i];

        nTotalTime = nKernelTime + nUserTime + nDpcTime + nInterruptTime;
        m_nUses[i] =100*(nTotalTime-nIdleTime)/nTotalTime;	//计算CPU使用率

        m_nOidleTime[i] = m_ProcessInfo[i].i64IdleTime;
        m_nOkerenelTime[i] = m_ProcessInfo[i].i64KernelTime;
        m_nOuserTime[i] = m_ProcessInfo[i].i64UserTime;
        m_nOdpctTime[i] = m_ProcessInfo[i].i64DpcTime;
        m_nOinterruptTime[i] = m_ProcessInfo[i].i64InterruptTime;
    }
}
#endif
