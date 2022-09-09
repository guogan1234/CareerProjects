#ifndef PERMGROUPRELATIONDATA_H
#define PERMGROUPRELATIONDATA_H

#include "data/permdefine.h"
#include <list>
using namespace std;

class PermGroupRelationData
{
public:
    PermGroupRelationData();

    list<int> * getPermIdsByGroupId(list<int> * groupIds);
    list<PermGroupRelation*> * getRecordsByGroupIds(list<int> * groupIds);
    int checkPermByGroupIdAndPermId(int groupId,int permId);
    int checkPermByGroupIdAndPermDefine(int groupId,const char* permDefine);

    int loadData();
    int loadDataByGroupId(int groupId);
    int checkPermByPermId(int permId);
    int checkPermByPermDefine(const char* permDefine);
private:
    list<PermGroupRelation*> * data;

    list<PermGroupRelation*> * loadDataBySql(const char* sql,int *ret);
};

#endif // PERMGROUPRELATIONDATA_H
