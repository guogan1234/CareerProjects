/**
 * @file rtdbcli_malloc.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本文件定义客户用到的内存操作接口。
 **/
#ifndef RTDBCLI_MALLOC_H
#define RTDBCLI_MALLOC_H

#include "rtdbcli_macros.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_mem_allocator
 * 
 * @detail 内存分配器。
 * 
 * @param  size  输入，内存大小(字节)
 * 
 * @return 返回分配的内存首地址，失败返回空
 */
typedef void*(*rtdbcli_mem_allocator)(size_t size);
/**
 * @brief  rtdbcli_mem_collector
 * 
 * @detail 内存回收器。
 * 
 * @param  ptr  输入，内存指针
 * 
 * @return 无
 */
typedef void(*rtdbcli_mem_collector)(void*);

/**
 * @brief  rtdbcli_set_malloc_functions
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
RTDBCLI_API void rtdbcli_set_malloc_functions(
    rtdbcli_mem_allocator allocator,
    rtdbcli_mem_collector collector
);

/// 内存操作接口变量
extern rtdbcli_mem_allocator rtdbcli_malloc;
extern rtdbcli_mem_collector rtdbcli_free;

#ifdef __cplusplus
}
#endif
#endif
