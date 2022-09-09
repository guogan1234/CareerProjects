#include "prosystemactivemodel.h"

ProSystemActiveModel::ProSystemActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

ProSystemActiveModel::~ProSystemActiveModel()
{
    qDebug("ProSystemActiveModel::~ProSystemActiveModel");
}

QString ProSystemActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString sql;
    if(NULL == listValues){
        sql = "select * from conf.pro_system_tbl";
    }
    return sql;
}

void ProSystemActiveModel::fillModelDatas(QString sql)
{
    QList<ProSystem*> * data;
    data = getModelDatas(sql);
    fillModel(data);
}

void ProSystemActiveModel::initShow()
{
    QString sql = conditionsToSQL(NULL);
    fillModelDatas(sql);
}

QList<ProSystem *> *ProSystemActiveModel::getModelDatas(QString sql)
{
    return manager->loadDataByCFDB(sql);
}

void ProSystemActiveModel::fillModel(QList<ProSystem *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        ProSystem* temp = data->at(i);
//        QStandardItem* item = new QStandardItem(temp->name);
        QStandardItem* item = new QStandardItem(QString(temp->name) + "-" + QString(temp->id));
        item->setData(temp->id,Qt::UserRole);
        appendRow(item);
    }
}

void ProSystemActiveModel::init()
{
    initParam();
}

void ProSystemActiveModel::initParam()
{
    manager = new ProSystemManager();
}
