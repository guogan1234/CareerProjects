#include "pointactivemodel.h"
#include <QDebug>

PointActiveModel::PointActiveModel(QObject *parent) : ActiveListModelBase(parent)
{

}

QString PointActiveModel::conditionsToSQL(QVariantMap *listValues)
{
    QString ret;
    if(listValues->isEmpty()) return ret;
    return ret;
}

void PointActiveModel::fillModelDatas(QString sql)
{
    qDebug()<<"PointActiveModel::fillModelDatas - "<<sql;
}
