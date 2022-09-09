#include "sendmsgworkerimpl.h"
#include <QJsonObject>

SendMsgWorkerImpl::SendMsgWorkerImpl()
{
    init();
}

SendMsg *SendMsgWorkerImpl::getData()
{
    int ret = -1;
    ret = loadData();
    if(-1 == ret) return NULL;
    return ferry->getData();
}

SendMsg *SendMsgWorkerImpl::getData(qint64 timestamp)
{
    int ret = -1;
    ret = loadData(timestamp);
    if(-1 == ret) return NULL;
    return ferry->getData();
}

QJsonDocument *SendMsgWorkerImpl::getDataJsonDoc(SendMsg *data)
{
    if(!data || !ferry) return NULL;
    QJsonDocument* doc = new QJsonDocument();
    QJsonObject obj;
    obj.insert("seq",QJsonValue(data->getSeq()));
    obj.insert("command",QJsonValue(data->getCommand()));
    doc->setObject(obj);
    if(ferry) ferry->removeData();

    return doc;
}

QByteArray SendMsgWorkerImpl::getDataJson(SendMsg *data)
{
    QJsonDocument* doc = getDataJsonDoc(data);
    if(NULL == doc) return NULL;
    return doc->toJson(QJsonDocument::Compact);
}

int SendMsgWorkerImpl::init()
{
    int ret = -1;
    ret = initParam();
    return ret;
}

int SendMsgWorkerImpl::initParam()
{
    ferry = new SendMsgDataFerry(this);
    if(NULL == ferry) return -1;
    return 0;
}

int SendMsgWorkerImpl::loadData()
{
    if(NULL == ferry) return -1;
    return ferry->loadData();
}

int SendMsgWorkerImpl::loadData(qint64 timestamp)
{
    if(NULL == ferry) return -1;
    return ferry->loadData(timestamp);
}
