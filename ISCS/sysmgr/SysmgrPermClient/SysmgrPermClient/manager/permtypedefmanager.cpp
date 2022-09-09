#include "permtypedefmanager.h"

PermTypeDefManager::PermTypeDefManager()
{
    init();
}

PermTypeDefManager::~PermTypeDefManager()
{
    delete data;
}

char *PermTypeDefManager::getPermTypeNameById(int typeId)
{
    return data->getPermTypeNameById(typeId);
}

void PermTypeDefManager::init()
{
    initParam();
}

void PermTypeDefManager::initParam()
{
    data = new PermTypeDefData();
}
