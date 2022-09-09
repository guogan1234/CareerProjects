#include "devtypeactivemodel.h"
#include <QDebug>

DevTypeActiveModel::DevTypeActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

DevTypeActiveModel::~DevTypeActiveModel()
{
    qDebug("DevTypeActiveModel::~DevTypeActiveModel");
}

QString DevTypeActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString sql;
    sql = "select * from conf.dev_type_tbl";
    if(NULL == listValues){
        return sql;
    }else {
        QVariant varSystemId = listValues->value("systemId");
        if(varSystemId.canConvert<QString>()){
            QString systemId = varSystemId.toString();
            sql += " where pro_system_id=";
            sql += systemId;
        }
    }
    qDebug()<<"DevTypeActiveModel::conditionsToSQL -- "<<sql;
    return sql;
}

void DevTypeActiveModel::fillModelDatas(QString sql)
{
    QList<DevType*> * data;
    data = getModelDatas(sql);
    fillModel(data);
}

void DevTypeActiveModel::initShow()
{
    QString sql = conditionsToSQL(NULL);
    fillModelDatas(sql);
}

QList<DevType *> *DevTypeActiveModel::getModelDatas(QString sql)
{
    return manager->loadDataByCFDB(sql);
}

void DevTypeActiveModel::fillModel(QList<DevType *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        DevType* temp = data->at(i);
//        QStandardItem* item = new QStandardItem(temp->name);
        QStandardItem* item = new QStandardItem(QString(temp->name) + "-" + QString(temp->label));
        item->setData(temp->label,Qt::UserRole);
        appendRow(item);
    }
}

void DevTypeActiveModel::init()
{
    initParam();
}

void DevTypeActiveModel::initParam()
{
    manager = new DevTypeManager();
}
