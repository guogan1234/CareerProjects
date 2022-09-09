/**
 * @file rtdbcli_client.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本接口文件定义客户端的通用接口。本客户端为线程安全客户端。
 * 
 **/
#ifndef RTDBCLI_CLIENT_H
#define RTDBCLI_CLIENT_H

#include "rtdbcli_macros.h"
#include "rtdbcli_data.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_create_client
 * 
 * @detail 创建客户端实例。
 *         本方用来创建“type”类型的客户端，支持的客户端类型“type”参见
 *         @see RTDBCLI_TYPE_XXX 定义。
 *         客户端不在使用后，调用 rtdbcli_free_client 释放资源。
 * 
 * @param  type    输入，客户端类型
 * @param  handle  输出，客户端实例句柄，失败返回-1
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_create_client(int32_t type, int32_t *handle);

/**
 * @brief  rtdbcli_free_client
 * 
 * @detail 释放客户端实例
 * 
 * @param  handle 输入，客户端实例句柄
 * 
 * @return 无
 */
RTDBCLI_API void rtdbcli_free_client(int32_t handle);

/**
 * @brief  rtdbcli_register_service
 * 
 * @detail 注册服务。向服务器注册客户端身份，客户端只有注册后，才能够从服务接收命令和消息。
 *         支持注册服务的客户端如下：
 *         RTDB客户端
 *         FES客户端
 *         SFES客户端
 *         
 *         注意：
 *         1. 安全控制客户端不支持注册服务。
 *         2. RTDB客户端注册，“id”不能与与RTDB内部插件的ID重复。
 *         3. FES、SFES客户端注册，"id"是FES、SFES本地RTU ID的组合，即“rtuid1 rtuid2 ... rtuidn”
 * 
 * @param handle  输入，客户端实例句柄
 * @param id      输入，服务ID
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_register_service(int32_t handle, const char *id);

/**
 * @brief  rtdbcli_unregister_service
 * 
 * @detail 注销服务。本接口用于取消客户端向服务器的注册。
 *         服务注销后，RTDB服务就不再向客户端发送消息和命令。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  id      输入，插件ID
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_unregister_service(int32_t handle, const char *id);

#ifdef __cplusplus
}
#endif
#endif
