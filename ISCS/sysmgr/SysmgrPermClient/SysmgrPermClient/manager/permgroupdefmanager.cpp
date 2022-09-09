#include "permgroupdefmanager.h"

PermGroupDefManager::PermGroupDefManager()
{
    init();
}

PermGroupDefManager::~PermGroupDefManager()
{
    delete data;
}

list<PermGroupDef *> *PermGroupDefManager::loadAll(int *ret)
{
    return data->loadAll(ret);
}

list<PermGroupDef *> *PermGroupDefManager::loadGroupInfoById(int groupId, int *ret)
{
    return data->loadGroupInfoById(groupId,ret);
}

list<PermGroupDef *> *PermGroupDefManager::loadGroupInfoByName(const char *groupName, int *ret)
{
    return data->loadGroupInfoByName(groupName,ret);
}

void PermGroupDefManager::releaseGroupInfo(list<PermGroupDef *> *datas)
{

}

void PermGroupDefManager::init()
{
    initParam();
}

void PermGroupDefManager::initParam()
{
    data = new PermGroupDefData();
}
