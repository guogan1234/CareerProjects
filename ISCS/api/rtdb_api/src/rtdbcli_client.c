/**
 * @file rtdbcli_client.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口实现文件。
 * 
 **/
#include "rtdbcli_client.h"
#include "rtdbcli_global.h"
#include "rtdbcli_privdata.h"
#include "rtdbcli_malloc.h"


RTDBCLI_API int32_t rtdbcli_create_client(int32_t type, int32_t *handle)
{
    if(handle == NULL) {
        return RTDBCLI_EC_INVALID_PARAMETER;
    }

    if(g_handle_map_instance == NULL){
        g_handle_map_instance = handle_map_init(INIT_SLOTS);
    }

    int isize = sizeof(_rtdbcli_client);
    _rtdbcli_client* pclient = rtdbcli_malloc(isize);
    memset((void*)pclient, 0, isize);

    if(pclient != NULL){
        pclient->type_client = type;
        *handle = handle_map_add_entry(g_handle_map_instance, pclient);
        return RTDBCLI_EC_OK;
    }
    else{
        return RTDBCLI_EC_ERROR;
    }
}

RTDBCLI_API void rtdbcli_free_client(int32_t handle)
{
    void *handle_entry = handle_map_release_entry(g_handle_map_instance, handle);
    if(handle_entry) {
        rtdbcli_free(handle_entry);
    }
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_register_service(int32_t handle, const char *id)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_unregister_service(int32_t handle, const char *id)
{
    return RTDBCLI_EC_OK;
}
