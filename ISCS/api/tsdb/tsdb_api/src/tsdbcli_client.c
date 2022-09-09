/**
 * @file tsdbcli_client.c
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口。本接口文件定义TSDB客户端接口实现。
 *
 **/
#include "tsdbcli_client.h"

TSDBCLI_API int32_t tsdbcli_create_client(int32_t *handle)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API void tsdbcli_free_client(int32_t handle)
{
}

TSDBCLI_API int32_t tsdbcli_save_records(
    int32_t handle,
    const char *table,
    const tsdbcli_record *records,
    int32_t num
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_query_records(
    int32_t handle,
    const char *cond,
    int32_t index,
    int32_t *totalNum,
    int32_t *num,
    const tsdbcli_record **records
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_update_records(
    int32_t handle,
    const char *table,
    const tsdbcli_record *records,
    int32_t num
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_delete_records(
    int32_t handle,
    const char *table,
    const char *cond
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_create_record(int32_t handle, tsdbcli_record **record)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_create_records(int32_t handle, tsdbcli_record **records, int32_t *num)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API void tsdbcli_free_record(int32_t handle, tsdbcli_record *record)
{
}

TSDBCLI_API void tsdbcli_free_records(int32_t handle, tsdbcli_record *records, int32_t num)
{
}
