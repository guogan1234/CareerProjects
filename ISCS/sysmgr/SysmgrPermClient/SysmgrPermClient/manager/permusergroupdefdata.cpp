#include "permusergroupdefdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermUserGroupDefData::PermUserGroupDefData()
{

}

list<int> *PermUserGroupDefData::getGroupIdsByUserId(int userId)
{
    char* baseSql = (char*)"select distinct group_id from conf.perm_user_group_def where user_id=";
    char sql[100] = {0};
    sprintf(sql,"%s%d",baseSql,userId);
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        list<int> * datas = new list<int>();
        int count = confdb_recordset_get_count(rs);
        for(int i = 0;i < count; ++i){
            int temp;
            sscanf(confdb_recordset_get(rs,i,"group_id"),"%d",&temp);
            datas->push_back(temp);
        }
        return datas;
    }
}

int PermUserGroupDefData::delUserGroupDefByUserId(int userId)
{
    char* baseSql = (char*)"delete from conf.perm_user_group_def where user_id=";
    char sql[100] = {0};
    sprintf(sql,"%s%d",baseSql,userId);
    printf("sql - %s\n",sql);
    int ret = -1;
    return simpleExecSql(sql,&ret);
}

int PermUserGroupDefData::simpleExecSql(const char *sql, int *ret)
{
    *ret = confdb_exec(sql);
    return *ret;
}
