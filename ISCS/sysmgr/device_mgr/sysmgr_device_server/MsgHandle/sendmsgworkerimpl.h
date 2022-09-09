#ifndef SENDMSGWORKERIMPL_H
#define SENDMSGWORKERIMPL_H

#include <QObject>
#include <QJsonDocument>
#include "sendmsgworker.h"
#include "sendmsgdataferry.h"

class SendMsgWorkerImpl : public SendMsgWorker
{
public:
    SendMsgWorkerImpl();

    SendMsg* getData();
    SendMsg* getData(qint64 timestamp);
    QJsonDocument* getDataJsonDoc(SendMsg* data);
    QByteArray getDataJson(SendMsg* data);
private:
    SendMsgDataFerry* ferry;

    int init();
    int initParam();

    int loadData();
    int loadData(qint64 timestamp);
};

#endif // SENDMSGWORKERIMPL_H
