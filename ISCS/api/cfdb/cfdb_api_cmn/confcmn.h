/*
 @Copyright Reserved.
 CONFDB.
 Define ConfDb http based query/exec interface.
 Create by Boris.F, 2018.05.23.
 Histories:

*/

#ifndef _CONF_CMN_H
#define _CONF_CMN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RecordSet ConfDbRecordSet;

//分页查询参数
typedef struct Pageable {
    int pageSize; //每页记录数量
    int pageNumber; //当前页，从0开始
} ConfDbPageable;

//返回分页查询结果对象
typedef struct Page {
    int number; //当前页，从0开始
    int size;   //每页记录数量
    int totalPages; //所有页数
    int hasPrevious;
    int hasNext;
    long totalElements; //所有记录数
} ConfDbPage;

/*
功能：数据库接口初始化
说明：
    装载配置信息
参数：

*/
int confdb_init();

/*
功能：数据库接口内部资源释放
说明：
    无
参数：

*/
int confdb_release();

/*
功能：执行update/insert/delete操作
说明：
    尽量避免使用多条sql语句，异常无法定位是那条SQL语句产生
参数：
    sql      :  通用sql语句
*/
int confdb_exec(const char* sql);

/*
功能：sql查询接口
说明：
    无
参数：
    sql      :  通用sql语句
*/
ConfDbRecordSet* confdb_query(const char* sql);

/*
功能：sql分页查询接口
说明：
    无
参数：
    sql      :  通用sql语句
    page     :  分页条件
*/
ConfDbRecordSet* confdb_query_page(const char* sql, const ConfDbPageable* page);

/*
功能：查询结果资源回收
说明：
    无
参数：
    rs      :  数据集
*/
void confdb_recordset_free(ConfDbRecordSet* rs);

/*
功能：查询指定位置（Row）数据
说明：
    无
参数：
    rs      :  数据集
    row     :  行数，以0起始
    colName :  数据库字段名，不区分大小写
*/
const char* confdb_recordset_get(ConfDbRecordSet* rs, const int row, const char* colName);

/*
功能：查询指定位置（Row）下多个Column数据
说明：
    无
参数：
    rs      :  数据集
    row     :  行数，以0起始
    colNames:  数据库字段名数组，不区分大小写
*/
const char** confdb_recordset_multi_get(ConfDbRecordSet* rs, const int row, const char* colNames[], int size);
int confdb_recordset_multi_free(ConfDbRecordSet* rs, char** values);


/*
功能：分页信息
说明：
    如果查询时不含分页条件，返回为NULL
参数：
    rs      :  数据集
*/
ConfDbPage* confdb_recordset_get_page(ConfDbRecordSet* rs);

/*
功能：数据集行数
说明：
    无
参数：
    rs      :  数据集
*/
int confdb_recordset_get_count(ConfDbRecordSet* rs);

/*
功能：取数据集列名数组
说明：
    无
参数：
    rs      :  数据集
    columns :  列名数组[out]
    size    :  数组大小[out]
*/
int confdb_recordset_get_columns(ConfDbRecordSet* rs, char*** columns, int* size);

int confdb_recordset_get_code(ConfDbRecordSet* rs);
#ifdef __cplusplus
}
#endif


#endif //_CONF_CMN_H
