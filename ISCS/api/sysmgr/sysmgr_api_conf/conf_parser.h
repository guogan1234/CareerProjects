#ifndef CONF_MANAGER
#define CONF_MANAGER

#ifdef __cplusplus
extern "C" {
#endif

//日志配置信息
struct sys_conf_log{
	char* file;
	int level;
	int max_size;
	int roll_num;
};
typedef struct sys_conf_log sys_conf_log;

typedef struct sys_domain_group {
	char **lables;
	int *ids;
	int num;
} sys_domain_group;
sys_domain_group* sys_conf_create_domain_group(int num);
void sys_conf_free_domain_group(sys_domain_group *domainGroup);
sys_domain_group* sys_conf_local_get_domain_group();

//RTDB
//实时库配置信息
typedef struct sys_conf_rtdb_server sys_conf_rtdb_server;
struct sys_conf_rtdb_server{
    sys_conf_rtdb_server* next;
	int basePort;
	int modePort;
	char* username;
	char* password;
	char* ip1;
	char* ip2;
};

//实时库域配置信息
struct sys_conf_rtdb_domain{
    sys_conf_rtdb_server* servers;
    sys_conf_log* log;
};
typedef struct sys_conf_rtdb_domain sys_conf_rtdb_domain;

//RTDB-FES
//实时库FES配置信息
typedef struct sys_conf_rtdb_fes_server sys_conf_rtdb_fes_server;
struct sys_conf_rtdb_fes_server{
    sys_conf_rtdb_fes_server* next;
    int port;
    char* username;
    char* password;

    char* ip1;
    char* ip2;
};

//实时库FES域配置信息
struct sys_conf_rtdb_fes_domain{
    sys_conf_rtdb_fes_server* first;
    sys_conf_log* log;
};
typedef struct sys_conf_rtdb_fes_domain sys_conf_rtdb_fes_domain;

//MQ
//中间件配置信息
typedef struct sys_conf_mq_server sys_conf_mq_server;
struct sys_conf_mq_server{
    sys_conf_mq_server* next;
	
	char* schema;
	char* username;
	char* password;
	int pool_size;
	int port;
	int timeout;
	int beat_internal;

	char* ip_1;
	char* ip_2;
};

typedef struct mq_param_key_value mq_param_key_value;
struct mq_param_key_value
{
    char* strkey;
    char* strvalue;
    mq_param_key_value* next;
};

//中间件域配置信息
struct sys_conf_mq_domain{
    sys_conf_mq_server* first;
    sys_conf_log* log;
    mq_param_key_value* params;
};
typedef struct sys_conf_mq_domain sys_conf_mq_domain;

//TSDB
//历史库配置信息
typedef struct sys_conf_tsdb_server sys_conf_tsdb_server;
struct sys_conf_tsdb_server{
    sys_conf_tsdb_server* next;

	char* schema;
	char* username;
	char* password;
	char* database;
	int port;
	int timeout;
	int beat_internal;

	char* ip_1;
	char* ip_2;
};

//历史库域配置信息
struct sys_conf_tsdb_domain{
    sys_conf_tsdb_server* first;
    sys_conf_log* log;
};
typedef struct sys_conf_tsdb_domain sys_conf_tsdb_domain;

//CONFDB
//配置库配置信息
typedef struct sys_conf_confdb_server{
    char* http_url; //confdb cmn sql

    // rtdb sync info
    char* rtdb_sync_ip;
    int rtdb_sync_port;
} sys_conf_confdb_server;

//配置库域配置信息
struct sys_conf_confdb_domain{
    int domain_id; //reserved
    char* domain_name;//reserved

    sys_conf_confdb_server* first;
    sys_conf_log* log;
};
typedef struct sys_conf_confdb_domain sys_conf_confdb_domain;

//SYS
//系统域配置信息
typedef struct sys_conf_system_domain sys_conf_system_domain;
struct sys_conf_system_domain{
    sys_conf_system_domain* next;

    int id;

    char* name;
    char* name_ch;
    int domain_type;
};

//系统节点配置信息
typedef struct sys_conf_system_node sys_conf_system_node;
struct sys_conf_system_node{
    sys_conf_system_node* next;

    int id;
    int domain_id;

    int type;
    char* name;
    char* name_ch;
    int status;
    int nic_num;
    char* nic_1_name;
    char* nic_1_netmask;
    char* nic_1_address;
    char* nic_2_name;
    char* nic_2_netmask;
    char* nic_2_address;
    int net_status;
    char* dev;
    char* os;
    char* location;
    int domain_info;    
};

//系统进程配置信息
typedef struct sys_conf_system_process sys_conf_system_process;
struct sys_conf_system_process{
    sys_conf_system_process* next;

    int id;

    char* alias;
    char* command;
    int instance_num;
    int start_type;
    int autorun;
    int cycle_start;
    int cycle_period;
    int offline_run;
    int duty_run;
    int standby_run;
    int run_order;
    int report_type;
    char* file_path;
    char* para;
    char* description;
    int run_style;
    int proc_type;
};

//系统域进程配置信息
typedef struct sys_conf_system_domain_process sys_conf_system_domain_process;
struct sys_conf_system_domain_process{
    sys_conf_system_domain_process* next;

    char* process_sub;

    int domain_id;
};

//系统管理域配置信息
typedef struct sys_conf_system_manager_domain sys_conf_system_manager_domain;
struct sys_conf_system_manager_domain{
    sys_conf_system_domain* domain_list_first;
    sys_conf_system_node* node_list_first;
    sys_conf_system_process* process_list_first;
    sys_conf_system_domain_process* domain_proc_list_first;
};

//API
/*
 * 功能：获取本域的实时库配置信息
 * 说明：获取本域的实时库配置信息
 * 参数：无
 * 返回值：实时库域配置信息
 */
sys_conf_rtdb_domain* sys_conf_get_rtdb_domain();

/*
 * 功能：释放获取的实时库域信息
 * 说明：释放获取的实时库域信息的内存占用
 * 参数：实时库域配置信息
 * 返回值：0，成功；否则，失败并返回错误码(暂无)
 */
int sys_conf_free_rtdb_domain(sys_conf_rtdb_domain* data);

/*
 * 功能：获取本域的实时库FES配置信息
 * 说明：获取本域的实时库FES配置信息
 * 参数：无
 * 返回值：实时库FES本域配置信息
 */
sys_conf_rtdb_fes_domain* sys_conf_get_rtdb_fes_domain();

/*
 * 功能：释放获取的实时库FES域信息
 * 说明：释放获取的实时库FES域信息的内存占用
 * 参数：实时库FES域配置信息
 * 返回值：0，成功；否则，失败并返回错误码(暂无)
 */
int sys_conf_free_rtdb_fes_domain(sys_conf_rtdb_fes_domain* data);

/*
 * 功能：获取本域的中间件配置信息
 * 说明：获取本域的中间件配置信息
 * 参数：无
 * 返回值：中间件域配置信息
 */
sys_conf_mq_domain* sys_conf_get_mq_domain();

/*
 * 功能：获取指定域的中间件配置信息
 * 说明：根据域编号获取指定域的中间件信息
 * 参数：域编号
 * 返回值：中间件的域配置信息
 */
sys_conf_mq_domain* sys_conf_get_mq_domain_by_id(int domain_id);

/*
 * 功能：释放获取的中间件域信息
 * 说明：释放获取的中间件域信息的内存占用
 * 参数：中间件域配置信息
 * 返回值：0，成功；否则，失败并返回错误码(暂无)
 */
int sys_conf_free_mq_domain(sys_conf_mq_domain* data);

/*
 * 功能：获取本域的历史库配置信息
 * 说明：获取本域的历史库配置信息
 * 参数：无
 * 返回值：历史库域配置信息
 */
sys_conf_tsdb_domain* sys_conf_get_tsdb_domain();

/*
 * 功能：获取指定域的历史库配置信息
 * 说明：根据域编号获取指定域的历史库信息
 * 参数：域编号
 * 返回值：历史库的域配置信息
 */
sys_conf_tsdb_domain* sys_conf_get_tsdb_domain_by_id(int domain_id);

/*
 * 功能：释放获取的历史库域信息
 * 说明：释放获取的历史库域信息的内存占用
 * 参数：历史库域配置信息
 * 返回值：0，成功；否则，失败并返回错误码(暂无)
 */
int sys_conf_free_tsdb_domain(sys_conf_tsdb_domain* data);

/*
 * 功能：获取本域的配置库配置信息
 * 说明：获取本域的配置库配置信息
 * 参数：无
 * 返回值：配置库域配置信息
 */
sys_conf_confdb_domain* sys_conf_get_confdb_domain();

/*
 * 功能：释放获取的配置库域信息
 * 说明：释放获取的配置库域信息的内存占用
 * 参数：配置库域配置信息
 * 返回值：0，成功；否则，失败并返回错误码(暂无)
 */
int sys_conf_free_confdb_domain(sys_conf_confdb_domain* data);

/*
 * 功能：获取本域的系统管理配置信息
 * 说明：获取本域的系统管理配置信息
 * 参数：无
 * 返回值：系统管理域配置信息
 */
sys_conf_system_manager_domain* sys_conf_get_system_manager_domain();

/*
 * 功能：释放获取的系统配置域信息
 * 说明：释放获取的系统配置域信息的内存占用
 * 参数：系统管理域配置信息
 * 返回值：0，成功；否则，失败并返回错误码(暂无)
 */
int sys_conf_free_system_manager_domain(sys_conf_system_manager_domain* data);

int sys_conf_get_local_domain_id(int* domain_id);

char* sys_conf_get_node_name_by_id(int node_id);
//int sys_conf_get_node_name(int node_id,char* result);

char* sys_conf_get_domain_name_id_list();

int sys_conf_get_domain_processes(int domain_id,int result[],int len);

//universal
/*
 * 参数：
 * p_element - 父节点名称，p_attr - 父节点属性名称，p_attr_value - 父节点属性名称对应的值
 * element - 节点名称
 * 返回值：节点的值
 */
char* sys_conf_get_value_by_key(char* p_element,char* p_attr,char* p_attr_value,char* element);

/*
 * 参数：
 * elements - 节点名称字符串，attributes - 节点属性名称字符串，attr_values - 节点属性名称对应的值字符串
 * 返回值：节点的值
 */
char* sys_conf_get_value_by_key_strings(char* elements,char* attributes,char* attr_values);

#ifdef __cplusplus
}
#endif


#endif
