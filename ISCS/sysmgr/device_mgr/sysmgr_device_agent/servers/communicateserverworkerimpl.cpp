#include "communicateserverworkerimpl.h"
#include "communicate/communicatethread.h"
#include "communicate/communicatesocket.h"
#include <QHostAddress>
#include <QTcpSocket>

CommunicateServerWorkerImpl::CommunicateServerWorkerImpl(QObject *parent) : CommunicateServerWorker(parent)
{
    init();
}

void CommunicateServerWorkerImpl::start()
{
    if(startTcpServer()){
        qDebug("server listened ok");
        startCommunicate();
        startSendMessage();
    }
}

void CommunicateServerWorkerImpl::setConfManager(ConfManager *manager)
{
    confManager = manager;
}

void CommunicateServerWorkerImpl::setInfoManager(DevInfoManager *manager)
{
    infoManager = manager;
    if(commandThread){
        commandThread->setInfoManager(manager);
    }
}

void CommunicateServerWorkerImpl::setCommandManager(CommandDataManager *manager)
{
    commandManager = manager;
    if(commandThread){
        commandThread->setCommandManager(manager);
    }
}

bool CommunicateServerWorkerImpl::startTcpServer()
{
    int listenPort = confManager->getCollectorListenPort("collector-port");
    qDebug()<<"listenPort - "<<listenPort;
    return listen(QHostAddress::Any,listenPort);
//    return listen(QHostAddress::Any,12345);
}

void CommunicateServerWorkerImpl::startCommunicate()
{

}

void CommunicateServerWorkerImpl::startSendMessage()
{

}

void CommunicateServerWorkerImpl::incomingConnection(qintptr socketDescriptor)
{
    qDebug("incomingConnection");
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);
    CommunicateSocket* communicateSocket = new CommunicateSocket();
    communicateSocket->setSocket(socket);
    communicateSocket->setCommandManager(commandManager);
    CommunicateThread* thread = new CommunicateThread();
    qDebug("before move to thread");
    socket->moveToThread(thread);
    communicateSocket->moveToThread(thread);
    qDebug("after move to thread");
    thread->start();

    qDebug("after thread start");
    if(!commandThread->isRunning()){
        commandThread->start();
    }else{
        qDebug("commandThread is running");
    }
}

void CommunicateServerWorkerImpl::init()
{
    initParam();
}

void CommunicateServerWorkerImpl::initParam()
{
    commandThread = new CommandHandleThread();
}
