#include "conf_byte_order.h"
#include <string.h>

int16_t conf_byteorder_h2n_int16(int16_t v)
{
    char bytes[2];
    int16_t nv;

    bytes[0] = (v >> 8) & 0xff;
    bytes[1] = (v >> 0) & 0xff;
    memcpy(&nv, bytes, 2);

    return nv;
}

int16_t conf_byteorder_n2h_int16(int16_t v)
{
    int16_t hv;

    hv = (((v >> 0) & 0xff) << 8) + (((v >> 8) & 0xff) << 0);
    return hv;
}

int32_t conf_byteorder_h2n_int32(int32_t v)
{
    char bytes[4];
    int32_t nv;

    bytes[0] = (v >> 24) & 0xff;
    bytes[1] = (v >> 16) & 0xff;
    bytes[2] = (v >> 8 ) & 0xff;
    bytes[3] = (v >> 0 ) & 0xff;
    memcpy(&nv, bytes, 4);

    return nv;
}

int32_t conf_byteorder_n2h_int32(int32_t v)
{
    int32_t hv;

    hv = (((v >> 0 ) & 0xff) << 24) +
         (((v >> 8 ) & 0xff) << 16) +
         (((v >> 16) & 0xff) << 8 ) +
         (((v >> 24) & 0xff) << 0 );

    return hv;
}

int64_t conf_byteorder_h2n_int64(int64_t v)
{
    char bytes[8];
    int64_t nv;

    bytes[0] = (v >> 56) & 0xff;
    bytes[1] = (v >> 48) & 0xff;
    bytes[2] = (v >> 40) & 0xff;
    bytes[3] = (v >> 32) & 0xff;
    bytes[4] = (v >> 24) & 0xff;
    bytes[5] = (v >> 16) & 0xff;
    bytes[6] = (v >> 8 ) & 0xff;
    bytes[7] = (v >> 0 ) & 0xff;
    memcpy(&nv, bytes, 8);

    return nv;
}

int64_t conf_byteorder_n2h_int64(int64_t v)
{
    int64_t hv;

    hv = (((v >> 0 ) & 0xff) << 56) +
         (((v >> 8 ) & 0xff) << 48) +
         (((v >> 16) & 0xff) << 40) +
         (((v >> 24) & 0xff) << 32) +
         (((v >> 32) & 0xff) << 24) +
         (((v >> 40) & 0xff) << 16) +
         (((v >> 48) & 0xff) << 8 ) +
         (((v >> 56) & 0xff) << 0 );

    return hv;
}

float conf_byteorder_h2n_float(float v)
{
    float fv;
    int32_t iv;

    memcpy(&iv, &v, 4);
    iv = conf_byteorder_h2n_int32(iv);
    memcpy(&fv, &iv, 4);

    return fv;
}

float conf_byteorder_n2h_float(float v)
{
    float fv;
    int32_t iv;

    memcpy(&iv, &v, 4);
    iv = conf_byteorder_n2h_int32(iv);
    memcpy(&fv, &iv, 4);

    return fv;
}

double conf_byteorder_h2n_double(double v)
{
    double df;
    int64_t lv;

    memcpy(&lv, &v, 8);
    lv = conf_byteorder_h2n_int64(lv);
    memcpy(&df, &lv, 8);

    return df;
}

double conf_byteorder_n2h_double(double v)
{
    double df;
    int64_t lv;

    memcpy(&lv, &v, 8);
    lv = conf_byteorder_n2h_int64(lv);
    memcpy(&df, &lv, 8);

    return df;
}
