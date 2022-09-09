#ifndef CONFHANDLEWORKIMPL_H
#define CONFHANDLEWORKIMPL_H

#include <QObject>
#include "confhandleworker.h"
#include "confdataferry.h"
//#include "Data/sysconfout.h"

class ConfHandleWorkImpl : public ConfHandleWorker
{
public:
    ConfHandleWorkImpl(QObject* parent);

    int getStartLevel();

    int getWorkPCList();
    int getStationList();
    int getLocalDomainList();

    QList<ConnClient*> * getWorkPCsInfo();
    QList<ConnClient*> * getStationsInfo();
    QList<ConnClient*> * getLocalDomainInfo();
    AlarmExtra* getAlarmExtraByIP(QString ip);
private:
    ConfDataFerry* ferry;

    int init();
    int initParam();

    QList<ConnClient*> * workPCList;
    QList<ConnClient*> * stationList;
    QList<ConnClient*> * localDomainList;

    int takeStartLevel();

    int takeWorkPCList();
    int takeStationList();
    int takeGetLocalDomainList();
};

#endif // CONFHANDLEWORKIMPL_H
