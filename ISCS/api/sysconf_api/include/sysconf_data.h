/**
 * @file sysconf_data.h
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口。本接口文件定义系统全局配置用到的数据结构。
 *
 **/
#ifndef SYSCONF_DATA_H
#define SYSCONF_DATA_H

#include "sysconf_macros.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/// 系统管理配置相关对象类型定义
typedef struct _sysconf_domain sysconf_domain;
typedef struct _sysconf_nic sysconf_nic;
typedef struct _sysconf_instance sysconf_instance;
typedef struct _sysconf_node sysconf_node;
typedef struct _sysconf_process sysconf_process;
typedef struct _sysconf_system_manager sysconf_system_manager;

/// 域对象
struct _sysconf_domain
{
    int32_t id;                             ///< 域编号
    int32_t type;                           ///< 域类型 @see SC_DOMAIN_TYPE_XXX
    char name[SYSCONF_NAME_MAX_LEN];        ///< 域名称
    char nameCH[SYSCONF_CHNAME_MAX_LEN];    ///< 域中文名
    int32_t nodeNum;                        ///< 域管理的节点个数
    sysconf_node **nodes;                   ///< 节点数组，元素是节点的引用
};

/// 网卡对象
struct _sysconf_nic
{
    int32_t nodeId;                     ///< 网卡所属节点编号
    int32_t id;                         ///< 网卡编号
    char name[SYSCONF_NAME_MAX_LEN];    ///< 网卡名称
    char netmask[SYSCONF_IP_MAX_LEN];   ///< 网络掩码
    char address[SYSCONF_IP_MAX_LEN];   ///< 网络地址
};

/// 进程实例对象
struct _sysconf_instance
{
    int32_t id;                         ///< 进程实例编号
    char param[SYSCONF_PARAM_MAX_LEN];  ///< 进程参数
    sysconf_process *process;           ///< 进程，进程的引用
};

/// 节点对象
struct _sysconf_node
{
    int32_t id;                             ///< 节点编号
    int32_t type;                           ///< 节点类型 @see SC_NODE_TYPE_XXX
    int32_t status;                         ///< 节点状态
    int32_t netStatus;                      ///< 网络状态
    int32_t stationId;                      ///< 车站编号
    char name[SYSCONF_NAME_MAX_LEN];        ///< 节点名称
    char nameCH[SYSCONF_CHNAME_MAX_LEN];    ///< 节点中文名称
    char dev[SYSCONF_NAME_MAX_LEN];         ///< 节点的设备名称
    char os[SYSCONF_NAME_MAX_LEN];          ///< 节点的操作系统
    int32_t nicNum;                         ///< 节点的网卡个数
    sysconf_nic *nics;                      ///< 节点的网卡数组
    int32_t instanceNum;                    ///< 节点的进程实例个数
    sysconf_instance *instances;            ///< 节点的进程实例个数
    sysconf_domain *domain;                 ///< 节点所属域，域的引用
};

/// 进程对象
struct _sysconf_process
{
    int32_t type;                           ///< 进程类型 @see SC_PROC_TYPE_XXX
    int32_t startType;                      ///< 启动类型
    int32_t autoRun;                        ///< 自动运行
    int32_t cycleStart;                     ///< 循环开始
    int32_t cyclePeriod;                    ///< 循环周期
    int32_t offlineRun;                     ///< 离线运行
    int32_t dutyRun;                        ///< 值班运行
    int32_t standbyRun;                     ///< 备份运行
    int32_t runOrder;                       ///< 运行顺序
    int32_t reportType;                     ///< 报告类型
    int32_t runStyle;                       ///< 运行类型
    char alias[SYSCONF_NAME_MAX_LEN];       ///< 进程别名
    char command[SYSCONF_PATH_MAX_LEN];     ///< 进程命令
    char filePath[SYSCONF_PATH_MAX_LEN];    ///< 进程文件路径
    char description[SYSCONF_DESC_MAX_LEN]; ///< 简称描述
};

/// 系统管理配置对象
struct _sysconf_system_manager
{
    int32_t domainNum;          ///< 域个数
    sysconf_domain *domains;    ///< 域数组
    int32_t nodeNum;            ///< 节点个数
    sysconf_node *nodes;        ///< 节点数组
    int32_t processNum;         ///< 进程个数
    sysconf_process *processes; ///< 进程数组
    sysconf_node *native;       ///< 本地节点，节点的引用
};

/// 键值对对象
typedef struct
{
    char key[SYSCONF_KEY_MAX_LEN];      ///< 键值对的键
    char value[SYSCONF_SVAL_MAX_LEN];   ///< 键值对的值
} sysconf_pair;

/// MQ 客户端配置对象
typedef struct
{
    int32_t port;                           ///< 端口
    int32_t poolSize;                       ///< 缓存大小
    int32_t timeout;                        ///< 超时
    int32_t heartbeatInterval;              ///< 心跳周期
    char schema[SYSCONF_NAME_MAX_LEN];      ///< 协议
    char userName[SYSCONF_NAME_MAX_LEN];    ///< 用户名
    char password[SYSCONF_NAME_MAX_LEN];    ///< 密码
    int32_t pairNum;                        ///< 键值对个数
    sysconf_pair *pairs;                    ///< 键值对数组
    int32_t nicNum;                         ///< 网卡个数
    sysconf_nic **nics;                     ///< 网卡数组，元素是网卡的引用
} sysconf_mq_client;

// RTDB 客户端配置对象
typedef struct
{
    int32_t rtdbPort;                       ///< RTDB 端口
    int32_t safePort;                       ///< 安全控制端口
    char userName[SYSCONF_NAME_MAX_LEN];    ///< 用户名
    char password[SYSCONF_NAME_MAX_LEN];    ///< 密码
    int32_t pairNum;                        ///< 键值对个数
    sysconf_pair *pairs;                    ///< 键值对数组
    int32_t nicNum;                         ///< 网卡个数
    sysconf_nic **nics;                     ///< 网卡数组，元素是网卡的引用
} sysconf_rtdb_client;

// TSDB 客户端配置对象
typedef struct
{
    int32_t port;                           ///< TSDB 端口
    int32_t timeout;                        ///< 超时
    int32_t heartbeatInterval;              ///< 心跳间隔
    char schema[SYSCONF_NAME_MAX_LEN];      ///< 协议
    char userName[SYSCONF_NAME_MAX_LEN];    ///< 用户名
    char password[SYSCONF_NAME_MAX_LEN];    ///< 密码
    char database[SYSCONF_NAME_MAX_LEN];    ///< 数据库
    int32_t nicNum;                         ///< 网卡个数
    sysconf_nic **nics;                     ///< 网卡数组，元素是网卡的引用
} sysconf_tsdb_client;

// 配置库服务访问配置
typedef struct
{
    int32_t confPort;                   ///< 配置服务端口
    int32_t syncPort;                   ///< 同步服务接口
    char urlPath[SYSCONF_PATH_MAX_LEN]; ///< URL路径
    int32_t nicNum;                     ///< 网卡个数
    sysconf_nic **nics;                 ///< 网卡数组，元素是网卡的引用
} sysconf_cfdb_client;

// 进程管理客户端配置
typedef struct
{
    int32_t port;
    int32_t switchPeriod;
    int32_t logonPeriod;
    int32_t maxClients;
    int32_t maxIdleTime;
    int32_t tcpBacklog;
    int32_t tcpKeepAlive;
    int32_t backGround;
    int32_t localDomain;
    int32_t nicNum;         ///< 网卡个数
    sysconf_nic **nics;     ///< 网卡数组，元素是网卡的引用
} sysconf_pm_client;

#ifdef __cplusplus
}
#endif
#endif
