/**
 * @file rtdbcli_fes.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。定义FES采样数据上报接口。
 *       本接口仅支持FES客户端和SFES客户端。
 **/
#ifndef RTDBCLI_FES_H
#define RTDBCLI_FES_H

#include "rtdbcli_macros.h"
#include "rtdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_prepare_datapoint
 * 
 * @detail 提供数据点对象。本接口用于提供一个可用的数据点对象。
 * 
 * @param  handle     输入，客户端实例句柄
 * @param  datapoint  输出，数据点对象，失败返回空
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_prepare_datapoint(int32_t handle, rtdbcli_datapoint **datapoint);

/**
 * @brief  rtdbcli_prepare_soe
 * 
 * @detail 提供SOE对象。本接口用于提供一个可用的SOE对象。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  soe     输出，SOE对象，失败返回空
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_prepare_soe(int32_t handle, rtdbcli_soe **soe);

/**
 * @brief  rtdbcli_prepare_datapoints
 * 
 * @detail 提供数据点对象数组。
 * 
 * @param  handle      输入，客户端实例句柄
 * @param  datapoints  输出，数据点数组
 * @param  num         输入/输出，输入希望创建的数据点个数，输出实际创建的数据点个数。
 *                     实际输出的数据点个数小于等于希望的个数。
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_prepare_datapoints(int32_t handle, rtdbcli_datapoint **datapoints, int32_t *num);

/**
 * @brief  rtdbcli_prepare_soes
 * 
 * @detail 提供SOE对象数组。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  soes    输出，数据点数组
 * @param  num     输入/输出，输入希望创建的数据点个数，输出实际创建的数据点个数。
 *                 实际输出的数据点个数小于等于希望的个数。
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_prepare_soes(int32_t handle, rtdbcli_soe **soes, int32_t *num);

/**
 * @brief  rtdbcli_destory_datapoint
 * 
 * @detail 销毁数据点对象。
 * 
 * @param  handle     输入，客户端实例句柄
 * @param  datapoint  输入，数据点对象
 * 
 * @return 空
 */
RTDBCLI_API void rtdbcli_destory_datapoint(int32_t handle, rtdbcli_datapoint *datapoint);

/**
 * @brief  rtdbcli_destory_soe
 * 
 * @detail 销毁SOE对象。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  soe     输入，SOE对象
 * 
 * @return 空
 */
RTDBCLI_API void rtdbcli_destory_soe(int32_t handle, rtdbcli_soe *soe);

/**
 * @brief  rtdbcli_destory_datapoints
 * 
 * @detail 销毁数据点对象数组。
 * 
 * @param  handle      输入，客户端实例句柄
 * @param  datapoints  输入，数据点对象数组
 * @param  num         输入，数据点对象个数
 * 
 * @return 空
 */
RTDBCLI_API void rtdbcli_destory_datapoints(int32_t handle, rtdbcli_datapoint *datapoints, int32_t num);

/**
 * @brief  rtdbcli_destory_soes
 * 
 * @detail 销毁SOE对象数组。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  soes    输入，SOE对象数组
 * @param  num     输入，数据点对象个数
 * 
 * @return 空
 */
RTDBCLI_API void rtdbcli_destory_soes(int32_t handle, rtdbcli_soe *soes, int32_t num);

/**
 * @brief  rtdbcli_upload_datapoint
 * 
 * @detail 上报数据点采样值。
 * 
 * @param  handle     输入，客户端实例句柄
 * @param  datapoint  输入，数据点
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_upload_datapoint(int32_t handle, const rtdbcli_datapoint *datapoint);

/**
 * @brief  rtdbcli_upload_datapoints
 * 
 * @detail 批量上报数据点采样值。
 * 
 * @param  handle      输入，客户端实例句柄
 * @param  datapoints  输入，数据点数组
 * @param  num         输入，数据点个数
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_upload_datapoints(int32_t handle, const rtdbcli_datapoint *datapoints, int32_t num);

/**
 * @brief  rtdbcli_upload_soe
 * 
 * @detail 上报SOE。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  soe    输入，SOE
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_upload_soe(int32_t handle, const rtdbcli_soe *soe);

/**
 * @brief  rtdbcli_upload_soes
 * 
 * @detail 批量上报SOE。
 * 
 * @param  handle  输入，客户端实例句柄
 * @param  soes    输入，SOE数组
 * @param  num     输入，SOE个数
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_upload_soes(int32_t handle, const rtdbcli_soe *soes, int32_t num);

#ifdef __cplusplus
}
#endif
#endif
