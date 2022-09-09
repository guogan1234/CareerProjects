#include "confhandleworkimpl.h"
#include <QDebug>

ConfHandleWorkImpl::ConfHandleWorkImpl(QObject *parent):ConfHandleWorker(parent)
{
    init();
}

int ConfHandleWorkImpl::getStartLevel()
{
    int ret = -1;
    ret = takeStartLevel();

    return ret;
}

int ConfHandleWorkImpl::getWorkPCList()
{
    int ret = -1;
    ret = takeWorkPCList();

    qDebug()<<"---\t workPCs \t---";
    foreach (ConnClient* value, *workPCList) {
        QList<QString>* ips = value->ips;
        foreach (QString str, *ips) {
            qDebug()<<"ip - "<<str;
        }
    }
    return ret;
}

int ConfHandleWorkImpl::getStationList()
{
    int ret = -1;
    ret = takeStationList();

    return ret;
}

int ConfHandleWorkImpl::getLocalDomainList()
{
    int ret = -1;
    ret = takeGetLocalDomainList();

    qDebug()<<"---\t local domain nodes connInfo \t---";
    foreach (ConnClient* value, *localDomainList) {
        QList<QString>* ips = value->ips;
        foreach (QString str, *ips) {
            qDebug()<<"ip - "<<str;
        }
    }
    return ret;
}

QList<ConnClient *> *ConfHandleWorkImpl::getWorkPCsInfo()
{
    return workPCList;
}

QList<ConnClient *> *ConfHandleWorkImpl::getStationsInfo()
{
    return stationList;
}

QList<ConnClient *> *ConfHandleWorkImpl::getLocalDomainInfo()
{
    return localDomainList;
}

AlarmExtra *ConfHandleWorkImpl::getAlarmExtraByIP(QString ip)
{
    return ferry->getAlarmExtraByIp(ip);
}

int ConfHandleWorkImpl::init()
{
    initParam();

    return 0;
}

int ConfHandleWorkImpl::initParam()
{
    ferry = new ConfDataFerry(this);
    ferry->loadConfData();

    return 0;
}

int ConfHandleWorkImpl::takeStartLevel()
{
    int localDomain = 0;//0 - station,1 - center

    return localDomain;
}

int ConfHandleWorkImpl::takeWorkPCList()
{
    if(NULL == ferry) return -1;

    workPCList = ferry->getWorkPCConnClients();

    return 0;
}

int ConfHandleWorkImpl::takeStationList()
{
    if(NULL == ferry) return -1;

    stationList = ferry->getStationConnClients();

    return 0;
}

int ConfHandleWorkImpl::takeGetLocalDomainList()
{
    if(NULL == ferry) return -1;

    localDomainList = ferry->getLocalDomainConnClients();

    return 0;
}
