/*
 * @Copyright Reserved by XXXX.
 * structure methods used by api users.
 * Create by KanWenDi, 2018.06.19.
 * Histories:
 *
 */
#ifndef RTDB_CLI_DATA_PUBLIC_H
#define RTDB_CLI_DATA_PUBLIC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "rtdb_cli_export.h"
#include "rtdb_cli_data.h"

RTDB_CLI_API rtdb_cli_variant* rtdb_cli_variant_create();
RTDB_CLI_API void rtdb_cli_variant_free(rtdb_cli_variant *vt);
rtdb_cli_variant* rtdb_cli_variant_dup(const rtdb_cli_variant* v);

RTDB_CLI_API char* rtdb_cli_variant_tostring(const rtdb_cli_variant *vt);

RTDB_CLI_API void rtdb_cli_variant_freestring(char* s);

RTDB_CLI_API rtdb_cli_record* rtdb_cli_record_create(int fieldNum);

RTDB_CLI_API void rtdb_cli_record_free(rtdb_cli_record* r);

RTDB_CLI_API rtdb_cli_record* rtdb_cli_record_dup(const rtdb_cli_record* r);

/*
 * rtdb_cli_record_setvalue:
 * 设置字段值，如果值存在则用新值替换旧值。
 * 成功返回0， 失败返回-1。
 */
RTDB_CLI_API int rtdb_cli_record_setvalue(rtdb_cli_record* r, const char* fieldName, rtdb_cli_variant* value);

RTDB_CLI_API const int rtdb_cli_record_getsize(const rtdb_cli_record* r);

RTDB_CLI_API const char* rtdb_cli_record_getfieldName(const rtdb_cli_record* r,int idx);

RTDB_CLI_API const rtdb_cli_variant* rtdb_cli_record_getvalue(const rtdb_cli_record* r, const char* fieldName);

RTDB_CLI_API rtdb_cli_command* rtdb_cli_command_create();

RTDB_CLI_API void rtdb_cli_command_free(rtdb_cli_command* cmd);

#ifdef __cplusplus
}
#endif
#endif
