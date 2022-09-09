#ifndef CONFDATA_H
#define CONFDATA_H

#include <QObject>
#include <QList>
#include "Data/sysconfin.h"
#include "Data/sysconfout.h"
#include "conf_parser.h"

class ConfData : public QObject
{
    Q_OBJECT
public:
    explicit ConfData(QObject *parent = nullptr);

    int loadData();
    QList<ConnClient*> * getWorkPCConnClients();
    QList<ConnClient*> * getStationConnClients();
    QList<ConnClient*> * getLocalDomainServer();
    QHash<int,DomainNodes*> * getDomainNodes();
    QHash<int,DomainNodes*> * getLocalDomainNodes();
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
    QList<ConnClient*> * takeGetLocalDomainServer();
    //获取本地域列表和域下节点
    QHash<int,DomainNodes*> * takeGetLocalDomainNodes();
    //获取本地域ID
    int getLocalDomainId();

    //获取所有域列表和域下节点
    QHash<int,DomainNodes*> * takeGetDomainNodes();

    //根据域ID获取所属域中节点列表
    QList<SysNode*> * getDomainSysNodes(int domainId);
};

#endif // CONFDATA_H
