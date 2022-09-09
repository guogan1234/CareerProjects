#include "permspecgroupregiondata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermSpecGroupRegionData::PermSpecGroupRegionData()
{

}

list<PermSpecGroupRegionRelation *> *PermSpecGroupRegionData::getPermSpecRegionsByGroupIds(list<int> *groupIds)
{
    if(!groupIds){
        printf("[params is NULL!]\n");
        return NULL;
    }
    char* baseSql = (char*)"select * from conf.perm_spec_group_region_realation where group_id in(";
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
    int ret = -1;
    return loadDataBySql(sql,&ret);
}

list<PermSpecGroupRegionRelation *> *PermSpecGroupRegionData::getPermSpecRegionsByGroupId(int groupId)
{
    char* baseSql = (char*)"select * from conf.perm_spec_group_region_realation where group_id=";
    char sql[256] = {0};
    sprintf(sql,"%s%d",baseSql,groupId);
    printf("sql - %s\n",sql);
    int ret = -1;
    return loadDataBySql(sql,&ret);
}

list<PermSpecGroupRegionRelation *> *PermSpecGroupRegionData::getPermSpecRegionsByPermId(int permId)
{
    char* baseSql = (char*)"select * from conf.perm_spec_group_region_realation where perm_id=";
    char sql[256] = {0};
    sprintf(sql,"%s%d",baseSql,permId);
    printf("sql - %s\n",sql);
    int ret = -1;
    return loadDataBySql(sql,&ret);
}

int PermSpecGroupRegionData::checkPermByGroupIdAndPermId(int groupId, int permId)
{
    char* baseSql = (char*)"select * from conf.perm_spec_group_region_realation where group_id=";
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

int PermSpecGroupRegionData::checkPermByGroupIdAndPermDefine(int groupId, const char *permDefine)
{
    if(!permDefine) return PERM_ERROR;
    char* baseSql = (char*)"select * from conf.perm_spec_group_region_realation where group_id=";
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

int PermSpecGroupRegionData::loadData()
{
    char* sql = (char*)"select * from conf.perm_spec_group_region_realation";
    printf("sql - %s\n",sql);
    int ret = -1;
    data = loadDataBySql(sql,&ret);
    return ret;
}

int PermSpecGroupRegionData::loadDataByGroupId(int groupId)
{
    char* sql = (char*)"select * from conf.perm_spec_group_region_realation where group_id=";
    sprintf(sql,"%s%d",sql,groupId);
    printf("sql - %s\n",sql);
    int ret = -1;
    data = loadDataBySql(sql,&ret);
    return ret;
}

int PermSpecGroupRegionData::checkPermByPermId(int permId)
{
    list<PermSpecGroupRegionRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupRegionRelation* temp = *iter;
        int tempPermId;
        sscanf(temp->permId,"%d",&tempPermId);
        if(permId == tempPermId){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

int PermSpecGroupRegionData::checkPermByPermIdAndRegionId(int permId, int regionId)
{
    list<PermSpecGroupRegionRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupRegionRelation* temp = *iter;
        int tempPermId;
        int tempRegionId;
        sscanf(temp->permId,"%d",&tempPermId);
        sscanf(temp->regionId,"%d",&tempRegionId);
        if(permId == tempPermId && regionId == tempRegionId){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

int PermSpecGroupRegionData::checkPermByPermDefine(const char *permDefine)
{
    if(!permDefine) return PERM_ERROR;
    list<PermSpecGroupRegionRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupRegionRelation* temp = *iter;
        if(!strcmp(permDefine,temp->permDefine)){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

int PermSpecGroupRegionData::checkPermByPermDefineAndRegionId(const char *permDefine, int regionId)
{
    if(!permDefine) return PERM_ERROR;
    list<PermSpecGroupRegionRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupRegionRelation* temp = *iter;
        int tempRegionId;
        sscanf(temp->regionId,"%d",&tempRegionId);
        if(!strcmp(permDefine,temp->permDefine) && regionId == tempRegionId){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

void PermSpecGroupRegionData::deleteInfoList(list<PermSpecGroupRegionRelation *> *datas)
{
    if(!datas) return;
    printf("[PermSpecGroupRegionData]deleteInfoList start...\n");
    list<PermSpecGroupRegionRelation*>::const_iterator iter;
    for(iter = datas->begin();iter != datas->end();iter++){
        delete *iter;
        datas->erase(iter);
    }
    delete datas;
    datas = NULL;
    printf("[PermSpecGroupRegionData]deleteInfoList end!\n");
}

list<PermSpecGroupRegionRelation *> *PermSpecGroupRegionData::loadDataBySql(const char *sql, int *ret)
{
    if(!sql || !ret) return NULL;
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        *ret = -1;
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        list<PermSpecGroupRegionRelation*> * datas = new list<PermSpecGroupRegionRelation*>();
        int count = confdb_recordset_get_count(rs);
        for(int i = 0;i < count; ++i){
            PermSpecGroupRegionRelation* temp = new PermSpecGroupRegionRelation();
            temp->relationId = strdup(confdb_recordset_get(rs,i,"relation_id"));
            temp->groupId = strdup(confdb_recordset_get(rs,i,"group_id"));
            temp->permId = strdup(confdb_recordset_get(rs,i,"perm_id"));
            temp->regionId = strdup(confdb_recordset_get(rs,i,"region_id"));
            temp->permDefine = strdup(confdb_recordset_get(rs,i,"perm_define"));
            printf("PermSpecGroupRegionRelation[relationId] - %s\n",temp->relationId);
            printf("PermSpecGroupRegionRelation[groupId] - %s\n",temp->groupId);
            printf("PermSpecGroupRegionRelation[permId] - %s\n",temp->permId);
            printf("PermSpecGroupRegionRelation[regionId] - %s\n",temp->regionId);
            printf("PermSpecGroupRegionRelation[permDefine] - %s\n",temp->permDefine);
            datas->push_back(temp);
        }
        *ret = 0;
        return datas;
    }
}
