/**
 * @file sysconf_data_private.h
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口。本接口文件定义系统全局配置用到的内部数据结构。
 *
 **/
#ifndef SYSCONF_DATA_PRIVATE_H
#define SYSCONF_DATA_PRIVATE_H

#include "sysconf_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// 定义系统配置对象
typedef struct
{
    int32_t log;                    ///< 日志记录句柄
    int32_t initialized;            ///< 初始化标记
    sysconf_mq_client mqcli;        ///< MQ客户端对象
    sysconf_rtdb_client rtdbcli;    ///< RTDB客户端对象
    sysconf_tsdb_client tsdbcli;    ///< TSDB客户端对象
    sysconf_cfdb_client cfdbcli;    ///< CFDB客户端对象
    sysconf_pm_client pmcli;        ///< 进程管理客户端对象
    sysconf_system_manager sm;      ///< 系统管理对象
} sysconf_conf;

/**
 * @brief  sysconf_get_sysconf
 * 
 * @detail 系统配置对象。不用释放。
 * 
 * @param  无
 * 
 * @return 系统配置对象
 */
sysconf_conf* sysconf_get_sysconf();

/**
 * @brief  sysconf_clean_system_manager
 * 
 * @detail 释放系统管理配置对象。
 * 
 * @param  conf  输入，系统管理配置对象
 * 
 * @return 无
 */
void sysconf_clean_system_manager(sysconf_system_manager *conf);

/**
 * @brief  sysconf_clean_mq_client
 * 
 * @detail MQ客户端配置对象。
 * 
 * @param  conf  输入，MQ客户端配置对象
 * 
 * @return 无
 */
void sysconf_clean_mq_client(sysconf_mq_client *conf);

/**
 * @brief  sysconf_clean_rtdb_client
 * 
 * @detail RTDB 客户端配置对象。
 * 
 * @param  conf  输入，RTDB API配置对象
 * 
 * @return 无
 */
void sysconf_clean_rtdb_client(sysconf_rtdb_client *conf);

/**
 * @brief  sysconf_clean_tsdb_client
 * 
 * @detail TSDB客户端配置对象。
 * 
 * @param  conf  输入，TSDB客户端配置对象
 * 
 * @return 无
 */
void sysconf_clean_tsdb_client(sysconf_tsdb_client *conf);

/**
 * @brief  sysconf_clean_cfdb_client
 * 
 * @detail CFDB客户端配置对象。
 * 
 * @param  conf  输入，CFDB客户端配置对象
 * 
 * @return 无
 */
void sysconf_clean_cfdb_client(sysconf_cfdb_client *conf);

/**
 * @brief  sysconf_clean_pm_client
 * 
 * @detail 进程管理客户端配置对象。
 * 
 * @param  conf  输入，进程管理客户端配置对象
 * 
 * @return 无
 */
void sysconf_clean_pm_client(sysconf_pm_client *conf);

#ifdef __cplusplus
}
#endif
#endif
