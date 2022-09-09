#include "collectworkerimplunix.h"

#ifdef Q_OS_UNIX
#include <sys/types.h>
#include <sys/wait.h>
#endif

CollectWorkerImplUnix::CollectWorkerImplUnix(QObject *parent) : CollectWorker(parent)
{

}

void CollectWorkerImplUnix::setInfoManager(DevInfoManager *manager)
{
    takeSetInfoManager(manager);
}

DevInfo *CollectWorkerImplUnix::collectInfo()
{
    DevInfo* devInfo = new DevInfo();
    devInfo->procList = collectProcesses();
    devInfo->cpuList = collectCpu();
    devInfo->memory = collectMemory();
    devInfo->diskList = collectDisk();
    return devInfo;
}

void CollectWorkerImplUnix::timeoutSlot(qint64 timestamp)
{
    DevInfo* info = collectInfo();
    if(info){
        info->timestamp = timestamp;
    }
    qDebug("add DevInfo");
    if(infoManager){
        infoManager->addDevInfo(info);
    }
}

QList<Process *> *CollectWorkerImplUnix::collectProcesses()
{
    FILE *file_ptr = NULL;
#ifdef Q_OS_UNIX
    file_ptr = popen("ps aux","r");
#endif
    if(file_ptr)
    {
#ifdef Q_OS_UNIX
        char buf[512];
        bzero(buf,sizeof(buf));
        if(NULL == fgets(buf,512,file_ptr))
            return NULL;
#endif
        QList<Process*> * processList = new QList<Process*>();
#ifdef Q_OS_UNIX
        while(NULL != fgets(buf,512,file_ptr))
        {
            Process* proc = new Process();
//            char temp[8] = {0};
            char temp[32] = {0};
            char pid[16] = {0};
            char cpu[16] = {0};
            char mem[16] = {0};
            char com[256] = {0};
            if(11 == sscanf(buf,"%s %s %s %s %s %s %s %s %s %s %s",
                    temp,pid,cpu,mem,temp,temp,temp,temp,
                    temp,temp,com))
            {
                proc->name = QString(com);
                proc->memUsed = QString(mem).toLongLong();
                proc->cpuRate = QString(cpu).toDouble();
                proc->bRun = 1;
                processList->append(proc);
            }
            bzero(buf,sizeof(buf));
        }
        pclose(file_ptr);
#endif
        return processList;
    }else {
        return NULL;
    }
}

QList<Cpu *> *CollectWorkerImplUnix::collectCpu()
{
    FILE *file_ptr = NULL;
#ifdef Q_OS_UNIX
    file_ptr = fopen("/proc/stat","r");
#endif
    if(file_ptr != NULL)
    {
        QList<Cpu*> * cpuList = new QList<Cpu*>();
#ifdef Q_OS_UNIX
        char buf[512];
        bzero(buf,sizeof(buf));
        while(NULL != fgets(buf,512,file_ptr))
        {
            Cpu* cpu = new Cpu();
            char name[32] = {'\0'};
            char user[32] = {'\0'};
            char nice[32] = {'\0'};
            char system[32] = {'\0'};
            char idle[32] = {'\0'};
            char iowait[32] = {'\0'};
            char irq[32] = {'\0'};
            char softirq[32] = {'\0'};
            if(8 == sscanf(buf,"%s %s %s %s %s %s %s %s",
                           name,user,nice,system,idle,iowait,irq,softirq))
            {
                QString name_qs = QString::fromStdString(name);
                if(!name_qs.contains("cpu"))
                {
                    delete cpu;
                    continue;
                }
                double d_user = QString(user).toDouble();
                double d_nice = QString(nice).toDouble();
                double d_sys = QString(system).toDouble();
                double d_idle = QString(idle).toDouble();
                double d_io = QString(iowait).toDouble();
                double d_irq = QString(irq).toDouble();
                double d_softirq = QString(softirq).toDouble();
                double total = d_user + d_nice + d_sys + d_idle + d_io + d_irq + d_softirq;
                cpu->name = QString(name);
                cpu->usedRate = (total - d_idle)/total*100.0;
                cpuList->append(cpu);
            }
            bzero(buf,sizeof(buf));
        }
        fclose(file_ptr);
#endif
        return cpuList;
    }else {
        return NULL;
    }
}

Memory *CollectWorkerImplUnix::collectMemory()
{
    FILE *file_ptr = NULL;
#ifdef Q_OS_UNIX
    file_ptr = fopen("/proc/meminfo","r");
#endif
    if(file_ptr != NULL)
    {
        Memory* memory = new Memory();
#ifdef Q_OS_UNIX
        char buf[512];
        bzero(buf,sizeof(buf));
        int count = 0;
        while(NULL != fgets(buf,512,file_ptr))
        {
            char name[32] = {'\0'};
            char size[16] = {'\0'};
            if(2 == sscanf(buf,"%s %s",name,size))
            {
                long long value;
                value = QString(size).toLongLong();
                if(0 == count){
                    memory->total = value * 1024;//Bytes
                }
                if(2 == count){
                    memory->free = value * 1024;//Bytes
                    memory->used = memory->total - memory->free;
                    break;
                }
            }
            bzero(buf,sizeof(buf));
            count++;
        }
        fclose(file_ptr);
#endif
        return memory;
    }else {
        return NULL;
    }
}

QList<Disk *> *CollectWorkerImplUnix::collectDisk()
{
    FILE *file_ptr = NULL;
#ifdef Q_OS_UNIX
    file_ptr = popen("df","r");
#endif
    if(file_ptr != NULL)
    {
        QList<Disk*> * diskList = new QList<Disk*>();
#ifdef Q_OS_UNIX
        char buf[512];
        bzero(buf,sizeof(buf));
        if(NULL == fgets(buf,512,file_ptr))
            return NULL;
        while(NULL != fgets(buf,512,file_ptr))
        {
            Disk* disk = new Disk();
            char filesystem[16] = {'\0'};
            char size[32] = {'\0'};
            char used[32] = {'\0'};
            char avail[32] = {'\0'};
            char use[16] = {'\0'};
            char mount[64] = {'\0'};
            if(6 == sscanf(buf,"%s %s %s %s %s %s",filesystem,size,used,avail,use,mount))
            {
                disk->name = QString(mount);
                disk->total = QString(size).toLongLong() * 1024;
                disk->used = QString(used).toLongLong() * 1024;
                disk->free = QString(avail).toLongLong() * 1024;
                disk->usedRate = QString(use).remove("%").toDouble();
                diskList->append(disk);
            }
            bzero(buf,sizeof(buf));
        }
        pclose(file_ptr);
#endif
        return diskList;
    }else {
        return NULL;
    }
}

void CollectWorkerImplUnix::takeSetInfoManager(DevInfoManager *manager)
{
    infoManager = manager;
}
