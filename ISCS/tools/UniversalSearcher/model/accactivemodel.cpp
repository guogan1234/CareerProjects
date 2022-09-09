#include "accactivemodel.h"
#include <QDebug>

AccActiveModel::AccActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

QString AccActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString ret;
    if(listValues->isEmpty()){
        return ret;
    }
    ret = "select * from conf.acc_tbl where dev_label=";
    QVariant varDevLabel = listValues->value("devLabel");
    if(varDevLabel.canConvert<QString>()){
        QString devLabel = varDevLabel.toString();
        ret = ret + "'" + devLabel + "'";
    }
    qDebug()<<"AccActiveModel::conditionsToSQL -- "<<ret;
    return ret;
}

void AccActiveModel::fillModelDatas(QString sql)
{
    if(sql.isEmpty()) return;
    QList<Acc*> * data = getModelDatas(sql);
    fillModel(data);
}

char **AccActiveModel::getRecordHeaderByCFDB()
{
    return manager->getRecordHeaderByCFDB();
}

int AccActiveModel::getRecordHeaderSizeByCFDB()
{
    return manager->getRecordHeaderSizeByCFDB();
}

Acc *AccActiveModel::getModelDataByIndex(int index)
{
    return modelData->at(index);
}

void AccActiveModel::init()
{
    initParam();
}

void AccActiveModel::initParam()
{
    manager = new AccManager();
    modelData = NULL;
}

QList<Acc *> *AccActiveModel::getModelDatas(QString sql)
{
    modelData = manager->loadDataByCFDB(sql);
    return modelData;
}

void AccActiveModel::fillModel(QList<Acc *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        Acc* temp = data->at(i);
        QStandardItem* item = new QStandardItem(temp->accName);
        item->setData(temp->accLabel,Qt::UserRole);
        appendRow(item);
    }
}
