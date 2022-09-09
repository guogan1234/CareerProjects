#include <stdlib.h>

#include "tsdb_driver.h"
#include "tsdb_priv.h"
#include "tsdb.h"

/*
* TSDB 初始化
*
*/
TsdbDao *tsdb_create(char *domain) {
    return tsdb_ctor(domain);
}

/*
* 销毁数据库访问对象
*/
int tsdb_close(TsdbDao *tsdbDao) {
    if (tsdbDao && tsdbDao->close)
        return tsdbDao->close(tsdbDao);

    return -1;
}

/*
* 创建状态数据
*/
TsdbData *tsdb_new_record(TsdbDao *tsdbDao) {
    if (tsdbDao && tsdbDao->new_record)
        return tsdbDao->new_record(tsdbDao);

    return NULL;
}


/*
* 释放状态数据
*/
void tsdb_free_record(TsdbDao *tsdbDao, TsdbData *status) {
    if (tsdbDao && tsdbDao->free_record)
        tsdbDao->free_record(tsdbDao, status);
}

/*
* 释放状态数据
*/
void tsdb_free_result(TsdbDao *tsdbDao, TsdbResultData *status, size_t count) {
    if (tsdbDao && tsdbDao->free_result)
        tsdbDao->free_result(tsdbDao, status, count);
}

/*
* 存储状态数据
*/
int tsdb_save_record(TsdbDao *tsdbDao, const char *table, const TsdbData **record, size_t count) {
    if (tsdbDao && tsdbDao->save_record)
        return tsdbDao->save_record(tsdbDao, table, record, count);

    return -1;
}

/*
* 查询状态数据
*/
int tsdb_select_record(TsdbDao *tsdbDao, const char *query, size_t *size, TsdbResultData **record) {
    if (tsdbDao && tsdbDao->select_record)
        return tsdbDao->select_record(tsdbDao, query, size, record);

    return -1;
}

/*
* 存储状态数据
*/
int tsdb_update_record(TsdbDao *tsdbDao, const char *table, const TsdbData **record, size_t count) {
    if (tsdbDao && tsdbDao->update_record)
        return tsdbDao->update_record(tsdbDao, table, record, count);

    return -1;
}

int tsdb_delete_record(TsdbDao *tsdbDao, const char *table, const TsdbData **record, size_t count) {
    if (tsdbDao && tsdbDao->delete_record)
        return tsdbDao->delete_record(tsdbDao, table, record, count);

    return -1;
}

int tsdb_delete_records(TsdbDao *tsdbDao, const char *query) {
    if (tsdbDao && tsdbDao->delete_records)
        return tsdbDao->delete_records(tsdbDao, query);

    return -1;
}