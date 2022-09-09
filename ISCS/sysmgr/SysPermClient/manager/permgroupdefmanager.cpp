#include "permgroupdefmanager.h"

PermGroupDefManager::PermGroupDefManager(QObject *parent) : QObject(parent)
{
  init();
}

QList<PermGroupDef *> *PermGroupDefManager::loadAll(int *ret)
{
  return data->loadAll(ret);
}

QList<PermGroupDef *> *PermGroupDefManager::loadGroupInfoById(int groupId, int *ret)
{
  return data->loadGroupInfoById(groupId,ret);
}

QList<PermGroupDef *> *PermGroupDefManager::loadGroupInfoByName(const char *groupName, int *ret)
{
  return data->loadGroupInfoByName(groupName,ret);
}

void PermGroupDefManager::releaseGroupInfo(QList<PermGroupDef *> *datas)
{
  return data->releaseGroupInfo(datas);
}

void PermGroupDefManager::init()
{
  initParam();
}

void PermGroupDefManager::initParam()
{
  data = new PermGroupDefData();
}
