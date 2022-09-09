#include "permspecgroupregionmanager.h"

PermSpecGroupRegionManager::PermSpecGroupRegionManager()
{
    init();
}

PermSpecGroupRegionManager::~PermSpecGroupRegionManager()
{
    delete data;
}

list<PermSpecGroupRegionRelation *> *PermSpecGroupRegionManager::getPermSpecRegionsByGroupIds(list<int> *groupIds)
{
    return data->getPermSpecRegionsByGroupIds(groupIds);
}

list<PermSpecGroupRegionRelation *> *PermSpecGroupRegionManager::getPermSpecRegionsByGroupId(int groupId)
{
    return data->getPermSpecRegionsByGroupId(groupId);
}

list<PermSpecGroupRegionRelation *> *PermSpecGroupRegionManager::getPermSpecRegionsByPermId(int permId)
{
    return data->getPermSpecRegionsByPermId(permId);
}

int PermSpecGroupRegionManager::checkPermByGroupIdAndPermId(int groupId, int permId)
{
    return data->checkPermByGroupIdAndPermId(groupId,permId);
}

int PermSpecGroupRegionManager::checkPermByGroupIdAndPermDefine(int groupId, const char *permDefine)
{
    return data->checkPermByGroupIdAndPermDefine(groupId,permDefine);
}

int PermSpecGroupRegionManager::loadData()
{
    return data->loadData();
}

int PermSpecGroupRegionManager::loadDataByGroupId(int groupId)
{
    return data->loadDataByGroupId(groupId);
}

int PermSpecGroupRegionManager::checkPermByPermId(int permId)
{
    return data->checkPermByPermId(permId);
}

int PermSpecGroupRegionManager::checkPermByPermIdAndRegionId(int permId, int regionId)
{
    return data->checkPermByPermIdAndRegionId(permId,regionId);
}

int PermSpecGroupRegionManager::checkPermByPermDefine(const char *permDefine)
{
    return data->checkPermByPermDefine(permDefine);
}

int PermSpecGroupRegionManager::checkPermByPermDefineAndRegionId(const char *permDefine, int regionId)
{
    return data->checkPermByPermDefineAndRegionId(permDefine,regionId);
}

void PermSpecGroupRegionManager::deleteInfoList(list<PermSpecGroupRegionRelation *> *datas)
{
    return data->deleteInfoList(datas);
}

void PermSpecGroupRegionManager::init()
{
    initParam();
}

void PermSpecGroupRegionManager::initParam()
{
    data = new PermSpecGroupRegionData();
}
