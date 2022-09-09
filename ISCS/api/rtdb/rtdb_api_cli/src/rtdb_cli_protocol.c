#include "rtdb_cli_protocol.h"
#include "rtdb_cli_cmn.h"
#include "rtdb_cli_net.h"
#include "rtdb_cli_data_private.h"
#include "rtdb_cli_data_public.h"
#include "rtdb_cli_data.h"
#include <stdlib.h>

extern cmn_logger* logger;

static int rtdb_client_fill_reply_table(rtdb_cli_reply *reply, unsigned char *buf);
static int rtdb_client_fill_reply_node(rtdb_cli_reply *reply,unsigned char *buf);
static int rtdb_client_fill_reply_variant(rtdb_cli_reply *reply,unsigned char *buf);

rtdb_cli_msg_header* rtdb_cli_create_msg_header()
{
    rtdb_cli_msg_header* header = NULL;
    header = calloc(1,sizeof(*header));
    if(!header) return NULL;    
    return header;
}

void rtdb_cli_free_msg_header(rtdb_cli_msg_header *header)
{
	rtdb_cli_cmn_release(header);
}

unsigned char* rtdb_cli_dump_msg_header(rtdb_cli_msg_header *header)
{
    unsigned char *bytes,*p;
    int curLen = 0;
    bytes = calloc(RTDB_CLI_MSGHEADERLEN,1);
    if(!bytes) return NULL;
    p = bytes;
    rtdb_cli_encode_int32(header->msgLen,p,curLen);
	rtdb_cli_encode_int32(header->domain, p, curLen);
    rtdb_cli_encode_int16(header->msgType,p,curLen);
    rtdb_cli_encode_int8(header->reqType,p,curLen);
    rtdb_cli_encode_int16(header->sender,p,curLen);
    rtdb_cli_encode_int16(header->receiver,p,curLen);
    rtdb_cli_encode_int8(header->version,p,curLen);
    rtdb_cli_encode_int32(header->crc32,p,curLen);
    if(curLen != RTDB_CLI_MSGHEADERLEN)
    {
        CMN_LOGGER_ERR(logger,
            "rtdb_cli_dump_msg_header : curLen(%d) != %d",
            curLen,RTDB_CLI_MSGHEADERLEN);
        free(bytes);
        return NULL;
    }
    return bytes;
}

rtdb_cli_msg_header* rtdb_cli_decode_msg_header(const unsigned char *bytes, int len)
{
	rtdb_cli_msg_header *header;
	unsigned char *p = bytes;

	if (!bytes || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	header = rtdb_cli_create_msg_header();
	if (!header) return NULL;

	rtdb_cli_decode_int32(header->msgLen, p);
	rtdb_cli_decode_int32(header->domain, p);
	rtdb_cli_decode_int16(header->msgType, p);
	rtdb_cli_decode_int8(header->reqType, p);
	rtdb_cli_decode_int16(header->sender, p);
	rtdb_cli_decode_int16(header->receiver, p);
	rtdb_cli_decode_int8(header->version, p);
	rtdb_cli_decode_int32(header->crc32, p);

	return header;
}

unsigned char* rtdb_cli_gen_header_bytes(int bytesLen,int domainId,int msgType)
{
	rtdb_cli_msg_header *header = NULL;
	unsigned char *hBytes = NULL;

	header = rtdb_cli_create_msg_header();
	if (!header) return NULL;
	header->msgLen = bytesLen - 4;
	header->domain = domainId;
	header->msgType = msgType;
	header->reqType = RTDB_CLI_REQTYPE_REQUEST;
	header->sender = RTDB_CLI_SENDER_CLI;
	header->receiver = RTDB_CLI_RECEIVER_RDB;

	hBytes = rtdb_cli_dump_msg_header(header);
	rtdb_cli_free_msg_header(header);
	return hBytes;
}

rtdb_cli_msg_rdbcmd* rtdb_cli_create_msg_rdbcmd(
    int domainId, 
	int type,
    const char* uuid,
	const char* destination, 
	const char *cmdJson
)
{
    rtdb_cli_msg_rdbcmd *rdbcmd;
    short cmdLen = 0;
    int msgLen = 0;
    
    if(!uuid || !cmdJson) return NULL;
    rdbcmd = calloc(1,sizeof(*rdbcmd));
    if(!rdbcmd) return NULL;
    rdbcmd->header = rtdb_cli_create_msg_header();
    if(!rdbcmd->header)
    {
        rtdb_cli_free_msg_rdbcmd(rdbcmd);
        return NULL;
    }
    cmdLen = strlen(cmdJson);
    msgLen = RTDB_CLI_MSGHEADERLEN - 4;
    msgLen += RTDB_CLI_RDBCMDBASELEN + cmdLen + RTDB_CLI_MD5LEN;
    rdbcmd->header->msgLen = msgLen;
	rdbcmd->header->domain = domainId;
    rdbcmd->header->msgType = RTDB_CLI_TYPE_RDBCMD;
    rdbcmd->header->reqType = RTDB_CLI_REQTYPE_REQUEST;
    rdbcmd->header->sender = RTDB_CLI_SENDER_CLI;    
    rdbcmd->header->receiver = RTDB_CLI_SENDER_RDB;
 //   rdbcmd->domainId = domainId;
    rdbcmd->type = type;
    memset(rdbcmd->id,0,sizeof(rdbcmd->id));
	memcpy(rdbcmd->id, uuid, RTDB_CLI_UUIDLEN);
    memset(rdbcmd->destination,0,sizeof(rdbcmd->destination));
    if(destination)
		memcpy(rdbcmd->destination, destination, strlen(destination));
    rdbcmd->cmdLen = cmdLen;
    rdbcmd->cmdJson = calloc(cmdLen + 1,1);
    if(!rdbcmd->cmdJson)
    {
        rtdb_cli_free_msg_rdbcmd(rdbcmd);
        return NULL;        
    }
    snprintf(rdbcmd->cmdJson,cmdLen + 1,"%s",cmdJson);
    
    return rdbcmd;    
}

void rtdb_cli_free_msg_rdbcmd(rtdb_cli_msg_rdbcmd *rdbcmd)
{
    if(!rdbcmd) return ;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header,rdbcmd->header);
	rtdb_cli_cmn_release(rdbcmd->cmdJson);
	rtdb_cli_cmn_release(rdbcmd);
}

rtdb_cli_byte* rtdb_cli_dump_msg_rdbcmd(rtdb_cli_msg_rdbcmd *rdbcmd,int *dumpLen)
{
    rtdb_cli_byte *bytes = NULL,*p = NULL;
    rtdb_cli_byte *headerBytes = NULL;
    int curLen = 0;
    
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_dump_msg_rdbcmd begin");
    if(!rdbcmd || !rdbcmd->header || !rdbcmd->cmdJson || !dumpLen)
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_dump_msg_rdbcmd : dump header fail");
        return NULL;
    }
    *dumpLen = 0;
    *dumpLen += RTDB_CLI_MSGHEADERLEN; 
    *dumpLen += RTDB_CLI_RDBCMDBASELEN;
    *dumpLen += strlen(rdbcmd->cmdJson);
    *dumpLen += RTDB_CLI_MD5LEN;
    
    CMN_LOGGER_DEBUG(logger,"dumpLen : %d",*dumpLen);

    if(!(p = bytes = calloc(*dumpLen,1)))
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_dump_msg_rdbcmd : bytes is NULL");
        return NULL;
    }
    
    headerBytes = rtdb_cli_dump_msg_header(rdbcmd->header);
    if(!headerBytes) 
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_dump_msg_rdbcmd : dump header fail");
        free(bytes);
        return NULL;    
    }
    
    rtdb_cli_encode_bytes(headerBytes,p,RTDB_CLI_MSGHEADERLEN,curLen);
    free(headerBytes);
    rtdb_cli_encode_int32(rdbcmd->type,p,curLen);
	rtdb_cli_encode_bytes(rdbcmd->id, p, RTDB_CLI_UUIDLEN, curLen);
    rtdb_cli_assign_string(rdbcmd->destination,p,sizeof(rdbcmd->destination),curLen);
    rtdb_cli_encode_int16(rdbcmd->cmdLen,p,curLen);
    rtdb_cli_encode_bytes(rdbcmd->cmdJson,p,rdbcmd->cmdLen,curLen); 
    rtdb_cli_encode_bytes(rdbcmd->md5,p,RTDB_CLI_MD5LEN,curLen); 
    
    if(curLen != *dumpLen)
    {
        CMN_LOGGER_ERR(logger,"curLen : %d , *dumpLen : %d",curLen,*dumpLen);
        free(bytes);
        *dumpLen = 0;
        return NULL;        
    }  
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_dump_msg_rdbcmd end");    
    return bytes;
}

rtdb_cli_subrep* rtdb_cli_create_subrep()
{
	rtdb_cli_subrep *subrep = NULL;
	subrep = calloc(1, sizeof(*subrep));
	if (!subrep) return NULL;
	subrep->header = NULL;
	subrep->noArr = NULL;
	subrep->resultArr = NULL;
	return subrep;
}
void rtdb_cli_free_subrep(rtdb_cli_subrep *subrep)
{
	if (!subrep) return;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, subrep->header);
	rtdb_cli_cmn_release(subrep->noArr);
	rtdb_cli_cmn_release(subrep->resultArr);
	rtdb_cli_cmn_release(subrep);
}

rtdb_cli_subrep* rtdb_cli_parse_subrep(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_subrep *subrep = NULL;
	rtdb_cli_byte *p = (rtdb_cli_byte*)bytes;
	int i = 0;

	if (!bytes || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	subrep = rtdb_cli_create_subrep();
	if (!subrep) return NULL;
	subrep->header = rtdb_cli_decode_msg_header(bytes, RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	if (!subrep->header) goto ERR_END;
	rtdb_cli_decode_bytes(subrep->id, p, RTDB_CLI_UUIDLEN);
	switch (subrep->header->msgType)
	{
	case RTDB_CLI_SUB_RDBTBL:
	case RTDB_CLI_SUB_RDBNODE:
		rtdb_cli_decode_int32(subrep->seqnum, p);
		break;
	default:
		break;
	}
	rtdb_cli_decode_int8(subrep->num, p);
	if (subrep->num <= 0) goto ERR_END;
	subrep->noArr = calloc(subrep->num, sizeof(int8_t));
	if (!subrep->noArr) goto ERR_END;
	subrep->resultArr = calloc(subrep->num, sizeof(int8_t));
	if (!subrep->resultArr) goto ERR_END;
	for (i = 0; i < subrep->num; i++)
	{
		rtdb_cli_decode_int8(subrep->noArr[i], p);
		rtdb_cli_decode_int8(subrep->resultArr[i], p);
	}
	return subrep;
ERR_END :
	rtdb_cli_free_subrep(subrep);
	return NULL;	
}

rtdb_cli_cmdsub* rtdb_cli_create_cmdsub(int domain)
{
    rtdb_cli_cmdsub *cmdsub;
    cmdsub = calloc(1,sizeof(*cmdsub));
    if(!cmdsub) return NULL;
    cmdsub->header = rtdb_cli_create_msg_header();
    if(!cmdsub->header)
    {
        free(cmdsub);
        return NULL;
    }
    cmdsub->header->msgLen = RTDB_CLI_MSGHEADERLEN - 4;
	cmdsub->header->domain = domain;
	cmdsub->header->msgType = RTDB_CLI_SUB_RDBCMD;
	cmdsub->header->reqType = RTDB_CLI_REQTYPE_REQUEST;
    cmdsub->header->sender = RTDB_CLI_SENDER_CLI;
    cmdsub->header->receiver = RTDB_CLI_RECEIVER_RDB;
    cmdsub->noArr = NULL;
    cmdsub->cmdidArr = NULL;
    return cmdsub;
}

void rtdb_cli_free_cmdsub(rtdb_cli_cmdsub *cmdsub)
{
    if(!cmdsub) return ;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header,cmdsub->header);
	rtdb_cli_cmn_release(cmdsub->noArr);
	rtdb_cli_cmn_release(cmdsub->cmdidArr);
	rtdb_cli_cmn_release(cmdsub);
}

rtdb_cli_byte* rtdb_cli_dump_cmdsub(rtdb_cli_cmdsub *cmdsub,int *dumpLen)
{
    rtdb_cli_byte *bytes = NULL,*p = NULL;
    rtdb_cli_byte *headerBytes = NULL;    
    int len = 0,curLen = 0;
    int8_t i = 0;
    
    if(!cmdsub|| !cmdsub->header || !cmdsub->noArr || !cmdsub->cmdidArr || !dumpLen) 
        return NULL;
    if(cmdsub->num <= 0) return NULL;
    *dumpLen = 0;
    
    len = RTDB_CLI_MSGHEADERLEN;
    len += RTDB_CLI_UUIDLEN + 2 + (1 + RTDB_CLI_UUIDLEN) * cmdsub->num;
    len += RTDB_CLI_MD5LEN;
	cmdsub->header->msgLen = len - 4;
    headerBytes = rtdb_cli_dump_msg_header(cmdsub->header);
    if(!headerBytes) return NULL;
    if(!(p = bytes = calloc(len,1))) return NULL;
    rtdb_cli_encode_bytes(headerBytes,p,RTDB_CLI_MSGHEADERLEN,curLen);
    free(headerBytes);
    rtdb_cli_encode_bytes(cmdsub->id,p,RTDB_CLI_UUIDLEN,curLen);
    rtdb_cli_encode_int8(cmdsub->action,p,curLen);
    rtdb_cli_encode_int8(cmdsub->num,p,curLen);
    for(i = 0; i < cmdsub->num ; i++)
    {
        rtdb_cli_encode_int8(cmdsub->noArr[i],p,curLen);
        rtdb_cli_encode_bytes(cmdsub->cmdidArr[i],p,RTDB_CLI_UUIDLEN,curLen);       
    }
    rtdb_cli_encode_bytes(cmdsub->md5,p,sizeof(cmdsub->md5),curLen);
	*dumpLen = len;
    if(curLen != len)
    {
        free(bytes);
        bytes = NULL;
        *dumpLen = 0;
    }
    return bytes;
}

rtdb_cli_tblsub* rtdb_cli_create_tblsub(int domain,const char *uuid, int seqnum, int8_t action, int subNum)
{
	rtdb_cli_tblsub *sub;
	sub = calloc(1, sizeof(*sub));
	if (!sub) return NULL;
	sub->header = rtdb_cli_create_msg_header();
	if (!sub->header) goto ERR_END;
	sub->header->domain = domain;
	sub->header->msgType = RTDB_CLI_SUB_RDBTBL;
	sub->header->reqType = RTDB_CLI_REQTYPE_REQUEST;
	sub->header->sender = RTDB_CLI_SENDER_CLI;
	sub->header->receiver = RTDB_CLI_RECEIVER_RDB;

	memcpy(sub->id,uuid,RTDB_CLI_UUIDLEN);
	sub->action = action;
	sub->seqnum = seqnum;
	sub->num = subNum;

	sub->noArr = calloc(subNum, sizeof(int8_t));
	if (!sub->noArr) goto ERR_END;
	sub->nameArr = calloc(subNum, sizeof(rtdb_cli_tbname));
	if (!sub->nameArr) goto ERR_END;

	return sub;
ERR_END:
	rtdb_cli_free_tblsub(sub);
	return NULL;	
}

void rtdb_cli_free_tblsub(rtdb_cli_tblsub *sub)
{
	if (!sub) return;
	
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, sub->header);
	rtdb_cli_cmn_release(sub->noArr);
	rtdb_cli_cmn_release(sub->nameArr);
	rtdb_cli_cmn_release(sub);
}

rtdb_cli_byte* rtdb_cli_dump_tblsub(rtdb_cli_tblsub *tblsub, int *dumpLen)
{
	rtdb_cli_byte *bytes,*p;
	rtdb_cli_byte *headerBytes;
	int i,len = 0;

	if (!tblsub || !tblsub->header) return NULL;
	if ((tblsub->num <= 0) || !tblsub->noArr ||!tblsub->nameArr || !dumpLen)
		return NULL;
	*dumpLen = 0;
	len = RTDB_CLI_MSGHEADERLEN + RTDB_CLI_TBLSUBBASELEN;
	len += tblsub->num * (sizeof(int8_t)+sizeof(rtdb_cli_tbname));
	len += RTDB_CLI_MD5LEN;

	if (!(p = bytes = calloc(len, 1))) return NULL;
	tblsub->header->msgLen = len - 4;

	headerBytes = rtdb_cli_dump_msg_header(tblsub->header);
	if (!headerBytes)
	{
		free(bytes);
		return NULL;
	}
	rtdb_cli_encode_bytes(headerBytes, p, RTDB_CLI_MSGHEADERLEN, *dumpLen);
	free(headerBytes);
	rtdb_cli_encode_bytes(tblsub->id, p, RTDB_CLI_UUIDLEN, *dumpLen);
	rtdb_cli_encode_int32(tblsub->seqnum, p, *dumpLen);
	rtdb_cli_encode_int8(tblsub->action, p, *dumpLen);
	rtdb_cli_encode_int8(tblsub->num, p, *dumpLen);

	for (i = 0; i < tblsub->num; i++)
	{
		rtdb_cli_encode_int8(tblsub->noArr[i], p, *dumpLen);
		rtdb_cli_encode_bytes(tblsub->nameArr[i], p, sizeof(rtdb_cli_tbname), *dumpLen);
	}
	rtdb_cli_encode_bytes(tblsub->md5, p, sizeof(tblsub->md5), *dumpLen);
	return bytes;
}

rtdb_cli_nodesub* rtdb_cli_create_nodesub(int domain,rtdb_cli_uuid uuid, const rtdb_cli_list *names, int8_t action, int subNum)
{
	rtdb_cli_nodesub *sub;
	int aNum = 0, i = 0;
	char *name = NULL;
	rtdb_cli_attrname *attrName = NULL;

	sub = calloc(1,sizeof(*sub));
	if (!sub) return NULL;
	sub->header = rtdb_cli_create_msg_header();
	if (!sub->header) goto ERR_END;
	sub->header->domain = domain;
	sub->header->msgType = RTDB_CLI_SUB_RDBNODE;
	sub->header->reqType = RTDB_CLI_REQTYPE_REQUEST;
	sub->header->sender = RTDB_CLI_SENDER_CLI;
	sub->header->receiver = RTDB_CLI_RECEIVER_RDB;

	memcpy(sub->id, uuid, RTDB_CLI_UUIDLEN);
	sub->action = action;
	sub->nNum = subNum;

	sub->noArr = calloc(subNum, sizeof(int8_t));
	if (!sub->noArr) goto ERR_END;
	sub->path = calloc(subNum, sizeof(rtdb_cli_sub_path));
	if (!sub->path) goto ERR_END;
	if (names)
	{
		aNum = rtdb_cli_list_get_size(names);
		sub->aNum = aNum;
		sub->nameArr = calloc(aNum, sizeof(rtdb_cli_attrname));
		if (!sub->path) goto ERR_END;

		i = 0;
		name = (char*)rtdb_cli_list_get_first(names);
		while (name)
		{
			attrName = &sub->nameArr[i];
			snprintf((char*)attrName, sizeof(rtdb_cli_attrname), "%s", name);
			i++;
			name = (char*)rtdb_cli_list_get_next(names);
		}
    }
	return sub;
ERR_END :
	rtdb_cli_free_nodesub(sub);
	return NULL;
}
void rtdb_cli_free_nodesub(rtdb_cli_nodesub *sub)
{
	if (!sub) return;

	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, sub->header);
	rtdb_cli_cmn_release(sub->noArr);
	rtdb_cli_cmn_release(sub->path);
	rtdb_cli_cmn_release(sub->nameArr);
	rtdb_cli_cmn_release(sub);
}
rtdb_cli_byte* rtdb_cli_dump_nodesub(const rtdb_cli_nodesub *sub, int *dumpLen)
{
	rtdb_cli_byte *bytes, *p;
	rtdb_cli_byte *headerBytes;
	int i,j, len = 0;
	rtdb_cli_attrname *name;

	if (!sub || !sub->header || !sub->noArr || !sub->path || (sub->nNum <= 0) || !dumpLen)
		return NULL;
	*dumpLen = 0;
	len = RTDB_CLI_MSGHEADERLEN + RTDB_CLI_NODESUBBASELEN + RTDB_CLI_MD5LEN;
	len += (1 + 128) * sub->nNum ;
	if (sub->action == RTDB_CLI_ACT_SUB)
	{
		len += 1 + 64 * sub->aNum;
	}
	if (!(p = bytes = calloc(len, 1))) return NULL;
	sub->header->msgLen = len - 4;
	headerBytes = rtdb_cli_dump_msg_header(sub->header);
	if (!headerBytes)
	{
		free(bytes);
		return NULL;
	}
	rtdb_cli_encode_bytes(headerBytes, p, RTDB_CLI_MSGHEADERLEN, *dumpLen);
	free(headerBytes);
	rtdb_cli_encode_bytes(sub->id, p, RTDB_CLI_UUIDLEN, *dumpLen);
	rtdb_cli_encode_int32(sub->seqnum, p, *dumpLen);
	rtdb_cli_encode_int8(sub->action, p, *dumpLen);
	rtdb_cli_encode_int8(sub->nNum, p, *dumpLen);

	for (i = 0; i < sub->nNum; i++)
	{
		rtdb_cli_encode_int8(sub->noArr[i], p, *dumpLen);
		rtdb_cli_encode_bytes(sub->path+i, p, sizeof(rtdb_cli_sub_path), *dumpLen);
	}
	if (sub->action == RTDB_CLI_ACT_SUB)
	{
		rtdb_cli_encode_int8(sub->aNum,p,*dumpLen);
		for (j = 0; j < sub->aNum; j++)
		{
			name = &sub->nameArr[j];
			rtdb_cli_encode_bytes(name, p, sizeof(rtdb_cli_attrname), *dumpLen);
		}
	}
    rtdb_cli_encode_bytes(sub->md5, p, RTDB_CLI_MD5LEN, *dumpLen);
	
	return bytes;
}

rtdb_cli_cmdpub_item* rtdb_cli_create_cmdpub_item()
{
	rtdb_cli_cmdpub_item *item;
	item = calloc(1, sizeof(*item));
	if (!item) return NULL;
	return item;
}

void rtdb_cli_free_cmdpub_item(rtdb_cli_cmdpub_item *item)
{
	if (!item) return ;
	rtdb_cli_cmn_release(item->result);
	rtdb_cli_cmn_release(item);
}

rtdb_cli_cmdpub* rtdb_cli_create_cmdpub()
{
	rtdb_cli_cmdpub *cmdpub;
	cmdpub = calloc(1, sizeof(*cmdpub));
	if (!cmdpub) return NULL;
	cmdpub->header = rtdb_cli_create_msg_header();
	if (!cmdpub->header)
	{
		free(cmdpub);
		return NULL;
	}
	cmdpub->items = NULL;
	return cmdpub;
}

void rtdb_cli_free_cmdpub(rtdb_cli_cmdpub *cmdpub)
{
	int i;
	rtdb_cli_cmdpub_item *item;
	if (!cmdpub) return ;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, cmdpub->header);
	if (cmdpub->items)
	{
		for (i = 0; i < cmdpub->num; i++)
		{
			item = &cmdpub->items[i];
			if (item && item->result)
				rtdb_cli_cmn_release(item->result);
		}
		rtdb_cli_cmn_release(cmdpub->items);
	}
	rtdb_cli_cmn_release(cmdpub);
}

rtdb_cli_cmdpub* rtdb_cli_parse_cmdpub(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_cmdpub *cmdpub = NULL;
	rtdb_cli_cmdpub_item *item = NULL;
	rtdb_cli_byte *p = (rtdb_cli_byte*)bytes;
	int i = 0;

	if (!p || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	if (!(cmdpub = rtdb_cli_create_cmdpub())) return NULL;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, cmdpub->header);
	cmdpub->header = rtdb_cli_decode_msg_header(bytes, RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	if (!cmdpub->header) goto ERR_END;
	memcpy(cmdpub->id, p, RTDB_CLI_UUIDLEN);
	p += RTDB_CLI_UUIDLEN;
	rtdb_cli_decode_int8(cmdpub->num, p);
	if (cmdpub->num <= 0) goto ERR_END;
	cmdpub->items = calloc(cmdpub->num, sizeof(rtdb_cli_cmdpub_item));
	if (!cmdpub->items) goto ERR_END;
	for (i = 0; i < cmdpub->num; i++)
	{
		item = &cmdpub->items[i];
		rtdb_cli_decode_bytes(item->cmdid, p, RTDB_CLI_UUIDLEN);
		rtdb_cli_decode_int8(item->code, p);
		rtdb_cli_decode_int16(item->resultLen, p);
		item->result = calloc(item->resultLen + 1, 1);
		if (!item->result) goto ERR_END;
		rtdb_cli_decode_bytes(item->result,p,item->resultLen);
	}
	return cmdpub;
	
ERR_END:
	CMN_LOGGER_ERR(logger,"rtdb_cli_parse_cmdpub fail");
	rtdb_cli_free_cmdpub(cmdpub);
	return NULL;	
}

rtdb_cli_tblpub* rtdb_cli_create_tblpub(int8_t rNum)
{
	rtdb_cli_tblpub *tblpub;
	tblpub = calloc(1, sizeof(*tblpub));
	if (!tblpub) return NULL;

	tblpub->header = rtdb_cli_create_msg_header();
	if (!tblpub->header) goto ERR_END;
	if (rNum > 0)
	{
		tblpub->rNum = rNum;
		tblpub->nodeItem = calloc(rNum, sizeof(rtdb_cli_node_item *));
		if (!tblpub->nodeItem) goto ERR_END;
	}
	return tblpub;
ERR_END:
	rtdb_cli_free_tblpub(tblpub);
	return NULL;	
}

void rtdb_cli_free_tblpub(rtdb_cli_tblpub *tblpub)
{
	int i,j,fNum;
	rtdb_cli_node_item *item;
	if (!tblpub) return;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header,tblpub->header);
	if (tblpub->nodeItem)
	{
		for (i = 0; i < tblpub->rNum; i++)
		{
			rtdb_cli_free_node_item(tblpub->nodeItem[i]);
		}
		rtdb_cli_cmn_release(tblpub->nodeItem);
	}
	rtdb_cli_cmn_release(tblpub);
}
rtdb_cli_tblpub* rtdb_cli_parse_tblpub(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_tblpub *tblpub = NULL;
	rtdb_cli_node_item *item = NULL;
	rtdb_cli_byte *p = (rtdb_cli_byte*)bytes;
	int8_t i=0,rNum = 0;

	if (!p || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	tblpub = rtdb_cli_create_tblpub(0);
	if (!tblpub) return NULL;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, tblpub->header);
	tblpub->header = rtdb_cli_decode_msg_header(p,RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	if (!tblpub->header) goto ERR_END;
	rtdb_cli_decode_bytes(tblpub->id, p, RTDB_CLI_UUIDLEN);
	rtdb_cli_decode_bytes(tblpub->tName, p, sizeof(tblpub->tName));
	rtdb_cli_decode_int8(tblpub->type, p);
	rtdb_cli_decode_int8(rNum, p);

	tblpub->rNum = rNum;
	tblpub->nodeItem = calloc(rNum, sizeof(rtdb_cli_node_item*));
	if (!tblpub->nodeItem) goto ERR_END;
	for (i = 0; i < rNum; i++)
	{
		item = rtdb_cli_parse_node_item(&p);
		if (!item) goto ERR_END;
		tblpub->nodeItem[i] = item;
	}
	return tblpub;
	
ERR_END:
	CMN_LOGGER_ERR(logger, "rtdb_cli_parse_tblpub fail");
	if(tblpub) rtdb_cli_free_tblpub(tblpub);
	return NULL;
}

rtdb_cli_attr_item* rtdb_cli_create_attr_item()
{
	rtdb_cli_attr_item *item = NULL;

	if (!(item = calloc(1, sizeof(*item)))) return NULL;
	if (!(item->vt = rtdb_cli_variant_create()))
	{
		rtdb_cli_cmn_release(item);
		return NULL;
	}
	return item;
}

void rtdb_cli_free_attr_item(rtdb_cli_attr_item *item)
{
	if (!item) return ;
	if (item->vt) rtdb_cli_variant_free(item->vt);
	rtdb_cli_cmn_release(item);
}

rtdb_cli_attr_item* rtdb_cli_attr_item_dup(rtdb_cli_attr_item* attrItem)
{
	rtdb_cli_attr_item *item = NULL;

	if (!attrItem) return NULL;
	if (!(item = calloc(1, sizeof(*item)))) return NULL;
	memcpy(item->aName, attrItem->aName, 64);
	item->aType = attrItem->aType;
	item->avLen = attrItem->avLen;
	if (!(item->vt = rtdb_cli_variant_dup(attrItem->vt)))
	{
		rtdb_cli_free_attr_item(item);
		return NULL;
	}
	return item;
}

rtdb_cli_node_item* rtdb_cli_create_node_item(int aNum)
{
	rtdb_cli_node_item *item = NULL;

	if (aNum <= 0) return NULL;
	if (!(item = calloc(1, sizeof(*item)))) return NULL;
	item->aNum = aNum;
	if (!(item->attrItems = calloc(aNum, sizeof(rtdb_cli_attr_item*))))
	{
		rtdb_cli_free_node_item(item);
		return NULL;
	}
	return item;
}
void rtdb_cli_free_node_item(rtdb_cli_node_item *item)
{
	int i = 0;

	if (!item) return;
	if (item->attrItems)
	{
		for (i = 0; i < item->aNum; i++)
		{
			if (item->attrItems[i]) 
				rtdb_cli_free_attr_item(item->attrItems[i]);
		}
		rtdb_cli_cmn_release(item->attrItems);
	}
	rtdb_cli_cmn_release(item);
}

rtdb_cli_node_item* rtdb_cli_node_item_dup(rtdb_cli_node_item *nodeItem)
{
	rtdb_cli_node_item *retItem = NULL;
	rtdb_cli_attr_item *item = NULL;
	uint8_t aNum = 0;
	int i = 0, isErr = 0;

	if (!nodeItem || (nodeItem->aNum == 0)) return NULL;
	
	aNum = nodeItem->aNum;
	if (!(retItem = rtdb_cli_create_node_item(aNum))) return NULL;

	for (i = 0; i < aNum; i++)
	{
		if (!(item = rtdb_cli_attr_item_dup(nodeItem->attrItems[i]))) goto ERR_END;
		retItem->attrItems[i] = item;
	}

	return retItem;
ERR_END:
	if (retItem) rtdb_cli_free_node_item(retItem);
	return NULL;
}

rtdb_cli_node_item* rtdb_cli_parse_node_item(unsigned char **p)
{
	rtdb_cli_node_item *nodeItem = NULL;
	rtdb_cli_attr_item *item = NULL;
	uint8_t aNum = 0;
	int i = 0, isErr = 0;
	char traceBuf[10240] = {0};

	rtdb_cli_decode_int8(aNum, *p);
//	CMN_LOGGER_DEBUG(logger, "aNum = %d", aNum);
	if (0 == aNum) return NULL;
	if (!(nodeItem = rtdb_cli_create_node_item(aNum))) return NULL;

	for (i = 0; i < aNum; i++)
	{
		if(!(item = rtdb_cli_create_attr_item())) goto ERR_END;
		rtdb_cli_decode_bytes(item->aName, *p, 64);
		rtdb_cli_decode_int8(item->aType, *p);

		snprintf(traceBuf + strlen(traceBuf),sizeof(traceBuf)-strlen(traceBuf),"(%s,%d,",item->aName,item->aType);
		item->vt->type = item->aType;
		switch (item->aType)
		{
		case RTDB_CLI_VARIANT_BOOL:
		case RTDB_CLI_VARIANT_INT8:
		case RTDB_CLI_VARIANT_INT16:
		case RTDB_CLI_VARIANT_INT32:
		case RTDB_CLI_VARIANT_INT64:
			rtdb_cli_decode_int64(item->vt->v.integer, *p);
			snprintf(traceBuf + strlen(traceBuf),sizeof(traceBuf)-strlen(traceBuf),"%lld",item->vt->v.integer);
			break;
		case RTDB_CLI_VARIANT_FLOAT:
		case RTDB_CLI_VARIANT_DOUBLE:
			rtdb_cli_decode_float64(item->vt->v.real, *p);
			snprintf(traceBuf + strlen(traceBuf),sizeof(traceBuf)-strlen(traceBuf),"%lf",item->vt->v.real);
			break;
		case RTDB_CLI_VARIANT_STRING:
			rtdb_cli_decode_int16(item->avLen, *p);
			item->vt->v.string = calloc(item->avLen + 1, 1);
			if (!item->vt->v.string)
			{
				rtdb_cli_free_attr_item(item);
				goto ERR_END;
			}
			rtdb_cli_decode_bytes(item->vt->v.string, *p, item->avLen);
			snprintf(traceBuf + strlen(traceBuf),sizeof(traceBuf)-strlen(traceBuf),"%s",item->vt->v.string);
			break;
		default:
			break;
		}
		snprintf(traceBuf + strlen(traceBuf),sizeof(traceBuf)-strlen(traceBuf),"),");
		nodeItem->attrItems[i] = item;
	}
//	CMN_LOGGER_DEBUG(logger,"%s",traceBuf);

	return nodeItem;
ERR_END:
	if(nodeItem) rtdb_cli_free_node_item(nodeItem);
	return NULL;
}

rtdb_cli_nodepub* rtdb_cli_create_nodepub()
{
	rtdb_cli_nodepub *nodepub;
	nodepub = calloc(1, sizeof(*nodepub));
	if (!nodepub) return NULL;
	nodepub->header = rtdb_cli_create_msg_header();
	if (!nodepub->header)
	{
		free(nodepub);
		return NULL;
	}
	nodepub->nodeItem = NULL;
	return nodepub;
}

void rtdb_cli_free_nodepub(rtdb_cli_nodepub *nodepub)
{
	int i;
	rtdb_cli_attr_item *item;
	if (!nodepub) return;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, nodepub->header);
	if (nodepub->nodeItem)
		rtdb_cli_free_node_item(nodepub->nodeItem);
	free(nodepub);
	nodepub = NULL;
}

rtdb_cli_nodepub* rtdb_cli_parse_nodepub(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_nodepub *nodepub = NULL;
	rtdb_cli_node_item *nodeItem = NULL;
	rtdb_cli_byte *p = (rtdb_cli_byte *)bytes;

	if (!p || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	nodepub = rtdb_cli_create_nodepub();
	if (!nodepub) return NULL;
	rtdb_cli_cmn_release2(rtdb_cli_free_msg_header, nodepub->header);
	nodepub->header = rtdb_cli_decode_msg_header(p, RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	if (!nodepub->header) goto ERR_END;
	rtdb_cli_decode_bytes(nodepub->id, p, RTDB_CLI_UUIDLEN);
	rtdb_cli_decode_bytes(nodepub->path, p, sizeof(rtdb_cli_sub_path));
	
    nodepub->nodeItem = rtdb_cli_parse_node_item(&p);
	if (!nodepub->nodeItem) goto ERR_END;
	return nodepub;

ERR_END:
	CMN_LOGGER_ERR(logger,"rtdb_cli_parse_nodepub fail");
	if(nodepub) rtdb_cli_free_nodepub(nodepub);
	return NULL;
}

rtdb_cli_reply* rtdb_cli_create_reply()
{
	rtdb_cli_reply *reply = NULL;

	reply = calloc(1,sizeof(*reply));
	if (!reply) return NULL;

	return reply;
}

void rtdb_cli_free_reply(rtdb_cli_reply *reply)
{
	int i = 0;
	if (!reply) return NULL;
	if (reply->arrRowid) free(reply->arrRowid);

	if (reply->vtItems)
	{
		for (i = 0; i < reply->elenum; i++)
		{
			if (reply->vtItems[i])
				rtdb_cli_variant_free(reply->vtItems[i]);
		}
		free(reply->vtItems);
	}
	if (reply->nodeItems)
	{
		for (i = 0; i < reply->elenum; i++)
		{
			if (reply->nodeItems[i])
				rtdb_cli_free_node_item(reply->nodeItems[i]);
		}
		free(reply->nodeItems);
	}
	free(reply);
}

rtdb_cli_reply* rtdb_cli_prase_reply(const unsigned char *bytes)
{
	rtdb_cli_reply *reply = NULL;
	unsigned char *p = bytes;
	rtdb_cli_msg_header *header = NULL;
	int code = -1;
	
	if(!p) return NULL;
	header = rtdb_cli_decode_msg_header(bytes, RTDB_CLI_MSGHEADERLEN);
	if (!header) return NULL;
	p += RTDB_CLI_MSGHEADERLEN;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_prase_reply : msgType = %d(0x%0x)", header->msgType,header->msgType);

	if(!(reply = rtdb_cli_create_reply())) return NULL;
	switch (header->msgType)
	{
	case RTDB_CLI_TYPE_QUERYTABLE:
		code = rtdb_client_fill_reply_table(reply, p);
		break;
	case RTDB_CLI_TYPE_QUERYNODE:
		code = rtdb_client_fill_reply_node(reply, p);
		break;
	case RTDB_CLI_TYPE_UPDATENODE:
	case RTDB_CLI_TYPE_RDBMSG:
	case RTDB_CLI_TYPE_UPDATETABLE:
	case RTDB_CLI_TYPE_DELTABLE:
	case RTDB_CLI_TYPE_CHECKRTDB:
		code = rtdb_client_fill_reply_variant(reply, p);
		break;
	default:
		break;
	}
	if (-1 == code)
	{
		rtdb_cli_free_reply(reply);
		reply = NULL;
	}
	rtdb_cli_free_msg_header(header);
	return reply;
}

///////////////////////////////////////////////////////////////////////////////////
static int rtdb_client_fill_reply_table(rtdb_cli_reply *reply, unsigned char *buf)
{
	// 0 : ok ; -1 : fail 
	int i = 0, j = 0;
	int32_t rowNum = 0, rowId = 0;
	unsigned char *p = buf;
	char tbName[33] = { 0 };

	if (!reply || !p) return -1;

	rtdb_cli_decode_bytes(tbName, p, 32);
	rtdb_cli_decode_int32(rowNum, p);
	reply->elenum = rowNum;
	reply->type = RTDB_CLI_REPLY_TYPE_TABLE;
	reply->nodeItems = calloc(rowNum, sizeof(rtdb_cli_node_item*));
	if (!reply->nodeItems) goto ERR_END;
	for (i = 0; i < rowNum; i++)
		reply->nodeItems[i] = rtdb_cli_parse_node_item(&p);
	return 0;
ERR_END:
	return -1;
}

static int rtdb_client_fill_reply_node(rtdb_cli_reply *reply,unsigned char *buf)
{
	// 0 : ok ; -1 : fail 
	int i = 0,j = 0;
	int32_t rowNum = 0,rowId = 0;
	unsigned char *p = buf;
	
	if(!reply || !p ) return -1;
	
	rtdb_cli_decode_int32(rowNum, p);
	reply->elenum = rowNum;
	reply->type = RTDB_CLI_REPLY_TYPE_NODE;
	reply->nodeItems = calloc(rowNum, sizeof(rtdb_cli_node_item*));
	if (!reply->nodeItems) goto ERR_END;
	reply->arrRowid = calloc(rowNum, sizeof(int32_t));
	if (!reply->arrRowid) goto ERR_END;
	for (i = 0; i < rowNum; i++)
	{
		rtdb_cli_decode_int32(rowId,p);
		reply->arrRowid[i] = rowId;
		reply->nodeItems[i] = rtdb_cli_parse_node_item(&p);
	}
	return 0;
ERR_END :
	return -1;
}

static int rtdb_client_fill_reply_variant(rtdb_cli_reply *reply,unsigned char *buf)
{
	// 0 : ok ; -1 : fail 
	rtdb_cli_variant *vt = NULL;
	int32_t rowNum = 0;
	int8_t retCode = 0;

	if(!reply || !buf) return -1;
	rowNum = 1;
	reply->type = RTDB_CLI_REPLY_TYPE_VARIANT;
	reply->elenum = rowNum;
	reply->vtItems = calloc(rowNum, sizeof(rtdb_cli_variant*));
	if (!reply->vtItems) goto ERR_END;	
	retCode = buf[0];
	if (!(vt = rtdb_cli_variant_create())) goto ERR_END;
	vt->type = RTDB_CLI_VARIANT_INT8;
	vt->v.integer = retCode;
	CMN_LOGGER_DEBUG(logger, "vt->v.integer : %d", vt->v.integer);
	reply->vtItems[0] = vt;

	return 0;
ERR_END :
	return -1;
}



