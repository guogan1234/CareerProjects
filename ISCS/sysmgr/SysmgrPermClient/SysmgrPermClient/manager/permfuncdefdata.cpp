#include "permfuncdefdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermFuncDefData::PermFuncDefData()
{

}

list<PermFuncDef *> *PermFuncDefData::getInfoByPermIdAndPermType(list<int> *permIds, char *typeName, int *ret)
{
    if(!typeName || !permIds){
        *ret = -1;
        printf("[params is NULL!]\n");
        return NULL;
    }
    char* baseSql = (char*)"select * from conf.perm_func_def where perm_id in(";
    char sql[256] = {0};
    for(auto it = permIds->begin();it != permIds->end();it++){
        if(it == permIds->begin()){
            sprintf(sql,"%s%d",baseSql,*it);
        }else{
            sprintf(sql,"%s,%d",sql,*it);
        }
    }
    char* param_1 = (char*)") and perm_type_name=";
    sprintf(sql,"%s%s'%s'",sql,param_1,typeName);
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

list<PermFuncDef *> *PermFuncDefData::getInfoByPermIdAndPermTypeId(list<int> *permIds, int permTypeId, int *ret)
{
    if(!permIds){
        *ret = -1;
        printf("[params is NULL!]\n");
        return NULL;
    }
    char* baseSql = (char*)"select * from conf.perm_func_def where perm_id in(";
    char sql[256] = {0};
    for(auto it = permIds->begin();it != permIds->end();it++){
        if(it == permIds->begin()){
            sprintf(sql,"%s%d",baseSql,*it);
        }else{
            sprintf(sql,"%s,%d",sql,*it);
        }
    }
    char* param_1 = (char*)") and perm_type_id=";
    sprintf(sql,"%s%s%d",sql,param_1,permTypeId);
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

list<PermFuncDef *> *PermFuncDefData::getInfoByPermIds(list<int> *permIds)
{
    if(!permIds){
        printf("[params is NULL!]\n");
        return NULL;
    }
    char* baseSql = (char*)"select * from conf.perm_func_def where perm_id in(";
    char sql[256] = {0};
    for(auto it = permIds->begin();it != permIds->end();it++){
        if(it == permIds->begin()){
            sprintf(sql,"%s%d",baseSql,*it);
        }else{
            sprintf(sql,"%s,%d",sql,*it);
        }
    }
    char* param_1 = (char*)")";
    sprintf(sql,"%s%s",sql,param_1);
    printf("sql - %s\n",sql);
    int ret = -1;
    return loadDataBySql(sql,&ret);
}

void PermFuncDefData::deleteInfoList(list<PermFuncDef *> *datas)
{
    if(!datas) return;
    printf("[PermFuncDefData]deleteInfoList start...\n");
    list<PermFuncDef*>::const_iterator iter;
    for(iter = datas->begin();iter != datas->end();iter++){
        delete *iter;
        datas->erase(iter);
    }
    delete datas;
    datas = NULL;
    printf("[PermFuncDefData]deleteInfoList end!\n");
}

list<PermFuncDef *> *PermFuncDefData::loadDataBySql(char *sql, int *ret)
{
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        *ret = -1;
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        list<PermFuncDef*> * datas = new list<PermFuncDef*>();
        int count = confdb_recordset_get_count(rs);
        for(int i = 0;i < count; ++i){
            PermFuncDef* temp = new PermFuncDef();
            temp->permId = strdup(confdb_recordset_get(rs,i,"perm_id"));
            temp->permName = strdup(confdb_recordset_get(rs,i,"perm_name"));
            temp->permTypeName = strdup(confdb_recordset_get(rs,i,"perm_type_name"));
            temp->permDefine = strdup(confdb_recordset_get(rs,i,"perm_define"));
            temp->permTypeId = strdup(confdb_recordset_get(rs,i,"perm_type_id"));
            printf("PermFuncDef[permId] - %s\n",temp->permId);
            printf("PermFuncDef[permName] - %s\n",temp->permName);
            printf("PermFuncDef[permTypeName] - %s\n",temp->permTypeName);
            printf("PermFuncDef[permDefine] - %s\n",temp->permDefine);
            printf("PermFuncDef[permTypeId] - %s\n",temp->permTypeId);
            datas->push_back(temp);
        }
        *ret = 0;
        return datas;
    }
}
