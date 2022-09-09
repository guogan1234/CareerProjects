#ifndef CONF_PARSER_INNER
#define CONF_PARSER_INNER

#include "conf_parser.h"

//rtdb
struct sys_conf_rtdb_server_list{
    int size;
    sys_conf_rtdb_server* first;
};
typedef struct sys_conf_rtdb_server_list sys_conf_rtdb_server_list;

//rtdb-fes
struct sys_conf_rtdb_fes_server_list{
    int size;
    sys_conf_rtdb_fes_server* first;
};
typedef struct sys_conf_rtdb_fes_server_list sys_conf_rtdb_fes_server_list;

//mq
struct sys_conf_mq_server_list{
    int size;
    sys_conf_mq_server* first;
};
typedef struct sys_conf_mq_server_list sys_conf_mq_server_list;

//tsdb
struct sys_conf_tsdb_server_list{
    int size;
    sys_conf_tsdb_server* first;
};
typedef struct sys_conf_tsdb_server_list sys_conf_tsdb_server_list;
#endif
