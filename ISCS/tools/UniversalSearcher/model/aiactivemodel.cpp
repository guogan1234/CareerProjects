#include "aiactivemodel.h"
#include <QDebug>

AiActiveModel::AiActiveModel(QObject *parent) : ActiveListModelBase(parent)
{
    init();
}

QString AiActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString ret;
    if(listValues->isEmpty()){
        return ret;
    }
    ret = "select * from conf.ai_tbl where dev_label=";
    QVariant varDevLabel = listValues->value("devLabel");
    if(varDevLabel.canConvert<QString>()){
        QString devLabel = varDevLabel.toString();
        ret = ret + "'" + devLabel + "'";
    }
    qDebug()<<"AiActiveModel::conditionsToSQL -- "<<ret;
    return ret;
}

void AiActiveModel::fillModelDatas(QString sql)
{
    if(sql.isEmpty()) return;
    QList<Ai*> * data = getModelDatas(sql);
    fillModel(data);
}

char **AiActiveModel::getRecordHeaderByCFDB()
{
    return manager->getRecordHeaderByCFDB();
}

int AiActiveModel::getRecordHeaderSizeByCFDB()
{
    return manager->getRecordHeaderSizeByCFDB();
}

Ai *AiActiveModel::getModelDataByIndex(int index)
{
    return modelData->at(index);
}

void AiActiveModel::init()
{
    initParam();
}

void AiActiveModel::initParam()
{
    manager = new AiManager();
    modelData = NULL;
}

QList<Ai *> *AiActiveModel::getModelDatas(QString sql)
{
    modelData = manager->loadDataByCFDB(sql);
    return modelData;
}

void AiActiveModel::fillModel(QList<Ai *> *data)
{
    if(!data) return;
    clear();
    for(int i = 0;i < data->size(); ++i){
        Ai* temp = data->at(i);
        QStandardItem* item = new QStandardItem(temp->aiName);
        item->setData(temp->aiLabel,Qt::UserRole);
        appendRow(item);
    }
}
