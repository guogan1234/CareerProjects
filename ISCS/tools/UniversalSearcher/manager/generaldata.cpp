#include "generaldata.h"
#include <QDebug>

GeneralData::GeneralData()
{

}

QList<QVariantMap *> *GeneralData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<QVariantMap*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *GeneralData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<QVariantMap *> *GeneralData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<QVariantMap*> * ret = new QList<QVariantMap*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
//        Domain* domain = new Domain();
//        domain->id = strdup(confdb_recordset_get(record,i,"domain_id"));
//        domain->label = strdup(confdb_recordset_get(record,i,"domain_label"));
//        domain->name = strdup(confdb_recordset_get(record,i,"domain_name"));
//        domain->type = strdup(confdb_recordset_get(record,i,"domain_type"));
    }
    return ret;
}
