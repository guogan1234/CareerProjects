#include "serviceserver.h"
#include "servicethread.h"
#include "Defines/baseserverdefine.h"
#include <QHostAddress>

ServiceServer::ServiceServer(QObject *parent) : QTcpServer(parent)
{
    init();
}

void ServiceServer::setRawList(QList<QByteArray> *list, QMutex *mutex)
{
    commandThread->setRawInfoList(list,mutex);
}

void ServiceServer::setRawHash(QHash<QString, QByteArray> *hash)
{
    commandThread->setRawInfoHash(hash);
}

void ServiceServer::setNetMap(QMap<QString, NetInfo *> *map, QReadWriteLock *lock)
{
    commandThread->setNetInfoMap(map,lock);
}

void ServiceServer::setConfigManager(ConfigManager *manager)
{
    configManager = manager;
}

int ServiceServer::start()
{
    int servicePort = configManager->getServicePort("service_port");
    qDebug("servicePort - %d\n",servicePort);
//    if(listen(QHostAddress::Any,SERVICEPORT)){
    if(listen(QHostAddress::Any,servicePort)){
        commandThread->start();
        return 0;
    }else {
        return -1;
    }
}

void ServiceServer::incomingConnection(qintptr handle)
{
    ServiceThread* thread = new ServiceThread();
    thread->setSockDescriptor(handle);
    connect(thread,SIGNAL(recvCommandSignal(QTcpSocket*,QByteArray)),commandThread,SIGNAL(recvCommandSignal(QTcpSocket*,QByteArray)));
    connect(commandThread,SIGNAL(sendResponseSignal(QTcpSocket*,QByteArray)),thread,SIGNAL(sendResponseSignal(QTcpSocket*,QByteArray)));
    thread->start();
}

void ServiceServer::init()
{
    initParam();
}

void ServiceServer::initParam()
{
    commandThread = new CommandHandleThread();
}
