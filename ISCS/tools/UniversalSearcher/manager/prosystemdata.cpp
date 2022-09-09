#include "prosystemdata.h"
#include <QDebug>

ProSystemData::ProSystemData()
{

}

QList<ProSystem *> *ProSystemData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<ProSystem*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *ProSystemData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<ProSystem *> *ProSystemData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<ProSystem*> * ret = new QList<ProSystem*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        ProSystem* temp = new ProSystem();
        temp->id = strdup(confdb_recordset_get(record,i,"pro_system_id"));
        temp->label = strdup(confdb_recordset_get(record,i,"pro_system_label"));
        temp->name = strdup(confdb_recordset_get(record,i,"pro_system_name"));
        qDebug("[ProSystem]id - %s ,label - %s ,name - %s",temp->id,temp->label,temp->name);
        ret->append(temp);
    }

    return ret;
}
