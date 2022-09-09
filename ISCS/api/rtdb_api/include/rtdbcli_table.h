/**
 * @file rtdbcli_table.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本文件定义表模型操作接口。
 *       仅RTDB客户端支持本文件定义的接口。
 **/
#ifndef RTDBCLI_TABLE_H
#define RTDBCLI_TABLE_H

#include "rtdbcli_macros.h"
#include "rtdbcli_data.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  rtdbcli_query_records
 * 
 * @detail 从RTDB表模型中查询记录。
 *         查询条件为空，则表示返回表的全部记录；否则返回满足条件的记录。
 *         字段名称数组为空，返回的记录中包含全部字段；否则，记录仅包含数组包含的字段。
 *         返回值在使用完后，调用rtdbcli_free_records释放。
 * 
 *         参数“cond”（查询条件）支持的运算符：=, !=, >, >=, <, <=, &, |
 *         查询条件类型支持：单字段，多字段与，多字段或。
 *         查询条件不要包含空格。示例如下：
 *         "name='Joe'"， "name!='Joe'"，
 *         "age=1"， "age!=1"， "age>1"， "age>=1"
 *         "age=1|name='Joe'"， "age=1&name='Joe'"
 * 
 *         参数"fieldnames"（表字段名）不能使用如下的关键字：
 *         and, break, do, else, elseif, end, false, for, function, if, in,
 *         local, nil, not, or, repeat, return, then, true, until, while,
 *         table, goto
 *         
 * @param  handle      输入，RTDB客户端实例句柄
 * @param  domain      输入，表所属域的ID
 * @param  tablename   输入，表名称
 * @param  cond        输入，查询条件
 * @param  fieldnames  输入，字段名称数组，空为全部。
 * @param  fieldnum    输入，字段名称个数
 * @param  index       输入，从查询结果的第几条记录开始返回(从1开始)
 * @param  totalnum    输出，本次查询命中的记录总条数
 * @param  recordnum   输入/出，输入需要返回的记录条数，输出实际返回的记录条数
 *                     实际输出的条数小于等于期望返回的条数(多余的丢掉)。
 * @param  records     输出，记录数组，记录不存在或失败时返回空。调用 rtdbcli_free_records 释放
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_query_records(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond,
    const char **fieldnames,
    int32_t fieldnum,
    int32_t index,
    int32_t *totalnum,
    int32_t *recordnum,
    rtdbcli_record **records
);

/**
 * @brief  rtdbcli_update_records
 * 
 * @detail 更新RTDB表记录。
 *         查询条件为空，则表示更新表的所有记录；否则，只更条件为真的记录。
 *         查询条件“cond”参见@see rtdbcli_query_records说明。
 *         
 * @param  handle      输入，RTDB客户端实例句柄
 * @param  domain      输入，表所属域的ID
 * @param  tablename   输入，表名称
 * @param  cond        输入，查询条件
 * @param  record      输入，目标值记录
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_update_records(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond,
    const rtdbcli_record *record
);

/**
 * @brief  rtdbcli_insert_record
 * 
 * @detail 向RTDB表模型中插入一条记录。
 *         只有在查询条件为空或查询条件为假时，执行记录插入操作；条件真时，不插入。
 *         查询条件“cond”参见@see rtdbcli_query_records说明。 
 *         
 * @param  handle      输入，RTDB客户端实例句柄
 * @param  domain      输入，域ID
 * @param  tablename   输入，表名称
 * @param  cond        输入，查询条件
 * @param  record      输入，要插入的新纪录
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_insert_record(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond,
    const rtdbcli_record *record
);

/**
 * @brief  rtdbcli_delete_records
 * 
 * @detail 删除RTDB表模型中的记录。
 *         删除条件为空，仅做清空表的所有记录操作，保留表对象；否则，仅删除条件为真的记录。
 *         查询条件“cond”参见@see rtdbcli_query_records说明。
 *         
 * @param  handle      输入，RTDB客户端实例句柄
 * @param  domain      输入，表所属域的ID
 * @param  tablename   输入，表名称
 * @param  cond        输入，删除条件
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_delete_records(
    int32_t handle,
    int32_t domain,
    const char *tablename,
    const char *cond
);

/**
 * @brief  rtdbcli_proxy_insert_record
 * 
 * @detail 向历史库插入一条记录。本接口把记录发送给RTDB，由RTDB负责把记录写入历史数据库。
 *         
 * @param  handle      输入，RTDB客户端实例句柄
 * @param  tablename   输入，表名称
 * @param  tags        输入，记录的Tags（记录的主键字段）部分
 * @param  fields      输入，记录的Fields（记录的普通字段）部分
 * 
 * @return 返回错误状态 @see RTDBCLI_EC_XXX
 */
RTDBCLI_API int32_t rtdbcli_proxy_insert_record(
    int32_t handle,
    const char *tablename,
    const rtdbcli_record *tags,
    const rtdbcli_record *fields
);

#ifdef __cplusplus
}
#endif
#endif
