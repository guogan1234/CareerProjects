#include "domainactivemodel.h"

DomainActiveModel::DomainActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

DomainActiveModel::~DomainActiveModel()
{
    qDebug("DomainActiveModel::~DomainActiveModel");
}

QString DomainActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString sql;
    if(NULL == listValues){
        sql = "select * from conf.domain_tbl";
    }
    return sql;
}

void DomainActiveModel::fillModelDatas(QString sql)
{
    QList<Domain*> * data;
    data = getModelDatas(sql);
    fillModel(data);
}

void DomainActiveModel::initShow()
{
    QString sql = conditionsToSQL(NULL);
    fillModelDatas(sql);
}

QList<Domain *> *DomainActiveModel::getModelDatas(QString sql)
{
    return manager->loadDataByCFDB(sql);
}

void DomainActiveModel::fillModel(QList<Domain *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        Domain* temp = data->at(i);
//        QStandardItem* item = new QStandardItem(temp->name);
        QStandardItem* item = new QStandardItem(QString(temp->name) + "-" + QString(temp->id));
        item->setData(temp->id,Qt::UserRole);
        appendRow(item);
    }
}

void DomainActiveModel::init()
{
    initParam();
}

void DomainActiveModel::initParam()
{
    manager = new DomainManager();
}
