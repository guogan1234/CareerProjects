#include "workthreadsocketferry.h"
#include <QThread>

WorkThreadSocketFerry::WorkThreadSocketFerry(QObject *parent) : QObject(parent)
{

}

void WorkThreadSocketFerry::setSocket(QTcpSocket *socket)
{
    takeSetSocket(socket);
}

void WorkThreadSocketFerry::setConnIp(QString ip)
{
    connIp = ip;
}

int WorkThreadSocketFerry::connectedSlot()
{
    qDebug()<<"connectedSlot...";
    getCurrentThreadId();

    return 0;
}

int WorkThreadSocketFerry::disconnectedSlot()
{
    qDebug()<<"disconnectedSlot...";
    getCurrentThreadId();

    return 0;
}

int WorkThreadSocketFerry::errorSlot(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"errorSlot..."<<"error:"<<socketError;
    getCurrentThreadId();

    return 0;
}

int WorkThreadSocketFerry::hostFoundSlot()
{
    qDebug()<<"hostFoundSlot...";
    getCurrentThreadId();

    return 0;
}

int WorkThreadSocketFerry::stateChangedSlot(QAbstractSocket::SocketState socketState)
{
//    qDebug()<<"stateChangedSlot..."<<"socketState:"<<socketState;
//    getCurrentThreadId();

    return 0;
}

int WorkThreadSocketFerry::aboutToCloseSlot()
{
    qDebug()<<"aboutToCloseSlot...";
    getCurrentThreadId();

    return 0;
}

int WorkThreadSocketFerry::bytesWrittenSlot(qint64 length)
{
    qDebug()<<"bytesWrittenSlot..."<<"length:"<<length;
    getCurrentThreadId();

    return 0;
}

int WorkThreadSocketFerry::readyReadSlot()
{
    qDebug()<<"readyReadSlot...";
    getCurrentThreadId();
    if(!socket){
        qDebug()<<"[Ferry]readyReadSlot:socket is NULL";
        return -1;
    }
    qint64 bAvail = socket->bytesAvailable();
    bool bLine = socket->canReadLine();
    QByteArray ba;
    if(bLine){
        ba = socket->readLine();
    }
    qDebug()<<"[Ferry]"<<bAvail<<ba.length()<<bLine;
//    qDebug()<<"[Ferry]"<<bAvail<<bLine;

    if(ba.isEmpty()) return -1;
    emit dataReady(connIp,ba);

    return 0;
}

int WorkThreadSocketFerry::destroyedSlot(QObject *obj)
{
    ((void)obj);
    qDebug()<<"destroyedSlot...";
    getCurrentThreadId();

    return 0;
}

void WorkThreadSocketFerry::dataTimeoutSlot(qint64 timestamp)
{
    qDebug()<<"Ferry:dataTimeoutSlot..."<<timestamp;
    getCurrentThreadId();
}

QString WorkThreadSocketFerry::getCurrentThreadId()
{
    Qt::HANDLE id = QThread::currentThreadId();
#ifdef Q_OS_LINUX
    long *pId = (long*)id;
    QString name = QString("%1").arg(*pId);
#elif defined(Q_OS_WIN)
    long pId = (long)id;
    QString name;
    name = QString("%1").arg(pId);
#endif
    qDebug()<<"getCurrentThreadId..."<<name;

    return name;
}

void WorkThreadSocketFerry::takeSetSocket(QTcpSocket *socket)
{
    this->socket = socket;
}
