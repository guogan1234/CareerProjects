#include "sendmsgdata.h"
#include <QDateTime>

SendMsgData::SendMsgData(QObject *parent) : QObject(parent)
{
    init();
}

int SendMsgData::loadData()
{
    int ret = -1;
    ret = initData();
    return ret;
}

int SendMsgData::loadData(qint64 timestamp)
{
    int ret = -1;
    ret = initData(timestamp);
    return ret;
}

int SendMsgData::removeData()
{
    if(!data) return -1;
    delete data;
    return 0;
}

SendMsg *SendMsgData::getData()
{
    return takeGetData();
}

int SendMsgData::init()
{
    return initParam();
}

int SendMsgData::initParam()
{
    return 0;
}

int SendMsgData::initData()
{
    data = new SendMsg();
    if(NULL == data) return -1;
    data->setSeq(QDateTime::currentMSecsSinceEpoch());
    data->setCommand("data");
    return 0;
}

int SendMsgData::initData(qint64 timestamp)
{
    data = new SendMsg();
    if(NULL == data) return -1;
    data->setSeq(timestamp);
    data->setCommand("data");
    return 0;
}

SendMsg *SendMsgData::takeGetData()
{
    return data;
}
