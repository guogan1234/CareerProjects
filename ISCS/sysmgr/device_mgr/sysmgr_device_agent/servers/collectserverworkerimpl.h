#ifndef COLLECTSERVERWORKERIMPL_H
#define COLLECTSERVERWORKERIMPL_H

#include <QObject>
#include <QTimer>
#include "collectserverworker.h"
#include "collectInfo/collectthread.h"

class CollectServerWorkerImpl : public CollectServerWorker
{
        Q_OBJECT
    public:
        explicit CollectServerWorkerImpl(QObject *parent = 0);

        void start();
        void setInfoManager(DevInfoManager* manager);
        void setConfManager(ConfManager *manager);
signals:
        void timeoutSignal(qint64 timestamp);
    public slots:
        void timeoutSlot();
    private:
        QTimer* timer;
        CollectThread* thread;
        ConfManager* confManager;
        void init();
        void initParam();

        void startThread();
        void collectInfo();

        void takeSetInfoManager(DevInfoManager* manager);
};

#endif // COLLECTSERVERWORKERIMPL_H
