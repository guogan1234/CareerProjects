/**
 * @file rtdbcli_node.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。本接口文件定义点模型的操作接口实现。
 * 
 **/
#include "rtdbcli_node.h"

RTDBCLI_API int32_t
rtdbcli_get_attribute(int32_t handle, const char *path, rtdbcli_attribute **attr)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t
rtdbcli_get_attributes(int32_t handle, const char *path, const char **names, int16_t size, rtdbcli_record **record)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_mget_attributes(
    int32_t handle, 
    const char **paths, 
    int16_t pnum, 
    const char **names, 
    int16_t nnum, 
    rtdbcli_record **records
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t
rtdbcli_get_node(int32_t handle, const char *path, rtdbcli_record **record)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t
rtdbcli_get_output_node(int32_t handle, const char *path, rtdbcli_record **record)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API char**
rtdbcli_enumerate_childkey(int32_t handle, const char *path, int32_t *num, int32_t *ec)
{
    return NULL;
}

RTDBCLI_API char**
rtdbcli_enumerate_attributekey(int32_t handle, const char *path, int32_t *num, int32_t *ec)
{
    return NULL;
}

RTDBCLI_API int32_t
rtdbcli_get_nodetype(int32_t handle, const char *path, int32_t *type)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t 
rtdbcli_set_attribute(int32_t handle, const char *path, const char *value)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t 
rtdbcli_bitset_status(int32_t handle, const char *path, int32_t bit, int32_t value)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t 
rtdbcli_get_svtc(int32_t handle, const char *path, char **svtc)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t 
rtdbcli_set_svtc(int32_t handle, const char *path, const char *svtc)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t 
rtdbcli_get_safecollection(int32_t handle, const char *path, char **sdata)
{
    return RTDBCLI_EC_OK;
}
