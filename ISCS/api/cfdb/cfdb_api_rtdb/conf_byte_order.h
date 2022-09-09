#ifndef CONF_BYTEORDER_H
#define CONF_BYTEORDER_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

int16_t conf_byteorder_h2n_int16(int16_t v);

int16_t conf_byteorder_n2h_int16(int16_t v);

int32_t conf_byteorder_h2n_int32(int32_t v);

int32_t conf_byteorder_n2h_int32(int32_t v);

int64_t conf_byteorder_h2n_int64(int64_t v);

int64_t conf_byteorder_n2h_int64(int64_t v);

float conf_byteorder_h2n_float(float v);

float conf_byteorder_n2h_float(float v);

double conf_byteorder_h2n_double(double v);

double conf_byteorder_n2h_double(double v);

#ifdef __cplusplus
}
#endif
#endif
