#include "permuserdefdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermUserDefData::PermUserDefData()
{

}

list<PermUserDef *> *PermUserDefData::loadAll(int *ret)
{
    char* sql = (char*)"select * from conf.perm_user_def";
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

list<PermUserDef *> *PermUserDefData::loadUserInfoById(int userId, int *ret)
{
    char* baseSql = (char*)"select * from conf.perm_user_def where user_id=";
    char sql[100] = {0};
    sprintf(sql,"%s%d",baseSql,userId);
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

list<PermUserDef *> *PermUserDefData::loadUserInfoByName(const char *userName, int *ret)
{
    char* baseSql = (char*)"select * from conf.perm_user_def where user_name=";
    char sql[100] = {0};
    sprintf(sql,"%s'%s'",baseSql,userName);
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

int PermUserDefData::delUserById(int userId, int *ret)
{
    char* baseSql = (char*)"delete from conf.perm_user_def where user_id=";
    char sql[100] = {0};
    sprintf(sql,"%s%d",baseSql,userId);
    printf("sql - %s\n",sql);
    return simpleExecSql(sql,ret);
}

int PermUserDefData::modifyUserPWD(int userId, const char *password)
{
    char* baseSql = (char*)"update conf.perm_user_def set user_password=";
    char sql[100] = {0};
    sprintf(sql,"%s'%s'",baseSql,password);
    char* param_1 = (char*)" where user_id=";
    sprintf(sql,"%s%s%d",sql,param_1,userId);
    int ret = -1;
    return simpleExecSql(sql,&ret);
}

void PermUserDefData::releaseUserInfo(list<PermUserDef *> *datas)
{

}

bool PermUserDefData::userUnlock(int userId, const char *password)
{
    if(!password) return false;
    char* baseSql = (char*)"select * from conf.perm_user_def where user_id=";
    char sql[100] = {0};
    sprintf(sql,"%s%d",baseSql,userId);
    char* param_1 = (char*)" and user_password=";
    sprintf(sql,"%s%s'%s'",sql,param_1,password);
    int ret = -1;
    list<PermUserDef*> * datas = loadDataBySql(sql,&ret);
    if(!datas) return false;
    if(datas->size() > 0){
        return true;
    }
    return false;
}

bool PermUserDefData::userUnlock(const char *userName, const char *password)
{
    if(!userName || !password) return false;
    char* baseSql = (char*)"select * from conf.perm_user_def where user_name=";
    char sql[100] = {0};
    sprintf(sql,"%s'%s'",baseSql,userName);
    char* param_1 = (char*)" and user_password=";
    sprintf(sql,"%s%s'%s'",sql,param_1,password);
    int ret = -1;
    list<PermUserDef*> * datas = loadDataBySql(sql,&ret);
    if(!datas) return false;
    if(datas->size() > 0){
        return true;
    }
    return false;
}

list<PermUserDef *> *PermUserDefData::loadDataBySql(const char *sql, int *ret)
{
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        *ret = -1;
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        list<PermUserDef*> * datas = new list<PermUserDef*>();
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
            printf("PermUserDef[userID] - %s\n",temp->userID);
            printf("PermUserDef[userName] - %s\n",temp->userName);
            printf("PermUserDef[userAlias] - %s\n",temp->userAlias);
            printf("PermUserDef[userPassword] - %s\n",temp->userPassword);
            printf("PermUserDef[userDesc] - %s\n",temp->userDesc);
            printf("PermUserDef[userIfLock] - %s\n",temp->userIfLock);
            printf("PermUserDef[userLockTime] - %s\n",temp->userLockTime);
            printf("PermUserDef[userExpireDate] - %s\n",temp->userExpireDate);
            printf("PermUserDef[userCreateDate] - %s\n",temp->userCreateDate);
            datas->push_back(temp);
        }
        *ret = 0;
        return datas;
    }
}

int PermUserDefData::simpleExecSql(const char *sql, int *ret)
{
    *ret = confdb_exec(sql);
    return *ret;
}
