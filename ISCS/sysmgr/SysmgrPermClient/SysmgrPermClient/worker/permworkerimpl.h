#ifndef PERMWORKERIMPL_H
#define PERMWORKERIMPL_H

#include "data/permdefine.h"
#include "data/perm_define.h"
#include <list>
using namespace std;

class PermWorkerImpl
{
public:
    PermWorkerImpl();

    //perm
    int permCheck(int groupId,int permType,const char* arg);
    int permCheck(int groupId, int permType, const char **arg,int permSize,int *ret);

    list<PermFuncDef*> * getGeneralPermByUserId(int userId,int permType,int *ret);
    list<PermSpecStationRegion*> * getSpecialPermByUserId(int userId,int *ret);
    spec_func_info_get *   get_spe_perm_by_userg_id(const int userg_ID,int &ret);
    //user
    list<PermUserDef*> * getUserInfoByUserId(int userId,int *ret);
    list<PermUserDef*> * getUserInfoByUserName(const char* userName,int *ret);
    list<PermUserDef*> * getUserInfoAll(int *ret);
    list<PermUserDef*> * getUserInfoByGroup(int groupId,int *ret);
    void userInfoDelete(list<PermUserDef*> * datas);
    int deleteUserById(int userId);
    int deleteUserGroupRelation(const int userId);
    int modifyUserPWD(int userId,const char* password);
    bool userUnlock(int userId,const char* password);
    bool userUnlock(const char* userName,const char* password);
    //group
    list<PermGroupDef*> * getGroupInfoByGroupId(int groupId,int *ret);
    list<PermGroupDef*> * getGroupInfoByGroupName(const char* groupName,int *ret);
    list<PermGroupDef*> * getGroupInfoAll(int *ret);
    void groupInfoDelete(list<PermGroupDef*> * datas);
    //perm_security
    PermSecurityDef* getPermSecurityInfo(int *ret);
};

#endif // PERMWORKERIMPL_H
