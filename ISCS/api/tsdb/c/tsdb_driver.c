#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

#define __USE_XOPEN

#include <time.h>
#include <sods/sods.h>

#include "tsdb_driver.h"
#include "tsdb_priv.h"
#include "tsdb.h"
#include "tsdb_types.h"

static int select_record(TsdbDao *_this, const char *query, size_t *size, TsdbResultData **record);

static int close(TsdbDao *_this) {
    if (_this) {
        // destroy clients
        c_iterator iter = c_vector_begin(&_this->influxdbContexts);
        c_iterator end = c_vector_end(&_this->influxdbContexts);
        for (; !ITER_EQUAL(iter, end); ITER_INC(iter)) {
            if (ITER_REF(iter)) {
                InfluxDbContext *client = (InfluxDbContext *) ITER_REF(iter);
                tsdb_influx_destroy(client);
            }
        }
        c_vector_destroy(&_this->influxdbContexts);

        //
        free(_this);
    }

    return 0;
}

/*
 * 创建状态数据
 */
static TsdbData *new_record(TsdbDao *_this) {
    if (_this) {
        TsdbData *status = malloc(sizeof(TsdbData));
        if (status) {
            memset(status, 0x00, sizeof(*status));
            status->data = TsdbRecordNew();
            return status;
        }
    }

    return NULL;
}

/*
 * 释放状态数据
 */
static void free_record(TsdbDao *_this, TsdbData *status) {
    if (_this && status) {
        if (status->data) TsdbRecordFree(status->data);

        free(status);
    }
}

/*
 * 释放状态数据
 */
static void free_result(TsdbDao *_this, TsdbResultData *status, size_t count) {
    if (!_this || !status)
        return;

    for (int i = 0; i < count; i++) {
        if (status[i].data) TsdbResultRecordFree(status[i].data);
    }

    free(status);
}

/*
 * 存储状态数据
 */
static int save_record(TsdbDao *_this, const char *table, const TsdbData **record, size_t count) {
    sods s = sodsempty();
    for (int i = 0; i < count; i++) {
        sods line = RecordToString(table, record[i]->data);
        char time[20] = {0};
        record[i]->timestamp ? sprintf(time, " %ld", record[i]->timestamp) : 0;
        s = sodscat(s, line);
        sodsfree(line);
        s = sodscat(s, time);
        s = sodscat(s, "\n");
    }
    s[sodslen(s) - 1] = '\0';
    sodsupdatelen(s);

    int ret = tsdb_influx_write_str(_this->master, "POST", s);
    sodsfree(s);

    return ret == 0 ? 0 : E_OTHERS;
}

/*
 * 更改状态数据
 */
static int update_record(TsdbDao *_this, const char *table, const TsdbData **record, size_t count) {
    for (size_t i = 0; i < count; i++) {
        const TsdbData *r = record[i];
        char *q = TsdbDataToQuery(r);
        if (q == NULL)
            return E_NULL_POINTER;

        sods s = sodsnew("SELECT * FROM ");
        s = sodscat(s, table);
        s = sodscat(s, " WHERE ");
        s = sodscat(s, q);
        free(q);
        s = sodscat(s, " LIMIT 1");

        size_t n = 0;
        TsdbResultData *v = NULL;
        select_record(_this, s, &n, &v);
        sodsfree(s);
        if (n == 0)
            continue;

        CopyRecordToTsdbData(v, (TsdbData *) r);
        free_result(_this, v, 1);
    }
    return save_record(_this, table, record, count);
}

/*
 * 删除状态数据
 */
static int delete_record(TsdbDao *_this, const char *table, const TsdbData **record, size_t count) {
    char buf[256] = {0};

    // KWD
    // 删除记录：根据record的条件查询出要删除的记录，然后根据要删除的记录的时间进行删除
    //
    for (size_t i = 0; i < count; i++) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "DELETE FROM %s WHERE time=%ld", table, record[i]->timestamp);
        JSON_Value *r = tsdb_influx_query(_this->master, "POST", buf);
        if (r != NULL) {
            json_value_free(r);
        }
    }
    return 0;
}

/*
 * 删除状态数据
 */
static int delete_records(TsdbDao *_this, const char *query) {
    JSON_Value *r = tsdb_influx_query(_this->master, "POST", query);
    if (r != NULL) {
        json_value_free(r);
        return 0;
    }
    return -1;
}

/*
 * 查询状态数据
 */
static int select_record(TsdbDao *_this, const char *query, size_t *size, TsdbResultData **record) {
    JSON_Value *result = tsdb_influx_query(_this->master, "GET", query);
    if (result == NULL) {
        *size = 0;
        return 0;
    }

    JSON_Object *object = json_value_get_object(result);
    if (object == NULL) {
        json_value_free(result);
        *size = 0;
        return 0;
    }

    JSON_Array *resultArr = json_object_get_array(object, "results");
    if (resultArr == NULL || json_array_get_count(resultArr) == 0) {
        json_value_free(result);
        *size = 0;
        return 0;
    }

    JSON_Object *resultObj = json_array_get_object(resultArr, 0);
    if (resultObj == NULL || json_object_get_count(resultObj) == 0) {
        json_value_free(result);
        *size = 0;
        return 0;
    }

    JSON_Array *series = json_object_get_array(resultObj, "series");
    if (series == NULL || json_array_get_count(series) == 0) {
        json_value_free(result);
        *size = 0;
        return 0;
    }

    JSON_Object *data = json_array_get_object(series, 0);
    JSON_Array *colArr = json_object_get_array(data, "columns");
    JSON_Array *rowArr = json_object_get_array(data, "values");
    size_t rowCount = json_array_get_count(rowArr);
    if (rowCount == 0) {
        json_value_free(result);
        *size = 0;
        return 0;
    }

    *size = rowCount;

    *record = (TsdbResultData *) malloc(sizeof(TsdbResultData) * rowCount);

    size_t colCount = json_array_get_count(colArr);
    for (size_t i = 0; i < rowCount; i++) {
        JSON_Value *v = json_value_init_object();
        JSON_Object *obj = json_value_get_object(v);
        JSON_Array *row = json_array_get_array(rowArr, i);
        for (size_t j = 0; j < colCount; j++) {// KWD 如果第一列统一为time列，这里是不是从1开始
            const char *name = json_array_get_string(colArr, j);
            JSON_Value *value = json_array_get_value(row, j);
            JSON_Value_Type type = json_value_get_type(value);
            JSON_Value *newValue = NULL;
            if (type == JSONString) {
                newValue = json_value_init_string(json_value_get_string(value));
            } else if (type == JSONNumber) {
                newValue = json_value_init_number(json_value_get_number(value));
            } else if (type == JSONInteger) {
                newValue = json_value_init_integer(json_value_get_integer(value));
            }
            if (newValue == NULL) {
                continue;
            }
            json_object_set_value(obj, name, newValue);
        }
        ResultRecord *r = JsonValueToResultRecord(v);
        const char *time = json_array_get_string(row, 0);
        struct tm tm;
        memset(&tm, 0, sizeof(struct tm));
        int sec = 0;
        // KWD
        // 以太网日期时间格式: "YYYY-mm-ddTHH:MM:SS.xxxZ"，它表示的意思是：
        // UTC时间YYYY年mm月dd日，HH时MM分SS秒，0.xxx秒。
        // 
        // 以太网日期时间转时间戳示例：
        // 精度为毫秒："2018-12-11T14:25:43.83Z"，转换时“.83Z”的处理是： 0.83*1000
        // 精度为微秒："2018-12-11T14:25:43.83Z"，转换时“.83Z”的处理是： 0.83*1000000
        // 精度为纳秒："2018-12-11T14:25:43.83Z"，转换时“.83Z”的处理是： 0.83*1000000000
        //
        // InfluxDB的时间精度为纳秒，例如InfluxDB数据库表中time的值“1528032113112460030”
        // 通过HTTP查询到的时间是"2018-06-03T13:21:53.11246003Z"，在转换成时数字时间戳时，
        // ".11246003Z"部分的处理方式是0.11246003*1000000000
        //
        sscanf(time, "%4d-%2d-%2dT%2d:%2d:%2d.%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
               &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &sec);
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;

        if (time[19] == '.') {
            for (int n = strlen(time + 19); n < 11; n++) {
                sec *= 10;
            }
        }

        int64_t seconds = mktime(&tm);
        seconds += 28800;
        seconds *= 1000000000;
        seconds += sec;

        (*record)[i].timestamp = (uint64_t) seconds;
        (*record)[i].data = r;
    }

    json_value_free(result);

    return 0;
}

TsdbDao *tsdb_ctor(char *domain) {
    domain = domain;//what use

    TsdbDao *tsdb = malloc(sizeof(TsdbDao));
    if (tsdb) {
        tsdb->close = close;
        tsdb->free_record = free_record;
        tsdb->free_result = free_result;
        tsdb->new_record = new_record;
        tsdb->save_record = save_record;
        tsdb->select_record = select_record;
        tsdb->update_record = update_record;
        tsdb->delete_record = delete_record;
        tsdb->delete_records = delete_records;

        //influx collection
        c_vector_create(&tsdb->influxdbContexts, NULL);
        c_vector_push_back(&tsdb->influxdbContexts, tsdb_influx_create());

        // TODO: set value in heart beat future
        tsdb->master = c_vector_at(&tsdb->influxdbContexts, 0);

        // todo:kick off heart beat
    }

    return tsdb;
}
