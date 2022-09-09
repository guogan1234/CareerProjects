#include "confdataferry.h"

ConfDataFerry::ConfDataFerry(QObject *parent) : QObject(parent)
{
    init();
}

int ConfDataFerry::loadConfData()
{
    confData->loadData();

    return 0;
}

QList<ConnClient *> *ConfDataFerry::getWorkPCConnClients()
{
    QList<ConnClient*> * ret = NULL;
    ret = confData->getWorkPCConnClients();

    return ret;
}

QList<ConnClient *> *ConfDataFerry::getStationConnClients()
{
    QList<ConnClient*> * ret = NULL;
    ret = confData->getStationConnClients();

    return ret;
}

QList<ConnClient *> *ConfDataFerry::getLocalDomainConnClients()
{
    QList<ConnClient*> * ret = NULL;
    ret = confData->getLocalDomainConnClients();

    return ret;
}

AlarmExtra *ConfDataFerry::getAlarmExtraByIp(QString ip)
{
    return confData->getAlarmExtraByIp(ip);
}

int ConfDataFerry::init()
{
    confData = new ConfData(this);

    return 0;
}
