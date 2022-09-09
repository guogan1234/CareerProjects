#ifndef PERMSPECGROUPSTATIONDATA_H
#define PERMSPECGROUPSTATIONDATA_H

#include "data/permdefine.h"

class PermSpecGroupStationData
{
public:
    PermSpecGroupStationData();

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
    list<PermSpecGroupStationRelation*> * loadDataBySql(char* sql,int *ret);

    list<PermSpecGroupStationRelation*> * data;
};

#endif // PERMSPECGROUPSTATIONDATA_H
