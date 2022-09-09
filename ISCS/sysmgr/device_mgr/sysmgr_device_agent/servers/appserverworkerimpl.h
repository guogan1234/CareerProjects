#ifndef APPSERVERWORKERIMPL_H
#define APPSERVERWORKERIMPL_H

#include <QObject>
#include <QReadWriteLock>
#include "appserverworker.h"
#include "collectserverworkerimpl.h"
#include "deviceInfo/devinfomanager.h"
#include "communicateserverworkerimpl.h"
#include "commandManager/commanddatamanager.h"
#include "conf/confmanager.h"

class AppServerWorkerImpl : public AppServerWorker
{
        Q_OBJECT
    public:
        AppServerWorkerImpl();

        void start();

    private:
        void startLoadConfig();
        void startCollectServer();
        void startCommunicateServer();

        void init();
        void initParam();
        ConfManager* confManager;
        DevInfoManager* devInfoManager;
        QReadWriteLock* devInfoLocker;
        CommandDataManager* commandManager;

        CollectServerWorker* collectServer;
        CommunicateServerWorker* communicateServer;
};

#endif // APPSERVERWORKERIMPL_H
