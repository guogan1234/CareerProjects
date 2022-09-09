#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sods/sods.h>
#include "parson.h"
#include "tsdb_types.h"

struct _Record {
    JSON_Value *value;
};

struct _ResultRecord {
    JSON_Value *value;
};

static int RecordSetStr(const Record *record, const char *field, const char *value, uint8_t is_tag, uint8_t quote);

static int shouldEscapeTag(const char *string);

static sods escapeTag(const char *string);

Record *TsdbRecordNew() {
    Record *record = malloc(sizeof(Record));
    memset(record, 0, sizeof(Record));
    record->value = json_value_init_object();
    json_object_set_value(json_object(record->value), "t", json_value_init_array());
    json_object_set_value(json_object(record->value), "qt", json_value_init_array());
    json_object_set_value(json_object(record->value), "v", json_value_init_array());

    return record;
}

void TsdbRecordFree(Record *record) {
    if (record == NULL || record->value == NULL) {
        return;
    }

    json_value_free(record->value);
    record->value = NULL;
    free(record);
}

void TsdbResultRecordFree(ResultRecord *record) {
    if (record == NULL || record->value == NULL) {
        return;
    }

    json_value_free(record->value);
    record->value = NULL;
    free(record);
}

int ResultRecordGetInt64(const ResultRecord *record, const char *field, int64_t *value) {
    if (record == NULL)
        return E_NULL_POINTER;

    int ret = (int) json_object_get_integer(json_object(record->value), field);
    *value = (int64_t) ret;

    return ret;
}

int ResultRecordGetInt32(const ResultRecord *record, const char *field, int32_t *value) {
    int64_t v;
    int ret = ResultRecordGetInt64(record, field, &v);
    *value = (int32_t) v;
    return ret;
}

int ResultRecordGetInt16(const ResultRecord *record, const char *field, int16_t *value) {
    int32_t v;
    int ret = ResultRecordGetInt32(record, field, &v);
    *value = (int16_t) v;
    return ret;
}

int ResultRecordGetInt8(const ResultRecord *record, const char *field, int8_t *value) {
    int32_t v;
    int ret = ResultRecordGetInt32(record, field, &v);
    *value = (int8_t) v;
    return ret;
}

int ResultRecordGetDouble(const ResultRecord *record, const char *field, double *value) {
    if (record == NULL)
        return E_NULL_POINTER;

    *value = json_object_get_number(json_object(record->value), field);

    return E_SUCCESS;
}

int ResultRecordGetString(const ResultRecord *record, const char *field, char **value) {
    if (record == NULL)
        return E_NULL_POINTER;

    const char *str = json_object_get_string(json_object(record->value), field);
    *value = strdup(str);

    return E_SUCCESS;
}

int RecordSetInt64(const Record *record, const char *field, int64_t value, uint8_t is_tag) {
    if (record == NULL)
        return E_NULL_POINTER;

    char s[32];
    sprintf(s, "%ldi", value);
    RecordSetStr(record, field, s, is_tag, 0);

    return E_SUCCESS;
}

int RecordSetInt32(const Record *record, const char *field, int32_t value, uint8_t is_tag) {
    if (record == NULL)
        return E_NULL_POINTER;

    char s[16];
    sprintf(s, "%di", value);
    RecordSetStr(record, field, s, is_tag, 0);

    return E_SUCCESS;
}

int RecordSetInt16(const Record *record, const char *field, int16_t value, uint8_t is_tag) {
    return RecordSetInt32(record, field, value, is_tag);
}

int RecordSetInt8(const Record *record, const char *field, int8_t value, uint8_t is_tag) {
    return RecordSetInt32(record, field, value, is_tag);
}

int RecordSetDouble(const Record *record, const char *field, double value, uint8_t is_tag) {
    if (record == NULL)
        return E_NULL_POINTER;

    char s[32];
    sprintf(s, "%f", value);
    RecordSetStr(record, field, s, is_tag, 0);

    return E_SUCCESS;
}

int RecordSetString(const Record *record, const char *field, const char *value, uint8_t is_tag) {
    return RecordSetStr(record, field, value, is_tag, 1);
}

static int RecordSetStr(const Record *record, const char *field, const char *value, uint8_t is_tag, uint8_t quote) {
    if (record == NULL)
        return E_NULL_POINTER;

    const char *tag = is_tag ? "t" : "v";
    JSON_Array *array = json_object_get_array(json_object(record->value), tag);
    char *buf = malloc(strlen(field) + strlen(value) + 8);

	// KWD
	// 这部分实现是否可以优化一下，例如逻辑精简。转义字符的检查与转移优化
    if (is_tag) {
        if (quote == 1) {
            if (shouldEscapeTag(value) == 0) {
                sprintf(buf, "%s=%s", field, value);
            } else {
                sods s = escapeTag(value);
                sprintf(buf, "%s=%s", field, s);
                sodsfree(s);
            }
        } else {
            sprintf(buf, "%s=%s", field, value);
        }
    } else {
        if (quote == 1) {
            sprintf(buf, "%s=\"%s\"", field, value);
        } else {
            sprintf(buf, "%s=%s", field, value);
        }
    }

    json_array_append_string(array, buf);
    if (is_tag) {
        array = json_object_get_array(json_object(record->value), "qt");
        if (quote == 1) {
            sprintf(buf, "%s='%s'", field, value);
        }
        json_array_append_string(array, buf);
    }
    free(buf);

    return E_SUCCESS;
}

sods RecordToString(const char *table, const Record *record) {
    sods s = sodsnew(table);

    JSON_Array *tag = json_object_get_array(json_object(record->value), "t");
    size_t count = json_array_get_count(tag);
    for (size_t i = 0; i < count; i++) {
        const char *pair = json_array_get_string(tag, i);
        s = sodscatprintf(s, ",%s", pair);
    }
    s = sodscat(s, " ");

    JSON_Array *v = json_object_get_array(json_object(record->value), "v");
    size_t v_count = json_array_get_count(v);
    for (size_t i = 0; i < v_count; i++) {
        const char *pair = json_array_get_string(v, i);
        s = sodscatprintf(s, "%s,", pair);
    }
    s[sodslen(s) - 1] = '\0';
    sodsupdatelen(s);

    return s;
}

ResultRecord *JsonValueToResultRecord(JSON_Value *value) {
    ResultRecord *record = malloc(sizeof(ResultRecord));
    memset(record, 0, sizeof(*record));
    record->value = value;
    return record;
}

char * TsdbDataToQuery(const TsdbData *data) {
    if (data == NULL || data->data == NULL || data->data->value == NULL)
        return NULL;

    JSON_Object *obj = json_object(data->data->value);
    if (obj == NULL)
        return NULL;

    JSON_Array *tag = json_object_get_array(obj, "qt");
    sods s = sodsempty();
    size_t count = json_array_get_count(tag);
    for (size_t i = 0; i < count; i++) {
        const char *pair = json_array_get_string(tag, i);
        s = sodscatprintf(s, "%s", pair);
        if (i + 1 != count) {
            s = sodscat(s, " AND ");
        }
    }

    if (data->timestamp != 0) {
        s = sodscatfmt(s, " AND time=%U", data->timestamp);
    }

    const char *buf = strdup(s);
    sodsfree(s);

    return buf;
}

static void ReverseJsonStringArray(JSON_Array *val) {
	// KWD
	// 在把Tags和Fields都添加到对于的记录里面, 这里的是否可以不用在执行反转操作
	//
    size_t count = json_array_get_count(val);
    for (size_t i = 0, j = count - 1; i < j; i++, j--) {
        char *str_i = strdup(json_array_get_string(val, i));
        char *str_j = strdup(json_array_get_string(val, j));
        json_array_replace_string(val, i, str_j);
        json_array_replace_string(val, j, str_i);
        free(str_i);
        free(str_j);
    }
}

void CopyRecordToTsdbData(const TsdbResultData *result, TsdbData *data) {
    if (data == NULL || data->data == NULL || data->data->value == NULL)
        return;

    if (result == NULL || result->data == NULL || result->data->value == NULL)
        return;

    JSON_Object *obj = json_object(result->data->value);
    if (obj == NULL)
        return;

    JSON_Object *dobj = json_object(data->data->value);
    if (dobj == NULL)
        return;

    data->timestamp = result->timestamp;

    JSON_Array *tag = json_object_get_array(dobj, "t");
    size_t dCount = json_array_get_count(tag);
    ReverseJsonStringArray(json_object_get_array(dobj, "v"));

	// KWD
	// 约定好所有的插入和更新操作，必须传入全部的Tags，这样在插入或更新记录时就能明确区分Tag与Field。
	//
	// select_record查询的结果记录包含表中所有“值非空”的字段（包括Tags和Fields）。
	// 为了防止调用接口时设置了部分Tag作为查询条件，在这里更新目标记录时，需要把结果记录
	// 中的所有Tags和Fields更新到目标记录的Tags和Feilds。其中：
	// Tags更新：把结果记录中有但目标记录中没有的Tags更新到目标记录的Tags中。
	// Fields更新新：把结果记录中有但目标记录中没有的Fields更新到目标记录的Fields中。
    size_t count = json_object_get_count(obj);
    for (size_t i = 0; i < count; i++) {
        const char *name = json_object_get_name(obj, i);
        if (strcmp(name, "time") == 0)
            continue;
        int found = 0;
        for (size_t j = 0; j < dCount; j++) {
            const char *v = json_array_get_string(tag, j);
            if (strncmp(name, v, strlen(name)) == 0) {
                found = 1;
                break;
            }
        }
        if (found)
            continue;

        JSON_Value *v = json_object_get_value(obj, name);
        JSON_Value_Type t = json_value_get_type(v);
        if (t == JSONString) {
            RecordSetString(data->data, name, json_object_get_string(obj, name), 0);

        } else if (t == JSONNumber) {
            RecordSetDouble(data->data, name, json_object_get_number(obj, name), 0);
        }
    }

    ReverseJsonStringArray(json_object_get_array(dobj, "v"));
}

static int shouldEscapeTag(const char *string) {
    char ch;
    while ((ch = *string++)) {
        switch (ch) {
            case ',':
            case '=':
            case ' ':
                return 1;
            default:
                break;
        }
    }

    return 0;
}

static sods escapeTag(const char *string) {
    sods s = sodsempty();
    char ch;
    while ((ch = *string++)) {
        switch (ch) {
            case ',':
            case '=':
            case ' ':
                s = sodscatlen(s, "\\", 1);
            default:
                s = sodscatlen(s, &ch, 1);
                break;
        }
    }

    return s;
}
