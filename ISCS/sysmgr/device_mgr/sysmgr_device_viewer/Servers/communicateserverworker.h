#ifndef COMMUNICATESERVERWORKER_H
#define COMMUNICATESERVERWORKER_H

#include <QObject>
#include "Communicate/communicatethread.h"

class CommunicateServerWorker : public QObject
{
        Q_OBJECT
    public:
        explicit CommunicateServerWorker(QObject *parent = 0);

        virtual void start(QList<ConnClient*> * server) = 0;
        virtual void setDevInfo(DevInfo* info) = 0;
    protected:
        CommunicateThread* thread;
    signals:
        void devInfoRequestSig(QStandardItem* item);
    public slots:
    private:
        virtual void startThread(QList<ConnClient*> * server) = 0;
};

#endif // COMMUNICATESERVERWORKER_H
