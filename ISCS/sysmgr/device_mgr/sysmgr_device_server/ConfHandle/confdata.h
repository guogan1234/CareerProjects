#ifndef CONFDATA_H
#define CONFDATA_H

#include <QObject>
#include <QList>
#include "Data/sysconfin.h"
#include "Data/sysconfout.h"
#include "Data/alarmconfig.h"
#include "conf_parser.h"

class ConfData : public QObject
{
    Q_OBJECT
public:
    explicit ConfData(QObject *parent = nullptr);

    int loadData();
    QList<ConnClient*> * getWorkPCConnClients();
    QList<ConnClient*> * getStationConnClients();
    QList<ConnClient*> * getLocalDomainConnClients();
    AlarmExtra* getAlarmExtraByIp(QString ip);
signals:

public slots:

private:
    int init();
    int initParam();

    int initData();

    QList<SysDomain*> * sysDomainList;
    QList<SysNode*> * sysNodeList;
    QList<SysProcess*> * sysProcessList;
    QList<SysDomainProc*> * sysDomainProcList;

    QList<ConnClient*> * takeWorkPCConnClients();
    QList<ConnClient*> * takeStationConnClients();
    QList<ConnClient*> * takeGetLocalDomainConnClients();
    int getLocalDomainId();
};

#endif // CONFDATA_H
