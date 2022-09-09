#include "devtypedata.h"
#include <QDebug>

DevTypeData::DevTypeData()
{

}

QList<DevType *> *DevTypeData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<DevType*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *DevTypeData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<DevType *> *DevTypeData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<DevType*> * ret = new QList<DevType*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        DevType* temp = new DevType();
        temp->label = strdup(confdb_recordset_get(record,i,"dev_type_label"));
        temp->name = strdup(confdb_recordset_get(record,i,"dev_type_name"));
        temp->systemId = strdup(confdb_recordset_get(record,i,"pro_system_id"));
        qDebug("[DevType]label - %s ,name - %s ,systemId - %s",temp->label,temp->name,temp->systemId);
        ret->append(temp);
    }

    return ret;
}
