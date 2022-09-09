/**
 * @file sysconf_malloc.c
 * @author
 * @date 
 * @version 
 * @note 系统配置口文件。本文件定义接口用到的内存操作接口实现。
 **/
#include "sysconf_malloc.h"
#include <stdlib.h>

/// 定义接口用到的内存申请接口
sysconf_mem_allocator sysconf_malloc = malloc;

/// 定义接口用到的内存释放接口
sysconf_mem_collector sysconf_free = free;

SYSCONF_API void sysconf_set_malloc_functions(
    sysconf_mem_allocator allocator,
    sysconf_mem_collector collector
)
{
    sysconf_malloc = allocator ? allocator : malloc;
    sysconf_free = collector ? collector : free;
}
