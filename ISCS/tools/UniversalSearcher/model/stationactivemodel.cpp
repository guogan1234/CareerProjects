#include "stationactivemodel.h"
#include <QDebug>

StationActiveModel::StationActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

StationActiveModel::~StationActiveModel()
{
    qDebug("StationActiveModel::~StationActiveModel");
}

QString StationActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString ret;
    if(listValues->isEmpty()){
        return ret;
    }
    ret = "select * from conf.station_tbl where domain_id=";
    QVariant varDomainId = listValues->value("domainId");
    if(varDomainId.canConvert<QString>()){
        QString domainId = varDomainId.toString();
        ret += domainId;
    }
    qDebug()<<"StationActiveModel::conditionsToSQL -- "<<ret;
    return ret;
}

void StationActiveModel::fillModelDatas(QString sql)
{
    if(sql.isEmpty()) return;
    QList<Station*> * data = getModelDatas(sql);
    fillModel(data);
}

void StationActiveModel::init()
{
    initParam();
}

void StationActiveModel::initParam()
{
    manager = new StationManager();
}

QList<Station *> *StationActiveModel::getModelDatas(QString sql)
{
    return manager->loadDataByCFDB(sql);
}

void StationActiveModel::fillModel(QList<Station *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        Station* temp = data->at(i);
//        QStandardItem* item = new QStandardItem(temp->name);
        QStandardItem* item = new QStandardItem(QString(temp->name) + "-" + QString(temp->id));
        item->setData(temp->id,Qt::UserRole);
        appendRow(item);
    }
}
