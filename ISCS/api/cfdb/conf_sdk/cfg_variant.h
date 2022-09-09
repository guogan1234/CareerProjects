/*
 @Copyright Reserved by XXXX.
 variant definition and operations.
 Create by KanWenDi, 2018.04.12.
 Histories:

 */
#ifndef CFG_VARIANT_H
#define CFG_VARIANT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CFG_VARIANT_UNDEF    0
#define CFG_VARIANT_BOOL     1
#define CFG_VARIANT_INT8     2
#define CFG_VARIANT_INT16    3
#define CFG_VARIANT_INT32    4
#define CFG_VARIANT_INT64    5
#define CFG_VARIANT_FLOAT    6
#define CFG_VARIANT_DOUBLE   7
#define CFG_VARIANT_STRING   8

typedef struct cfg_variant cfg_variant;

cfg_variant* cfg_variant_create();
cfg_variant* cfg_variant_create_bool(int8_t val);
cfg_variant* cfg_variant_int8(int8_t val);
cfg_variant* cfg_variant_int16(int16_t val);
cfg_variant* cfg_variant_int32(int32_t val);
cfg_variant* cfg_variant_int64(int64_t val);
cfg_variant* cfg_variant_float(float val);
cfg_variant* cfg_variant_double(double val);
cfg_variant* cfg_variant_string(const char *val);
void cfg_variant_release(cfg_variant *vt);

int8_t cfg_variant_get_type(const cfg_variant *vt);
int8_t cfg_variant_get_bool(const cfg_variant *vt);
int8_t cfg_variant_get_int8(const cfg_variant *vt);
int16_t cfg_variant_get_int16(const cfg_variant *vt);
int32_t cfg_variant_get_int32(const cfg_variant *vt);
int64_t cfg_variant_get_int64(const cfg_variant *vt);
float cfg_variant_get_float(const cfg_variant *vt);
double cfg_variant_get_double(const cfg_variant *vt);
const char* cfg_variant_get_string(const cfg_variant *vt);
void cfg_variant_set_type(cfg_variant *vt, int8_t type);
void cfg_variant_set_bool(cfg_variant *vt, int8_t val);
void cfg_variant_set_int8(cfg_variant *vt, int8_t val);
void cfg_variant_set_int16(cfg_variant *vt, int16_t val);
void cfg_variant_set_int32(cfg_variant *vt, int32_t val);
void cfg_variant_set_int64(cfg_variant *vt, int64_t val);
void cfg_variant_set_float(cfg_variant *vt, float val);
void cfg_variant_set_double(cfg_variant *vt, double val);
void cfg_variant_set_string(cfg_variant *vt, const char *val);

#ifdef __cplusplus
}
#endif
#endif
