/**
 * @file rtdbcli_subscribe.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。本接口文件定义订阅相关的操作接口实现。
 * 
 **/
#include "rtdbcli_subscribe.h"

#ifdef __cplusplus
extern "C"
{
#endif

RTDBCLI_API int32_t rtdbcli_create_subscriber(int32_t handle, const char *name, int32_t *subHandle)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API void rtdbcli_free_subscriber(int32_t cliHandle, int32_t subHandle)
{
}

RTDBCLI_API int32_t rtdbcli_set_subscriber_notifiers(
    int32_t handle,
	rtdbcli_node_notifier nodeNotifier,
    const void *nduserdata,
	rtdbcli_table_notifier tableNotifier,
    const void *tbluserdata,
	rtdbcli_exception_notifier exceptionNotifier,
	const void *expuserdata
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_subscribe_table(
    int32_t handle,
    int32_t domain,
    const char *tablename
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_subscribe_tables(
    int32_t handle,
    int32_t domain,
    const char **tablenames,
    int16_t num
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_subscribe_node(
    int32_t handle,
    int32_t domain,
    const char *path,
    const char **names,
    int16_t num
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_subscribe_nodes(
    int32_t handle,
    int32_t domain,
    const char **paths,
    int16_t pnum,
    const char **names,
    int16_t nnum
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_unsubscribe_tables(
    int32_t handle,
    int32_t domain,
    const char **tablenames,
    int16_t num
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_unsubscribe_node(int32_t handle, int32_t domain, const char *path)
{
    return RTDBCLI_EC_OK;
}
 
RTDBCLI_API int32_t rtdbcli_unsubscribe_nodes(int32_t handle, int32_t domain, const char **path, int16_t num)
{
    return RTDBCLI_EC_OK;
}