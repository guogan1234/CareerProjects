/**
 * @file rtdbcli_malloc.h
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。本文件定义客户用到的内存操作接口。
 **/
#include "rtdbcli_malloc.h"
#include <stdlib.h>

/// 定义接口用到的内存申请接口
rtdbcli_mem_allocator rtdbcli_malloc = malloc;

/// 定义接口用到的内存释放接口
rtdbcli_mem_collector rtdbcli_free = free;

RTDBCLI_API void rtdbcli_set_malloc_functions(
    rtdbcli_mem_allocator allocator,
    rtdbcli_mem_collector collector
)
{
    rtdbcli_malloc = allocator ? allocator : malloc;
    rtdbcli_free = collector ? collector : free;
}
