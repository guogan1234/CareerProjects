#include "accdata.h"
#include <QDebug>

AccData::AccData()
{

}

QList<Acc *> *AccData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<Acc*> * ret = fillList(rs);

    return ret;
}

char **AccData::getRecordHeaderByCFDB()
{
    return columns;
}

int AccData::getRecordHeaderSizeByCFDB()
{
    return size;
}

ConfDbRecordSet *AccData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<Acc *> *AccData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    confdb_recordset_get_columns(record, &columns, &size);

    QList<Acc*> * ret = new QList<Acc*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        Acc* temp = new Acc();
        temp->accLabel = strdup(confdb_recordset_get(record,i,"acc_label"));
        temp->accName = strdup(confdb_recordset_get(record,i,"acc_name"));
        qDebug("[Acc]accLabel - %s",temp->accLabel);
        qDebug("[Acc]accName - %s",temp->accName);
        ret->append(temp);
    }

    return ret;
}
