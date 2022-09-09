/**
 * @file rtdbcli_message.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本文件定义客户的消息接口。
 *       RTDB客户端、FES客户端、SFES客户端支持消息接口。客户端的消息接口说明如下：
 *
 *       1. RTDB客户端
 *       支持向RTDB发送消息，支持从RTDB接收消息。
 * 
 *       1.1. 发送消息时客户端的接口调用过程：
 *       步骤1：创建客户端实例，调用接口 rtdbcli_create_client
 *       步骤2：给RTDB发送消息，调用接口 rtdbcli_send_message
 * 
 *       1.2. 接收消息时客户端的接口调用过程：
 *       步骤1：创建客户端实例，调用接口 rtdbcli_create_client
 *       步骤2：向客户端注册消息接收函数，调用接口 rtdbcli_register_message_notifier
 *       步骤3：向RTDB注册服务，调用接口 rtdbcli_register_service
 * 
 *       1.3. 注销消息接收的客户端接口调用过程：
 *       步骤1：从RTDB注销注册，调用接口 rtdbcli_unregister_service
 *       步骤2：从客户端注销消息接收函数，调用接口 rtdbcli_register_message_notifier
 * 
 *       2.FES客户端和SFES客户端
 *       仅支持向RTDB发送消息。
 * 
 *       2.1. 发送消息时客户端的接口调用过程：
 *       步骤1：创建客户端，调用接口 rtdbcli_create_client
 *       步骤2：给RTDB发送消息，调用接口 rtdbcli_send_message
 **/
#ifndef RTDBCLI_MESSAGE_H
#define RTDBCLI_MESSAGE_H

#include "rtdbcli_macros.h"
#include "rtdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_send_message
 * 
 * @detail 发送消息。本接口用于向目标系统发送消息。
 * 
 * @param  handle       输入，客户端实例句柄
 * @param  domain       输入，接收消息的服务所在的域ID
 * @param  type         输入，处理消息的目标系统类型,参见@see RTDBCLI_DEST__XXX定义
 * @param  destination  输入，处理消息的目标系统ID
 * @param  message      输入，消息内容
 * @param  size         输入，消息长度
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_send_message(
    int32_t handle,
    int32_t domain,
    int32_t type,
    const char *destination,
    const char *message,
    int32_t size
);

/**
 * @brief  rtdbcli_register_message_notifier
 * 
 * @detail 注册消息通知接口。本接口用于向客户端注册接收消息的接口。
 * 
 * @param  handle    输入，客户端实例句柄
 * @param  notifier  输入，消息通知接口
 * @param  userdata  输入，用户数据（不做处理，仅作为消息通知接口参数）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_register_message_notifier(
    int32_t handle,
    rtdbcli_message_notifier notifier,
    void *userdata
);

/**
 * @brief  rtdbcli_unregister_message_notifier
 * 
 * @detail 注销消息通知接口。本接口用于注销客户端的消息通知接口。
 * 
 * @param  handle  输入，客户端实例句柄
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_unregister_message_notifier(int32_t handle);

#ifdef __cplusplus
}
#endif
#endif
