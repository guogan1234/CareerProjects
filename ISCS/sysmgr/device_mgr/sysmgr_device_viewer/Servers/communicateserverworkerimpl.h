#ifndef COMMUNICATESERVERWORKERIMPL_H
#define COMMUNICATESERVERWORKERIMPL_H

#include "communicateserverworker.h"

class CommunicateServerWorkerImpl : public CommunicateServerWorker
{
        Q_OBJECT
    public:
        explicit CommunicateServerWorkerImpl(QObject *parent = 0);

        void start(QList<ConnClient*> * server);
        void setDevInfo(DevInfo* info);
        void setFullDevInfo(FullDevInfo* info);
    signals:
        void gettedDevInfoSignal();
        void updateCollectPeriod(int msec);
    public slots:
    private:
        void startThread(QList<ConnClient *> *server);

        void init();
        void initParam();
};

#endif // COMMUNICATESERVERWORKERIMPL_H
