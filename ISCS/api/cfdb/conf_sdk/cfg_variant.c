/*
 @Copyright Reserved by XXXX.
 variant definition and operations.
 Create by KanWenDi, 2018.04.12.
 Histories:

 */
#include "cfg_variant.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct cfg_variant
{
    int8_t type;
    union
    {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        float flt;
        double dbl;
        char *str;
    } v;
};

cfg_variant* cfg_variant_create()
{
    cfg_variant *vt = malloc(sizeof(*vt));
    if (vt)
    {
        memset(vt, 0, sizeof(*vt));
    }

    return vt;
}

cfg_variant* cfg_variant_create_bool(int8_t val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_BOOL;
        vt->v.i8 = 0x01 & val;
    }

    return vt;
}

cfg_variant* cfg_variant_int8(int8_t val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_INT8;
        vt->v.i8 = val;
    }

    return vt;
}

cfg_variant* cfg_variant_int16(int16_t val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_INT16;
        vt->v.i16 = val;
    }

    return vt;
}

cfg_variant* cfg_variant_int32(int32_t val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_INT32;
        vt->v.i32 = val;
    }

    return vt;
}

cfg_variant* cfg_variant_int64(int64_t val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_INT64;
        vt->v.i64 = val;
    }

    return vt;
}

cfg_variant* cfg_variant_float(float val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_FLOAT;
        vt->v.flt = val;
    }

    return vt;
}

cfg_variant* cfg_variant_double(double val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_DOUBLE;
        vt->v.dbl = val;
    }

    return vt;
}

cfg_variant* cfg_variant_string(const char *val)
{
    cfg_variant *vt = cfg_variant_create();
    if (vt)
    {
        vt->type =CFG_VARIANT_STRING;
        vt->v.str = val ? strdup(val) : NULL;
    }

    return vt;
}

void cfg_variant_release(cfg_variant *vt)
{
    if (vt)
    {
        if (vt->type == CFG_VARIANT_STRING && vt->v.str)
        {
            free(vt->v.str);
        }
        free(vt);
    }
}

int8_t cfg_variant_get_type(const cfg_variant *vt)
{
    return (vt ? vt->type : CFG_VARIANT_UNDEF);
}

int8_t cfg_variant_get_bool(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_BOOL)
    {
        return vt->v.i8;
    }
    else
    {
        return 0;
    }
}

int8_t cfg_variant_get_int8(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_INT8)
    {
        return vt->v.i8;
    }
    else
    {
        return 0;
    }
}

int16_t cfg_variant_get_int16(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_INT16)
    {
        return vt->v.i16;
    }
    else
    {
        return 0;
    }
}

int32_t cfg_variant_get_int32(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_INT32)
    {
        return vt->v.i32;
    }
    else
    {
        return 0;
    }
}

int64_t cfg_variant_get_int64(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_INT64)
    {
        return vt->v.i64;
    }
    else
    {
        return 0;
    }
}

float cfg_variant_get_float(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_FLOAT)
    {
        return vt->v.flt;
    }
    else
    {
        return 0.0;
    }
}

double cfg_variant_get_double(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_DOUBLE)
    {
        return vt->v.flt;
    }
    else
    {
        return 0.0;
    }
}

const char* cfg_variant_get_string(const cfg_variant *vt)
{
    if (vt && vt->type == CFG_VARIANT_STRING)
    {
        return vt->v.str;
    }
    else
    {
        return NULL;
    }
}

void cfg_variant_set_type(cfg_variant *vt, int8_t type)
{
    if (vt)
    {
        vt->type = type;
    }
}

void cfg_variant_set_bool(cfg_variant *vt, int8_t val)
{
    if (vt && vt->type == CFG_VARIANT_BOOL)
    {
        vt->v.i8 = val;
    }
}

void cfg_variant_set_int8(cfg_variant *vt, int8_t val)
{
    if (vt && vt->type == CFG_VARIANT_INT8)
    {
        vt->v.i8 = val;
    }
}

void cfg_variant_set_int16(cfg_variant *vt, int16_t val)
{
    if (vt && vt->type == CFG_VARIANT_INT16)
    {
        vt->v.i16 = val;
    }
}

void cfg_variant_set_int32(cfg_variant *vt, int32_t val)
{
    if (vt && vt->type == CFG_VARIANT_INT32)
    {
        vt->v.i32 = val;
    }
}

void cfg_variant_set_int64(cfg_variant *vt, int64_t val)
{
    if (vt && vt->type == CFG_VARIANT_INT64)
    {
        vt->v.i64 = val;
    }
}

void cfg_variant_set_float(cfg_variant *vt, float val)
{
    if (vt && vt->type == CFG_VARIANT_FLOAT)
    {
        vt->v.flt = val;
    }
}

void cfg_variant_set_double(cfg_variant *vt, double val)
{
    if (vt && vt->type == CFG_VARIANT_DOUBLE)
    {
        vt->v.dbl = val;
    }
}

void cfg_variant_set_string(cfg_variant *vt, const char *val)
{
    if (vt && vt->type == CFG_VARIANT_STRING)
    {
        if (vt->v.str)
        {
            free(vt->v.str);
        }
        vt->v.str = val ? strdup(val) : NULL;
    }
}
