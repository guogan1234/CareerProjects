/*
 * @Copyright Reserved by XXXX.
 * structures used by api users.
 * Create by KanWenDi, 2018.06.19.
 * Histories:
 *
 */
#ifndef RTDB_CLI_DATA_H
#define RTDB_CLI_DATA_H
#ifdef __cplusplus
extern "C" {
#endif

#include "rtdb_cli_list.h"


#define RTDB_CLI_VARIANT_UNDEF    0
#define RTDB_CLI_VARIANT_BOOL     1
#define RTDB_CLI_VARIANT_INT8     2
#define RTDB_CLI_VARIANT_INT16    3
#define RTDB_CLI_VARIANT_INT32    4
#define RTDB_CLI_VARIANT_INT64    5
#define RTDB_CLI_VARIANT_FLOAT    6
#define RTDB_CLI_VARIANT_DOUBLE   7
#define RTDB_CLI_VARIANT_STRING   8


typedef struct _rtdb_cli_variant
{
    int type;
    union
    {
        char *string; // RTDB_CLI_VARIANT_STRING 
        double real; // RTDB_CLI_VARIANT_DOUBLE
        long long integer; // RTDB_CLI_VARIANT_BOOL or RTDB_CLI_VARIANT_INTxx
    } v;
} rtdb_cli_variant;

typedef struct
{
	int type;
	char *target;
	int proSysId;
	int ctrType;
	int expiredDigValue;
	float anaValue;
	float expiredAnaValue;
} rtdb_cli_command;

typedef struct _rtdb_cli_record rtdb_cli_record;
typedef struct _rtdb_cli rtdb_cli;
typedef struct _rtdb_cli_cmd_notifier rtdb_cli_cmd_notifier;
typedef struct _rtdb_cli_subscriber rtdb_cli_subscriber;


/*
 * rtdb_cli_table_notifier.
 * tableName -- 输入，表名称。
 * type -- 输入， 数据通知类型。[1 添加；2 更新； 3 删除]
 * records -- 输入， 记录列表。
 * 订阅通知接口，用于向订阅者通知表类型的订阅数据。由订阅者实现。
 *
 * rtdb_cli_node_notifier。
 * nodePath -- 输入，点路径。
 * record -- 输入，点属性记录。
 * 订阅通知接口，用于订阅者通知点类型的订阅数据。由订阅者实现。
 *
 * rtdb_cli_exception_notifier.
 * code -- 输入，异常码，0 正常， !0 异常。
 * 异常通知接口，用于发生异常后进行异常状态通知。
 */
typedef void(rtdb_cli_table_notifier)(const rtdb_cli_subscriber *subscriber,const char *tableName, int type, const rtdb_cli_list *records);
typedef void(rtdb_cli_node_notifier)(const rtdb_cli_subscriber *subscriber,const char *nodePath, const rtdb_cli_record *record);
typedef void(rtdb_cli_exception_notifier)(const rtdb_cli_subscriber *subscriber,int code);

/*
 * rtdb_cli_cmdResult_notifier.
 * userData -- 输入，用户数据。
 * domainId -- 输入，域ID。
 * type -- 输入，命令类型。
 * destination -- 输入，命令的目标系统。
 * resultCode -- 输入，命令发送状态码：0 成功，-1 失败。
 * result -- 输入，结果描述（内容由应用系统、FES系统商定）。
 * 命令结果通知函数。
 */
typedef void(rtdb_cli_cmdResult_notifier)(const void *userData, int domainId, int type, const char* destination, int resultCode, const char *result);

struct _rtdb_cli_subscriber
{ 
	char uuid[37];
	int subType;
	char *name;
	int seqnum;
	int subTime; // timestamp

	rtdb_cli_table_notifier *tableNotifier;
	rtdb_cli_node_notifier *nodeNotifier;
	rtdb_cli_exception_notifier *expectionNotifier;  
	rtdb_cli_cmd_notifier *cmdNotifier;  

	rtdb_cli_list *tables;
	rtdb_cli_list *nodes; 
};

#ifdef __cplusplus
}
#endif
#endif
