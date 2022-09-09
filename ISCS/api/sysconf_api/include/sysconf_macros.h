/**
 * @file sysconf_macros.h
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口。本接口文件定义系统全局配置用到的宏定义。
 *
 **/
#ifndef SYSCONF_MACROS_H
#define SYSCONF_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _MSC_VER
#ifdef USE_DLL
#define SYSCONF_API __declspec(dllexport)
#else
#define SYSCONF_API __declspec(dllimport)
#endif
#else
#define SYSCONF_API
#endif

#define SYSCONF_KEY_MAX_LEN     96  ///< 定义键最大长度
#define SYSCONF_SVAL_MAX_LEN    32  ///< 定义值最大长度
#define SYSCONF_DESC_MAX_LEN    512 ///< 定义描述的最大长度
#define SYSCONF_PATH_MAX_LEN    512 ///< 定义文件路径最大长度
#define SYSCONF_PARAM_MAX_LEN   256 ///< 定义参数最大长度
#define SYSCONF_NAME_MAX_LEN    64  ///< 定义名称的最大长度
#define SYSCONF_CHNAME_MAX_LEN  128 ///< 定义中文名称最大长度
#define SYSCONF_NIC_MAX_NUM     2   ///< 定义一个节点拥有的最大网卡个数
#define SYSCONF_IP_MAX_LEN      40  ///< 定义IP地址最大长度（兼容IPV6）

/// 域类型定义
enum
{
    SYSCONF_DOM_TYPE_CENTER = 0,  ///< 定义中心域类型
    SYSCONF_DOM_TYPE_STATION = 1, ///< 定义车站域类型
    SYSCONF_DOM_TYPE_UNDEF = 2    ///< 定义未定义类型
};

/// 节点类型定义
enum
{
    SYSCONF_NOD_TYPE_REALTIME = 1,  ///< 实时服务器节点类型
    SYSCONF_NOD_TYPE_HISTORY = 2,   ///< 历史服务器节点类型
    SYSCONF_NOD_TYPE_WORKBENCH = 3, ///< 操作工作台类型
    SYSCONF_NOD_TYPE_MIXED = 4      ///< 实时兼历史节点类型
};

/// 进程类型定义
enum
{
    SYSCONF_PROC_TYPE_UNDEF = -1,       ///< 未定义的进程模板类型
    SYSCONF_PROC_TYPE_PM = 0x10,        ///< 定义进程管理进程类型
    SYSCONF_PROC_TYPE_DM = 0x11,        ///< 定义设备管理进程类型
    SYSCONF_PROC_TYPE_UM = 0x12,        ///< 定义用户管理进程类型
    SYSCONF_PROC_TYPE_RTDB = 0x20,      ///< 定义RTDB服务进程类型
    SYSCONF_PROC_TYPE_SAFE = 0x21,      ///< 定义RTDB服务进程类型
    SYSCONF_PROC_TYPE_TSDB = 0x30,      ///< 定义历史库服务进程类型
    SYSCONF_PROC_TYPE_CFDB = 0x40,      ///< 定义配置库服务进程类型
    SYSCONF_PROC_TYPE_MQ = 0x50,        ///< 定义消息中间件服务进程类型
    SYSCONF_PROC_TYPE_SUBSYS = 0x60,    ///< 定义专业服务进程类型
    SYSCONF_PROC_TYPE_FES = 0x70,       ///< 定义前置处理服务进程类型
    SYSCONF_PROC_TYPE_PROXY = 0x80      ///< 定会代理服务进程类型
};

/// 错误状态码定义
enum
{
    SYSCONF_EC_ERR = -1,        ///< 错误
    SYSCONF_EC_OK = 0,          ///< 成功
    SYSCONF_EC_CFGERR = 1,      ///< 配置文件错误
    SYSCONF_EC_NOEXIST = 2,     ///< 不存在
    SYSCONF_EC_ARGERR = 3,      ///< 参数错误
    SYSCONF_EC_NULLPTR = 4,     ///< 空指针
    SYSCONF_EC_OOM = 5,         ///< 内存溢出
    SYSCONF_EC_NOINIT = 6,      ///< 未初始化
    SYSCONF_EC_LOGERR = 7,      ///< 日志错误
    SYSCONF_EC_TOLONG = 8,      ///< 长度太长
    SYSCONF_EC_EMPTY = 9,       ///< 空
    SYSCONF_EC_NONATIVE = 10,   ///< 无本地节点
    SYSCONF_EC_NOPROC = 11      ///< 无进程
};

#ifdef __cplusplus
}
#endif
#endif
