#include "permuserdefdata.h"
#include "confcmn.h"
#include <QDebug>

PermUserDefData::PermUserDefData()
{

}

QList<PermUserDef *> *PermUserDefData::loadAll(int *ret)
{
//    char* chSql = "select * from conf.perm_user_def";
    QString sql = "select * from conf.perm_user_def";
    qDebug()<<"sql - "<<sql;
    return loadDataBySql(sql,ret);
}

QList<PermUserDef *> *PermUserDefData::loadUserInfoById(int userId, int *ret)
{
  QString sql = QString("select * from conf.perm_user_def where user_id=%1").arg(userId);
  qDebug()<<"sql - "<<sql;
  return loadDataBySql(sql,ret);
}

QList<PermUserDef *> *PermUserDefData::loadUserInfoByName(const char *userName, int *ret)
{
    QString sql = QString("select * from conf.perm_user_def where user_name='%1'").arg(userName);
    qDebug()<<"sql - "<<sql;
    return loadDataBySql(sql,ret);
}

QList<PermUserDef *> *PermUserDefData::loadUserInfoByIdBatch(QList<int> *userIds, int *ret)
{
    return NULL;
}

int PermUserDefData::delUserById(int userId, int *ret)
{
    return 0;
}

void PermUserDefData::releaseUserInfo(QList<PermUserDef *> *datas)
{
    int len = datas->length();
//    for(int i = 0;i < len; ++i){
//        PermUserDef *temp = datas->at(i);
//        if(temp){
//            delete temp->userID;
//            delete temp->userName;
//            delete temp->userAlias;
//            delete temp->userPassword;
//            delete temp->userDesc;
//            delete temp->userIfLock;
//            delete temp->userLockTime;
//            delete temp->userExpireDate;
//            delete temp->userCreateDate;
//        }
//        delete temp;
//    }
//    delete datas;
    return;
}

QList<PermUserDef *> *PermUserDefData::loadDataBySql(QString sql, int *ret)
{
  ConfDbRecordSet* rs = confdb_query(sql.toLocal8Bit().data());
  if(!rs){
      *ret = -1;
      qDebug()<<"record is NULL!"<<" sql- "<<sql.toLocal8Bit().data();
      return NULL;
  }
  else{
      QList<PermUserDef*> * datas = new QList<PermUserDef*>();
      int count = confdb_recordset_get_count(rs);
      for(int i = 0;i < count; ++i){
          PermUserDef* temp = new PermUserDef();
          temp->userID = strdup(confdb_recordset_get(rs,i,"user_id"));
          temp->userName = strdup(confdb_recordset_get(rs,i,"user_name"));
          temp->userAlias = strdup(confdb_recordset_get(rs,i,"user_alias"));
          temp->userPassword = strdup(confdb_recordset_get(rs,i,"user_password"));
          temp->userDesc = strdup(confdb_recordset_get(rs,i,"user_desc"));
          temp->userIfLock = strdup(confdb_recordset_get(rs,i,"user_if_lock"));
          temp->userLockTime = strdup(confdb_recordset_get(rs,i,"user_lock_time"));
          temp->userExpireDate = strdup(confdb_recordset_get(rs,i,"user_expire_date"));
          temp->userCreateDate = strdup(confdb_recordset_get(rs,i,"user_create_date"));
          qDebug()<<"PermUserDef[userID] - "<<temp->userID;
          qDebug()<<"PermUserDef[userName] - "<<temp->userName;
          qDebug()<<"PermUserDef[userAlias] - "<<temp->userAlias;
          qDebug()<<"PermUserDef[userPassword] - "<<temp->userPassword;
          qDebug()<<"PermUserDef[userDesc] - "<<temp->userDesc;
          qDebug()<<"PermUserDef[userIfLock] - "<<temp->userIfLock;
          qDebug()<<"PermUserDef[userLockTime] - "<<temp->userLockTime;
          qDebug()<<"PermUserDef[userExpireDate] - "<<temp->userExpireDate;
          qDebug()<<"PermUserDef[userCreateDate] - "<<temp->userCreateDate;
          datas->append(temp);
        }
      return datas;
    }
}
