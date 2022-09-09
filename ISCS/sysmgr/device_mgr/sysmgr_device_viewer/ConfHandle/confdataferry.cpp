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

QList<ConnClient *> *ConfDataFerry::getLocalDomainServer()
{
    return confData->getLocalDomainServer();
}

QHash<int, DomainNodes *> *ConfDataFerry::getDomainNodes()
{
    return confData->getDomainNodes();
}

QHash<int, DomainNodes *> *ConfDataFerry::getLocalDomainNodes()
{
    return confData->getLocalDomainNodes();
}

int ConfDataFerry::init()
{
    confData = new ConfData(this);

    return 0;
}
