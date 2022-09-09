/**
 * @file rtdbcli_command.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。客户端命令处理的接口实现。
 *
 **/
#include "rtdbcli_command.h"

RTDBCLI_API int32_t rtdbcli_generate_commandid(int32_t handle, char* id, int32_t len)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_send_command(
	int32_t handle, 
	int32_t domain, 
	int32_t type,
    const char *destination, 
    const char *id,
	const char *cmdjson
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_send_command_result(
    int32_t handle,
    int32_t domain,
    int32_t type,
    const char *destination,
    const char *id,
    int32_t code,
    const char *result
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_register_command_notifier(
    int32_t handle,
    rtdbcli_command_notifier notifier,
    const void *userdata
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_register_command_result_notifier(
    int32_t handle,
    rtdbcli_command_result_notifier notifier,
    const void *userdata
)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_unregister_command_notifier(int32_t handle)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_unregister_command_result_notifier(int32_t handle)
{
    return RTDBCLI_EC_OK;
}

