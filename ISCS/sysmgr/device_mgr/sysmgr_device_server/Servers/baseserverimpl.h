#ifndef BASESERVERIMPL_H
#define BASESERVERIMPL_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QMap>
#include <QReadWriteLock>
#include "baseserverworker.h"
#include "Communicate/threadmanager.h"
#include "Communicate/threadmanagerferry.h"
#include "Data/devinfo.h"
#include "Data/netinfo.h"
#include "Service/serviceserver.h"
#include "ConfHandle/configmanager.h"
#include "ConfHandle/confhandleworker.h"

class BaseServerImpl : public BaseServerWorker
{
    Q_OBJECT
public:
    BaseServerImpl(QList<ConnClient*> * baseConfig);

    int start();

    void setConfHandleWorker(ConfHandleWorker* confWorker);
signals:
    void dataTimeout(qint64);
private:
    ThreadManager* tManager;
    ThreadManagerFerry* tManagerFerry;
    QTimer* connTimer;
    QTimer* dataTimer;

    int init();
    int initParam();

    int startThreads();
    int startCommunicate();
    int startServer();
    int startConfigServer();

    QList<ConnClient*> * config;

    int initStartConf(QList<ConnClient*> * baseConfig);

    //data
    QList<QByteArray> * rawDevInfoList;
    QHash<QString,QByteArray> * rawDevInfoHash;
    QMutex* rawListMutex;
    QMap<QString,NetInfo*> * netInfoMap;
    QReadWriteLock* mapLock;

    //config
    ConfigManager* configManager;
    int initLoadConfig();

    //xml config
    ConfHandleWorker* confHandleWorker;

    //
    ServiceServer* serviceServer;
private slots:
    void dataTimeoutSlot();
};

#endif // BASESERVERIMPL_H
