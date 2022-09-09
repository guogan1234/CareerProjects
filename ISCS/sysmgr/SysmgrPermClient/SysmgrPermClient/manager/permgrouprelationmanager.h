#ifndef PERMGROUPRELATIONMANAGER_H
#define PERMGROUPRELATIONMANAGER_H

#include "permgrouprelationdata.h"

class PermGroupRelationManager
{
public:
    PermGroupRelationManager();
    ~PermGroupRelationManager();

    list<int> * getPermIdsByGroupId(list<int> * groupIds);
    int checkPermByGroupIdAndPermId(int groupId,int permId);
    int checkPermByGroupIdAndPermDefine(int groupId,const char* permDefine);

    int loadData();
    int loadDataByGroupId(int groupId);
    int checkPermByPermId(int permId);
    int checkPermByPermDefine(const char* permDefine);
private:
    PermGroupRelationData* data;
    void init();
    void initParam();
};

#endif // PERMGROUPRELATIONMANAGER_H
