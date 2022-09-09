#include "communicateworkerimpl.h"
#include <QDebug>

CommunicateWorkerImpl::CommunicateWorkerImpl(QObject *parent) : QObject(parent)
{
    init();
}

void CommunicateWorkerImpl::setConnServer(QList<ConnClient *> *server)
{
    socketManager->setConnServer(server);
}

void CommunicateWorkerImpl::devInfoRequestSlot(QStandardItem *item)
{
    qDebug()<<"item - "<<item->text();
//    if(socketManager->connectServer()){
//        DevInfoRequest* dev = NULL;
//        QVariant nodeData = item->data(Qt::UserRole);
//        dev = itemDataToRequest(nodeData);

//        socketWriter->setSocket(socketManager->getConnSocket());
//        QByteArray request = DevInfoRequestParser::jsonClassToByteArray(dev);
//        qDebug()<<"request - "<<request;
//        qint64 writeLen = socketWriter->writeData(DevInfoRequestParser::jsonClassToByteArray(dev));
//        if(-1 == writeLen){
//            qDebug()<<socketWriter->getSocketError();
//        }
//    }
    if(socketManager->connectServer()){
        socketWriter->setSocket(socketManager->getConnSocket());
        currentItem = item;
    }
}

int sendCount = 0;
void CommunicateWorkerImpl::timerSlot()
{
    if(!currentItem) return;
    DevInfoRequest* dev = NULL;
    QVariant nodeData = currentItem->data(Qt::UserRole);
    dev = itemDataToRequest(nodeData);

//    QByteArray request = DevInfoRequestParser::jsonClassToByteArray(dev);
//    qDebug()<<"request - "<<request;
    qDebug()<<"sendCount - "<<sendCount;
    sendCount++;
    qint64 writeLen = socketWriter->writeData(DevInfoRequestParser::jsonClassToByteArray(dev));
    if(-1 == writeLen){
        qDebug()<<socketWriter->getSocketError();
    }
}

void CommunicateWorkerImpl::updateCollectPeriodSlot(int msec)
{
    qDebug("new collect period - %d",msec);
    if(msec <= 1000) return;
    timer->start(msec);
}

void CommunicateWorkerImpl::init()
{
    initParam();
}

void CommunicateWorkerImpl::initParam()
{
    socketManager = new SocketManagerImpl();
    connect(socketManager,SIGNAL(recvDataSignal(QByteArray)),this,SIGNAL(recvDataSignal(QByteArray)));
    socketWriter = new CommunicateSocketWriter();

    currentItem = NULL;
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    timer->start(1000);
}

DevInfoRequest *CommunicateWorkerImpl::itemDataToRequest(QVariant &nodeData)
{
    DevInfoRequest* dev = new DevInfoRequest();
    if(nodeData.canConvert<QVariantMap>()){
        QVariantMap map = nodeData.toMap();
        QVariant varName = map.value("name");
        if(varName.canConvert<QString>()){
            dev->name = varName.toString();
        }
        QVariant varIps = map.value("ips");
        if(varIps.canConvert<QVariantList>()){
            QVariantList ipList = varIps.toList();
            QList<QString> * devIpList = new QList<QString>();
            for(int i = 0;i < ipList.size(); ++i){
                QVariant varIp = ipList.at(i);
                QString ip;
                if(varIp.canConvert<QString>()){
                    ip = varIp.toString();
                    devIpList->append(ip);
                }
            }
            dev->ips = devIpList;
        }
    }
    return dev;
}
