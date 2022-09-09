#include "socketwriterworkerimpl.h"
#include <QThread>

SocketWriterWorkerImpl::SocketWriterWorkerImpl(QObject *parent) : QObject(parent)
{
    init();
}

void SocketWriterWorkerImpl::setSocket(QTcpSocket *socket)
{
    this->socket = socket;
}

int SocketWriterWorkerImpl::sendDataRequest()
{
    return takeSendDataRequest(0);
}

void SocketWriterWorkerImpl::dataTimoutSlot(qint64 timestamp)
{
//    qDebug()<<"Impl::dataTimoutSlot...";

    int ret = -1;
    ret = takeSendDataRequest(timestamp);
    if(-1 == ret){
        takeSendDataRequest(timestamp);
    }
}

void SocketWriterWorkerImpl::init()
{
    initParam();
}

void SocketWriterWorkerImpl::initParam()
{
    sendMsgImpl = new SendMsgWorkerImpl();
}

int SocketWriterWorkerImpl::takeSendDataRequest(qint64 timestamp)
{
    qDebug("takeSendDataRequest...\n");
    if(!socket){
        qDebug()<<"socket not init!";
        return -1;
    }else {
        if(socket->state() != QAbstractSocket::ConnectedState){
            qDebug()<<"socket not connected!"<<socket->state();
            return -1;
        }else {
            QByteArray ba;
            ba = sendMsgImpl->getDataJson(sendMsgImpl->getData(timestamp));
//            qDebug()<<"ba - "<<ba.length();
            ba.append('\n');
            int ret = -1;
            ret = socket->write(ba);
//            qDebug()<<"write len - "<<ret;
//            socket->flush();
            if(-1 == ret){
                ret = socket->write(ba);
                return ret;
            }
        }
    }

    return 0;
}
