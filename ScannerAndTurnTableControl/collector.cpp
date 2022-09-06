//#include "collector.h"
//#include <QMutex>
//#include "SocketData.h"

//QMutex udpMutex;
//QList<UdpRawData*> rawDataList;

//Collector::Collector(QObject *parent) : QObject(parent)
//{
//    initObject();
//}

//Collector::~Collector()
//{
//    delete socket;
//}

//void Collector::startNetwork()
//{
//    initSocketConn();
//}

//void Collector::initSocketConn()
//{
//    socket = new QUdpSocket();
//    connect(socket,&QUdpSocket::readyRead,this,&Collector::collerctData);
//    bool ret = socket->bind(QHostAddress(ip),port);
//    qDebug()<<"Collector::initSocketConn() return ="<<ret;
//}

//void Collector::collerctData()
//{
////    qDebug()<<"msg:";
//    if(socket->hasPendingDatagrams()){
////    while (socket->hasPendingDatagrams()) {//use while to decrease slot trigger, when udp datas send in while loop
////        qDebug()<<"Collector::collerctData";
//        qint64 dataSize = socket->pendingDatagramSize();
//        char* rawData = new char[dataSize];
//        qint64 readRet = -1;
//        readRet = socket->readDatagram(rawData, dataSize);
////        qDebug()<<"msg:"<<dataSize<<rawData;

//        UdpRawData* udpRawData = new UdpRawData();
//        udpRawData->rawData = rawData;
//        udpRawData->rawDataSize = dataSize;
//        udpMutex.lock();
////        qDebug()<<"list length(before add):"<<rawDataList.length();
//        rawDataList.append(udpRawData);
////        qDebug()<<"list length(after add):"<<rawDataList.length();
//        udpMutex.unlock();
//    }
//}

//void Collector::initObject()
//{
//    initParams();
//    initConnects();
//}

//void Collector::initParams()
//{
//    ip = "127.0.0.1";
//    port = 12345;
//}

//void Collector::initConnects()
//{
////    connect(socket,&QUdpSocket::readyRead,this,&Collector::collerctData);
//}
