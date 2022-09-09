#include "socketmanager.h"
#include <QHostAddress>

SocketManager::SocketManager(QObject *parent) : QObject(parent)
{

}

void SocketManager::setSocket(QTcpSocket *sock)
{
    socket = sock;
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorSlot(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
}

void SocketManager::disconnectedSlot()
{

}

void SocketManager::errorSlot(QAbstractSocket::SocketError socketError)
{

}

void SocketManager::readyReadSlot()
{
    qDebug("SocketManager::readyReadSlot");
    emit recvCommandSignal(socket,socket->readLine());
}

void SocketManager::sendResponseSlot(QTcpSocket *client, QByteArray response)
{
    qDebug("SocketManager::sendResponseSlot");
    QString clientAddr = client->peerAddress().toString();
    quint16 clientPort = client->peerPort();
    if(socket){
        QString socketAddr = socket->peerAddress().toString();
        quint16 socketPort = socket->peerPort();
        qDebug()<<"socket: "<<socketAddr<<socketPort;
        qDebug()<<"client: "<<clientAddr<<clientPort;
        if(clientAddr == socketAddr && clientPort == socketPort){
            if(socket->state()==QAbstractSocket::ConnectedState){
                qint64 len = socket->write(response);
                qDebug("socket send response - %lld",len);
            }
        }
    }
}
