#include <stdio.h>
#include "conf_sdk.h"

const char* test_convert_type(int type)
{
    switch(type)
    {
    case CONF_VT_BOOL: return "bool";
    case CONF_VT_INT8: return "byte";
    case CONF_VT_INT16: return "int16_t";
    case CONF_VT_INT32: return "int32_t";
    case CONF_VT_INT64: return "int64_t";
    case CONF_VT_FLOAT: return "float";
    case CONF_VT_DOUBLE: return "double";
    case CONF_VT_STRING: return "string";
    default: return "undef";
    }
}

void test_load_notify_table(const conf_table* table)
{
    int i, j;

    if (!table) return;

    // print table head
    printf("table:%s,\tfield_num:%d,\trecord_num:%d,\t", table->tableName, table->fieldNum, table->recordNum);
    if (!table->status)
    {
        printf("status:ok.\n");
    }
    else
    {
        printf("status: error[%d]\n",table->status);
        return;
    }

    printf("field info:\n");
    for (j = 0; j < table->fieldNum; j++)
    {
        printf("field_%d: name=%s, type=%s\n", j, table->fieldNames[j], test_convert_type(table->fieldTypes[j]));
    }

    printf("records:\n");
    for (i = 0; i < table->recordNum; i++)
    {
        printf("record[%d]:", i);
        for (j = 0; j < table->fieldNum; j++)
        {
            switch(table->fieldTypes[j])
            {
            case CONF_VT_BOOL: printf("%d\t", table->data[i][j].i1);break;
            case CONF_VT_INT8: printf("%d\t", table->data[i][j].i1);break;
            case CONF_VT_INT16: printf("%d\t", table->data[i][j].i16);break;
            case CONF_VT_INT32: printf("%d\t", table->data[i][j].i32);break;
            case CONF_VT_INT64: printf("%ld\t", table->data[i][j].i64);break;
            case CONF_VT_FLOAT: printf("%f\t", table->data[i][j].f32);break;
            case CONF_VT_DOUBLE: printf("%f\t", table->data[i][j].f64);break;
            case CONF_VT_STRING: printf("%s\t", table->data[i][j].s);break;
            }
        }
        printf("\n");
    }
}

void test_sync_notify_table(const conf_table* table)
{
    if (!table) return;

    // print table head
    printf("test_sync_notify_table ok , table:%s,\tfield_num:%d,\trecord_num:%d,\t", table->tableName, table->fieldNum, table->recordNum);
    if (!table->status)
    {
        printf("status:ok.\n");
    }
    else
    {
        printf("status: error[%d]\n",table->status);
        return;
    }
}

int main()
{
    char *tableNames[] = { 
         "conf.table_ctrl_tbl",
         "conf.unit_tbl",
         "conf.domain_tbl"
    };
    int i;
    int tableNum = sizeof(tableNames)/sizeof(char*);

    conf_init(test_load_notify_table, test_sync_notify_table);
    conf_startup();

    for (i = 0; i < tableNum; i++)
    {
        printf("%s\n",tableNames[i]);
        conf_async_load_table(tableNames[i],0);
    }

    getchar();

    return 0;
}
