#include "domaindata.h"

DomainData::DomainData()
{

}

QList<Domain *> * DomainData::loadDataByCFDB(QString sql)
{
//    if(sql.isEmpty()) return NULL;

    confdb_init();

    ConfDbRecordSet* rs = confdb_query("select * from conf.domain_tbl");
    QList<Domain*> * ret = fillList(rs);

    confdb_release();
    return ret;
}

QList<Domain *> *DomainData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<Domain*> * ret = new QList<Domain*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        Domain* domain = new Domain();
        domain->id = strdup(confdb_recordset_get(record,i,"domain_id"));
        domain->label = strdup(confdb_recordset_get(record,i,"domain_label"));
        domain->name = strdup(confdb_recordset_get(record,i,"domain_name"));
        domain->type = strdup(confdb_recordset_get(record,i,"domain_type"));
        qDebug("id - %s ,label - %s ,name - %s ,type - %s",domain->id,domain->label,domain->name,domain->type);
        ret->append(domain);
    }

    return ret;
}
