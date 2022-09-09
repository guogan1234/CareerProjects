#include "configserverimpl.h"
#include <QTcpSocket>
#include <QThread>
#include "ConfHandle/configworkerimpl.h"

#define CONFIG_PORT 9102

ConfigServerImpl::ConfigServerImpl(QObject *parent) : QTcpServer(parent)
{
    init();
}

void ConfigServerImpl::start()
{
    listen(QHostAddress::Any,CONFIG_PORT);
}

void ConfigServerImpl::incomingConnection(qintptr handle)
{
    QThread* thread = new QThread();

    socket = new QTcpSocket();
    socket->setSocketDescriptor(handle);
    socket->moveToThread(thread);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readSlot()));

    configWorker->moveToThread(thread);

    thread->start();
}

void ConfigServerImpl::readSlot()
{
    QByteArray bArray; 
    if(socket->canReadLine()){
        bArray = socket->readLine();
    }
    qDebug()<<"[ConfigServerImpl]"<<bArray;
    emit dataSignal(bArray);
}

void ConfigServerImpl::init()
{
    configWorker = new ConfigWorkerImpl();
    connect(this,SIGNAL(dataSignal(QByteArray)),configWorker,SLOT(dataIncome(QByteArray)));
}
