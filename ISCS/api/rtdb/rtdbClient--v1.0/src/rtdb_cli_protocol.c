#include "rtdb_cli_protocol.h"
#include "rtdb_cli_cmn.h"
#include <stdlib.h>

extern cmn_logger* logger;

rtdb_cli_msg_header* rtdb_cli_create_msg_header()
{
    rtdb_cli_msg_header* header = NULL;
    header = calloc(1,sizeof(*header));
    if(!header) return NULL;    
    return header;
}

void rtdb_cli_free_msg_header(rtdb_cli_msg_header *header)
{
    if(header)
    {
        free(header);
        header = NULL;
    }
}

rtdb_cli_byte* rtdb_cli_dump_msg_header(rtdb_cli_msg_header *header)
{
    rtdb_cli_byte *bytes,*p;
    int curLen = 0;
    bytes = calloc(RTDB_CLI_MSGHEADERLEN,1);
    if(!bytes) return NULL;
    p = bytes;
    rtdb_cli_encode_int32(header->msgLen,p,curLen);
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

rtdb_cli_msg_header* rtdb_cli_decode_msg_header(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_msg_header *header;
	rtdb_cli_byte *p = (rtdb_cli_byte*)bytes;

	if (!bytes || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	header = rtdb_cli_create_msg_header();
	if (!header) return NULL;

	rtdb_cli_decode_int32(header->msgLen, p);
	rtdb_cli_decode_int16(header->msgType, p);
	rtdb_cli_decode_int8(header->reqType, p);
	rtdb_cli_decode_int16(header->sender, p);
	rtdb_cli_decode_int16(header->receiver, p);
	rtdb_cli_decode_int8(header->version, p);
	rtdb_cli_decode_int32(header->crc32, p);

	return header;
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
    
    if(!uuid || !destination || !cmdJson) return NULL;
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
    msgLen += RTDB_CLI_RDBCMDBASELEN + cmdLen;
    rdbcmd->header->msgLen = msgLen;
    rdbcmd->header->msgType = RTDB_CLI_TYPE_RDBCMD;
    rdbcmd->header->reqType = RTDB_CLI_REQTYPE_REQUEST;
    rdbcmd->header->sender = RTDB_CLI_SENDER_CLI;    
    rdbcmd->header->receiver = RTDB_CLI_SENDER_RDB;
    rdbcmd->domainId = domainId;
    rdbcmd->type = type;
    memset(rdbcmd->id,0,sizeof(rdbcmd->id));
    memcpy(rdbcmd->id,uuid,sizeof(rdbcmd->id));
    memset(rdbcmd->destination,0,sizeof(rdbcmd->destination));
    memcpy(rdbcmd->destination,destination,sizeof(rdbcmd->destination));
    
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
    if(rdbcmd->header) rtdb_cli_free_msg_header(rdbcmd->header);
    if(rdbcmd->cmdJson) free(rdbcmd->cmdJson);
    free(rdbcmd);
    rdbcmd = NULL;
}

rtdb_cli_byte* rtdb_cli_dump_msg_rdbcmd(rtdb_cli_msg_rdbcmd *rdbcmd,int *dumpLen)
{
    rtdb_cli_byte *bytes = NULL,*p = NULL;
    rtdb_cli_byte *headerBytes = NULL;
    int curLen = 0;
    
    CMN_LOGGER_DEBUG(logger,"rtdb_cli_dump_msg_rdbcmd start");
    if(!rdbcmd || !rdbcmd->header || !rdbcmd->cmdJson || !dumpLen)
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_dump_msg_rdbcmd : dump header fail");
        return NULL;
    }
    *dumpLen = 0;
    *dumpLen += RTDB_CLI_MSGHEADERLEN; 
    *dumpLen += RTDB_CLI_RDBCMDBASELEN;
    *dumpLen += strlen(rdbcmd->cmdJson);

    bytes = calloc(*dumpLen,1);
    if(!bytes) return NULL;
    
    headerBytes = rtdb_cli_dump_msg_header(rdbcmd->header);
    if(!headerBytes) 
    {
        CMN_LOGGER_ERR(logger,"rtdb_cli_dump_msg_rdbcmd : dump header fail");
        free(bytes);
        return NULL;    
    }
    p = bytes;
    
    rtdb_cli_encode_bytes(headerBytes,p,RTDB_CLI_MSGHEADERLEN,curLen);
    free(headerBytes);
    rtdb_cli_encode_int32(rdbcmd->domainId,p,curLen);
    rtdb_cli_encode_int32(rdbcmd->type,p,curLen);
    rtdb_cli_encode_bytes(rdbcmd->id,p,sizeof(rdbcmd->id),curLen); 
    rtdb_cli_assign_string(rdbcmd->destination,p,sizeof(rdbcmd->destination),curLen);
    rtdb_cli_encode_int16(rdbcmd->cmdLen,p,curLen);
    rtdb_cli_encode_bytes(rdbcmd->cmdJson,p,rdbcmd->cmdLen,curLen); 
    
    if(curLen != *dumpLen)
    {
        CMN_LOGGER_ERR(logger,"curLen : %d , *dumpLen : %d",curLen,*dumpLen);
        free(bytes);
        *dumpLen = 0;
        return NULL;        
    }    
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
	if (subrep->header)
	{
		rtdb_cli_free_msg_header(subrep->header);
		subrep->header = NULL;
	}
	if (subrep->noArr)
	{
		free(subrep->noArr);
		subrep->noArr = NULL;
	}
	if (subrep->resultArr)
	{
		free(subrep->resultArr);
		subrep->resultArr = NULL;
	}
	free(subrep);
	subrep = NULL;
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
	if (!subrep->header)
	{
		rtdb_cli_free_subrep(subrep);
		return NULL;
	}
	rtdb_cli_decode_bytes(subrep->id, p, sizeof(rtdb_cli_uuid));
	rtdb_cli_decode_int8(subrep->num, p);
	if (subrep->num <= 0)
	{
		rtdb_cli_free_subrep(subrep);
		return NULL;
	}
	subrep->noArr = calloc(subrep->num, sizeof(int8_t));
	if (!subrep->noArr)
	{
		rtdb_cli_free_subrep(subrep);
		return NULL;
	}
	subrep->resultArr = calloc(subrep->num, sizeof(int8_t));
	if (!subrep->resultArr)
	{
		rtdb_cli_free_subrep(subrep);
		return NULL;
	}
	for (i = 0; i < subrep->num; i++)
	{
		rtdb_cli_decode_int8(subrep->noArr[i], p);
		rtdb_cli_decode_int8(subrep->resultArr[i], p);
	}
	return subrep;
}

rtdb_cli_cmdsub* rtdb_cli_create_cmdsub()
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
    if(cmdsub->header) rtdb_cli_free_msg_header(cmdsub->header);
    if(cmdsub->noArr) free(cmdsub->noArr);
    if(cmdsub->cmdidArr) free(cmdsub->cmdidArr);
    free(cmdsub);
    cmdsub = NULL;
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
    len += sizeof(rtdb_cli_uuid) + 2 + (1 + sizeof(rtdb_cli_uuid)) * cmdsub->num;
	cmdsub->header->msgLen = len - 4;
    headerBytes = rtdb_cli_dump_msg_header(cmdsub->header);
    if(!headerBytes) return NULL;
    
    bytes = calloc(len,1);
    if(!bytes) return NULL;
    p = bytes;
    
    rtdb_cli_encode_bytes(headerBytes,p,RTDB_CLI_MSGHEADERLEN,curLen);
    free(headerBytes);
    rtdb_cli_encode_bytes(cmdsub->id,p,sizeof(rtdb_cli_uuid),curLen);
    rtdb_cli_encode_int8(cmdsub->action,p,curLen);
    rtdb_cli_encode_int8(cmdsub->num,p,curLen);
    for(i = 0; i < cmdsub->num ; i++)
    {
        rtdb_cli_encode_int8(cmdsub->noArr[i],p,curLen);
        rtdb_cli_encode_bytes(cmdsub->cmdidArr[i],p,sizeof(rtdb_cli_uuid),curLen);       
    }
    if(curLen != len)
    {
        free(bytes);
        bytes = NULL;
        *dumpLen = 0;
    }
    else
    {
        *dumpLen = len;
    }
    return bytes;
}

rtdb_cli_tblsub* rtdb_cli_create_tblsub()
{
	rtdb_cli_tblsub *sub;
	sub = calloc(1, sizeof(*sub));
	if (!sub) return NULL;
	sub->header = rtdb_cli_create_msg_header();
	if (!sub->header)
	{
		free(sub);
		return NULL;
	}
	sub->header->msgType = RTDB_CLI_SUB_RDBTBL;
	sub->header->reqType = RTDB_CLI_REQTYPE_REQUEST;
	sub->header->sender = RTDB_CLI_SENDER_CLI;
	sub->header->receiver = RTDB_CLI_RECEIVER_RDB;
	sub->noArr = NULL;
	sub->nameArr = NULL;
	return sub;
}

void rtdb_cli_free_tblsub(rtdb_cli_tblsub *sub)
{
	if (!sub) return;
	if (sub->header) rtdb_cli_free_msg_header(sub->header);
	if (sub->noArr) free(sub->noArr);
	if (sub->nameArr) free(sub->nameArr);
	free(sub);
	sub = NULL;
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
	bytes = calloc(len, 1);
	if (!bytes) return NULL;
	p = bytes;

	headerBytes = rtdb_cli_dump_msg_header(tblsub->header);
	if (!headerBytes)
	{
		free(bytes);
		return NULL;
	}
	rtdb_cli_encode_bytes(headerBytes, p, RTDB_CLI_MSGHEADERLEN, *dumpLen);
	free(headerBytes);
	rtdb_cli_encode_bytes(tblsub->id, p, sizeof(tblsub->id), *dumpLen);
	rtdb_cli_encode_int8(tblsub->action, p, *dumpLen);
	rtdb_cli_encode_int32(tblsub->domain, p, *dumpLen);
	rtdb_cli_encode_int8(tblsub->num, p, *dumpLen);

	for (i = 0; i < tblsub->num; i++)
	{
		rtdb_cli_encode_int8(tblsub->noArr[i], p, *dumpLen);
		rtdb_cli_encode_bytes(tblsub->nameArr[i], p, sizeof(rtdb_cli_tbname), *dumpLen);
	}
	return bytes;
}

rtdb_cli_nodesub* rtdb_cli_create_nodesub()
{
	rtdb_cli_nodesub *sub;
	sub = calloc(1,sizeof(*sub));
	if (!sub) return NULL;
	sub->header = rtdb_cli_create_msg_header();
	if (!sub->header)
	{
		free(sub);
		return NULL;
	}
	sub->header->msgType = RTDB_CLI_SUB_RDBNODE;
	sub->header->reqType = RTDB_CLI_REQTYPE_REQUEST;
	sub->header->sender = RTDB_CLI_SENDER_CLI;
	sub->header->receiver = RTDB_CLI_RECEIVER_RDB;
	sub->noArr = NULL;
	sub->path = NULL;
	sub->nameArr = NULL;
	return sub;
}
void rtdb_cli_free_nodesub(rtdb_cli_nodesub *sub)
{
	if (!sub) return;
	if (sub->header) rtdb_cli_free_msg_header(sub->header);
	if (sub->noArr) free(sub->noArr);
	if (sub->path) free(sub->path);
	if (sub->nameArr) free(sub->nameArr);
	free(sub);
	sub = NULL;
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
	len = RTDB_CLI_MSGHEADERLEN + RTDB_CLI_NODESUBBASELEN;
	len += (1 + 128) * sub->nNum;
	if (sub->action == RTDB_CLI_ACT_SUB)
	{
		len += 1 + 64 * sub->aNum;
	}
	bytes = calloc(len, 1);
	if (!bytes) return NULL;
	p = bytes;

	headerBytes = rtdb_cli_dump_msg_header(sub->header);
	if (!headerBytes)
	{
		free(bytes);
		return NULL;
	}
	rtdb_cli_encode_bytes(headerBytes, p, RTDB_CLI_MSGHEADERLEN, *dumpLen);
	free(headerBytes);
	rtdb_cli_encode_bytes(sub->id, p, sizeof(sub->id), *dumpLen);
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
	
	return bytes;
}

int rtdb_cli_check_subcriber_cmdreq(rtdb_cli_byte* bytes,int len)
{
    // one subscriber req reply have one cmdreq req reply
    int result = 0; // 0 : fail , 1 : success  
    
    if(!bytes || (len != 3)) return 0;
    
    return result;    
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
	if (item->result) free(item->result);
	free(item);
	item = NULL;
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
	if (cmdpub->header) rtdb_cli_free_msg_header(cmdpub->header);
	if (cmdpub->items)
	{
		for (i = 0; i < cmdpub->num; i++)
		{
			item = &cmdpub->items[i];
			if (item && item->result)
				free(item->result);
		}
		free(cmdpub->items);
	}
	free(cmdpub);
	cmdpub=NULL;
}

rtdb_cli_cmdpub* rtdb_cli_parse_cmdpub(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_cmdpub *cmdpub;
	rtdb_cli_cmdpub_item *item;
	rtdb_cli_byte *p = (rtdb_cli_byte*)bytes;
	int i = 0;

	if (!bytes || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	cmdpub = rtdb_cli_create_cmdpub();
	if (!cmdpub) return NULL;
	if (cmdpub->header) rtdb_cli_free_msg_header(cmdpub->header);
	cmdpub->header = rtdb_cli_decode_msg_header(bytes, RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	if (!cmdpub->header)
	{
		rtdb_cli_free_cmdpub(cmdpub);
		return NULL;
	}
	memcpy(cmdpub->id, p, sizeof(rtdb_cli_uuid));
	p += sizeof(rtdb_cli_uuid);
	rtdb_cli_decode_int8(cmdpub->num, p);
	if (cmdpub->num <= 0)
	{
		rtdb_cli_free_cmdpub(cmdpub);
		return NULL;
	}
	cmdpub->items = calloc(cmdpub->num, sizeof(rtdb_cli_cmdpub_item));
	if (!cmdpub->items)
	{
		rtdb_cli_free_cmdpub(cmdpub);
		return NULL;
	}

	for (i = 0; i < cmdpub->num; i++)
	{
		item = &cmdpub->items[i];
		rtdb_cli_decode_bytes(item->cmdid, p, sizeof(rtdb_cli_uuid));
		rtdb_cli_decode_int8(item->code, p);
		rtdb_cli_decode_int16(item->resultLen, p);
		item->result = calloc(item->resultLen + 1, 1);
		if (!item->result)
		{
			rtdb_cli_free_cmdpub(cmdpub);
			return NULL;
		}
		rtdb_cli_decode_bytes(item->result,p,item->resultLen);
	}

	return cmdpub;
}

rtdb_cli_tblpub_item* rtdb_cli_create_tblpub_item()
{
	rtdb_cli_tblpub_item *item;
	item = calloc(1, sizeof(*item));
	if (!item) return NULL;
	item->vt = rtdb_cli_variant_create();
	if (!item->vt)
	{
		free(item);
		return NULL;
	}
	return item;
}

void rtdb_cli_free_tblpub_item(rtdb_cli_tblpub_item *item)
{
	if (!item) return;
	if (item->vt) rtdb_cli_variant_free(item->vt);
	free(item);
	item = NULL;
}

rtdb_cli_tblpub* rtdb_cli_create_tblpub()
{
	rtdb_cli_tblpub *tblpub;
	tblpub = calloc(1, sizeof(*tblpub));
	if (!tblpub) return NULL;

	tblpub->header = rtdb_cli_create_msg_header();
	if (!tblpub->header)
	{
		free(tblpub);
		return NULL;
	}
	tblpub->items = NULL;
	return tblpub;
}

void rtdb_cli_free_tblpub(rtdb_cli_tblpub *tblpub)
{
	int i,j,fNum;
	rtdb_cli_tblpub_item *item;
	if (!tblpub) return;
	if (tblpub->header) rtdb_cli_free_msg_header(tblpub->header);
	if (tblpub->items)
	{
		for (i = 0; i < tblpub->rNum; i++)
		{
			fNum = tblpub->fNum;
			for (j = 0; j < fNum; j++)
			{
				item = tblpub->items + i*fNum + j;
				if (item && item->vt) rtdb_cli_variant_free(item->vt);
			}
		}
		free(tblpub->items);
		tblpub->items = NULL;
	}
	free(tblpub);
	tblpub = NULL;
}
rtdb_cli_tblpub* rtdb_cli_parse_tblpub(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_tblpub *tblpub = NULL;
	rtdb_cli_tblpub_item *item = NULL;
	rtdb_cli_byte *p = (rtdb_cli_byte*)bytes;
	int i = 0,isErr = 0;
	int itemNum = 0;

	if (!bytes || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	tblpub = rtdb_cli_create_tblpub();
	if (!tblpub) return NULL;
	if (tblpub->header) rtdb_cli_free_msg_header(tblpub->header);
	tblpub->header = rtdb_cli_decode_msg_header(p,RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	if (!tblpub->header)
	{
		rtdb_cli_free_tblpub(tblpub);
		return NULL;
	}
	rtdb_cli_decode_bytes(tblpub->id, p, sizeof(tblpub->id));
	rtdb_cli_decode_bytes(tblpub->tName, p, sizeof(tblpub->tName));
	rtdb_cli_decode_int8(tblpub->type, p);
	rtdb_cli_decode_int8(tblpub->rNum, p);
	rtdb_cli_decode_int8(tblpub->fNum, p);
	if (tblpub->rNum <= 0 || tblpub->fNum <= 0)
	{
		rtdb_cli_free_tblpub(tblpub);
		return NULL;
	}
	itemNum = tblpub->rNum * tblpub->fNum;
	CMN_LOGGER_DEBUG(logger, "itemNum : %d", itemNum);
	tblpub->items = calloc(itemNum, sizeof(rtdb_cli_tblpub_item));
	if (!tblpub->items)
	{
		rtdb_cli_free_tblpub(tblpub);
		return NULL;
	}
	for (i = 0; i < itemNum; i++)
	{
		item = &tblpub->items[i];
		if (!item)
		{
			CMN_LOGGER_ERR(logger, "item is NULL");
			isErr = 1;
			break;
		}
		memset(item->fName, 0, sizeof(item->fName));
		rtdb_cli_decode_bytes(item->fName,p,sizeof(item->fName));
		rtdb_cli_decode_int8(item->fType, p);
		if (!item->vt) 
			item->vt = calloc(1, sizeof(rtdb_cli_variant));
		if (!item->vt)
		{
			CMN_LOGGER_ERR(logger, "item->vt is NULL");
			isErr = 1;
			break;
		}
		item->vt->type = item->fType;
		switch (item->fType)
		{
		case RTDB_CLI_VARIANT_BOOL:
		case RTDB_CLI_VARIANT_INT8:
		case RTDB_CLI_VARIANT_INT16:
		case RTDB_CLI_VARIANT_INT32:
		case RTDB_CLI_VARIANT_INT64:
			rtdb_cli_decode_int64(item->vt->v.integer, p);
			break;
		case RTDB_CLI_VARIANT_FLOAT:
		case RTDB_CLI_VARIANT_DOUBLE:
			rtdb_cli_decode_float64(item->vt->v.real, p);
			//printf("item->vt->v.real : %lf\n", item->vt->v.real);
			break;
		case RTDB_CLI_VARIANT_STRING:
			rtdb_cli_decode_int16(item->fvLen, p);
			item->vt->v.string = calloc(item->fvLen + 1, 1);
			if (!item->vt->v.string)
			{
				isErr = 1;
				break;
			}
			rtdb_cli_decode_bytes(item->vt->v.string, p, item->fvLen);
			break;
		default:
			break;
		}
		if (isErr) break;
	}
	if (isErr)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_parse_tblpub : error occur");
		rtdb_cli_free_tblpub(tblpub);
		return NULL;
	}
	return tblpub;
}


rtdb_cli_nodepub_item* rtdb_cli_create_nodepub_item()
{
	rtdb_cli_nodepub_item *item;
	item = calloc(1, sizeof(*item));
	if (!item) return NULL;
	item->vt = rtdb_cli_variant_create();
	if (!item->vt)
	{
		free(item);
		return NULL;
	}
	return item;
}

void rtdb_cli_free_nodepub_item(rtdb_cli_nodepub_item *item)
{
	if (!item) return ;
	if (item->vt) rtdb_cli_variant_free(item->vt);
	free(item);
	item = NULL;
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
	nodepub->items = NULL;
	return nodepub;
}

void rtdb_cli_free_nodepub(rtdb_cli_nodepub *nodepub)
{
	int i;
	rtdb_cli_nodepub_item *item;
	if (!nodepub) return;
	if (nodepub->header) rtdb_cli_free_msg_header(nodepub->header);
	if (nodepub->items)
	{
		for (i = 0; i < nodepub->aNum; i++)
		{
			item = &nodepub->items[i];
			if (item && item->vt)
				rtdb_cli_variant_free(item->vt);
		}
		free(nodepub->items);
		nodepub->items = NULL;
	}
	free(nodepub);
	nodepub = NULL;
}

rtdb_cli_nodepub* rtdb_cli_parse_nodepub(const rtdb_cli_byte *bytes, int len)
{
	rtdb_cli_nodepub *nodepub;
	rtdb_cli_nodepub_item *item;
	rtdb_cli_byte *p = (rtdb_cli_byte *)bytes;
	int i = 0,isErr = 0;

	if (!bytes || (len < RTDB_CLI_MSGHEADERLEN)) return NULL;
	nodepub = rtdb_cli_create_nodepub();
	if (!nodepub) return NULL;
	if (nodepub->header) rtdb_cli_free_msg_header(nodepub->header);
	nodepub->header = rtdb_cli_decode_msg_header(p, RTDB_CLI_MSGHEADERLEN);
	p += RTDB_CLI_MSGHEADERLEN;
	if (!nodepub->header)
	{
		rtdb_cli_free_nodepub(nodepub);
		return NULL;
	}
	rtdb_cli_decode_bytes(nodepub->id, p, sizeof(rtdb_cli_uuid));
	rtdb_cli_decode_bytes(nodepub->path, p, sizeof(rtdb_cli_sub_path));
	rtdb_cli_decode_int8(nodepub->aNum, p);
	if (nodepub->aNum <= 0)
	{
		rtdb_cli_free_nodepub(nodepub);
		return NULL;
	}
	nodepub->items = calloc(nodepub->aNum,sizeof(rtdb_cli_nodepub_item));
	if (!nodepub->items)
	{
		rtdb_cli_free_nodepub(nodepub);
		return NULL;
	}
	for (i = 0; i < nodepub->aNum; i++)
	{	
		item = &nodepub->items[i];
		if (!item)
		{
			isErr = 1;
			break;
		}
		if (!item->vt)
			item->vt = calloc(1,sizeof(*item->vt));
		if (!item->vt)
		{
			isErr = 1;
			break;
		}
		rtdb_cli_decode_bytes(item->aName, p, 64);
		rtdb_cli_decode_int8(item->aType, p);

		item->vt->type = item->aType;
		switch (item->aType)
		{
		case RTDB_CLI_VARIANT_BOOL:
		case RTDB_CLI_VARIANT_INT8:
		case RTDB_CLI_VARIANT_INT16:
		case RTDB_CLI_VARIANT_INT32:
		case RTDB_CLI_VARIANT_INT64:
			rtdb_cli_decode_int64(item->vt->v.integer, p);
			break;
		case RTDB_CLI_VARIANT_FLOAT:
		case RTDB_CLI_VARIANT_DOUBLE:
			rtdb_cli_decode_float64(item->vt->v.real, p);
			break;
		case RTDB_CLI_VARIANT_STRING:
			rtdb_cli_decode_int16(item->avLen, p);
			item->vt->v.string = calloc(item->avLen + 1, 1);
			if (!item->vt->v.string)
			{
				isErr = 1;
				break;
			}
			rtdb_cli_decode_bytes(item->vt->v.string, p, item->avLen);
			break;
		default:
			break;
		}
		if (isErr) break;
	}
	if (isErr)
	{
		rtdb_cli_free_nodepub(nodepub);
		return NULL;
	}
	return nodepub;
}
 
 
