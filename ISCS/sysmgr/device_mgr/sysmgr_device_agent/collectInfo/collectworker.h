#ifndef COLLECTWORKER_H
#define COLLECTWORKER_H

#include <QObject>
#include "deviceInfo/devinfomanager.h"

class CollectWorker : public QObject
{
        Q_OBJECT
    public:
        explicit CollectWorker(QObject *parent = 0);

        virtual DevInfo* collectInfo() = 0;
        virtual void setInfoManager(DevInfoManager* manager) = 0;
    signals:

    public slots:

    protected:
        DevInfoManager* infoManager;
    private:
        virtual QList<Process*> * collectProcesses() = 0;
        virtual QList<Cpu*> * collectCpu() = 0;
        virtual Memory* collectMemory() = 0;
        virtual QList<Disk*> * collectDisk() = 0;
};

#endif // COLLECTWORKER_H
