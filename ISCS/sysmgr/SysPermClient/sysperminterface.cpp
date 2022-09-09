#include "sysperminterface.h"
#include "cfdbmanager.h"

SysPermInterface::SysPermInterface()
{
    init();
}

SysPermInterface::~SysPermInterface()
{
    CFDBManager* manager = static_cast<CFDBManager*>(cfdbManager);
    manager->endCFDBModule();
}

QList<PermUserDef *> *SysPermInterface::getUserInfoByUserId(int userId, int *ret)
{
    return permWorker->getUserInfoByUserId(userId,ret);
}

QList<PermUserDef *> *SysPermInterface::getUserInfoByUserName(const char *userName, int *ret)
{
    return permWorker->getUserInfoByUserName(userName,ret);
}

QList<PermUserDef *> *SysPermInterface::getUserInfoAll(int *ret)
{
    return permWorker->getUserInfoAll(ret);
}

QList<PermUserDef *> *SysPermInterface::getUserInfoByGroup(int groupId, int *ret)
{
    return permWorker->getUserInfoByGroup(groupId,ret);
}

void SysPermInterface::userInfoDelete(QList<PermUserDef *> *datas)
{
    return permWorker->userInfoDelete(datas);
}

QList<PermGroupDef *> *SysPermInterface::getGroupInfoByGroupId(int groupId, int *ret)
{
    return permWorker->getGroupInfoByGroupId(groupId,ret);
}

QList<PermGroupDef *> *SysPermInterface::getGroupInfoByGroupName(const char *groupName, int *ret)
{
    return permWorker->getGroupInfoByGroupName(groupName,ret);
}

QList<PermGroupDef *> *SysPermInterface::getGroupInfoAll(int *ret)
{
    return permWorker->getGroupInfoAll(ret);
}

void SysPermInterface::groupInfoDelete(QList<PermGroupDef *> *datas)
{
    return permWorker->groupInfoDelete(datas);
}

void SysPermInterface::init()
{
    initParam();
    initModule();
}

void SysPermInterface::initParam()
{
    cfdbManager = new CFDBManager();
    permWorker = new PermWorkerImpl();
}

void SysPermInterface::initModule()
{
    initCFDBModule();
}

void SysPermInterface::initCFDBModule()
{
    CFDBManager* manager = static_cast<CFDBManager*>(cfdbManager);
    manager->startCFDBModule();
}
