#include "permgroupdefdata.h"
#include "confcmn.h"
#include <QDebug>

PermGroupDefData::PermGroupDefData()
{

}

QList<PermGroupDef *> *PermGroupDefData::loadAll(int *ret)
{
    QString sql = "select * from conf.perm_group_def";
    qDebug()<<"sql - "<<sql;
    return loadDataBySql(sql,ret);
}

QList<PermGroupDef *> *PermGroupDefData::loadGroupInfoById(int groupId, int *ret)
{
    QString sql = QString("select * from conf.perm_group_def where group_id=%1").arg(groupId);
    qDebug()<<"sql - "<<sql;
    return loadDataBySql(sql,ret);
}

QList<PermGroupDef *> *PermGroupDefData::loadGroupInfoByName(const char *groupName, int *ret)
{
    QString sql = QString("select * from conf.perm_group_def where group_name='%1'").arg(groupName);
    qDebug()<<"sql - "<<sql;
    return loadDataBySql(sql,ret);
}

void PermGroupDefData::releaseGroupInfo(QList<PermGroupDef *> *datas)
{
    return;
}

QList<PermGroupDef *> *PermGroupDefData::loadDataBySql(QString sql, int *ret)
{
    ConfDbRecordSet* rs = confdb_query(sql.toLocal8Bit().data());
    if(!rs){
        *ret = -1;
        qDebug()<<"record is NULL!"<<" sql- "<<sql.toLocal8Bit().data();
        return NULL;
    }
    else{
        QList<PermGroupDef*> * datas = new QList<PermGroupDef*>();
        int count = confdb_recordset_get_count(rs);
        for(int i = 0;i < count; ++i){
            PermGroupDef* temp = new PermGroupDef();
            temp->groupId = strdup(confdb_recordset_get(rs,i,"group_id"));
            temp->groupName = strdup(confdb_recordset_get(rs,i,"group_name"));
            temp->groupLevel = strdup(confdb_recordset_get(rs,i,"group_level"));
            temp->groupDesc = strdup(confdb_recordset_get(rs,i,"group_desc"));
            temp->groupPic1 = strdup(confdb_recordset_get(rs,i,"group_pic1"));
            temp->groupPic2 = strdup(confdb_recordset_get(rs,i,"group_pic2"));
            temp->groupPic3 = strdup(confdb_recordset_get(rs,i,"group_pic3"));
            qDebug()<<"PermGroupDef[groupId] - "<<temp->groupId;
            qDebug()<<"PermGroupDef[groupName] - "<<temp->groupName;
            qDebug()<<"PermGroupDef[groupLevel] - "<<temp->groupLevel;
            qDebug()<<"PermGroupDef[groupDesc] - "<<temp->groupDesc;
            qDebug()<<"PermGroupDef[groupPic1] - "<<temp->groupPic1;
            qDebug()<<"PermGroupDef[groupPic2] - "<<temp->groupPic2;
            qDebug()<<"PermGroupDef[groupPic3] - "<<temp->groupPic3;
            datas->append(temp);
          }
        return datas;
      }
}
