#include <string.h>
#include <stdlib.h>

#include "parson.h"

#include "recordset.h"
#include "confcmn.h"

static int recrodset_json_parse(struct RecordSet* rs, const char* jstring);

struct RecordSet* confdb_recordset_create(const char* entities) {
    if (!entities) return NULL;

    struct RecordSet* rs = (struct RecordSet*)malloc(sizeof(struct RecordSet));
    memset(rs, 0x00, sizeof(struct RecordSet));

    recrodset_json_parse(rs, entities);

    return rs;
}

void confdb_recordset_free(struct RecordSet* rs){
    int i;
    if(rs){
        if(rs->rsRoot) {
            json_value_free(rs->rsRoot);
        }
        if(rs->page) free(rs->page);
        if(rs->columns) {
            for(i = 0; i < rs->columnSize; i++)
                free(rs->columns[i]);
            free(rs->columns);
        }

        free(rs);
    }
}

const char* confdb_recordset_get(struct RecordSet* rs, const int row, const char* colName) {
    if(!rs || row < 0 || !colName) return NULL;

    if(row >= json_array_get_count(rs->dataList)) return NULL;

    int colIndex = -1;
    int i;
    for(i = 0; i < rs->columnSize; i++) {
        if(strcasecmp(rs->columns[i], colName) == 0){
            colIndex = i; //get it
        }
    }
    if(colIndex >= 0) {
        JSON_Array* jsonRow = json_array_get_array(rs->dataList, row);
        if(jsonRow) {
            return json_array_get_string(jsonRow, colIndex);
        }
    }

    return NULL;
}

const char** confdb_recordset_multi_get(ConfDbRecordSet* rs, const int row, const char* colNames[], int size) {
    if(!rs || !colNames || size <=0) return NULL;

    if(row >= json_array_get_count(rs->dataList)) return NULL;

    const char** values = malloc(sizeof(char *) * size);
    int i;
    for(i = 0; i < size; i++) {
        values[i] = confdb_recordset_get(rs, row, colNames[i]);
    }
    return values;
}

int confdb_recordset_multi_free(ConfDbRecordSet* rs, char** values) {
    if(values) free(values);

    return 0;
}

struct Page* confdb_recordset_get_page(struct RecordSet* rs) {
    if(rs) return rs->page;

    return NULL;
}

int confdb_recordset_get_count(struct RecordSet* rs) {
    if(rs && rs->dataList)
        return json_array_get_count(rs->dataList);

    return 0;
}

int confdb_recordset_get_columns(struct RecordSet* rs, char*** columns, int* size) {
    if(!rs || !columns || !size) return -1;

    *columns = rs->columns;
    *size = rs->columnSize;

    return 0;
}

int confdb_recordset_get_code(ConfDbRecordSet* rs){
    if(!rs) return -1;

    return rs->code;
}

static int recrodset_json_parse(struct RecordSet* rs, const char* jstring) {
    rs->rsRoot = json_parse_string(jstring);
    if(rs->rsRoot) {
        // columns, datas
        JSON_Value* result = json_object_get_value(json_object(rs->rsRoot), "result");
        long long code = json_object_get_integer(json_object(rs->rsRoot),"code");
        rs->code = code;
        rs->message = json_object_get_string(json_object(rs->rsRoot),"message");
        int i;
        if(result) {
            JSON_Array* columnList = json_object_get_array(json_object(result), "columnList");
            if(columnList) {
                rs->columnSize = json_array_get_count(columnList);
                rs->columns = malloc(sizeof(*rs->columns) * rs->columnSize);
                for(i = 0; i < json_array_get_count(columnList); i++) {
                    rs->columns[i] = strdup(json_array_get_string(columnList, i));
                }
            }
            rs->dataList = json_object_get_array(json_object(result), "dataList");

            JSON_Value* page = json_object_get_value(json_object(result), "page");
            if(page) {
                rs->page = malloc(sizeof(struct Page));
                rs->page->number = json_object_get_number(json_object(page), "number");
                rs->page->size = json_object_get_number(json_object(page), "size");
                rs->page->totalPages = json_object_get_number(json_object(page), "totalPages");
                rs->page->hasPrevious = json_object_get_number(json_object(page), "hasPrevious");
                rs->page->hasNext = json_object_get_number(json_object(page), "hasNext");
                rs->page->totalElements = json_object_get_number(json_object(page), "totalElements");
            }

            return 0;
        }
    }

    return -1;
}

