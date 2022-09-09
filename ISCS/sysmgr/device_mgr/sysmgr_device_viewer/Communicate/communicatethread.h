#ifndef COMMUNICATETHREAD_H
#define COMMUNICATETHREAD_H

#include <QThread>
#include "Communicate/communicateworkerimpl.h"
#include "recvdataworkerimpl.h"

class CommunicateThread : public QThread
{
        Q_OBJECT
    public:
        explicit CommunicateThread(QObject *parent = 0);

        void setConnServer(QList<ConnClient*> * server);
        void setRecvDevInfo(DevInfo* info);
        void setFullDevInfo(FullDevInfo* info);
        void run();
    signals:
        void devInfoRequestSig(QStandardItem* item);
        void gettedDevInfoSignal();
        void updateCollectPeriod(int msec);
    public slots:

    private:
        QList<ConnClient*> * connServer;
        CommunicateWorkerImpl* worker;
        RecvDataWorkerImpl* recvDataWorker;

        void init();
        void initParam();
};

#endif // COMMUNICATETHREAD_H
