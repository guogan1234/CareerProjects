#include "permspecgroupstationmanager.h"

PermSpecGroupStationManager::PermSpecGroupStationManager()
{
    init();
}

PermSpecGroupStationManager::~PermSpecGroupStationManager()
{
    delete data;
}

list<PermSpecGroupStationRelation *> *PermSpecGroupStationManager::getPermSpecStationsByGroupIds(list<int> *groupIds)
{
    return data->getPermSpecStationsByGroupIds(groupIds);
}

list<PermSpecGroupStationRelation *> *PermSpecGroupStationManager::getPermSpecStationsByGroupId(int groupId)
{
    return data->getPermSpecStationsByGroupId(groupId);
}

list<PermSpecGroupStationRelation *> *PermSpecGroupStationManager::getPermSpecStationsByPermId(int permId)
{
    return data->getPermSpecStationsByPermId(permId);
}

int PermSpecGroupStationManager::checkPermByGroupIdAndPermId(int groupId, int permId)
{
    return data->checkPermByGroupIdAndPermId(groupId,permId);
}

int PermSpecGroupStationManager::checkPermByGroupIdAndPermDefine(int groupId, const char *permDefine)
{
    return data->checkPermByGroupIdAndPermDefine(groupId,permDefine);
}

int PermSpecGroupStationManager::loadData()
{
    return data->loadData();
}

int PermSpecGroupStationManager::loadDataByGroupId(int groupId)
{
    return data->loadDataByGroupId(groupId);
}

int PermSpecGroupStationManager::checkPermByPermId(int permId)
{
    return data->checkPermByPermId(permId);
}

int PermSpecGroupStationManager::checkPermByPermIdAndStationId(int permId, int stationId)
{
    return data->checkPermByPermIdAndStationId(permId,stationId);
}

int PermSpecGroupStationManager::checkPermByPermDefine(const char *permDefine)
{
    return data->checkPermByPermDefine(permDefine);
}

int PermSpecGroupStationManager::checkPermByPermDefineAndStationId(const char *permDefine, int stationId)
{
    return data->checkPermByPermDefineAndStationId(permDefine,stationId);
}

void PermSpecGroupStationManager::deleteInfoList(list<PermSpecGroupStationRelation *> *datas)
{
    return data->deleteInfoList(datas);
}

void PermSpecGroupStationManager::init()
{
    initParam();
}

void PermSpecGroupStationManager::initParam()
{
    data = new PermSpecGroupStationData();
}
