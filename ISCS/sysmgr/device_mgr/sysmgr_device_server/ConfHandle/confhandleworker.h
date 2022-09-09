#ifndef CONFHANDLEWORKER_H
#define CONFHANDLEWORKER_H

#include <QObject>
#include <QList>
#include "Data/sysconfout.h"
#include "Data/alarmconfig.h"

class ConfHandleWorker : public QObject
{
    Q_OBJECT
public:
    explicit ConfHandleWorker(QObject *parent = nullptr);

    virtual int getStartLevel() = 0;
    virtual int getWorkPCList() = 0;
    virtual int getStationList() = 0;
    virtual int getLocalDomainList() = 0;

    virtual QList<ConnClient*> * getWorkPCsInfo() = 0;
    virtual QList<ConnClient*> * getStationsInfo() = 0;
    virtual QList<ConnClient*> * getLocalDomainInfo() = 0;
    virtual AlarmExtra* getAlarmExtraByIP(QString ip) = 0;
signals:

public slots:
};

#endif // CONFHANDLEWORKER_H
