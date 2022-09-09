#ifndef CONF_MESSAGE_H
#define CONF_MESSAGE_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "conf_sdk.h"
#include <stdint.h>

typedef struct _conf_message_response_head
{
    int32_t msgLen;
    int16_t recordNum; // 0 -- no record; < 0 -- error code; > 0 record number.
    int8_t respType;   // 0 -- loading; 1 -- synchronizing; 2 -- synchronizing add;
                       // 3 -- synchronizing update; 5 -- synchronizing delete.
    int8_t tblNameLen;
    char *tblName;
} conf_message_response_head;

/* conf_message_generate_load_request used to generate request message,
 * reqLen output the request message length.
 * return request message on ok, null on error.
 */
char* conf_message_generate_load_request(const char *tblName,int domain_id_flag,int *reqLen);

conf_table* conf_message_parse_response(const char *msg, int msgLen);

#ifdef __cplusplus
}
#endif
#endif
