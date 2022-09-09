#ifndef PERMSPECGROUPSTATIONMANAGER_H
#define PERMSPECGROUPSTATIONMANAGER_H

#include "permspecgroupstationdata.h"

class PermSpecGroupStationManager
{
public:
    PermSpecGroupStationManager();
    ~PermSpecGroupStationManager();

    list<PermSpecGroupStationRelation*> * getPermSpecStationsByGroupIds(list<int> * groupIds);
    list<PermSpecGroupStationRelation*> * getPermSpecStationsByGroupId(int groupId);
    list<PermSpecGroupStationRelation*> * getPermSpecStationsByPermId(int permId);
    int checkPermByGroupIdAndPermId(int groupId,int permId);
    int checkPermByGroupIdAndPermDefine(int groupId,const char* permDefine);

    int loadData();
    int loadDataByGroupId(int groupId);
    int checkPermByPermId(int permId);
    int checkPermByPermIdAndStationId(int permId,int stationId);
    int checkPermByPermDefine(const char* permDefine);
    int checkPermByPermDefineAndStationId(const char* permDefine,int stationId);

    void deleteInfoList(list<PermSpecGroupStationRelation*> * datas);
private:
private:
    PermSpecGroupStationData* data;
    void init();
    void initParam();
};

#endif // PERMSPECGROUPSTATIONMANAGER_H
