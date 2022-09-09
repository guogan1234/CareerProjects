#include "permsecuritydefmanager.h"

PermSecurityDefManager::PermSecurityDefManager()
{
    init();
}

PermSecurityDefManager::~PermSecurityDefManager()
{
    delete data;
}

PermSecurityDef *PermSecurityDefManager::getPermSecurityInfo(int *ret)
{
    return data->getPermSecurityInfo(ret);
}

void PermSecurityDefManager::init()
{
    initParam();
}

void PermSecurityDefManager::initParam()
{
    data = new PermSecurityDefData();
}
