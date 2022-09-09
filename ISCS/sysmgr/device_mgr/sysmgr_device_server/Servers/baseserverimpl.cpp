#include "baseserverimpl.h"
#include "Communicate/workthread.h"
#include "Communicate/datahandlethread.h"
#include "Defines/baseserverdefine.h"
#include "NetHandle/netthreadimpl.h"
#include "configserverimpl.h"
#include <QDateTime>

BaseServerImpl::BaseServerImpl(QList<ConnClient *> *baseConfig)
{
    initStartConf(baseConfig);
    init();
    initLoadConfig();
}

int BaseServerImpl::start()
{
    int ret = -1;
    ret = startThreads();
    if(-1 == ret) return ret;

    ret = startCommunicate();
    if(-1 == ret) return ret;

    ret = startServer();
    if(-1 == ret) return ret;

    ret = startConfigServer();
    if(-1 == ret) return ret;

    return ret;
}

void BaseServerImpl::setConfHandleWorker(ConfHandleWorker *confWorker)
{
    confHandleWorker = confWorker;
}

int BaseServerImpl::init()
{
    int ret = -1;
    ret = initParam();
    return ret;
}

int BaseServerImpl::initParam()
{
    tManager = new ThreadManager(this);
    tManagerFerry = new ThreadManagerFerry(this);
    connTimer = new QTimer(this);
    connTimer->setInterval(CONNINTERVAL);
    dataTimer = new QTimer(this);
    dataTimer->setInterval(DATAINTERVAL);

    //data
    rawDevInfoList = new QList<QByteArray>();
    rawDevInfoHash = new QHash<QString,QByteArray>();
    rawListMutex = new QMutex();
    netInfoMap = new QMap<QString,NetInfo*>();
    mapLock = new QReadWriteLock();

    //config
    configManager = new ConfigManager();
    return 0;
}

int BaseServerImpl::startThreads()
{
    qDebug("BaseServerImpl::startThreads...");
    if(NULL == config) return -1;
    NetThreadImpl* netThread = new NetThreadImpl();
    netThread->setConnConfigs(config);
    netThread->setNetInfoMap(netInfoMap,mapLock);
    connect(this,SIGNAL(dataTimeout(qint64)),netThread,SIGNAL(dataTimeout(qint64)));
    netThread->start();

    foreach (ConnClient* client, *config) {
        if(NULL == client) continue;
        WorkThread* thread = new WorkThread();
        thread->setClientInfo(client);
        thread->setTManagerFerry(tManagerFerry);
        thread->setConfigManager(configManager);
        connect(connTimer,SIGNAL(timeout()),thread,SLOT(connTimeoutSlot()));
        connect(this,SIGNAL(dataTimeout(qint64)),thread,SLOT(dataTimeoutSlot(qint64)));

        DataHandleThread* dataThread = new DataHandleThread();
        dataThread->setRawDataList(rawDevInfoList,rawListMutex);
        dataThread->setRawDataHash(rawDevInfoHash);
        dataThread->setConfigManager(configManager);
        dataThread->setConfHandleWorker(confHandleWorker);
        connect(thread,SIGNAL(dataReady(QString,QByteArray)),dataThread,SLOT(dataReadySlot(QString,QByteArray)));

        thread->start();
    }

    return 0;
}

int BaseServerImpl::startCommunicate()
{
    connTimer->start();
    dataTimer->start();
    connect(dataTimer,SIGNAL(timeout()),this,SLOT(dataTimeoutSlot()));

    return 0;
}

int BaseServerImpl::startServer()
{
    serviceServer = new ServiceServer();
    serviceServer->setRawList(rawDevInfoList,rawListMutex);
    serviceServer->setRawHash(rawDevInfoHash);
    serviceServer->setNetMap(netInfoMap,mapLock);
    serviceServer->setConfigManager(configManager);
    serviceServer->start();

    return 0;
}

int BaseServerImpl::startConfigServer()
{
    ConfigServerImpl* configServer = new ConfigServerImpl();
    configServer->start();

    return 0;
}

int BaseServerImpl::initStartConf(QList<ConnClient *> *baseConfig)
{
    config = baseConfig;

    return 0;
}

int BaseServerImpl::initLoadConfig()
{
    return configManager->initConf();
}

void BaseServerImpl::dataTimeoutSlot()
{
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    emit dataTimeout(timestamp);
}
