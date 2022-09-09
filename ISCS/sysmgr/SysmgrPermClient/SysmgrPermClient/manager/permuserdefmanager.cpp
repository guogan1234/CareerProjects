#include "permuserdefmanager.h"

PermUserDefManager::PermUserDefManager()
{
    init();
}

PermUserDefManager::~PermUserDefManager()
{
    delete data;
}

list<PermUserDef *> *PermUserDefManager::loadAll(int *ret)
{
    return data->loadAll(ret);
}

list<PermUserDef *> *PermUserDefManager::loadUserInfoById(int userId, int *ret)
{
    return data->loadUserInfoById(userId,ret);
}

list<PermUserDef *> *PermUserDefManager::loadUserInfoByName(const char *userName, int *ret)
{
    return data->loadUserInfoByName(userName,ret);
}

int PermUserDefManager::delUserById(int userId, int *ret)
{
    return data->delUserById(userId,ret);
}

int PermUserDefManager::modifyUserPWD(int userId, const char *password)
{
    return data->modifyUserPWD(userId,password);
}

void PermUserDefManager::releaseUserInfo(list<PermUserDef *> *datas)
{

}

bool PermUserDefManager::userUnlock(int userId, const char *password)
{
    return data->userUnlock(userId,password);
}

bool PermUserDefManager::userUnlock(const char *userName, const char *password)
{
    return data->userUnlock(userName,password);
}

void PermUserDefManager::init()
{
    initParam();
}

void PermUserDefManager::initParam()
{
    data = new PermUserDefData();
}
