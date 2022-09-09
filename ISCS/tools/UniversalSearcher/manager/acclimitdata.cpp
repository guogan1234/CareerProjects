#include "acclimitdata.h"
#include <QDebug>

AccLimitData::AccLimitData()
{

}

QList<AccLimit *> *AccLimitData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<AccLimit*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *AccLimitData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<AccLimit *> *AccLimitData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<AccLimit*> * ret = new QList<AccLimit*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        AccLimit* temp = new AccLimit();
        temp->label = strdup(confdb_recordset_get(record,i,"acc_limit_label"));
        temp->alarmDelayTime = strdup(confdb_recordset_get(record,i,"alarm_delay_time"));
        temp->alarmLevel = strdup(confdb_recordset_get(record,i,"alarm_level"));
        qDebug("[AccLimit]acc_limit_label - %s",temp->label);
        qDebug("[AccLimit]alarm_delay_time - %s",temp->alarmDelayTime);
        qDebug("[AccLimit]alarm_level - %s",temp->alarmLevel);
        ret->append(temp);
    }

    return ret;
}
