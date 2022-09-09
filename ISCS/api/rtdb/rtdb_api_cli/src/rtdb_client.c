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
#include <windows.h>
#include <winsock2.h>
#include <stddef.h>
#include <ws2tcpip.h>
#include <winerror.h>
#define EINPROGRESS WSAEINPROGRESS
#define usleep(usec) Sleep(usec/1000.0)
#else
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
#include "rtdb_cli_path.h"
#include "rtdb_cli_data_private.h"
#include "rtdb_cli_data_public.h"
#include "rtdb_cli_data.h"
#include "rtdb_cli_protocol.h"
#include "conf_parser.h"


struct _rtdb_cli
{
	rtdb_cli_list *clients;
	rtdb_server_node *master;
	int domainId;
	rtdb_cli_sockObj **sockObj;
	int sockNum;
	pthread_mutex_t mtx;
	int last_error; /* 0正常，1网络故障，2服务器错误，3其它 */
};

cmn_logger* logger = NULL;
int g_sub_notify_status = 0 ; /* 0 : not start , 1 : start ok */
rtdb_cli_list *gNotifyList = NULL; /* rtdb_cli_subscriber list */
pthread_mutex_t g_notify_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_cmdpub_mutex=PTHREAD_MUTEX_INITIALIZER;
rtdb_cli *gSubCli = NULL;

static int rtdb_cli_reset_lasterror(rtdb_cli* c);
static int rtdb_cli_check_master_status(int domainId,int client);
static rtdb_server_node* rtdb_cli_get_master(rtdb_cli *c);
static rtdb_cli_reply* rtdb_cli_do_query(rtdb_cli *c,int domainId,const unsigned char *bytes,int bytesLen);
static int rtdb_cli_do_write_query(rtdb_cli *c,int domainId,const unsigned char *bytes, int bytesLen);
static rtdb_cli_variant* rtdb_cli_unique_query_attr_base(int cmdType,rtdb_cli *c,const char *spath);
static rtdb_cli_record* rtdb_cli_mquery_attrs_base(int cmdType,rtdb_cli *c,const char *path, const rtdb_cli_list *names);

static rtdb_cli_list* rtdb_cli_get_single_node_key_list(int cmdType,rtdb_cli *c,const char *path);
static rtdb_cli_list* rtdb_cli_query_table_base(int cmdType,rtdb_cli *c,int domainId,const char *tblName,const char *cond,const rtdb_cli_list *filters);
static void rtdb_cli_thread_clean_fun(void *arg);

static int rtdb_cli_get_subreq(int domainId,rtdb_cli *cli,int msgType,
	const char *subId,int seqnum,int subNum,const unsigned char *bytes,int dumpLen);
static int rtdb_cli_do_cmd_subscribe(int doamin,rtdb_cli *cli,const char *uuid,int action);
static int rtdb_cli_do_sub_notify();
static int rtdb_cli_msub_tables_base(rtdb_cli_subscriber *sub, int domainId,int seqnum, const rtdb_cli_list *tableNames, int8_t action);
static int rtdb_cli_msub_nodes_base(rtdb_cli_subscriber *sub,const rtdb_cli_list *paths,const rtdb_cli_list *names,int8_t action);
static int rtdb_cli_do_tbl_subcribe(rtdb_cli *cli, const rtdb_cli_tblsub *tblsub);
static int rtdb_cli_do_node_subcribe(rtdb_cli *cli, const rtdb_cli_nodesub *nodesub);
static int rtdb_cli_nodepub_notify(const rtdb_cli_nodepub *pub);
static int rtdb_cli_init_rtdb_sub_cli();
static void rtdb_cli_remove_subscriber(rtdb_cli_subscriber *sub);
static int rtdb_cli_add_notify_item(const rtdb_cli_subscriber *sub);
static int rtdb_cli_check_str_in_list(rtdb_cli_list *list, const char* value);
static int rtdb_cli_addListitems2list(rtdb_cli_list *srcList, rtdb_cli_list *dstList);
static int rtdb_cli_delListitemsFromList(rtdb_cli_list *srcList, rtdb_cli_list *dstList);
static rtdb_cli_list* rtdb_cli_mmulti_query_attrs_base(int cmdType, rtdb_cli *c, const rtdb_cli_list *paths, const rtdb_cli_list *names);
static int rtdb_cli_update_record_base(int cmdType,	rtdb_cli *c,int domainId,const char *tblName,const char *cond,const rtdb_cli_record *record);
static rtdb_cli_node_item* rtdb_cli_uquery_get_nodeitem(int type,rtdb_cli *c,const char *spath);
static rtdb_cli_sockObj* rtdb_cli_get_client(rtdb_cli *c,int domainId,int type);
static rtdb_cli_node_item* rtdb_cli_uquery_and_getNodeitem(rtdb_cli *c, int domainId,const unsigned char *bytes, int bytesLen, int *lastErr);
static rtdb_cli_sockObj** rtdb_cli_get_domainGoup(int *outNum);
static int rtdb_cli_check_domainId(rtdb_cli *c, int domainId);
static int rtdb_cli_get_domainid_fromPaths(rtdb_cli *c, const rtdb_cli_list *paths,int *outValue);
static int rtdb_cli_get_domainIdBySpath(rtdb_cli *c,const char *spath);
static int rtdb_cli_check_response(rtdb_cli *cli, int domainId, int type, const char* subId, int seqnum, int subNum);
static int rtdb_cli_fill_cliClients(rtdb_cli* cli,sys_conf_rtdb_domain* rtdbConfig);
static int rtdb_cli_init_master(rtdb_cli* cli,int fdType);
static rtdb_cli* rtdb_cli_create_base(const int *domainId);

#ifdef WIN32

#include "windows.h"

int _rtdbInitLibc()
{
	int result = -1;
	WSADATA wsaData;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
		return -1;

	return 0;
}

int _rtdbUninitLibc()
{
	WSACleanup();
	return 0;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		_rtdbInitLibc();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		_rtdbUninitLibc();
		break;
	default:
		break;
	}
	return TRUE;
}
#endif

rtdb_cli* rtdb_cli_create(const int *domainId)
{  
    rtdb_cli* cli = NULL;
  
	cli = rtdb_cli_create_base(domainId);
	if(!cli) return NULL;
	if (-1 == rtdb_cli_init_master(cli,RTDB_CLI_BASEFD))
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_init_master fail!");
		rtdb_cli_free(cli);
		return NULL;
	}
	return cli;
}

void rtdb_cli_free(rtdb_cli *c)
{
	int i = 0;

    if(!c) return;
    if(c->clients) rtdb_cli_list_free(c->clients);
    
	if (c->sockObj)
	{
		for (i = 0; i < c->sockNum; i++)
		{
			if (c->sockObj[i])
				rtdb_cli_free_sockObj(c->sockObj[i]);
		}
		rtdb_cli_cmn_release(c->sockObj);
	}
	rtdb_cli_cmn_release(c);    
}

int rtdb_cli_get_lasterror(rtdb_cli* c)
{
    if(!c) return RTDB_STATUS_OTHERERR;
    return c->last_error;
}

rtdb_cli_variant* rtdb_cli_unique_query_device_attribute(rtdb_cli *c, const char *path)
{
	return rtdb_cli_unique_query_attr_base(RTDB_CMD_GETDEVATTR, c, path);
}

rtdb_cli_variant* rtdb_cli_unique_query_acc_attribute(rtdb_cli *c,const char *path)
{
	return rtdb_cli_unique_query_attr_base(RTDB_CMD_GETACCATTR,c,path);
}

rtdb_cli_variant* rtdb_cli_unique_query_dig_attribute(rtdb_cli *c,const char *path)
{
	return rtdb_cli_unique_query_attr_base(RTDB_CMD_GETDIGATTR,c,path);
}

rtdb_cli_variant* rtdb_cli_unique_query_ana_attribute(rtdb_cli *c,const char *path)
{
	return rtdb_cli_unique_query_attr_base(RTDB_CMD_GETANAATTR,c,path);
}

rtdb_cli_record* rtdb_cli_multi_query_device_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_mquery_attrs_base(RTDB_CMD_MULTIGETDEVATTR,c,path,names);
}

rtdb_cli_record* rtdb_cli_multi_query_acc_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_mquery_attrs_base(RTDB_CMD_MULTIGETACCATTR,c,path,names);
}

rtdb_cli_record* rtdb_cli_multi_query_dig_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_mquery_attrs_base(RTDB_CMD_MULTIGETDIGATTR,c,path,names);
}

rtdb_cli_record* rtdb_cli_multi_query_ana_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
	return rtdb_cli_mquery_attrs_base(RTDB_CMD_MULTIGETANAATTR,c,path,names);
}

rtdb_cli_variant* rtdb_cli_unique_query_attribute(rtdb_cli *c,const char *path)
{    
	return rtdb_cli_unique_query_attr_base(RTDB_CMD_GETATTR, c, path);
}

rtdb_cli_record* rtdb_cli_multi_query_attributes(rtdb_cli *c,const char *path, const rtdb_cli_list *names)
{
    return rtdb_cli_mquery_attrs_base(RTDB_CMD_MULTIGETATTR,c,path,names);  
}

rtdb_cli_record* rtdb_cli_query_node(rtdb_cli *c,const char *spath)
{
    rtdb_cli_record *record = NULL;
	rtdb_cli_node_item *pNodeItem = NULL;
	
	pNodeItem = rtdb_cli_uquery_get_nodeitem(RTDB_CMD_GETNODE,c,spath);
	if(!pNodeItem || (pNodeItem->aNum == 0))  goto ERR_END;
	if(!(record = rtdb_cli_data_fill_record(pNodeItem))) goto ERR_END;
	rtdb_cli_free_node_item(pNodeItem);
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_query_node ok");
    return record;
	
ERR_END:
	if(c) c->last_error = RTDB_STATUS_OTHERERR;
	CMN_LOGGER_ERR(logger, "rtdb_cli_query_node error");
	if(pNodeItem) rtdb_cli_free_node_item(pNodeItem);
	if(record) rtdb_cli_record_free(record);
	return NULL;
}

int rtdb_cli_get_node_type(rtdb_cli *c, const char *spath)
{ 
	rtdb_cli_node_item *pNodeItem = NULL;
	int nodeType = -1;
	
	pNodeItem = rtdb_cli_uquery_get_nodeitem(RTDB_CMD_GETNODETYPE,c,spath);
	if(!pNodeItem || (pNodeItem->aNum == 0)) goto ERR_END;
	nodeType = pNodeItem->attrItems[0]->vt->v.integer ;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_query_node ok");
	rtdb_cli_free_node_item(pNodeItem);
    return (nodeType > 0)?nodeType:-1;
	
ERR_END:
	if(c) c->last_error = RTDB_STATUS_OTHERERR;
	CMN_LOGGER_ERR(logger, "rtdb_cli_query_node error");
	if(pNodeItem) rtdb_cli_free_node_item(pNodeItem);
	return -1;
}

rtdb_cli_list* rtdb_cli_multi_query_nodes(rtdb_cli *c, const rtdb_cli_list *paths)
{
	return rtdb_cli_mmulti_query_attrs_base(RTDB_CMD_MGETNODE, c, paths, NULL);
}

rtdb_cli_list* rtdb_cli_mmulti_query_attributes(rtdb_cli *c, const rtdb_cli_list *paths, const rtdb_cli_list *names)
{
	return rtdb_cli_mmulti_query_attrs_base(RTDB_CMD_MMULTIGETATTR,c, paths, names);
}

rtdb_cli_list* rtdb_cli_list_children(rtdb_cli *c,const char *spath)
{
//	return rtdb_cli_get_single_node_key_list(RTDB_CMD_LISTCHILDREN,c,spath);

	rtdb_cli_list *retList = NULL;
	unsigned char *bytes = NULL, *nBytes = NULL, *p = NULL;
	int bytesLen = 0, nBytesLen = 0, i = 0, domainId = 0;
	rtdb_cli_reply *reply = NULL;
	rtdb_cli_node_item *pNodeItem = NULL;
	int lastErr = RTDB_STATUS_OTHERERR;
	rtdb_cli_attr_item *pAttrItem = NULL;
	char *pvalue = NULL;

	if (!c || !spath)
	{
		CMN_LOGGER_ERR(logger,"spath is NULL");
		return NULL;
	}
	rtdb_cli_reset_lasterror(c);
	domainId = rtdb_cli_get_domainIdBySpath(c,spath);
	if (!rtdb_cli_check_domainId(c,domainId)) goto ERR_LABEL;
	nBytes = rtdb_cli_data_get_queryNodeBytes(RTDB_CMD_LISTCHILDREN,spath,&nBytesLen);
	if (!nBytes) goto ERR_LABEL;
	bytes = rtdb_cli_data_get_finalBytes(domainId,RTDB_CLI_TYPE_QUERYNODE,nBytes,nBytesLen,&bytesLen);
	reply = rtdb_cli_do_query(c, domainId,bytes, bytesLen);
	if (!reply || !reply->nodeItems || (reply->elenum <= 0))
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_query_node reply error");
		lastErr = RTDB_STATUS_SERVERERR;
		goto ERR_LABEL;
	}
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_query_node reply ok");
	retList = rtdb_cli_list_create(free, NULL);
	if (!retList) goto ERR_LABEL;

	for(i = 0; i < reply->elenum;i++)
	{
		pNodeItem = reply->nodeItems[i];
		if(!pNodeItem || pNodeItem->aNum != 1) goto ERR_LABEL;
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_query_node : pNodeItem->aNum = %d", pNodeItem->aNum);
		pAttrItem = pNodeItem->attrItems[0];
		if (!pAttrItem || !pAttrItem->aName || !strlen(pAttrItem->aName)) goto ERR_LABEL;
		if (!(pvalue = strdup(pAttrItem->aName))) goto ERR_LABEL;
		rtdb_cli_list_push_tail(retList, pvalue);
	}

	CMN_LOGGER_DEBUG(logger, "rtdb_cli_list_children ok");
	goto QUIT_LABEL;
	
ERR_LABEL:
	c->last_error = lastErr;
	if (retList)
	{
		rtdb_cli_list_free(retList);
		retList = NULL;
	}
	CMN_LOGGER_ERR(logger, "rtdb_cli_list_children error");
QUIT_LABEL:
	rtdb_cli_cmn_release(nBytes);
	rtdb_cli_cmn_release(bytes);
	if(reply) rtdb_cli_free_reply(reply);
	return retList;
}

rtdb_cli_list* rtdb_cli_list_attributes(rtdb_cli *c, const char *path)
{
    // attributes + refRecordNodes     
    return rtdb_cli_get_single_node_key_list(RTDB_CMD_GETATTRKEYS,c,path); 
}

int rtdb_cli_set_attribute(rtdb_cli *c, const char *path, const char *value)
{
	// 0 : ok ; -1 : error 
	int code = -1, vtType = 0;
    rtdb_cli_variant *vt = NULL;
	unsigned char *bytes = NULL,*nBytes = NULL;
	int bytesLen = 0,nBytesLen = 0,domainId = 0;

	rtdb_cli_reset_lasterror(c);
	domainId = rtdb_cli_get_domainIdBySpath(c,path);
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
	if (!c || !path || !value) goto ERR_LABEL;
	CMN_LOGGER_DEBUG(logger, "path : %s , value : %s", path,value);

	if(!(vt = rtdb_cli_unique_query_attribute(c,path)))
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_set_attribute :  rtdb_cli_get_node_type error , path(%s)",path);
		goto ERR_LABEL;
	}
	vtType = vt->type;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_set_attribute : rtdb_cli_get_node_type ok , vtType = %d", vtType);
	if (!(vt = rtdb_cli_strvalue_to_variant(vtType,value))) goto ERR_LABEL;
	nBytes = rtdb_cli_data_get_updateNodeBytes(RTDB_CMD_SETATTR,path,vt,&nBytesLen);
	if (!nBytes) goto ERR_LABEL;
	bytes = rtdb_cli_data_get_finalBytes(domainId,RTDB_CLI_TYPE_UPDATENODE,nBytes,nBytesLen,&bytesLen);
	code = rtdb_cli_do_write_query(c,domainId,bytes,bytesLen);
	goto QUIT_LABEL;

ERR_LABEL:
	if(c) c->last_error = RTDB_STATUS_OTHERERR;
	CMN_LOGGER_ERR(logger, "rtdb_cli_set_attribute error");

QUIT_LABEL:
	rtdb_cli_cmn_release(nBytes);
    rtdb_cli_cmn_release(bytes);
	rtdb_cli_variant_free(vt);
	return code; 
}

void rtdb_cli_send_command(rtdb_cli *c,int domainId,int type,const char* destination,
	const char *cmdJson,rtdb_cli_cmdResult_notifier *resultNotifier,const void *userData)
{
    int retCode = 0;
    char *uuid = NULL;
    rtdb_cli_msg_rdbcmd *rdbcmd = NULL; // release in rtdb_cli_data_free_cmdNotifier
    rtdb_cli_cmd_notifier *pNotify = NULL;
    rtdb_cli_subscriber *pSubcriber = NULL;
    rtdb_cli_byte *bytes = NULL;
    int dumpLen = 0,nwritten = 0;
	rtdb_cli_sockObj *obj = NULL;
    
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_send_command start");
    if(!c || !cmdJson )
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_send_command : c or cmdJson is NULL");
        goto ERR_END;
    }
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_END;    
    if(!(uuid = rtdb_cli_data_gen_uuid())) goto ERR_END; 
    rdbcmd = rtdb_cli_create_msg_rdbcmd(domainId,type,uuid,destination,cmdJson);
	if (!rdbcmd) goto ERR_END;
	pSubcriber = rtdb_cli_create_subscriber(NULL,NULL,NULL,NULL); // release in thread
    if(!pSubcriber) goto ERR_END;

    // do subscribe
	retCode = rtdb_cli_do_cmd_subscribe(domainId,gSubCli,rdbcmd->id, RTDB_CLI_ACT_SUB);
	if (retCode != 0)
	{
		rtdb_cli_free_msg_rdbcmd(rdbcmd);
		goto ERR_END;
	}
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_send_command : rtdb_cli_do_cmd_subscribe ok");

    pNotify = rtdb_cli_data_create_cmdNotifier(rdbcmd,resultNotifier,userData); // release in thread (rtdb_cli_free_subscriber)
    if(!pNotify) 
    {
        rtdb_cli_free_msg_rdbcmd(rdbcmd);
        goto ERR_END;
    }

    pSubcriber->cmdNotifier = pNotify;
	memcpy(pSubcriber->uuid, rdbcmd->id, RTDB_CLI_UUIDLEN);
	pSubcriber->subType = RTDB_CLI_SUBTYPE_CMD; 
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_send_command : pSubcriber->uuid(%s)", pSubcriber->uuid);
	rtdb_cli_add_notify_item(pSubcriber);
	//rtdb_cli_free_msg_rdbcmd(rdbcmd);  // release in rtdb_cli_data_free_cmdNotifier

	// send cmd
	obj = rtdb_cli_get_client(gSubCli, domainId, RTDB_CLI_SUBFD);
	if(!obj || obj->subfd < 0) goto ERR_END;
	bytes = rtdb_cli_dump_msg_rdbcmd(rdbcmd, &dumpLen);
	if (!bytes || !dumpLen)  goto ERR_END;
	pthread_mutex_lock(&obj->mtx);
	nwritten = rtdb_cli_net_write(obj->subfd, bytes, dumpLen);
	pthread_mutex_unlock(&obj->mtx);
	if (nwritten != dumpLen)  goto ERR_END;
	rtdb_cli_cmn_release(bytes);
	CMN_LOGGER_DEBUG(logger,"rtdb_cli_send_command ok");
    return ;

ERR_END:
    CMN_LOGGER_ERR(logger, "rtdb_cli_send_command error");
	if(uuid) rtdb_cli_cmn_release(uuid);
    if(pSubcriber) rtdb_cli_free_subscriber(pSubcriber);
	if(bytes) rtdb_cli_cmn_release(bytes);
}

int rtdb_cli_send_message(rtdb_cli *c, int domainId, int type, const char* destination, const char *msg, int msgSize)
{
    int retCode = -1; // 0 : ok , -1 : error
	unsigned char *bytes = NULL,*hBytes = NULL,*p = NULL;
	int bytesLen = 0, dstLen = 0, msgBytesLen = 0, curLen = 0 ;
	int newDominId = 0;

    if(!c || !msg) goto ERR_LABEL;
    rtdb_cli_reset_lasterror(c); 

	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
    if(destination) dstLen = strlen(destination);

    msgBytesLen = 4 + 64 + 4 + msgSize;	
    bytesLen = RTDB_CLI_MSGHEADERLEN + msgBytesLen;
	hBytes = rtdb_cli_gen_header_bytes(bytesLen,domainId,RTDB_CLI_TYPE_RDBMSG);
	if (!hBytes) goto ERR_LABEL;
	if (!(p = bytes = calloc(bytesLen,1))) goto ERR_LABEL;
	memcpy(p,hBytes,RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;

	rtdb_cli_encode_int32(type, p, curLen);
	if (dstLen)
	{
		memcpy(p, destination, dstLen);
		p += 64;
	}
	rtdb_cli_encode_int32(msgSize, p, curLen);
	rtdb_cli_encode_bytes((unsigned char*)msg, p, msgSize, curLen);

	rtdb_cli_trace_bin("bytes ", bytes, bytesLen);
    retCode = rtdb_cli_do_write_query(c,domainId,bytes,bytesLen);
	goto QUIT_LABEL;

ERR_LABEL:
	if(c) c->last_error = RTDB_STATUS_OTHERERR;
	
QUIT_LABEL:	
    rtdb_cli_cmn_release(hBytes);
    rtdb_cli_cmn_release(bytes);
    return retCode;
}

rtdb_cli_list* rtdb_cli_query_table(rtdb_cli *c, int domainId, const char *tblName, const rtdb_cli_list *filters)
{
	return rtdb_cli_query_table_base(RTDB_CMD_QUERYTABLE,c, domainId, tblName, NULL, filters);
}

rtdb_cli_record* rtdb_cli_unique_query_record(
	rtdb_cli *c,int domainId,const char *tblName,
	const char *cond,const rtdb_cli_list *filters
)
{
	rtdb_cli_list *retList = NULL;
	rtdb_cli_record* retRecord = NULL;

	retList = rtdb_cli_query_table_base(RTDB_CMD_UQUERYRECORD, c, domainId, tblName, cond, filters);
	if (!retList || (rtdb_cli_list_get_size(retList) == 0))
	{
		if (c) c->last_error = RTDB_STATUS_OTHERERR;
		return NULL;
	}
	retRecord = rtdb_cli_list_get_first(retList);
	retRecord = rtdb_cli_record_dup(retRecord);
	rtdb_cli_list_free(retList);
	return retRecord;
}

rtdb_cli_list* rtdb_cli_multi_query_records(
	rtdb_cli *c,int domainId,const char *tblName,
	const char *cond,const rtdb_cli_list *filters
)
{
	return rtdb_cli_query_table_base(RTDB_CMD_QUERYTABLE,c, domainId, tblName, cond, filters);
}

int rtdb_cli_delete_records(rtdb_cli *c, int domainId,const char *tblName,const char *cond)
{
	int retCode = -1; // 0 : ok ; -1 : error
	char nameBuf[129] = {0};
	unsigned char *bytes = NULL, *hBytes = NULL,*p = NULL;
	int bytesLen = 0, curLen = 0;

	if(!c || !tblName) goto ERR_LABEL;
	rtdb_cli_reset_lasterror(c);
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
	bytesLen = RTDB_CLI_MSGHEADERLEN + 32 + 128 + RTDB_CLI_MD5LEN;
	CMN_LOGGER_DEBUG(logger, "bytesLen = %d ", bytesLen);

	hBytes = rtdb_cli_gen_header_bytes(bytesLen,domainId,RTDB_CLI_TYPE_DELTABLE);
	if(!hBytes) goto ERR_LABEL;
	if(!(p = bytes = calloc(bytesLen, 1))) goto ERR_LABEL;
	
	memcpy(p,hBytes,RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;

	snprintf(nameBuf,sizeof(nameBuf),"%s",tblName);
	rtdb_cli_encode_bytes(nameBuf,p,32,curLen);
	
	snprintf(nameBuf,sizeof(nameBuf),"%s",cond?cond:"");
	rtdb_cli_encode_bytes(nameBuf,p,128,curLen);

	retCode = rtdb_cli_do_write_query(c,domainId, bytes, bytesLen);
	goto QUIT_LABEL;
	
ERR_LABEL:
	if(c) c->last_error = RTDB_STATUS_OTHERERR;
QUIT_LABEL:
	rtdb_cli_cmn_release(hBytes);
	rtdb_cli_cmn_release(bytes);
	return retCode;	
}

int rtdb_cli_update_records(rtdb_cli *c,int domainId,const char *tblName,
	const char *cond,const rtdb_cli_record *record
)
{
	return rtdb_cli_update_record_base(RTDB_CMD_UPDATERECORD,c,domainId,tblName,cond,record);
}

int rtdb_cli_insert_record(rtdb_cli *c,int domainId,const char *tblName,
	const char *cond,const rtdb_cli_record *record)
{
	return rtdb_cli_update_record_base(RTDB_CMD_INSERTRECORD,c,domainId,tblName,cond,record);
}

rtdb_cli_subscriber* rtdb_cli_create_subscriber(
	const char *subscriberName,
	rtdb_cli_node_notifier *nodeNotifier,
	rtdb_cli_table_notifier *tableNotifier,
	rtdb_cli_exception_notifier *expectionNotifier
)
{
    rtdb_cli_subscriber *retSub = NULL;
	char *uuid = NULL;
	
    if(!(retSub = calloc(1,sizeof(*retSub)))) return NULL;
	if (!(uuid = rtdb_cli_data_gen_uuid())) goto ERR_END;
	memcpy(retSub->uuid, uuid, RTDB_CLI_UUIDLEN);
	rtdb_cli_cmn_release(uuid);
    if(subscriberName) retSub->name = strdup(subscriberName);
    retSub->subTime = time(NULL);
    retSub->nodeNotifier = nodeNotifier;
    
	retSub->tableNotifier = tableNotifier;
    retSub->expectionNotifier = expectionNotifier;
    retSub->cmdNotifier = NULL;
	retSub->tables = rtdb_cli_list_create(free, rtdb_cli_data_cmpstr);
	if (!retSub->tables) goto ERR_END;
	retSub->nodes = rtdb_cli_list_create(rtdb_cli_free_nodeSubInfo,NULL);
	if (!retSub->nodes) goto ERR_END;

	rtdb_cli_init_rtdb_sub_cli();
	rtdb_cli_do_sub_notify();
	if (nodeNotifier || tableNotifier)	rtdb_cli_add_notify_item(retSub);
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_create_subscriber : retSub->uuid(%s)", retSub->uuid);
    return retSub;

ERR_END:
	rtdb_cli_cmn_release(retSub);
	return NULL;
}

static void rtdb_cli_remove_subscriber(rtdb_cli_subscriber *sub)
{
	// remove from notify list
	if (!sub) return ;
	if (rtdb_cli_list_get_size(sub->tables) > 0 || rtdb_cli_list_get_size(sub->nodes) > 0) return;
	if (gNotifyList)
    {
        pthread_mutex_lock(&g_notify_mutex);
        rtdb_cli_list_remove(gNotifyList, sub);
        pthread_mutex_unlock(&g_notify_mutex);
    }
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_remove_subscriber : uuid(%s) sz(%d)",
		sub->uuid,rtdb_cli_list_get_size(gNotifyList));
}

void rtdb_cli_free_subscriber(rtdb_cli_subscriber *subscriber)
{
    if(!subscriber) return ;
	rtdb_cli_remove_subscriber(subscriber);
	rtdb_cli_cmn_release(subscriber->name);
    subscriber->nodeNotifier = NULL;
    subscriber->tableNotifier = NULL;
    subscriber->expectionNotifier = NULL;
	rtdb_cli_cmn_release2(rtdb_cli_data_free_cmdNotifier, subscriber->cmdNotifier);
	rtdb_cli_cmn_release2(rtdb_cli_list_free, subscriber->tables);
	rtdb_cli_cmn_release2(rtdb_cli_list_free, subscriber->nodes);
    rtdb_cli_cmn_release(subscriber);
}

int rtdb_cli_unique_subscribe_table(rtdb_cli_subscriber *subscriber, int domainId, const char *tableName)
{
    int retCode = 0;
	int subNum = 1;
	rtdb_cli_tblsub *tblsub = NULL;
	char *tmp = NULL;

    if(!subscriber || !tableName) return -1;
	if (!rtdb_cli_check_domainId(gSubCli, domainId)) return -1;
	rtdb_cli_add_notify_item(subscriber);

	tblsub = rtdb_cli_create_tblsub(domainId,subscriber->uuid,subscriber->seqnum++,RTDB_CLI_ACT_SUB,subNum);
	if (!tblsub) return -1;

	tblsub->noArr[0] = 1;
	snprintf((char*)tblsub->nameArr,sizeof(rtdb_cli_tbname),"%s",tableName);
	retCode = rtdb_cli_do_tbl_subcribe(gSubCli,tblsub);
	rtdb_cli_free_tblsub(tblsub);
	if (retCode != 0) return -1;
	if(!rtdb_cli_check_str_in_list(subscriber->tables,tableName))
	{
		if (tmp = strdup(tableName))
			rtdb_cli_list_push_tail(subscriber->tables,tmp);
	}
	return 0;
}

int rtdb_cli_multi_subscribe_tables(rtdb_cli_subscriber *sub, int domainId, const rtdb_cli_list *tbNames)
{
	int code = 0;
	if (!sub || !tbNames) return -1;
	code = rtdb_cli_msub_tables_base(sub, domainId,sub->seqnum++, tbNames, RTDB_CLI_ACT_SUB);
	if (0 == code)
		rtdb_cli_addListitems2list(tbNames,sub->tables);
	return code;
}

int rtdb_cli_unique_subscribe_node(rtdb_cli_subscriber *subscriber, const char *nodePath, const rtdb_cli_list *names)
{
	int retCode = 0,subNum = 1,domainId = -1;
	rtdb_cli_nodesub *sub = NULL;
	rtdb_cli_nodeSubInfo *subInfo = NULL;
	
	if (!subscriber || !nodePath || !names) return -1;
	domainId = rtdb_cli_get_domainIdBySpath(gSubCli,nodePath);
	if (!rtdb_cli_check_domainId(gSubCli, domainId)) return -1;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_unique_subscribe_node : subscriber->uuid(%s)", subscriber->uuid);
	rtdb_cli_add_notify_item(subscriber);

	sub = rtdb_cli_create_nodesub(domainId,subscriber->uuid,names,RTDB_CLI_ACT_SUB,subNum);
	if (!sub) return -1;

	sub->noArr[0] = 1;
	snprintf((char*)sub->path,sizeof(rtdb_cli_sub_path),"%s",nodePath);

	if (!rtdb_cli_get_client(gSubCli,sub->header->domain,RTDB_CLI_SUBFD)) return -1;
	retCode = rtdb_cli_do_node_subcribe(gSubCli,sub);
	rtdb_cli_free_nodesub(sub);
	if (retCode != 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_unique_subscribe_node : rtdb_cli_do_node_subcribe error");
		return -1;
	}
	if(!rtdb_cli_check_nodeSubInfo(subscriber->nodes,nodePath))
	{
		if(subInfo = rtdb_cli_create_nodeSubInfo(nodePath,names))
		{
			rtdb_cli_list_push_tail(subscriber->nodes, subInfo);
			CMN_LOGGER_DEBUG(logger,"add %s ok",nodePath);
		}
	}

	return 0;
}

int rtdb_cli_multi_subscribe_nodes(
	rtdb_cli_subscriber *subscriber,
	const rtdb_cli_list *nodePaths,
	const rtdb_cli_list *names
)
{
	int code = 0;
	rtdb_cli_nodeSubInfo *subInfo = NULL;
	char *path = NULL;

	if (!subscriber || !nodePaths || !names) return -1;
	code = rtdb_cli_msub_nodes_base(subscriber, nodePaths, names, RTDB_CLI_ACT_SUB);
	if(code != 0) return code;
	
	path = (const char*)rtdb_cli_list_get_first(nodePaths);
	while (path)
	{
		if(!rtdb_cli_check_nodeSubInfo(subscriber->nodes,path))
		{
			if(subInfo = rtdb_cli_create_nodeSubInfo(path,names))
				rtdb_cli_list_push_tail(subscriber->nodes, subInfo);
		}
		path = (const char*)rtdb_cli_list_get_next(nodePaths);
	}
	return code;
}


int rtdb_cli_multi_unsubscribe_tables(rtdb_cli_subscriber *subscriber, int domainId, const rtdb_cli_list *tableNames)
{
	int code = 0;

	code = rtdb_cli_msub_tables_base(subscriber, domainId, subscriber->seqnum++, tableNames, RTDB_CLI_ACT_CANCELSUB);
	if (0 == code) 
		rtdb_cli_delListitemsFromList(tableNames,subscriber->tables);

	return code;
}

int rtdb_cli_multi_unsubscribe_nodes(rtdb_cli_subscriber *subscriber, const rtdb_cli_list *nodePaths)
{
	int code = 0;
	char *path = NULL;

	if(!subscriber || !subscriber->nodes || !nodePaths) return -1;
	code = rtdb_cli_msub_nodes_base(subscriber, nodePaths, NULL, RTDB_CLI_ACT_CANCELSUB);
	if(code != 0) return -1;

	path = (const char*)rtdb_cli_list_get_first(nodePaths);
	while (path)
	{
		rtdb_cli_remove_nodeSubInfo(subscriber->nodes,path);
		path = (const char*)rtdb_cli_list_get_next(nodePaths);
	}
	return code;
}

void rtdb_cli_publish(rtdb_cli_subscriber *subscriber)
{
	int retCode = 0 ;
	retCode = rtdb_cli_do_sub_notify();
	if (retCode != 0)
		CMN_LOGGER_ERR(logger, "rtdb_cli_send_command : rtdb_cli_do_sub_notify error");
}

////////////////////////////////////////////////////////////////////////////////////////

static int rtdb_cli_check_master_status(int domainId,int client)
{
	int masterFlag = 0; // 0 : no , 1 : yes
	unsigned char *bytes = NULL,nBytes[2] = {0},*p = nBytes;
	int bytesLen = 0,nBytesLen = 2,curLen = 0;
	rtdb_cli_reply *reply = NULL;
	rtdb_cli_variant *vt = NULL;

	rtdb_cli_encode_int16(RTDB_CMD_CHECKMASTER,p,curLen);
	bytes = rtdb_cli_data_get_finalBytes(domainId, RTDB_CLI_TYPE_CHECKRTDB, nBytes, nBytesLen, &bytesLen);
	reply = rtdb_cli_data_get_reply(client, bytes, bytesLen);
	if (!reply || !reply->vtItems || (reply->elenum != 1)) goto QUIT_LABEL;
	if(!(vt = reply->vtItems[0])) goto QUIT_LABEL;
	masterFlag = vt->v.integer;
	
QUIT_LABEL:
	if (bytes) rtdb_cli_cmn_release(bytes);
	if (reply) rtdb_cli_free_reply(reply);
	return masterFlag;
}

static rtdb_server_node* rtdb_cli_get_master(rtdb_cli *c)
{
	int masterFlag = 0,pStatus=0,code = 0;
    rtdb_server_node *node = NULL;
    int client = NULL;
	char msgBuf[1024] = { 0 };
    
    if(!c) return NULL;
    node = rtdb_cli_list_get_first(c->clients);
	do {  
		code = rtdb_cli_check_node_status(c->domainId,node, node->modePort, 1);
		if(node->ip1 && node->ip2)
			snprintf(msgBuf,sizeof(msgBuf),"node(%s,%s),port(%d) %s ", 
			node->ip1, node->ip2, node->modePort, 0 == code ? "open" : "not open");
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_get_master : %s", msgBuf);
		if (0 != code) continue; // not open , continue 
        pStatus = 1;
        client = rtdb_cli_net_cnn_rtdb(node->active_ip,node->modePort);
        if (client < 0) continue;
        masterFlag = rtdb_cli_check_master_status(c->domainId,client);
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_get_master : get client ok(%s:%d),masterFlag(%d)", node->active_ip,node->modePort,masterFlag);
        if(masterFlag) break; 
    }while(node = rtdb_cli_list_get_next(c->clients));
    
    if(0 == pStatus)
    {
        // all port closed , no active node 
        CMN_LOGGER_INFO(logger,"rtdb_cli_get_master : all port closed");
        c->last_error = RTDB_STATUS_NETERR;
        return NULL;
    }
    // check if there is no master, then select the first active one
    if (!node)
    {   
        CMN_LOGGER_INFO(logger,"rtdb_cli_get_master : get master fail,use the first active node!");
        node = (rtdb_server_node*)rtdb_cli_list_get_first(c->clients);
        while (node)
        {
            if(node->active_ip) break;
            node = (rtdb_server_node*)rtdb_cli_list_get_next(c->clients);
        }
	}
    CMN_LOGGER_INFO(logger,"rtdb_cli_get_master : master is (%s:%d)",node->active_ip,node->modePort);
    c->master = node;
    return node;
}

static rtdb_cli_sockObj* rtdb_cli_get_client(rtdb_cli *c,int domainId,int type)
{
    rtdb_server_node *master = NULL;
	char *ip = NULL;
    int i = 0, clientFd = -1,port = 0;
	rtdb_cli_sockObj *obj = NULL;
	
    if(!c) return NULL; 
    while(i++ < RTDB_CLI_MAX_RETRY)
    {
        if(!c->master) rtdb_cli_get_master(c);
		if(!(master = c->master)) goto RETRY_NEXT; 
		ip = master->active_ip;
		port = master->modePort;
		
		obj = rtdb_cli_get_sockObj(c->sockObj, c->sockNum, domainId);
		if (!obj) 
			CMN_LOGGER_ERR(logger, "rtdb_cli_get_client ：rtdb_cli_get_sockObj fail(domainId = %d)", domainId);
		clientFd = -1;
        if(master && obj)
        {	
			if(RTDB_CLI_BASEFD == type)
				clientFd = rtdb_cli_net_getfd(ip, port, &obj->fd);
			else if(RTDB_CLI_SUBFD == type)
				clientFd = rtdb_cli_net_getfd(ip,port,&obj->subfd);
        }
		if (clientFd > 0) break;
RETRY_NEXT:		
		c->last_error = RTDB_STATUS_NETERR;
		c->master = NULL;
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_get_client retry count : %d",i);
        usleep(200 * 1000);
    }
	if(-1 == clientFd) 
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_get_client fail");
		obj = NULL;
	}
	return obj;
}

static rtdb_cli_reply* rtdb_cli_do_query(rtdb_cli *c, int domainId,const unsigned char *bytes, int bytesLen)
{
    int baseFd = -1;
    rtdb_cli_reply *reply = NULL;
    int i = 0;
	rtdb_cli_sockObj *obj = NULL;
    
	if (!c || !bytes)
    {
        if(c) c->last_error = RTDB_STATUS_OTHERERR;
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_do_query : c or bytes is NULL");
        return NULL;
    }
    while(i++ < RTDB_CLI_QUERY_MAX_RETRY)
    {
		obj = rtdb_cli_get_client(c, domainId, RTDB_CLI_BASEFD);
		if(!obj) goto RETRY_NEXT;
		pthread_mutex_lock(&obj->mtx);
        if(obj->fd < 0)
            c->last_error = RTDB_STATUS_OTHERERR;
        else
            reply = rtdb_cli_data_get_reply(obj->fd,bytes,bytesLen);
		pthread_mutex_unlock(&obj->mtx);
        if(reply)
		{
			rtdb_cli_reset_lasterror(c);
			break;
		}
		close(obj->fd);
		obj->fd = -1;

RETRY_NEXT:
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_do_query retry count : %d ",i);
        usleep(10 * 1000);
    }
    return reply;	
}

static int rtdb_cli_do_write_query(rtdb_cli *c,int domainId,const unsigned char *bytes, int bytesLen)
{
	// 0 : success ; -1 : error 
    int ret = -1;
    rtdb_cli_reply *reply = NULL;
    
	reply = rtdb_cli_do_query(c,domainId,bytes,bytesLen);
	if (!reply || (reply->type == RTDB_REPLY_TYPE_ERROR) 
		|| (reply->type != RTDB_CLI_REPLY_TYPE_VARIANT) || (reply->elenum != 1))
    {
		if (reply) 
			CMN_LOGGER_ERR(logger, "reply->type(%d) , reply->elenum(%d)", reply->type, reply->elenum);
		else 
			CMN_LOGGER_ERR(logger, "reply is NULL");
		goto ERR_END;
    }
	if (reply->vtItems && reply->vtItems[0] && (1 == reply->vtItems[0]->v.integer))
		ret = 0;
	rtdb_cli_free_reply(reply);
    return ret;
ERR_END:
	c->last_error = RTDB_STATUS_SERVERERR;
	CMN_LOGGER_ERR(logger,"rtdb_cli_do_write_query reply error");
	if(reply) rtdb_cli_free_reply(reply);
	return -1;	
}

static int rtdb_cli_reset_lasterror(rtdb_cli* c)
{
    if(!c) return -1;
    c->last_error = RTDB_STATUS_OK;
    return 0;
}

static int rtdb_cli_get_domainIdBySpath(rtdb_cli *c,const char *spath)
{
	int domainId = -1;
	char *domainName = NULL;

	if (!c || !spath) return -1;
	if (!(domainName = rtdb_cli_path_get_domain2(spath))) return -1;
	domainId = rtdb_cli_get_domainIdbyLabel(c->sockObj,c->sockNum ,domainName);
	free(domainName);
	return domainId;
}

static rtdb_cli_variant* rtdb_cli_unique_query_attr_base(int cmdType,rtdb_cli *c,const char *spath)
{
    rtdb_cli_variant* retValue = NULL;
    rtdb_cli_reply *reply = NULL;
	unsigned char *bytes = NULL,*nBytes = NULL;
	int bytesLen = 0,nBytesLen = 0;
	int lastErr = RTDB_STATUS_OTHERERR; 
	int domainId = -1;

	CMN_LOGGER_DEBUG(logger, "rtdb_cli_unique_query_attr_base begin");
    if(!c || !spath) goto ERR_LABEL;
    rtdb_cli_reset_lasterror(c);
	domainId = rtdb_cli_get_domainIdBySpath(c, spath);
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
	nBytes = rtdb_cli_data_get_queryNodeBytes(cmdType,spath,&nBytesLen);
	if (!nBytes) goto ERR_LABEL;
	bytes = rtdb_cli_data_get_finalBytes(domainId,RTDB_CLI_TYPE_QUERYNODE,nBytes,nBytesLen,&bytesLen);
	reply = rtdb_cli_do_query(c,domainId,bytes, bytesLen);
	if (!reply || !reply->nodeItems || (reply->elenum != 1))
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_unique_query_attr_base reply error");
        lastErr = RTDB_STATUS_SERVERERR;
		goto ERR_LABEL;
    }
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_unique_query_attr_base reply ok");
	if (reply->nodeItems[0] && reply->nodeItems[0]->attrItems[0])
		retValue = rtdb_cli_variant_dup(reply->nodeItems[0]->attrItems[0]->vt);
	goto QUIT_LABEL;
	
ERR_LABEL:
	CMN_LOGGER_ERR(logger,"rtdb_cli_unique_query_attr_base error");
	c->last_error = lastErr;
QUIT_LABEL:	
	rtdb_cli_cmn_release(nBytes);
    rtdb_cli_cmn_release(bytes);
	if(reply) rtdb_cli_free_reply(reply);
	return retValue;
}

static rtdb_cli_record* rtdb_cli_mquery_attrs_base(int cmdType,rtdb_cli *c,const char *spath, const rtdb_cli_list *names)
{
	int pathLen = 0,sz = 0,lastErr = RTDB_STATUS_OTHERERR,domainId = -1;
    rtdb_cli_record *record = NULL;
	int16_t type = cmdType;
	int32_t rowNum = 1, rowId = 1;
	unsigned char *bytes = NULL,*hBytes = NULL,*nBytes = NULL,*nameBytes = NULL,*p = NULL;
	int bytesLen = 0,nBytesLen = 0,nameBytesLen = 0,curLen = 0;
	rtdb_cli_node_item *nodeItem = NULL;
	int8_t attrNum = 0;
    
    if(!c || !spath || !names ) goto ERR_LABEL;
    pathLen = strlen(spath);
    sz = rtdb_cli_list_get_size(names);
    if(!pathLen || !sz)  
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_mquery_attrs_base : pathLen or sz is zero!");
        goto ERR_LABEL;
    }

	attrNum = sz;
    rtdb_cli_reset_lasterror(c);

	domainId = rtdb_cli_get_domainIdBySpath(c, spath);
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;

    bytesLen = RTDB_CLI_MSGHEADERLEN + 2 + 4;
	// encode path nodes 
	if(-1 == rtdb_cli_data_encode_spath(spath,&nBytes,&nBytesLen)) return NULL;
	bytesLen += 4 + nBytesLen + 1 + sz * (32 + 32);	
	CMN_LOGGER_DEBUG(logger, "bytesLen = %d ", bytesLen);	
	hBytes = rtdb_cli_gen_header_bytes(bytesLen,domainId,RTDB_CLI_TYPE_QUERYNODE);
	if (!hBytes) goto ERR_LABEL;
	if (!(p = bytes = calloc(bytesLen, 1))) goto ERR_LABEL;
	
	memcpy(p,hBytes,RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	curLen += RTDB_CLI_MSGHEADERLEN;
    
    rtdb_cli_encode_int16(type,p,curLen);
	rtdb_cli_encode_int32(rowNum, p, curLen);
	rtdb_cli_encode_int32(rowId, p, curLen);
	rtdb_cli_encode_bytes(nBytes, p, nBytesLen, curLen);
	rtdb_cli_encode_int8(attrNum, p, curLen);
	
	// encode path attrNames
	if (-1 == rtdb_cli_data_dump_names(&nameBytes,&nameBytesLen,names)) goto ERR_LABEL;
	rtdb_cli_encode_bytes(nameBytes,p,nameBytesLen,curLen);
	CMN_LOGGER_DEBUG(logger, "bytesLen = %d , curLen = %d ", bytesLen,curLen);
	nodeItem = rtdb_cli_uquery_and_getNodeitem(c,domainId,bytes,bytesLen,&lastErr);
	if(!nodeItem || nodeItem->aNum != sz) goto ERR_LABEL;
    if (!(record = rtdb_cli_data_fill_record(nodeItem))) goto ERR_LABEL;	
	goto QUIT_LABEL;

ERR_LABEL:
	CMN_LOGGER_ERR(logger,"rtdb_cli_mquery_attrs_base error");
	c->last_error = lastErr;
	if(record) 
	{
		rtdb_cli_record_free(record);
		record = NULL;
	}
QUIT_LABEL :
	rtdb_cli_cmn_release(hBytes);
	rtdb_cli_cmn_release(nBytes);
	rtdb_cli_cmn_release(nameBytes);
	rtdb_cli_cmn_release(bytes);
	if(nodeItem) rtdb_cli_free_node_item(nodeItem);
	return record;
}

static rtdb_cli_subscriber* rtdb_cli_get_subcriber_by_id(const char *uuid)
{
    rtdb_cli_subscriber *pSub = NULL;
    
    if(!uuid) return NULL;
    pthread_mutex_lock(&g_notify_mutex); 
	pSub = rtdb_cli_data_find_sub(gNotifyList,uuid);
	pthread_mutex_unlock(&g_notify_mutex);
    
    return pSub;
}

/*
* 一个订阅对象里面只有一个命令
* 0 : success , -1 : fail
*/
static int rtdb_cli_cmdpub_notify(const rtdb_cli_cmdpub *pub)
{
	const void *userData;
	int domainId,type,resultCode;
	const char *destination,*result;
	rtdb_cli_subscriber *pSub = NULL;
	rtdb_cli_msg_rdbcmd *rdbcmd = NULL;
	rtdb_cli_cmdResult_notifier *resultNotifier = NULL;

    pSub = rtdb_cli_get_subcriber_by_id(pub->id);
	if (!pSub || !pSub->cmdNotifier)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_cmdpub_notify : pSub or pSub->cmdNotifier is NULL");
		return -1;
	}

	rdbcmd = pSub->cmdNotifier->rdbcmd;
	resultNotifier = pSub->cmdNotifier->resultNotifier;
	if (!rdbcmd || !resultNotifier) return -1;

	userData = pSub->cmdNotifier->userData;
	domainId = rdbcmd->header->domain;
	type = rdbcmd->type;
	destination = rdbcmd->destination;

	if (pub->num < 1) return -1;
	resultCode = pub->items[0].code;
	result = pub->items[0].result;

	resultNotifier(userData, domainId, type, destination, resultCode, result);
	rtdb_cli_free_subscriber(pSub);	
    return 0;
}

static int rtdb_cli_check_str_in_list(rtdb_cli_list *list,const char* value)
{
	// 0 : no ; 1 : yes
	char *item;
	int flag = 0;

	if (!list || !value) return 0;
	item = (char*)rtdb_cli_list_get_first(list);
	while (item)
	{
		if (0 == strcmp(item, value))
		{
			flag = 1;
			break;
		}
		item = (char*)rtdb_cli_list_get_next(list);
	}
	CMN_LOGGER_DEBUG(logger,"rtdb_cli_check_str_in_list : value(%s),flag(%d)", value,flag);
	return flag;
}

static int rtdb_cli_tblpub_notify(const rtdb_cli_tblpub *pub)
{
	rtdb_cli_subscriber *pSub = NULL;
	int code = 0,i = 0 ;
	const char *tableName = NULL;
	rtdb_cli_record *r = NULL;
	rtdb_cli_list *records = NULL;
	rtdb_cli_node_item *item = NULL; 

	CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify begin");
	if (!pub || !pub->rNum || !pub->nodeItem) return -1;
	
	pSub = rtdb_cli_get_subcriber_by_id(pub->id);
	if (!pSub) return -1;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify : find it!");
	tableName = pub->tName;
	if (!rtdb_cli_check_str_in_list(pSub->tables,tableName)) return -1;
	records = rtdb_cli_list_create(rtdb_cli_record_free, NULL);
	if (!records) goto ERR_END;
    
	for (i = 0; i < pub->rNum; i++)
	{
		item = pub->nodeItem[i];
		if(!(r = rtdb_cli_data_fill_record(item))) goto ERR_END;
		rtdb_cli_list_push_tail(records,r);
	}
	code = 0;
	if (pSub->tableNotifier)
		pSub->tableNotifier(pSub,tableName, pub->type, records);
	else
		code = -1;
	if (pSub->expectionNotifier)
        pSub->expectionNotifier(pSub,code);
	rtdb_cli_list_free(records);
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify end");
	return code;
    
ERR_END :
    CMN_LOGGER_DEBUG(logger, "rtdb_cli_tblpub_notify error");
    if (pSub->expectionNotifier) 
        pSub->expectionNotifier(pSub,-1);
	if(records) rtdb_cli_list_free(records);
    return -1;    
}

static int rtdb_cli_nodepub_notify(const rtdb_cli_nodepub *pub)
{
	rtdb_cli_subscriber *pSub = NULL;
	int code = 0;
	const char *nodePath = NULL;
	rtdb_cli_record *r = NULL;

	if (!pub || !pub->nodeItem) 
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_nodepub_notify : params error");
		return -1;
	}
    CMN_LOGGER_ERR(logger, "pub->id : %.*s ", RTDB_CLI_UUIDLEN,pub->id);
    pSub = rtdb_cli_get_subcriber_by_id(pub->id);
    if (!pSub) goto ERR_END;
	nodePath = pub->path;
	if (!rtdb_cli_check_nodeSubInfo(pSub->nodes, nodePath)) goto ERR_END;
	if(!(r = rtdb_cli_data_fill_record(pub->nodeItem))) goto ERR_END;

	if (pSub->nodeNotifier)
		pSub->nodeNotifier(pSub,nodePath,r);
	else
		code = -1;
	if (pSub->expectionNotifier) 
        pSub->expectionNotifier(pSub,code);
	rtdb_cli_record_free(r);
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_nodepub_notify ok");
	return code;
ERR_END :
    CMN_LOGGER_ERR(logger, "rtdb_cli_nodepub_notify error");
	if(r) rtdb_cli_record_free(r);
    if (pSub && pSub->expectionNotifier) 
        pSub->expectionNotifier(pSub,-1);
    return -1;    
}

static int rtdb_cli_do_notify(rtdb_cli_sockObj *obj)
{
	rtdb_cli_byte *bytes = NULL;
	rtdb_cli_msg_header *header = NULL;
	int msgLen = 0,retCode = 0,status = 0;
	rtdb_cli_cmdpub *cmdpub = NULL;
	rtdb_cli_tblpub *tblpub = NULL;
	rtdb_cli_nodepub *nodepub = NULL;
	int fd = -1;

	fd = obj->subfd;
	bytes = rtdb_cli_data_read_bytes(fd, &msgLen);
	if (!bytes || !msgLen) goto ERR_LABEL;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_notify , msgLen(%d)", msgLen);
	rtdb_cli_trace_bin("rtdb_cli_do_notify bytes:", bytes, msgLen);
	header = rtdb_cli_decode_msg_header(bytes, RTDB_CLI_MSGHEADERLEN);
    if(!header) goto ERR_LABEL;
    CMN_LOGGER_DEBUG(logger, "msgType : %d(0X%02X)",header->msgType,header->msgType);
	switch (header->msgType)
	{
	case RTDB_CLI_SUB_RDBCMD :
	case RTDB_CLI_SUB_RDBTBL :
	case RTDB_CLI_SUB_RDBNODE :
		if(-1 == rtdb_cli_data_add_subrep(obj,bytes,msgLen))
			CMN_LOGGER_ERR(logger, "rtdb_cli_do_notify :  rtdb_cli_data_add_subrep fail!");
		else
			CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_notify :  rtdb_cli_data_add_subrep success!");
		break;
	case RTDB_CLI_PUB_RDBCMD:
		cmdpub = rtdb_cli_parse_cmdpub(bytes, msgLen);
		if (cmdpub)
		{
			rtdb_cli_cmdpub_notify(cmdpub);
			CMN_LOGGER_DEBUG(logger, "cmdpub->header->domain(%d)",cmdpub->header->domain);
			rtdb_cli_free_cmdpub(cmdpub);		
		}
		break;
	case RTDB_CLI_PUB_RDBTBL:
		tblpub = rtdb_cli_parse_tblpub(bytes, msgLen);
		if (tblpub)
		{
			if(-1 == rtdb_cli_tblpub_notify(tblpub))
			 	CMN_LOGGER_ERR(logger, "rtdb_cli_do_notify : rtdb_cli_tblpub_notify fail");
			rtdb_cli_free_tblpub(tblpub);
		}
		break;
	case RTDB_CLI_PUB_RDBNODE:
		nodepub = rtdb_cli_parse_nodepub(bytes, msgLen);
		if (nodepub)
		{
			if(-1 == rtdb_cli_nodepub_notify(nodepub))
                CMN_LOGGER_ERR(logger, "rtdb_cli_do_notify : rtdb_cli_nodepub_notify fail");
			rtdb_cli_free_nodepub(nodepub);
		}
		break;
	default:
		break;
	}
	goto QUIT_LABEL;
ERR_LABEL:
	retCode = -1;
	CMN_LOGGER_ERR(logger, "rtdb_cli_do_notify error!");
QUIT_LABEL:	
	rtdb_cli_cmn_release(bytes);
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, header);
	return retCode;
}

static int rtdb_cli_send_heart_beat(rtdb_cli *cli)
{
	int i = 0,code = 0,tLen = 0;
	rtdb_cli_sockObj *obj = NULL;
	rtdb_cli_msg_header *header= NULL;
	unsigned char *bytes = NULL,*hBytes = NULL;

    if(!cli || !(header = rtdb_cli_create_msg_header())) goto ERR_LABEL;
	tLen = RTDB_CLI_MSGHEADERLEN+RTDB_CLI_MD5LEN;

    header->msgLen = tLen - 4;
	header->domain = cli->domainId;
    header->msgType = RTDB_CLI_TYPE_HEARTBEAT;
    header->reqType = RTDB_CLI_REQTYPE_REQUEST;
    header->sender = RTDB_CLI_SENDER_CLI;    
    header->receiver = RTDB_CLI_SENDER_RDB;	

	if(!(hBytes = rtdb_cli_dump_msg_header(header))) goto ERR_LABEL;
	if(!(bytes = calloc(tLen,1))) goto ERR_LABEL;
	memcpy(bytes,hBytes,RTDB_CLI_MSGHEADERLEN);

	for (i = 0; i < cli->sockNum; i++)
	{
		if (!(obj = cli->sockObj[i])) continue;
		if(obj->subfd < 0) goto ERR_LABEL;
		pthread_mutex_lock(&obj->mtx);
		code = rtdb_cli_net_write(obj->subfd, bytes,tLen);
		pthread_mutex_unlock(&obj->mtx);
		if(code < 0) goto ERR_LABEL;
	}
	CMN_LOGGER_DEBUG(logger,"rtdb_cli_send_heart_beat ok!");
	goto QUIT_LABEL;
ERR_LABEL:
	CMN_LOGGER_ERR(logger,"rtdb_cli_send_heart_beat error!");
	code = -1;
QUIT_LABEL:
	if(header) rtdb_cli_free_msg_header(header);
	if(hBytes) free(hBytes);
	if(bytes) free(bytes);
	return code;	
}

static int rtdb_cli_resub(rtdb_cli *cli,rtdb_cli_list *subList)
{
	// 0 : ok , -1 : fail
	int code = 0;
   	rtdb_cli_subscriber *sub = NULL;
    char *tbName = NULL;
	rtdb_cli_nodeSubInfo *info = NULL;
	rtdb_cli_list *tbNameList = NULL,*nodeList = NULL;
	
	CMN_LOGGER_DEBUG(logger,"rtdb_cli_resub begin");
    if(!cli || !subList) return -1;

    sub = rtdb_cli_list_get_first(subList);
    while(sub)
    {
		// resub
		if(rtdb_cli_list_get_size(sub->tables)>0)
		{
			tbNameList = rtdb_cli_list_create(NULL,NULL);
			if(tbNameList)
			{
				tbName = rtdb_cli_list_get_first(sub->tables);
				while(tbName)
				{
					rtdb_cli_list_push_tail(tbNameList,tbName);
					tbName = rtdb_cli_list_get_next(sub->tables);
				}
				if(cli->sockObj)
					code = rtdb_cli_multi_subscribe_tables(sub,cli->sockObj[0]->domainid,tbNameList);
				//if(-1 == code) break;
				rtdb_cli_list_free(tbNameList);
			}
		}		
		if(-1 == code) break;
		if(rtdb_cli_list_get_size(sub->nodes)>0)
		{
			nodeList = rtdb_cli_list_create(NULL,NULL);
			if(nodeList)
			{
				info = rtdb_cli_list_get_first(sub->nodes);
				while(info)
				{
					rtdb_cli_list_push_tail(nodeList,info);
					info = rtdb_cli_list_get_next(sub->nodes);
				}
				info = rtdb_cli_list_get_first(nodeList);
				while(info)
				{
					code = rtdb_cli_unique_subscribe_node(sub,info->path,info->names);
					if(-1 == code) break;
					info = rtdb_cli_list_get_next(nodeList);
				}
				rtdb_cli_list_free(nodeList);
			}
		}
		if(-1 == code) break;
		sub = rtdb_cli_list_get_next(subList);
    }
	return code;
}

static void rtdb_cli_notify_instance(rtdb_cli *cli, long long *retryCount)
{
	// ignore last_error
	int fd = -1, i = 0;
	fd_set read_fds;
	int activity = 0, maxfd = 0, isErr = 0;
	struct timeval tv;
	rtdb_cli_sockObj *obj = NULL;
	time_t curTime,lastHeartbeat;

	CMN_LOGGER_DEBUG(logger,"rtdb_cli_notify_instance begin!");
	curTime = lastHeartbeat = time(NULL);
	// check conn and do reconnect
	if (-1 == rtdb_cli_init_master(cli,RTDB_CLI_SUBFD))
	{
		CMN_LOGGER_ERR(logger,"rtdb_cli_notify_instance : rtdb_cli_init_master error!");
		goto ERR_END;
	}	
	if(*retryCount > 0)
	{
		if(-1 == rtdb_cli_resub(cli,gNotifyList))
		{
			CMN_LOGGER_WARN(logger,"rtdb_cli_notify_instance : rtdb_cli_resub error!");
			//goto ERR_END;
		}
	} 
	
	// connect ok
	*retryCount = 0;
	
	tv.tv_sec = 0;
	tv.tv_usec = 100;
	while(1)
	{
		isErr = 0;
		// send heartbeat
		curTime = time(NULL);
		
		if ((curTime - lastHeartbeat > 30))
		{	
			if(-1 == rtdb_cli_send_heart_beat(cli)) isErr = 1;
			lastHeartbeat = curTime;
		}
		if(isErr) break;
		
		// do select 
		maxfd = -1;
		FD_ZERO(&read_fds);
		for (i = 0; i < cli->sockNum; i++)
		{
			if (!(obj = cli->sockObj[i])) continue;
			if (obj->subfd != -1) FD_SET(obj->subfd, &read_fds);
		}
		for (i = 0; i < cli->sockNum; i++)
		{
			if (!(obj = cli->sockObj[i])) continue;
			if (obj->subfd > maxfd) maxfd = obj->subfd;
		}
		activity = select(maxfd + 1, &read_fds, NULL, NULL, &tv);
		if (activity)
		{
			for (i = 0; i < cli->sockNum; i++)
			{
				if (!(obj = cli->sockObj[i])) continue;
				if (FD_ISSET(obj->subfd, &read_fds))
				{
					// recvdata and notify
					pthread_mutex_lock(&obj->mtx);
					if(-1 == rtdb_cli_do_notify(obj)) isErr = 1;
					pthread_mutex_unlock(&obj->mtx);
				}
			}
		}
		else
		{
			//CMN_LOGGER_DEBUG(logger,"do sleep!");
			usleep(1000);
		}
		if(isErr) break;
	}
ERR_END:
	for (i = 0; i < cli->sockNum; i++)
	{
		if (!(obj = cli->sockObj[i])) continue;
		pthread_mutex_lock(&obj->mtx);
		close(obj->subfd);
		obj->subfd = -1;
		pthread_mutex_unlock(&obj->mtx);
	}
	CMN_LOGGER_ERR(logger,"rtdb_cli_notify_instance error!");
}

static void* rtdb_cli_notify_thread(void *param)
{
	// ignore last_error
	rtdb_cli *cli = (rtdb_cli*)param;
	long long retryCount = 0;

	pthread_cleanup_push(rtdb_cli_thread_clean_fun,cli);

	CMN_LOGGER_INFO(logger, "rtdb_cli_notify_thread start!\n");
	while(1)
	{
		rtdb_cli_notify_instance(cli,&retryCount);
		retryCount += 1;
        CMN_LOGGER_ERR(logger,"irtdb_fes_rtdb_clientthread : retryCount = %d",retryCount);
        sleep(1);
	}
	g_sub_notify_status = 0;
	pthread_cleanup_pop(0);
}

static rtdb_cli_list* rtdb_cli_get_single_node_key_list(int cmdType,rtdb_cli *c,const char *spath)
{
	rtdb_cli_list *retList = NULL;
	unsigned char *bytes = NULL, *nBytes = NULL, *p = NULL;
	int bytesLen = 0, nBytesLen = 0;
	rtdb_cli_node_item *pNodeItem = NULL;
	int lastErr = RTDB_STATUS_OTHERERR;
	int domainId = 0;

	if (!c || !spath)
	{
		CMN_LOGGER_ERR(logger,"spath is NULL");
		return NULL;
	}
	rtdb_cli_reset_lasterror(c);
	domainId = rtdb_cli_get_domainIdBySpath(c, spath);
	if (!rtdb_cli_check_domainId(c,domainId)) goto ERR_LABEL;
	nBytes = rtdb_cli_data_get_queryNodeBytes(cmdType,spath,&nBytesLen);
	if (!nBytes) goto ERR_LABEL;
	bytes = rtdb_cli_data_get_finalBytes(domainId,RTDB_CLI_TYPE_QUERYNODE,nBytes,nBytesLen,&bytesLen);
	pNodeItem = rtdb_cli_uquery_and_getNodeitem(c, domainId,bytes, bytesLen, &lastErr);
	if (!pNodeItem || (pNodeItem->aNum == 0)) goto ERR_LABEL;
	if (!(retList = rtdb_cli_data_get_nkeyList(pNodeItem))) goto ERR_LABEL;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_list_children ok");

	goto QUIT_LABEL;
	
ERR_LABEL:
	c->last_error = lastErr;
	CMN_LOGGER_ERR(logger, "rtdb_cli_list_children error");
QUIT_LABEL:
	if (pNodeItem) rtdb_cli_free_node_item(pNodeItem);
	rtdb_cli_cmn_release(nBytes);
	rtdb_cli_cmn_release(bytes);
	return retList;
}

static rtdb_cli_list* rtdb_cli_query_table_base(int cmdType,rtdb_cli *c,int domainId,const char *tblName,const char *cond,const rtdb_cli_list *filters)
{
    rtdb_cli_list *retList = NULL;
    rtdb_cli_reply *reply = NULL;
	unsigned char *bytes = NULL, *hBytes = NULL,*nBytes = NULL;
	int bytesLen = 0,nBytesLen = 0;
	int lastErr = RTDB_STATUS_OTHERERR;

    if(!c) return NULL;
    rtdb_cli_reset_lasterror(c);
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
    if(!tblName) goto ERR_LABEL;
	nBytes = rtdb_cli_data_get_queryTableBytes(cmdType,tblName,cond,filters,&nBytesLen);
    if(!nBytes) goto ERR_LABEL;
	rtdb_cli_trace_bin("nBytes", nBytes, nBytesLen);
	
	bytes = rtdb_cli_data_get_finalBytes(domainId,RTDB_CLI_TYPE_QUERYTABLE,nBytes,nBytesLen,&bytesLen);
    reply = rtdb_cli_do_query(c,domainId,bytes,bytesLen);
	if (!reply || (reply->type != RTDB_CLI_REPLY_TYPE_TABLE) || (reply->elenum < 0) || !reply->nodeItems)
    {
        CMN_LOGGER_DEBUG(logger,"rtdb_cli_query_table_base reply error");
        lastErr = RTDB_STATUS_SERVERERR;
		goto ERR_LABEL;
    }
	retList = rtdb_cli_data_get_recordList(reply);
	goto QUIT_LABEL;
	
ERR_LABEL:
	c->last_error = lastErr;
QUIT_LABEL:
	rtdb_cli_cmn_release(hBytes);
	rtdb_cli_cmn_release(nBytes);
    if(reply) rtdb_cli_free_reply(reply);
	rtdb_cli_cmn_release(bytes);
    return retList;    
}

static void rtdb_cli_thread_clean_fun(void *arg)
{
    rtdb_cli *c = NULL;
    CMN_LOGGER_INFO(logger,"rtdb_cli_thread_clean_fun called \n");
    g_sub_notify_status = 0;
    CMN_LOGGER_INFO(logger, "rtdb_cli_thread_clean_fun: set g_sub_notify_status = 0");
    c = (rtdb_cli*)arg;
	rtdb_cli_cmn_release(c);    
}

static int rtdb_cli_get_subreq(int domainId,rtdb_cli *cli,int msgType,
	const char *subId,int seqnum,int subNum,const unsigned char *bytes,int dumpLen)
{
	/* 0 : false ; 1 : true */
	int nwritten = 0;
	rtdb_cli_sockObj *obj = NULL;
	
	if(!cli || !cli->sockObj || !subId || !bytes) return 0;
	rtdb_cli_trace_bin("rtdb_cli_get_subreq bytes", bytes, dumpLen);

	obj = rtdb_cli_get_sockObj(cli->sockObj,cli->sockNum, domainId);
	if (!obj || obj->subfd < 0) return 0;
	pthread_mutex_lock(&obj->mtx);
	nwritten = rtdb_cli_net_write(obj->subfd, bytes, dumpLen);
	pthread_mutex_unlock(&obj->mtx);
	CMN_LOGGER_DEBUG(logger,"nwritten = %d, dumpLen=%d",nwritten,dumpLen);
	if (nwritten != dumpLen) return 0;
    CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_cmd_subscribe : rtdb_cli_net_write ok");
	return rtdb_cli_check_response(cli, domainId, msgType, subId, seqnum, subNum);
}

/*
* 一个订阅对象里面只有一个命令
* 0 : success , -1 : fail
*/
static int rtdb_cli_do_cmd_subscribe(int domain,rtdb_cli *cli,const char *uuid,int action)
{
	rtdb_cli_cmdsub *sub = NULL;
	rtdb_cli_byte *bytes = NULL;
	int len = 0,code = 0;
	
	if(!cli || !cli->master || !uuid) goto ERR_LABEL;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_do_cmd_subscribe : domain(%d),uuid(%.*s)",domain,RTDB_CLI_UUIDLEN,uuid);
	if (!(sub = rtdb_cli_create_cmdsub(domain))) goto ERR_LABEL;
	memcpy(sub->id, uuid, RTDB_CLI_UUIDLEN);
	sub->action = action;
	sub->num = 1;
	if(!(sub->noArr = calloc(1, sizeof(int8_t)))) goto ERR_LABEL;
	if(!(sub->cmdidArr = calloc(1,RTDB_CLI_UUIDLEN))) goto ERR_LABEL;
	memcpy(sub->cmdidArr[0], uuid, RTDB_CLI_UUIDLEN);
	if (!(bytes = rtdb_cli_dump_cmdsub(sub,&len))) goto ERR_LABEL;
	if (!rtdb_cli_get_subreq(domain,cli,RTDB_CLI_SUB_RDBCMD,uuid,0,1,bytes,len))
		goto ERR_LABEL;	
	goto QUIT_LABEL;
	
ERR_LABEL:
	code = -1;
	CMN_LOGGER_ERR(logger, "rtdb_cli_do_cmd_subscribe error");
QUIT_LABEL:
	if(sub) rtdb_cli_free_cmdsub(sub);
	if(bytes) rtdb_cli_cmn_release(bytes);
	return code;	
}

static int rtdb_cli_do_tbl_subcribe(rtdb_cli *cli, const rtdb_cli_tblsub *sub)
{
	// 0 : ok , -1 : error
	rtdb_cli_byte *bytes = NULL;
	int len = 0,code = 0,subResult = 0;

	if (!cli || !cli->master || !sub || !sub->header) goto ERR_LABEL;
	if (!(bytes = rtdb_cli_dump_tblsub(sub,&len))) goto ERR_LABEL;
	subResult = rtdb_cli_get_subreq(sub->header->domain,cli,RTDB_CLI_SUB_RDBTBL,
		sub->id,sub->seqnum,sub->num,bytes,len);
	if (!subResult)
		goto ERR_LABEL;	   
    goto QUIT_LABEL;
	
ERR_LABEL:
	code = -1;
	CMN_LOGGER_ERR(logger, "rtdb_cli_do_tbl_subcribe error");
QUIT_LABEL:
	if(bytes) rtdb_cli_cmn_release(bytes);
	return code;		
}

static int rtdb_cli_do_node_subcribe(rtdb_cli *cli,const rtdb_cli_nodesub *sub)
{
	// 0 : ok , -1 : error
	rtdb_cli_byte *bytes = NULL;
	int len = 0,code = 0,subResult = 0;

	if (!cli || !cli->master || !sub) goto ERR_LABEL;
	if (!(bytes=rtdb_cli_dump_nodesub(sub,&len))) goto ERR_LABEL;	
	subResult = rtdb_cli_get_subreq(sub->header->domain,cli,RTDB_CLI_SUB_RDBNODE,
		sub->id,sub->seqnum,sub->nNum,bytes,len);
	if (!subResult)
		goto ERR_LABEL;	
	goto QUIT_LABEL;
	
ERR_LABEL:
	code = -1;
	CMN_LOGGER_ERR(logger, "rtdb_cli_do_node_subcribe error");
QUIT_LABEL:
	rtdb_cli_cmn_release(bytes);
	return code;			
}

static int rtdb_cli_do_sub_notify()
{
    pthread_attr_t attr;
    pthread_t tid,tid2;

    CMN_LOGGER_DEBUG(logger,"rtdb_cli_do_sub_notify start , g_sub_notify_status = %d",g_sub_notify_status);	
    if(g_sub_notify_status) return 0;
	g_sub_notify_status = 1;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &attr, &rtdb_cli_notify_thread, gSubCli);
    pthread_attr_destroy(&attr);
    return 0;
}


static int rtdb_cli_msub_tables_base(rtdb_cli_subscriber *sub, int domainId,int seqnum, const rtdb_cli_list *tableNames, int8_t action)
{
	int retCode = 0;
	int i = 0,subNum = 0;
	const char *tableName = NULL;
	rtdb_cli_tblsub *tblsub = NULL;

	if (!sub || !tableNames)
		return -1;
	if (!rtdb_cli_check_domainId(gSubCli, domainId)) return -1;
	rtdb_cli_add_notify_item(sub);
	subNum = rtdb_cli_list_get_size((rtdb_cli_list*)tableNames);
	tblsub = rtdb_cli_create_tblsub(domainId,sub->uuid, seqnum, action, subNum);
	if (!tblsub) return -1;

	tableName = (const char*)rtdb_cli_list_get_first(tableNames);
	while(tableName)
	{
		tblsub->noArr[i] = i+1;
		snprintf((char*)&tblsub->nameArr[i],sizeof(rtdb_cli_tbname),"%s",tableName);
		i++;
		tableName = (const char*)rtdb_cli_list_get_next(tableNames);
	}
	retCode = rtdb_cli_do_tbl_subcribe(gSubCli,tblsub);
	if (retCode != 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_msub_tables_base error");
		rtdb_cli_free_tblsub(tblsub);
		return -1;
	}
	if (tblsub->action == RTDB_CLI_ACT_CANCELSUB)
		rtdb_cli_remove_subscriber(sub);
	rtdb_cli_free_tblsub(tblsub);
	return 0;
}

static int rtdb_cli_msub_nodes_base(rtdb_cli_subscriber *sub,const rtdb_cli_list *nodePaths,const rtdb_cli_list *names,int8_t action)
{
	int retCode = 0;
	int subNum = 0, i = 0;
	const char *path = NULL;
	rtdb_cli_nodesub *nodesub = NULL;
	int domainId = -1;

	if (!sub || !nodePaths) return -1;

	if (-1 == rtdb_cli_get_domainid_fromPaths(gSubCli, nodePaths, &domainId))
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_query_node : rtdb_cli_get_domainid_fromPaths error");
		return -1;
	}
	if (!rtdb_cli_check_domainId(gSubCli, domainId)) return -1;
	rtdb_cli_add_notify_item(sub);
	subNum = (int)rtdb_cli_list_get_size(nodePaths);
	nodesub = rtdb_cli_create_nodesub(domainId,sub->uuid,names,action,subNum);
	if (!nodesub) return -1;

	path = (const char*)rtdb_cli_list_get_first(nodePaths);
	while (path)
	{
		nodesub->noArr[i] = i + 1;
		snprintf((char*)&nodesub->path[i], sizeof(rtdb_cli_sub_path), "%s", path);
		i++;
		path = (const char*)rtdb_cli_list_get_next(nodePaths);
	}

	retCode = rtdb_cli_do_node_subcribe(gSubCli, nodesub);
	if (retCode != 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_unique_subscribe_table : rtdb_cli_do_node_subcribe error");
		rtdb_cli_free_nodesub(nodesub);
		return -1;
	}
	if (nodesub->action == RTDB_CLI_ACT_CANCELSUB)
		rtdb_cli_remove_subscriber(sub);
	rtdb_cli_free_nodesub(nodesub);

	return 0;
}

static int rtdb_cli_init_rtdb_sub_cli()
{
	if(gSubCli) return 0;

	gSubCli = rtdb_cli_create_base(NULL);
	if(!gSubCli)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_init_rtdb_sub_cli : rtdb_cli_create fail!");
		return -1;
	}	
	if (-1 == rtdb_cli_init_master(gSubCli,RTDB_CLI_SUBFD))
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_init_master fail!");
		return -1;
	}
			
	CMN_LOGGER_DEBUG(logger,"rtdb_cli_init_rtdb_sub_cli : gSubCli->sockNum = %d",gSubCli->sockNum);
	return 0;
}

static int rtdb_cli_add_notify_item(const rtdb_cli_subscriber *sub)
{
    // add to notify list
    if(!sub || !sub->uuid) return -1;
	pthread_mutex_lock(&g_notify_mutex);
	if (!gNotifyList)
		gNotifyList = rtdb_cli_list_create(NULL,rtdb_cli_data_cmpsub);

	if (!rtdb_cli_data_find_sub(gNotifyList, sub->uuid))
	{
		rtdb_cli_list_push_tail(gNotifyList, sub);
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_add_notify_item : add ok(%s) , sz : %d",
			sub->uuid,rtdb_cli_list_get_size((rtdb_cli_list*)gNotifyList));
	}
	else
	{
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_add_notify_item : alread exist(%s)",sub->uuid);
	}
	pthread_mutex_unlock(&g_notify_mutex);
    return 0;
}

static int rtdb_cli_addListitems2list(rtdb_cli_list *srcList, rtdb_cli_list *dstList)
{
	char *item, *tmp;

	if (!srcList || !dstList) return -1;
	item = (const char*)rtdb_cli_list_get_first(srcList);
	while (item)
	{
		if(!rtdb_cli_check_str_in_list(dstList,item))
		{
			if (tmp = strdup(item))
				rtdb_cli_list_push_tail(dstList, tmp);
		}
	
		item = (const char*)rtdb_cli_list_get_next(srcList);
	}
	return 0;
}

static int rtdb_cli_delListitemsFromList(rtdb_cli_list *srcList, rtdb_cli_list *dstList)
{
	char *item, *tmp;

	if (!srcList || !dstList) return -1;
	item = (const char*)rtdb_cli_list_get_first(srcList);
	while (item)
	{
		rtdb_cli_list_remove(dstList,item);
		item = (const char*)rtdb_cli_list_get_next(srcList);
	}
	return 0;
}

static int rtdb_cli_get_domainid_fromPaths(rtdb_cli *c, const rtdb_cli_list *paths,int *outValue)
{
	//0 : success ; -1 : error  
	int domainId = -1, tmpId = -1,i = 0;
	char *spath = NULL;

	if (!c || !paths) return -1;
	spath = rtdb_cli_list_get_first(paths);
	while (spath)
	{
		tmpId = domainId;
		domainId = rtdb_cli_get_domainIdBySpath(c,spath);
		if(0 == i) tmpId = domainId;
		if (tmpId != domainId) 
		{
			CMN_LOGGER_ERR(logger, "rtdb_cli_get_domainid_fromPaths : spath(%s),tmpId(%d),domainId(%d)", spath,tmpId,domainId);
			return -1;
		}
		i++;
		spath = (char*)rtdb_cli_list_get_next(paths);
	}
	*outValue = domainId;
	return 0;
}

static rtdb_cli_list* rtdb_cli_mmulti_query_attrs_base(int cmdType ,rtdb_cli *c, const rtdb_cli_list *paths, const rtdb_cli_list *names)
{
	rtdb_cli_list* retList = NULL;
    rtdb_cli_reply *reply = NULL;
	unsigned char *bytes = NULL,*nBytes = NULL,*p = NULL;
	int bytesLen = 0,nBytesLen = 0;
	int lastErr = RTDB_STATUS_OTHERERR;
	int domainId = -1;
	
    if(!c || !paths)
    {
        CMN_LOGGER_ERR(logger,"paths is NULL");
        return NULL;
    }
	rtdb_cli_reset_lasterror(c);

	if (-1 == rtdb_cli_get_domainid_fromPaths(c, paths, &domainId))
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_query_node : rtdb_cli_get_domainid_fromPaths error");
		goto ERR_LABEL;
	}
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
	// combin paths and names , gen new paths list
	nBytes = rtdb_cli_data_get_mqueryNodeBytes(cmdType, paths, names, &nBytesLen);
	if (!nBytes) goto ERR_LABEL;
	bytes = rtdb_cli_data_get_finalBytes(domainId,RTDB_CLI_TYPE_QUERYNODE,nBytes,nBytesLen,&bytesLen);
	reply = rtdb_cli_do_query(c,domainId,bytes, bytesLen);
	if (!reply || !reply->nodeItems || (reply->elenum < 0))
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_query_node reply error");
        lastErr = RTDB_STATUS_SERVERERR;
		goto ERR_LABEL;
    }

    CMN_LOGGER_DEBUG(logger,"rtdb_cli_query_node reply ok");
	if (RTDB_CMD_MMULTIGETATTR == cmdType)
		retList = rtdb_cli_data_get_vtList(reply);
	else if (RTDB_CMD_MGETNODE == cmdType)
		retList = rtdb_cli_data_get_recordList(reply);
	goto QUIT_LABEL;
	
ERR_LABEL:
	c->last_error = lastErr;
	CMN_LOGGER_ERR(logger, "rtdb_cli_query_node error");

QUIT_LABEL:	
	rtdb_cli_cmn_release(nBytes);
	rtdb_cli_cmn_release(bytes);
	if(reply) rtdb_cli_free_reply(reply);
	return retList; 
}

static int rtdb_cli_update_record_base(int cmdType,rtdb_cli *c,int domainId,const char *tblName,
	const char *cond,const rtdb_cli_record *record)
{
	int retCode = -1; // 0 : ok ; -1 : error 
	unsigned char *bytes = NULL, *hBytes = NULL, *recordBytes = NULL, *p = NULL;
	int bytesLen = 0, rBytesLen = 0;

	if (!c || !tblName || !record) goto ERR_LABEL;
	rtdb_cli_reset_lasterror(c);
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
	recordBytes = rtdb_cli_data_get_updateRecordBytes(cmdType,tblName,cond,record,&rBytesLen);
	if (!recordBytes) goto ERR_LABEL;
	rtdb_cli_trace_bin("recordBytes", recordBytes, rBytesLen);
	bytesLen = RTDB_CLI_MSGHEADERLEN + rBytesLen + RTDB_CLI_MD5LEN;
	CMN_LOGGER_DEBUG(logger, "bytesLen = %d ", bytesLen);

	hBytes = rtdb_cli_gen_header_bytes(bytesLen,domainId, RTDB_CLI_TYPE_UPDATETABLE);
	if (!hBytes) goto ERR_LABEL;
	if (!(p = bytes = calloc(bytesLen, 1))) goto ERR_LABEL;

	memcpy(p, hBytes, RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	memcpy(p, recordBytes, rBytesLen);

	retCode = rtdb_cli_do_write_query(c,domainId,bytes, bytesLen);
	goto QUIT_LABEL;

ERR_LABEL:
	if (c) c->last_error = RTDB_STATUS_OTHERERR;
QUIT_LABEL:
	rtdb_cli_cmn_release(recordBytes);
	rtdb_cli_cmn_release(hBytes);
	rtdb_cli_cmn_release(bytes);
	return retCode;
}

static rtdb_cli_node_item* rtdb_cli_uquery_get_nodeitem(int type,rtdb_cli *c,const char *spath)
{
	unsigned char *bytes = NULL,*nBytes = NULL;
	int bytesLen = 0,nBytesLen = 0;
	rtdb_cli_node_item *pNodeItem = NULL;
	int lastErr = RTDB_STATUS_OTHERERR;
	int domainId = -1;
	
	if (!c || !spath) return NULL;
    rtdb_cli_reset_lasterror(c);
	domainId = rtdb_cli_get_domainIdBySpath(c, spath);
	if (!rtdb_cli_check_domainId(c, domainId)) goto ERR_LABEL;
	nBytes = rtdb_cli_data_get_queryNodeBytes(type,spath,&nBytesLen);
	if (!nBytes) goto ERR_LABEL;

	bytes = rtdb_cli_data_get_finalBytes(domainId,RTDB_CLI_TYPE_QUERYNODE,nBytes,nBytesLen,&bytesLen);
	pNodeItem = rtdb_cli_uquery_and_getNodeitem(c,domainId,bytes,bytesLen,&lastErr);
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_query_node ok");
	goto QUIT_LABEL;
	
ERR_LABEL:
	if(c) c->last_error = lastErr;
	CMN_LOGGER_ERR(logger, "rtdb_cli_uquery_get_nodeitem error");
QUIT_LABEL:
	rtdb_cli_cmn_release(nBytes);
    rtdb_cli_cmn_release(bytes);
	return pNodeItem;
}

static rtdb_cli_node_item* rtdb_cli_uquery_and_getNodeitem(rtdb_cli *c, int domainId,const unsigned char *bytes, int bytesLen, int *lastErr)
{
	rtdb_cli_reply *reply = NULL;
	rtdb_cli_node_item *pNodeItem = NULL;

	reply = rtdb_cli_do_query(c, domainId,bytes, bytesLen);
	if (!reply || !reply->nodeItems || (reply->elenum != 1))
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_query_node reply error");
		if (lastErr) *lastErr = RTDB_STATUS_SERVERERR;
		if (reply) rtdb_cli_free_reply(reply);
		return NULL;
	}
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_query_node reply ok");
	pNodeItem = rtdb_cli_node_item_dup(reply->nodeItems[0]);
	if(pNodeItem) CMN_LOGGER_ERR(logger,"pNodeItem->aNum : %d",pNodeItem->aNum);
	rtdb_cli_free_reply(reply);

	return pNodeItem;
}

static rtdb_cli_sockObj** rtdb_cli_get_domainGoup(int *outNum)
{
	rtdb_cli_sockObj **outObj = NULL, *ptmp = NULL;
	int i = 0, num = 0;
	sys_domain_group *dGroup = NULL;

	dGroup = sys_conf_local_get_domain_group();
	if (!dGroup) return NULL;
	num = dGroup->num;
	if (!(outObj = calloc(num, sizeof(*outObj))))
	{
		sys_conf_free_domain_group(dGroup);
		return NULL;
	}
	for (i = 0; i < num; i++)
	{
		if (!(ptmp = rtdb_cli_create_sockObj())) goto ERR_END;
		CMN_LOGGER_DEBUG(logger, "label:%s , id : %d", dGroup->lables[i],dGroup->ids[i]);
		ptmp->domainid = dGroup->ids[i];
		ptmp->label = strdup(dGroup->lables[i]);
		if (!ptmp->label)
		{
			rtdb_cli_free_sockObj(ptmp);
			goto ERR_END;
		}
		outObj[i] = ptmp;
	}
	*outNum = num;
	sys_conf_free_domain_group(dGroup);

	return outObj;

ERR_END:
	sys_conf_free_domain_group(dGroup);
	for (i = 0; i < num; i++)
	{
		if (outObj[i]) rtdb_cli_free_sockObj(outObj[i]);
	}
	free(outObj);
	return NULL;
}

static int rtdb_cli_check_domainId(rtdb_cli *c, int domainId)
{
	// 0 : false ; 1 : true
	int ret = 0;

	if (!c || !(c->sockObj)) return 0;
//	if (0 == c->domainId) // center
	{
		if (rtdb_cli_get_sockObj(c->sockObj, c->sockNum, domainId))
			ret = 1;
	}
	/*
	else // station 
	{
		ret = (c->domainId == domainId);
	}
	*/
	if (!ret) CMN_LOGGER_ERR(logger, "rtdb_cli_check_domainId fail(domainId = %d)", domainId);
	return ret;
}

static int rtdb_cli_check_response(rtdb_cli *cli, int domainId, int type, const char* subId, int seqnum, int subNum)
{
	/* 0 : false ; 1 : true */
	int flag = 0,count=0,maxRetry = 2000;
	rtdb_cli_sockObj *obj = NULL;
	int find = 0;

	if (!cli || !cli->sockObj || subNum <= 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_check_response : params error!");
		return 0;
	}

	while(1) 
	{
		obj = rtdb_cli_get_sockObj(cli->sockObj, cli->sockNum, domainId);
		if (obj)
		{
			pthread_mutex_lock(&obj->mtx);
			find = rtdb_cli_data_check_subresp(obj->subRespList,type,subId,seqnum,subNum,&flag);
			pthread_mutex_unlock(&obj->mtx);
			if (find) break;
		}
		else
		{
			CMN_LOGGER_ERR(logger, "rtdb_cli_check_response : rtdb_cli_get_sockObj error(domainId = %d)!", domainId);
		}
		if (count++ >= maxRetry)
		{
			CMN_LOGGER_ERR(logger, "rtdb_cli_check_response timeout(count = %d)!", count);
			break;
		}
		usleep(1000);
	}
	
	return flag;
}

static int rtdb_cli_fill_cliClients(rtdb_cli* cli,sys_conf_rtdb_domain* rtdbConfig)
{
	// 0 : success ; -1 : fail
    sys_conf_rtdb_server* confNode = NULL;
    rtdb_server_node *snode = NULL;
 
	if (!cli || !rtdbConfig || !(confNode = rtdbConfig->servers)) goto ERR_END;
	while (confNode)
	{
		if (!(snode = rtdb_server_node_create())) goto ERR_END;
		if (!confNode->ip1 || !(snode->ip1 = strdup(confNode->ip1))) goto ERR_END;
		if (!confNode->ip2 || !(snode->ip2 = strdup(confNode->ip2))) goto ERR_END;
		if (!confNode->username || !(snode->user = strdup(confNode->username))) goto ERR_END;
		if (!confNode->password || !(snode->pswd = strdup(confNode->password))) goto ERR_END;
		snode->modePort = confNode->modePort;
		if (!rtdb_cli_list_push_tail(cli->clients, snode)) goto ERR_END;
		confNode = confNode->next;
	}
	return 0;
ERR_END:
	CMN_LOGGER_ERR(logger, "rtdb_cli_fill_cliClients error!");
	return -1;
}

static int rtdb_cli_init_master(rtdb_cli* cli,int fdType)
{
	// 0 : success ; -1 : fail 
	int i = 0,fd = -1;
	rtdb_cli_sockObj *obj = NULL;
	char *ip = NULL;
	int port = 0;

	if (!cli || !cli->sockObj) goto ERR_END;
	if (!rtdb_cli_get_master(cli)) goto ERR_END;

	ip = cli->master->active_ip;
	port = cli->master->modePort;

	for (i = 0; i < cli->sockNum; i++)
	{
		obj = cli->sockObj[i];
		if (!obj) goto ERR_END;
		if(RTDB_CLI_BASEFD == fdType)
			fd = rtdb_cli_net_getfd(ip, port, &obj->fd);
		else if(RTDB_CLI_SUBFD == fdType)
			fd = rtdb_cli_net_getfd(ip, port, &obj->subfd);
		if (fd < 0)
		{
			CMN_LOGGER_ERR(logger, "rtdb_cli_net_getfd fail!"); 
			goto ERR_END;
		}
	}
	return 0;

ERR_END:
	CMN_LOGGER_ERR(logger, "rtdb_cli_init_master fail!"); 
	return -1;
}

static rtdb_cli* rtdb_cli_create_base(const int *domainId)
{  
    rtdb_cli* cli = NULL;
    sys_conf_rtdb_domain* rtdbConfig = NULL;
    sys_conf_log* log = NULL;
	int cnfDomainId = -1;
  
	if(!(cli = calloc(1,sizeof(*cli)))) return NULL;
	cli->domainId = -1;
	if (domainId) 
		cli->domainId = *domainId;
	else
	{
		sys_conf_get_local_domain_id(&cnfDomainId);
		cli->domainId = cnfDomainId;
	}
    cli->last_error = 0;
    cli->clients = rtdb_cli_list_create(rtdb_server_node_free,NULL);
    if (!cli->clients) goto ERR_END;
    cli->master = NULL;
    pthread_mutex_init(&cli->mtx,NULL);

	if (!(rtdbConfig = sys_conf_get_rtdb_domain()))
    {
		printf("rtdb_cli_create : sys_conf_get_rtdb_domain fail,domainId(%d)!\n",cli->domainId);
		goto ERR_END;
    }
    log = rtdbConfig->log;
    if(!logger && log && log->file)
    {
        logger = cmn_logger_init(log->file,log->level,log->max_size,log->roll_num);
        CMN_LOGGER_DEBUG(logger,"logger init ok");
    }
	if (!logger)
	{
		printf("rtdb_cli_create : logger init fail!\n");
		goto ERR_END;
	}
	if (-1 == rtdb_cli_fill_cliClients(cli, rtdbConfig)) goto ERR_END;
	pthread_mutex_lock(&cli->mtx);
	cli->sockObj = rtdb_cli_get_domainGoup(&cli->sockNum);
	pthread_mutex_unlock(&cli->mtx);
	if (!cli->sockObj) goto ERR_END;
	sys_conf_free_rtdb_domain(rtdbConfig);
    return cli;
ERR_END :
	if(logger)
		CMN_LOGGER_ERR(logger, "rtdb_cli_create : error occur(domainId=%d)!", cli->domainId);
	else
		printf("rtdb_cli_create : error occur(domainId=%d)!\n", cli->domainId);
	if(rtdbConfig) sys_conf_free_rtdb_domain(rtdbConfig);
	if (cli) rtdb_cli_free(cli);
	return NULL;
}
