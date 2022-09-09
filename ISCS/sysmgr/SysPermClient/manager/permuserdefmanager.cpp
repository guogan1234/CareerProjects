#include "permuserdefmanager.h"

PermUserDefManager::PermUserDefManager(QObject *parent) : QObject(parent)
{
    init();
}

QList<PermUserDef *> *PermUserDefManager::loadAll(int *ret)
{
    return data->loadAll(ret);
}

QList<PermUserDef *> *PermUserDefManager::loadUserInfoById(int userId, int *ret)
{
  return data->loadUserInfoById(userId,ret);
}

QList<PermUserDef *> *PermUserDefManager::loadUserInfoByName(const char *userName, int *ret)
{
  return data->loadUserInfoByName(userName,ret);
}

QList<PermUserDef *> *PermUserDefManager::loadUserInfoByIdBatch(QList<int> *userIds, int *ret)
{
  return data->loadUserInfoByIdBatch(userIds,ret);
}

int PermUserDefManager::delUserById(int userId, int *ret)
{
  return data->delUserById(userId,ret);
}

void PermUserDefManager::releaseUserInfo(QList<PermUserDef *> *datas)
{
  return data->releaseUserInfo(datas);
}

void PermUserDefManager::init()
{
  initParam();
}

void PermUserDefManager::initParam()
{
  data = new PermUserDefData();
}
