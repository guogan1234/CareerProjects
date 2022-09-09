#include "permsecuritydefmanager.h"

PermSecurityDefManager::PermSecurityDefManager(QObject *parent) : QObject(parent)
{
    init();
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
