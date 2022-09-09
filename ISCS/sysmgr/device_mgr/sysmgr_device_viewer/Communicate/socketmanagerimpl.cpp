#include "socketmanagerimpl.h"
#include <QDateTime>

SocketManagerImpl::SocketManagerImpl(QObject *parent) : QObject(parent)
{

}

void SocketManagerImpl::setConnServer(QList<ConnClient *> *server)
{
    connServer = server;
}

bool SocketManagerImpl::connectServer()
{
    socket = new QTcpSocket();
    for(int i = 0;i < connServer->size(); ++i){
        ConnClient* client = connServer->at(i);
        QList<QString> * connIps = NULL;
        if(client){
            connIps = client->ips;
        }
        if(connIps){
            for(int a = 0;a < connIps->size(); ++a){
                QString ip = connIps->at(a);
                socket->connectToHost(ip,12346);
                qDebug()<<"start connect,server ip - "<<ip;
                if(socket->waitForConnected(1000)){
                    initConnects();
                    qDebug()<<"connected ok!--server ip->"<<ip;
                    return true;
                }else {
                    socket->connectToHost(ip,12346);
                    if(socket->waitForConnected(1000)){
                        initConnects();
                        qDebug()<<"connected ok!--server ip->"<<ip;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

QTcpSocket *SocketManagerImpl::getConnSocket()
{
    return socket;
}

void SocketManagerImpl::connectedSlot()
{

}

void SocketManagerImpl::disconnectedSlot()
{
    socket->close();
    socket->waitForDisconnected();
    delete socket;
    socket = NULL;
}

void SocketManagerImpl::errorSlot(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
}

int receiveNum = 0;
int realReceiveCount = 0;
void SocketManagerImpl::readyReadSlot()
{
    qDebug("SocketManagerImpl::readyReadSlot - %d",receiveNum);
    receiveNum++;
    QByteArray data;
    if(socket->canReadLine()){
        data = socket->readLine();
    }else{
        return;
    }
    qDebug()<<"[startTime]"<<QDateTime::currentDateTime()<<realReceiveCount;
    emit recvDataSignal(data);
    qDebug()<<"[startTime2]"<<QDateTime::currentDateTime()<<realReceiveCount;
    realReceiveCount++;
}

void SocketManagerImpl::initConnects()
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    connect(socket,SIGNAL(connected()),this,SLOT(connectedSlot()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorSlot(QAbstractSocket::SocketError)));
}
