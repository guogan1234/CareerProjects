#include "conf_message.h"
#include "conf_byte_order.h"
#include "conf_priv_data.h"
#include <pxthread/px_time.h>
#include <stdlib.h>
#include <string.h>

static int conf_convert_type(int8_t dbType)
{
    switch (dbType)
    {
    case -6:
    case 1:
        return CONF_VT_INT8;
    case 4:
        return CONF_VT_INT32;
    case -5:
    case 93:
        return CONF_VT_INT64;
    case 6:
    case 7:
        return CONF_VT_FLOAT;
    case 8:
        return CONF_VT_DOUBLE;
    case -7:
    case 16:
        return CONF_VT_BOOL;
    case 12:
        return CONF_VT_STRING;
    case 5:
        return CONF_VT_INT16;
    default:
        return CONF_VT_UNDEF;
    }
}

char* conf_message_generate_load_request(const char *tblName,int domain_id_flag,int *reqLen)
{
    int strLen;
    int8_t i8, tblNameLen;
    int32_t i32;
    int64_t i64;
    
    char *msg;

    if (!tblName || !reqLen) return NULL;

    tblNameLen = (int8_t)strlen(tblName);
    *reqLen = 20 + tblNameLen;

    msg = malloc(*reqLen+1);
    if (msg)
    {
        memset(msg, 0, *reqLen+1);
    }
    else
    {
        return NULL;
    }

    i32 = conf_byteorder_h2n_int32(*reqLen);
    memcpy(msg, &i32, 4);
    strLen = 4;

    i32 = -1;
    if(domain_id_flag) i32 = g_confMgr.domainId;
    //printf("real dominId : %d\n",i32);
    i32 = conf_byteorder_h2n_int32(i32);
    memcpy(msg+strLen, &i32, 4);
    strLen += 4;

    i8 = 0;
    memcpy(msg+strLen, &i8, 1);
    strLen += 1;

    i64 = conf_byteorder_h2n_int64(px_time_ms());
    memcpy(msg+strLen, &i64, 8);
    strLen += 8;

    memcpy(msg+strLen, &tblNameLen, 1);
    strLen += 1;

    memcpy(msg+strLen, tblName, tblNameLen);

    // no crc code
    return msg;
}

conf_table* conf_message_parse_response(const char *msg, int msgLen)
{
    int16_t ridx, recordNum, v16;
    int8_t fidx, responseType, tblNameLen, fieldNum, fieldType, fieldNameLen, v8;
    int32_t index, v32;
    int64_t v64;
    float vf;
    double vd;
    char *tblName;
    conf_table *table;

    ((void)msgLen);

    // record number(int16_t 2bytes)
    memcpy(&recordNum, msg, 2);
    recordNum = conf_byteorder_n2h_int16(recordNum);
    index = 2;

    // response(operation) type (int8_t 1byte)
    responseType = *(msg+index);
    index += 1;

    // table name len (int8_t 1byte)
    tblNameLen = *(msg+index);
    index += 1;

    // table name (string tblNameLen bytes)
    tblName = malloc(tblNameLen+1);
    if (!tblName) return NULL;
    memcpy(tblName, msg+index, tblNameLen);
    tblName[tblNameLen] = '\0';
    index += tblNameLen;

    if (recordNum < 0)
    {
        table = conf_create_table(tblName, 0, 0, recordNum, responseType);
        if (!table) free(tblName);
        return table;
    }
    else if (recordNum == 0)
    {
        table = conf_create_table(tblName, 0, 0, 0, responseType);
        if (!table) free(tblName);
        return table;
    }

    // field number (int8_t 1byte)
    fieldNum = *(msg+index);
    index += 1;

    table = conf_create_table(tblName, fieldNum, recordNum, 0, responseType);
    if (!table)
    {
        free(tblName);
        return NULL;
    }

    // field name and type.
    // (field name length 1byte, field name nbytes, field type 1byte),...
    for (fidx = 0; fidx < fieldNum; fidx++)
    {
        fieldNameLen = *(msg+index);
        index += 1;

        table->fieldNames[fidx] = malloc(fieldNameLen+1);
        if (!table->fieldNames[fidx])
        {
            conf_free_table(table);
            return NULL;
        }
        memcpy(table->fieldNames[fidx], msg+index, fieldNameLen);
        table->fieldNames[fidx][fieldNameLen] = '\0';
        index += fieldNameLen;

        fieldType = *(msg+index);
        index += 1;
        table->fieldTypes[fidx] = conf_convert_type(fieldType);
    }

    // records
    for (ridx = 0; ridx < recordNum; ridx++)
    {
        for (fidx = 0; fidx < fieldNum; fidx++)
        {
            switch(table->fieldTypes[fidx])
            {
            case CONF_VT_BOOL:
            case CONF_VT_INT8:
                v8 = *(msg+index);
                index += 1;
                table->data[ridx][fidx].i1 = v8;
                break;
            case CONF_VT_INT16:
                memcpy(&v16, msg+index, 2);
                index += 2;
                table->data[ridx][fidx].i16 = conf_byteorder_n2h_int16(v16);
                break;
            case CONF_VT_INT32:
                memcpy(&v32, msg+index, 4);
                index += 4;
                table->data[ridx][fidx].i32 = conf_byteorder_n2h_int32(v32);
                break;
            case CONF_VT_INT64:
                memcpy(&v64, msg+index, 8);
                index += 8;
                table->data[ridx][fidx].i64 = conf_byteorder_n2h_int64(v64);
                break;
            case CONF_VT_FLOAT:
                memcpy(&vf, msg+index, 4);
                index += 4;
                table->data[ridx][fidx].f32 = conf_byteorder_n2h_float(vf);
                break;
            case CONF_VT_DOUBLE:
                memcpy(&vd, msg+index, 8);
                index += 8;
                table->data[ridx][fidx].f64 = conf_byteorder_n2h_double(vd);
                break;
            case CONF_VT_STRING:
                memcpy(&v32, msg+index, 4);
                index += 4;
                v32 = conf_byteorder_n2h_int32(v32);
                table->data[ridx][fidx].s = malloc(v32+1);
                if (!table->data[ridx][fidx].s)
                {
                    conf_free_table(table);
                    return NULL;
                }
                memcpy(table->data[ridx][fidx].s, msg+index, v32);
                table->data[ridx][fidx].s[v32] = '\0';
                index += v32;
                break;
            default:
                break;
            }
        }
    }

    return table;
}
