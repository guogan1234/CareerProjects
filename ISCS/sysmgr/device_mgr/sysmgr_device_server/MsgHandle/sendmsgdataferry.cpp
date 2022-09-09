#include "sendmsgdataferry.h"

SendMsgDataFerry::SendMsgDataFerry(QObject *parent) : QObject(parent)
{
    init();
}

int SendMsgDataFerry::loadData()
{
    return msgData->loadData();
}

int SendMsgDataFerry::loadData(qint64 timestamp)
{
    return msgData->loadData(timestamp);
}

int SendMsgDataFerry::removeData()
{
    return msgData->removeData();
}

SendMsg *SendMsgDataFerry::getData()
{
    return msgData->getData();
}

int SendMsgDataFerry::init()
{
    return initParam();
}

int SendMsgDataFerry::initParam()
{
    msgData = new SendMsgData(this);
    if(NULL == msgData) return -1;
    return 0;
}
