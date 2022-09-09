/*
 * conf_sdk.h.
 * configuration database api.
 *
 * return values:
 * conf_init, conf_startup, conf_shutdown, conf_uninit, and conf_async_load_table
 * return 0 on ok, !0 on error.
 *
 * usages:
 *   before you can load or synchronize records from table,
 *   you should call conf_init and conf_startup to initialize and start api service,
 *   then call conf_load_table to load data, after finishing all the loading works,
 *   your should call conf_shutdown and call conf_uninit to stop api service and release resources.
 */
#ifndef CONF_SDK_H
#define CONF_SDK_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

enum
{
    CONF_VT_UNDEF = 0,
    CONF_VT_BOOL,
    CONF_VT_INT8,
    CONF_VT_INT16,
    CONF_VT_INT32,
    CONF_VT_INT64,
    CONF_VT_FLOAT,
    CONF_VT_DOUBLE,
    CONF_VT_STRING
};

typedef union _conf_variant
{
    int8_t i1;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float f32;
    double f64;
    char *s;
} conf_variant;

enum
{
    TABLE_STATUS_OK = 0,
    TABLE_STATUS_CONNBREAK
};

typedef struct _conf_table
{
    char *tableName;
    int8_t type;            // 0 -- load, 1 -- sync, 2 -- sync add, 3 -- sync update, 5 -- sync delete.
    int8_t status;          // 0 -- normal, !0 -- error.
    int fieldNum;           // number of field
    int recordNum;          // number of record
    int *fieldTypes;        // equals to "int fieldTypes[fieldNum]", CONF_VT_XXX
    char **fieldNames;      // equals to "char *fieldNames[fieldNum]"
    conf_variant **data;    // equals to "conf_variant data[recordNum][fieldNum]";
} conf_table;

/*
 * conf_load_notifier, be used to notify loading records.
 * this function is implemented and registered by api caller.
 * when api received any loading response records, conf_load_notifier
 * will be trigged to notify loading records to api caller.
 *
 */
typedef void(conf_load_notifier)(const conf_table *table);
/*
 * conf_sync_notifier.
 * this function is implemented and registered by api caller.
 * when api received any synchronizing response records, conf_sync_notifier
 * will be trigged to notify synchronizing records to api caller.
 *
 * type is add or update, records contain the whole fields data.
 * type is delete, records contain conditions to delete records. the detail of
 * conditions will be defined in future.
 */
typedef void(conf_sync_notifier)(const conf_table *table);

int conf_init(conf_load_notifier *load_notifier, conf_sync_notifier *sync_notifier);

void conf_startup();

void conf_shutdown();

void conf_uninit();

int conf_async_load_table(const char *tblname,int domain_id_flag);

#ifdef __cplusplus
}
#endif
#endif
