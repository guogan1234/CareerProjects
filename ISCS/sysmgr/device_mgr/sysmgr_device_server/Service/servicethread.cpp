#include "servicethread.h"

ServiceThread::ServiceThread(QObject *parent) : QThread(parent)
{

}

void ServiceThread::setSockDescriptor(qintptr sockfd)
{
    sock = sockfd;
}

void ServiceThread::run()
{
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(sock);
    socketManager = new SocketManager();
    socketManager->setSocket(socket);
    connect(socketManager,SIGNAL(recvCommandSignal(QTcpSocket*,QByteArray)),this,SIGNAL(recvCommandSignal(QTcpSocket*,QByteArray)));
    connect(this,SIGNAL(sendResponseSignal(QTcpSocket*,QByteArray)),socketManager,SLOT(sendResponseSlot(QTcpSocket*,QByteArray)));
    exec();
}
