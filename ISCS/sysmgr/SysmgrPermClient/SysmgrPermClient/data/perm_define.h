#ifndef PERM_DEFINE_H
#define PERM_DEFINE_H
/**********************
 *定义权限相关的基本宏定义
 * Author liutao
 * Time   2018-05-15
**********************/
//#include <string>
//#include <vector>
//#include <map>
//#include <iostream>
//#include <string.h>
//#include "time.h"
//using namespace std;



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





/*
 ***0821 modify
 ***add login info to user info
 ***by liutao
 */
struct interface_stru_user_info
{
   //static
    int    user_id;             //用户ID
    char * user_name;           //用户名称
    char * user_alias;          //用户全名
    char * user_desc;           //用户描述
    char * user_password_md5;   //用户加密密文
    int    expire_date;         //失效日期
    int    if_lock;             //是否锁定
    int    lock_time;           //锁定动作执行时的时间 1970年1月1日之后的秒数
    int    create_date;         //创建日期

    //dynamic
    int  if_login;            //登陆状态 0 未登陆 1 登陆 -1 未知
    int  login_err_no;        //用户登陆错误次数
    int  login_time;          //登陆时间
    int  login_expire_time;   //登陆失效时间 登陆时间 + 本次允许在线时间
    int  login_station_id;    //登陆车站ID

    interface_stru_user_info()
    {
        user_id = -1;
        expire_date  = -1;
        if_lock   = -1;
        lock_time = -1;
        create_date = -1;

        if_login  = -1;
        login_err_no = 0;
        login_time   = -1;
        login_expire_time = -1;
        login_station_id  = -1;
    }
//    interface_stru_user_info operator =(const interface_stru_user_info & value)
//    {
//        this->user_id = value.user_id;
//        this->expire_date = value.expire_date;
//        this->create_date = value.create_date;
//        this->if_lock    =  value.if_lock;
//        this->lock_time  = value.lock_time;

//        this->if_login   = value.if_login;
//        this->login_err_no = value.login_err_no;
//        this->login_time = value.login_time;
//        this->login_expire_time = value.login_expire_time;
//        this->login_station_id = value.login_station_id;

//        strcpy(this->user_name,value.user_name);
//        strcpy(this->user_desc,value.user_desc);
//        strcpy(this->user_alias,value.user_alias);
//        strcpy(this->user_password_md5,value.user_password_md5);
//        return *this;
//    }
};

//用户组定义表  interface
// add by liutao 2018.827
struct interface_stru_group_info
{

   int       perm_group_id;
   char *    perm_group_name;
   int       perm_group_level;
   char *    perm_group_pic1;  //屏1 登陆画面
   char *    perm_group_pic2;  //屏2 登陆画面
   char *    perm_group_pic3;  //屏3 登陆画面


   interface_stru_group_info()
   {
       perm_group_id        = -1;
       perm_group_level     = -1;
   }
};

struct interface_one_user_more_group
{
  int         group_amount;
  int         perm_user_id;
  int *       perm_group_id;
  int *       perm_if_group_leader; //是否组长
  interface_one_user_more_group(){
     group_amount   = 0;
     perm_user_id   = 0;
  }
};

struct interface_more_user_one_group
{
  int         user_amount;
  int         perm_group_id;
  int *       perm_user_id;
  int *       perm_if_group_leader; //是否组长
  interface_more_user_one_group(){
     user_amount     = 0;
     perm_group_id   = 0;
  }
};


//权限定义表
struct stru_nom_func_def
{
  int       perm_id;                                   //权限ID号
  int       perm_type_id;                              //权限类型
//  string    perm_name;                                 //权限名称
//  string    perm_define;                               //权限宏定义
  char*    perm_name;                                 //权限名称
  char*    perm_define;                               //权限宏定义

  stru_nom_func_def (){
      perm_id = -1;
      perm_type_id = -1;
      perm_define = "";
      perm_name   = "";
  }

};


//特殊权限请求定义

struct stru_spe_func_def : public stru_nom_func_def
{
    int perm_region_id;                 //责任区ID号
    int perm_station_id;               //车站ID号


    stru_spe_func_def():stru_nom_func_def(){
        this->perm_region_id   = -1;
        this->perm_station_id = -1;
    }
};


//用户组普通权限返回值
struct nom_func_info_get
{
  int        perm_amount;       //权限数量
  int *      perm_id;           //权限ID号
  char **    perm_define;       //权限宏定义
  char **    perm_name;         //权限描述
};

//用户组特殊权限返回
struct spec_func_info_get : public nom_func_info_get
{
  int *        station_amount;    //车站数量 for every perm
  int **       station_id;        //车站ID
  int *        region_amount;     //区域数量 for every perm
  int **       region_id;         //区域ID
};

struct login_info_get
{
   int                 user_id;
   int                 group_amount;
   int *               group_id;
   char **             group_name;
};
//登陆结果信息
struct  sys_login_result
{
  int  login_result;     //登陆结果
  int  extra_info;       //附加信息 已登陆车站ID
  int  group_level;      //组level
  char * group_pic[3];   //该用户初始化三个屏幕界面分配

};

//安全策略定义表
struct stru_security_def
{
    int perm_security_id;
    int passwd_min_length;  //最小长度
    int passwd_min_letter;  //最少字符数
    int passwd_min_number;  //最少数字数
    int account_max_lock;   //锁定阈值
    int account_lock_timer; //锁定时间  单位秒
    int account_invalid_timer;//过期时间
    stru_security_def(){
        perm_security_id = -1;
        passwd_min_length = 0;
        passwd_min_letter = 0;
        passwd_min_number = 0;
        account_max_lock  = 0;
        account_lock_timer = 0;
        account_invalid_timer = 0;
    }

};


#endif // PERM_DEFINE_H
