/**
 * @file rtdbcli_node.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本文件定义点模型的操作接口。
 *       仅RTDB客户端支持点模型接口。
 **/
#ifndef RTDBCLI_NODE_H
#define RTDBCLI_NODE_H

#include "rtdbcli_macros.h"
#include "rtdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_get_attribute
 * 
 * @detail 从RTDB查询点的指定属性。本接口仅支持查询点的可见属性。
 *         可能的路径格式有：
 *         domain.attribute
 *         domain:station.attribute
 *         domain:station:system.attribute
 *         domain:station:system:device.attribute
 *         domain:station:system:device:point.attribute
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点属性的路径
 * @param  attr    输出，属性对象,调用 rtdbcli_free_attribute 释放
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_get_attribute(int32_t handle, const char *path, rtdbcli_attribute **attr);

/**
 * @brief  rtdbcli_get_attributes
 * 
 * @detail 从RTDB查询点的多个属性。本接口仅支持查询点的可见属性。
 *         返回的属性记录中属性顺序为输入的属性顺序，属性不存在，对应位置为空值。
 * 
 *         可能的路径格式有：
 *         domain
 *         domain:station
 *         domain:station:system
 *         domain:station:system:device
 *         domain:station:system:device:point
 * 
 * @param  handle  输入，RTDB客户端实例实例句柄
 * @param  path    输入，点路径
 * @param  names   输入，属性名称（键）数组
 * @param  size    输入，属性名称个数
 * @param  record  输出，属性记录,调用 rtdbcli_free_record 释放
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_get_attributes(
    int32_t handle, 
    const char *path, 
    const char **names, 
    int16_t size, 
    rtdbcli_record **record
);

/**
 * @brief  rtdbcli_mget_attributes
 * 
 * @detail 从RTDB查询多个点的多个属性。本接口仅支持查询点的可见属性。
 *         返回的点的属性记录顺序为输入的点的顺序，点不存在，对应的位置为空值。
 *         返回的属性记录中属性顺序为输入的属性顺序，属性不存在，对应位置为空值。
 *         
 *         可能的路径格式有：
 *         domain
 *         domain:station
 *         domain:station:system
 *         domain:station:system:device
 *         domain:station:system:device:point
 * 
 * @param  handle  输入，RTDB客户端实例实例句柄
 * @param  paths   输入，点路径数组
 * @param  pnum    输入，路径个数
 * @param  names   输入，属性名称数组
 * @param  nnum    输入，属性名称个数
 * @param  records 输出，属性记录数组，记录个数等于点路径个数。调用 rtdbcli_free_records 释放
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_mget_attributes(
    int32_t handle, 
    const char **paths, 
    int16_t pnum, 
    const char **names, 
    int16_t nnum, 
    rtdbcli_record **records
);

/**
 * @brief  rtdbcli_get_node
 * 
 * @detail 从RTDB查询点所有属性。本接口仅支持查询点的可见属性。
 *         点的路径格式参看@see rtdbcli_get_attribute 注释。
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点路径
 * @param  record  输出，属性记录,调用 rtdbcli_free_record 释放
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_get_node(int32_t handle, const char *path, rtdbcli_record **record);

/**
 * @brief  rtdbcli_get_output_node
 * 
 * @detail 从RTDB查询输出点。返回输出点的全部属性。
 *         点的路径格式参看@see rtdbcli_get_attribute 注释。
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点路径
 * @param  record  输出，属性记录,调用 rtdbcli_free_record 释放
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_get_output_node(int32_t handle, const char *path, rtdbcli_record **record);

/**
 * @brief  rtdbcli_enumerate_childkey
 * 
 * @detail 从RTDB枚举点的所有直接孩子节点的KEY。
 *         点的路径格式参看@see rtdbcli_get_attribute 注释。
 *         返回值在使用完后，调用 rtdbcli_free_strings 资源。
 * 
 * @param  handle  输入，RTDB客户端实例
 * @param  path    输入，点路径
 * @param  num     输出，节点KEY个数
 * @param  ec      输出，错误码 @see RTDBCLI_EC_XXX
 * 
 * @return 返回点的孩子节点的KEY数组，失败时返回空
 */
RTDBCLI_API char** rtdbcli_enumerate_childkey(int32_t handle, const char *path, int32_t *num, int32_t *ec);

/**
 * @brief  rtdbcli_enumerate_attributekey
 * 
 * @detail 从RTDB枚举点的所有属性KEY。点的路径格式参看@see rtdbcli_get_attribute 注释。
 *         返回值在使用完后，调用 rtdbcli_free_strings 资源。
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点路径
 * @param  num     输出，点的属性KEY个数
 * @param  ec      输出，错误码 @see RTDBCLI_EC_XXX
 * 
 * @return 返回点的属性KEY数组，失败时返回空
 */
RTDBCLI_API char** rtdbcli_enumerate_attributekey(int32_t handle, const char *path, int32_t *num, int32_t *ec);

/**
 * @brief  rtdbcli_get_nodetype
 * 
 * @detail 从RTDB查询点的类型。点的路径格式参看@see rtdbcli_get_attribute 注释。
 *         点的类型参看@see RTDBCLI_NODE_XXX定义。
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点路径
 * @param  type    输出，输出点的类型，失败时为-1
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_get_nodetype(int32_t handle, const char *path, int32_t *type);

/**
 * @brief  rtdbcli_set_attribute
 * 
 * @detail 修改RTDB中点的属性值。点的路径格式参看@see rtdbcli_get_attribute 注释。
 *         参数"value"为字符串形式的值，例如属性值为整数123，则value的值为"123"；
 *         属性值为浮点数123.45，则value的值为“123.45”。
 * 
 * @param  handle  输入，RTDB客户端实例
 * @param  path    输入，点属性路径
 * @param  value   输入，属性值
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_set_attribute(int32_t handle, const char *path, const char *value);

/**
 * @brief  rtdbcli_bitset_status
 * 
 * @detail 按位设置RTDB中点的状态值。
 *         点的路径格式参看@see rtdbcli_get_attribute 注释。
 *         参数"bit"（比特位）的值范围定义如下：
 *         [0 -- 31]：属性值类型为int32_t
 *         参数"value"（值）的范围：[0,1]
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点路径
 * @param  bit     输入，比特位
 * @param  value   输入，属性值
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_bitset_status(int32_t handle, const char *path, int32_t bit, int32_t value);

/**
 * @brief  rtdbcli_get_svtc
 * 
 * @detail 从RTDB数据点查询SVTC。点的路径格式参看@see rtdbcli_get_attribute 注释。
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点的路径
 * @param  svtc    输出，点的SVTC
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_get_svtc(int32_t handle, const char *path, char **svtc);

/**
 * @brief  rtdbcli_set_svtc
 * 
 * @detail 修改RTDB数据点的SVTC。点的路径格式参看@see rtdbcli_get_attribute 注释。
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点的路径
 * @param  svtc    输入，点的SVTC
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_set_svtc(int32_t handle, const char *path, const char *svtc);

/**
 * @brief  rtdbcli_get_safecollection
 * 
 * @detail 获取数据点的安全采样数据。点的路径格式参看@see rtdbcli_get_attribute 注释。
 *         如果是模拟量点的路径，则返回安全的模拟量采样数据；
 *         如果是累积量点的路径，则返回安全的累积量采样数据；
 *         如果是数字量点的路径，则返回安全的数字量采样数据；
 * 
 * @param  handle  输入，RTDB客户端实例句柄
 * @param  path    输入，点的路径
 * @param  sdata   输出，数据点的安全采样数据
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_get_safecollection(int32_t handle, const char *path, char **sdata);

#ifdef __cplusplus
}
#endif
#endif
