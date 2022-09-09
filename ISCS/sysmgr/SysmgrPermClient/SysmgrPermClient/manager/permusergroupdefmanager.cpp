#include "permusergroupdefmanager.h"

PermUserGroupDefManager::PermUserGroupDefManager()
{
    init();
}

PermUserGroupDefManager::~PermUserGroupDefManager()
{
    delete data;
}

list<int> *PermUserGroupDefManager::getGroupIdsByUserId(int userId)
{
    return data->getGroupIdsByUserId(userId);
}

int PermUserGroupDefManager::delUserGroupDefByUserId(int userId)
{
    return data->delUserGroupDefByUserId(userId);
}

void PermUserGroupDefManager::init()
{
    initParam();
}

void PermUserGroupDefManager::initParam()
{
    data = new PermUserGroupDefData();
}
