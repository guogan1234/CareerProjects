#ifndef PERMGROUPDEFMANAGER_H
#define PERMGROUPDEFMANAGER_H

#include <QObject>
#include "permgroupdefdata.h"

class PermGroupDefManager : public QObject
{
  Q_OBJECT
public:
  explicit PermGroupDefManager(QObject *parent = 0);

  QList<PermGroupDef*> * loadAll(int *ret);
  QList<PermGroupDef*> * loadGroupInfoById(int groupId,int* ret);
  QList<PermGroupDef*> * loadGroupInfoByName(const char* groupName,int* ret);
  void releaseGroupInfo(QList<PermGroupDef*> * datas);
signals:

public slots:

private:
  PermGroupDefData* data;

  void init();
  void initParam();
};

#endif // PERMGROUPDEFMANAGER_H
