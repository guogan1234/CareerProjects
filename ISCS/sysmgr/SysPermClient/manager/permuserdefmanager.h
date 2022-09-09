#ifndef PERMUSERDEFMANAGER_H
#define PERMUSERDEFMANAGER_H

#include <QObject>
#include "permuserdefdata.h"

class PermUserDefManager : public QObject
{
  Q_OBJECT
public:
  explicit PermUserDefManager(QObject *parent = 0);

  QList<PermUserDef*> * loadAll(int *ret);
  QList<PermUserDef*> * loadUserInfoById(int userId,int* ret);
  QList<PermUserDef*> * loadUserInfoByName(const char* userName,int* ret);
  QList<PermUserDef*> * loadUserInfoByIdBatch(QList<int> * userIds,int* ret);
  int delUserById(int userId,int* ret);

  void releaseUserInfo(QList<PermUserDef*> * datas);
signals:

public slots:

private:
  PermUserDefData* data;

  void init();
  void initParam();
};

#endif // PERMUSERDEFMANAGER_H
