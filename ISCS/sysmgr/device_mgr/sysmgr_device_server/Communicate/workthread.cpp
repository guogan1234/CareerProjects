#include "workthread.h"
#include <QDateTime>

#define PORT 12345

WorkThread::WorkThread()
{
    init();
}

void WorkThread::run()
{
    addThread();
    initSocket();
    int bConn = -1;
    bConn = initConn();
    if(0 != bConn){
        bConn = loopConnectClient();
    }
    if(0 == bConn){//connected
        setFerrySocket(socket);
        setSocketWriter(socket);
//        sendMsg();
    }

    exec();
}

void WorkThread::setTManagerFerry(ThreadManagerFerry *ferry)
{
    takeSetTManagerFerry(ferry);
}

void WorkThread::setClientInfo(ConnClient *info)
{
    takeSetClientInfo(info);
}

void WorkThread::setConfigManager(ConfigManager *manager)
{
    configManager = manager;
}

int WorkThread::init()
{
    initParam();

    return 0;
}

void WorkThread::initParam()
{
    bConnAlarm = false;
    bDataAlarm = false;
}

void WorkThread::takeSetTManagerFerry(ThreadManagerFerry *ferry)
{
    tManagerFerry = ferry;
}

void WorkThread::takeSetClientInfo(ConnClient *info)
{
    clientInfo = info;
}

int WorkThread::addThread()
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
    setObjectName(name);

    return 0;
}

int WorkThread::initSocket()
{
    socket = new QTcpSocket();
    socketFerry = new WorkThreadSocketFerry();
    socketWriter = new SocketWriterWorkerImpl();

    connect(socket,SIGNAL(aboutToClose()),socketFerry,SLOT(aboutToCloseSlot()));
    connect(socket,SIGNAL(bytesWritten(qint64)),socketFerry,SLOT(bytesWrittenSlot(qint64)));
    connect(socket,SIGNAL(connected()),socketFerry,SLOT(connectedSlot()));
    connect(socket,SIGNAL(destroyed(QObject*)),socketFerry,SLOT(destroyedSlot(QObject*)));
    connect(socket,SIGNAL(disconnected()),socketFerry,SLOT(disconnectedSlot()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),socketFerry,SLOT(errorSlot(QAbstractSocket::SocketError)));
//    connect(socket,SIGNAL(hostFound()),socketFerry,SLOT(hostFoundSlot()));
    connect(socket,SIGNAL(readyRead()),socketFerry,SLOT(readyReadSlot()));
//    connect(socket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),socketFerry,SLOT(stateChangedSlot(QAbstractSocket::SocketState)));

    connect(this,SIGNAL(dataTimeout(qint64)),socketFerry,SLOT(dataTimeoutSlot(qint64)));
    connect(this,SIGNAL(dataTimeout(qint64)),socketWriter,SLOT(dataTimoutSlot(qint64)));

    connect(socketFerry,SIGNAL(dataReady(QString,QByteArray)),this,SIGNAL(dataReady(QString,QByteArray)));

    return 0;
}

int WorkThread::initConn()
{
    if(NULL == clientInfo) return -1;
    qDebug()<<"len - "<<clientInfo->ips->length()<<objectName();
    int connectPort = configManager->getConnectPort("connect_port");
    qDebug("connectPort - %d\n",connectPort);
    foreach (QString ip, *clientInfo->ips) {
        QHostAddress hostAddr(ip);
//        socket->connectToHost(hostAddr,PORT);
        socket->connectToHost(hostAddr,connectPort);
        if(socket->waitForConnected()){
            socketFerry->setConnIp(ip);
            return 0;
        }else {
//            int ret = reconnectClient(socket,hostAddr,PORT);
            int ret = reconnectClient(socket,hostAddr,connectPort);
            socketFerry->setConnIp(ip);
            if(0 == ret){
                return 0;
            }else {
                qDebug()<<"initConn:"<<socket->errorString()<<ip<<objectName();
            }
        }
    }

    return -1;
}

int WorkThread::reconnectClient(QTcpSocket *socket, QHostAddress &host, int port)
{
    socket->connectToHost(host,port);
    if(socket->waitForConnected()){
        return 0;
    }else {
        return -1;
    }
}

int WorkThread::loopConnectClient()
{
    while (true) {
        if(bConnAlarm){
            bConnAlarm = false;

            int ret = -1;
            ret = initConn();
            if(0 == ret) return ret;
        }else {
            msleep(100);
        }
    }
}

int WorkThread::setSocketWriter(QTcpSocket *socket)
{
    if(NULL != socketWriter){
        socketWriter->setSocket(socket);
        return 0;
    }else {
        return -1;
    }
}

int WorkThread::setFerrySocket(QTcpSocket *socket)
{
    if(!socketFerry) return -1;
    socketFerry->setSocket(socket);
    return 0;
}

void WorkThread::sendMsg()
{
//    qDebug("sendMsg...\n");
    while (true) {
        if(bDataAlarm){
            sendDataRequest();

            bDataAlarm = false;
        }else{
            msleep(100);
        }
    }
}

int WorkThread::sendDataRequest()
{
    if(NULL != socketWriter){
        int ret = -1;
        ret = socketWriter->sendDataRequest();
        if(-1 == ret){
            qDebug()<<"SendError:"<<socket->errorString();
            return -1;
        }
    }

    return 0;
}

QString WorkThread::getCurrentThreadId()
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

void WorkThread::connTimeoutSlot()
{
//    qDebug()<<"connTimeoutSlot...";
    bConnAlarm = true;
}

void WorkThread::dataTimeoutSlot(qint64 timestamp)
{
    qDebug()<<"dataTimeoutSlot...";
    bDataAlarm = true;
    emit dataTimeout(timestamp);
}
