#include "devdata.h"
#include <QDebug>

DevData::DevData()
{

}

QList<Dev *> *DevData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<Dev*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *DevData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<Dev *> *DevData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<Dev*> * ret = new QList<Dev*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        Dev* temp = new Dev();
        temp->devLabel = strdup(confdb_recordset_get(record,i,"dev_label"));
        temp->devName = strdup(confdb_recordset_get(record,i,"dev_name"));
        qDebug("[Dev]devLabel - %s",temp->devLabel);
        qDebug("[Dev]devName - %s",temp->devName);
        ret->append(temp);
    }

    return ret;
}
