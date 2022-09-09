/**
 * @file tsdbcli_data.h
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口。本接口文件定义TSDB客户端的数据对象和对象操作接口。
 *
 **/
#ifndef TSDBCLI_DATA_H
#define TSDBCLI_DATA_H

#include "tsdbcli_macros.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/// 定义记录对象
typedef struct _tsdbcli_record tsdbcli_record;

/**
 * @brief  tsdbcli_free_string
 * 
 * @detail 释放字符串。
 * 
 * @param  s  输入，记录
 * 
 * @return 无
 */
TSDBCLI_API void tsdbcli_free_string(char *s);

#ifdef __cplusplus
}
#endif
#endif
