/*
 * @Copyright Reserved by XXXX.
 * variant definition and operations.
 * Create by KanWenDi, 2018.06.19.
 * Histories:
 */
#include "rtdb_cli_data.h"
#include "rtdb_cli_data_public.h"
#include "rtdb_cli_data_private.h"
#include "rtdb_cli_cmn.h"
#include "rtdb_cli_protocol.h"
#include <stddef.h>

extern cmn_logger* logger;



    
/***********************************************************************
 *
 * 内部数据结构定义。这些数据结构不对外开放结构体内容，仅对外开放类型定义。
 * 
 **********************************************************************/
struct _rtdb_cli_record
{
    int num; // column number
    char **names;// column names
    rtdb_cli_variant **values;	// column values.
};





/***********************************************************************
 *
 * 对外开放的方法接口实现
 * 
 **********************************************************************/
rtdb_cli_variant* rtdb_cli_variant_create()
{
    rtdb_cli_variant *vt = malloc(sizeof(*vt));

    if (vt) memset(vt, 0, sizeof(*vt));

    return vt;
}

void rtdb_cli_variant_free(rtdb_cli_variant *vt)
{
    if (!vt) return;
    if (vt->type == RTDB_CLI_VARIANT_STRING && vt->v.string)
    {
        free(vt->v.string);
    }
    free(vt);
}

rtdb_cli_variant* rtdb_cli_variant_dup(const rtdb_cli_variant* v)
{
	rtdb_cli_variant *copy;

	if (!v || !(copy = rtdb_cli_variant_create()))
	{
		return NULL;
	}

	copy->type = v->type;
	switch (copy->type)
	{
	case RTDB_CLI_VARIANT_BOOL:
	case RTDB_CLI_VARIANT_INT8:
	case RTDB_CLI_VARIANT_INT16:
	case RTDB_CLI_VARIANT_INT32:
	case RTDB_CLI_VARIANT_INT64:
		copy->v.integer = v->v.integer;
		break;
	case RTDB_CLI_VARIANT_FLOAT:
	case RTDB_CLI_VARIANT_DOUBLE:
		copy->v.real = v->v.real;
		break;
	case RTDB_CLI_VARIANT_STRING:
		copy->v.string = strdup(v->v.string);
		break;
	default:		
		free(copy);
		copy = NULL;
	}

	return copy;
}

char* rtdb_cli_variant_tostring(const rtdb_cli_variant *vt)
{
    size_t rsLen;
    char *rs = NULL;
    char buf[128] = {0};

    switch(vt->type)
    {
        case RTDB_CLI_VARIANT_BOOL :
        case RTDB_CLI_VARIANT_INT8 :
        case RTDB_CLI_VARIANT_INT16 :
        case RTDB_CLI_VARIANT_INT32 :         
        case RTDB_CLI_VARIANT_INT64 :
            sprintf(buf, "%lld", vt->v.integer);
            break;
        case RTDB_CLI_VARIANT_FLOAT :
        case RTDB_CLI_VARIANT_DOUBLE :
            sprintf(buf, "%lf", vt->v.real);
            break;
        case RTDB_CLI_VARIANT_STRING :
            rsLen = strlen(vt->v.string);
            if((rs = malloc(rsLen + 1)))
            {
                memcpy(rs, vt->v.string, rsLen);    
                rs[rsLen] = '\0';     
            }
            return rs;
        default:
            return NULL;
    }
    
    rsLen = strlen(buf);
    if((rs = malloc(rsLen + 1)))
    {
        memcpy(rs, buf, rsLen);
        rs[rsLen] = '\0';
    }
    
    return rs;        
}

void rtdb_cli_variant_freestring(char* s)
{
    if (s) free(s);
}

rtdb_cli_record* rtdb_cli_record_create(int fieldNum)
{
    rtdb_cli_record *r = malloc(sizeof(*r));

    if (r)
    {
        memset(r, 0, sizeof(*r));
        if (fieldNum > 0)
        {
            r->names = malloc(sizeof(char*)*fieldNum);
            if (!r->names)
            {
                free(r);
                return NULL;
            }
            r->values = malloc(sizeof(rtdb_cli_variant*)*fieldNum);
            if (!r->values)
            {
                free(r->names);
                free(r);
                return NULL;
            }

            r->num = fieldNum;
            memset(r->names,0,sizeof(char*)*fieldNum);
            memset(r->values,0,sizeof(rtdb_cli_variant*)*fieldNum);
        }
    }

    return r;
}

void rtdb_cli_record_free(rtdb_cli_record* r)
{
    int i;

    if (!r) return;
    if (r->names)
    {
        for (i = 0; i < r->num; i++)
        {
            if (r->names[i]) free(r->names[i]);
        }
        free(r->names);
    }
    if (r->values)
    {
        for (i = 0; i < r->num; i++)
        {
            if (r->values[i]) rtdb_cli_variant_free(r->values[i]);
        }
        free(r->values);
    }
    free(r);
}

int rtdb_cli_record_setvalue(rtdb_cli_record* r, const char* fieldName, rtdb_cli_variant* value)
{
    int i, found;

    if (!r || !fieldName || !value) return -1;

    found = 0;
    if (r->names)
    {
        for (i = 0; i < r->num; i++)
        {
            if (r->names[i] && !strcmp(r->names[i], fieldName))
            {
                found = 1;
                break;
            }
        }
    }

    if (found)
    {
        if (!r->values) return -1;
        if (r->values[i]) rtdb_cli_variant_free(r->values[i]);
        r->values[i] = value;
        return 0;
    }
    
    found = 0;
	for (i = 0; i < r->num; i++)
	{
		if (!r->names[i])
		{
			found = 1;
			break;
		}
	}
		
	if (!found)
	{
		char **names;
		rtdb_cli_variant **values;
		
		names = realloc(r->names, sizeof(char*)*(r->num+1));
		if(!names) return -1;
		names[r->num] = strdup(fieldName);
		if (!names[r->num]) return -1;
		
		values = realloc(r->values, sizeof(rtdb_cli_variant*)*(r->num+1));
		if (!values) return -1;
		values[r->num] = value;
		
		r->names = names;
		r->values = values;
		++ r->num;
		
		return 0;
	}
	
	r->names[i] = strdup(fieldName);
	if (!r->names[i]) return -1;
	r->values[i] = value;
	
	return 0;
}

const int rtdb_cli_record_getsize(const rtdb_cli_record* r)
{
    return (r==NULL) ? 0 : r->num;
}

const char* rtdb_cli_record_getfieldName(const rtdb_cli_record* r,int idx)
{
    if(!r || (idx < 0)) return NULL;
    if(idx > r->num -1) return NULL;
    if(!r->names) return NULL;
    return r->names[idx];
}


const rtdb_cli_variant* rtdb_cli_record_getvalue(const rtdb_cli_record* r, const char* fieldName)
{
	int i;
	
	if (!r || !fieldName) return NULL;
	
	for (i = 0; i < r->num; i++)
	{
		if (r->names[i] && !strcmp(r->names[i], fieldName))
		{
			return r->values[i];
		}
	}
	
	return NULL;
}

rtdb_cli_command* rtdb_cli_command_create()
{
	rtdb_cli_command *r = malloc(sizeof(*r));
	
	if (r) memset(r, 0, sizeof(*r));
	
	return r;
}

void rtdb_cli_command_free(rtdb_cli_command* r)
{
	if (!r) return;
	if (r->target) free(r->target);
	free(r);
}




/***********************************************************************
 *
 * 内部接口方法接口实现
 * 
 **********************************************************************/


 
 
