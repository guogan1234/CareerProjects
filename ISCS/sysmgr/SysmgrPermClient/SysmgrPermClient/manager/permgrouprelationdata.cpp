#include "permgrouprelationdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermGroupRelationData::PermGroupRelationData()
{

}

list<int> *PermGroupRelationData::getPermIdsByGroupId(list<int> *groupIds)
{
    if(!groupIds){
        printf("[params is NULL!]\n");
        return NULL;
    }
    char* baseSql = (char*)"select * from conf.perm_group_realation where group_id in(";
    char sql[256] = {0};
    for(auto it = groupIds->begin();it != groupIds->end();it++){
        if(it == groupIds->begin()){
            sprintf(sql,"%s%d",baseSql,*it);
        }else{
            sprintf(sql,"%s,%d",sql,*it);
        }
    }
    char* param_1 = (char*)")";
    sprintf(sql,"%s%s",sql,param_1);
    printf("sql - %s\n",sql);

    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        list<int> * datas = new list<int>();
        int count = confdb_recordset_get_count(rs);
        for(int i = 0;i < count; ++i){
            int temp;
            sscanf(confdb_recordset_get(rs,i,"perm_id"),"%d",&temp);
            datas->push_back(temp);
        }
        return datas;
    }
}

list<PermGroupRelation *> *PermGroupRelationData::getRecordsByGroupIds(list<int> *groupIds)
{
    return NULL;
}

int PermGroupRelationData::checkPermByGroupIdAndPermId(int groupId, int permId)
{
    char* baseSql = (char*)"select * from conf.perm_group_realation where group_id=";
    char sql[256] = {0};
    sprintf(sql,"%s%d",baseSql,groupId);
    char* param_1 = (char*)" and perm_id=";
    sprintf(sql,"%s%s%d",sql,param_1,permId);
    printf("sql - %s\n",sql);

    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        printf("[record is NULL!]sql - %s\n",sql);
        return PERM_ERROR;
    }else{
        int count = confdb_recordset_get_count(rs);
        if(count > 0){
            return PERM_PERMIT;
        }else{
            return PERM_FORBIT;
        }
    }
}

int PermGroupRelationData::checkPermByGroupIdAndPermDefine(int groupId, const char *permDefine)
{
    char* baseSql = (char*)"select * from conf.perm_group_realation where group_id=";
    char sql[256] = {0};
    sprintf(sql,"%s%d",baseSql,groupId);
    char* param_1 = (char*)" and perm_define=";
    sprintf(sql,"%s%s'%s'",sql,param_1,permDefine);
    printf("sql - %s\n",sql);

    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        printf("[record is NULL!]sql - %s\n",sql);
        return PERM_ERROR;
    }else{
        int count = confdb_recordset_get_count(rs);
        if(count > 0){
            return PERM_PERMIT;
        }else{
            return PERM_FORBIT;
        }
    }
}

int PermGroupRelationData::loadData()
{
    char* sql = (char*)"select * from conf.perm_group_realation";
    printf("sql - %s\n",sql);
    int ret = -1;
    data = loadDataBySql(sql,&ret);
    return ret;
}

int PermGroupRelationData::loadDataByGroupId(int groupId)
{
    char* sql = (char*)"select * from conf.perm_group_realation where group_id=";
    sprintf(sql,"%s%d",sql,groupId);
    printf("sql - %s\n",sql);
    int ret = -1;
    data = loadDataBySql(sql,&ret);
    return ret;
}

int PermGroupRelationData::checkPermByPermId(int permId)
{
    list<PermGroupRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermGroupRelation* temp = *iter;
        int tempPermId;
        sscanf(temp->permId,"%d",&tempPermId);
        if(permId == tempPermId){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

int PermGroupRelationData::checkPermByPermDefine(const char *permDefine)
{
    list<PermGroupRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermGroupRelation* temp = *iter;
        if(!strcmp(permDefine,temp->permDefine)){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

list<PermGroupRelation *> *PermGroupRelationData::loadDataBySql(const char *sql, int *ret)
{
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        printf("[record is NULL!]sql - %s\n",sql);
        *ret = -1;
        return NULL;
    }else{
        list<PermGroupRelation*> * datas = new list<PermGroupRelation*>();
        int count = confdb_recordset_get_count(rs);
        for(int i = 0;i < count; ++i){
            PermGroupRelation* temp = new PermGroupRelation();
            temp->relationId = strdup(confdb_recordset_get(rs,i,"relation_id"));
            temp->groupId = strdup(confdb_recordset_get(rs,i,"group_id"));
            temp->permId = strdup(confdb_recordset_get(rs,i,"perm_id"));
            temp->permDefine = strdup(confdb_recordset_get(rs,i,"perm_define"));
            printf("PermGroupRelation[relationId] - %s\n",temp->relationId);
            printf("PermGroupRelation[groupId] - %s\n",temp->groupId);
            printf("PermGroupRelation[permId] - %s\n",temp->permId);
            printf("PermGroupRelation[permDefine] - %s\n",temp->permDefine);
            datas->push_back(temp);
        }
        *ret = 0;
        return datas;
    }
}
