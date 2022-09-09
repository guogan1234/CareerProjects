#ifndef COLLECTWORKERIMPLWIN_H
#define COLLECTWORKERIMPLWIN_H

#include <QObject>
#include "collectworker.h"

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

class CollectWorkerImplWin : public CollectWorker
{
        Q_OBJECT
    public:
        explicit CollectWorkerImplWin(QObject *parent = 0);

#ifdef Q_OS_WIN
        DevInfo* collectInfo();
        void setInfoManager(DevInfoManager* manager);
    signals:

    public slots:
        void timeoutSlot(qint64 timestamp);
    private:
        void takeSetInfoManager(DevInfoManager* manager);


        FILETIME prev_idle_t;
        FILETIME prev_kernel_t;
        FILETIME prev_user_t;

        QList<Process*> * collectProcesses();
        int getSysProcessMemory(int nPid,long long& mem);
        QList<Cpu*> * collectCpu();
        Memory* collectMemory();
        QList<Disk*> * collectDisk();

        int collectEachCpu(QList<Cpu*> * partList);
#endif
};

#endif // COLLECTWORKERIMPLWIN_H
