/**
 * @file rtdbcli_data.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本文件定义客户用到的数据和数据操作接口。
 **/
#ifndef RTDBCLI_DATA_H
#define RTDBCLI_DATA_H

#include "rtdbcli_macros.h"
#include <stdint.h>
#include <stddef.h> 

#ifdef __cplusplus
extern "C"
{
#endif

/// 定义点属性对象
typedef struct
{
    int32_t type;                           ///< 属性值类型，@see RTDBCLI_ATTR_XXX
    int64_t integer;                        ///< 整数值
    double real;                            ///< 浮点数值
    char string[RTDBCLI_STRVAL_MAX_LEN];    ///< 字符串值
    char key[RTDBCLI_KEY_MAX_LEN];          ///< 属性键
} rtdbcli_attribute;

/// 定义记录对象
typedef struct
{
    int32_t num;                            ///< 属性个数
    rtdbcli_attribute *attrs;               ///< 属性数组
} rtdbcli_record;

/// 定义FES上送数据点的采样数据对象
typedef struct
{
    char path[RTDBCLI_PATH_MAX_LEN];    ///< 数据点的路径
    char label[RTDBCLI_KEY_MAX_LEN];    ///< DI值的标签(仅DI时有效)
    int8_t isChanged;                   ///< 是否变化值（0 -- 定时值，1 -- 变化值）
    int8_t status;                      ///< 采样值的质量状态（默认填0）
    int8_t type;                        ///< 数据对象类型，参见@see RTDBCLI_FES_XXX定义
    int32_t integer;                    ///< 数字量或累积量的值
    float real;                         ///< 模拟量的值
    int64_t timestamp;                  ///< 时间戳（精度：微秒）
} rtdbcli_datapoint;

/// 定义FES上送SOE对象
typedef struct
{
    char path[RTDBCLI_PATH_MAX_LEN];        ///< 数据点的路径
    char timemark[RTDBCLI_SOETM_MAX_LEN];   ///< 时标:YYYY-mm-dd HH:MM:SS.mmm
    char value;                             ///< 值
    int64_t timestamp;                      ///< 时间戳（精度：微秒）
} rtdbcli_soe;


/**
 * @brief  rtdbcli_message_notifier
 * 
 * @detail 消息通知接口。用于向客户端通知消息(方法由注册者实现)。
 *
 * @param  domain       输入，接收消息的RTDB服务所在的域
 * @param  type         输入，处理消息的目标系统类型,参见@see RTDBCLI_DEST__XXX定义
 * @param  destination  输入，处理消息的目标系统ID
 * @param  message      输入，消息内容
 * @param  size         输入，消息长度
 * @param  userdata     输入，用户数据
 * 
 * @return 无
 */
typedef void(*rtdbcli_message_notifier)(
    int32_t domain,
    int32_t type,
    const char *destination,
    const char *message,
    int32_t size,
    void *userdata
);

/**
 * @brief  rtdbcli_command_notifier
 * 
 * @detail 命令通知接口。用于向客户端通知接收到的命令（方法由注册者实现）。
 *
 * @param  domain       输入，接收命令的RTDB服务所在的域
 * @param  type         输入，处理命令的目标系统类型,参见@see RTDBCLI_DEST__XXX定义
 * @param  destination  输入，处理命令的目标系统ID
 * @parma  id,          输入，命令ID
 * @param  cmdjson      输入，命令数据（JSON格式，内容由发送和处理系统定义，例如命令类型）
 * @param  userdata     输入，用户数据
 * 
 * @return 无
 */
typedef void(*rtdbcli_command_notifier)(
	int32_t domain, 
	int32_t type, 
	const char *destination,
    const char *id, 
	const char *cmdjson, 
	const void *userdata
);

/**
 * @brief  rtdbcli_command_result_notifier
 * 
 * @detail 命令执行结果通知接口。用于通知命令的执行结果(方法由注册者实现)。
 *
 * @param  domain       输入，接收命令的RTDB服务所在的域
 * @param  type         输入，处理命令的目标系统类型,参见@see RTDBCLI_DEST__XXX定义
 * @param  destination  输入，处理命令的目标系统ID
 * @param  id           输入，命令ID
 * @param  code         输入，命令执行结果状态(0 -- 成功，!0 -- 失败)，参见@see RTDBCLI_CMD_XXX定义
 * @param  result       输入，命令执行结果描述（内容由命令发送和处理系统商定）
 * @param  userdata     输入，用户数据
 * 
 * @return 无
 */
typedef void(*rtdbcli_command_result_notifier)(
    int32_t domain,
    int32_t type,
    const char *destination,
    const char *id,
    int32_t code,
    const char *result,
    const void *userdata
);

/**
 * @brief  rtdbcli_table_notifier
 * 
 * @detail 表记录变更通知。用来向订阅者通知变化的表记录数据（方法由订阅者实现）。
 *         
 * @param  handle     输入，订阅对象句柄
 * @param  type       输入，记录变更类型，参见@see RTDBCLI_NOTIFY_XXX定义
 * @param  tablename  输入，表名称
 * @param  record     输入，发生变更的记录
 * @param  userdata   输入，用户数据
 * 
 * @return 无
 */
typedef void(*rtdbcli_table_notifier)(
    int32_t handle,
    int32_t type,
    const char *tablename,
    const rtdbcli_record *record,
    const void *userdata
);

/**
 * @brief  rtdbcli_node_notifier
 * 
 * @detail 点属性变更通知。用来向订阅者通知变化的点的属性（方法由订阅者实现）。
 *         
 * @param  handle    输入，订阅对象句柄
 * @param  path      输入，点路径
 * @param  record    输入，发生变化的点的属性集合
 * @param  userdata  输入，用户数据
 * 
 * @return 无
 */
typedef void(*rtdbcli_node_notifier)(
    int32_t handle,
    const char *path,
    const rtdbcli_record *record,
    const void *userdata
);

/**
 * @brief  rtdbcli_exception_notifier
 * 
 * @detail 异常通知接口。异常发生后向订阅者通知状态（方法由订阅者实现）。
 *
 * @param  handle    输入，订阅对象句柄
 * @param  code      输入，错误码，参见@see RTDBCLI_EC_XXX定义
 * @param  userdata  输入，用户数据
 * 
 * @return 无
 */
typedef void(*rtdbcli_exception_notifier)(int32_t handle, int32_t code,const void *userdata);

/**
 * @brief  rtdbcli_free_attribute
 * 
 * @detail 释放属性。
 * 
 * @param  attr  输入，属性对象
 * 
 * @return 无
 */
RTDBCLI_API void rtdbcli_free_attribute(rtdbcli_attribute *attr);

/**
 * @brief  rtdbcli_free_record
 * 
 * @detail 释放记录。
 * 
 * @param  record  输入，记录对象
 * 
 * @return 无
 */
RTDBCLI_API void rtdbcli_free_record(rtdbcli_record *record);

/**
 * @brief  rtdbcli_free_records
 * 
 * @detail 释放记录数组。
 * 
 * @param  records  输入，记录对象数组
 * @param  num      输入，记录个数
 * 
 * @return 无
 */
RTDBCLI_API void rtdbcli_free_records(rtdbcli_record *record, int32_t num);

/**
 * @brief  rtdbcli_free_string
 * 
 * @detail 释放字符串。
 * 
 * @param  s  输入，字符串
 * 
 * @return 无
 */
RTDBCLI_API void rtdbcli_free_string(char *s);

/**
 * @brief  rtdbcli_free_strings
 * 
 * @detail 释放字符串数组。
 * 
 * @param  ss   输入，字符串数组
 * @param  num  输入，数组元素个数
 * 
 * @return 无
 */
RTDBCLI_API void rtdbcli_free_strings(char **ss, int32_t num);

#ifdef __cplusplus
}
#endif
#endif
