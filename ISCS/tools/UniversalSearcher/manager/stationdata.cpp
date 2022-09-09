#include "stationdata.h"
#include <QDebug>

StationData::StationData()
{

}

QList<Station *> *StationData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<Station*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *StationData::getRecordByCFDB(QString sql)
{
    qDebug()<<"getRecordByCFDB -- "<<sql;
    return confdb_query(sql.toLocal8Bit().data());
//    return confdb_query("select * from conf.station_tbl where domain_id=99");
}

QList<Station *> *StationData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<Station*> * ret = new QList<Station*>();
    int count = confdb_recordset_get_count(record);
    qDebug()<<"[Station]result length - "<<count;
    for(int i = 0;i < count; ++i){
        Station* temp = new Station();
        temp->id = strdup(confdb_recordset_get(record,i,"station_id"));
        temp->label = strdup(confdb_recordset_get(record,i,"station_label"));
        temp->name = strdup(confdb_recordset_get(record,i,"station_name"));
        temp->pid = strdup(confdb_recordset_get(record,i,"station_pid"));
        temp->domainId = strdup(confdb_recordset_get(record,i,"domain_id"));
        qDebug("[Station]id - %s ,label - %s ,name - %s",temp->id,temp->label,temp->name);
        qDebug("[Station]pid - %s ,domainId - %s",temp->pid,temp->domainId);
        ret->append(temp);
    }

    return ret;
}
