#ifndef PERMUSERDEFDATA_H
#define PERMUSERDEFDATA_H

#include "data/permdefine.h"
#include <QList>

class PermUserDefData
{
public:
  PermUserDefData();

  QList<PermUserDef*> * loadAll(int *ret);
  QList<PermUserDef*> * loadUserInfoById(int userId,int* ret);
  QList<PermUserDef*> * loadUserInfoByName(const char* userName,int* ret);
  QList<PermUserDef*> * loadUserInfoByIdBatch(QList<int> * userIds,int* ret);
  int delUserById(int userId,int* ret);

  void releaseUserInfo(QList<PermUserDef*> * datas);

private:
  QList<PermUserDef*> * loadDataBySql(QString sql,int *ret);
};

#endif // PERMUSERDEFDATA_H
