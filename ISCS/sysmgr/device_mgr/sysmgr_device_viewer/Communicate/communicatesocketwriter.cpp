#include "communicatesocketwriter.h"

CommunicateSocketWriter::CommunicateSocketWriter(QObject *parent) : QObject(parent)
{

}

void CommunicateSocketWriter::setSocket(QTcpSocket *socket)
{
    this->socket = socket;
}

qint64 CommunicateSocketWriter::writeData(const QByteArray &data)
{
    if(socket && socket->state()==QAbstractSocket::ConnectedState){
        return socket->write(data);
    }
    return -1;
}

QString CommunicateSocketWriter::getSocketError()
{
    return socket->errorString();
}
