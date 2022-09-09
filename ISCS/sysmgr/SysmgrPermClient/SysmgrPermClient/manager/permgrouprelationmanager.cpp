#include "permgrouprelationmanager.h"

PermGroupRelationManager::PermGroupRelationManager()
{
    init();
}

PermGroupRelationManager::~PermGroupRelationManager()
{
    delete data;
}

list<int> *PermGroupRelationManager::getPermIdsByGroupId(list<int> *groupIds)
{
    return data->getPermIdsByGroupId(groupIds);
}

int PermGroupRelationManager::checkPermByGroupIdAndPermId(int groupId, int permId)
{
    return data->checkPermByGroupIdAndPermId(groupId,permId);
}

int PermGroupRelationManager::checkPermByGroupIdAndPermDefine(int groupId, const char *permDefine)
{
    return data->checkPermByGroupIdAndPermDefine(groupId,permDefine);
}

int PermGroupRelationManager::loadData()
{
    return data->loadData();
}

int PermGroupRelationManager::loadDataByGroupId(int groupId)
{
    return data->loadDataByGroupId(groupId);
}

int PermGroupRelationManager::checkPermByPermId(int permId)
{
    return data->checkPermByPermId(permId);
}

int PermGroupRelationManager::checkPermByPermDefine(const char *permDefine)
{
    return data->checkPermByPermDefine(permDefine);
}

void PermGroupRelationManager::init()
{
    initParam();
}

void PermGroupRelationManager::initParam()
{
    data = new PermGroupRelationData();
}
