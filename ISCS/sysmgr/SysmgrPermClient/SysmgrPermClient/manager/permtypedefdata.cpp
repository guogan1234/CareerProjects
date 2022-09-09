#include "permtypedefdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermTypeDefData::PermTypeDefData()
{

}

char *PermTypeDefData::getPermTypeNameById(int typeId)
{
    char* baseSql = (char*)"select * from conf.perm_type_def where perm_type_id=";
    char sql[100] = {0};
    sprintf(sql,"%s%d\n",baseSql,typeId);
    char* ret = NULL;
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        ret = strdup(confdb_recordset_get(rs,0,"perm_type_name"));
        return ret;
    }
}

void *PermTypeDefData::loadDataBySql(char *sql)
{
    return confdb_query(sql);
}
