#ifndef SDK_LIBRARY_H
#define SDK_LIBRARY_H

#include <stdint.h>
#include <parson.h>

typedef struct _Record Record;
typedef struct _ResultRecord ResultRecord;

typedef enum _error {
    E_SUCCESS = 0,
    E_NULL_POINTER = -1,
    E_USER_AUTH = -2,
    E_WRITE_SLAVE = -3,
    E_OTHERS = -4,
} error_code;

/*
 * 状态数据结构
 */
typedef struct {
    uint64_t timestamp;
    Record *data;
} TsdbData;

typedef struct {
    uint64_t timestamp;
    ResultRecord *data;
} TsdbResultData;

int ResultRecordGetInt64(const ResultRecord *record, const char *field, int64_t *value);

int ResultRecordGetInt32(const ResultRecord *record, const char *field, int32_t *value);

int ResultRecordGetInt16(const ResultRecord *record, const char *field, int16_t *value);

int ResultRecordGetInt8(const ResultRecord *record, const char *field, int8_t *value);

int ResultRecordGetDouble(const ResultRecord *record, const char *field, double *value);

int ResultRecordGetString(const ResultRecord *record, const char *field, char **value);

int RecordSetInt64(const Record *record, const char *field, int64_t value, uint8_t is_tag);

int RecordSetInt32(const Record *record, const char *field, int32_t value, uint8_t is_tag);

int RecordSetInt16(const Record *record, const char *field, int16_t value, uint8_t is_tag);

int RecordSetInt8(const Record *record, const char *field, int8_t value, uint8_t is_tag);

int RecordSetDouble(const Record *record, const char *field, double value, uint8_t is_tag);

int RecordSetString(const Record *record, const char *field, const char *value, uint8_t is_tag);


#endif