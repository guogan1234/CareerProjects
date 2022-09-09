#ifndef SDK_TSDB_DRIVER_H
#define SDK_TSDB_DRIVER_H

#include "c_vector.h"

#include "tsdb_types.h"
#include "hinfluxdb.h"

#define BUF_SIZE 1000

struct TsdbDao {
    //master在心跳中被赋值
    InfluxDbContext *master;

    c_vector influxdbContexts;
    //

    /*
     * 销毁数据库访问对象
     */
    int (*close)(struct TsdbDao *_this);

    /*
     * 创建状态数据
     */
    TsdbData *(*new_record)(struct TsdbDao *_this);

    /*
     * 释放状态数据
     */
    void (*free_record)(struct TsdbDao *_this, TsdbData *status);

    void (*free_result)(struct TsdbDao *_this, TsdbResultData *status, size_t count);

    /*
     * 存储状态数据
     */
    int (*save_record)(struct TsdbDao *_this, const char *table, const TsdbData **record, size_t count);

    /*
     * 更改状态数据
     */
    int (*update_record)(struct TsdbDao *_this, const char *table, const TsdbData **record, size_t count);

    /*
     * 删除状态数据
     */
    int (*delete_record)(struct TsdbDao *_this, const char *table, const TsdbData **record, size_t count);

    /*
     * 删除状态数据
     */
    int (*delete_records)(struct TsdbDao *_this, const char *query);

    /*
     * 查询状态数据
     */
    int (*select_record)(struct TsdbDao *_this, const char *query, size_t *size, TsdbResultData **record);
};

struct TsdbDao *tsdb_ctor(char *domain);

#endif //SDK_TSDB_DRIVER_H
