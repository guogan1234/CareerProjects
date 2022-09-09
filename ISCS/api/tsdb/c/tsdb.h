/**************************************************************************
 * TSDB Interface
 * Version: 1.0
 * Track:
 * 1.0  20180408 Create
 **************************************************************************/


#ifndef TSDB_INTERFACE
#define TSDB_INTERFACE

#include <stddef.h>



#ifdef __cplusplus
extern "C" {
#endif

#include "tsdb_types.h"

typedef struct TsdbDao TsdbDao;

/*
* TSDB 初始化
*/
TsdbDao *tsdb_create(char *domain);

/*
* 销毁数据库访问对象
*/
int tsdb_close(TsdbDao *tsdbDao);

/*
* 创建状态数据
*/
TsdbData *tsdb_new_record(TsdbDao *tsdbDao);

/*
* 释放状态数据
*/
void tsdb_free_record(TsdbDao *tsdbDao, TsdbData *status);

void tsdb_free_result(TsdbDao *tsdbDao, TsdbResultData *status, size_t count);

/*
* 存储状态数据
* 注：必须给出全部的字段
*/
int tsdb_save_record(TsdbDao *tsdbDao, const char *table, const TsdbData **record, size_t count);

/*
* 查询状态数据
* 注：查询条件"query"包含允许只有TAGS，或只有FIELDS，或TAGS和FIELDS。
*/
int tsdb_select_record(TsdbDao *tsdbDao, const char *query, size_t *size, TsdbResultData **record);

/*
 * 更改状态数据
 * 注：
 *   1. TAGS值不允许更新
 *   2. 必须给出表的全部TAGS（TAG的值和名称）
 *   3. FIELDS只要给出需要更新的FIELDS（名称和值）
 *   4. 记录的时间设置到TsdbData.timestamp（如果timestamp不为0，则timestamp应该作为更新时查询的条件"time"）
 */
int tsdb_update_record(TsdbDao *tsdbDao, const char *table, const TsdbData **record, size_t count);

/*
 * 删除状态数据
 * 注：
 *   1. 删除记录，删除条件与查询接口的查询条件一样，即tsdb_select_record的query参数。
 *   2. 把查询出来的所有记录都删除。
 *   3. 删除记录需要增加接口: int tsdb_delete_records(TsdbDao *tsdbDao, const char *query);
 *   4. tsdb_delete_record接口仅根据"record"参数的时间戳（即TsdbData.timestamp）进行删除。
 */
int tsdb_delete_record(TsdbDao *tsdbDao, const char *table, const TsdbData **record, size_t count);


int tsdb_delete_records(TsdbDao *tsdbDao, const char *query);

#ifdef __cplusplus
}
#endif

#endif
