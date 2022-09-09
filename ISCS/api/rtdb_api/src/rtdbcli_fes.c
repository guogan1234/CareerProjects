/**
 * @file rtdbcli_fes.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。本文件定义客户的FES接口实现。
 *          
 **/
#include "rtdbcli_fes.h"

RTDBCLI_API int32_t rtdbcli_prepare_datapoint(int32_t handle, rtdbcli_datapoint **datapoint)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_prepare_soe(int32_t handle, rtdbcli_soe **soe)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_prepare_datapoints(int32_t handle, rtdbcli_datapoint **datapoints, int32_t *num)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_prepare_soes(int32_t handle, rtdbcli_soe **soes, int32_t *num)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API void rtdbcli_destory_datapoint(int32_t handle, rtdbcli_datapoint *datapoint)
{
}

RTDBCLI_API void rtdbcli_destory_soe(int32_t handle, rtdbcli_soe *soe)
{
}

RTDBCLI_API void rtdbcli_destory_datapoints(int32_t handle, rtdbcli_datapoint *datapoints, int32_t num)
{
}

RTDBCLI_API void rtdbcli_destory_soes(int32_t handle, rtdbcli_soe *soes, int32_t num)
{
}

RTDBCLI_API int32_t rtdbcli_upload_datapoint(int32_t handle, const rtdbcli_datapoint *datapoint)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_upload_datapoints(int32_t handle, const rtdbcli_datapoint *datapoints, int32_t num)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_upload_soe(int32_t handle, const rtdbcli_soe *soe)
{
    return RTDBCLI_EC_OK;
}

RTDBCLI_API int32_t rtdbcli_upload_soes(int32_t handle, const rtdbcli_soe *soes, int32_t num)
{
    return RTDBCLI_EC_OK;
}
