#include "permsecuritydefdata.h"
#include "confcmn.h"
#include <QDebug>

PermSecurityDefData::PermSecurityDefData()
{

}

PermSecurityDef *PermSecurityDefData::getPermSecurityInfo(int *ret)
{
    QString sql = "select * from conf.perm_security_def";
    qDebug()<<"sql - "<<sql;
    return getDataBySql(sql,ret);
}

PermSecurityDef *PermSecurityDefData::getDataBySql(QString sql, int *ret)
{
    ConfDbRecordSet* rs = confdb_query(sql.toLocal8Bit().data());
    if(!rs){
        *ret = -1;
        qDebug()<<"record is NULL!"<<" sql- "<<sql.toLocal8Bit().data();
        return NULL;
    }
    else{
        PermSecurityDef * data = new PermSecurityDef();
        data->permSecurityId = strdup(confdb_recordset_get(rs,0,"perm_security_id"));
        data->permPWDMinLength = strdup(confdb_recordset_get(rs,0,"perm_password_min_length"));
        data->permPWDMinLetter = strdup(confdb_recordset_get(rs,0,"perm_password_min_letr"));
        data->permPWDMinNumber = strdup(confdb_recordset_get(rs,0,"perm_password_min_num"));
        data->permAccountMaxLock = strdup(confdb_recordset_get(rs,0,"perm_account_max_lock"));
        data->permAccountLockTime = strdup(confdb_recordset_get(rs,0,"perm_account_lock_timer"));
        data->permAccountInvalidTime = strdup(confdb_recordset_get(rs,0,"perm_account_invalid_timer"));
        qDebug()<<"PermSecurityDef[permSecurityId] - "<<data->permSecurityId;
        qDebug()<<"PermSecurityDef[permPWDMinLength] - "<<data->permPWDMinLength;
        qDebug()<<"PermSecurityDef[permPWDMinLetter] - "<<data->permPWDMinLetter;
        qDebug()<<"PermSecurityDef[permPWDMinNumber] - "<<data->permPWDMinNumber;
        qDebug()<<"PermSecurityDef[permAccountMaxLock] - "<<data->permAccountMaxLock;
        qDebug()<<"PermSecurityDef[permAccountLockTime] - "<<data->permAccountLockTime;
        qDebug()<<"PermSecurityDef[permAccountInvalidTime] - "<<data->permAccountInvalidTime;
        return data;
      }
}
