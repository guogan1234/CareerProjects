/**
 * @file tsdbcli_malloc.h
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口。本文件定义接口用到的内存操作接口。
 **/
#ifndef TSDBCLI_MALLOC_H
#define TSDBCLI_MALLOC_H

#include "tsdbcli_macros.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  tsdbcli_mem_allocator
 * 
 * @detail 内存分配器。
 * 
 * @param  size  输入，内存大小(字节)
 * 
 * @return 返回分配的内存首地址，失败返回空
 */
typedef void*(*tsdbcli_mem_allocator)(size_t size);
/**
 * @brief  tsdbcli_mem_collector
 * 
 * @detail 内存回收器。
 * 
 * @param  ptr  输入，内存指针
 * 
 * @return 无
 */
typedef void(*tsdbcli_mem_collector)(void*);

/**
 * @brief  tsdbcli_set_malloc_functions
 * 
 * @detail 设置内存分配器和内存回收器。如果指定内存分配和回收方法，
 *         必须在调用本接口的其它接口前，调用本方法。
 *         如果没有调用本方法，接口默认使用系统的内存分配和回收方法。
 * 
 * @param  allocator  输入，内存分配器
 * @param  collector  输出，内存回收器
 * 
 * @return 无
 */
TSDBCLI_API void tsdbcli_set_malloc_functions(
    tsdbcli_mem_allocator allocator,
    tsdbcli_mem_collector collector
);

/// 内存操作接口变量
extern tsdbcli_mem_allocator tsdbcli_malloc;
extern tsdbcli_mem_collector tsdbcli_free;

#ifdef __cplusplus
}
#endif
#endif
