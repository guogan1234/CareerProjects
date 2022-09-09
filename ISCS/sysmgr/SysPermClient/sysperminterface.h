#ifndef SYSPERMINTERFACE_H
#define SYSPERMINTERFACE_H

#include "syspermclient_global.h"
#include "worker/permworkerimpl.h"
#include <QObject>

class SYSPERMCLIENTSHARED_EXPORT SysPermInterface : public QObject
{
    Q_OBJECT
public:
    SysPermInterface();
    ~SysPermInterface();

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
private:
    void init();
    void initParam();
    void initModule();
    void initCFDBModule();

    QObject* cfdbManager;
    PermWorkerImpl* permWorker;
};

#endif // SYSPERMINTERFACE_H
