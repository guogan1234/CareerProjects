/**
 * @file tsdbcli_client.h
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口。本接口文件定义TSDB客户端接口。
 *       本接口为线程安全。
 *
 **/
#ifndef TSDBCLI_CLIENT_H
#define TSDBCLI_CLIENT_H

#include "tsdbcli_macros.h"
#include "tsdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  tsdbcli_create_client
 * 
 * @detail 创建客户端实例。客户端实例是线程安全的。
 *         调用tsdbcli_free_client释放资源。
 * 
 * @param  handle  输出，客户端实例句柄，失败返回-1
 * 
 * @return 返回错误状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_create_client(int32_t *handle);

/**
 * @brief  tsdbcli_create_client
 * 
 * @detail 释放客户端实例。
 * 
 * @param  handle  输入，客户端实例句柄
 * 
 * @return 无
 */
TSDBCLI_API void tsdbcli_free_client(int32_t handle);

/**
 * @brief  tsdbcli_save_records
 * 
 * @detail 保存记录。
 * 
 * @param  handle   输入，客户端实例句柄
 * @param  table    输入，表名
 * @param  records  输入，记录数组
 * @param  num      输入，记录个数
 * 
 * @return 返回错误状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_save_records(
    int32_t handle,
    const char *table,
    const tsdbcli_record *records,
    int32_t num
);

/**
 * @brief  tsdbcli_query_records
 * 
 * @detail 查询记录。
 * 
 * @param  handle    输入，客户端实例句柄
 * @param  cond      输入，查询条件，TAG和FIELD都可以作为查询条件
 * @param  index,    输入，从第几条记录开始返回
 * @param  totalNum  输出，本次查询的总记录条数
 * @param  num       输入/出，输入希望返回的记录条数，输出实际返回的条数
 *                   实际输出条数小于等于希望返回的条数(多余的记录丢弃)
 * @param  records   输出，记录数组
 * 
 * @return 返回错误状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_query_records(
    int32_t handle,
    const char *cond,
    int32_t index,
    int32_t *totalNum,
    int32_t *num,
    const tsdbcli_record **records
);

/**
 * @brief  tsdbcli_update_records
 * 
 * @detail 更新记录。注意：
 *         1.TAGS值不允许更新
 *         2.必须给出表的全部TAGS（TAG的值和名称）。
 *         3.FIELDS只要给出需要更新的FIELDS（名称和值）
 *         4.tsdbcli_record.timestamp为记录的时间字段，如果timestamp不为0，
 *           则timestamp作为更新时查询的条件"time"字段的值。
 * 
 * @param  handle    输入，客户端实例句柄
 * @param  table     输入，表名称
 * @param  records   输入，记录数组
 * @param  num       输入，记录条数
 * 
 * 
 * @return 返回错误状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_update_records(
    int32_t handle,
    const char *table,
    const tsdbcli_record *records,
    int32_t num
);

/**
 * @brief  tsdbcli_delete_records
 * 
 * @detail 删除记录。
 * 
 * @param  handle    输入，客户端实例句柄
 * @param  table     输入，表名称
 * @param  cond      输入，删除条件
 * 
 * 
 * @return 返回错误状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_delete_records(
    int32_t handle,
    const char *table,
    const char *cond
);

/**
 * @brief  tsdbcli_create_record
 * 
 * @detail 创建记录对象。创建的记录调用tsdbcli_free_record释放资源。
 * 
 * @param  handle  输出，客户端实例句柄
 * @param  record  输出，创建的记录对象
 * 
 * @return 返回错误状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_create_record(int32_t handle, tsdbcli_record **record);

/**
 * @brief  tsdbcli_create_records
 * 
 * @detail 创建记录对象。创建的记录调用tsdbcli_free_record释放资源。
 * 
 * @param  handle   输出，客户端实例句柄
 * @param  records  输出，创建的记录对象数组
 * @param  num      输入/出，记录个数。输入希望创建的记录个数，输出实际创建的记录个数。
 *                  实际创建的记录个数小于等于希望的个数。
 * 
 * @return 返回错误状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_create_records(int32_t handle, tsdbcli_record **records, int32_t *num);

/**
 * @brief  tsdbcli_free_record
 * 
 * @detail 释放记录对象。
 * 
 * @param  handle   输出，客户端实例句柄
 * @param  record  输入，记录对象
 * 
 * @return 无
 */
TSDBCLI_API void tsdbcli_free_record(int32_t handle, tsdbcli_record *record);

/**
 * @brief  tsdbcli_free_records
 * 
 * @detail 释放记录数组。
 * 
 * @param  handle   输出，客户端实例句柄
 * @param  records  输入，记录数组
 * @param  num      输入，记录个数
 * 
 * @return 无
 */
TSDBCLI_API void tsdbcli_free_records(int32_t handle, tsdbcli_record *records, int32_t num);

#ifdef __cplusplus
}
#endif

#endif
