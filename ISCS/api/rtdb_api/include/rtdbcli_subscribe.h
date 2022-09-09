/**
 * @file rtdbcli_subscribe.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本接口文件定义订阅相关的操作接口。
 *       仅RTDB客户端支持订阅功能。
 * 
 *         订阅处理流程：
 *         步骤1：创建客户端实例，调用接口 rtdbcli_create_client
 *         步骤2：获取订阅对象，调用接口 rtdbcli_get_subscriber
 *         步骤3：向订阅对象设置通知接收函数，调用接口 rtdbcli_set_subscriber_notifiers
 *         步骤4：向RTDB订阅数据，调用接口 rtdbcli_subscribe_XXX
 *         步骤5：归还订阅对象，调用接口 rtdbcli_return_subscriber
 **/
#ifndef RTDBCLI_SUBSCRIBE_H
#define RTDBCLI_SUBSCRIBE_H

#include "rtdbcli_macros.h"
#include "rtdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_create_subscriber
 * 
 * @detail 创建订阅对象。本函数返回定义对象的句柄。
 *         订阅对象不在使用后，需要调用 rtdbcli_free_subscriber 释放订阅对象占用的资源。
 *         
 * @param  handle     输入，客户端实例句柄
 * @param  name       输入，订阅名称
 * @param  subHandle  输出，订阅对象句柄，失败时为-1
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_create_subscriber(int32_t handle, const char *name, int32_t *subHandle);

/**
 * @brief  rtdbcli_free_subscriber
 * 
 * @detail 释放订阅对象。本函数函数用来释放订阅对象占用的资源。
 *
 * @param  cliHandle  输入，客户端实例句柄
 * @param  subHandle  输入，订阅对象句柄
 * 
 * @return 无
 */
RTDBCLI_API void rtdbcli_free_subscriber(int32_t cliHandle, int32_t subHandle);

/**
 * @brief  rtdbcli_set_subscriber_notifiers
 * 
 * @detail 设置订阅对象的通知接口。本函数用于给订阅对象设置数据通知接口，
 *         用于通知表记录、点属性和异常通知。
 *
 * @param  handle,            输入，订阅对象句柄
 * @param  nodeNotifier       输入，点属性通知接口
 * @param  nduserdata         输入，用户数据，用于点属性通知接口
 * @param  tableNotifier      输入，表记录通知接口
 * @param  tbluserdata        输入，用户数据，用于表记录通知接口
 * @param  exceptionNotifier  输入，异常通知接口
 * @param  expuserdata        输入，用户数据，用于异常通知接口
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_set_subscriber_notifiers(
    int32_t handle,
	rtdbcli_node_notifier nodeNotifier,
    const void *nduserdata,
	rtdbcli_table_notifier tableNotifier,
    const void *tbluserdata,
	rtdbcli_exception_notifier exceptionNotifier,
	const void *expuserdata
);

/**
 * @brief  rtdbcli_subscribe_table
 * 
 * @detail 单表订阅。
 *
 * @param  handle      输入，订阅对象句柄
 * @param  domain      输入，表（即RTDB服务）所属域的ID
 * @param  tablename   输入，表名称
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_subscribe_table(
    int32_t handle,
    int32_t domain,
    const char *tablename
);

/**
 * @brief  rtdbcli_subscribe_tables
 * 
 * @detail 多表订阅。返回成功，或部分失败，或失败。
 *
 * @param  handle      输入，订阅对象句柄
 * @param  domain      输入，表（即RTDB服务）所属域的ID
 * @param  tablenames  输入，表名称数组
 * @param  num         输入，表名称个数（最大32767）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_subscribe_tables(
    int32_t handle,
    int32_t domain,
    const char **tablenames,
    int16_t num
);

/**
 * @brief  rtdbcli_subscribe_node
 * 
 * @detail 单点订阅。订阅一个点的多个属性。返回成功，或部分失败，或失败。
 *
 * @param  handle  输入，订阅对象句柄
 * @param  domain  输入，点(即RTDB服务)所属域的ID
 * @param  path    输入，点路径
 * @param  names   输入，属性名称数组
 * @param  num     输入，属性名称个数（最大32767）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_subscribe_node(
    int32_t handle,
    int32_t domain,
    const char *path,
    const char **names,
    int16_t num
);

/**
 * @brief  rtdbcli_subscribe_nodes
 * 
 * @detail 多点订阅。订阅多个点的多个属性。返回成功，或部分失败，或失败。
 *
 * @param  handle  输入，订阅对象句柄
 * @param  domain  输入，点(即RTDB服务)所属域的ID
 * @param  paths   输入，点路径数组
 * @param  pnum    输入，点路径个数
 * @param  names   输入，属性名称数组
 * @param  nnum    输入，属性名称个数（最大32767）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_subscribe_nodes(
    int32_t handle,
    int32_t domain,
    const char **paths,
    int16_t pnum,
    const char **names,
    int16_t nnum
);

/**
 * @brief  rtdbcli_unsubscribe_tables
 * 
 * @detail 取消表订阅。本接口用于取消1至多个表的订阅。
 *
 * @param  handle      输入，订阅对象句柄
 * @param  domain      输入，表所属域的ID
 * @param  tablenames  输入，表名称数组
 * @param  num         输入，表名称个数（最大32767）
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_unsubscribe_tables(
    int32_t handle,
    int32_t domain,
    const char **tablenames,
    int16_t num
);

/**
 * @brief  rtdbcli_unsubscribe_node
 * 
 * @detail 取消点订阅。本接口用于取消对点的所有属性的订阅。
 *
 * @param  handle  输入，订阅对象句柄
 * @param  domain  输入，点所属域的ID
 * @param  path    输入，点路径
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_unsubscribe_node(int32_t handle, int32_t domain, const char *path);

/**
 * @brief  rtdbcli_unsubscribe_nodes
 * 
 * @detail 取消点订阅。本接口用于取消对多个点的所有属性的订阅。
 *
 * @param  handle  输入，订阅对象句柄
 * @param  domain  输入，点所属域的ID
 * @param  paths   输入，点路径
 * @param  num     输入，点路径个数
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_unsubscribe_nodes(int32_t handle, int32_t domain, const char **path, int16_t num);

#ifdef __cplusplus
}
#endif
#endif
 
