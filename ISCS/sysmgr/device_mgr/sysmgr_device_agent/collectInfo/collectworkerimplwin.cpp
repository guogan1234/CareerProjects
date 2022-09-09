#include "collectworkerimplwin.h"

#ifdef Q_OS_WIN
#include <tlhelp32.h>
#include <psapi.h>
#include "cpuusage.h"
#endif

#include <QDebug>

CollectWorkerImplWin::CollectWorkerImplWin(QObject *parent) : CollectWorker(parent)
{

}

#ifdef Q_OS_WIN
__int64 CompareFileTime ( FILETIME time1, FILETIME time2 )
{
    __int64 a = (time1.dwHighDateTime << 32) | time1.dwLowDateTime ;
    __int64 b = (time2.dwHighDateTime << 32) | time2.dwLowDateTime ;
    return (b - a);
}

//将单字节char*转化为宽字节wchar_t*
wchar_t* AnsiToUnicode( const char* szStr )
{
    int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
    if (nLen == 0)
    {
        return NULL;
    }
    wchar_t* pResult = new wchar_t[nLen];
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
    return pResult;
}

//将宽字节wchar_t*转化为单字节char*
inline char* UnicodeToAnsi( const wchar_t* szStr )
{
    int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
    if (nLen == 0)
    {
        return NULL;
    }
    char* pResult = new char[nLen];
    WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
    return pResult;
}

DevInfo *CollectWorkerImplWin::collectInfo()
{
    DevInfo* devInfo = new DevInfo();
    devInfo->procList = collectProcesses();
    devInfo->cpuList = collectCpu();
    devInfo->memory = collectMemory();
    devInfo->diskList = collectDisk();
    return devInfo;
}

void CollectWorkerImplWin::setInfoManager(DevInfoManager *manager)
{
    takeSetInfoManager(manager);
}

void CollectWorkerImplWin::timeoutSlot(qint64 timestamp)
{
    DevInfo* info = collectInfo();
    if(info){
        info->timestamp = timestamp;
    }
    if(infoManager){
        infoManager->addDevInfo(info);
    }
}

void CollectWorkerImplWin::takeSetInfoManager(DevInfoManager *manager)
{
    infoManager = manager;
}

QList<Process *> *CollectWorkerImplWin::collectProcesses()
{
    HANDLE hProcessSnap;    //进程快照的句柄
//    HANDLE hProcess;    //用于获取进程的句柄
    PROCESSENTRY32 pe32;//进程信息的结构体
    // 获取系统进程信息的快照
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
    {
        qDebug()<<"CreateToolhelp32Snapshot Failed!";// 打印错误信息
        if(NULL !=hProcessSnap)
        {
            CloseHandle( hProcessSnap );          // 关闭进程快照信息
            hProcessSnap = NULL;
        }
        return NULL;
    }
    // 在使用之前设置PROCESSENTRY32结构体的初始大小值,如果不初始化dwSize, Process32First 调用会失败.
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    if( !Process32First( hProcessSnap, &pe32 ) )// 开始获取第一个进程的信息，如果获取失败就返回
    {
        qDebug()<<"Process32First Failed!"; // 打印错误信息
        if(NULL !=hProcessSnap)
        {
            CloseHandle( hProcessSnap );          // 关闭进程快照信息
            hProcessSnap = NULL;
        }
        return NULL;
    }
    //开始遍历所有进程
    QList<Process*> * processList = new QList<Process*>();
    do
    {
        char* szFile = NULL;
        szFile = UnicodeToAnsi(pe32.szExeFile);
        int p_id = pe32.th32ProcessID;
        long long memory = 0;
        getSysProcessMemory(p_id,memory);
//        GetProcessTimes();

        Process* proc = new Process();
        proc->name = QString(szFile);
        proc->memUsed = memory;
        proc->cpuRate = 0;
        proc->bRun = 1;

        processList->append(proc);
        //当然还可以获取到很多其他信息，例如进程名字(szExeFile[MAX_PATH])、父进程PPID(th32ParentProcessID)。。。
    }
    while( Process32Next( hProcessSnap, &pe32 ) );// 获取下一个进程的信息
    if(NULL !=hProcessSnap)//最后关闭快照句柄
    {
        CloseHandle( hProcessSnap );
        hProcessSnap = NULL;
    }
    return processList;
}

int CollectWorkerImplWin::getSysProcessMemory(int nPid, long long &mem)
{
    HANDLE hProcess;//该线程的句柄
    PROCESS_MEMORY_COUNTERS pmc;//该线程的内存信息结构体
    hProcess = OpenProcess( PROCESS_ALL_ACCESS ,FALSE, nPid );//利用最大权限打开该线程并获得句柄
    if (NULL == hProcess)
    {
//        qDebug()<<"OpenProcess Failed!";
        return -1;
    }
    if ( !GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
        qDebug()<<"GetProcessMemoryInfo Failed!";
        return -1;
    }
    mem = (long long)pmc.WorkingSetSize;
//    qDebug()<<"OpenProcess Success! - "<<mem;

    CloseHandle(hProcess);
    return 0;
}

QList<Cpu *> *CollectWorkerImplWin::collectCpu()
{
    QList<Cpu*> * cpuList = new QList<Cpu*>();
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
    GetSystemTimes( &idleTime, &kernelTime, &userTime );

    int idle = CompareFileTime( prev_idle_t,idleTime);
    int kernel = CompareFileTime( prev_kernel_t, kernelTime);
    int user = CompareFileTime(prev_user_t, userTime);

    int cpu = (kernel +user - idle) *100/(kernel+user);
    Cpu* temp = new Cpu();
    if(cpu < 0) cpu = 0;
    temp->usedRate = (double)cpu;
    temp->name = QString("cpu");
    cpuList->append(temp);

    prev_idle_t = idleTime;
    prev_kernel_t = kernelTime;
    prev_user_t = userTime ;

    //获取windows下每个cpu使用率
    collectEachCpu(cpuList);
    return cpuList;
}

Memory *CollectWorkerImplWin::collectMemory()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);

    Memory* memory = new Memory();
    memory->total = (long long)statex.ullTotalPhys;
    memory->free = (long long)statex.ullAvailPhys;
    memory->used = memory->total - memory->free;
    return memory;
}

QList<Disk *> *CollectWorkerImplWin::collectDisk()
{
    DWORD dwSize = MAX_PATH;
    WCHAR szLogicalDrives[MAX_PATH] = {0};
    //获取逻辑驱动器号字符串
    DWORD dwResult = GetLogicalDriveStrings(dwSize,szLogicalDrives);
    //处理获取到的结果
    if (dwResult > 0 && dwResult <= MAX_PATH) {
        WCHAR* szSingleDrive = szLogicalDrives;//从缓冲区起始地址开始
        QList<Disk*> * diskList = new QList<Disk*>();
        while(*szSingleDrive) {
//            qDebug("drive:%s",szSingleDrive);
            char* chName = UnicodeToAnsi(szSingleDrive);
            Disk* disk = new Disk();
            //获取磁盘容量
            unsigned long long available,total,free;
            if(GetDiskFreeSpaceEx(szSingleDrive,(ULARGE_INTEGER*)&available,(ULARGE_INTEGER*)&total,(ULARGE_INTEGER*)&free)){
//                qDebug()<<"res--"<<*szSingleDrive<<" "<<available<<" "<<total<<" "<<free;
//                printf("Drives %s | total = %lld MB,available = %lld MB,free = %lld MB\n",
//                       szSingleDrive,total>>20,available>>20,free>>20);

                disk->name = QString(chName);
                disk->total = total;
                disk->free = free;
                disk->used = total - free;
                disk->usedRate = (disk->used * 100.0)/total;
                diskList->append(disk);
            }else{
//                qDebug("获取容量信息失败");
                disk->name = QString(chName);
                disk->total = 0;
                disk->free = 0;
                disk->used = 0;
                disk->usedRate = 0;
                diskList->append(disk);
            }
            szSingleDrive += strlen(chName) + 1;
//            szSingleDrive += strlen(szSingleDrive) + 1;
        }
        return diskList;
    }else {
        return NULL;
    }
}

int CollectWorkerImplWin::collectEachCpu(QList<Cpu *> *partList)
{
    if(!partList) return -1;
    CpuUsage cpu;
    cpu.Init();
    int count = cpu.GetNumberProcessors();

    cpu.CalProcessorsUses();
    for(int i = 0;i < count;i++){
        int used = cpu.GetUseage(i);

        Cpu* temp = new Cpu();
        temp->name = QString("cpu%1").arg(i);
        temp->usedRate = qAbs(used);//有时会取到负值，暂取

        partList->append(temp);
    }
    return 0;
}
#endif
