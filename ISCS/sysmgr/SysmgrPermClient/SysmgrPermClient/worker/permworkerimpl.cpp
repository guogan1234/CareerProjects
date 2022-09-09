#include "permworkerimpl.h"
#include "manager/permuserdefmanager.h"
#include "manager/permgroupdefmanager.h"
#include "manager/permsecuritydefmanager.h"
#include "manager/permfuncdefmanager.h"
#include "manager/permusergroupdefmanager.h"
#include "manager/permgrouprelationmanager.h"
#include "manager/permspecgroupstationmanager.h"
#include "manager/permspecgroupregionmanager.h"
#include <stdio.h>
#include <string.h>

PermWorkerImpl::PermWorkerImpl()
{

}

int PermWorkerImpl::permCheck(int groupId, int permType, const char *arg)
{
    int permId = -1;
    switch (permType) {
    case PERM_NOM_FUNC_ID:
    case PERM_HMI_FUNC_ID:
    case PERM_REPORT_FUNC_ID:
    {
        permId = *((int*)arg);
        PermGroupRelationManager geneManager;
        return geneManager.checkPermByGroupIdAndPermId(groupId,permId);
    }
        break;
    case PERM_SPE_FUNC_ID:
    {
        permId = *((int*)arg);
        int ret_1 = -1;int ret_2 = -1;
        PermSpecGroupStationManager specStationManager;
        ret_1 = specStationManager.checkPermByGroupIdAndPermId(groupId,permId);
        PermSpecGroupRegionManager specRegionManager;
        ret_2 = specRegionManager.checkPermByGroupIdAndPermId(groupId,permId);
        if(ret_1 == PERM_ERROR || ret_2 == PERM_ERROR){
            return PERM_ERROR;
        }
        if(ret_1 == PERM_PERMIT || ret_2 == PERM_PERMIT){
            return PERM_PERMIT;
        }else{
            return PERM_FORBIT;
        }
    }
        break;
    case PERM_NOM_FUNC_DEF:
    case PERM_HMI_FUNC_DEF:
    case PERM_REPORT_FUNC_DEF:
    {
        PermGroupRelationManager geneManager;
        return geneManager.checkPermByGroupIdAndPermDefine(groupId,arg);
    }
        break;
    case PERM_SPE_FUNC_DEF:
    {
        int ret_1 = -1;int ret_2 = -1;
        PermSpecGroupStationManager specStationManager;
        ret_1 = specStationManager.checkPermByGroupIdAndPermDefine(groupId,arg);
        PermSpecGroupRegionManager specRegionManager;
        ret_2 = specRegionManager.checkPermByGroupIdAndPermDefine(groupId,arg);
        if(ret_1 == PERM_ERROR || ret_2 == PERM_ERROR){
            return PERM_ERROR;
        }
        if(ret_1 == PERM_PERMIT || ret_2 == PERM_PERMIT){
            return PERM_PERMIT;
        }else{
            return PERM_FORBIT;
        }
    }
        break;
    default:
        printf("[permType is error!]\n");
        break;
    }
    return PERM_ERROR;
}

int PermWorkerImpl::permCheck(int groupId, int permType, const char **arg, int permSize, int *ret)
{
    switch (permType) {
    case PERM_NOM_FUNC_ID:
    case PERM_HMI_FUNC_ID:
    case PERM_REPORT_FUNC_ID:
    {
        PermGroupRelationManager manager;
        manager.loadDataByGroupId(groupId);
        printf("[general perm ids list check!]\n");
        for(int i = 0;i < permSize;i++){
            int **argInt = (int**)arg;
            int * permIdPt = argInt[i];
            int result = manager.checkPermByPermId(*permIdPt);
            ret[i] = result;
        }
    }
        break;
    case PERM_SPE_FUNC_ID:
    {
        PermSpecGroupStationManager stationManager;
        stationManager.loadDataByGroupId(groupId);
        PermSpecGroupRegionManager regionManager;
        regionManager.loadDataByGroupId(groupId);
        printf("[special perm ids list check!]\n");
        for(int i = 0;i < permSize;i++){
            stru_spe_func_def **argStructs = (stru_spe_func_def**)arg;
            stru_spe_func_def *argStruct = argStructs[i];
            int stationResult = stationManager.checkPermByPermIdAndStationId(argStruct->perm_id,argStruct->perm_station_id);
            int regionResult = regionManager.checkPermByPermIdAndRegionId(argStruct->perm_id,argStruct->perm_region_id);
            if(stationResult == PERM_PERMIT && regionResult == PERM_PERMIT){
                ret[i] = PERM_PERMIT;
            }else{
                ret[i] = PERM_FORBIT;
            }
        }
    }
        break;
    case PERM_NOM_FUNC_DEF:
    case PERM_HMI_FUNC_DEF:
    case PERM_REPORT_FUNC_DEF:
    {
        PermGroupRelationManager manager;
        manager.loadDataByGroupId(groupId);
        printf("[general perm defines list check!]\n");
        for(int i = 0;i < permSize;i++){
            ret[i] = manager.checkPermByPermDefine(arg[i]);
        }
    }
        break;
    case PERM_SPE_FUNC_DEF:
    {
        PermSpecGroupStationManager stationManager;
        stationManager.loadDataByGroupId(groupId);
        PermSpecGroupRegionManager regionManager;
        regionManager.loadDataByGroupId(groupId);
        printf("[special perm defines list check!]\n");
        for(int i = 0;i < permSize;i++){
            stru_spe_func_def **argStructs = (stru_spe_func_def**)arg;
            stru_spe_func_def *argStruct = argStructs[i];
            int stationResult = stationManager.checkPermByPermDefineAndStationId(argStruct->perm_define,argStruct->perm_station_id);
            int regionResult = regionManager.checkPermByPermDefineAndRegionId(argStruct->perm_define,argStruct->perm_region_id);
            if(stationResult == PERM_PERMIT && regionResult == PERM_PERMIT){
                ret[i] = PERM_PERMIT;
            }else{
                ret[i] = PERM_FORBIT;
            }
        }
    }
        break;
    default:
        break;
    }
    return 0;
}

list<PermFuncDef *> *PermWorkerImpl::getGeneralPermByUserId(int userId, int permType, int *ret)
{
    if(!ret) return NULL;
    PermUserGroupDefManager userGroupManager;
    list<int> * groupIds = userGroupManager.getGroupIdsByUserId(userId);
    PermGroupRelationManager groupManager;
    list<int> * permIds = groupManager.getPermIdsByGroupId(groupIds);
    PermFuncDefManager permManager;
    return permManager.getInfoByPermIdAndPermTypeId(permIds,permType,ret);
}

list<PermSpecStationRegion *> *PermWorkerImpl::getSpecialPermByUserId(int userId, int *ret)
{
    if(!ret) return NULL;
    PermUserGroupDefManager userGroupManager;
    list<int> * groupIds = userGroupManager.getGroupIdsByUserId(userId);
    PermSpecGroupStationManager specStationManager;
    list<PermSpecGroupStationRelation*> * specStations = specStationManager.getPermSpecStationsByGroupIds(groupIds);
    PermSpecGroupRegionManager specRegionManager;
    list<PermSpecGroupRegionRelation*> * specRegions = specRegionManager.getPermSpecRegionsByGroupIds(groupIds);
    if(!specStations && !specRegions){
        *ret = -1;
        return NULL;
    }else{
        list<PermSpecStationRegion*> * retValue = new list<PermSpecStationRegion*>();
        PermSpecStationRegion* specStationRegion = new PermSpecStationRegion();
        specStationRegion->specStations = specStations;
        specStationRegion->specRegions = specRegions;
        retValue->push_back(specStationRegion);
        *ret = 0;

        //delete
        specStationManager.deleteInfoList(specStations);
        specRegionManager.deleteInfoList(specRegions);
        return retValue;
    }
}

spec_func_info_get *PermWorkerImpl::get_spe_perm_by_userg_id(const int userg_ID, int &ret)
{
    PermUserGroupDefManager userGroupManager;
    list<int> * groupIds = userGroupManager.getGroupIdsByUserId(userg_ID);
    if(!groupIds) return NULL;
    PermGroupRelationManager permGroupManager;
    list<int> * permIds = permGroupManager.getPermIdsByGroupId(groupIds);
    if(!permIds) return NULL;
    PermFuncDefManager permInfoManager;
    list<PermFuncDef*> * permInfos = permInfoManager.getInfoByPermIds(permIds);
    if(!permInfos) return NULL;
    spec_func_info_get* retData = new spec_func_info_get();
    int perm_amount = permIds->size();
    if(!perm_amount){
        printf("[get_spe_perm_by_userg_id]perm_amount is 0!");
        return NULL;
    }
    retData->perm_amount = perm_amount;

    int* perm_id = new int[perm_amount];
    char** perm_define = new char*[perm_amount];
    char** perm_name = new char*[perm_amount];

    int permInfosSize = permInfos->size();
    if(permInfosSize != perm_amount){
        printf("[get_spe_perm_by_userg_id]perm_amount isn't equal permInfosSize!");
        return NULL;
    }
    list<PermFuncDef*>::const_iterator infoIter;
    int a = 0;
    for(infoIter = permInfos->begin();infoIter != permInfos->end();infoIter++){
        PermFuncDef* temp = (PermFuncDef*)(*infoIter);
        perm_define[a] = strdup(temp->permDefine);
        perm_name[a] = strdup(temp->permName);
    }
    retData->perm_define = perm_define;
    retData->perm_name = perm_name;
    //delete
    permInfoManager.deleteInfoList(permInfos);

    int* station_amount = new int[perm_amount];
    int** station_id = new int*[perm_amount];
    int* region_amount = new int[perm_amount];
    int** region_id = new int*[perm_amount];
    PermSpecGroupStationManager specStationManager;
    PermSpecGroupRegionManager specRegionManager;
    list<int>::const_iterator idIter;
    int i = 0;
    for(idIter = permIds->begin();idIter != permIds->end();idIter++){
        int permId = *idIter;
        perm_id[i] = permId;
        list<PermSpecGroupStationRelation*> * specStations = specStationManager.getPermSpecStationsByPermId(permId);
        if(!specStations){
            station_amount[i] = 0;
            station_id[i] = NULL;
        }
        list<PermSpecGroupRegionRelation*> * specRegions = specRegionManager.getPermSpecRegionsByPermId(permId);
        if(!specRegions){
            region_amount[i] = 0;
            region_id[i] = NULL;

            i++;
            continue;
        }
        int stationSize = specStations->size();
        int regionSize = specRegions->size();
        station_id[i] = new int[stationSize];
        region_id[i] = new int[regionSize];

        station_amount[i] = specStations->size();
        region_amount[i] = specRegions->size();
        list<PermSpecGroupStationRelation*>::const_iterator stationIter;
        int m = 0;
        for(stationIter = specStations->begin();stationIter != specStations->end();stationIter++){
            PermSpecGroupStationRelation* temp = (PermSpecGroupStationRelation*)(*stationIter);
            int stationId = -1;
            sscanf(temp->stationId,"%d",&stationId);
            station_id[i][m] = stationId;

            m++;
        }

        list<PermSpecGroupRegionRelation*>::const_iterator regionIter;
        int n = 0;
        for(regionIter = specRegions->begin();regionIter != specRegions->end();regionIter++){
            PermSpecGroupRegionRelation* temp = (PermSpecGroupRegionRelation*)(*regionIter);
            int regionId = -1;
            sscanf(temp->regionId,"%d",&regionId);
            region_id[i][n] = regionId;

            n++;
        }

//        //delete
//        specStationManager.deleteInfoList(specStations);
//        specRegionManager.deleteInfoList(specRegions);
        i++;
    }
    retData->station_amount = station_amount;
    retData->station_id = station_id;
    retData->region_amount = region_amount;
    retData->region_id = region_id;
    return retData;
}

list<PermUserDef *> *PermWorkerImpl::getUserInfoByUserId(int userId, int *ret)
{
    if(!ret) return NULL;
//    PermUserDefManager* manager = new PermUserDefManager();
    PermUserDefManager manager;
    return manager.loadUserInfoById(userId,ret);
}

list<PermUserDef *> *PermWorkerImpl::getUserInfoByUserName(const char *userName, int *ret)
{
    if(!userName || !ret) return NULL;
    PermUserDefManager manager;
    return manager.loadUserInfoByName(userName,ret);
}

list<PermUserDef *> *PermWorkerImpl::getUserInfoAll(int *ret)
{
    if(!ret) return NULL;
    PermUserDefManager manager;
    return manager.loadAll(ret);
}

list<PermUserDef *> *PermWorkerImpl::getUserInfoByGroup(int groupId, int *ret)
{
    return NULL;
}

void PermWorkerImpl::userInfoDelete(list<PermUserDef *> *datas)
{
    if(!datas) return;
    PermUserDefManager manager;
    return manager.releaseUserInfo(datas);
}

int PermWorkerImpl::deleteUserById(int userId)
{
    PermUserDefManager manager;
    int ret = -1;
    return manager.delUserById(userId,&ret);
}

int PermWorkerImpl::deleteUserGroupRelation(const int userId)
{
    PermUserGroupDefManager manager;
    return manager.delUserGroupDefByUserId(userId);
}

int PermWorkerImpl::modifyUserPWD(int userId, const char *password)
{
    if(!password) return -1;
    PermUserDefManager manager;
    return manager.modifyUserPWD(userId,password);
}

bool PermWorkerImpl::userUnlock(int userId, const char *password)
{
    if(!password) return false;
    PermUserDefManager manager;
    return manager.userUnlock(userId,password);
}

bool PermWorkerImpl::userUnlock(const char *userName, const char *password)
{
    if(!userName || !password) return false;
    PermUserDefManager manager;
    return manager.userUnlock(userName,password);
}

list<PermGroupDef *> *PermWorkerImpl::getGroupInfoByGroupId(int groupId, int *ret)
{
    if(!ret) return NULL;
    PermGroupDefManager manager;
    return manager.loadGroupInfoById(groupId,ret);
}

list<PermGroupDef *> *PermWorkerImpl::getGroupInfoByGroupName(const char *groupName, int *ret)
{
    if(!groupName || !ret) return NULL;
    PermGroupDefManager manager;
    return manager.loadGroupInfoByName(groupName,ret);
}

list<PermGroupDef *> *PermWorkerImpl::getGroupInfoAll(int *ret)
{
    if(!ret) return NULL;
    PermGroupDefManager manager;
    return manager.loadAll(ret);
}

void PermWorkerImpl::groupInfoDelete(list<PermGroupDef *> *datas)
{
    return;
}

PermSecurityDef *PermWorkerImpl::getPermSecurityInfo(int *ret)
{
    if(!ret) return NULL;
    PermSecurityDefManager manager;
    return manager.getPermSecurityInfo(ret);
}
