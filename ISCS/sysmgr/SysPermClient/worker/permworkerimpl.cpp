#include "permworkerimpl.h"
#include "manager/permuserdefmanager.h"
#include "manager/permgroupdefmanager.h"
#include "manager/permsecuritydefmanager.h"

PermWorkerImpl::PermWorkerImpl(QObject *parent) : QObject(parent)
{

}

QList<PermUserDef *> *PermWorkerImpl::getUserInfoByUserId(int userId, int *ret)
{
    PermUserDefManager* manager = new PermUserDefManager(this);
    return manager->loadUserInfoById(userId,ret);
}

QList<PermUserDef *> *PermWorkerImpl::getUserInfoByUserName(const char *userName, int *ret)
{
    PermUserDefManager* manager = new PermUserDefManager(this);
    return manager->loadUserInfoByName(userName,ret);
}

QList<PermUserDef *> *PermWorkerImpl::getUserInfoAll(int *ret)
{
    PermUserDefManager* manager = new PermUserDefManager(this);
    return manager->loadAll(ret);
}

QList<PermUserDef *> *PermWorkerImpl::getUserInfoByGroup(int groupId, int *ret)
{
    return NULL;
}

void PermWorkerImpl::userInfoDelete(QList<PermUserDef *> *datas)
{
    PermUserDefManager* manager = new PermUserDefManager(this);
    return manager->releaseUserInfo(datas);
}

QList<PermGroupDef *> *PermWorkerImpl::getGroupInfoByGroupId(int groupId, int *ret)
{
    PermGroupDefManager* manager = new PermGroupDefManager(this);
    return manager->loadGroupInfoById(groupId,ret);
}

QList<PermGroupDef *> *PermWorkerImpl::getGroupInfoByGroupName(const char *groupName, int *ret)
{
    PermGroupDefManager* manager = new PermGroupDefManager(this);
    return manager->loadGroupInfoByName(groupName,ret);
}

QList<PermGroupDef *> *PermWorkerImpl::getGroupInfoAll(int *ret)
{
    PermGroupDefManager* manager = new PermGroupDefManager(this);
    return manager->loadAll(ret);
}

void PermWorkerImpl::groupInfoDelete(QList<PermGroupDef *> *datas)
{
    PermGroupDefManager* manager = new PermGroupDefManager(this);
    return manager->releaseGroupInfo(datas);
}

PermSecurityDef *PermWorkerImpl::getPermSecurityInfo(int *ret)
{
    PermSecurityDefManager* manager = new PermSecurityDefManager(this);
    return manager->getPermSecurityInfo(ret);
}
