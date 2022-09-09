/**
 * @file rtdbcli_macros.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本文件定义客户端用到的常量和宏。
 **/
#ifndef RTDBCLI_MACROS_H
#define RTDBCLI_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _MSC_VER
#ifdef USE_DLL
#define RTDBCLI_API __declspec(dllexport)
#else
#define RTDBCLI_API __declspec(dllimport)
#endif
#else
#define RTDBCLI_API
#endif

#define RTDBCLI_KEY_MAX_LEN         64      ///< 定义属性键的最大长度
#define RTDBCLI_SOETM_MAX_LEN       24      ///< 定义SOE时标最大长度
#define RTDBCLI_STRVAL_MAX_LEN      4000    ///< 定义字符串支持的最大长度
#define RTDBCLI_PATH_MAX_LEN        256     ///< 定义点路径的最大长度
#define RTDBCLI_FIELD_MAX_NUM       100     ///< 定义一条记录支持的最大字段数
#define RTDBCLI_RECORD_MAX_NUM      10000   ///< 定义接口支持的最大记录数
#define RTDBCLI_SUBSCRIBER_MAX_NUM  6000    ///< 定义接口支持的最大订阅者数

/// 客户端类型定义
enum
{
    RTDBCLI_TYPE_RTDB = 0,      ///< 实时数据库客户端
    RTDBCLI_TYPE_FES = 1,       ///< 前置服务客户端
    RTDBCLI_TYPE_SFES = 2,      ///< 安全前置服务客户端
    RTDBCLI_TYPE_SCTRL = 3      ///< 安全控制客户端
};

/// 目标系统类型定义
enum
{
    RTDBCLI_DEST_RTDB = 1,      ///< RTDB
    RTDBCLI_DEST_PLUGIN = 2,    ///< 插件
    RTDBCLI_DEST_FES = 3,       ///< FES
    RTDBCLI_DEST_EXTPLUGIN = 4  ///< 外部插件(RTDB服务外部的进程)
};

/// 定义数据订阅中表记录的发布类型1 添加；2 更新； 3 删除
enum
{
    RTDBCLI_NOTIFY_ADD = 1,     ///< 添加表记录通知
    RTDBCLI_NOTIFY_UPDATE = 2,  ///< 更新表记录通知
    RTDBCLI_NOTIFY_DELETE = 3   ///< 删除表记录通知
};

/// 定义属性的数据类型
enum
{
    RTDBCLI_ATTR_UNDEF = 0,
    RTDBCLI_ATTR_BOOL = 1,
    RTDBCLI_ATTR_INT8 = 2,
    RTDBCLI_ATTR_INT16 = 3,
    RTDBCLI_ATTR_INT32 = 4,
    RTDBCLI_ATTR_INT64 = 5,
    RTDBCLI_ATTR_FLOAT = 6,
    RTDBCLI_ATTR_DOUBLE = 7,
    RTDBCLI_ATTR_STRING = 8
};

/// 点模型中点的类型定义
enum
{
    RTDBCLI_NODE_UNDEF = -1,
    RTDBCLI_NODE_TABLE = 1,
    RTDBCLI_NODE_RECORD = 2,
    RTDBCLI_NODE_COMPLEXRECORD = 3,
    RTDBCLI_NODE_DOMAIN = 4,
    RTDBCLI_NODE_STATION = 5,
    RTDBCLI_NODE_SYSTEM = 6,
    RTDBCLI_NODE_DEVICE = 7,
    RTDBCLI_NODE_ACC = 8,
    RTDBCLI_NODE_DIO = 9,
    RTDBCLI_NODE_DI = 10,
    RTDBCLI_NODE_DO = 11,
    RTDBCLI_NODE_AIO = 12,
    RTDBCLI_NODE_AI = 13,
    RTDBCLI_NODE_AO = 14
};

/// 命令执行状态定义
enum
{
    RTDBCLI_CMD_OK = 0,             ///< 执行成功
    RTDBCLI_CMD_EXEC_ERR = -1,      ///< 执行错误
    RTDBCLI_CMD_TIMEOUT = -2,       ///< 执行超时
    RTDBCLI_CMD_INVALID = -3,       ///< 无效命令
    RTDBCLI_CMD_CHANNEL_FAULT = -4, ///< 通道故障
    RTDBCLI_CMD_NOEXIST = -5,       ///< 目标不存在
    RTDBCLI_CMD_ERR = -6            ///< 位置错误
};

/// 定义FES采样数据点的数据类型
enum
{
    RTDBCLI_FES_DI = 1,     ///< 数字量值
    RTDBCLI_FES_AI = 2,     ///< 模拟量值
    RTDBCLI_FES_ACC = 3,    ///< 累积量值
    RTDBCLI_FES_SOE = 4     ///< SOE
};

/// 错误码定义
enum
{
    RTDBCLI_EC_OK = 0,              ///< 正常
    RTDBCLI_EC_ERROR = -1,          ///< 错误
    RTDBCLI_EC_NETFAULT = 1,        ///< 网络故障
    RTDBCLI_EC_SERVFAULT = 2,       ///< 服务故障
    RTDBCLI_EC_NOTSUPPORTED = 3,    ///< 不支持
    RTDBCLI_EC_NOTEXIST = 4,         ///< 不存在

    RTDBCLI_EC_INVALID_PARAMETER = 0x80000001,

    RTDBCLI_EC_END = 0x80FFFFFF
};

#ifdef __cplusplus
}
#endif
#endif
