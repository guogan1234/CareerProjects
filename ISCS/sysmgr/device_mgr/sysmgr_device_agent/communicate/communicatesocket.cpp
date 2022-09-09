#include "communicatesocket.h"
#include <QThread>
#include <QDebug>

CommunicateSocket::CommunicateSocket(QObject *parent) : QObject(parent)
{

}

void CommunicateSocket::setSocket(QTcpSocket *socket)
{
    takeSetSocket(socket);
    initConnects();
}

void CommunicateSocket::setCommandManager(CommandDataManager *manager)
{
    takeSetCommandManager(manager);
}

void CommunicateSocket::initConnects()
{
    connect(clientSocket,SIGNAL(connected()),this,SLOT(connectedSlot()));
    connect(clientSocket,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
    connect(clientSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorSlot(QAbstractSocket::SocketError)));
    connect(clientSocket,SIGNAL(hostFound()),this,SLOT(hostFoundSlot()));
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
}

void CommunicateSocket::connectedSlot()
{
    qDebug("connectedSlot");
}

void CommunicateSocket::disconnectedSlot()
{
    qDebug("disconnectedSlot");
}

void CommunicateSocket::errorSlot(QAbstractSocket::SocketError socketError)
{
    qDebug("errorSlot");
}

void CommunicateSocket::hostFoundSlot()
{
    qDebug("hostFoundSlot");
}

void CommunicateSocket::readyReadSlot()
{
    qDebug("readyReadSlot");
    QByteArray ba = clientSocket->readLine();
    qDebug("read - %d",ba.length());
    qDebug()<<"ba - "<<ba;
    CommandMsg* msg = new CommandMsg();
    msg->clientSocket = clientSocket;
    msg->command = ba;

//    clientSocket->write("this is the data");
    commandManager->addCommandMsg(msg);
}

void CommunicateSocket::takeSetSocket(QTcpSocket *socket)
{
    clientSocket = socket;
}

void CommunicateSocket::takeSetCommandManager(CommandDataManager *manager)
{
    commandManager = manager;
}
