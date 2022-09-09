#ifndef PERMDEFINE_H
#define PERMDEFINE_H

#include <list>
using namespace std;

#define PERM_NOM_FUNC_TYPE               1       //!<权限类型：普通功能
#define PERM_HMI_FUNC_TYPE               2       //!<权限类型：画面功能
#define PERM_REPORT_FUNC_TYPE            3       //!<权限类型：报表功能
#define PERM_SPE_FUNC_TYPE               4       //!<权限类型：特殊功能

#define PERM_NOM_FUNC_ID               1       //!<权限请求类型：普通功能ID
#define PERM_HMI_FUNC_ID               2       //!<权限请求类型：画面类型ID
#define PERM_REPORT_FUNC_ID            3       //!<权限请求类型：报表操作ID
#define PERM_SPE_FUNC_ID               4       //!<权限请求类型：特殊功能ID


#define PERM_NOM_FUNC_DEF              5       //!<权限请求类型：普通功能ID
#define PERM_HMI_FUNC_DEF              6       //!<权限请求类型：画面类型ID
#define PERM_REPORT_FUNC_DEF           7       //!<权限请求类型：报表操作ID
#define PERM_SPE_FUNC_DEF              8       //!<权限请求类型：特殊功能ID



#define PERM_DEPT                   9       //!<权限类型：部门

//权限请求结果
#define PERM_NORMAL                 0       //!<正常返回
#define PERM_FORBIT                 1       //!<用户不具有指定权限
#define PERM_PERMIT                 2       //!<用户具有指定权限
#define PERM_ERROR                  -1      //!<权限接口错误

/***
 * error message
 *
 * */
#define PERM_USER_IS_EXISTED        -997    //!<用户名已存在
#define PERM_NO_USER_INFO           -998    //!<无用户信息
#define PERM_PARAM_ILLEGAL          -999    //!<参数非法
#define PERM_NO_USER_LOGIN          -1000   //!<无用户登录信息
#define PERM_NO_GROUP_INFO          -1001   //!<无用户组对应信息
#define PERM_GROUP_ILLEGAL          -1002   //!<用户组匹配失败
#define PERM_GROUP_NO_STATION       -1003   //!<用户组无可登陆车站信息
#define PERM_GROUP_NO_NOM_PERM      -1004   //!<用户组一般权限未找到
#define PERM_GROUP_NO_SPE_PERM      -1005   //!<用户组特殊权限未找到


#define PERM_NO_NAME                -1006   //!<输入名称不存在
#define PERM_REDUP_NAME             -1007   //!<输入名称不唯一

#define PERM_NODE_DENY              -1010   //!<不允许在该车站登录
#define PERM_PASSWORD_ERROR         -1011   //!<用户口令错误
#define PERM_USER_EXPIRED           -1012   //!<用户已失效
#define PERM_USER_LOCKED            -1013   //!<用户已锁定
#define PERM_USERG_ERROR            -1014   //!<用户不属于所选用户组

#define PERM_USER_LOGINED           -1015   //!<用户已处于合法登陆状态，不允许异地登陆
#define PERM_PERM_TYPE_ERR          -1016   //!<错误的权限请求类型
#define PERM_PERM_DEF_ERR           -1017   //!<错误的权限请求宏定义
#define PERM_PERM_AREA_ERR          -1018   //!<找不到指定权限的区域信息
#define PERM_DATABASE_ERROR         -2000   //!<用户权限数据库操作错误
#define PERM_CFDB_ADD_USER_ERROR    -1019   //!<用户数据库新增加用户返回错误
#define PERM_RTDB_USER_EXISTED      -1020   //!<用户已在缓存中存在

#define PERM_USER_LOGIN_EXPIRED     1       //!用户登陆已失效
#define PERM_USER_LOGIN_INTIME      0       //!用户登陆状态有效



#define PERM_ONE_USER_MORE_GROUP  1
#define PERM_ONE_GROUP_MORE_USER  2


#define SUCCESS                1
#define FAIL                   -1

#define DEFAULT_EXPIRED_TIMER  1839600 //24 * 60 * 60 *365  for one year

//权限定义表
//struct stru_nom_func_def
//{
//  int       perm_id;                                   //权限ID号
//  int       perm_type_id;                              //权限类型
//  char*    perm_name;                                 //权限名称
//  char*    perm_define;                               //权限宏定义

//  stru_nom_func_def (){
//      perm_id = -1;
//      perm_type_id = -1;
//      perm_define = "";
//      perm_name   = "";
//  }
//};

//特殊权限请求定义
//struct stru_spe_func_def : public stru_nom_func_def
//{
//    int perm_region_id;                 //责任区ID号
//    int perm_station_id;               //车站ID号

//    stru_spe_func_def():stru_nom_func_def(){
//        this->perm_region_id   = -1;
//        this->perm_station_id = -1;
//    }
//};

struct PermUserDef{
    char* userID;
    char* userName;
    char* userAlias;
    char* userPassword;
    char* userCreateDate;
    char* userExpireDate;
    char* userDesc;
    char* userIfLock;
    char* userLockTime;
};

struct PermGroupDef{
    char* groupId;
    char* groupName;
    char* groupLevel;
    char* groupDesc;
    char* groupPic1;
    char* groupPic2;
    char* groupPic3;
};

struct PermSecurityDef{
    char* permSecurityId;
    char* permPWDMinLength;
    char* permPWDMinLetter;
    char* permPWDMinNumber;
    char* permAccountMaxLock;
    char* permAccountLockTime;
    char* permAccountInvalidTime;
};

struct PermTypeDef{
    char* permTypeId;
    char* permTypeName;
};

struct PermFuncDef{
    char* permId;
    char* permName;
    char* permDefine;
    char* permTypeName;
    char* permTypeId;
};

struct PermGroupRelation{
    char* relationId;
    char* groupId;
    char* permId;
    char* permDefine;
};

struct PermUserGroupDef{
    char* relationId;
    char* userId;
    char* groupId;
    char* ifLead;
};

struct PermSpecGroupStationRelation{
    char* relationId;
    char* groupId;
    char* permId;
    char* stationId;
    char* permDefine;
};

struct PermSpecGroupRegionRelation{
    char* relationId;
    char* groupId;
    char* permId;
    char* regionId;
    char* permDefine;
};

//combine
struct PermSpecStationRegion{
    list<PermSpecGroupStationRelation*> * specStations;
    list<PermSpecGroupRegionRelation*> * specRegions;
};

#endif // PERMDEFINE_H
