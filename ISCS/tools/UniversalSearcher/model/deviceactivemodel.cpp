#include "deviceactivemodel.h"
#include <QDebug>

DeviceActiveModel::DeviceActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

QString DeviceActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString ret;
    if(listValues->isEmpty()){
        return ret;
    }
    ret = "select * from conf.dev_tbl where";
    QVariant varDomainId = listValues->value("domainId");
    if(varDomainId.canConvert<QString>()){
        QString domainId = varDomainId.toString();
        ret = ret + " domain_id=" + domainId;
    }
    QVariant varStationId = listValues->value("stationId");
    if(varStationId.canConvert<QString>()){
        QString stationId = varStationId.toString();
        ret = ret + " and station_id=" + stationId;
    }
    QVariant varProSystemId = listValues->value("proSystemId");
    if(varProSystemId.canConvert<QString>()){
        QString proSystemId = varProSystemId.toString();
        ret = ret + " and pro_system_id=" + proSystemId;
    }
    QVariant varDevTypeLabel = listValues->value("devTypeLabel");
    if(varDevTypeLabel.canConvert<QString>()){
        QString devTypeLabel = varDevTypeLabel.toString();
        ret = ret + " and dev_type_label='" + devTypeLabel + "'";
    }
    qDebug()<<"DeviceActiveModel::conditionsToSQL -- "<<ret;
    return ret;
}

void DeviceActiveModel::fillModelDatas(QString sql)
{
    if(sql.isEmpty()) return;
    QList<Dev*> * data = getModelDatas(sql);
    fillModel(data);
}

void DeviceActiveModel::init()
{
    initParam();
}

void DeviceActiveModel::initParam()
{
    manager = new DevManager();
}

QList<Dev *> *DeviceActiveModel::getModelDatas(QString sql)
{
    return manager->loadDataByCFDB(sql);
}

void DeviceActiveModel::fillModel(QList<Dev *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        Dev* temp = data->at(i);
        QStandardItem* item = new QStandardItem(temp->devName);
        item->setData(temp->devLabel,Qt::UserRole);
        appendRow(item);
    }
}
