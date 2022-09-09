#ifndef LOCAL_DOMAIN_INFO
#define LOCAL_DOMAIN_INFO
#include "mxml/mxml.h"

typedef struct Sys_conf_domain_server sys_conf_domain_server;
struct Sys_conf_domain_server{
    sys_conf_domain_server* next;
	char* net_1_ip;
	char* net_2_ip;

	int size;
};

typedef struct Sys_conf_local_domain_worker sys_conf_local_domain_worker;
struct Sys_conf_local_domain_worker{
	int local_domain_id;
    sys_conf_domain_server* datas;

	int (*get_id)(mxml_node_t* xml);
    sys_conf_domain_server* (*get_servers)(mxml_node_t* xml);
};

int sys_conf_get_domain_id(mxml_node_t* xml);
sys_conf_domain_server* sys_conf_get_domain_servers(mxml_node_t* xml);
sys_conf_domain_server* sys_conf_get_domain_servers_by_id(mxml_node_t* xml,int id);

char* sys_conf_get_domain_id_char(mxml_node_t* xml);

typedef struct sys_domain_group sys_domain_group;
sys_domain_group* sys_conf_get_domain_group(mxml_node_t* xml);

#endif
