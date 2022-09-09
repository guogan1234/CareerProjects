#include "accdefdata.h"
#include <QDebug>

AccDefData::AccDefData()
{

}

QList<AccDef *> *AccDefData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<AccDef*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *AccDefData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<AccDef *> *AccDefData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<AccDef*> * ret = new QList<AccDef*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        AccDef* temp = new AccDef();
        temp->label = strdup(confdb_recordset_get(record,i,"acc_def_label"));
        temp->accBase_1 = strdup(confdb_recordset_get(record,i,"acc_base1"));
        qDebug("[AccDef]acc_def_label - %s",temp->label);
        qDebug("[AccDef]acc_base1 - %s",temp->accBase_1);
        ret->append(temp);
    }

    return ret;
}
