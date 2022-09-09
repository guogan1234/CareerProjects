/*
 @Copyright Reserved by XXXX.
 Path definition and operations. Path is a string with colons and dots as delimiters.
 Colons are nodes separators, and dots are separators of node and attribute, and separator
 of attribute and attribute. Examples:
    "tableName"
    "tableName.fieldName"
    "domain_label"
    "domain_label.field"
    "domain_label.tableName.field"
    ...
    "domain_label:station_label:pro_system_label:device_label:ai_label"
    "domain_label:station_label:pro_system_label:device_label:ai_label.field"
    "domain_label:station_label:pro_system_label:device_label:ai_label.tableName.field"

 Create by KanWenDi, 2018.04.02.
 Histories:

 */
#ifndef RTDB_CLI_PATH_H
#define RTDB_CLI_PATH_H
#ifdef __cplusplus
extern "C" {
#endif

#include "rtdb_cli_cmn.h"

typedef struct rtdb_cli_path rtdb_cli_path;

rtdb_cli_path* rtdb_cli_path_create();
void rtdb_cli_path_free(rtdb_cli_path *path);
int8_t rtdb_cli_path_get_nodesize(const rtdb_cli_path *path);
int8_t rtdb_cli_path_get_fieldsize(const rtdb_cli_path *path);
const char* crtdb_cli_path_get_node(const rtdb_cli_path *path, int8_t index);
const char* rtdb_cli_path_get_field(const rtdb_cli_path *path, int8_t index);
int rtdb_cli_path_add_node(rtdb_cli_path *path, const char *node);
int rtdb_cli_path_add_field(rtdb_cli_path *path,const char *field);
rtdb_cli_path* rtdb_cli_path_parse(const char *spath);
unsigned char* rtdb_cli_path_dump_nodes(const rtdb_cli_path *path, int *outLen);
unsigned char* rtdb_cli_path_dump(const rtdb_cli_path *path,int *outLen,int attrFlag);
char* rtdb_cli_path_get_domain(const rtdb_cli_path *path);
char* rtdb_cli_path_get_domain2(const char *path);

#ifdef __cplusplus
}
#endif
#endif
