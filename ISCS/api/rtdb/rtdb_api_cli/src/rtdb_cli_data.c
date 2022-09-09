/*
 * @Copyright Reserved by XXXX.
 * variant definition and operations.
 * Create by KanWenDi, 2018.06.19.
 * Histories:
 */
#include "rtdb_cli_data.h"
#include "rtdb_cli_data_public.h"
#include "rtdb_cli_data_private.h"
#include "rtdb_cli_cmn.h"
#include "rtdb_cli_protocol.h"
#include "rtdb_cli_path.h"
#include "rtdb_cli_net.h"
#include <stddef.h>
#include <pthread.h>

#ifdef WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

extern cmn_logger* logger;
    
/***********************************************************************
 *
 * 内部数据结构定义。这些数据结构不对外开放结构体内容，仅对外开放类型定义。
 * 
 **********************************************************************/
struct _rtdb_cli_record
{
    int num; // column number
    char **names;// column names
    rtdb_cli_variant **values;	// column values.
};





/***********************************************************************
 *
 * 对外开放的方法接口实现
 * 
 **********************************************************************/
rtdb_cli_variant* rtdb_cli_variant_create()
{
    rtdb_cli_variant *vt = malloc(sizeof(*vt));

    if (vt) memset(vt, 0, sizeof(*vt));

    return vt;
}

void rtdb_cli_variant_free(rtdb_cli_variant *vt)
{
    if (!vt) return;
    if (vt->type == RTDB_CLI_VARIANT_STRING && vt->v.string)
    {
        rtdb_cli_cmn_release(vt->v.string);
    }
    rtdb_cli_cmn_release(vt);
}

rtdb_cli_variant* rtdb_cli_variant_dup(const rtdb_cli_variant* v)
{
	rtdb_cli_variant *copy = NULL;

	if (!v || !(copy = rtdb_cli_variant_create()))
	{
		return NULL;
	}

	copy->type = v->type;
	switch (copy->type)
	{
	case RTDB_CLI_VARIANT_BOOL:
	case RTDB_CLI_VARIANT_INT8:
	case RTDB_CLI_VARIANT_INT16:
	case RTDB_CLI_VARIANT_INT32:
	case RTDB_CLI_VARIANT_INT64:
		copy->v.integer = v->v.integer;
		break;
	case RTDB_CLI_VARIANT_FLOAT:
	case RTDB_CLI_VARIANT_DOUBLE:
		copy->v.real = v->v.real;
		break;
	case RTDB_CLI_VARIANT_STRING:
		copy->v.string = strdup(v->v.string);
		break;
	default:		
        rtdb_cli_cmn_release(copy);
		break;
	}

	return copy;
}

char* rtdb_cli_variant_tostring(const rtdb_cli_variant *vt)
{
    size_t rsLen;
    char *rs = NULL;
    char buf[128] = {0};

    switch(vt->type)
    {
        case RTDB_CLI_VARIANT_BOOL :
        case RTDB_CLI_VARIANT_INT8 :
        case RTDB_CLI_VARIANT_INT16 :
        case RTDB_CLI_VARIANT_INT32 :         
        case RTDB_CLI_VARIANT_INT64 :
            sprintf(buf, "%lld", vt->v.integer);
            break;
        case RTDB_CLI_VARIANT_FLOAT :
        case RTDB_CLI_VARIANT_DOUBLE :
            sprintf(buf, "%lf", vt->v.real);
            break;
        case RTDB_CLI_VARIANT_STRING :
            rsLen = strlen(vt->v.string);
            if((rs = malloc(rsLen + 1)))
            {
                memcpy(rs, vt->v.string, rsLen);    
                rs[rsLen] = '\0';     
            }
            return rs;
        default:
            return NULL;
    }
    
    rsLen = strlen(buf);
    if((rs = malloc(rsLen + 1)))
    {
        memcpy(rs, buf, rsLen);
        rs[rsLen] = '\0';
    }
    
    return rs;        
}

void rtdb_cli_variant_freestring(char* s)
{
    rtdb_cli_cmn_release(s);
}

rtdb_cli_record* rtdb_cli_record_create(int fieldNum)
{
    rtdb_cli_record *r = malloc(sizeof(*r));

    if (r)
    {
        memset(r, 0, sizeof(*r));
        if (fieldNum > 0)
        {
            r->names = malloc(sizeof(char*)*fieldNum);
            if (!r->names)
            {
                rtdb_cli_cmn_release(r);
                return NULL;
            }
            r->values = malloc(sizeof(rtdb_cli_variant*)*fieldNum);
            if (!r->values)
            {
                rtdb_cli_cmn_release(r->names);
                rtdb_cli_cmn_release(r);
                return NULL;
            }

            r->num = fieldNum;
            memset(r->names,0,sizeof(char*)*fieldNum);
            memset(r->values,0,sizeof(rtdb_cli_variant*)*fieldNum);
        }
    }

    return r;
}

void rtdb_cli_record_free(rtdb_cli_record* r)
{
    int i;

    if (!r) return;
    if (r->names)
    {
        for (i = 0; i < r->num; i++)
        {
            if (r->names[i]) rtdb_cli_cmn_release(r->names[i]);
        }
        rtdb_cli_cmn_release(r->names);
    }
    if (r->values)
    {
        for (i = 0; i < r->num; i++)
        {
            if (r->values[i]) 
            {
                rtdb_cli_variant_free(r->values[i]);
                r->values[i] = NULL;
            }
        }
        rtdb_cli_cmn_release(r->values);
    }
    rtdb_cli_cmn_release(r);
}

rtdb_cli_record* rtdb_cli_record_dup(const rtdb_cli_record* r)
{
	rtdb_cli_record *retRecord = NULL;
	int fieldNum = 0, i = 0;
	rtdb_cli_variant *value = NULL;
	char *pKey = NULL;

	if (!r || !(fieldNum = rtdb_cli_record_getsize(r))) return NULL;
	if (!(retRecord = rtdb_cli_record_create(fieldNum))) return NULL;
	for (i = 0; i < fieldNum; i++)
	{
		pKey = rtdb_cli_record_getfieldName(r, i);
		if (!pKey) goto ERR_END;
		value = rtdb_cli_record_getvalue(r, pKey);
		if (!value || !(value = rtdb_cli_variant_dup(value))) goto ERR_END;
		rtdb_cli_record_setvalue(retRecord, pKey, value);
	}
	return retRecord;
ERR_END:
	if (retRecord) rtdb_cli_record_free(retRecord);
	return NULL;
}

int rtdb_cli_record_setvalue(rtdb_cli_record* r, const char* fieldName, rtdb_cli_variant* value)
{
    int i, found;

    if (!r || !fieldName || !value) return -1;

    found = 0;
    if (r->names)
    {
        for (i = 0; i < r->num; i++)
        {
            if (r->names[i] && !strcmp(r->names[i], fieldName))
            {
                found = 1;
                break;
            }
        }
    }

    if (found)
    {
        if (!r->values) return -1;
        if (r->values[i])
        {
            rtdb_cli_variant_free(r->values[i]);
            r->values[i] = NULL;
        }            
        r->values[i] = value;
        return 0;
    }
    
    found = 0;
	for (i = 0; i < r->num; i++)
	{
		if (!r->names[i])
		{
			found = 1;
			break;
		}
	}
		
	if (!found)
	{
		char **names;
		rtdb_cli_variant **values;
		
		names = realloc(r->names, sizeof(char*)*(r->num+1));
		if(!names) return -1;
		names[r->num] = strdup(fieldName);
		if (!names[r->num]) return -1;
		
		values = realloc(r->values, sizeof(rtdb_cli_variant*)*(r->num+1));
		if (!values) return -1;
		values[r->num] = value;
		
		r->names = names;
		r->values = values;
		++ r->num;
		
		return 0;
	}
	
	r->names[i] = strdup(fieldName);
	if (!r->names[i]) return -1;
	r->values[i] = value;
	
	return 0;
}

const int rtdb_cli_record_getsize(const rtdb_cli_record* r)
{
    return (r==NULL) ? 0 : r->num;
}

const char* rtdb_cli_record_getfieldName(const rtdb_cli_record* r,int idx)
{
    if(!r || (idx < 0)) return NULL;
    if(idx > r->num -1) return NULL;
    if(!r->names) return NULL;
    return r->names[idx];
}

const rtdb_cli_variant* rtdb_cli_record_getvalue(const rtdb_cli_record* r, const char* fieldName)
{
	int i;
	
	if (!r || !fieldName) return NULL;
	
	for (i = 0; i < r->num; i++)
	{
		if (r->names[i] && !strcmp(r->names[i], fieldName))
		{
			return r->values[i];
		}
	}
	
	return NULL;
}

rtdb_cli_command* rtdb_cli_command_create()
{
	rtdb_cli_command *r = malloc(sizeof(*r));
	
	if (r) memset(r, 0, sizeof(*r));
	
	return r;
}

void rtdb_cli_command_free(rtdb_cli_command* r)
{
	if (!r) return;
    rtdb_cli_cmn_release(r->target);
    rtdb_cli_cmn_release(r);
}




/***********************************************************************
 *
 * 内部接口方法接口实现
 * 
 **********************************************************************/

rtdb_server_node* rtdb_server_node_create()
{
	rtdb_server_node* node = malloc(sizeof(*node));
	if (node) memset(node, 0, sizeof(*node));
	return node;
}

void rtdb_server_node_free(rtdb_server_node *node)
{
	if (!node) return;
	rtdb_cli_cmn_release(node->ip1);
	rtdb_cli_cmn_release(node->ip2);
	rtdb_cli_cmn_release(node->user);
	rtdb_cli_cmn_release(node->pswd);
	rtdb_cli_cmn_release(node);
}

int rtdb_cli_check_node_status(int domain,rtdb_server_node *node, int port, int timeout)
{
	int iRet = -1; // 0 : ok , -1 : fail

	int i = 0,code = 0,tLen = 0;

	rtdb_cli_msg_header *header= NULL;
	unsigned char *bytes = NULL,*hBytes = NULL;

    if(!(header = rtdb_cli_create_msg_header())) goto QUIT_LABEL;
	tLen = RTDB_CLI_MSGHEADERLEN+RTDB_CLI_MD5LEN;

    header->msgLen = tLen - 4;
	header->domain = domain;
    header->msgType = RTDB_CLI_TYPE_HEARTBEAT;
    header->reqType = RTDB_CLI_REQTYPE_REQUEST;
    header->sender = RTDB_CLI_SENDER_CLI;    
    header->receiver = RTDB_CLI_SENDER_RDB;	

	if(!(hBytes = rtdb_cli_dump_msg_header(header))) goto QUIT_LABEL;
	if(!(bytes = calloc(tLen,1))) goto QUIT_LABEL;
	memcpy(bytes,hBytes,RTDB_CLI_MSGHEADERLEN);	

	if (!node || !node->ip1 || !node->ip2 || !port || !timeout)
		return -1;
	CMN_LOGGER_DEBUG(logger, "tLen = %d",tLen);	
	rtdb_cli_trace_bin("rtdb_cli_check_node_status bytes : ",bytes,tLen);
	node->active_ip = NULL;
	if (0 == rtdb_cli_net_check_tcp(node->ip1, port, bytes,tLen,timeout))
	{
		node->active_ip = node->ip1;
		return 0;
	}

	if (0 == rtdb_cli_net_check_tcp(node->ip2, port,bytes,tLen, timeout))
	{
		node->active_ip = node->ip2;
		return 0;
	}
QUIT_LABEL :
	if(hBytes) free(hBytes);
	if(bytes) free(bytes);
	return iRet;
}


char* rtdb_cli_data_gen_uuid()
{
	char buf[64] = { 0 };
#ifdef WIN32
	GUID guid;
	CoCreateGuid(&guid);
	snprintf(buf, sizeof(buf),
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
#else
	uuid_t uu;
	uuid_generate(uu);
	uuid_unparse(uu, buf);
#endif
	return (char*)strdup(buf);
}

rtdb_cli_variant* rtdb_cli_strvalue_to_variant(int vtType, const char *value)
{
	rtdb_cli_variant *vt = NULL;

	if (vtType < 0 || !value) return NULL;
	vt = rtdb_cli_variant_create();
	vt->type = vtType;
	switch (vt->type)
	{
	case RTDB_CLI_VARIANT_BOOL:
	case RTDB_CLI_VARIANT_INT8:
	case RTDB_CLI_VARIANT_INT16:
	case RTDB_CLI_VARIANT_INT32:
		vt->v.integer = atol(value);
		CMN_LOGGER_DEBUG(logger, "value(%s), vt->v.integer : %ld", value, vt->v.integer);
		break;
	case RTDB_CLI_VARIANT_INT64:
		vt->v.integer = atoll(value);
		CMN_LOGGER_DEBUG(logger, "value(%s), vt->v.integer : %ld", value, vt->v.integer);
		break;
	case RTDB_CLI_VARIANT_FLOAT:
	case RTDB_CLI_VARIANT_DOUBLE:
		vt->v.real = atof(value);
		break;
	case RTDB_CLI_VARIANT_STRING:
		vt->v.string = strdup(value);
		break;
	default:
		rtdb_cli_variant_free(vt);
		vt = NULL;
		break;
	}
	return vt;
}

rtdb_cli_bindata* rtdb_cli_data_create_bindata(int num)
{
	rtdb_cli_bindata *bindata = NULL;
	if (num <= 0) return NULL;
	bindata = calloc(sizeof(*bindata),1);
	if (!bindata) return NULL;
	bindata->arrBytes = calloc(num * sizeof(unsigned char*),1);
	if (!bindata->arrBytes)
	{
		rtdb_cli_cmn_release(bindata);
		return NULL;
	}
	bindata->arrBytesLen = calloc(num * sizeof(int),1);
	if (!bindata->arrBytesLen)
	{
		rtdb_cli_cmn_release(bindata->arrBytes);
		rtdb_cli_cmn_release(bindata);
		return NULL;
	}

	bindata->num = num;

	return bindata;
}

void rtdb_cli_data_free_bindata(rtdb_cli_bindata *bindata)
{
	int i = 0;
	if (!bindata) return;
	if (bindata->arrBytes)
	{
		for (i = 0; i < bindata->num; i++)
			rtdb_cli_cmn_release(bindata->arrBytes[i]);
		rtdb_cli_cmn_release(bindata->arrBytes);
	}
	rtdb_cli_cmn_release(bindata->arrBytesLen);
	rtdb_cli_cmn_release(bindata);
}


static unsigned char* rtdb_cli_data_get_newPathBytes(const unsigned char *pathBytes,int pathBytesLen,
	const unsigned char *nameBytes,int nameBytesLen,int *newPathBytesLen
)
{
	unsigned char *newPathBytes=NULL;

	if(!pathBytes) return NULL;
	if(!nameBytes) nameBytesLen = 0;
	*newPathBytesLen = pathBytesLen + nameBytesLen;
	if (!(newPathBytes = calloc(*newPathBytesLen, 1))) return NULL;	
	CMN_LOGGER_DEBUG(logger,"pathBytesLen = %d, newPathBytesLen = %d",pathBytesLen, *newPathBytesLen);
	
	memcpy(newPathBytes,pathBytes,pathBytesLen);
	if(nameBytes)
		memcpy(newPathBytes + pathBytesLen,nameBytes,nameBytesLen);
	return newPathBytes;
}

int rtdb_cli_data_fill_bindata_paths(rtdb_cli_bindata *bindata,int8_t attrNum,const rtdb_cli_list *paths,const unsigned char *nameBytes,int nameBytesLen)
{
	unsigned char *pathBytes = NULL,*p = NULL;
	int bytesLen = 0, pathBytesLen = 0,newPathBytesLen = 0;
	const char *path = NULL;
	int i = 0;
	
	if(!paths || !bindata) return -1;
	CMN_LOGGER_DEBUG(logger, "size(paths) : %d", rtdb_cli_list_get_size(paths));
	path = rtdb_cli_list_get_first(paths);
	while (path != NULL)
	{
		CMN_LOGGER_DEBUG(logger, "path : %s", path);
		pathBytes = rtdb_cli_data_get_pathBytes(path,&pathBytesLen,0);
		if (!pathBytes)
		{
			CMN_LOGGER_ERR(logger, "rtdb_cli_data_get_pathBytes error");
			goto ERR_END;
		}
		newPathBytesLen = pathBytesLen;
		if(attrNum)
		{
			pathBytes[pathBytesLen-1] = attrNum;
			p = rtdb_cli_data_get_newPathBytes(pathBytes,pathBytesLen,nameBytes,nameBytesLen,&newPathBytesLen);
			free(pathBytes);
			if(!p) goto ERR_END;
		}
		else
			p = pathBytes;
		bindata->arrBytes[i] = p;
		bindata->arrBytesLen[i] = newPathBytesLen;
		bindata->length += newPathBytesLen;
		bytesLen += newPathBytesLen;

		i++;
		path = (char*)rtdb_cli_list_get_next(paths);
	}
	return bytesLen;
ERR_END:
	return -1;
}

 
rtdb_cli_cmd_notifier* rtdb_cli_data_create_cmdNotifier(
	rtdb_cli_msg_rdbcmd *rdbcmd,
	rtdb_cli_cmdResult_notifier *resultNotifier,
	const void *userData
	)
{
	rtdb_cli_cmd_notifier *pNotify = NULL;
	pNotify = calloc(1, sizeof(rtdb_cli_cmd_notifier));
	if (!pNotify) return NULL;
	pNotify->rdbcmd = rdbcmd;
	pNotify->resultNotifier = resultNotifier;
	pNotify->userData = userData;
	return pNotify;
}

void rtdb_cli_data_free_cmdNotifier(rtdb_cli_cmd_notifier *pNotify)
{
	if (!pNotify) return;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_rdbcmd, pNotify->rdbcmd);
	rtdb_cli_cmn_release(pNotify);
}

rtdb_cli_sockObj* rtdb_cli_create_sockObj()
{
	rtdb_cli_sockObj *ret = NULL;

	if (!(ret = calloc(1,sizeof(*ret)))) return NULL;
	pthread_mutex_init(&ret->mtx,NULL);
	ret->subRespList = rtdb_cli_list_create(rtdb_cli_free_subrep,NULL); 
//	ret->subRespList = rtdb_cli_list_create(NULL, NULL);
	if (!ret->subRespList)
	{
		rtdb_cli_cmn_release(ret);
		return NULL;
	}
	return ret;
}

void rtdb_cli_free_sockObj(rtdb_cli_sockObj *obj)
{
	if (!obj) return NULL;
	if (obj->label) rtdb_cli_cmn_release(obj->label);
	close(obj->fd);
	close(obj->subfd);
	if (obj->subRespList) rtdb_cli_list_free(obj->subRespList);
	rtdb_cli_cmn_release(obj);
}

int rtdb_cli_get_domainIdbyLabel(rtdb_cli_sockObj** sockObjs, int sz,char *label)
{
	int ret = -1;
	int i = 0;
	rtdb_cli_sockObj *obj = NULL;

	if (!sockObjs || !label) return -1;
	for(i = 0; i < sz; i++)
	{
		obj = sockObjs[i];
		if (!obj || !obj->label) continue;
		if (!strcmp(obj->label, label))
		{
			ret = obj->domainid;
			break;
		}
	}
	return ret;
}


rtdb_cli_sockObj* rtdb_cli_get_sockObj(rtdb_cli_sockObj** sockObjs,int sz, int domainid)
{
	int i = 0;
	rtdb_cli_sockObj *obj = NULL;

//	CMN_LOGGER_DEBUG(logger,"domainid : %d , sz : %d", domainid,sz);
	if (!sockObjs) return NULL;
	for (i = 0; i < sz; i++)
	{
		obj = sockObjs[i];
		if (!obj) continue;
		//printf("domainid(%d),obj->domainid(%d)", domainid, obj->domainid);
		if (domainid == obj->domainid) return obj;
	}
	return NULL;
}

rtdb_cli_nodeSubInfo* rtdb_cli_create_nodeSubInfo(const char *path,const rtdb_cli_list *names)
{
	rtdb_cli_nodeSubInfo *info = NULL;
	char *name = NULL,*tmp = NULL;
	
	if(!path || !names) return NULL;
	if(!(info = calloc(1,sizeof((*info))))) return NULL;
	if(!(info->path = strdup(path))) goto ERR_END;
	info->names = rtdb_cli_list_create(free,NULL);
	if(!info->names) goto ERR_END;

	name = rtdb_cli_list_get_first(names);
	while(name)
	{
		if(name && (tmp = strdup(name)))
		{
			rtdb_cli_list_push_tail(info->names,tmp);
		}
		name = rtdb_cli_list_get_next(names);
	}
	return info;
ERR_END:
	rtdb_cli_free_nodeSubInfo(info);
	return NULL;
}

void rtdb_cli_free_nodeSubInfo(rtdb_cli_nodeSubInfo *info)
{
	if(!info) return ;
	rtdb_cli_cmn_release(info->path);
	rtdb_cli_cmn_release2(rtdb_cli_list_free,info->names);
	rtdb_cli_cmn_release(info);
}


int rtdb_cli_check_nodeSubInfo(rtdb_cli_list *L,const char *path)
{
	// 0 : no ; 1 : yes
	int ret = 0;
	rtdb_cli_nodeSubInfo *info = NULL;

	if(!L || !path) return 0;

	info = rtdb_cli_list_get_first(L);
	while(info)
	{
//		CMN_LOGGER_DEBUG(logger,"rtdb_cli_check_nodeSubInfo: path(%s),info->path(%s)",path,info->path);
		if(info->path && (!strcmp(path,info->path)))
			ret = 1;
		info = rtdb_cli_list_get_next(L);
	}
	return ret;
}

int rtdb_cli_remove_nodeSubInfo(rtdb_cli_list *L,const char *path)
{
	rtdb_cli_nodeSubInfo *info = NULL;

	if(!rtdb_cli_check_nodeSubInfo(L,path)) return -1;
	rtdb_cli_list_remove(L,info);
	return 0;
}

unsigned char* rtdb_cli_data_dump_variant(rtdb_cli_variant *vt, int *outLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int bytesLen = 0, curLen = 0;
	int16_t sLen = 0;

	if (!vt || !outLen) return NULL;
	switch (vt->type)
	{
	case RTDB_CLI_VARIANT_BOOL:
	case RTDB_CLI_VARIANT_INT8:
	case RTDB_CLI_VARIANT_INT16:
	case RTDB_CLI_VARIANT_INT32:
	case RTDB_CLI_VARIANT_INT64:
		bytesLen = 1 + 8;
		if (!(p = bytes = calloc(bytesLen, 1))) return NULL;
		bytes[0] = vt->type;
		p += 1;
		rtdb_cli_encode_int64(vt->v.integer, p, curLen);
		CMN_LOGGER_DEBUG(logger, "vt->v.integer : %ld", vt->v.integer);
		break;
	case RTDB_CLI_VARIANT_FLOAT:
	case RTDB_CLI_VARIANT_DOUBLE:
		bytesLen = 1 + 8;
		if (!(p = bytes = calloc(bytesLen, 1))) return NULL;
		bytes[0] = vt->type;
		p += 1;
		rtdb_cli_encode_float64(vt->v.real, p, curLen);
		CMN_LOGGER_DEBUG(logger, "vt->v.real : %s", vt->v.real);
		break;
	case RTDB_CLI_VARIANT_STRING:
		if (!vt->v.string) return NULL;
		sLen = strlen(vt->v.string);
		bytesLen = 1 + 2 + sLen;
		if (!(p = bytes = calloc(bytesLen, 1))) return NULL;
		bytes[0] = vt->type;
		p += 1;
		rtdb_cli_encode_int16(sLen, p, curLen);
		rtdb_cli_encode_bytes(vt->v.string, p, sLen, curLen);
		CMN_LOGGER_DEBUG(logger, "vt->v.string : %s", vt->v.string);
		break;
	default:
		break;
	}
	rtdb_cli_trace_bin("bytes", bytes, bytesLen);
	*outLen = bytesLen;
	return bytes;
}

int rtdb_cli_data_check_subresp(rtdb_cli_list *subRespList,int type,const char* subId,int seqnum, int subNum,int *flag)
{
	/* 0 : false ; 1 : true */
	int i = 0,cond = 0;
	rtdb_cli_subrep *subrep = NULL;

	if(!subRespList || !flag) goto ERR_END;
//	CMN_LOGGER_DEBUG(logger,"rtdb_cli_list_get_size(subRespList) = %d",rtdb_cli_list_get_size(subRespList));
	subrep = rtdb_cli_list_get_first(subRespList);
	while(subrep)
	{
		cond = (0 == strncmp(subId, subrep->id, RTDB_CLI_UUIDLEN));
		if (RTDB_CLI_SUB_RDBTBL == type || RTDB_CLI_SUB_RDBNODE == type)
			cond = cond && (seqnum == subrep->seqnum);
		//CMN_LOGGER_DEBUG(logger, "subId(%s),subrep->id(%s),seqnum(%d),subreq->seqnum(%d),cond(%d)", 
		//	subId,subrep->id,seqnum,subrep->seqnum,cond);
		if (cond)
		{
			if (subrep->num != subNum)
			{
				CMN_LOGGER_ERR(logger, "subrep->num(%d) != subNum(%d)", subrep->num, subNum);
				*flag = 0;
			}
			else
			{
				*flag = 1;
				for (i = 0; i < subrep->num; i++)
					*flag = *flag && subrep->resultArr[i];
		//		CMN_LOGGER_DEBUG(logger, "flag = %d", *flag);
			}
			break;
		}
		subrep = rtdb_cli_list_get_next(subRespList);
	}
	if (cond) rtdb_cli_list_remove(subRespList, subrep);	
//	CMN_LOGGER_DEBUG(logger,"rtdb_cli_data_check_subresp : cond(%d),*flag(%d)",cond,*flag);
	return cond;
ERR_END:
	CMN_LOGGER_ERR(logger,"rtdb_cli_data_check_subresp fail!");
	return 0;
}

rtdb_cli_record* rtdb_cli_data_fill_record(const rtdb_cli_node_item *pNodeItem)
{
	rtdb_cli_record *record = NULL;
	int code = 0,i = 0;
	rtdb_cli_attr_item *pAttrItem = NULL;
	rtdb_cli_variant *v = NULL;
	
	if (!pNodeItem) return NULL;
//	CMN_LOGGER_DEBUG(logger, "rtdb_cli_query_node : pNodeItem->aNum = %d", pNodeItem->aNum);
	record = rtdb_cli_record_create(pNodeItem->aNum);
    if (!record) return NULL;

	for(i = 0; i < pNodeItem->aNum; i++)
	{
		pAttrItem = pNodeItem->attrItems[i];
		if (!pAttrItem || !pAttrItem->aName || !strlen(pAttrItem->aName) || !pAttrItem->vt) goto ERR_END;
		if(!(v = rtdb_cli_variant_dup(pAttrItem->vt))) goto ERR_END;
		code = rtdb_cli_record_setvalue(record,pAttrItem->aName,v);
		if (-1 == code)
		{
			rtdb_cli_variant_free(v);
			goto ERR_END;
		}
	}	
	
	return record;
	
ERR_END:
	CMN_LOGGER_ERR(logger, "rtdb_cli_query_node error");
	if(record) rtdb_cli_record_free(record);
	return NULL;
} 

rtdb_cli_list* rtdb_cli_data_get_nkeyList(rtdb_cli_node_item *pNodeItem)
{
	rtdb_cli_list *retList = NULL;
	int i = 0;
	rtdb_cli_attr_item *pAttrItem = NULL;
	char *pvalue = NULL;

	if (!pNodeItem) return NULL;
//	CMN_LOGGER_DEBUG(logger, "rtdb_cli_data_get_nkeyList : pNodeItem->aNum = %d", pNodeItem->aNum);

	retList = rtdb_cli_list_create(free, NULL);
	if (!retList) return NULL;
	for (i = 0; i < pNodeItem->aNum; i++)
	{
		pAttrItem = pNodeItem->attrItems[i];
		if (!pAttrItem || !pAttrItem->aName || !strlen(pAttrItem->aName)) goto ERR_END;
		if (!(pvalue = strdup(pAttrItem->aName))) goto ERR_END;
		rtdb_cli_list_push_tail(retList, pvalue);
	}
	return retList;

ERR_END:
	CMN_LOGGER_ERR(logger, "rtdb_cli_query_node error");
	if (retList) rtdb_cli_list_free(retList);
	return NULL;
}

unsigned char* rtdb_cli_data_get_pathBytes(const char* spath,int *outLen,int attrFlag)
{
	unsigned char *bytes = NULL;
	rtdb_cli_path *path = NULL;

	if (!spath || !outLen || !(path = rtdb_cli_path_parse(spath)))
	{
		CMN_LOGGER_WARN(logger, "parse path failed");
		return NULL;
	}
	bytes = rtdb_cli_path_dump(path, outLen, attrFlag);
	if (!bytes)
		CMN_LOGGER_WARN(logger, "rtdb_cli_path_dump failed");
	rtdb_cli_path_free(path);

	return bytes;
}

unsigned char* rtdb_cli_data_get_updateNodeBytes(int cmdType, const char* path, rtdb_cli_variant *vt,int *outLen)
{
	unsigned char *bytes = NULL, *pathBytes = NULL, *vtBytes = NULL, *p = NULL;
	int bytesLen = 0, pathBytesLen = 0,vtBytesLen = 0;
	int16_t type = cmdType;
//	int32_t rowNum = 1, rowId = 1;
	int curLen = 0;

	if (!path) return NULL;

	pathBytes = rtdb_cli_data_get_pathBytes(path, &pathBytesLen, 1);
	if (!pathBytes) goto QUIT_LABEL;
	vtBytes = rtdb_cli_data_dump_variant(vt,&vtBytesLen);
	if (!vtBytes) goto QUIT_LABEL;
	rtdb_cli_trace_bin("vtBytes", vtBytes, vtBytesLen);

	//bytesLen = 2 + 4 + 4 + pathBytesLen + vtBytesLen;
	bytesLen = 2 + pathBytesLen + vtBytesLen;
	if (!(p = bytes = calloc(bytesLen, 1))) goto QUIT_LABEL;

	rtdb_cli_encode_int16(type, p, curLen);
//	rtdb_cli_encode_int32(rowNum, p, curLen);
//	rtdb_cli_encode_int32(rowId, p, curLen);
	rtdb_cli_encode_bytes(pathBytes, p, pathBytesLen, curLen);
	rtdb_cli_encode_bytes(vtBytes, p, vtBytesLen, curLen);
	
	*outLen = bytesLen;
QUIT_LABEL:
	if (pathBytes) free(pathBytes);
	if (vtBytes) free(vtBytes);
	return bytes;
}

unsigned char* rtdb_cli_data_get_queryNodeBytes(int cmdType,const char* spath,int *outLen)
{
	unsigned char *bytes = NULL, *pathBytes = NULL,*p = NULL;
    int bytesLen = 0,pathBytesLen = 0;
	int16_t type = cmdType;
	int32_t rowNum = 1, rowId = 1;
	int curLen = 0;

	if (!spath) return NULL;
	pathBytes = rtdb_cli_data_get_pathBytes(spath, &pathBytesLen, 1);
	if (!pathBytes)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_data_get_pathBytes error");
		return NULL;
	}

	bytesLen = 2 + 4 + 4 + pathBytesLen;
	if (!(p = bytes = calloc(bytesLen, 1)))
	{
		free(pathBytes);
		return NULL;
	}
	rtdb_cli_encode_int16(type,p,curLen);
	rtdb_cli_encode_int32(rowNum, p, curLen);
	rtdb_cli_encode_int32(rowId, p, curLen);
	rtdb_cli_encode_bytes(pathBytes, p, pathBytesLen, curLen);

	*outLen = bytesLen;
	free(pathBytes);
	return bytes;
}

unsigned char* rtdb_cli_data_get_mqueryNodeBytes(int cmdType, const rtdb_cli_list *paths, const rtdb_cli_list *names, int *outLen)
{
	unsigned char *bytes = NULL,*nameBytes = NULL,*p = NULL;
	int bytesLen = 0,tmpLen=0,curLen = 0,i = 0;
	int8_t attrNum = 0;
	int16_t type = cmdType;
	int32_t rowNum = 0, rowId = 0;
	rtdb_cli_bindata *bindata = NULL;
	int nameBytesLen = 0;

	if (!paths || !outLen) return NULL;
	rowNum = rtdb_cli_list_get_size((rtdb_cli_list*)paths);
	if(!rowNum) return NULL;
	if(names)
	{
		attrNum = rtdb_cli_list_get_size(names);
		if (!attrNum) return NULL;
		if (-1 == rtdb_cli_data_dump_names(&nameBytes,&nameBytesLen,names))
			goto ERR_LABEL;
	}
	
	CMN_LOGGER_DEBUG(logger, "attrNum : %d", attrNum);
	if (!(bindata = rtdb_cli_data_create_bindata(rowNum))) goto ERR_LABEL;
	tmpLen = rtdb_cli_data_fill_bindata_paths(bindata,attrNum,paths,nameBytes,nameBytesLen);
	if(tmpLen <= 0) goto ERR_LABEL;
	bytesLen = 2 + 4 + tmpLen + rowNum * 4;
	if (!(p = bytes = calloc(bytesLen,1))) goto ERR_LABEL;
	rtdb_cli_encode_int16(type, p, curLen);
	rtdb_cli_encode_int32(rowNum, p, curLen);
	for (i = 0; i < rowNum; i++)
	{
		rowId = i + 1;
		rtdb_cli_encode_int32(rowId, p, curLen);
		rtdb_cli_encode_bytes(bindata->arrBytes[i], p, bindata->arrBytesLen[i], curLen);
	}
	*outLen = bytesLen;
	goto QUIT_LABEL;

ERR_LABEL:
	rtdb_cli_cmn_release(bytes);
QUIT_LABEL:
	if(nameBytes) free(nameBytes);
	if(bindata) rtdb_cli_data_free_bindata(bindata);
	return bytes;
}

unsigned char* rtdb_cli_data_get_queryTableBytes(int cmdType,const char *tableName,const char *cond, const rtdb_cli_list *filters,int *outLen)
{
	unsigned char *bytes = NULL,*p = NULL;
	int8_t fieldNum = 0;
    int bytesLen = 0,curLen = 0;
	int16_t type = cmdType;
	char nameBuf[129] = {0},*pFiedName = NULL;

    if(!tableName) return NULL;     
	if(filters)	fieldNum = rtdb_cli_list_get_size(filters);
	bytesLen = 2 + 32 + 128 + 1 + fieldNum * 64 ;
	if (!(bytes = calloc(bytesLen, 1))) return NULL;
	p = bytes;
	
	rtdb_cli_encode_int16(type,p,curLen);
	
	snprintf(nameBuf,sizeof(nameBuf),"%s",tableName);
	rtdb_cli_encode_bytes(nameBuf,p,32,curLen);
	
	snprintf(nameBuf,sizeof(nameBuf),"%s",cond?cond:"");
	rtdb_cli_encode_bytes(nameBuf,p,128,curLen);
	
	rtdb_cli_encode_int8(fieldNum,p,curLen);
	if(filters)
    {
        pFiedName = rtdb_cli_list_get_first(filters);
        while(pFiedName != NULL)
        {  
			snprintf(nameBuf,sizeof(nameBuf),"%s",pFiedName);
			rtdb_cli_encode_bytes(nameBuf,p,64,curLen);
            pFiedName = rtdb_cli_list_get_next(filters);
        }    
    }
	*outLen = bytesLen;
	return bytes;
}

unsigned char* rtdb_cli_data_get_updateRecordBytes(int cmdType, const char *tableName, const char *cond,const rtdb_cli_record *record,int *outLen)
{
	unsigned char *bytes = NULL, *vtBytes = NULL, *p = NULL;
	int bytesLen = 0, vtBytesLen = 0,curLen = 0,i = 0;
	int8_t fieldNum = 0;
	int16_t type = cmdType;
	rtdb_cli_bindata *bindata = NULL;
	rtdb_cli_variant *value = NULL;
	char *pKey = NULL, pBuf[129] = { 0 }, *fName = NULL;

	if (!tableName || !record || !(fieldNum = rtdb_cli_record_getsize(record))) return NULL;
	bindata = rtdb_cli_data_create_bindata(2*fieldNum);
	if (!bindata) return NULL;
	bytesLen = 2 + 32 + 128 + 1;
	for (i = 0; i < fieldNum; i++)
	{
		pKey = rtdb_cli_record_getfieldName(record,i);
		if (!pKey) goto ERR_LABEL;
		value = rtdb_cli_record_getvalue(record,pKey);
		if(!value) goto ERR_LABEL;
		vtBytes = rtdb_cli_data_dump_variant(value,&vtBytesLen);
		if(!vtBytes) goto ERR_LABEL;
		snprintf(pBuf,sizeof(pBuf),"%s",pKey);

		if (!(fName = calloc(65, 1)))
		{
			free(vtBytes);
			goto ERR_LABEL;
		}
		snprintf(fName,65,"%s",pKey);
		
		bindata->arrBytes[2 * i] = fName;
		bindata->arrBytesLen[2 * i] = 64;
		bindata->length += 64;
		bytesLen += 64;

		bindata->arrBytes[2*i+1] = vtBytes;
		bindata->arrBytesLen[2*i+1] = vtBytesLen;
		bindata->length += vtBytesLen;
		bytesLen += vtBytesLen;
	}
	
	if (!(p = bytes = calloc(bytesLen, 1))) goto ERR_LABEL;
	rtdb_cli_encode_int16(type, p, curLen);
	snprintf(pBuf,sizeof(pBuf),"%s",tableName);
	rtdb_cli_encode_bytes(pBuf,p,32,curLen);
	snprintf(pBuf,sizeof(pBuf),"%s",cond?cond:"");
	rtdb_cli_encode_bytes(pBuf,p,128,curLen);
	rtdb_cli_encode_int8(fieldNum,p,curLen);
	for (i = 0; i < 2*fieldNum; i++)
		rtdb_cli_encode_bytes(bindata->arrBytes[i],p,bindata->arrBytesLen[i],curLen);
	*outLen = bytesLen;
	goto QUIT_LABEL;

ERR_LABEL:
	rtdb_cli_cmn_release(bytes);
QUIT_LABEL:
	if(bindata) rtdb_cli_data_free_bindata(bindata);
	return bytes;
}


/*
0 unequal, !0 equal
*/
int rtdb_cli_data_cmpstr(const char *val1, const char *val2)
{
	int itmp = 0;
	if (!val1 && !val2) return 0;
	itmp = strcmp(val1, val2);
	return !itmp;
}

/*
0 unequal, !0 equal
*/
int rtdb_cli_data_cmpsub(rtdb_cli_subscriber *val, rtdb_cli_subscriber *key) 
{
	int itmp = 0;

	if(!val || !key) return 0;
	itmp = strncmp(val->uuid, key->uuid, RTDB_CLI_UUIDLEN);
	return !itmp;
}

rtdb_cli_subscriber* rtdb_cli_data_find_sub(rtdb_cli_list *subList,const char *uuid)
{
    rtdb_cli_subscriber *pRet = NULL;
    int find = 0;
    if(!subList || !uuid) return NULL;

    pRet = rtdb_cli_list_get_first(subList);
    while(pRet)
    {
		if (0 == strncmp(pRet->uuid, uuid, RTDB_CLI_UUIDLEN))
        {
            find = 1;
            break;
        }
        pRet = rtdb_cli_list_get_next(subList);
    }
    if(!find) pRet = NULL;    
    return pRet;
}

int rtdb_cli_data_add_subrep(rtdb_cli_sockObj *obj,unsigned char *bytes,int msgLen)
{
	int code = 0;
	rtdb_cli_subrep *subrep = NULL;
	
	if (!obj || !obj->subRespList || !bytes || msgLen <= 0) return -1;
	subrep = rtdb_cli_parse_subrep(bytes, msgLen);
	if(!subrep) return -1;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_data_add_subrep , subrep->id(%s)", subrep->id);
	if(!rtdb_cli_list_push_tail(obj->subRespList,subrep)) code = -1;
	if(-1 == code)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_list_push_tail fail!");
		rtdb_cli_free_subrep(subrep);
		return -1;
	}
	return 0;
}

int rtdb_cli_data_dump_names(unsigned char **out,int *outLen,const rtdb_cli_list *names)
{
	char *name = NULL, *newName = NULL;
	unsigned char *p = NULL;
	int isErr = 0,sz = 0,totalLen = 0;
	int iName = 0, iNameLen = 0;

	if(!out || !outLen || !names) return -1;
	sz = rtdb_cli_list_get_size(names);
	totalLen = sz * 64;
	*outLen = totalLen;
	if (!(p = *out = calloc(totalLen, 1))) return -1;
	name = rtdb_cli_list_get_first((rtdb_cli_list*)names);
	while (name != NULL)
	{
		CMN_LOGGER_DEBUG(logger, "name : %s", name);

		iNameLen = strlen(name);
		for (iName = 0; iName < iNameLen; iName++)
		{
			if (name[iName] == '.') break;
		}
		if (iName == iNameLen)
		{
			// have no dot
			snprintf(p, 33, "%s", name);
		}
		else
		{
			// have dot
			newName = strdup(name);
			if (!newName)
			{
				isErr = 1;
				break;
			}
			newName[iName] = '\0';
			snprintf(p, 33, "%s", newName);
			snprintf(p + 32, 33, "%s", newName + iName + 1);
			rtdb_cli_cmn_release(newName);
		}
		p += 64;
		name = (char*)rtdb_cli_list_get_next((rtdb_cli_list*)names);
	}
	if (isErr)
	{
		free(*out);
		*out = NULL;
		return -1;
	}
	return 0;
}

rtdb_cli_reply* rtdb_cli_data_get_reply(int fd,unsigned char *bytes,int bytesLen)
{
	rtdb_cli_reply *reply = NULL;
	int recvLen = 0, readLen = 0;
	unsigned char *recvBuf = NULL;

	if(!bytes) return NULL;
	rtdb_cli_trace_bin("bytes send : ",bytes,bytesLen);

	// send 
	if (rtdb_cli_net_write(fd, bytes, bytesLen) < 0) return NULL;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_net_write ok(fd=%d,bytesLen=%d)",fd,bytesLen);

	// recv
	if (rtdb_cli_net_read(fd, &recvLen, 4) < 0)
	{
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_net_read error ");
		return NULL;
	}
	recvLen = rtdb_cli_ntohi32(recvLen);
	CMN_LOGGER_DEBUG(logger, "recvLen = %d ", recvLen);
	if (!(recvBuf = calloc(recvLen+4,1))) return NULL;
	memcpy(recvBuf, &recvLen, 4);
	readLen = rtdb_cli_net_read(fd, recvBuf + 4, recvLen);
	if (readLen != recvLen)
	{
		CMN_LOGGER_DEBUG(logger, "rtdb_cli_net_read error(readLen = %d)", readLen);
		free(recvBuf);
		return NULL;
	}
	reply = rtdb_cli_prase_reply(recvBuf);
	free(recvBuf);
	return reply;
}

unsigned char* rtdb_cli_data_read_bytes(int fd,int *len)
{
    int nread = 0,msgLen = 0;
    unsigned char *bytes = NULL,*p = NULL;
    
	if (fd <= 0) goto ERR_END;
	*len = 0;
	nread = rtdb_cli_net_read(fd,&msgLen,4);
    if(nread <= 0) return NULL;
	if (nread != 4)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_data_read_bytes : nread(%d) != 4",nread);
		return NULL;
	}
    *len = rtdb_cli_ntohi32(msgLen);
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_data_read_bytes len : %d",*len);
    if (*len <= 0 || *len > 1024*1024)
    {
        CMN_LOGGER_ERR(logger,"len(%d) <= 0 or > 1024*1024",*len);
		*len = 0;
        return NULL;
    }
    if(!(bytes = calloc(*len + 4,1))) goto ERR_END;
    memcpy(bytes,&msgLen,4);
	p = bytes + 4;
	nread = rtdb_cli_net_read(fd,p,*len);
	if (nread != *len) 
	{
		CMN_LOGGER_ERR(logger, "nread(%d) != %d", nread, *len);
		goto ERR_END;
	}
	*len += 4;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_data_read_bytes ok");
    return bytes;
	
ERR_END:
	CMN_LOGGER_ERR(logger,"rtdb_cli_data_read_bytes error");
	rtdb_cli_cmn_release(bytes);
	*len = 0;
	return NULL;		
}

int rtdb_cli_data_encode_spath(const char *spath,unsigned char **nBytes,int *nBytesLen)
{
	// encode path nodes 
	int code = -1;
	rtdb_cli_path* path = NULL;

	if (!spath || !nBytes || !nBytesLen) return -1;

	path = rtdb_cli_path_parse(spath);
	if (!path)
	{
		CMN_LOGGER_WARN(logger, "parse path failed");
		return -1;
	}
	*nBytes = rtdb_cli_path_dump_nodes(path,nBytesLen);
	if (!(*nBytes))
	{
		rtdb_cli_path_free(path);
		return -1;
	}
	rtdb_cli_path_free(path);
	return 0;	
}

unsigned char* rtdb_cli_data_get_finalBytes(int domainId,int cmdType,const unsigned char *nBytes,int nBytesLen,int *bytesLen)
{
	unsigned char *bytes = NULL,*hBytes = NULL,*p = NULL;
	
	if(!nBytes || nBytesLen <= 0) return NULL;
	*bytesLen = RTDB_CLI_MSGHEADERLEN + nBytesLen + RTDB_CLI_MD5LEN;
	CMN_LOGGER_DEBUG(logger, "bytesLen = %d ", *bytesLen);
	if (!(p = bytes = calloc(*bytesLen,1))) return NULL;
	hBytes = rtdb_cli_gen_header_bytes(*bytesLen,domainId,cmdType);
	if(!hBytes) 
	{
		free(bytes);
		return NULL;
	}
	memcpy(p,hBytes,RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	memcpy(p,nBytes,nBytesLen);
	rtdb_cli_cmn_release(hBytes);
	return bytes;
}

static int rtdb_cli_fill_record_attrs(rtdb_cli_list* retList, const rtdb_cli_node_item *pNodeItem)
{
	int i = 0;
	rtdb_cli_attr_item *pAttrItem = NULL;
	rtdb_cli_variant *v = NULL;

	if (!pNodeItem) return -1;
//	CMN_LOGGER_DEBUG(logger, "rtdb_cli_fill_record_attrs : pNodeItem->aNum = %d", pNodeItem->aNum);

	for(i = 0; i < pNodeItem->aNum; i++)
	{
		pAttrItem = pNodeItem->attrItems[i];
		if (!pAttrItem || !pAttrItem->vt) goto ERR_END;
		if (!(v = rtdb_cli_variant_dup(pAttrItem->vt))) goto ERR_END;
		if(!rtdb_cli_list_push_tail(retList, v))
		{
			rtdb_cli_variant_free(v);
			goto ERR_END;
		}
	}
	return 0;

ERR_END:
	CMN_LOGGER_ERR(logger, "rtdb_cli_query_node error");
	return -1;
}


rtdb_cli_list* rtdb_cli_data_get_vtList(rtdb_cli_reply *reply)
{
	rtdb_cli_list* retList = NULL;
	int i = 0;

	if (!reply) return NULL;
	retList = rtdb_cli_list_create(rtdb_cli_variant_free, NULL);
	if (!retList) return NULL;

	// fill record
	for (i = 0; i < reply->elenum; i++)
	{
		if (-1 == rtdb_cli_fill_record_attrs(retList, reply->nodeItems[i]))
		{
			rtdb_cli_list_free(retList);
			return NULL;
		}
	}
	return retList;
}

rtdb_cli_list* rtdb_cli_data_get_recordList(rtdb_cli_reply *reply)
{
	rtdb_cli_list* retList = NULL;
	rtdb_cli_record *record = NULL;
	int i = 0;

	if (!reply) return NULL;
	retList = rtdb_cli_list_create(rtdb_cli_record_free, NULL);
	if (!retList) return NULL;

	// fill record
	for (i = 0; i < reply->elenum; i++)
	{		
		record = rtdb_cli_data_fill_record(reply->nodeItems[i]);
		rtdb_cli_list_push_tail(retList, record);
	}
	return retList;
}



