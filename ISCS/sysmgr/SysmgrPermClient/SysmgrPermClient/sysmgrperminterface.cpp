#include "sysmgrperminterface.h"
#include <stdio.h>
#include <string.h>

SysmgrPermInterface::SysmgrPermInterface()
{
    init();
}

SysmgrPermInterface::~SysmgrPermInterface()
{
    cfdbManager->endCFDBModule();

    delete cfdbManager;
    delete permWorker;
}

int SysmgrPermInterface::permCheck(int groupId, int permType, const char *arg)
{
    return permWorker->permCheck(groupId,permType,arg);
}

int SysmgrPermInterface::permCheck(int groupId, int permType, const char **arg, int permSize, int *ret)
{
    return permWorker->permCheck(groupId,permType,arg,permSize,ret);
}

list<PermFuncDef *> *SysmgrPermInterface::getGeneralPermByUserId(int userId, int permType, int *ret)
{
    return permWorker->getGeneralPermByUserId(userId,permType,ret);
}

nom_func_info_get *SysmgrPermInterface::get_gen_perm_by_userg_id(const int userg_ID, const int type, int &ret)
{
    list<PermFuncDef*> * rawDatas = permWorker->getGeneralPermByUserId(userg_ID,type,&ret);
    if(!rawDatas) return NULL;
    int len = rawDatas->size();
    nom_func_info_get* returnData = new nom_func_info_get();
    returnData->perm_amount = len;
    int* permIds = new int[len];
    char** permDefines = new char*[len];
    char** permNames = new char*[len];

    int i = 0;
    list<PermFuncDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        PermFuncDef* data = (PermFuncDef*)(*iter);
        int permId = -1;
        sscanf(data->permId,"%d",&permId);
        permIds[i] = permId;
        permDefines[i] = strdup(data->permDefine);
        permNames[i] = strdup(data->permName);

        i++;
    }
    returnData->perm_id = permIds;
    returnData->perm_define = permDefines;
    returnData->perm_name = permNames;
    return returnData;
}

void SysmgrPermInterface::gen_perm_delete(nom_func_info_get *temp_nom_func_info)
{
    if(!temp_nom_func_info) return;
    if(temp_nom_func_info->perm_id){
        delete []temp_nom_func_info->perm_id;
    }
    for(int i = 0;i < temp_nom_func_info->perm_amount;i++){
        if(temp_nom_func_info->perm_id){
            delete temp_nom_func_info->perm_id;
            temp_nom_func_info->perm_id = NULL;
        }
        if(temp_nom_func_info->perm_define){
            delete temp_nom_func_info->perm_define[i];
            temp_nom_func_info->perm_define[i] = NULL;
        }
        if(temp_nom_func_info->perm_name){
            delete temp_nom_func_info->perm_name[i];
            temp_nom_func_info->perm_name[i] = NULL;
        }
    }
    delete temp_nom_func_info;
}

list<PermSpecStationRegion *> *SysmgrPermInterface::getSpecialPermByUserId(int userId, int *ret)
{
    return permWorker->getSpecialPermByUserId(userId,ret);
}

spec_func_info_get *SysmgrPermInterface::get_spe_perm_by_userg_id(const int userg_ID, int &ret)
{
    return permWorker->get_spe_perm_by_userg_id(userg_ID,ret);
}

void SysmgrPermInterface::spe_perm_delete(spec_func_info_get *temp_spec_func_info)
{
    if(!temp_spec_func_info) return;
    printf("[SysmgrPermInterface]spe_perm_delete start...\n");
    delete[] temp_spec_func_info->perm_id;
    for(int i = 0;i < temp_spec_func_info->perm_amount;i++){
        delete temp_spec_func_info->perm_define[i];
        delete temp_spec_func_info->perm_name[i];
        delete[] temp_spec_func_info->station_id[i];
        delete[] temp_spec_func_info->region_id[i];
    }
    delete[] temp_spec_func_info->perm_define;
    delete[] temp_spec_func_info->perm_name;
    delete[] temp_spec_func_info->station_amount;
    delete[] temp_spec_func_info->region_amount;
    printf("[SysmgrPermInterface]spe_perm_delete end!\n");
}

list<PermUserDef *> *SysmgrPermInterface::getUserInfoByUserId(int userId, int *ret)
{
    return permWorker->getUserInfoByUserId(userId,ret);
}

interface_stru_user_info *SysmgrPermInterface::get_user_info_by_user_ID(const int user_id, int &ret)
{
    list<PermUserDef*> * rawDatas = permWorker->getUserInfoByUserId(user_id,&ret);
    if(!rawDatas) return NULL;
//    int len = rawDatas->size();

    interface_stru_user_info* returnData = new interface_stru_user_info();
    int i = 0;
    list<PermUserDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        if(0 == i){
            PermUserDef* data = (PermUserDef*)(*iter);
            int userId = -1;
            sscanf(data->userID,"%d",&userId);
            returnData->user_id = userId;
            returnData->user_name = strdup(data->userName);
            returnData->user_alias = strdup(data->userAlias);
            returnData->user_desc = strdup(data->userDesc);
            returnData->user_password_md5 = strdup(data->userPassword);
            int expireDate = -1;
            sscanf(data->userExpireDate,"%d",&expireDate);
            returnData->expire_date = expireDate;
            int ifLock = -1;
            sscanf(data->userIfLock,"%d",&ifLock);
            returnData->if_lock = ifLock;
            int lockTime = -1;
            sscanf(data->userLockTime,"%d",&lockTime);
            returnData->lock_time = lockTime;
            int createDate = -1;
            sscanf(data->userCreateDate,"%d",&createDate);
            returnData->create_date = createDate;

            break;
        }
        i++;
    }
    return returnData;
}

list<PermUserDef *> *SysmgrPermInterface::getUserInfoByUserName(const char *userName, int *ret)
{
    return permWorker->getUserInfoByUserName(userName,ret);
}

interface_stru_user_info *SysmgrPermInterface::get_user_info_by_user_name(const char *user_name, int &ret)
{
    list<PermUserDef*> * rawDatas = permWorker->getUserInfoByUserName(user_name,&ret);
    if(!rawDatas) return NULL;

    interface_stru_user_info* returnData = new interface_stru_user_info();
    int i = 0;
    list<PermUserDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        if(0 == i){
            PermUserDef* data = (PermUserDef*)(*iter);
            int userId = -1;
            sscanf(data->userID,"%d",&userId);
            returnData->user_id = userId;
            returnData->user_name = strdup(data->userName);
            returnData->user_alias = strdup(data->userAlias);
            returnData->user_desc = strdup(data->userDesc);
            returnData->user_password_md5 = strdup(data->userPassword);
            int expireDate = -1;
            sscanf(data->userExpireDate,"%d",&expireDate);
            returnData->expire_date = expireDate;
            int ifLock = -1;
            sscanf(data->userIfLock,"%d",&ifLock);
            returnData->if_lock = ifLock;
            int lockTime = -1;
            sscanf(data->userLockTime,"%d",&lockTime);
            returnData->lock_time = lockTime;
            int createDate = -1;
            sscanf(data->userCreateDate,"%d",&createDate);
            returnData->create_date = createDate;

            break;
        }
        i++;
    }
    return returnData;
}

void SysmgrPermInterface::user_info_delete(interface_stru_user_info *temp_int_user_info)
{
    if(!temp_int_user_info) return;
    if(temp_int_user_info->user_name) delete temp_int_user_info->user_name;
    if(temp_int_user_info->user_alias) delete temp_int_user_info->user_alias;
    if(temp_int_user_info->user_desc) delete temp_int_user_info->user_desc;
    if(temp_int_user_info->user_password_md5) delete temp_int_user_info->user_password_md5;
    delete temp_int_user_info;
}

list<PermUserDef *> *SysmgrPermInterface::getUserInfoAll(int *ret)
{
    return permWorker->getUserInfoAll(ret);
}

interface_stru_user_info **SysmgrPermInterface::get_user_info_all(int &ret)
{
    list<PermUserDef*> * rawDatas = permWorker->getUserInfoAll(&ret);
    if(!rawDatas) return NULL;

    int len = rawDatas->size();
    interface_stru_user_info** returnData = new interface_stru_user_info* [len];
    int i = 0;
    list<PermUserDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        PermUserDef* data = (PermUserDef*)(*iter);
        interface_stru_user_info* temp = new interface_stru_user_info();
        int userId = -1;
        sscanf(data->userID,"%d",&userId);
        temp->user_id = userId;
        temp->user_name = strdup(data->userName);
        temp->user_alias = strdup(data->userAlias);
        temp->user_desc = strdup(data->userDesc);
        temp->user_password_md5 = strdup(data->userPassword);
        int expireDate = -1;
        sscanf(data->userExpireDate,"%d",&expireDate);
        temp->expire_date = expireDate;
        int ifLock = -1;
        sscanf(data->userIfLock,"%d",&ifLock);
        temp->if_lock = ifLock;
        int lockTime = -1;
        sscanf(data->userLockTime,"%d",&lockTime);
        temp->lock_time = lockTime;
        int createDate = -1;
        sscanf(data->userCreateDate,"%d",&createDate);
        temp->create_date = createDate;

        returnData[i] = temp;
        i++;
    }
    return returnData;
}

void SysmgrPermInterface::user_info_delete(interface_stru_user_info **temp_int_user_info, int amount)
{
    if(!temp_int_user_info) return;
    for(int i = 0;i < amount;i++){
        interface_stru_user_info* data = temp_int_user_info[i];
        if(data){
            if(data->user_name) delete data->user_name;
            if(data->user_alias) delete data->user_alias;
            if(data->user_desc) delete data->user_desc;
            if(data->user_password_md5) delete data->user_password_md5;
            delete data;
        }
    }
    delete temp_int_user_info;
}

list<PermUserDef *> *SysmgrPermInterface::getUserInfoByGroup(int groupId, int *ret)
{
    return permWorker->getUserInfoByGroup(groupId,ret);
}

interface_stru_user_info **SysmgrPermInterface::get_user_info_by_group(const int group_id, int &ret)
{
    list<PermUserDef*> * rawDatas = permWorker->getUserInfoByGroup(group_id,&ret);
    if(!rawDatas) return NULL;

    int len = rawDatas->size();
    interface_stru_user_info** returnData = new interface_stru_user_info* [len];
    int i = 0;
    list<PermUserDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        PermUserDef* data = (PermUserDef*)(*iter);
        interface_stru_user_info* temp = new interface_stru_user_info();
        int userId = -1;
        sscanf(data->userID,"%d",&userId);
        temp->user_id = userId;
        temp->user_name = strdup(data->userName);
        temp->user_alias = strdup(data->userAlias);
        temp->user_desc = strdup(data->userDesc);
        temp->user_password_md5 = strdup(data->userPassword);
        int expireDate = -1;
        sscanf(data->userExpireDate,"%d",&expireDate);
        temp->expire_date = expireDate;
        int ifLock = -1;
        sscanf(data->userIfLock,"%d",&ifLock);
        temp->if_lock = ifLock;
        int lockTime = -1;
        sscanf(data->userLockTime,"%d",&lockTime);
        temp->lock_time = lockTime;
        int createDate = -1;
        sscanf(data->userCreateDate,"%d",&createDate);
        temp->create_date = createDate;

        returnData[i] = temp;
        i++;
    }
    return returnData;
}

void SysmgrPermInterface::userInfoDelete(list<PermUserDef *> *datas)
{
    return permWorker->userInfoDelete(datas);
}

int SysmgrPermInterface::deleteUserGroupRelation(const int userId)
{
    return permWorker->deleteUserGroupRelation(userId);
}

int SysmgrPermInterface::deleteUserById(int userId)
{
    return permWorker->deleteUserById(userId);
}

int SysmgrPermInterface::modifyUserPWD(int userId, const char *password)
{
    return permWorker->modifyUserPWD(userId,password);
}

bool SysmgrPermInterface::userUnlock(int userId, const char *password)
{
    return permWorker->userUnlock(userId,password);
}

bool SysmgrPermInterface::userUnlock(const char *userName, const char *password)
{
    return permWorker->userUnlock(userName,password);
}

list<PermGroupDef *> *SysmgrPermInterface::getGroupInfoByGroupId(int groupId, int *ret)
{
    return permWorker->getGroupInfoByGroupId(groupId,ret);
}

interface_stru_group_info *SysmgrPermInterface::get_group_info_by_group_ID(const int group_id, int &ret)
{
    list<PermGroupDef*> * rawDatas = permWorker->getGroupInfoByGroupId(group_id,&ret);
    if(!rawDatas) return NULL;

    interface_stru_group_info* returnData = new interface_stru_group_info();
    int i = 0;
    list<PermGroupDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        if(0 == i){
            PermGroupDef* data = (PermGroupDef*)(*iter);
            int groupId = -1;
            sscanf(data->groupId,"%d",&groupId);
            returnData->perm_group_id = groupId;
            int groupLevel = -1;
            sscanf(data->groupLevel,"%d",&groupLevel);
            returnData->perm_group_level = groupLevel;
            returnData->perm_group_name = strdup(data->groupName);
            returnData->perm_group_pic1 = strdup(data->groupPic1);
            returnData->perm_group_pic2 = strdup(data->groupPic2);
            returnData->perm_group_pic3 = strdup(data->groupPic3);
        }
    }
    return returnData;
}

list<PermGroupDef *> *SysmgrPermInterface::getGroupInfoByGroupName(const char *groupName, int *ret)
{
    return permWorker->getGroupInfoByGroupName(groupName,ret);
}

interface_stru_group_info *SysmgrPermInterface::get_group_info_by_group_name(const char *group_name, int &ret)
{
    list<PermGroupDef*> * rawDatas = permWorker->getGroupInfoByGroupName(group_name,&ret);
    if(!rawDatas) return NULL;

    interface_stru_group_info* returnData = new interface_stru_group_info();
    int i = 0;
    list<PermGroupDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        if(0 == i){
            PermGroupDef* data = (PermGroupDef*)(*iter);
            int groupId = -1;
            sscanf(data->groupId,"%d",&groupId);
            returnData->perm_group_id = groupId;
            int groupLevel = -1;
            sscanf(data->groupLevel,"%d",&groupLevel);
            returnData->perm_group_level = groupLevel;
            returnData->perm_group_name = strdup(data->groupName);
            returnData->perm_group_pic1 = strdup(data->groupPic1);
            returnData->perm_group_pic2 = strdup(data->groupPic2);
            returnData->perm_group_pic3 = strdup(data->groupPic3);
        }
    }
    return returnData;
}

void SysmgrPermInterface::group_info_delete(interface_stru_group_info *temp_int_group_info)
{
    if(!temp_int_group_info) return;
    if(temp_int_group_info->perm_group_name) delete temp_int_group_info->perm_group_name;
    if(temp_int_group_info->perm_group_pic1) delete temp_int_group_info->perm_group_pic1;
    if(temp_int_group_info->perm_group_pic2) delete temp_int_group_info->perm_group_pic2;
    if(temp_int_group_info->perm_group_pic3) delete temp_int_group_info->perm_group_pic3;
    delete temp_int_group_info;
}

list<PermGroupDef *> *SysmgrPermInterface::getGroupInfoAll(int *ret)
{
    return permWorker->getGroupInfoAll(ret);
}

void SysmgrPermInterface::groupInfoDelete(list<PermGroupDef *> *datas)
{
    return permWorker->groupInfoDelete(datas);
}

interface_stru_group_info **SysmgrPermInterface::get_group_info_all(int &ret)
{
    list<PermGroupDef*> * rawDatas = permWorker->getGroupInfoAll(&ret);
    if(!rawDatas) return NULL;

    int len = rawDatas->size();
    interface_stru_group_info** returnData = new interface_stru_group_info* [len];
    int i = 0;
    list<PermGroupDef*>::const_iterator iter;
    for(iter = rawDatas->begin();iter != rawDatas->end();iter++){
        PermGroupDef* data = (PermGroupDef*)(*iter);
        interface_stru_group_info* temp = new interface_stru_group_info();
        int groupId = -1;
        sscanf(data->groupId,"%d",&groupId);
        temp->perm_group_id = groupId;
        int groupLevel = -1;
        sscanf(data->groupLevel,"%d",&groupLevel);
        temp->perm_group_level = groupLevel;
        temp->perm_group_name = strdup(data->groupName);
        temp->perm_group_pic1 = strdup(data->groupPic1);
        temp->perm_group_pic2 = strdup(data->groupPic2);
        temp->perm_group_pic3 = strdup(data->groupPic3);

        returnData[i] = temp;
        i++;
    }
    return returnData;
}

void SysmgrPermInterface::group_info_delete(interface_stru_group_info **temp_int_group_info, int amount)
{
    if(!temp_int_group_info) return;
    for(int i = 0;i < amount;i++){
        interface_stru_group_info* temp = temp_int_group_info[i];
        if(temp){
            if(temp->perm_group_name) delete temp->perm_group_name;
            if(temp->perm_group_pic1) delete temp->perm_group_pic1;
            if(temp->perm_group_pic2) delete temp->perm_group_pic2;
            if(temp->perm_group_pic3) delete temp->perm_group_pic3;
            delete temp;
        }
    }
    delete temp_int_group_info;
}

PermSecurityDef *SysmgrPermInterface::getPermSecurityInfo(int *ret)
{
    return permWorker->getPermSecurityInfo(ret);
}

int SysmgrPermInterface::get_security_def(stru_security_def &temp_sec_def)
{
    int ret = -1;
    PermSecurityDef* rawData = permWorker->getPermSecurityInfo(&ret);
    int permSecurityId = -1;
    sscanf(rawData->permSecurityId,"%d",&permSecurityId);
    temp_sec_def.perm_security_id = permSecurityId;
    int passwdMinLength = -1;  //最小长度
    sscanf(rawData->permPWDMinLength,"%d",&passwdMinLength);
    temp_sec_def.passwd_min_length = passwdMinLength;
    int passwdMinLetter = -1;  //最少字符数
    sscanf(rawData->permPWDMinLetter,"%d",&passwdMinLetter);
    temp_sec_def.passwd_min_letter = passwdMinLetter;
    int passwdMinNumber = -1;  //最少数字数
    sscanf(rawData->permPWDMinNumber,"%d",&passwdMinNumber);
    temp_sec_def.passwd_min_number = passwdMinNumber;
    int accountMaxLock = -1;   //锁定阈值
    sscanf(rawData->permAccountMaxLock,"%d",&accountMaxLock);
    temp_sec_def.account_max_lock = accountMaxLock;
    int accountLockTimer = -1; //锁定时间  单位秒
    sscanf(rawData->permAccountLockTime,"%d",&accountLockTimer);
    temp_sec_def.account_lock_timer = accountLockTimer;
    int accountInvalidTimer = -1;//过期时间
    sscanf(rawData->permAccountInvalidTime,"%d",&accountInvalidTimer);
    temp_sec_def.account_invalid_timer = accountInvalidTimer;
    return ret;
}

void SysmgrPermInterface::init()
{
    initParam();
    initModule();
}

void SysmgrPermInterface::initParam()
{
    cfdbManager = new CFDBManager();
    permWorker = new PermWorkerImpl();
}

void SysmgrPermInterface::initModule()
{
    initCFDBModule();
}

void SysmgrPermInterface::initCFDBModule()
{
    cfdbManager->startCFDBModule();
}
