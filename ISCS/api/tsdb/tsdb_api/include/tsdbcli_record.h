/**
 * @file tsdbcli_record.h
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口。本接口文件定义TSDB客户端用到的记录操作接口。
 *
 **/
#ifndef TSDBCLI_RECORD_H
#define TSDBCLI_RECORD_H

#include "tsdbcli_macros.h"
#include "tsdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  tsdbcli_get_field_int64
 * 
 * @detail 获取字段值。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输出，字段值
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_get_field_int64(
    const tsdbcli_record *record,
    const char *field,
    int64_t *value
);

/**
 * @brief  tsdbcli_get_field_int32
 * 
 * @detail 获取字段值。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输出，字段值
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_get_field_int32(
    const tsdbcli_record *record,
    const char *field,
    int32_t *value
);

/**
 * @brief  tsdbcli_get_field_int16
 * 
 * @detail 获取字段值。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输出，字段值
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_get_field_int16(
    const tsdbcli_record *record,
    const char *field,
    int16_t *value
);

/**
 * @brief  tsdbcli_get_field_int8
 * 
 * @detail 获取字段值。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输出，字段值
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_get_field_int8(
    const tsdbcli_record *record,
    const char *field,
    int8_t *value
);

/**
 * @brief  tsdbcli_get_field_double
 * 
 * @detail 获取字段值。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输出，字段值
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_get_field_double(
    const tsdbcli_record *record,
    const char *field,
    double *value
);

/**
 * @brief  tsdbcli_get_field_string
 * 
 * @detail 获取字段值。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输出，字段值，调用tsdbcli_free_string释放
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_get_field_string(
    const tsdbcli_record *record,
    const char *field,
    char **value
);

/**
 * @brief  tsdbcli_set_field_int64
 * 
 * @detail 设置字段。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输入，字段值
 * @param  isTag   输入，字段是否是TAG（0 -- 不是TAG， 1 -- 是TAG）
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_set_field_int64(
    tsdbcli_record *record,
    const char *field,
    int64_t value,
    int8_t isTag
);

/**
 * @brief  tsdbcli_set_field_int32
 * 
 * @detail 设置字段。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输入，字段值
 * @param  isTag   输入，字段是否是TAG（0 -- 不是TAG， 1 -- 是TAG）
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_set_field_int32(
    tsdbcli_record *record,
    const char *field,
    int32_t value,
    int8_t isTag
);

/**
 * @brief  tsdbcli_set_field_int16
 * 
 * @detail 设置字段。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输入，字段值
 * @param  isTag   输入，字段是否是TAG（0 -- 不是TAG， 1 -- 是TAG）
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_set_field_int16(
    tsdbcli_record *record,
    const char *field,
    int16_t value,
    int8_t isTag
);

/**
 * @brief  tsdbcli_set_field_int8
 * 
 * @detail 设置字段。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输入，字段值
 * @param  isTag   输入，字段是否是TAG（0 -- 不是TAG， 1 -- 是TAG）
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_set_field_int8(
    tsdbcli_record *record,
    const char *field,
    int8_t value,
    int8_t isTag
);

/**
 * @brief  tsdbcli_set_field_double
 * 
 * @detail 设置字段。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输入，字段值
 * @param  isTag   输入，字段是否是TAG（0 -- 不是TAG， 1 -- 是TAG）
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_set_field_double(
    tsdbcli_record *record,
    const char *field,
    double value,
    int8_t isTag
);

/**
 * @brief  tsdbcli_set_field_string
 * 
 * @detail 设置字段。
 * 
 * @param  record  输入，记录
 * @param  field   输入，字段名
 * @param  value   输入，字段值
 * @param  isTag   输入，字段是否是TAG（0 -- 不是TAG， 1 -- 是TAG）
 * 
 * @return 返回状态 @see TSDBCLI_EC_XXX
 */
TSDBCLI_API int32_t tsdbcli_set_field_string(
    tsdbcli_record *record,
    const char *field,
    const char *value,
    int8_t isTag
);

#ifdef __cplusplus
}
#endif
#endif
