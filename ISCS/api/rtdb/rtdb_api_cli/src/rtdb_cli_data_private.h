/*
 * @Copyright Reserved by XXXX.
 * structure methods used by api internally.
 * Create by KanWenDi, 2018.06.19.
 * Histories:
 *
 */
#ifndef RTDB_CLI_DATA_PRIVATE_H
#define RTDB_CLI_DATA_PRIVATE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "rtdb_cli_protocol.h"
#include <pthread.h>

#define RTDB_CMD_CHECKMASTER		1
#define	RTDB_CMD_GETATTR			2
#define	RTDB_CMD_GETDEVATTR			3
#define RTDB_CMD_GETACCATTR			4
#define RTDB_CMD_GETDIGATTR			5
#define RTDB_CMD_GETANAATTR			6
#define RTDB_CMD_MULTIGETATTR		7
#define RTDB_CMD_MULTIGETDEVATTR	8
#define RTDB_CMD_MULTIGETACCATTR	9
#define RTDB_CMD_MULTIGETDIGATTR	10
#define RTDB_CMD_MULTIGETANAATTR	11
#define RTDB_CMD_GETNODE        	12
#define RTDB_CMD_GETNODETYPE       	13
#define RTDB_CMD_LISTCHILDREN      	14  // RTDB_CMD_GETNODEKEYS
#define RTDB_CMD_MGETNODE        	15
#define RTDB_CMD_MMULTIGETATTR		16 
#define RTDB_CMD_GETATTRKEYS      	17
#define RTDB_CMD_SETATTR          	18
#define RTDB_CMD_SENDCMD          	19
#define RTDB_CMD_SENDMSG          	20
#define RTDB_CMD_QUERYTABLE         21
#define RTDB_CMD_UQUERYRECORD       22
#define RTDB_CMD_DELETERECORD      	23
#define RTDB_CMD_INSERTRECORD       24
#define RTDB_CMD_UPDATERECORD       25

#define RTDB_STATUS_OK              0
#define RTDB_STATUS_NETERR          1
#define RTDB_STATUS_SERVERERR       2
#define RTDB_STATUS_OTHERERR        3

#define RTDB_CLI_NOTIFY_TIMEOUT     60

#define RTDB_CLI_MAX_RETRY          2
#define RTDB_CLI_QUERY_MAX_RETRY    2

#define RTDB_CLI_BASEFD 			1
#define RTDB_CLI_SUBFD 				2

#define rtdb_cli_trace_bin(label,bytes,bytesLen) if(0) do{\
	int k = 0; \
	char bytesBuf[10240] = { 0 }; \
	if (bytes && bytesLen>0) for (k = 0; k < bytesLen; k++) \
	snprintf(bytesBuf + strlen(bytesBuf), sizeof(bytesBuf), "%02x ", (unsigned char)bytes[k]); \
	CMN_LOGGER_DEBUG(logger, "%s: %s", label, bytesBuf); \
}while (0)

typedef struct
{
	int modePort;
	char *ip1; /* 双网卡，有两个ip地址（仅从Master获取信息）*/
	char *ip2; /* 双网卡，有两个ip地址（仅从Master获取信息） */
	char *active_ip; /* 当前可用ip，首选 ip1，不分配内存 */
	char *user;
	char *pswd;
} rtdb_server_node;

rtdb_server_node* rtdb_server_node_create();
void rtdb_server_node_free(rtdb_server_node *node);
int rtdb_cli_check_node_status(int domain,rtdb_server_node *node, int port, int timeout);

char* rtdb_cli_data_gen_uuid();
rtdb_cli_variant* rtdb_cli_strvalue_to_variant(int vtType, const char *value);

typedef struct rtdb_cli_bindata {
	int num;
	unsigned char **arrBytes;
	int *arrBytesLen;
	int length;
} rtdb_cli_bindata;

rtdb_cli_bindata* rtdb_cli_data_create_bindata(int num);
void rtdb_cli_data_free_bindata(rtdb_cli_bindata *bindata);
int rtdb_cli_data_fill_bindata_paths(rtdb_cli_bindata *bindata,int8_t attrNum,const rtdb_cli_list *paths,const unsigned char *nameBytes,int nameBytesLen);

rtdb_cli_cmd_notifier* rtdb_cli_data_create_cmdNotifier(
	rtdb_cli_msg_rdbcmd *rdbcmd,
	rtdb_cli_cmdResult_notifier *resultNotifier,
	const void *userData
	);
void rtdb_cli_data_free_cmdNotifier(rtdb_cli_cmd_notifier *pNotify);


typedef struct {
	int domainid;
	char *label;
	int fd;
	int subfd;
	rtdb_cli_list *subRespList; // sub response list [rtdb_cli_subrep*,]
	pthread_mutex_t mtx;
} rtdb_cli_sockObj;
rtdb_cli_sockObj* rtdb_cli_create_sockObj();
void rtdb_cli_free_sockObj(rtdb_cli_sockObj *obj);
int rtdb_cli_get_domainIdbyLabel(rtdb_cli_sockObj** sockObjs,int sz,char *label);
rtdb_cli_sockObj* rtdb_cli_get_sockObj(rtdb_cli_sockObj** sockObjs, int sz, int domainid);

typedef struct {
	char *path;
	rtdb_cli_list *names;
} rtdb_cli_nodeSubInfo;
rtdb_cli_nodeSubInfo* rtdb_cli_create_nodeSubInfo(const char *path,const rtdb_cli_list *names);
void rtdb_cli_free_nodeSubInfo(rtdb_cli_nodeSubInfo *info);
int rtdb_cli_check_nodeSubInfo(rtdb_cli_list *L,const char *path);
int rtdb_cli_remove_nodeSubInfo(rtdb_cli_list *L,const char *path);


unsigned char* rtdb_cli_data_dump_variant(rtdb_cli_variant *vt, int *outLen);
int rtdb_cli_data_check_subresp(rtdb_cli_list *subRespList,int type,const char* subId,int seqnum, int subNum,int *flag);
rtdb_cli_record* rtdb_cli_data_fill_record(const rtdb_cli_node_item *pNodeItem);
rtdb_cli_list* rtdb_cli_data_get_nkeyList(rtdb_cli_node_item *pNodeItem);
rtdb_cli_list* rtdb_cli_data_get_vtList(rtdb_cli_reply *reply);
rtdb_cli_list* rtdb_cli_data_get_recordList(rtdb_cli_reply *reply);
unsigned char* rtdb_cli_data_get_pathBytes(const char* spath,int *outLen,int attrFlag);
unsigned char* rtdb_cli_data_get_updateNodeBytes(int cmdType, const char* path, rtdb_cli_variant *vt,int *outLen);
unsigned char* rtdb_cli_data_get_queryNodeBytes(int cmdType, const char* path, int *outLen);
unsigned char* rtdb_cli_data_get_mqueryNodeBytes(int cmdType, const rtdb_cli_list *paths, const rtdb_cli_list *names, int *outLen);
unsigned char* rtdb_cli_data_get_queryTableBytes(int cmdType,const char *tableName,const char *cond, const rtdb_cli_list *filters,int *outLen);
unsigned char* rtdb_cli_data_get_updateRecordBytes(int cmdType, const char *tableName, const char *cond, const rtdb_cli_record *record, int *outLen);
int rtdb_cli_data_cmpstr(const char *val1, const char *val2);
int rtdb_cli_data_cmpsub(rtdb_cli_subscriber *val, rtdb_cli_subscriber *key);
rtdb_cli_subscriber* rtdb_cli_data_find_sub(rtdb_cli_list *subList,const char *uuid);
int rtdb_cli_data_add_subrep(rtdb_cli_sockObj *obj,unsigned char *bytes,int msgLen);
int rtdb_cli_data_dump_names(unsigned char **p, int *outLen, const rtdb_cli_list *names);
rtdb_cli_reply* rtdb_cli_data_get_reply(int fd,unsigned char *bytes,int bytesLen);
unsigned char* rtdb_cli_data_read_bytes(int fd,int *len);
int rtdb_cli_data_encode_spath(const char *spath,unsigned char **nBytes,int *nBytesLen);
unsigned char* rtdb_cli_data_get_finalBytes(int domainId,int cmdType,const unsigned char *nBytes,int nBytesLen,int *bytesLen);	


#ifdef __cplusplus
}
#endif
#endif