/**
 * @file rtdbcli_message.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。本文件定义客户的消息处理接口实现。
 * 
 **/
#include "rtdbcli_message.h"

RTDBCLI_API int32_t rtdbcli_send_message(
    int32_t handle,
    int32_t domain,
    int32_t type,
    const char *destination,
    const char *message,
    int32_t size
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_register_message_notifier(
    int32_t handle,
    rtdbcli_message_notifier notifier,
    void *userdata
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_unregister_message_notifier(int32_t handle)
{
    return RTDBCLI_EC_OK;
}
