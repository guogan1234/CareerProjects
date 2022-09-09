#ifndef SYSMGRPERMINTERFACE_H
#define SYSMGRPERMINTERFACE_H

#include <list>
#include "sysmgrpermclient_global.h"
#include "data/permdefine.h"
#include "module/cfdbmanager.h"
#include "worker/permworkerimpl.h"

#include "data/perm_define.h"
using namespace std;

class SYSMGRPERMCLIENTSHARED_EXPORT SysmgrPermInterface
{
public:
    SysmgrPermInterface();
    ~SysmgrPermInterface();

    //perm
    int permCheck(int groupId,int permType,const char* arg);
    int permCheck(int groupId, int permType, const char **arg,int permSize,int *ret);

    list<PermFuncDef*> * getGeneralPermByUserId(int userId,int permType,int *ret);
    nom_func_info_get * get_gen_perm_by_userg_id(const int userg_ID, const int type,int &ret);
    void gen_perm_delete(nom_func_info_get * temp_nom_func_info);

    list<PermSpecStationRegion*> * getSpecialPermByUserId(int userId,int *ret);
    spec_func_info_get *   get_spe_perm_by_userg_id(const int userg_ID,int &ret);
    void spe_perm_delete(spec_func_info_get * temp_spec_func_info);
    //user
    list<PermUserDef*> * getUserInfoByUserId(int userId,int *ret);
    interface_stru_user_info * get_user_info_by_user_ID(const int user_id,int &ret);

    list<PermUserDef*> * getUserInfoByUserName(const char* userName,int *ret);
    interface_stru_user_info * get_user_info_by_user_name(const char * user_name,int &ret);
    void user_info_delete(interface_stru_user_info * temp_int_user_info);

    list<PermUserDef*> * getUserInfoAll(int *ret);
    interface_stru_user_info ** get_user_info_all(int &ret);
    void user_info_delete(interface_stru_user_info ** temp_int_user_info, int  amount);

    list<PermUserDef*> * getUserInfoByGroup(int groupId,int *ret);
    interface_stru_user_info ** get_user_info_by_group(const int group_id,int &ret);

    void userInfoDelete(list<PermUserDef*> * datas);
    int deleteUserById(int userId);
    int deleteUserGroupRelation(const int userId);
    int modifyUserPWD(int userId,const char* password);
    bool userUnlock(int userId,const char* password);
    bool userUnlock(const char* userName,const char* password);
    //group
    list<PermGroupDef*> * getGroupInfoByGroupId(int groupId,int *ret);
    interface_stru_group_info * get_group_info_by_group_ID(const int group_id,int &ret);

    list<PermGroupDef*> * getGroupInfoByGroupName(const char* groupName,int *ret);
    interface_stru_group_info * get_group_info_by_group_name(const char * group_name,int &ret);
    void group_info_delete(interface_stru_group_info * temp_int_group_info);

    list<PermGroupDef*> * getGroupInfoAll(int *ret);
    void groupInfoDelete(list<PermGroupDef*> * datas);
    interface_stru_group_info ** get_group_info_all(int &ret);
    void group_info_delete(interface_stru_group_info **temp_int_group_info, int amount);
    //perm_security
    PermSecurityDef* getPermSecurityInfo(int *ret);
    int  get_security_def(stru_security_def &temp_sec_def);
private:
    void init();
    void initParam();
    void initModule();
    void initCFDBModule();

//    QObject* cfdbManager;
    CFDBManager* cfdbManager;
    PermWorkerImpl* permWorker;
};

#endif // SYSMGRPERMINTERFACE_H
