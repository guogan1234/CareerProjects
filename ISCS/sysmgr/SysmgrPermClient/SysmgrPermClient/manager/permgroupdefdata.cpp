#include "permgroupdefdata.h"
#include "confcmn.h"
#include <stdio.h>
#include <string.h>

PermGroupDefData::PermGroupDefData()
{

}

list<PermGroupDef *> *PermGroupDefData::loadAll(int *ret)
{
    char* sql = (char*)"select * from conf.perm_group_def";
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

list<PermGroupDef *> *PermGroupDefData::loadGroupInfoById(int groupId, int *ret)
{
    char* baseSql = (char*)"select * from conf.perm_group_def where group_id=";
    char sql[100] = {0};
    sprintf(sql,"%s%d\n",baseSql,groupId);
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

list<PermGroupDef *> *PermGroupDefData::loadGroupInfoByName(const char *groupName, int *ret)
{
    char* baseSql = (char*)"select * from conf.perm_group_def where group_name=";
    char sql[100] = {0};
    sprintf(sql,"%s'%s'\n",baseSql,groupName);
    printf("sql - %s\n",sql);
    return loadDataBySql(sql,ret);
}

void PermGroupDefData::releaseGroupInfo(list<PermGroupDef *> *datas)
{

}

list<PermGroupDef *> *PermGroupDefData::loadDataBySql(char *sql, int *ret)
{
    ConfDbRecordSet* rs = confdb_query(sql);
    if(!rs){
        *ret = -1;
        printf("[record is NULL!]sql - %s\n",sql);
        return NULL;
    }else{
        list<PermGroupDef*> * datas = new list<PermGroupDef*>();
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
            printf("PermGroupDef[groupId] - %s\n",temp->groupId);
            printf("PermGroupDef[groupName] - %s\n",temp->groupName);
            printf("PermGroupDef[groupLevel] - %s\n",temp->groupLevel);
            printf("PermGroupDef[groupDesc] - %s\n",temp->groupDesc);
            printf("PermGroupDef[groupPic1] - %s\n",temp->groupPic1);
            printf("PermGroupDef[groupPic2] - %s\n",temp->groupPic2);
            printf("PermGroupDef[groupPic3] - %s\n",temp->groupPic3);
            datas->push_back(temp);
        }
        *ret = 0;
        return datas;
    }
}
