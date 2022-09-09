#ifndef PERMSPECGROUPREGIONMANAGER_H
#define PERMSPECGROUPREGIONMANAGER_H

#include "permspecgroupregiondata.h"

class PermSpecGroupRegionManager
{
public:
    PermSpecGroupRegionManager();
    ~PermSpecGroupRegionManager();

    list<PermSpecGroupRegionRelation*> * getPermSpecRegionsByGroupIds(list<int> * groupIds);
    list<PermSpecGroupRegionRelation*> * getPermSpecRegionsByGroupId(int groupId);
    list<PermSpecGroupRegionRelation*> * getPermSpecRegionsByPermId(int permId);
    int checkPermByGroupIdAndPermId(int groupId,int permId);
    int checkPermByGroupIdAndPermDefine(int groupId,const char* permDefine);

    int loadData();
    int loadDataByGroupId(int groupId);
    int checkPermByPermId(int permId);
    int checkPermByPermIdAndRegionId(int permId,int regionId);
    int checkPermByPermDefine(const char* permDefine);
    int checkPermByPermDefineAndRegionId(const char* permDefine,int regionId);

    void deleteInfoList(list<PermSpecGroupRegionRelation*> * datas);
private:
    PermSpecGroupRegionData* data;
    void init();
    void initParam();
};

#endif // PERMSPECGROUPREGIONMANAGER_H
