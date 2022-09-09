#include "diactivemodel.h"
#include <QDebug>

DiActiveModel::DiActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

QString DiActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString ret;
    if(listValues->isEmpty()){
        return ret;
    }
    ret = "select * from conf.di_tbl where dev_label=";
    QVariant varDevLabel = listValues->value("devLabel");
    if(varDevLabel.canConvert<QString>()){
        QString devLabel = varDevLabel.toString();
        ret = ret + "'" + devLabel + "'";
    }
    qDebug()<<"DiActiveModel::conditionsToSQL -- "<<ret;
    return ret;
}

void DiActiveModel::fillModelDatas(QString sql)
{
    if(sql.isEmpty()) return;
    QList<Di*> * data = getModelDatas(sql);
    fillModel(data);
}

char **DiActiveModel::getRecordHeaderByCFDB()
{
    return manager->getRecordHeaderByCFDB();
}

int DiActiveModel::getRecordHeaderSizeByCFDB()
{
    return manager->getRecordHeaderSizeByCFDB();
}

Di *DiActiveModel::getModelDataByIndex(int index)
{
    return modelData->at(index);
}

void DiActiveModel::init()
{
    initParam();
}

void DiActiveModel::initParam()
{
    manager = new DiManager();
}

QList<Di *> *DiActiveModel::getModelDatas(QString sql)
{
    modelData = manager->loadDataByCFDB(sql);
    return modelData;
}

void DiActiveModel::fillModel(QList<Di *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        Di* temp = data->at(i);
        QStandardItem* item = new QStandardItem(temp->diName);
        item->setData(temp->diLabel,Qt::UserRole);
        appendRow(item);
    }
}
