#ifndef COLLECTTHREAD_H
#define COLLECTTHREAD_H

#include <QObject>
#include <QThread>
#include "deviceInfo/devinfomanager.h"

#ifdef Q_OS_WIN
#include "collectworkerimplwin.h"
#elif defined(Q_OS_UNIX)
#include "collectworkerimplunix.h"
#endif

class CollectThread : public QThread
{
        Q_OBJECT
    public:
        explicit CollectThread(QObject *parent = 0);

        void run();
        void setInfoManager(DevInfoManager* manager);
    signals:
        void timeoutSignal(qint64);
    public slots:

    private:
        DevInfoManager* infoManager;
        void takeSetInfoManager(DevInfoManager* manager);

#ifdef Q_OS_WIN
        CollectWorkerImplWin* collectWorker;
#elif defined(Q_OS_UNIX)
        CollectWorkerImplUnix* collectWorker;
#endif
};

#endif // COLLECTTHREAD_H
