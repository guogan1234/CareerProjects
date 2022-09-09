/**
 * @file rtdbcli_command.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件，本文件定义客户端的命令接口。
 *       RTDB客户端、SCTRL客户端、FES客户端、SFES客户端支持命令接口。客户端的命令接口说明如下：
 *       1. RTDB客户端
 *       支持客户端向RTDB发送命令，支持客户端从RTDB接收命令。
 * 
 *       1.1. 发送命令时客户端的接口调用过程：
 *       步骤1：创建客户端实例，调用接口 rtdbcli_create_client
 *       步骤2：向客户端注册接收注册命令执行结果函数，调用接口 rtdbcli_register_command_result_notifier
 *       步骤3：给RTDB发送命令，调用接口 rtdbcli_send_command
 *       
 *       1.2. 接收命令时客户端的接口调用过程：
 *       步骤1：创建客户端实例，调用接口 rtdbcli_create_client
 *       步骤2：向客户端注册命令接收函数，调用接口 rtdbcli_register_command_notifier
 *       步骤3：向RTDB注册服务，调用接口 rtdbcli_register_service
 *       步骤4：向RTDB发送命令执行结果，调用接口 rtdbcli_send_command_result
 * 
 *       1.3. 客户端注销命令接收处理过程：
 *       步骤1：注销服务，调用接口 rtdbcli_unregister_service
 *       步骤2：注销命令接收，调用接口 rtdbcli_unregister_command_notifier
 * 
 *       2. FES客户端、SFES客户端
 *       FES客户端支持从RTDB接收命令，SFES客户端支持从SCTRL接收命令。
 *       FES客户端不支持从SCTRL接收命令，SFES客户端不支持从RTDB接收命令。
 *       FES、SFES客户端不支持发送命令。
 * 
 *       2.1. 接收命令时客户端的接口调用过程：
 *       步骤1：创建客户端，调用接口 rtdbcli_create_client
 *       步骤2：向客户端注册命令接收函数，调用接口 rtdbcli_register_command_notifier
 *       步骤3：向RTDB注册服务， 调用接口 rtdbcli_register_service
 *       步骤4：FES客户端向RTDB（或SFES客户端向SCTRL）发送命令执行结果，调用接口 rtdbcli_send_command_result
 * 
 *       2.2. 客户端注销命令接收处理过程：
 *       步骤1：注销服务，调用接口 rtdbcli_unregister_service
 *       步骤2：注销命令接收，调用接口 rtdbcli_unregister_command_notifier
 * 
 *       3. SCTRL客户端
 *       不支持接收命令，支持发送命令。
 * 
 *       3.1. 发送命令时客户端的接口调用过程：
 *       步骤1：创建RTDB客户端实例，调用接口 rtdbcli_create_client
 *       步骤2：向客户端注册接收注册命令执行结果函数，调用接口 rtdbcli_register_command_result_notifier
 *       步骤3：给RTDB发送命令，调用接口 rtdbcli_send_command
 **/
#ifndef RTDBCLI_COMMAND_H
#define RTDBCLI_COMMAND_H

#include "rtdbcli_macros.h"
#include "rtdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_generate_commandid
 * 
 * @detail 生成命令唯一标示。本接口生成长度为36字节的命令ID。
 *         发送命令、接收命令、发送命令执行结果、接收命令执行结果接口都以本方法生成的ID作为命令的唯一标示。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  id      输出，命令的ID
 * @param  len     输入，id的缓存长度
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_generate_commandid(int32_t handle, char* id, int32_t len);

/**
 * @brief  rtdbcli_send_command
 * 
 * @detail 发送命令。本接口用于向目标系统发送命令，命令的执行结果异步返回。
 * 
 * @param  handle       输入，客户端实例句柄
 * @param  domain       输入，接收命令的服务所在的域ID
 * @param  type         输入，处理命令的目标系统类型,参见@see RTDBCLI_DEST__XXX定义
 * @param  destination  输入，处理命令的目标系统ID
 * @param  id           输入，命令的ID
 * @param  cmdjson      输入，命令数据（JSON格式，内容由发送和处理系统定义，例如命令类型）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_send_command(
	int32_t handle, 
	int32_t domain, 
	int32_t type,
    const char *destination, 
    const char *id,
	const char *cmdjson
);

/**
 * @brief  rtdbcli_send_command_result
 * 
 * @detail 发送命令执行结果。本接口用于返回命令的执行结果给命令发送者。
 *
 * @param  handle       输入，客户端实例句柄
 * @param  domain       输入，处理命令的RTDB服务所在的域ID
 * @param  type         输入，处理命令的目标系统类型,参见@see RTDBCLI_DEST__XXX定义
 * @param  destination  输入，处理命令的目标系统ID
 * @param  id           输入，命令ID
 * @param  code         输入，命令执行结果状态，参见@see RTDBCLI_CMD_XXX定义
 * @param  result       输入，命令执行结果描述（内容由命令发送和处理系统商定）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_send_command_result(
    int32_t handle,
    int32_t domain,
    int32_t type,
    const char *destination,
    const char *id,
    int32_t code,
    const char *result
);

/**
 * @brief  rtdbcli_register_command_notifier
 * 
 * @detail 向客户端注册命令接收函数。
 * 
 * @param  handle   输入，客户端实例句柄
 * @param  notifier 输入，命令通知接口
 * @param  userdata  输入，用户数据（接口不做任何处理，仅作为命令通知函数参数）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_register_command_notifier(
    int32_t handle,
    rtdbcli_command_notifier notifier,
    const void *userdata
);

/**
 * @brief  rtdbcli_register_command_result_notifier
 * 
 * @detail 向客户端注册接收命令的执行结果函数。
 * 
 * @param  handle    输入，客户端实例句柄
 * @param  notifier  输入，命令结果通知接口
 * @param  userdata  输入，用户数据（接口不做任何处理，仅作为命令结果通知函数参数）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_register_command_result_notifier(
    int32_t handle,
    rtdbcli_command_result_notifier notifier,
    const void *userdata
);

/**
 * @brief  rtdbcli_unregister_command_notifier
 * 
 * @detail 注销命令通知接口。本接口用于注销客户端的命令通知接口。
 *         注销后，客户端不在处理服务端发送的命令。
 * 
 * @param  handle  输入，客户端实例句柄
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_unregister_command_notifier(int32_t handle);

/**
 * @brief  rtdbcli_unregister_command_result_notifier
 * 
 * @detail 注销命令结果通知接口。本接口用于注销客户端的命令结果通知接口。
 *         注销后，客户端不在处理服务端返回的命令结果。
 * 
 * @param  handle  输入，客户端实例句柄
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_unregister_command_result_notifier(int32_t handle);

#ifdef __cplusplus
}
#endif
#endif
