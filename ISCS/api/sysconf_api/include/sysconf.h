/**
 * @file sysconf.h
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口。本接口文件定义系统全局配置接口。接口线程安全。
 *       系统配置接口使用流程：
 *       首先，调用接口sysconf_init，初始化系统配置接口环境；
 *       其次，调用接口sysconf_get_xxx，获取具体的系统配置数据；
 *       最后，调用接口sysconf_uninit，清理系统配置接口环境；
 **/
#ifndef SYSCONF_H
#define SYSCONF_H

#include "sysconf_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  sysconf_init
 * 
 * @detail 初始化系统配置环境。
 *         本方法功能是加载系统全局配置文件。本接口为同步接口，在一个进程内只需初始化一次。
 *         重复调用，只有第一次调用时执行配置加载。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_init();

/**
 * @brief  sysconf_uninit
 * 
 * @detail 清理系统配置环境。本接口为同步接口，且在一个进程内只需清理一次。
 *         重复调用，只有第一次调用时执行配置清理。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API void sysconf_uninit();

/**
 * @brief  sysconf_get_domain
 * 
 * @detail 获取域。
 * 
 * @param  id      输入，域编号
 * @param  domain  输出，域对象，失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_domain(int32_t id, sysconf_domain **domain);

/**
 * @brief  sysconf_get_native_domain
 * 
 * @detail 获取本地域。
 * 
 * @param  id      输入，域编号
 * @param  domain  输出，域对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_native_domain(sysconf_domain **domain);

/**
 * @brief  sysconf_get_node
 * 
 * @detail 获取节点。
 * 
 * @param  id    输入，节点编号
 * @param  node  输出，节点对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_node(int32_t id, sysconf_node **node);

/**
 * @brief  sysconf_get_native_node
 * 
 * @detail 获取本地节点。
 *
 * @param  node  输出，节点对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_native_node(sysconf_node **node);

/**
 * @brief  sysconf_get_process
 * 
 * @detail 获取进程。
 * 
 * @param  type     输入，进程类型
 * @param  process  输出，进程对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_process(int32_t type, sysconf_process **process);

/**
 * @brief  sysconf_get_system_manager
 * 
 * @detail 获系统配置。
 * 
 * @param  conf  输出，系统配置对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_system_manager(sysconf_system_manager **conf);

/**
 * @brief  sysconf_get_mq_client
 * 
 * @detail 获取MQ客户端配置。
 * 
 * @param  conf  输出，MQ客户端配置对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_mq_client(sysconf_mq_client **conf);

/**
 * @brief  sysconf_get_rtdb_client
 * 
 * @detail 获取RTDB客户端配置。
 * 
 * @param  conf  输出，RTDB接口配置对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_rtdb_client(sysconf_rtdb_client **conf);

/**
 * @brief  sysconf_get_tsdb_client
 * 
 * @detail 获取TSDB客户端配置。
 * 
 * @param  conf  输出，TSDB客户端配置对象，不存或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_tsdb_client(sysconf_tsdb_client **conf);

/**
 * @brief  sysconf_get_cfdb_client
 * 
 * @detail 获取配置服务客户端配置。
 * 
 * @param  conf  输出，配置服务客户端配置对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_cfdb_client(sysconf_cfdb_client **conf);

/**
 * @brief  sysconf_get_pm_client
 * 
 * @detail 获取进程管理客户端配置。
 * 
 * @param  conf  输出，进程管理客户端配置对象，不存在或失败时为空。禁止释放。
 * 
 * @return 返回错误状态 @see SYSCONF_EC_XXX
 */
SYSCONF_API int32_t sysconf_get_pm_client(sysconf_pm_client **conf);

#ifdef __cplusplus
}
#endif
#endif
