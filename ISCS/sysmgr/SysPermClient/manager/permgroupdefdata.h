#ifndef PERMGROUPDEFDATA_H
#define PERMGROUPDEFDATA_H

#include "data/permdefine.h"
#include <QList>

class PermGroupDefData
{
public:
  PermGroupDefData();

  QList<PermGroupDef*> * loadAll(int *ret);
  QList<PermGroupDef*> * loadGroupInfoById(int groupId,int* ret);
  QList<PermGroupDef*> * loadGroupInfoByName(const char* groupName,int* ret);
  void releaseGroupInfo(QList<PermGroupDef*> * datas);

private:
  QList<PermGroupDef*> * loadDataBySql(QString sql,int *ret);
};

#endif // PERMGROUPDEFDATA_H
