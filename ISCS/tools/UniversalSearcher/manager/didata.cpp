#include "didata.h"
#include <QDebug>

DiData::DiData()
{

}

QList<Di *> *DiData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<Di*> * ret = fillList(rs);

    return ret;
}

char **DiData::getRecordHeaderByCFDB()
{
    return columns;
}

int DiData::getRecordHeaderSizeByCFDB()
{
    return size;
}

ConfDbRecordSet *DiData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<Di *> *DiData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    confdb_recordset_get_columns(record, &columns, &size);

    QList<Di*> * ret = new QList<Di*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        Di* temp = new Di();
        temp->diLabel = strdup(confdb_recordset_get(record,i,"di_label"));
        temp->diName = strdup(confdb_recordset_get(record,i,"di_name"));
        qDebug("[Di]diLabel - %s",temp->diLabel);
        qDebug("[Di]diName - %s",temp->diName);
        ret->append(temp);
    }

    return ret;
}
