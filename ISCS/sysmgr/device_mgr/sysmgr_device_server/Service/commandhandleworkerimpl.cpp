#include "commandhandleworkerimpl.h"
#include "Parser/devinforequestparser.h"
#include "Parser/fulldevinfoparser.h"

CommandHandleWorkerImpl::CommandHandleWorkerImpl(QObject *parent) : QObject(parent)
{

}

void CommandHandleWorkerImpl::setDataFinder(DataFinder *finder)
{
    dataFinder = finder;
}

void CommandHandleWorkerImpl::recvMessageSlot(QTcpSocket *client, QByteArray msg)
{
    qDebug("CommandHandleWorkerImpl::recvMessageSlot - %d",msg.length());
    DevInfoRequest* request = DevInfoRequestParser::byteArrayToJsonClass(msg);
    FullDevInfo* response = NULL;
    if(request){
        response = dataFinder->getMatchedData(request);
    }else {
        qDebug("CommandHandleWorkerImpl::request is NULL.");
    }
    if(response){
        QByteArray bArray = FullDevInfoParser::jsonClassToByteArray(response);
        bArray.append('\n');
//        qDebug()<<"CommandHandleWorkerImpl::response view request data - "<<bArray;
        emit sendResponseSignal(client,bArray);
    }
}
