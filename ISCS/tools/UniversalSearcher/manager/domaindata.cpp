#include "domaindata.h"
#include <QDebug>

DomainData::DomainData()
{

}

QList<Domain *> * DomainData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

//    ConfDbRecordSet* rs = confdb_query("select * from conf.domain_tbl");
    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<Domain*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *DomainData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<Domain *> *DomainData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<Domain*> * ret = new QList<Domain*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        Domain* temp = new Domain();
        temp->id = strdup(confdb_recordset_get(record,i,"domain_id"));
        temp->label = strdup(confdb_recordset_get(record,i,"domain_label"));
        temp->name = strdup(confdb_recordset_get(record,i,"domain_name"));
        temp->type = strdup(confdb_recordset_get(record,i,"domain_type"));
        qDebug("[Domain]id - %s ,label - %s ,name - %s ,type - %s",temp->id,temp->label,temp->name,temp->type);
        ret->append(temp);
    }

    return ret;
}
