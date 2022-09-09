#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include "Data/sysconfout.h"
#include "Communicate/threadmanagerferry.h"
#include "workthreadsocketferry.h"
#include "socketwriterworkerimpl.h"
#include "ConfHandle/configmanager.h"

class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread();

    void run();
    void setTManagerFerry(ThreadManagerFerry* ferry);
    void setClientInfo(ConnClient* info);
    void setConfigManager(ConfigManager* manager);

signals:
    void dataTimeout(qint64);
    void dataReady(QString,QByteArray);
private:
    bool bConnAlarm;
    bool bDataAlarm;

    int init();
    void initParam();

    ConnClient* clientInfo;
    QTcpSocket* socket;
    WorkThreadSocketFerry* socketFerry;
    ThreadManagerFerry* tManagerFerry;
    SocketWriterWorkerImpl* socketWriter;
    ConfigManager* configManager;

    void takeSetTManagerFerry(ThreadManagerFerry* ferry);
    void takeSetClientInfo(ConnClient* info);
    int addThread();
    int initSocket();
    int initConn();

    int reconnectClient(QTcpSocket* socket,QHostAddress &host,int port);
    int loopConnectClient();
    int setSocketWriter(QTcpSocket* socket);
    int setFerrySocket(QTcpSocket* socket);

    void sendMsg();
    int sendDataRequest();

    QString getCurrentThreadId();

public slots:
    void connTimeoutSlot();
    void dataTimeoutSlot(qint64 timestamp);
};

#endif // WORKTHREAD_H
