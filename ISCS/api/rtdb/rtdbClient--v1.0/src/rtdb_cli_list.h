/*
 * @Copyright Reserved by XXXX.
 * Double link list.
 * Create by KanWenDi, 2018.06.13.
 * Histories:
 */
#ifndef RTDB_CLI_LIST_H
#define RTDB_CLI_LIST_H
#ifdef __cplusplus
extern "C" {
#endif

#include "rtdb_cli_export.h"

typedef struct rtdb_cli_list rtdb_cli_list;

typedef void(*rtdb_cli_list_data_free)(void *val);
typedef int(*rtdb_cli_list_data_compare)(void *val, void *key); /* 0 unequal, !0 equal */

RTDB_CLI_API rtdb_cli_list* rtdb_cli_list_create(rtdb_cli_list_data_free free, rtdb_cli_list_data_compare compare);
RTDB_CLI_API void rtdb_cli_list_free(rtdb_cli_list* l);
RTDB_CLI_API void rtdb_cli_list_clear(rtdb_cli_list* l);
RTDB_CLI_API rtdb_cli_list* rtdb_cli_list_push_head(rtdb_cli_list *l, void *value);
RTDB_CLI_API rtdb_cli_list* rtdb_cli_list_push_tail(rtdb_cli_list *l, void *value);
RTDB_CLI_API void rtdb_cli_list_remove(rtdb_cli_list *l, void *value);
RTDB_CLI_API void* rtdb_cli_list_get_first(rtdb_cli_list *l);
RTDB_CLI_API void* rtdb_cli_list_get_next(rtdb_cli_list *l);
RTDB_CLI_API int rtdb_cli_list_has_next(rtdb_cli_list *l);
RTDB_CLI_API unsigned long rtdb_cli_list_get_size(rtdb_cli_list *l);

#ifdef __cplusplus
}
#endif
#endif
