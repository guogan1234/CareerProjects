/**
 * @file sysconf_malloc.h
 * @author
 * @date 
 * @version 
 * @note 系统配置口文件。本文件定义接口用到的内存操作接口。
 **/
#ifndef SYSCONF_MALLOC_H
#define SYSCONF_MALLOC_H

#include "sysconf_macros.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  sysconf_mem_allocator
 * 
 * @detail 内存分配器。
 * 
 * @param  size  输入，内存大小(字节)
 * 
 * @return 返回分配的内存首地址，失败返回空
 */
typedef void*(*sysconf_mem_allocator)(size_t size);
/**
 * @brief  sysconf_mem_collector
 * 
 * @detail 内存回收器。
 * 
 * @param  ptr  输入，内存指针
 * 
 * @return 无
 */
typedef void(*sysconf_mem_collector)(void*);

/**
 * @brief  sysconf_set_malloc_functions
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
SYSCONF_API void sysconf_set_malloc_functions(
    sysconf_mem_allocator allocator,
    sysconf_mem_collector collector
);

/// 内存操作接口变量
extern sysconf_mem_allocator sysconf_malloc;
extern sysconf_mem_collector sysconf_free;

#ifdef __cplusplus
}
#endif
#endif
