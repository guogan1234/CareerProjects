/*
 @Copyright Reserved by XXXX.
 RTDB client.
 Create by KanWenDi, 2018.04.03.
 Histories:

 */
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include <objbase.h>
#include <winsock2.h>
#include <stddef.h>
#include <ws2tcpip.h>
#include <winerror.h>
#define EINPROGRESS WSAEINPROGRESS
#else
#include <uuid/uuid.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#endif
#include <pthread.h>

#include "rtdb_cli_cmn.h"
#include "rtdb_cli_list.h"
#include "rtdb_client.h"
#include "rtdb_cli_net.h"
#include "rtdb_cli_base.h"
#include "rtdb_cli_path.h"
#include "rtdb_cli_data.h"
#include "rtdb_cli_data_private.h"
#include "rtdb_cli_data_public.h"
#include "rtdb_cli_sds.h"
#include "rtdb_cli_protocol.h"
#include "conf_parser.h"

#define RTDB_CMD_CHECKMASTER		"checkMasterStatus"

#define RTDB_CMD_GETATTR			"GETATTR"
#define RTDB_CMD_GETDEVATTR			"GETDEVATTR"
#define RTDB_CMD_GETACCATTR			"GETACCATTR"
#define RTDB_CMD_GETDIGATTR			"GETDIGATTR"
#define RTDB_CMD_GETANAATTR			"GETANAATTR"
#define RTDB_CMD_MULTIGETATTR		"MGETATTR"
#define RTDB_CMD_MULTIGETDEVATTR	"MGETDEVATTR"
#define RTDB_CMD_MULTIGETACCATTR	"MGETACCATTR"
#define RTDB_CMD_MULTIGETDIGATTR	"MGETDIGATTR"
#define RTDB_CMD_MULTIGETANAATTR	"MGETANAATTR"
#define RTDB_CMD_GETNODE        	"GETNODE"
#define RTDB_CMD_GETNODEKEYS      	"GETNODEKEYS"
#define RTDB_CMD_MGETNODE        	"MGETNODE"
#define RTDB_CMD_MMULTIGETATTR		"MMGETATTR"

#define RTDB_CMD_GETATTRKEYS      	"GETATTRKEYS"
#define RTDB_CMD_SETATTR          	"SETATTR"

#define RTDB_CMD_SENDCMD          	"SENDCMD"
#define RTDB_CMD_SENDMSG          	"SENDMSG"

#define RTDB_CMD_QUERYTABLE         "QUERYTABLE"
#define RTDB_CMD_UQUERYRECORD       "UQUERYRECORD"
#define RTDB_CMD_DELETERECORD      	"DELETERECORD"
#define RTDB_CMD_INSERTRECORD       "INSERTRECORD"
#define RTDB_CMD_UPDATERECORD       "UPDATERECORD"

#define RTDB_STATUS_OK              0
#define RTDB_STATUS_NETERR          1
#define RTDB_STATUS_SERVERERR       2
#define RTDB_STATUS_OTHERERR        3

#define RTDB_CLI_NOTIFY_TIMEOUT     60

#define RTDB_CLI_MAX_RETRY          3

cmn_logger* logger = NULL;

typedef struct
{
    int basePort;
    int modePort;
    char *ip1; /* 双网卡，有两个ip地址（仅从Master获取信息）*/
    char *ip2; /* 双网卡，有两个ip地址（仅从Master获取信息） */
    char *active_ip; /* 当前可用ip，首选 ip1，不分配内存 */
    char *user;
    char *pswd;
    rtdb_cli_context *baseContext;
    rtdb_cli_context *modeContext;
} rtdb_server_node;

struct _rtdb_cli
{
    rtdb_cli_list *clients;
    rtdb_server_node *master;
    int last_error; /* 0正常，1网络故障，2服务器错误，3其它 */
};

int g_sub_notify_status = 0 ; /* 0 : not start , 1 : start ok */
rtdb_cli_list *gNotifyList = NULL; /* rtdb_cli_subscriber list */
pthread_mutex_t g_notify_mutex=PTHREAD_MUTEX_INITIALIZER;

rtdb_cli *gRtdbSubCli = NULL;

static rtdb_cli_cmd_notifier* rtdb_cli_cmd_notifier_create(
        rtdb_cli_msg_rdbcmd *rdbcmd,
        rtdb_cli_cmdResult_notifier *resultNotifier,
        const void *userData
);
static void rtdb_cli_cmd_notifier_free(rtdb_cli_cmd_notifier *pNotify);    
static rtdb_server_node* rtdb_server_node_create();
static void rtdb_server_node_free(rtdb_server_node *node);
static int rtdb_cli_reset_lasterror(rtdb_cli* c);
static int rtdb_cli_check_tcp_status(const char* ip,int port,int timeout);
static int rtdb_cli_check_node_status(rtdb_server_node *retNode,int port,int timeout);
static rtdb_server_node* rtdb_cli_get_master(rtdb_cli *c);
static char* rtdb_cli_gen_generic_path_str(const char* path);
static char* rtdb_cli_gen_query_node_generic_cmd(const char* cmdName,const char* path);
static char* rtdb_cli_gen_table_generic_cmd_prefix(
    const char* cmdName,int domainId,const char *tableName, const char *cond,int condFlag);
static char* rtdb_cli_gen_query_table_generic_cmd(
    const char* cmdName,int domainId,const char *tableName,
    const char *cond, const rtdb_cli_list *filters);
static char* rtdb_cli_gen_update_table_generic_cmd(
    const char* cmdName,int domainId,const char *tableName,
    const char *cond,int condFlag,const rtdb_cli_record *record);
static rtdb_cli_variant* rtdb_cli_reply_tovariant(const rtdb_cli_reply **element);
static rtdb_cli_context* rtdb_cli_get_baseclient(rtdb_cli *c);
static rtdb_cli_context* rtdb_cli_get_modeclient(rtdb_cli *c);
static rtdb_cli_reply* rtdb_cli_do_read_query(rtdb_cli *c,const char *queryString);
static int rtdb_cli_do_write_query(rtdb_cli *c,const char *queryString);
static rtdb_cli_variant* rtdb_cli_unique_query_attribute_base(const char* cmdName,rtdb_cli *c,const char *path);
static rtdb_cli_record* rtdb_cli_multi_query_attributes_base(const char* cmdName,rtdb_cli *c,const char *path, const rtdb_cli_list *names);
static char* rtdb_cli_combine_cmd(const char *cmd,const char *attrBuf,const char *refAttrBuf);
static char* rtdb_cli_multi_query_attr_fill_cmd(const char *cmd,const rtdb_cli_list *names);
static int rtdb_cli_fill_record_use_reply3(rtdb_cli_record *record,
    int elementNum,const rtdb_cli_reply **element);
static char* rtdb_cli_gen_mquery_nodes_cmd_prefix(const char* cmdName,const rtdb_cli_list *paths);
static rtdb_cli_list* rtdb_cli_get_single_node_key_list(const char* cmdName,rtdb_cli *c,const char *path);
static rtdb_cli_list* rtdb_cli_query_table_base(rtdb_cli *c,int domainId,const char *tblName,const char *cond,const rtdb_cli_list *filters);
static void rtdb_cli_thread_clean_fun(void *arg);
static char* rtdb_cli_get_uuid_str();

static int rtdb_cli_do_cmd_subscribe(rtdb_cli *cli,const char *uuid);
static int rtdb_cli_do_sub_notify();
static rtdb_cli_subscriber* rtdb_cli_find_subscriber(rtdb_cli_list *subList,const char *uuid);

static rtdb_cli_byte* rtdb_cli_read_bytes(rtdb_cli_context *client,int *len);
static int rtdb_cli_compare_subscriber(rtdb_cli_subscriber *val, rtdb_cli_subscriber *key);
static rtdb_cli_tblsub* rtdb_cli_subtable_base(rtdb_cli_uuid uuid, int domainId, int8_t action, int subNum);
static int rtdb_cli_msubscribe_tables_base(rtdb_cli_subscriber *subscriber, int domainId, const rtdb_cli_list *tableNames, int8_t action);
static rtdb_cli_nodesub* rtdb_cli_subnode_base(rtdb_cli_uuid uuid,const rtdb_cli_list *names,int8_t action, int subNum);
static int rtdb_cli_msubscribe_nodes_base(
	rtdb_cli_subscriber *subscriber,
	const rtdb_cli_list *nodePaths,
	const rtdb_cli_list *names,
	int8_t action);
static int rtdb_cli_do_tbl_subcribe(rtdb_cli *cli, const rtdb_cli_tblsub *tblsub);
static int rtdb_cli_do_node_subcribe(rtdb_cli *cli, const rtdb_cli_nodesub *nodesub);
static int rtdb_cli_nodepub_notify(const rtdb_cli_nodepub *pub);

static int rtdb_cli_compare_subscriber(rtdb_cli_subscriber *val, rtdb_cli_subscriber *key);
static int rtdb_cli_init_rtdb_sub_cli();
static int rtdb_cli_check_response(const rtdb_cli_byte *response, int msgLen,int subNum);
static void rtdb_cli_remove_subscriber(rtdb_cli_subscriber *subscriber);

rtdb_cli* rtdb_cli_create(const int* domainId)
{  
    rtdb_cli* cli = malloc(sizeof(*cli));
    sys_conf_rtdb_domain* rtdbConfig = NULL;
    sys_conf_rtdb_server* confNode = NULL;
    rtdb_server_node *snode;
    sys_conf_log* log;
    int tmpLen = 0;
  
    (void*)domainId;
    if(!cli) return NULL;
    cli->last_error = 0;
    cli->clients = rtdb_cli_list_create(rtdb_server_node_free,NULL);
    cli->master = NULL;
    if (!cli->clients)
    {
        free(cli);
        return NULL;
    }
    
    rtdbConfig = sys_conf_get_rtdb_domain();
    if(!rtdbConfig)
    {
        CMN_LOGGER_ERR(logger,"sys_conf_get_rtdb_domain fail!");
        free(cli);
        return NULL;
    }
    confNode = rtdbConfig->servers;
    while(confNode)
    {
        CMN_LOGGER_DEBUG(logger,"confNode->ip1 : %s\n",confNode->ip1);
        CMN_LOGGER_DEBUG(logger,"confNode->ip2 : %s\n",confNode->ip2);
        CMN_LOGGER_DEBUG(logger,"confNode->basePort : %d\n",confNode->basePort);
        CMN_LOGGER_DEBUG(logger,"confNode->modePort : %d\n",confNode->modePort);
        
        snode = rtdb_server_node_create();
        if(snode)
        {
            tmpLen = strlen(confNode->ip1);        
            snode->ip1 = malloc(tmpLen+1);
            memset(snode->ip1,0,tmpLen+1);
            memcpy(snode->ip1,confNode->ip1,tmpLen);

            tmpLen = strlen(confNode->ip2);
            snode->ip2 = malloc(tmpLen+1);
            memset(snode->ip2,0,tmpLen+1);
            memcpy(snode->ip2,confNode->ip2,tmpLen);

            snode->user = strdup(confNode->username);
            snode->pswd = strdup(confNode->password);
            snode->basePort = confNode->basePort;
            snode->modePort = confNode->modePort;
            rtdb_cli_list_push_tail(cli->clients,snode);
        } 
        confNode = confNode->next;
    } 

    log = rtdbConfig->log;
    if(!logger && log && log->file)
    {
        logger = cmn_logger_init(log->file,log->level,log->max_size,log->roll_num);
        //CMN_LOGGER_DEBUG(logger,"logger init ok");
    }
    sys_conf_free_rtdb_domain(rtdbConfig);
    //if(cli) rtdb_cli_get_master(cli);
    
    return cli;
}

void rtdb_cli_free(rtdb_cli *c)
{
    if(!c) return;
    if(c->clients) rtdb_cli_list_free(c->clients);
    free(c);

    if(logger)
    {
        cmn_logger_destory(logger);
        logger = NULL;
    }
    
}

int rtdb_cli_get_lasterror(rtdb_cli* c)
{
    if(!c) return RTDB_STATUS_OTHERERR;
    return c->last_error;
}

rtdb_cli_variant* rtdb_cli_unique_query_device_attribute(rtdb_cli *c, const char *path)
{
    return rtdb_cli_unique_query_attribute_base(RTDB_CMD_GETDEVATTR,c,path);
}

rtdb_cli_variant* rtdb_cli_unique_query_acc_attribute(rtdb_cli *c,const char *path)
{
	return rtdb_cli_unique_query_attribute_base(RTDB_CMD_GETACCATTR,c,path);
}

rtdb_cli_variant* rtdb_cli_unique_query_dig_attribute(rtdb_cli *c,const char *path)
{
	return rtdb_cli_unique_query_attribute_base(RTDB_CMD_GETDIGATTR,c,path);
}

rtdb_cli_variant* rtdb_cli_unique_query_ana_attribute(rtdb_cli *c,const char *path)
{
	return rtdb_cli_unique_query_attribute_base(RTDB_CMD_GETANAATTR,c,path);
}

rtdb_cli_record* rtdb_cli_multi_query_device_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_multi_query_attributes_base(RTDB_CMD_MULTIGETDEVATTR,c,path,names);
}

rtdb_cli_record* rtdb_cli_multi_query_acc_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_multi_query_attributes_base(RTDB_CMD_MULTIGETACCATTR,c,path,names);
}

rtdb_cli_record* rtdb_cli_multi_query_dig_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_multi_query_attributes_base(RTDB_CMD_MULTIGETDIGATTR,c,path,names);
}

rtdb_cli_record* rtdb_cli_multi_query_ana_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_multi_query_attributes_base(RTDB_CMD_MULTIGETANAATTR,c,path,names);
}

rtdb_cli_variant* rtdb_cli_unique_query_attribute(rtdb_cli *c,const char *path)
{    
    return rtdb_cli_unique_query_attribute_base(RTDB_CMD_GETATTR,c,path);
}

rtdb_cli_record* rtdb_cli_multi_query_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
    return rtdb_cli_multi_query_attributes_base(RTDB_CMD_MULTIGETATTR,c,path,names);  
}

rtdb_cli_record* rtdb_cli_query_node(rtdb_cli *c,const char *path)
{
    rtdb_cli_record *record;
    rtdb_cli_reply *reply = NULL;
    char *pStrCmd = NULL;
    int isError = 0;
    
    if(!c || !path)
    {
        CMN_LOGGER_ERR(logger,"path is NULL");        
        return NULL;
    }
    rtdb_cli_reset_lasterror(c);
    
    CMN_LOGGER_DEBUG(logger,"path : %s",path);
    pStrCmd = rtdb_cli_gen_query_node_generic_cmd(RTDB_CMD_GETNODE,path); 
    if(!pStrCmd)
    {
        CMN_LOGGER_WARN(logger,"rtdb_cli_query_node : pStrCmd is NULL");
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }    
    reply = rtdb_cli_do_read_query(c,pStrCmd);    
    free(pStrCmd);
    
    if (!reply || reply->type != RTDB_REPLY_ARRAY || reply->elements%3 != 0)
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_query_node : reply error");
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }
    record = rtdb_cli_record_create(reply->elements/3);
    if (!record)
    {
        free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_ERR(logger,"rtdb_cli_query_node : record is NULL");
		return NULL;
	}
    isError = rtdb_cli_fill_record_use_reply3(
            record,reply->elements,(const rtdb_cli_reply **)reply->element);
    if(isError)
    {
        rtdb_cli_record_free(record);
        c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_ERR(logger,"rtdb_cli_query_node : isError=%d",isError);
        record = NULL;
	} 
    free_reply_object(reply);
    return record;
}

rtdb_cli_list* rtdb_cli_multi_query_nodes(rtdb_cli *c, const rtdb_cli_list *paths)
{
    rtdb_cli_list* retList = NULL;   
    rtdb_cli_reply *reply = NULL;
    int isError = 0;
    char *cmd = NULL;
    int sz = 0,lineCount = 0,fieldCount = 0;
    int i;
    rtdb_cli_reply **element;
    
    if(!c || !paths) 
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    rtdb_cli_reset_lasterror(c);
    
    cmd = rtdb_cli_gen_mquery_nodes_cmd_prefix(RTDB_CMD_MGETNODE,paths);
    if(!cmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    reply = rtdb_cli_do_read_query(c,cmd);    
    free(cmd);
    
    if (!reply || reply->type != RTDB_REPLY_ARRAY )
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_multi_query_nodes reply error");
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }
    
    sz = rtdb_cli_list_get_size((rtdb_cli_list*)paths);    
    retList = rtdb_cli_list_create(rtdb_cli_record_free,NULL);
    if(!retList)
    {
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;        
    }
    
    element = reply->element;
    lineCount = reply->element[0]->integer;// line count
    CMN_LOGGER_DEBUG(logger,"lineCount : %d",lineCount);
    if(sz != lineCount)
    {
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        rtdb_cli_list_free(retList);
        return NULL;
    }
    
    element += 1;
    for(i = 0 ; i < lineCount ; i++)
    {
        rtdb_cli_record *record = NULL;        
        fieldCount = element[0]->integer;
        CMN_LOGGER_DEBUG(logger,"fieldCount : %d",fieldCount);
        if(fieldCount)
        {
            record = rtdb_cli_record_create(fieldCount);
            if (!record)
            {
                isError = 1;
                CMN_LOGGER_WARN(logger,"record is NULL");
                break;
            }
            isError = rtdb_cli_fill_record_use_reply3(
                    record,fieldCount*3,(const rtdb_cli_reply **)element+1);
            if(isError)
            {
                rtdb_cli_record_free(record);
                CMN_LOGGER_ERR(logger,"rtdb_cli_fill_record_use_reply3 error");
                break;
            }
        }
        if(!record)
            record = rtdb_cli_record_create(0);
        rtdb_cli_list_push_tail(retList,record);
        element += (fieldCount*3 +1);
    }
    if (isError)
    {
        free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        rtdb_cli_list_free(retList);
		return NULL;
	}
    free_reply_object(reply);  
    CMN_LOGGER_DEBUG(logger,"retList size : %d",rtdb_cli_list_get_size((rtdb_cli_list*)retList)); 
    return retList;
}

rtdb_cli_list* rtdb_cli_mmulti_query_attributes(rtdb_cli *c, const rtdb_cli_list *paths, const rtdb_cli_list *names)
{
    rtdb_cli_list* retList = NULL;   
    rtdb_cli_reply *reply = NULL;
    int isError = 0;
    char *cmdPrefix = NULL,*cmd = NULL;
    int sz = 0,lineCount = 0,fieldCount = 0;
    int i,j;
    rtdb_cli_reply **element;
    rtdb_cli_variant *v = NULL;
    char *name = NULL,*eleName = NULL;
    
    if(!c || !paths || !names) 
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    rtdb_cli_reset_lasterror(c);
    
    cmdPrefix = rtdb_cli_gen_mquery_nodes_cmd_prefix(RTDB_CMD_MMULTIGETATTR,paths);
    if(!cmdPrefix)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    
    cmd = rtdb_cli_multi_query_attr_fill_cmd(cmdPrefix,names);
    free(cmdPrefix);
    if(!cmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }    
    reply = rtdb_cli_do_read_query(c,cmd);    
    free(cmd);
    
    if (!reply || reply->type != RTDB_REPLY_ARRAY )
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_mmulti_query_attributes reply error");
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }
    
    sz = rtdb_cli_list_get_size((rtdb_cli_list*)paths);    
    retList = rtdb_cli_list_create(rtdb_cli_variant_free,NULL);
    if(!retList)
    {
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;        
    }
    
    element = reply->element;
    lineCount = reply->element[0]->integer;// line count
    CMN_LOGGER_DEBUG(logger,"lineCount : %d",lineCount);
    if(sz != lineCount)
    {
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        rtdb_cli_list_free(retList);
        return NULL;
    }
    
    sz = rtdb_cli_list_get_size((rtdb_cli_list*)names);
    element += 1;
    for(i = 0 ; i < lineCount ; i++)
    {
        fieldCount = element[0]->integer;
        CMN_LOGGER_DEBUG(logger,"fieldCount : %d",fieldCount);
        name = (char*)rtdb_cli_list_get_first((rtdb_cli_list*)names);
        element += 1;
        while (name)
        {
         //   CMN_LOGGER_DEBUG(logger,"name : %s",name);  
            // find 
            for (j = 0; j < fieldCount * 3; j += 3) 
            {
                eleName = element[j]->str;
                if(!eleName) break;
                //CMN_LOGGER_DEBUG(logger,"eleName : %s",eleName); 
                v = rtdb_cli_reply_tovariant((const rtdb_cli_reply **)&element[j+1]);
                if (!v) break;
                if(!strcmp(eleName,name))
                {
                    // find it 
                  //  CMN_LOGGER_DEBUG(logger,"find it"); 
                    break;
                }
                else
                {
                    // not find 
                    rtdb_cli_variant_free(v);
                    v = rtdb_cli_variant_create();
                    v->type = RTDB_CLI_VARIANT_UNDEF;
                }
            }
            rtdb_cli_list_push_tail(retList,v);
            name = (char*)rtdb_cli_list_get_next((rtdb_cli_list*)names);
        }
        element += (fieldCount*3);
    }
    if (isError)
    {
        free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        rtdb_cli_list_free(retList);
		return NULL;
	}
    free_reply_object(reply);  
    CMN_LOGGER_DEBUG(logger,"retList size : %d",rtdb_cli_list_get_size((rtdb_cli_list*)retList)); 
    return retList;
}

rtdb_cli_list* rtdb_cli_list_children(rtdb_cli *c,const char *path)
{
    // subNode keys
    return rtdb_cli_get_single_node_key_list(RTDB_CMD_GETNODEKEYS,c,path);   
}

rtdb_cli_list* rtdb_cli_list_attributes(rtdb_cli *c, const char *path)
{
    // attributes + refRecordNodes     
    return rtdb_cli_get_single_node_key_list(RTDB_CMD_GETATTRKEYS,c,path);    
}

int rtdb_cli_set_attribute(rtdb_cli *c, const char *path, const char *value)
{
    int retCode = 0;
    char *pStrCmd = NULL,*cmdPrefix,*pTmp;
    int cmdLen = 0;
    
    if(!c || !path || !value)
        return -1;
    
    rtdb_cli_reset_lasterror(c);
    CMN_LOGGER_DEBUG(logger,"path : %s",path);
    cmdPrefix = rtdb_cli_gen_query_node_generic_cmd(RTDB_CMD_SETATTR,path); 
    if(!cmdPrefix)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_WARN(logger,"cmdPrefix is NULL");
        return -1;
    }    
   
    cmdLen = strlen(cmdPrefix) + 1 + strlen(value);
    pStrCmd = malloc(cmdLen + 1);
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        free(cmdPrefix);
        return -1;
    }
    memset(pStrCmd,0,cmdLen + 1);
    pTmp = pStrCmd;
    sprintf(pTmp,"%s",cmdPrefix);
    sprintf(pTmp + strlen(pStrCmd)," %s",value);
    retCode = rtdb_cli_do_write_query(c,pStrCmd);
    free(pStrCmd);
    return retCode;
}

void rtdb_cli_send_command(
	rtdb_cli *c, 
	int domainId, 
	int type, 
	const char* destination, 
	const char *cmdJson, 
	rtdb_cli_cmdResult_notifier *resultNotifier,
	const void *userData
)
{
    int retCode = 0;
    char *uuid = NULL;
    rtdb_cli_msg_rdbcmd *rdbcmd = NULL;
    rtdb_cli_cmd_notifier *pNotify = NULL;
    rtdb_cli_subscriber *pSubcriber = NULL;
    rtdb_cli_byte *bytes;
    int dumpLen = 0,nwritten = 0;
    rtdb_cli_context *client;
    
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_send_command start");
    if(!c || !cmdJson || !resultNotifier)
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_send_command : c or cmdJson or resultNotifier is NULL");
        return;
    }
    
    uuid = rtdb_cli_get_uuid_str();
    if(!uuid) return ;
    rdbcmd = rtdb_cli_create_msg_rdbcmd(domainId,type,uuid,destination,cmdJson);
    free(uuid);
    if(!rdbcmd)  return ;
    
    // send cmd
    client = rtdb_cli_get_modeclient(c);
    if(!client) return;
    bytes = rtdb_cli_dump_msg_rdbcmd(rdbcmd,&dumpLen);
    if(!bytes || !dumpLen )
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_send_command : dump data error");
        rtdb_cli_free_msg_rdbcmd(rdbcmd);
        return ;
    }
    
    nwritten = network_write(client->fd,bytes,dumpLen);
    free(bytes);
    if(nwritten != dumpLen)
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_send_command : send data error");
        return;
    }

	rtdb_cli_init_rtdb_sub_cli();
    // do subscribe
	retCode = rtdb_cli_do_cmd_subscribe(gRtdbSubCli, rdbcmd->id);
	if (retCode != 0)
	{
		rtdb_cli_free_subscriber(pSubcriber);
		CMN_LOGGER_ERR(logger, "rtdb_cli_send_command : rtdb_cli_do_cmd_subscribe error");
		return;
	}
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_send_command : rtdb_cli_do_cmd_subscribe ok");

    pNotify = rtdb_cli_cmd_notifier_create(
        rdbcmd,resultNotifier,userData
    ); // release in thread
    if(!pNotify) 
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_send_command : rtdb_cli_cmd_notifier_create fail");
        rtdb_cli_free_msg_rdbcmd(rdbcmd);
        return ;
    }

    pSubcriber = rtdb_cli_create_subscriber(NULL,NULL,NULL,NULL); // release in thread
    if(!pSubcriber) 
    {
        rtdb_cli_free_msg_rdbcmd(rdbcmd);
        rtdb_cli_cmd_notifier_free(pNotify);
        CMN_LOGGER_ERR(logger,"rtdb_cli_send_command : rtdb_cli_create_subscriber fail");
        return ;
    }
    pSubcriber->cmdNotifier = pNotify;
	memcpy(pSubcriber->uuid, rdbcmd->id, sizeof(pSubcriber->uuid));
	pSubcriber->subType = RTDB_CLI_SUBTYPE_CMD; 
   
    // create thread to subscribe and recv result
    retCode = rtdb_cli_do_sub_notify();
    if(retCode != 0)
    {   
        CMN_LOGGER_ERR(logger,"rtdb_cli_send_command : rtdb_cli_do_sub_notify error");
        return ;
    }    
    return ;
}

int rtdb_cli_send_message(rtdb_cli *c, int domainId, int type, const char* destination, const char *msg, int msgSize)
{
    int retCode = 0; // 0 : ok , -1 : error
    char *pStrCmd;
    int cmdLen = 0;
 
    (void)msgSize;
    if(!c || !msg) return -1;
    rtdb_cli_reset_lasterror(c);
    
    cmdLen = strlen(RTDB_CMD_SENDMSG) + strlen(destination) + strlen(msg) + 64;
    pStrCmd = malloc(cmdLen);
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return -1;
    }
    sprintf(pStrCmd,"%s %d %d %s \"%s\"",
        RTDB_CMD_SENDMSG,domainId,type,destination,msg
    );    
    retCode = rtdb_cli_do_write_query(c,pStrCmd);
    free(pStrCmd);
    return retCode;
}

rtdb_cli_list* rtdb_cli_query_table(rtdb_cli *c, int domainId, const char *tblName, const rtdb_cli_list *filters)
{
    return rtdb_cli_query_table_base(c,domainId,tblName,NULL,filters);
}

rtdb_cli_record* rtdb_cli_unique_query_record(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_list *filters
)
{
    rtdb_cli_record* retRecord = NULL;
    rtdb_cli_reply *reply = NULL;
    char *pStrCmd = NULL;
    int isError = 0;
    
    if(!c || !tblName) 
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    rtdb_cli_reset_lasterror(c);
    pStrCmd = rtdb_cli_gen_query_table_generic_cmd(RTDB_CMD_UQUERYRECORD,domainId,tblName,cond,filters);
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    
    reply = rtdb_cli_do_read_query(c,pStrCmd);
    free(pStrCmd);

    if (!reply || reply->type != RTDB_REPLY_ARRAY || (reply->elements%3 != 0))
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_unique_query_record reply error");
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }
    retRecord = rtdb_cli_record_create(1);
    if (!retRecord)
    {
        free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
		return NULL;
	}
    isError = rtdb_cli_fill_record_use_reply3(
            retRecord,reply->elements,(const rtdb_cli_reply **)reply->element);
    if(isError)
    {
        rtdb_cli_record_free(retRecord);
        retRecord = NULL;
        c->last_error = RTDB_STATUS_OTHERERR;
	}  
    free_reply_object(reply); 
    return retRecord;    
}

RTDB_CLI_API rtdb_cli_list* rtdb_cli_multi_query_records(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_list *filters
)
{
	return rtdb_cli_query_table_base(c,domainId,tblName,cond,filters);
}

int rtdb_cli_delete_records(rtdb_cli *c, int domainId, const char *tblName, const char *cond)
{
    int retCode = 0 ;
    char *pStrCmd = NULL;

    if(!c || !tblName)
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        return -1;
    }
    rtdb_cli_reset_lasterror(c);
    pStrCmd = rtdb_cli_gen_query_table_generic_cmd(RTDB_CMD_DELETERECORD,domainId,tblName,cond,NULL);
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_WARN(logger,"pStrCmd is NULL");
        return -1;
    }
    retCode = rtdb_cli_do_write_query(c,pStrCmd);
    free(pStrCmd);
    return retCode;
}

int rtdb_cli_update_records(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_record *record
)
{
    int retCode = 0 ;
    char *pStrCmd = NULL;
    
    if(!c || !tblName ||!record)
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_DEBUG(logger,"tableName or queryString or record is NULL");
        return -1;
    }
    rtdb_cli_reset_lasterror(c);

    // do update
    pStrCmd = rtdb_cli_gen_update_table_generic_cmd(RTDB_CMD_UPDATERECORD,domainId,tblName,cond,1,record);
   
    // execute query command 
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_DEBUG(logger,"pStrCmd is NULL");
        return -1;
    }
    CMN_LOGGER_DEBUG(logger,"pStrCmd : %s ",pStrCmd);
    retCode = rtdb_cli_do_write_query(c,pStrCmd);
    free(pStrCmd);
    return retCode;
}

int rtdb_cli_insert_record(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_record *record
)
{
    int retCode = 0 ;
    char *pStrCmd = NULL;

    if(!c || !tblName || !cond || !record)
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_ERR(logger,"tableName or queryString or record is NULL");
        return -1;
    }
    rtdb_cli_reset_lasterror(c);
    // do insert
    pStrCmd = rtdb_cli_gen_update_table_generic_cmd(
        RTDB_CMD_INSERTRECORD,domainId,tblName,cond,1,record);
    // execute query command 
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_WARN(logger,"pStrCmd is NULL");
        return -1;
    }
    CMN_LOGGER_DEBUG(logger,"pStrCmd : %s ",pStrCmd);
    retCode = rtdb_cli_do_write_query(c,pStrCmd);
    free(pStrCmd);   
    return retCode;
}

rtdb_cli_subscriber* rtdb_cli_create_subscriber(
	const char *subscriberName,
	rtdb_cli_node_notifier *nodeNotifier,
	rtdb_cli_table_notifier *tableNotifier,
	rtdb_cli_exception_notifier *expectionNotifier
)
{
    rtdb_cli_subscriber *retSub = NULL;
	char *uuid;
    retSub = calloc(1,sizeof(*retSub));
    if(!retSub) return NULL;
	uuid = rtdb_cli_get_uuid_str();
	if (!uuid)
	{
		free(retSub);
		return NULL;
	}
	memcpy(retSub->uuid, uuid, sizeof(retSub->uuid));
	free(uuid);
    if(subscriberName) retSub->name = strdup(subscriberName);
    retSub->subTime = 0;
    retSub->nodeNotifier = nodeNotifier;
    
	retSub->tableNotifier = tableNotifier;
    retSub->expectionNotifier = expectionNotifier;
    retSub->cmdNotifier = NULL;

	rtdb_cli_init_rtdb_sub_cli();
	// add to notify list
	if (!gNotifyList)
		gNotifyList = rtdb_cli_list_create(NULL,rtdb_cli_compare_subscriber);
	if (!rtdb_cli_find_subscriber(gNotifyList, retSub->uuid))
	{
        pthread_mutex_lock(&g_notify_mutex);
		rtdb_cli_list_push_tail(gNotifyList, retSub);
        pthread_mutex_unlock(&g_notify_mutex);
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_create_subscriber : add ok , sz : %d",
			rtdb_cli_list_get_size((rtdb_cli_list*)gNotifyList));
	}
	else
	{
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_create_subscriber : alread exist");
	}
    return retSub;
}

static void rtdb_cli_remove_subscriber(rtdb_cli_subscriber *subscriber)
{
	// remove from notify list
	if (gNotifyList)
    {
        pthread_mutex_lock(&g_notify_mutex);
        rtdb_cli_list_remove(gNotifyList, subscriber);
        pthread_mutex_unlock(&g_notify_mutex);
    }
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_remove_subscriber : sz : %d",
		rtdb_cli_list_get_size((rtdb_cli_list*)gNotifyList));
}

void rtdb_cli_free_subscriber(rtdb_cli_subscriber *subscriber)
{
    if(!subscriber) return ;
	rtdb_cli_remove_subscriber(subscriber);
    if(subscriber->name) free(subscriber->name);
    subscriber->nodeNotifier = NULL;
    subscriber->tableNotifier = NULL;
    subscriber->expectionNotifier = NULL;
    if(subscriber->cmdNotifier)
    {
        rtdb_cli_cmd_notifier_free(subscriber->cmdNotifier);
        subscriber->cmdNotifier = NULL;
    }
    free(subscriber);
}


int rtdb_cli_unique_subscribe_table(rtdb_cli_subscriber *subscriber, int domainId, const char *tableName)
{
    int retCode = 0;
	int subNum = 1;
	rtdb_cli_tblsub *tblsub;

    if(!subscriber || !tableName)
        return -1;

	rtdb_cli_init_rtdb_sub_cli();
	tblsub = rtdb_cli_subtable_base(subscriber->uuid,domainId,RTDB_CLI_ACT_SUB,subNum);
	if (!tblsub) return -1;

	tblsub->noArr[0] = 1;
	snprintf((char*)tblsub->nameArr,sizeof(rtdb_cli_tbname),"%s",tableName);
	retCode = rtdb_cli_do_tbl_subcribe(gRtdbSubCli,tblsub);
	rtdb_cli_free_tblsub(tblsub);
	if (retCode != 0) return -1;
	/*rtdb_cli_do_sub_notify();*/
	return 0;
}

int rtdb_cli_multi_subscribe_tables(rtdb_cli_subscriber *subscriber, 
	int domainId, const rtdb_cli_list *tableNames)
{
	if (!subscriber || !tableNames) return -1;
	return rtdb_cli_msubscribe_tables_base(subscriber, domainId, tableNames, RTDB_CLI_ACT_SUB);
}

int rtdb_cli_unique_subscribe_node(rtdb_cli_subscriber *subscriber, const char *nodePath, const rtdb_cli_list *names)
{
	int retCode = 0;
	int subNum = 1;
	rtdb_cli_nodesub *nodesub;
	
	if (!subscriber || !nodePath || !names) return -1;

	nodesub = rtdb_cli_subnode_base(subscriber->uuid,names,RTDB_CLI_ACT_SUB,subNum);
	if (!nodesub) return -1;

	nodesub->noArr[0] = 1;
	snprintf((char*)nodesub->path,sizeof(rtdb_cli_sub_path),"%s",nodePath);

	rtdb_cli_init_rtdb_sub_cli();
	retCode = rtdb_cli_do_node_subcribe(gRtdbSubCli,nodesub);
	rtdb_cli_free_nodesub(nodesub);
	if (retCode != 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_unique_subscribe_node : rtdb_cli_do_node_subcribe error");
		return -1;
	}
	/* rtdb_cli_do_sub_notify(); */
	return 0;
}

int rtdb_cli_multi_subscribe_nodes(
	rtdb_cli_subscriber *subscriber,
	const rtdb_cli_list *nodePaths,
	const rtdb_cli_list *names
)
{
	if (!subscriber || !nodePaths || !names) return -1;
	return rtdb_cli_msubscribe_nodes_base(subscriber, nodePaths, names, RTDB_CLI_ACT_SUB);
}


int rtdb_cli_multi_unsubscribe_tables(rtdb_cli_subscriber *subscriber, int domainId, const rtdb_cli_list *tableNames)
{
	return rtdb_cli_msubscribe_tables_base(subscriber, domainId, tableNames, RTDB_CLI_ACT_CANCELSUB);
}

int rtdb_cli_multi_unsubscribe_nodes(rtdb_cli_subscriber *subscriber, const rtdb_cli_list *nodePaths)
{
	return rtdb_cli_msubscribe_nodes_base(subscriber, nodePaths, NULL, RTDB_CLI_ACT_CANCELSUB);
}

void rtdb_cli_publish(rtdb_cli_subscriber *subscriber)
{
	int retCode = 0 ;
	retCode = rtdb_cli_do_sub_notify();
	if (retCode != 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_send_command : rtdb_cli_do_sub_notify error");
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////

static rtdb_cli_cmd_notifier* rtdb_cli_cmd_notifier_create(
    rtdb_cli_msg_rdbcmd *rdbcmd,
	rtdb_cli_cmdResult_notifier *resultNotifier,
	const void *userData
)
{
    rtdb_cli_cmd_notifier *pNotify = NULL;
    pNotify = calloc(1,sizeof(rtdb_cli_cmd_notifier));
    if(!pNotify) return NULL;
    pNotify->rdbcmd = rdbcmd;
    pNotify->resultNotifier = resultNotifier;
    pNotify->userData = userData;    
    return pNotify;
}

static void rtdb_cli_cmd_notifier_free(rtdb_cli_cmd_notifier *pNotify)
{
    if(!pNotify) return ;
    if(pNotify->rdbcmd) rtdb_cli_free_msg_rdbcmd(pNotify->rdbcmd);  
	free(pNotify);
	pNotify = NULL;
}
    
static rtdb_server_node* rtdb_server_node_create()
{
    rtdb_server_node* node = malloc(sizeof(*node));    
    if (node) memset(node, 0, sizeof(*node));    
    return node;
}

static void rtdb_server_node_free(rtdb_server_node *node)
{
    if(!node) return;
    if (node->ip1) free(node->ip1);
    if (node->ip2) free(node->ip2);
    if (node->user) free(node->user);
    if (node->pswd) free(node->pswd);
    if(node->baseContext) rtdb_cli_contex_free(node->baseContext);
    if(node->modeContext) rtdb_cli_contex_free(node->modeContext);
    free(node);
	node = NULL;
}

static int rtdb_cli_check_tcp_status(const char* ip,int port,int timeout)
{
    int iRet = 0; // 0 : ok , -1 : fail
    int sockfd = 0;
    struct sockaddr_in saddr;
    int activity = 0;
    struct timeval tv;
    int status = 0;
    fd_set read_fds;
#ifdef _WIN32    
    long noblock = 1; // 1 : yes
    WSADATA wsaData;
    int iResult;
#endif

    if(!ip || !port) return -1;
   // CMN_LOGGER_DEBUG(logger,"rtdb_cli_check_tcp_status (%s:%d) ",ip,port);
#ifdef _WIN32 
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR) return -1;    
#endif  
 
    sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(port);

    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);

#ifdef _WIN32
    status = ioctlsocket(sockfd,FIONBIO,(unsigned long*)&noblock); 
    if (status != NO_ERROR)
    {
        WSACleanup();
        return -1;   
    }
#else
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif   
    
    status = connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(status < 0)
    {
        #ifndef _WIN32
        if (errno != EINPROGRESS) return -1;
        #endif
    }
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    activity = select(sockfd + 1,&read_fds,&read_fds,NULL,&tv);
    if (activity)
    {
        if(send(sockfd,"\x00",1,0) == 1) 
            iRet = 0;
        else
            iRet = -1;
    }
    else
        iRet = -1;
    
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
    return iRet;
}

static int rtdb_cli_check_node_status(rtdb_server_node *retNode,int port,int timeout)
{
    int iRet = -1; // 0 : ok , -1 : fail
    
    if(!retNode || !retNode->ip1 || !retNode->ip2 || !port || !timeout)
        return -1;
    
    retNode->active_ip = NULL;
    iRet = rtdb_cli_check_tcp_status(retNode->ip1,port,timeout);
    if(0 == iRet)
    {
        retNode->active_ip = retNode->ip1;
        return iRet;
    }
    
    iRet = rtdb_cli_check_tcp_status(retNode->ip2,port,timeout);
    if(0 == iRet)
        retNode->active_ip = retNode->ip2; 
    
    return iRet;    
}

static rtdb_server_node* rtdb_cli_get_master(rtdb_cli *c)
{
	int masterFlag,portStatus=0;
    rtdb_server_node *retNode = NULL;
    rtdb_cli_context *client = NULL;
    rtdb_cli_reply *reply = NULL;
    int status=-1;
    
    if(!c) return NULL;
	masterFlag = 0;
    retNode = (rtdb_server_node*)rtdb_cli_list_get_first((rtdb_cli_list*)c->clients);
    while (retNode)
    {
        status = rtdb_cli_check_node_status(retNode,retNode->basePort,1);
        
        if(0 == status)
        {
            if(retNode->ip1 && retNode->ip2)
                CMN_LOGGER_DEBUG(logger,"node(%s,%s),port(%d) open",retNode->ip1,retNode->ip2,retNode->basePort);
            portStatus = 1;
            // port is open
            client = rtdb_cli_connect(retNode->active_ip,retNode->basePort);
            if (!client) continue;
            CMN_LOGGER_DEBUG(logger,"get client ok(%s:%d)",retNode->active_ip,retNode->basePort);
            reply = rtdb_client_command(client,RTDB_CMD_CHECKMASTER);
            if(reply)
            {
                CMN_LOGGER_DEBUG(logger,"reply->type : %d",reply->type);
                if(reply->type == RTDB_REPLY_INTEGER)
                {
                    masterFlag = reply->integer;
                    CMN_LOGGER_DEBUG(logger,"reply->integer : %d",reply->integer);
                }
                free_reply_object(reply);
            }
            rtdb_cli_contex_free(client);
            if(masterFlag) break; 
        }
        else
        {
            if(retNode->ip1 && retNode->ip2)
                CMN_LOGGER_ERR(logger,"node(%s,%s),port(%d) not open",retNode->ip1,retNode->ip2,retNode->basePort);
        }
        retNode = (rtdb_server_node*)rtdb_cli_list_get_next((rtdb_cli_list*)c->clients);
    }
    
    if(0 == portStatus)
    {
        // all port closed , no active node 
        CMN_LOGGER_INFO(logger,"all port closed");
        c->last_error = RTDB_STATUS_NETERR;
        return NULL;
    }
    // check if there is no master, then select the first active one
    if (!retNode)
    {   
        CMN_LOGGER_INFO(logger,"get master fail,use the first active node!");
        retNode = (rtdb_server_node*)rtdb_cli_list_get_first((rtdb_cli_list*)c->clients);
        while (retNode)
        {
            if(retNode->active_ip) break;
            retNode = (rtdb_server_node*)rtdb_cli_list_get_next((rtdb_cli_list*)c->clients);
        }
	}
    CMN_LOGGER_INFO(logger,"master is (%s:%d)",retNode->active_ip,retNode->basePort);
    c->master = retNode;
    return retNode;
}

static char* rtdb_cli_gen_generic_path_str(const char* path)
{
    char *cmd; 
    const char *itemName;
    int cmdLen;  
    int8_t i, itemNum;
    rtdb_cli_path* spath;
    
    if (!path) return NULL;
    
    spath = rtdb_cli_path_parse(path);
    if (!spath)
    {
        CMN_LOGGER_WARN(logger,"parse path failed");
        return NULL;
    }

    itemNum = rtdb_cli_path_get_nodesize(spath);    
    cmdLen = strlen(path) + itemNum*2 + 3; 
    cmd = malloc(cmdLen);
    if (!cmd)
    {
		CMN_LOGGER_WARN(logger,"parse path failed");
		rtdb_cli_path_free(spath);
		return NULL;
	}	
    memset(cmd, 0, cmdLen);  
	
    for (i = 0; i < itemNum; i++)
    {
        itemName = crtdb_cli_path_get_node(spath, i);
        if (!itemName) continue;
        strcat(cmd," ");    
        strcat(cmd, itemName);
    }
   
	itemNum = rtdb_cli_path_get_fieldsize(spath);
	if (itemNum > 0)
	{
		strcat(cmd, " .");
		for (i = 0; i < itemNum; i++)
		{
			itemName = rtdb_cli_path_get_field(spath, i);
			if (!itemName) continue;
			strcat(cmd, " ");
			strcat(cmd, itemName);
		}
	}
    
    rtdb_cli_path_free(spath);
    //CMN_LOGGER_DEBUG(logger,"cmd : %s",cmd);
    return cmd;
}

static char* rtdb_cli_gen_query_node_generic_cmd(const char* cmdName,const char* path)
{
    char *cmd, *suffix;
    int cmdLen;
    
    if (!cmdName || !path) return NULL;    
    suffix = rtdb_cli_gen_generic_path_str(path);
    if(!suffix) return NULL;

    cmdLen = strlen(cmdName) + strlen(suffix) + 2; 
    cmd = malloc(cmdLen);
    if (!cmd)
    {
        free(suffix);
		return NULL;
	}	
    memset(cmd, 0, cmdLen);
    sprintf(cmd,"%s %s",cmdName,suffix);
    free(suffix);
    //CMN_LOGGER_DEBUG(logger,"cmd : %s",cmd);
    return cmd;
}

static char* rtdb_cli_gen_table_generic_cmd_prefix(
    const char* cmdName,int domainId,const char *tableName, const char *cond,int condFlag)
{
    char *retCmd = NULL,*pTmp;
    int curLen = 0;
    
    if(!cmdName || !tableName) 
        return NULL;
    curLen = strlen(cmdName) + 10 + strlen(tableName) + 30;
    if(cond) curLen += strlen(cond) ;
    retCmd = malloc(curLen + 1);
    if(!retCmd) return NULL;
    memset(retCmd,0,curLen + 1);
    sprintf(retCmd,"%s %d %s",cmdName,domainId,tableName);
    
    pTmp = retCmd + strlen(retCmd);
    if(cond)
    {
        sprintf(pTmp," \"%s\"",cond);
    }
    else
    {
        sprintf(pTmp,"   ");
        if(condFlag) sprintf(pTmp," ''");
    }
   // CMN_LOGGER_DEBUG(logger,"prefix : %s",retCmd);
    return retCmd;
}

static char* rtdb_cli_gen_query_table_generic_cmd(
    const char* cmdName,int domainId,const char *tableName,
    const char *cond, const rtdb_cli_list *filters)
{
    char *retCmd = NULL,*cmdPrefix = NULL;
    int curLen = 0 ,tlen = 0;
   
    if(!cmdName || !tableName) 
        return NULL;    
    cmdPrefix = rtdb_cli_gen_table_generic_cmd_prefix(cmdName,domainId,tableName,cond,1);
    if(!cmdPrefix) return NULL;    
    tlen = strlen(cmdPrefix) + 1;
    if(filters) 
        tlen += 64 * rtdb_cli_list_get_size((rtdb_cli_list*)filters);
    retCmd = calloc(tlen,1);
    if(!retCmd) 
    {
        free(cmdPrefix);
        return NULL;
    }    
    snprintf(retCmd,tlen,"%s",cmdPrefix);
    free(cmdPrefix);
    
    if(filters)
    {
        char *pFiedName = NULL;
        pFiedName = rtdb_cli_list_get_first((rtdb_cli_list*)filters);
        while(pFiedName != NULL)
        {  
            curLen = strlen(retCmd);
            //CMN_LOGGER_DEBUG(logger,"pFiedName : %s",pFiedName); 
            snprintf(retCmd + curLen,tlen - curLen," %s",pFiedName);
            pFiedName = rtdb_cli_list_get_next((rtdb_cli_list*)filters);
        }    
    } 
    CMN_LOGGER_DEBUG(logger,"retCmd : %s",retCmd);
    return retCmd;
}

static char* rtdb_cli_gen_update_table_generic_cmd(
    const char* cmdName,int domainId,const char *tableName,
    const char *cond,int condFlag,const rtdb_cli_record *record)
{
    char *retCmd,*cmdPrefix;
    int curLen = 0,tLen = 0;
    int i = 0 , isError = 0;
    int sz = 0;
    const char *pKey = NULL;
    char *pValue = NULL;
    const rtdb_cli_variant *v = NULL;

    if(!cmdName || !tableName || !record) 
        return NULL;
    sz = rtdb_cli_record_getsize(record);
    if(!sz) return NULL;
    cmdPrefix = rtdb_cli_gen_table_generic_cmd_prefix(cmdName,domainId,tableName,cond,condFlag);
    if(!cmdPrefix) return NULL;    
    
    tLen = strlen(cmdPrefix) + 1 + sz * 128;
    retCmd = calloc(tLen,1);
    if(!retCmd) 
    {
        free(cmdPrefix);
        return NULL;
    }
    snprintf(retCmd,tLen,"%s",cmdPrefix);
    free(cmdPrefix);
    
    for(i = 0; i < sz; i++)
    {
        pKey = rtdb_cli_record_getfieldName(record,i);
        if(!pKey) 
        {
            isError = 1;
            break;
        }
        v = rtdb_cli_record_getvalue(record,pKey);
        if(!v)
        {
            isError = 1;
            break;
        }
        pValue = rtdb_cli_variant_tostring(v);
        if(!pValue)
        {
            isError = 1;
            break;
        }
        curLen = strlen(retCmd);
        snprintf(retCmd + curLen,tLen - curLen," %s %s",pKey,pValue);
        free(pValue);
    }
    if(isError)
    {
        free(retCmd);
        return NULL;
    }
    CMN_LOGGER_DEBUG(logger,"retCmd : %s",retCmd);
    return retCmd;
}

static rtdb_cli_variant* rtdb_cli_reply_tovariant(const rtdb_cli_reply **element)
{
    rtdb_cli_variant* retValue = rtdb_cli_variant_create();
    
    if (!retValue) return NULL;    
    retValue->type = element[0]->integer;
    switch(retValue->type)
    {
        case RTDB_CLI_VARIANT_BOOL :
        case RTDB_CLI_VARIANT_INT8 :
        case RTDB_CLI_VARIANT_INT16 :
        case RTDB_CLI_VARIANT_INT32 :
        case RTDB_CLI_VARIANT_INT64 :
            retValue->v.integer = element[1]->integer;
            break;
        case RTDB_CLI_VARIANT_FLOAT :
        case RTDB_CLI_VARIANT_DOUBLE :
            retValue->v.real = atof(element[1]->str);
            break;
        case RTDB_CLI_VARIANT_STRING :
            retValue->v.string = strdup(element[1]->str);
            break;
        default :
            break;
    }
    return retValue;
}

static rtdb_cli_context* rtdb_cli_get_baseclient(rtdb_cli *c)
{
    rtdb_server_node *masterNode = NULL;
    rtdb_cli_context *client = NULL;
    int i = 0;
    
    if(!c) return NULL; 
    while(i++ < RTDB_CLI_MAX_RETRY)
    {
        if(!c->master)
            rtdb_cli_get_master(c);
        masterNode = c->master;
        if(masterNode)
        {
            if(masterNode->baseContext && (masterNode->baseContext->fd < 0))
            {
                CMN_LOGGER_DEBUG(logger,"masterNode->baseContext->fd : %d",masterNode->baseContext->fd);
                rtdb_cli_contex_free(masterNode->baseContext);
                masterNode->baseContext = NULL;
            }
            if(!masterNode->baseContext)
            {
                client = rtdb_cli_connect(masterNode->active_ip,masterNode->basePort);
                if(!client)
                {
                    CMN_LOGGER_DEBUG(logger,"rtdb_cli_get_baseclient : client is NULL");
                    c->last_error = RTDB_STATUS_NETERR;
                    c->master = NULL;
                }
                masterNode->baseContext = client;
            }
            client = masterNode->baseContext;
        }
        else
        {
            c->last_error = RTDB_STATUS_NETERR;
        }
        if(client && client->fd > 0) break;
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_get_baseclient retry count : %d",i);
        usleep(500000);
    }
    return client;
}

static rtdb_cli_context* rtdb_cli_get_modeclient(rtdb_cli *c)
{
    rtdb_server_node *masterNode = NULL;
    rtdb_cli_context *client = NULL;
    int i = 0,status = 0;
	char buf[1024];
    
    if(!c) return NULL; 
    while(i++ < RTDB_CLI_MAX_RETRY)
    {
        if(!c->master)
            rtdb_cli_get_master(c);
        masterNode = c->master;
        if(masterNode)
        {
            if(masterNode->modeContext && (masterNode->modeContext->fd < 0))
            {
                CMN_LOGGER_DEBUG(logger,"masterNode->modeContext->fd : %d",masterNode->modeContext->fd);
                rtdb_cli_contex_free(masterNode->modeContext);
                masterNode->modeContext = NULL;
            }
            if(!masterNode->modeContext)
            {
				status = rtdb_cli_check_node_status(masterNode, masterNode->modePort, 1);
				if (status == 0)
				{
					client = rtdb_cli_connect(masterNode->active_ip, masterNode->modePort);
					if (!client)
					{
						CMN_LOGGER_ERR(logger, "rtdb_cli_get_modeclient : client is NULL");
						c->master = NULL;
					}
					CMN_LOGGER_DEBUG(logger, "rtdb_cli_get_modeclient(%s:%d) ok", masterNode->active_ip, masterNode->modePort);
					masterNode->modeContext = client;
				}
				else
				{
					snprintf(buf, sizeof(buf),"connect (%s,%s):%d fail!", 
						masterNode->ip1, masterNode->ip2, masterNode->modePort);
					CMN_LOGGER_ERR(logger, buf);
					if (i == RTDB_CLI_MAX_RETRY - 1)
					{
						fprintf(stderr,"%s\n",buf);
					}
				}
            }
            client = masterNode->modeContext;
        }
        
        if(client && client->fd > 0) break;
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_get_modeclient retry count : %d",i);
        usleep(500000);
    }
    return client;
}


static rtdb_cli_reply* rtdb_cli_do_read_query(rtdb_cli *c,const char *queryString)
{
    rtdb_cli_context *client = NULL;
    rtdb_cli_reply *reply = NULL;
    int i = 0;
    
    if(!c || !queryString)
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_do_read_query : c or queryString is NULL");
        return NULL;
    }    
    
    while(i++ < RTDB_CLI_MAX_RETRY)
    {
        client = rtdb_cli_get_baseclient(c);
        if(!client)
        {
            c->last_error = RTDB_STATUS_OTHERERR;
            CMN_LOGGER_DEBUG(logger,"client is NULL\n");
        }
        else
        {
            reply = rtdb_client_command(client,queryString);
        }
        if(reply) break;
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_do_read_query retry count : %d",i);
        usleep(10000);
    }
    return reply;
}

static int rtdb_cli_do_write_query(rtdb_cli *c,const char *queryString)
{
    int ret = 0,i = 0;
    rtdb_cli_context *client = NULL;
    rtdb_cli_reply *reply = NULL;

    if(!c || !queryString)
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        return -1;
    }
    //CMN_LOGGER_DEBUG(logger,"queryString : %s\n",queryString);
    while(i++ < RTDB_CLI_MAX_RETRY)
    {
        client = rtdb_cli_get_baseclient(c);
        if(!client)
        {
            c->last_error = RTDB_STATUS_OTHERERR;
            CMN_LOGGER_DEBUG(logger,"client is NULL");
        } 
        else
        {
            reply = rtdb_client_command(client,queryString);
        }
        if(reply) break;
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_do_write_query retry count : %d ",i);        
        usleep(10000);
    }

    if(!reply || (reply->type == RTDB_REPLY_ERROR))
    {
        c->last_error = RTDB_STATUS_SERVERERR;
        CMN_LOGGER_ERR(logger,"rtdb_cli_do_write_query reply error");
        ret = -1;
    }
    if(reply) free_reply_object(reply);
    return ret;
}

static int rtdb_cli_reset_lasterror(rtdb_cli* c)
{
    if(!c) return -1;
    c->last_error = RTDB_STATUS_OK;
    return 0;
}

static rtdb_cli_variant* rtdb_cli_unique_query_attribute_base(const char* cmdName,rtdb_cli *c,const char *path)
{
    rtdb_cli_variant* retValue = NULL;
    rtdb_cli_reply *reply = NULL;
    char *pStrCmd = NULL;

    if(!c || !cmdName || !path) 
        return NULL;
    rtdb_cli_reset_lasterror(c);
    pStrCmd = rtdb_cli_gen_query_node_generic_cmd(cmdName,path); 
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    reply = rtdb_cli_do_read_query(c,pStrCmd);
    free(pStrCmd);
    if(!reply || (reply->type != RTDB_REPLY_ARRAY) || (reply->elements != 2))
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_unique_query_attribute_base reply error");
        if(reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_unique_query_attribute_base reply ok");
    retValue = rtdb_cli_reply_tovariant((const rtdb_cli_reply **)&reply->element[0]);
    free_reply_object(reply);
    return retValue;
}

static rtdb_cli_record* rtdb_cli_multi_query_attributes_base(const char* cmdName,rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	int pathLen = 0,sz = 0,isError = 0;
	char *cmdPrefix;  
	char *pStrCmd;  
    rtdb_cli_record *record;
    rtdb_cli_reply *reply;
    
    if(!cmdName || !c || !path || !names )
    {
		CMN_LOGGER_ERR(logger,"rtdb_cli_multi_query_attributes_base error");
		return NULL;
	}
    pathLen = strlen(path);
    sz = rtdb_cli_list_get_size((rtdb_cli_list*)names);
    if(!pathLen || !sz) 
    {
        CMN_LOGGER_ERR(logger,"pathLen or sz is zero!");
        return NULL;
    }
    rtdb_cli_reset_lasterror(c);
    record = rtdb_cli_record_create(sz);
    if (!record) return NULL;
    
    cmdPrefix = rtdb_cli_gen_query_node_generic_cmd(cmdName,path); 
    if (!cmdPrefix)
    {
        rtdb_cli_record_free(record);
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }    
    pStrCmd = rtdb_cli_multi_query_attr_fill_cmd(cmdPrefix,names);
    free(cmdPrefix);
    if(!pStrCmd)
    {
        rtdb_cli_record_free(record);
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    CMN_LOGGER_DEBUG(logger,"pStrCmd : %s",pStrCmd); 
    reply = rtdb_cli_do_read_query(c,pStrCmd);    
    free(pStrCmd); 
    if (!reply || reply->type != RTDB_REPLY_ARRAY || (reply->elements%3 != 0))
    {
        CMN_LOGGER_DEBUG(logger,"reply error");
        if (reply) free_reply_object(reply);
        rtdb_cli_record_free(record);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }    
    isError = rtdb_cli_fill_record_use_reply3(
            record,reply->elements,(const rtdb_cli_reply **)reply->element);
    if(isError)
    {
        rtdb_cli_record_free(record);
        c->last_error = RTDB_STATUS_OTHERERR;
        record = NULL;
	} 
    free_reply_object(reply);
    return record;
}

static char* rtdb_cli_combine_cmd(const char *cmd,const char *attrBuf,const char *refAttrBuf)
{
    char *retCmd = NULL;
    int newLen = 0,attrLen = 0,refAttrLen = 0;
    
    if(!cmd) return NULL;
    if(attrBuf && strlen(attrBuf)) attrLen = strlen(attrBuf);
    if(refAttrBuf && strlen(refAttrBuf)) refAttrLen = strlen(refAttrBuf);
    
    newLen = strlen(cmd) + attrLen + refAttrLen + 32;    
    retCmd = malloc(newLen);
    if(!retCmd)
        return NULL;  
    memset(retCmd,0,newLen);
    sprintf(retCmd,"%s ",cmd);   
    if(attrBuf && strlen(attrBuf))
        sprintf(retCmd + strlen(retCmd)," . %s ",attrBuf);
    if(refAttrBuf && strlen(refAttrBuf))
        sprintf(retCmd + strlen(retCmd)," .. %s ",refAttrBuf);
    return retCmd;
}

static char* rtdb_cli_multi_query_attr_fill_cmd(const char *cmd,const rtdb_cli_list *names)
{
    char *retCmd = NULL;
    int bufLen = 0,attrLen = 0,refAttrLen = 0;
    char *attrBuf = NULL, *refAttrBuf = NULL,*name;
    int iName,iNameLen;
    char *newName = NULL;
    
    if(!cmd || !names) return NULL;
    bufLen = 64 * rtdb_cli_list_get_size((rtdb_cli_list*)names);
    attrBuf = calloc(bufLen,1);
    if(!attrBuf) return NULL;
    refAttrBuf = calloc(bufLen,1);
    if(!refAttrBuf)
    {
        free(attrBuf);
        return NULL;
    }
    
    name = (char*)rtdb_cli_list_get_first((rtdb_cli_list*)names);
    while (name)
    {   
        //CMN_LOGGER_DEBUG(logger,"name : %s",name);
        iNameLen = strlen(name);
        for(iName = 0; iName < iNameLen; iName++)
        {
            if(name[iName] == '.') break;
        }
        if(iName == iNameLen) 
        {
            // have no dot
            attrLen = strlen(attrBuf);
            snprintf(attrBuf + attrLen,bufLen - attrLen," %s ",name);
        }
        else
        {
            // have dot
            refAttrLen = strlen(refAttrBuf);
            newName = strdup(name);
            if(!newName) 
            {
                free(attrBuf);
                free(refAttrBuf); 
                return NULL;
            }
            newName[iName] = '\0';
            snprintf(refAttrBuf + refAttrLen,bufLen - attrLen," %s %s ",newName,newName+iName+1);
            free(newName);
        }
        name = (char*)rtdb_cli_list_get_next((rtdb_cli_list*)names);
    }

    retCmd = rtdb_cli_combine_cmd(cmd,attrBuf,refAttrBuf);
    if(retCmd)
        CMN_LOGGER_DEBUG(logger,"retCmd : %s",retCmd);
    free(attrBuf);
    free(refAttrBuf);    
    return retCmd;
}

static int rtdb_cli_fill_record_use_reply3(rtdb_cli_record *record,
    int elementNum,const rtdb_cli_reply **element)
{
    int isError = 0;
    int i,iRetTmp;
    char *name;
    rtdb_cli_variant *v;
        
    if(!record || !element) return 1;
    for (i = 0; i < elementNum; i += 3) 
    {   
        name = element[i]->str;
        if(!name)
        {
            isError = 1;
            break;
        }
        v = rtdb_cli_reply_tovariant((const rtdb_cli_reply **)&element[i+1]);
        if (!v)
        {
            isError = 1;
            break;
        }        
        iRetTmp = rtdb_cli_record_setvalue(record, name, v);
        if (iRetTmp == -1)
        {
			rtdb_cli_variant_free(v);
			isError = 1;
			break;
		}
    }
    return isError;    
}

static char* rtdb_cli_gen_mquery_nodes_cmd_prefix(const char* cmdName,const rtdb_cli_list *paths)
{
    int isError = 0;
    char *path = NULL,*suffix = NULL;
    char *cmd = NULL;
    int cmdLen = 0;
    
    if(!paths) return NULL;
    cmdLen += 1024 + rtdb_cli_list_get_size((rtdb_cli_list*)paths) * 64;
    cmd = malloc(cmdLen);
    if(!cmd) return NULL;
    memset(cmd,0,cmdLen);
    sprintf(cmd,"%s",cmdName);
    path = rtdb_cli_list_get_first((rtdb_cli_list*)paths);
    while(path != NULL)
    {
        CMN_LOGGER_DEBUG(logger,"path : %s",path);
        suffix = rtdb_cli_gen_generic_path_str(path);
        if(!suffix) 
        {
            isError = 1;
            break;
        }
        sprintf(cmd + strlen(cmd)," %s",suffix);
        free(suffix);
        path = (char*)rtdb_cli_list_get_next((rtdb_cli_list*)paths);
        if(path) sprintf(cmd + strlen(cmd)," |");
    }
    if(isError)
    {
        free(cmd);
        return NULL;
    }
    return cmd;
}

static rtdb_cli_byte* rtdb_cli_read_bytes(rtdb_cli_context *client,int *len)
{
    int nread,msgLen;
    rtdb_cli_byte *bytes;
    
	nread = msgLen = *len = 0;
	if (!client)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_read_bytes : client is NULL");
		return NULL;
	}
    nread = network_read(client->fd, &msgLen, 4);
	if (nread != 4)
	{
		//CMN_LOGGER_ERR(logger, "rtdb_cli_read_bytes : nread(%d) != 4",nread);
		return NULL;
	}
    *len = rtdb_cli_ntohi32(msgLen);
    if (*len <= 0 || *len > 16*1024)
    {
        CMN_LOGGER_ERR(logger,"len <= 0 || len > 16*1024");
		*len = 0;
        return NULL;
    }
    bytes = calloc(*len + 4,1);
	if (!bytes)
	{
		*len = 0;
		return NULL;
	}
    memcpy(bytes,&msgLen,4);
    nread = network_read(client->fd, bytes+4,*len);
	if (nread != *len)
	{
		CMN_LOGGER_ERR(logger, "nread(%d) != %d", nread, *len);
		free(bytes);
		*len = 0;
        return NULL;
	}
	*len += 4;
    return bytes;
}

/*
* 一个订阅对象里面只有一个命令
* 0 : success , -1 : fail
*/
static int rtdb_cli_cmdpub_notify(const rtdb_cli_cmdpub *pub)
{
	const void *userData;
	int domainId;
	int type;
	const char* destination;
	int resultCode;
	const char *result;
	rtdb_cli_subscriber *pSub = NULL;
	rtdb_cli_msg_rdbcmd *rdbcmd;
	rtdb_cli_cmdResult_notifier *resultNotifier;

	pSub = (rtdb_cli_subscriber*)rtdb_cli_list_get_first((rtdb_cli_list*)gNotifyList);
	while (pSub)
	{
		if (0 == strncmp(pSub->uuid, pub->id, sizeof(rtdb_cli_uuid)))
			break;
		pSub = (rtdb_cli_subscriber*)rtdb_cli_list_get_next((rtdb_cli_list*)gNotifyList);
	}
	if (!pSub || !pSub->cmdNotifier ) return -1;

	rdbcmd = pSub->cmdNotifier->rdbcmd;
	resultNotifier = pSub->cmdNotifier->resultNotifier;
	if (!rdbcmd || !resultNotifier) return -1;

	userData = pSub->cmdNotifier->userData;
	domainId = rdbcmd->domainId;
	type = rdbcmd->type;
	destination = rdbcmd->destination;

	if (pub->num < 1) return -1;
	resultCode = pub->items[0].code;
	result = pub->items[0].result;

	resultNotifier(userData, domainId, type, destination, resultCode, result);
	rtdb_cli_free_subscriber(pSub);	
    return 0;
}

static int rtdb_cli_tblpub_notify(const rtdb_cli_tblpub *pub)
{
	rtdb_cli_subscriber *pSub = NULL;
	int code,i,j,isErr = 0,pos = 0 ;
	const char *tableName = NULL;
	int type;
	rtdb_cli_record *r = NULL;
	rtdb_cli_variant *v = NULL;
	rtdb_cli_list *records = NULL;
	rtdb_cli_tblpub_item *item = NULL; 

	CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify begin");
	if (!pub || !pub->rNum || !pub->fNum) return -1;

	pSub = (rtdb_cli_subscriber*)rtdb_cli_list_get_first((rtdb_cli_list*)gNotifyList);
	while(pSub)
	{
		if (0 == strncmp(pSub->uuid, pub->id, sizeof(rtdb_cli_uuid)))
			break;
		pSub = (rtdb_cli_subscriber*)rtdb_cli_list_get_next((rtdb_cli_list*)gNotifyList);
	}
	if (!pSub) return -1;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify : find it!");
	tableName = pub->tName;
	type = pub->type;
	records = rtdb_cli_list_create(rtdb_cli_record_free, NULL);
	if (!records)
	{
		if (pSub->expectionNotifier) pSub->expectionNotifier(-1);
		return -1;
	}
	for (i = 0; i < pub->rNum; i++)
	{
		r = rtdb_cli_record_create(pub->fNum);
		if (!r)
		{
			isErr = 1;
			break;
		}
		for (j = 0; j < pub->fNum; j++)
		{
			pos = i*pub->fNum + j;
			item = &pub->items[pos];
			if (!item || !item->vt)
			{
				CMN_LOGGER_ERR(logger, "rtdb_cli_tblpub_notify : item or item->vt is NULL!");
				isErr = 1;
				break;
			}
			//CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify : item->vt->type = %d!", item->vt->type);
			v = rtdb_cli_variant_dup(item->vt);
			if (!v || !item->fName)
			{
				CMN_LOGGER_ERR(logger, "rtdb_cli_tblpub_notify : v or item->fName  is NULL!");
				isErr = 1;
				break;
			}
			//CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify , item->fName : %s", (const char*)item->fName);
			code = rtdb_cli_record_setvalue(r, (const char*)item->fName, v);
			if (code != 0)
			{
				CMN_LOGGER_ERR(logger, "rtdb_cli_tblpub_notify : rtdb_cli_record_setvalue fail!");
				isErr = 1;
				break;
			}
		}
		if (isErr)
		{
			rtdb_cli_record_free(r);
			break;
		}
		rtdb_cli_list_push_tail(records,r);
	}
	if (isErr)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_tblpub_notify : error occur!");
		rtdb_cli_list_free(records);
		if (pSub->expectionNotifier) pSub->expectionNotifier(-1);
		return -1;
	}
	code = 0;
	if (pSub->tableNotifier)
		pSub->tableNotifier(tableName, type, records);
	else
		code = -1;
	if (pSub->expectionNotifier)
		pSub->expectionNotifier(code);
	rtdb_cli_list_free(records);
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify end");
	return code;
}

static int rtdb_cli_nodepub_notify(const rtdb_cli_nodepub *pub)
{
	rtdb_cli_subscriber *pSub = NULL;
	int i,isErr = 0,code = 0;
	const char *nodePath;
	rtdb_cli_record *r;
	rtdb_cli_variant *v;
	rtdb_cli_nodepub_item *item;

	if (!pub || !pub->items ) return -1;
	pSub = (rtdb_cli_subscriber*)rtdb_cli_list_get_first((rtdb_cli_list*)gNotifyList);
	while (pSub)
	{
		if (0 == strncmp(pSub->uuid, pub->id, sizeof(rtdb_cli_uuid)))
			break;
		pSub = (rtdb_cli_subscriber*)rtdb_cli_list_get_next((rtdb_cli_list*)gNotifyList);
	}
	if (!pSub) return -1;
	nodePath = pub->path;
	r = rtdb_cli_record_create(pub->aNum);
	if (!r)
	{
		if (pSub->expectionNotifier) pSub->expectionNotifier(-1);
		return -1;
	}
	for (i = 0; i < pub->aNum; i++)
	{
		item = &pub->items[i];
		v = rtdb_cli_variant_dup(item->vt);
		if (!v)
		{
			isErr = 1;
			break;
		}
		if (rtdb_cli_record_setvalue(r, item->aName, v) != 0)
		{
			rtdb_cli_variant_free(v);
			isErr = 1;
			break;
		}
	}

	if (isErr)
	{
		rtdb_cli_record_free(r);
		if (pSub->expectionNotifier) pSub->expectionNotifier(-1);
		return -1;
	}
	code = 0;
	if (pSub->nodeNotifier)
		pSub->nodeNotifier(nodePath, r);
	else
		code = -1;
	if (pSub->expectionNotifier) pSub->expectionNotifier(code);
	rtdb_cli_record_free(r);
	return code;
}

static void rtdb_cli_do_notify(rtdb_cli_context *client)
{
	rtdb_cli_byte *bytes = NULL;
	rtdb_cli_msg_header *header = NULL;
	int msgLen = 0;
	rtdb_cli_cmdpub *cmdpub = NULL;
	rtdb_cli_tblpub *tblpub = NULL;
	rtdb_cli_nodepub *nodepub = NULL;

	bytes = rtdb_cli_read_bytes(client, &msgLen);
	if (!bytes) return;
	if (!msgLen)
	{
		free(bytes);
		return;
	}
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_notify , msgLen = %d",msgLen);
	header = rtdb_cli_decode_msg_header(bytes, RTDB_CLI_MSGHEADERLEN);
    if(!header)
    {
        CMN_LOGGER_ERR(logger, "rtdb_cli_do_notify :  header is NULL!");
		free(bytes);
        return ;
    }

	switch (header->msgType)
	{
	case RTDB_CLI_PUB_RDBCMD:
		cmdpub = rtdb_cli_parse_cmdpub(bytes, msgLen);
		if (cmdpub)
		{
			rtdb_cli_cmdpub_notify(cmdpub);
			rtdb_cli_free_cmdpub(cmdpub);
		}
		break;
	case RTDB_CLI_PUB_RDBTBL:
		tblpub = rtdb_cli_parse_tblpub(bytes, msgLen);
		if (tblpub)
		{
			rtdb_cli_tblpub_notify(tblpub);
			rtdb_cli_free_tblpub(tblpub);
		}		
		break;
	case RTDB_CLI_PUB_RDBNODE:
		nodepub = rtdb_cli_parse_nodepub(bytes, msgLen);
		if (nodepub)
		{
			rtdb_cli_nodepub_notify(nodepub);
			rtdb_cli_free_nodepub(nodepub);
		}
		break;
	default:
		break;
	}
	free(bytes);
	rtdb_cli_free_msg_header(header);
}

static void* rtdb_cli_notify_thread(void *param)
{
	// ignore last_error
	rtdb_cli *c = (rtdb_cli*)param;
	rtdb_cli_context *client = NULL;

	g_sub_notify_status = 1;
	pthread_cleanup_push(rtdb_cli_thread_clean_fun,c);

	CMN_LOGGER_INFO(logger, "rtdb_cli_notify_thread start!\n");
	while(1)
	{
		// check connect and do reconnect
		client = rtdb_cli_get_modeclient(c);
		if (!client) continue;
		// recvdata and notify
		rtdb_cli_do_notify(client);
		if (rtdb_cli_list_get_size(gNotifyList) == 0) break;
		usleep(1000);
	}

	pthread_cleanup_pop(0);
}

static rtdb_cli_list* rtdb_cli_get_single_node_key_list(const char* cmdName,rtdb_cli *c,const char *path)
{
    // subNodes 
    rtdb_cli_list *retList = NULL;
    rtdb_cli_reply *reply = NULL;
    char *pStrCmd = NULL;
    int i,isError = 0;
    
    if(!cmdName || !c || !path)
    {
        CMN_LOGGER_DEBUG(logger,"path is NULL");
        return NULL;
    }
    rtdb_cli_reset_lasterror(c);
    CMN_LOGGER_DEBUG(logger,"path : %s",path);
    pStrCmd = rtdb_cli_gen_query_node_generic_cmd(cmdName,path); 
    if(!pStrCmd)
    {
        CMN_LOGGER_ERR(logger,"pStrCmd is NULL");
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }    
    reply = rtdb_cli_do_read_query(c,pStrCmd);
    free(pStrCmd);
    
    if (!reply || reply->type != RTDB_REPLY_ARRAY )
    {
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_get_single_node_key_list reply error");
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }    
    
    retList = rtdb_cli_list_create(free,NULL);
    if(!retList)
    {
        free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    for (i = 0; i < (int)reply->elements; i++) 
    {
        char *pvalue = NULL;            
        if(reply->element[i]->type == RTDB_REPLY_STRING)
        {
            pvalue = strdup(reply->element[i]->str);
        }
        if(pvalue)
            rtdb_cli_list_push_tail(retList,pvalue);
        else 
        {
            isError = 1;
            break;
        }   
    }
    if(isError)
    {
        rtdb_cli_list_free(retList);
        c->last_error = RTDB_STATUS_OTHERERR;
        retList = NULL;
    }    
    free_reply_object(reply);
    return retList;
}

static rtdb_cli_list* rtdb_cli_query_table_base(rtdb_cli *c,int domainId,const char *tblName,const char *cond,const rtdb_cli_list *filters)
{
    // a record is also a rtdb_cli_list with element of rtdb_cli_record type
    rtdb_cli_list *retList = NULL;
    rtdb_cli_record *pRecord = NULL;
    rtdb_cli_reply *reply = NULL;
    char *pStrCmd = NULL;
    rtdb_cli_reply **element;
    int j;
    int recordsCount = 0,fieldsCount = 0 ;
    int totalElemnts = 0;
    int recordStep = 0; 

    if(!c) return NULL;
    rtdb_cli_reset_lasterror(c);
    if(!tblName)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }

    pStrCmd = rtdb_cli_gen_query_table_generic_cmd(RTDB_CMD_QUERYTABLE,domainId,tblName,cond,filters);
    if(!pStrCmd)
    {
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    reply = rtdb_cli_do_read_query(c,pStrCmd);
    free(pStrCmd);
    if (!reply || reply->type != RTDB_REPLY_ARRAY || reply->elements < 2)
    {
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_query_table_base reply error");
        if (reply) free_reply_object(reply);
        c->last_error = RTDB_STATUS_SERVERERR;
        return NULL;
    }        

    recordsCount = reply->element[0]->integer;
    fieldsCount = reply->element[1]->integer;
    if(!recordsCount || !fieldsCount) 
    {
       // c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;  
    }
    totalElemnts = 2 + recordsCount * fieldsCount * 3;
    
    CMN_LOGGER_DEBUG(logger,"recordsCount=%d,fieldsCount=%d,totalElemnts=%d",
        recordsCount,fieldsCount,totalElemnts);
    
    if((int)reply->elements != totalElemnts)
    {
        CMN_LOGGER_WARN(logger,"reply->elements != totalElemnts ");
        free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }
    retList = rtdb_cli_list_create(rtdb_cli_record_free,NULL);
    if(!retList) 
    {
        free_reply_object(reply);
        c->last_error = RTDB_STATUS_OTHERERR;
        return NULL;
    }    
    element = &reply->element[2];
    recordStep = fieldsCount*3;
    CMN_LOGGER_DEBUG(logger,"recordStep : %d",recordStep);

    for(j = 0; j < (int)reply->elements - 2 ; j += recordStep)
    {
        int isError = 0;
        pRecord = rtdb_cli_record_create(fieldsCount);
        if(!pRecord)
        {
            free_reply_object(reply);
            c->last_error = RTDB_STATUS_OTHERERR;
            return NULL;    
        }
        isError = rtdb_cli_fill_record_use_reply3(
                pRecord,recordStep,(const rtdb_cli_reply **)element + j);
        if(isError)    
        {
            rtdb_cli_record_free(pRecord);  
            free_reply_object(reply);
            c->last_error = RTDB_STATUS_OTHERERR;
            return NULL;
        }
        rtdb_cli_list_push_tail(retList,pRecord);
    }    
    free_reply_object(reply);
    return retList;
}

static void rtdb_cli_thread_clean_fun(void *arg)
{
    rtdb_cli *c = NULL;
    CMN_LOGGER_INFO(logger,"rtdb_cli_thread_clean_fun\n");
    g_sub_notify_status = 0;
    c = (rtdb_cli*)arg;
    if(c)
    {
        rtdb_cli_free(c);
    }    
}

static char* rtdb_cli_get_uuid_str()
{
    char buf[64] = {0};
#ifdef WIN32
    GUID guid;
    CoCreateGuid(&guid);
    snprintf(buf,sizeof(buf),
         "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1],
        guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);    
#else
    uuid_t uu;
    uuid_generate(uu);
    uuid_unparse(uu,buf);
#endif
    return (char*)strdup(buf);
}

/*
* 一个订阅对象里面只有一个命令
* 0 : success , -1 : fail
*/
static int rtdb_cli_do_cmd_subscribe(rtdb_cli *cli,const char *uuid)
{
	rtdb_cli_cmdsub *cmdsub;
	rtdb_cli_byte *bytes;
	int dumpLen, nwritten;
	int msgLen = 0;
	rtdb_cli_byte *response;
	rtdb_cli_context *client;
	int flag = 1;

	CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_cmd_subscribe begin");
	if (!cli || !cli->master || !cli->master->modeContext || !uuid)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_do_cmd_subscribe : error");
		return -1;
	}
    client = cli->master->modeContext;
	cmdsub = rtdb_cli_create_cmdsub();
	if (!cmdsub)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_do_cmd_subscribe : cmdsub is NULL", flag);
		return -1;
	}
	memcpy(cmdsub->id, uuid, sizeof(cmdsub->id));
	cmdsub->num = 1;
	cmdsub->noArr = calloc(1, sizeof(int8_t));
	if (!cmdsub->noArr)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_do_cmd_subscribe error");
		rtdb_cli_free_cmdsub(cmdsub);
		return -1;
	}
	cmdsub->cmdidArr = calloc(1,sizeof(rtdb_cli_uuid));
	if (!cmdsub->cmdidArr)
	{
		CMN_LOGGER_ERR(logger,"rtdb_cli_do_cmd_subscribe error");
		rtdb_cli_free_cmdsub(cmdsub);
		return -1;
	}
	memcpy(cmdsub->cmdidArr[0], uuid, sizeof(rtdb_cli_uuid));

	bytes = rtdb_cli_dump_cmdsub(cmdsub,&dumpLen);
	if (!bytes)
	{
		CMN_LOGGER_ERR(logger,"rtdb_cli_do_cmd_subscribe : rtdb_cli_dump_cmdsub error");
		rtdb_cli_free_cmdsub(cmdsub);
		return -1;
	}
    CMN_LOGGER_DEBUG(logger, "rtdb_cli_dump_cmdsub ok");

    nwritten = network_write(client->fd,bytes,dumpLen);
    free(bytes);
    if(nwritten != dumpLen)
    {
        CMN_LOGGER_ERR(logger, "rtdb_cli_do_cmd_subscribe : network_write error");
		rtdb_cli_free_cmdsub(cmdsub);
		return -1;
    }
    CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_cmd_subscribe : network_write ok");
    
	response = rtdb_cli_read_bytes(client,&msgLen);
	if (!response)
	{
		CMN_LOGGER_ERR(logger,"rtdb_cli_do_cmd_subscribe : response is NULL");
		rtdb_cli_free_cmdsub(cmdsub);
		return -1;
	}

	flag = rtdb_cli_check_response(response, msgLen, cmdsub->num);
	free(response);
	if (!flag)
	{
		CMN_LOGGER_ERR(logger, "flag = %d", flag);
		return -1;
	}
    CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_cmd_subscribe end");
	return 0;
}

static int rtdb_cli_check_response(const rtdb_cli_byte *response, int msgLen,int subNum)
{
	/* 0 : false ; 1 : true */
	int i = 0,flag = 1;
	rtdb_cli_subrep *subrep = NULL;

	subrep = rtdb_cli_parse_subrep(response, msgLen);
	if (!subrep)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_do_tbl_subcribe : rtdb_cli_parse_subrep error");
		return 0;
	}
	if (subrep->num <= 0 || subrep->num != subNum)
	{
		rtdb_cli_free_tblsub(subrep);
		CMN_LOGGER_ERR(logger, "subrep->num(%d) != subNum(%d)", subrep->num, subNum);
		return 0;
	}
	for (i = 0; i < subrep->num; i++)
	{
		flag = flag && subrep->resultArr[i];
	}
	rtdb_cli_free_subrep(subrep);
	return flag;
}

static int rtdb_cli_do_tbl_subcribe(rtdb_cli *cli, const rtdb_cli_tblsub *tblsub)
{
	// 0 : ok , -1 : error
	rtdb_cli_byte *bytes;
	int dumpLen,nwritten;
	int msgLen = 0,flag = 0;
	rtdb_cli_byte *response = NULL;
	rtdb_cli_context *client = NULL;

	CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_tbl_subcribe start");

	if (!cli || !cli->master || !cli->master->modeContext || !tblsub)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_do_tbl_subcribe : error occur");
		return -1;
	}

	client = cli->master->modeContext;

	bytes = rtdb_cli_dump_tblsub(tblsub, &dumpLen);
	if (!bytes)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_do_tbl_subcribe : rtdb_cli_dump_tblsub error");
		return -1;
	}
    
    nwritten = network_write(client->fd,bytes,dumpLen);
    free(bytes);
    if(nwritten != dumpLen)
    {
		CMN_LOGGER_ERR(logger, "network_write error");
		return -1;        
    }        

	response = rtdb_cli_read_bytes(client, &msgLen);
	if (!response)
	{
		CMN_LOGGER_ERR(logger, "response is NULL");
		return -1;
	}
	// dispose response 
	flag = rtdb_cli_check_response(response, msgLen, tblsub->num);
	free(response);
	return 0; // for test
	if (!flag)
	{
		CMN_LOGGER_ERR(logger, "flag = %d",flag);
		return -1;
	}
	return 0;
}

static int rtdb_cli_do_node_subcribe(rtdb_cli *cli,const rtdb_cli_nodesub *nodesub)
{
	// 0 : ok , -1 : error
	rtdb_cli_byte *bytes = NULL;
	int dumpLen,nwritten;
	int msgLen = 0, flag = 1;
	rtdb_cli_byte *response = NULL;
	rtdb_cli_context *client = NULL;

	CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_node_subcribe start");
	if (!cli || !cli->master || !cli->master->modeContext || !nodesub)
		return -1;

	client = cli->master->modeContext;
	bytes = rtdb_cli_dump_nodesub(nodesub,&dumpLen);
	if (!bytes)
	{
		CMN_LOGGER_ERR(logger,"rtdb_cli_do_tbl_subcribe : rtdb_cli_dump_tblsub error");
		return -1;
	}
    nwritten = network_write(client->fd,bytes,dumpLen);
    free(bytes);
    if(nwritten != dumpLen)
    {
		CMN_LOGGER_ERR(logger, "network_write error");
		return -1;        
    }
	response = rtdb_cli_read_bytes(client,&msgLen);
	if (!response)
	{
		CMN_LOGGER_ERR(logger,"response is NULL");
		return -1;
	}
	// dispose response 
	flag = rtdb_cli_check_response(response, msgLen, nodesub->nNum);
	free(response);
	return 0; // for test
	if (!flag)
	{
		CMN_LOGGER_ERR(logger, "flag = %d", flag);
		return -1;
	}
	return 0;
}

static int rtdb_cli_do_sub_notify()
{
    pthread_attr_t attr;
    pthread_t tid;
    
	rtdb_cli_init_rtdb_sub_cli();

    CMN_LOGGER_DEBUG(logger,"rtdb_cli_do_sub_notify start");	
    if(g_sub_notify_status) return 0;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &attr, &rtdb_cli_notify_thread, gRtdbSubCli);
    pthread_attr_destroy(&attr);
    return 0;
}

static rtdb_cli_subscriber* rtdb_cli_find_subscriber(rtdb_cli_list *subList,const char *uuid)
{
    rtdb_cli_subscriber *pRet = NULL;
    int find = 0;
    if(!subList || !uuid) return NULL;

    pRet = (rtdb_cli_subscriber*)rtdb_cli_list_get_first((rtdb_cli_list*)subList);
    while(pRet)
    {
        if(!strcmp(pRet->uuid,uuid)) 
        {
            find = 1;
            break;
        }
        pRet = (rtdb_cli_subscriber*)rtdb_cli_list_get_next((rtdb_cli_list*)subList);
    }
    if(!find) pRet = NULL;    
    return pRet;
}

static rtdb_cli_tblsub* rtdb_cli_subtable_base(rtdb_cli_uuid uuid, int domainId, int8_t action, int subNum)
{
	rtdb_cli_tblsub *tblsub;

	tblsub = rtdb_cli_create_tblsub();
	if (!tblsub) return NULL;

	memcpy(tblsub->id, uuid, sizeof(rtdb_cli_uuid));
	tblsub->action = action;
	tblsub->domain = domainId;
	tblsub->num = subNum;

	tblsub->noArr = calloc(subNum, sizeof(int8_t));
	if (!tblsub->noArr)
	{
		rtdb_cli_free_tblsub(tblsub);
		return NULL;
	}

	tblsub->nameArr = calloc(subNum, sizeof(rtdb_cli_tbname));
	if (!tblsub->nameArr)
	{
		rtdb_cli_free_tblsub(tblsub);
		return NULL;
	}
	return tblsub;
}

static int rtdb_cli_msubscribe_tables_base(rtdb_cli_subscriber *subscriber, int domainId, const rtdb_cli_list *tableNames, int8_t action)
{
	int retCode = 0;
	int i = 0,subNum = 0;
	const char *tableName = NULL;
	rtdb_cli_tblsub *tblsub = NULL;

	if (!subscriber || !tableNames)
		return -1;
	subNum = rtdb_cli_list_get_size((rtdb_cli_list*)tableNames);
	tblsub = rtdb_cli_subtable_base(subscriber->uuid,domainId,action,subNum);
	if (!tblsub) return -1;

	tableName = (const char*)rtdb_cli_list_get_first((rtdb_cli_list*)tableNames);
	while(tableName)
	{
		tblsub->noArr[i] = i+1;
		snprintf((char*)&tblsub->nameArr[i],sizeof(rtdb_cli_tbname),"%s",tableName);
		i++;
		tableName = (const char*)rtdb_cli_list_get_next((rtdb_cli_list*)tableNames);
	}
	rtdb_cli_init_rtdb_sub_cli();
	retCode = rtdb_cli_do_tbl_subcribe(gRtdbSubCli,tblsub);
	if (retCode != 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_msubscribe_tables_base error");
		rtdb_cli_free_tblsub(tblsub);
		return -1;
	}
	if (tblsub->action == RTDB_CLI_ACT_CANCELSUB)
		rtdb_cli_remove_subscriber(subscriber);
	rtdb_cli_free_tblsub(tblsub);
	/* rtdb_cli_do_sub_notify(); */
	return 0;
}

static rtdb_cli_nodesub* rtdb_cli_subnode_base(rtdb_cli_uuid uuid, const rtdb_cli_list *names, int8_t action, int subNum)
{
	int aNum = 0, i;
	char *name;
	rtdb_cli_nodesub *nodesub;
	rtdb_cli_attrname *attrName;

	nodesub = rtdb_cli_create_nodesub();
	if (!nodesub) return NULL;

	memcpy(nodesub->id, uuid, sizeof(rtdb_cli_uuid));
	nodesub->action = action;
	nodesub->nNum = subNum;

	nodesub->noArr = calloc(subNum, sizeof(int8_t));
	if (!nodesub->noArr)
	{
		rtdb_cli_free_nodesub(nodesub);
		return NULL;
	}

	nodesub->path = calloc(subNum, sizeof(rtdb_cli_sub_path));
	if (!nodesub->path)
	{
		rtdb_cli_free_nodesub(nodesub);
		return NULL;
	}

	if (names)
	{
		aNum = rtdb_cli_list_get_size((rtdb_cli_list*)names);
		nodesub->aNum = aNum;
		nodesub->nameArr = calloc(aNum, sizeof(rtdb_cli_attrname));
		if (!nodesub->path)
		{
			rtdb_cli_free_nodesub(nodesub);
			return NULL;
		}

		i = 0;
		name = (char*)rtdb_cli_list_get_first((rtdb_cli_list*)names);
		while (name)
		{
			attrName = &nodesub->nameArr[i];
			snprintf((char*)attrName, sizeof(rtdb_cli_attrname), "%s", name);
			//memcpy(attrName, name, strlen(name));
			i++;
			name = (char*)rtdb_cli_list_get_next((rtdb_cli_list*)names);
		}
    }
	return nodesub;
}

static int rtdb_cli_msubscribe_nodes_base(
	rtdb_cli_subscriber *subscriber,
	const rtdb_cli_list *nodePaths,
	const rtdb_cli_list *names,
	int8_t action)
{
	int retCode = 0;
	int subNum = 0, i = 0;
	const char *path = NULL;
	rtdb_cli_nodesub *nodesub;

	if (!subscriber || !nodePaths) return -1;
	subNum = (int)rtdb_cli_list_get_size((rtdb_cli_list*)nodePaths);

	nodesub = rtdb_cli_subnode_base(subscriber->uuid,names,action,subNum);
	if (!nodesub) return -1;

	path = (const char*)rtdb_cli_list_get_first((rtdb_cli_list*)nodePaths);
	while (path)
	{
		nodesub->noArr[i] = i + 1;
		snprintf((char*)nodesub->path+i, sizeof(rtdb_cli_sub_path), "%s", path);
		i++;
		path = (const char*)rtdb_cli_list_get_next((rtdb_cli_list*)nodePaths);
	}
	rtdb_cli_init_rtdb_sub_cli();
	retCode = rtdb_cli_do_node_subcribe(gRtdbSubCli, nodesub);
	if (retCode != 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_unique_subscribe_table : rtdb_cli_do_node_subcribe error");
		rtdb_cli_free_nodesub(nodesub);
		return -1;
	}
	if (nodesub->action == RTDB_CLI_ACT_CANCELSUB)
		rtdb_cli_remove_subscriber(subscriber);
	rtdb_cli_free_nodesub(nodesub);
	//rtdb_cli_do_sub_notify();
	return 0;
}

/*
0 unequal, !0 equal
*/
static int rtdb_cli_compare_subscriber(rtdb_cli_subscriber *val, rtdb_cli_subscriber *key) 
{
	int itmp = 0;
	if (!val && !key) return 0;
	if (!val && key) return 0;
	if (val && !key) return 0;
	itmp = strncmp(val->uuid, key->uuid, sizeof(rtdb_cli_uuid));
	return !itmp;
}

static int rtdb_cli_init_rtdb_sub_cli()
{
	if (gRtdbSubCli) return 0;
	gRtdbSubCli = rtdb_cli_create(NULL);
	if (!gRtdbSubCli)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_init_rtdb_sub_cli : rtdb_cli_create fail!");
		return -1;
	}
	rtdb_cli_get_modeclient(gRtdbSubCli);
	return 0;
}

