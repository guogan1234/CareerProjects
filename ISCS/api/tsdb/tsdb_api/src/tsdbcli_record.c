/**
 * @file tsdbcli_record.c
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口。本接口文件定义TSDB客户端用到的记录操作接口实现。
 *
 **/
#include "tsdbcli_record.h"

TSDBCLI_API int32_t tsdbcli_get_field_int64(
    const tsdbcli_record *record,
    const char *field,
    int64_t *value
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_get_field_int32(
    const tsdbcli_record *record,
    const char *field,
    int32_t *value
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_get_field_int16(
    const tsdbcli_record *record,
    const char *field,
    int16_t *value
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_get_field_int8(
    const tsdbcli_record *record,
    const char *field,
    int8_t *value
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_get_field_double(
    const tsdbcli_record *record,
    const char *field,
    double *value
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_get_field_string(
    const tsdbcli_record *record,
    const char *field,
    char **value
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_set_field_int64(
    tsdbcli_record *record,
    const char *field,
    int64_t value,
    int8_t isTag
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_set_field_int32(
    tsdbcli_record *record,
    const char *field,
    int32_t value,
    int8_t isTag
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_set_field_int16(
    tsdbcli_record *record,
    const char *field,
    int16_t value,
    int8_t isTag
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_set_field_int8(
    tsdbcli_record *record,
    const char *field,
    int8_t value,
    int8_t isTag
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_set_field_double(
    tsdbcli_record *record,
    const char *field,
    double value,
    int8_t isTag
)
{
    return TSDBCLI_EC_OK;
}

TSDBCLI_API int32_t tsdbcli_set_field_string(
    tsdbcli_record *record,
    const char *field,
    const char *value,
    int8_t isTag
)
{
    return TSDBCLI_EC_OK;
}
