/**
 * @file rtdbcli_table.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。本文件定义表模型操作接口实现。
 * 
 **/
#include "rtdbcli_table.h"

RTDBCLI_API int32_t rtdbcli_query_records(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond,
    const char **fieldnames,
    int32_t fieldnum,
    int32_t index,
    int32_t *totalnum,
    int32_t *recordnum,
    rtdbcli_record **records
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_update_records(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond,
    const rtdbcli_record *record
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_insert_record(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond,
    const rtdbcli_record *record
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_delete_records(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_proxy_insert_record(
    int32_t handle,
    const char *tablename,
    const rtdbcli_record *tags,
    const rtdbcli_record *fields
)
{
    return RTDBCLI_EC_OK;
}
