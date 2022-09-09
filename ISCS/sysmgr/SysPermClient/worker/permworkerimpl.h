#ifndef PERMWORKERIMPL_H
#define PERMWORKERIMPL_H

#include "data/permdefine.h"
#include <QObject>

class PermWorkerImpl : public QObject
{
    Q_OBJECT
public:
    explicit PermWorkerImpl(QObject *parent = 0);

    //perm

    //user
    QList<PermUserDef*> * getUserInfoByUserId(int userId,int *ret);
    QList<PermUserDef*> * getUserInfoByUserName(const char* userName,int *ret);
    QList<PermUserDef*> * getUserInfoAll(int *ret);
    QList<PermUserDef*> * getUserInfoByGroup(int groupId,int *ret);
    void userInfoDelete(QList<PermUserDef*> * datas);
    //group
    QList<PermGroupDef*> * getGroupInfoByGroupId(int groupId,int *ret);
    QList<PermGroupDef*> * getGroupInfoByGroupName(const char* groupName,int *ret);
    QList<PermGroupDef*> * getGroupInfoAll(int *ret);
    void groupInfoDelete(QList<PermGroupDef*> * datas);
    //perm_security
    PermSecurityDef* getPermSecurityInfo(int *ret);
signals:

public slots:
};

#endif // PERMWORKERIMPL_H
