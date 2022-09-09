#include "aidata.h"
#include <QDebug>

AiData::AiData()
{

}

QList<Ai *> *AiData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<Ai*> * ret = fillList(rs);

    return ret;
}

char **AiData::getRecordHeaderByCFDB()
{
    return columns;
}

int AiData::getRecordHeaderSizeByCFDB()
{
    return size;
}

ConfDbRecordSet *AiData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<Ai *> *AiData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    confdb_recordset_get_columns(record, &columns, &size);

    QList<Ai*> * ret = new QList<Ai*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        Ai* temp = new Ai();
        temp->aiLabel = strdup(confdb_recordset_get(record,i,"ai_label"));
        temp->aiName = strdup(confdb_recordset_get(record,i,"ai_name"));
        qDebug("[Ai]aiLabel - %s",temp->aiLabel);
        qDebug("[Ai]aiName - %s",temp->aiName);
        ret->append(temp);
    }

    return ret;
}
