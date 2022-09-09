#include "acctpdata.h"
#include <QDebug>

AccTpData::AccTpData()
{

}

QList<AccTp *> *AccTpData::loadDataByCFDB(QString sql)
{
    if(sql.isEmpty()) return NULL;

    ConfDbRecordSet* rs = getRecordByCFDB(sql);
    if(!rs){
        qDebug()<<sql<<"--"<<"result is null!";
    }
    QList<AccTp*> * ret = fillList(rs);

    return ret;
}

ConfDbRecordSet *AccTpData::getRecordByCFDB(QString sql)
{
    return confdb_query(sql.toLocal8Bit().data());
}

QList<AccTp *> *AccTpData::fillList(ConfDbRecordSet *record)
{
    if(!record) return NULL;
    QList<AccTp*> * ret = new QList<AccTp*>();
    int count = confdb_recordset_get_count(record);
    for(int i = 0;i < count; ++i){
        AccTp* temp = new AccTp();
        temp->label = strdup(confdb_recordset_get(record,i,"acc_tp_label"));
        temp->accBase_1 = strdup(confdb_recordset_get(record,i,"acc_base1"));
        temp->accBase_2 = strdup(confdb_recordset_get(record,i,"acc_base2"));
        qDebug("[AccTp]acc_tp_label - %s",temp->label);
        qDebug("[AccTp]acc_base1 - %s",temp->accBase_1);
        qDebug("[AccTp]acc_base2 - %s",temp->accBase_2);
        ret->append(temp);
    }

    return ret;
}
