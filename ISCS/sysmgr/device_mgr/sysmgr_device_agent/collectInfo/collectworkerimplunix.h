#ifndef COLLECTWORKERIMPLUNIX_H
#define COLLECTWORKERIMPLUNIX_H

#include <QObject>
#include "collectworker.h"

class CollectWorkerImplUnix : public CollectWorker
{
    Q_OBJECT
public:
    explicit CollectWorkerImplUnix(QObject *parent = nullptr);

    void setInfoManager(DevInfoManager *manager);
    DevInfo* collectInfo();
signals:

public slots:
    void timeoutSlot(qint64 timestamp);
private:
    QList<Process*> * collectProcesses();
    QList<Cpu*> * collectCpu();
    Memory* collectMemory();
    QList<Disk*> * collectDisk();

    void takeSetInfoManager(DevInfoManager *manager);
};

#endif // COLLECTWORKERIMPLUNIX_H
