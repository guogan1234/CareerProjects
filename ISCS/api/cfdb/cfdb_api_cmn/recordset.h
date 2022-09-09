#ifndef _RECORDSET_H
#define _RECORDSET_H

#include "parson.h"

//@Deprecated
struct Column {
    char* columnName;
    int   columnIndex;
    int   variantType;
};

//@Deprecated
struct Row {
    char**   values;
};
////////////////////////////////


struct RecordSet {
    JSON_Value* rsRoot;

    JSON_Array* dataList; //ref

    char** columns;
    int columnSize;

    struct Page* page;

    int code;
    char* message;
};

struct RecordSet* confdb_recordset_create(const char* entities);







#endif //_RECORDSET_H
