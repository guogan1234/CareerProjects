#ifndef CONFHANDLEWORKIMPL_H
#define CONFHANDLEWORKIMPL_H

#include <QObject>
#include "confhandleworker.h"
#include "confdataferry.h"
#include "Data/sysconfout.h"

class ConfHandleWorkImpl : public ConfHandleWorker
{
public:
    ConfHandleWorkImpl(QObject* parent = 0);

    int getStartLevel();
    int getWorkPCList();
    int getStationList();

    QList<ConnClient*> * getWorkPCsInfo();
    QList<ConnClient*> * getStationsInfo();
    QList<ConnClient*> * getLocalDomainServer();

    QHash<int,DomainNodes*> * getDomainNodes();
    QHash<int,DomainNodes*> * getLocalDomainNodes();
private:
    ConfDataFerry* ferry;

    int init();
    int initParam();

    QList<ConnClient*> * workPCList;
    QList<ConnClient*> * stationList;

    int takeStartLevel();
    int takeWorkPCList();
    int takeStationList();
};

#endif // CONFHANDLEWORKIMPL_H
