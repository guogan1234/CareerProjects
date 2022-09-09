/**
 * @file rtdbcli_privdata.h
 * @author
 * @date 2019.0321
 * @version v1.0
 * @note 客户端接口文件。本文件定义客户用到的内部数据和数据操作接口。
 **/
#ifndef RTDBCLI_PRIVDATA_H
#define RTDBCLI_PRIVDATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
    int32_t             hid_client;
    int32_t             type_client; 
    int32_t             hid_net;
} _rtdbcli_client, *_prtdbcli_client;

struct _rtdbcli_subscriber
{};

#ifdef __cplusplus
}

#endif
#endif
