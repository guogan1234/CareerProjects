/**
 * @file tsdbcli_malloc.c
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口文件。本文件定义接口用到的内存操作接口实现。
 **/
#include "tsdbcli_malloc.h"
#include <stdlib.h>

/// 定义接口用到的内存申请接口
tsdbcli_mem_allocator tsdbcli_malloc;

/// 定义接口用到的内存释放接口
tsdbcli_mem_collector tsdbcli_free;

TSDBCLI_API void tsdbcli_set_malloc_functions(
    tsdbcli_mem_allocator allocator,
    tsdbcli_mem_collector collector
)
{
    tsdbcli_malloc = allocator ? allocator : malloc;
    tsdbcli_free = collector ? collector : free;
}
