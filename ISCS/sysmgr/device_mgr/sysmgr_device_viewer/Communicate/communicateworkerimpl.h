#ifndef COMMUNICATEWORKERIMPL_H
#define COMMUNICATEWORKERIMPL_H

#include <QObject>
#include <QTcpSocket>
#include <QStandardItem>
#include "socketmanagerimpl.h"
#include "communicatesocketwriter.h"
#include "Parser/devinforequestparser.h"
#include <QTimer>

//通讯线程工作者类
class CommunicateWorkerImpl : public QObject
{
        Q_OBJECT
    public:
        explicit CommunicateWorkerImpl(QObject *parent = 0);
        void setConnServer(QList<ConnClient*> * server);
    signals:
        void recvDataSignal(QByteArray ba);
    public slots:
        void devInfoRequestSlot(QStandardItem* item);
        void timerSlot();
        void updateCollectPeriodSlot(int msec);
    private:
        //socket通讯管理类
        SocketManagerImpl* socketManager;
        CommunicateSocketWriter* socketWriter;

        void init();
        void initParam();

        DevInfoRequest* itemDataToRequest(QVariant& itemData);

        QStandardItem* currentItem;
        QTimer* timer;
};

#endif // COMMUNICATEWORKERIMPL_H
