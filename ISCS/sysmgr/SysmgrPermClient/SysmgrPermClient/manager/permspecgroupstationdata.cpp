#include "permspecgroupstationdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermSpecGroupStationData::PermSpecGroupStationData()
{

}

list<PermSpecGroupStationRelation *> *PermSpecGroupStationData::getPermSpecStationsByGroupIds(list<int> *groupIds)
{
    if(!groupIds){
        printf("[params is NULL!]\n");
        return NULL;
    }
    char* baseSql = (char*)"select * from conf.perm_spec_group_station_realation where group_id in(";
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

list<PermSpecGroupStationRelation *> *PermSpecGroupStationData::getPermSpecStationsByGroupId(int groupId)
{
    char* baseSql = (char*)"select * from conf.perm_spec_group_station_realation where group_id=";
    char sql[256] = {0};
    sprintf(sql,"%s%d",baseSql,groupId);
    printf("sql - %s\n",sql);
    int ret = -1;
    return loadDataBySql(sql,&ret);
}

list<PermSpecGroupStationRelation *> *PermSpecGroupStationData::getPermSpecStationsByPermId(int permId)
{
    char* baseSql = (char*)"select * from conf.perm_spec_group_station_realation where perm_id=";
    char sql[256] = {0};
    sprintf(sql,"%s%d",baseSql,permId);
    printf("sql - %s\n",sql);
    int ret = -1;
    return loadDataBySql(sql,&ret);
}

int PermSpecGroupStationData::checkPermByGroupIdAndPermId(int groupId, int permId)
{
    char* baseSql = (char*)"select * from conf.perm_spec_group_station_realation where group_id=";
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

int PermSpecGroupStationData::checkPermByGroupIdAndPermDefine(int groupId, const char *permDefine)
{
    if(!permDefine) return PERM_ERROR;
    char* baseSql = (char*)"select * from conf.perm_spec_group_station_realation where group_id=";
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

int PermSpecGroupStationData::loadData()
{
    char* sql = (char*)"select * from conf.perm_spec_group_station_realation";
    printf("sql - %s\n",sql);
    int ret = -1;
    data = loadDataBySql(sql,&ret);
    return ret;
}

int PermSpecGroupStationData::loadDataByGroupId(int groupId)
{
    char* sql = (char*)"select * from conf.perm_spec_group_station_realation where group_id=";
    sprintf(sql,"%s%d",sql,groupId);
    printf("sql - %s\n",sql);
    int ret = -1;
    data = loadDataBySql(sql,&ret);
    return ret;
}

int PermSpecGroupStationData::checkPermByPermId(int permId)
{
    list<PermSpecGroupStationRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupStationRelation* temp = *iter;
        int tempPermId;
        sscanf(temp->permId,"%d",&tempPermId);
        if(permId == tempPermId){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

int PermSpecGroupStationData::checkPermByPermIdAndStationId(int permId, int stationId)
{
    list<PermSpecGroupStationRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupStationRelation* temp = *iter;
        int tempPermId;
        int tempStationId;
        sscanf(temp->permId,"%d",&tempPermId);
        sscanf(temp->stationId,"%d",&tempStationId);
        if(permId == tempPermId && stationId == tempStationId){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

int PermSpecGroupStationData::checkPermByPermDefine(const char *permDefine)
{
    if(!permDefine) return PERM_ERROR;
    list<PermSpecGroupStationRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupStationRelation* temp = *iter;
        if(!strcmp(permDefine,temp->permDefine)){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

int PermSpecGroupStationData::checkPermByPermDefineAndStationId(const char *permDefine, int stationId)
{
    if(!permDefine) return PERM_ERROR;
    list<PermSpecGroupStationRelation*>::const_iterator iter;
    for(iter = data->begin();iter != data->end();iter++){
        PermSpecGroupStationRelation* temp = *iter;
        int tempStationId;
        sscanf(temp->stationId,"%d",&tempStationId);
        if(!strcmp(permDefine,temp->permDefine) && stationId == tempStationId){
            return PERM_PERMIT;
        }
    }
    return PERM_FORBIT;
}

void PermSpecGroupStationData::deleteInfoList(list<PermSpecGroupStationRelation *> *datas)
{
    if(!datas) return;
    printf("[PermSpecGroupStationData]deleteInfoList start...\n");
    list<PermSpecGroupStationRelation*>::const_iterator iter;
    for(iter = datas->begin();iter != datas->end();iter++){
        delete *iter;
        datas->erase(iter);
    }
    delete datas;
    datas = NULL;
    printf("[PermSpecGroupStationData]deleteInfoList end!\n");
}

list<PermSpecGroupStationRelation *> *PermSpecGroupStationData::loadDataBySql(char *sql, int *ret)
{
    if(!sql || !ret) return NULL;
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        *ret = -1;
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        list<PermSpecGroupStationRelation*> * datas = new list<PermSpecGroupStationRelation*>();
        int count = confdb_recordset_get_count(rs);
        for(int i = 0;i < count; ++i){
            PermSpecGroupStationRelation* temp = new PermSpecGroupStationRelation();
            temp->relationId = strdup(confdb_recordset_get(rs,i,"relation_id"));
            temp->groupId = strdup(confdb_recordset_get(rs,i,"group_id"));
            temp->permId = strdup(confdb_recordset_get(rs,i,"perm_id"));
            temp->stationId = strdup(confdb_recordset_get(rs,i,"station_id"));
            temp->permDefine = strdup(confdb_recordset_get(rs,i,"perm_define"));
            printf("PermSpecGroupStationRelation[relationId] - %s\n",temp->relationId);
            printf("PermSpecGroupStationRelation[groupId] - %s\n",temp->groupId);
            printf("PermSpecGroupStationRelation[permId] - %s\n",temp->permId);
            printf("PermSpecGroupStationRelation[stationId] - %s\n",temp->stationId);
            printf("PermSpecGroupStationRelation[permDefine] - %s\n",temp->permDefine);
            datas->push_back(temp);
        }
        *ret = 0;
        return datas;
    }
}
