#include "appserverworkerimpl.h"
#include <QThread>

AppServerWorkerImpl::AppServerWorkerImpl()
{
    init();
}

void AppServerWorkerImpl::start()
{
    startLoadConfig();
    startCollectServer();
    startCommunicateServer();
}

void AppServerWorkerImpl::startLoadConfig()
{
    confManager->initConf();
}

void AppServerWorkerImpl::startCollectServer()
{ 
    qDebug("AppServerWorkerImpl::startCollectServer");
    collectServer = new CollectServerWorkerImpl(this);
    collectServer->setConfManager(confManager);
    collectServer->setInfoManager(devInfoManager);
    collectServer->start();
}

void AppServerWorkerImpl::startCommunicateServer()
{
    qDebug("AppServerWorkerImpl::startCommunicateServer");
    communicateServer = new CommunicateServerWorkerImpl();
    communicateServer->setConfManager(confManager);
    communicateServer->setCommandManager(commandManager);
    communicateServer->setInfoManager(devInfoManager);
    communicateServer->start();
}

void AppServerWorkerImpl::init()
{
    initParam();
}

void AppServerWorkerImpl::initParam()
{
    confManager = new ConfManager();
    devInfoManager = new DevInfoManager(this);
    devInfoLocker = new QReadWriteLock();
    commandManager = new CommandDataManager();
    qDebug("AppServerWorkerImpl::initParam");
}
