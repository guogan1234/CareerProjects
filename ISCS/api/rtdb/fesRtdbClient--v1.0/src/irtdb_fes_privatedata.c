#include <stdlib.h>
#include <cmn_logger.h>
#include "irtdb_fes_common.h"
#include "irtdb_fes_privatecommon.h"
#include "irtdb_fes_privatedata.h"
#include "cmn_list.h"

extern cmn_logger* logger;

#define irtdb_fes_pointer_acc(p,n,inc) do { \
    p += inc; \
    n += inc; \
} while(0)

#define irtdb_fes_encode_bytes(src,srcLen,p,dumpLen) do {\
	memcpy(p, src, srcLen); \
	p += srcLen; \
	dumpLen += srcLen; \
}while (0)
    
#define irtdb_fes_decode_bytes(dst,p,dstLen) do {\
	memcpy(dst, p, dstLen); \
	p += dstLen; \
}while (0)

#define irtdb_fes_encode_base(type,label,base,src,p,dumpLen) do { \
    type tmp = irtdb_fes_hton##label##base(src); \
    irtdb_fes_encode_bytes(&tmp,base/8,p,dumpLen); \
} while(0)

#define irtdb_fes_decode_base(type,label,base,dst,p) do { \
    type tmp;\
    irtdb_fes_decode_bytes(&tmp,p,base/8);\
    dst = irtdb_fes_ntoh##label##base(tmp); \
} while(0)

#define irtdb_fes_encode_int8(src,p,dumpLen) irtdb_fes_encode_bytes(&src,1,p,dumpLen)    
#define irtdb_fes_encode_int16(src,p,dumpLen) irtdb_fes_encode_base(int16_t,i,16,src,p,dumpLen)
#define irtdb_fes_encode_int32(src,p,dumpLen) irtdb_fes_encode_base(int32_t,i,32,src,p,dumpLen)
#define irtdb_fes_encode_int64(src,p,dumpLen) irtdb_fes_encode_base(int64_t,i,64,src,p,dumpLen)
#define irtdb_fes_encode_float32(src,p,dumpLen) irtdb_fes_encode_base(float,f,32,src,p,dumpLen)
#define irtdb_fes_encode_float64(src,p,dumpLen) irtdb_fes_encode_base(double,f,64,src,p,dumpLen)

#define irtdb_fes_decode_int8(dst,p) irtdb_fes_decode_bytes(&dst,p,1)
#define irtdb_fes_decode_int16(dst,p) irtdb_fes_decode_base(int16_t,i,16,dst,p) 
#define irtdb_fes_decode_int32(dst,p) irtdb_fes_decode_base(int32_t,i,32,dst,p) 
#define irtdb_fes_decode_int64(dst,p) irtdb_fes_decode_base(int64_t,i,64,dst,p)
#define irtdb_fes_decode_float32(dst,p) irtdb_fes_decode_base(float,f,32,dst,p)
#define irtdb_fes_decode_float64(dst,p) irtdb_fes_decode_base(double,f,64,dst,p)

#define irtdb_fes_encode_label(dstLen,dstLabel,srcLabel,dataOutLen) do { \
    int32_t srcLen = strlen(srcLabel); \
    dstLen = srcLen; \
    dstLabel = malloc(srcLen+1); \
    memset(dstLabel,0,srcLen+1); \
    memcpy(dstLabel,srcLabel,srcLen); \
    dataOutLen += sizeof(dstLen) + dstLen; \
} while(0)    

#define irtdb_fes_decode_label(dstLen,dstLabel,p) do { \
    int32_t uilen = 0; \
    irtdb_fes_decode_int32(dstLen,p);  \
    uilen = dstLen; \
    dstLabel = malloc(uilen + 1); \
    memset(dstLabel,0,uilen + 1); \
    memcpy(dstLabel,p,uilen); \
    p += uilen; \
} while (0)

#define irtdb_fes_encode_bytes(src,srcLen,dst,dumpLen) do {\
	memcpy(dst, src, srcLen); \
	dst += srcLen; \
	dumpLen += srcLen; \
} while (0)

#define irtdb_fes_decode_bytes(dst,src,dstLen) do {\
	memcpy(dst, src, dstLen); \
	src += dstLen; \
} while (0)


irtdb_fes_dbserver* irtdb_fes_create_dbserver(
    const char *ip1,const char *ip2,int port,
    const char *username,
    const char *password
)
{
    irtdb_fes_dbserver *pObj = NULL;
    int tmpLen = 0;

    pObj = calloc(1,sizeof(*pObj));
    if(!pObj) return NULL;
    
    if(ip1) pObj->ip1 = strdup(ip1);
    if(ip2) pObj->ip2 = strdup(ip2);
    pObj->activeIp = NULL;
    pObj->port = port;
    
    if(username) pObj->username = strdup(username);
    if(password) pObj->password = strdup(password);
    pObj->isAvailable = 0;

    CMN_LOGGER_DEBUG(logger,"irtdb_fes_create_dbserver ok");
    return pObj;    
}

void irtdb_fes_free_dbserver(irtdb_fes_dbserver *pServer)
{
    if(!pServer) return ;
    irtdb_fes_free_pointer(pServer->ip1);
    irtdb_fes_free_pointer(pServer->ip2);
    irtdb_fes_free_pointer(pServer->username);
    irtdb_fes_free_pointer(pServer->password);
    irtdb_fes_free_pointer(pServer);
}


irtdb_fes_message_bin* irtdb_fes_create_message_bin(
    const unsigned char *data,int dataLen
)
{
    irtdb_fes_message_bin *msgbin = NULL;
    
    msgbin = calloc(1,sizeof(*msgbin));
    if(!msgbin) return NULL;

    msgbin->data = calloc(dataLen,1);
    if(!msgbin->data)
    {
        free(msgbin);
        return NULL;
    }
    memcpy(msgbin->data,data,dataLen);
    msgbin->length = dataLen;

    return msgbin;    
}

void irtdb_fes_delete_message_bin(irtdb_fes_message_bin* msgbin)
{
    if(!msgbin) return;
    irtdb_fes_free_pointer(msgbin->data);
    irtdb_fes_free_pointer(msgbin);
}

irtdb_fes_message_bin* irtdb_fes_dup_message_bin(
    const irtdb_fes_message_bin* msgbinIn
)
{
    irtdb_fes_message_bin *msgbinOut = NULL;   
    if(!msgbinIn) return NULL;    
    msgbinOut = calloc(1,sizeof(*msgbinOut));
    if(!msgbinOut) return NULL;
    
    msgbinOut->data = calloc(msgbinIn->length,1);
    if(!msgbinOut->data)
    {
        free(msgbinOut);
        return NULL;
    }
    memcpy(msgbinOut->data,msgbinIn->data,msgbinIn->length);
    msgbinOut->length = msgbinIn->length;

    return msgbinOut;
}

static void* irtdb_fes_listdup_message_bin(void *val)
{
	return irtdb_fes_dup_message_bin(val);
}

static void irtdb_fes_listdelete_message_bin(void *val)
{
	irtdb_fes_delete_message_bin(val);
}

irtdb_fes_peer* irtdb_fes_create_peer()
{
    irtdb_fes_peer *peer = NULL;
    peer = calloc(1,sizeof(*peer));
    if(!peer) return NULL;
    peer->send_buffer = cmn_list_create(NULL,irtdb_fes_listdelete_message_bin,NULL);
    if(!peer->send_buffer)
    {
        free(peer);
        return NULL;
    }
    peer->recvBuffer = NULL;
    return peer;
}

void irtdb_fes_delete_peer(irtdb_fes_peer *peer)
{
    if(!peer) return;
    
    close(peer->socket);
    #ifdef WIN32
        WSACleanup();
    #endif
    if(peer->send_buffer)
    {
        cmn_list_release(peer->send_buffer);
        peer->send_buffer = NULL;
    }
    irtdb_fes_delete_message_bin(peer->recvBuffer);
    free(peer);
    peer = NULL;
}

int irtdb_fes_peer_add_to_send(irtdb_fes_peer *peer, const irtdb_fes_message_bin *msg)
{
    int iRet = -1;
    if(peer && peer->send_buffer && msg)
    {
        irtdb_fes_message_bin *dupMsg = NULL;
        if(peer->socket > 0)
        {
            dupMsg = irtdb_fes_dup_message_bin(msg);
        }
        if(dupMsg)
        {
            cmn_list_add_tail(peer->send_buffer,dupMsg);
            iRet = 0; 
        }
    }
    if(0 == iRet)
    {
        CMN_LOGGER_DEBUG(logger,"irtdb_fes_peer_add_to_send success");
    }   
    else 
    {
        CMN_LOGGER_DEBUG(logger,"irtdb_fes_peer_add_to_send fail");
    }
    return iRet;
}

int irtdb_fes_recv_from_peer(irtdb_fes_peer *peer, int (*message_handler)(irtdb_fes_message_bin *))
{
    ssize_t recvdCount = 0;
    unsigned char binBuf[MAX_BUFFER_SIZE];
    irtdb_fes_message_bin *msgbin = NULL;
    
    if(!peer) return -1;
    do 
    {
        recvdCount = recv(peer->socket,binBuf,MAX_BUFFER_SIZE,MSG_DONTWAIT);
        CMN_LOGGER_DEBUG(logger,"recvdCount : %d",recvdCount);
        if (recvdCount < 0) 
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                CMN_LOGGER_DEBUG(logger,"peer is not ready right now, try again later.");
                break;
            }
            else 
            {
                CMN_LOGGER_DEBUG(logger,"recv() from peer error");
                return -1;
            }
        } 
        else if (recvdCount == 0) 
        {
            return -1;
        }
    
        msgbin = irtdb_fes_create_message_bin(binBuf,recvdCount);
        message_handler(msgbin);

    } while (recvdCount > 0);
    return 0;
}

int irtdb_fes_send_to_peer(irtdb_fes_peer *peer)
{
    size_t send_count;
    cmn_list_iterator *pIter ;
    cmn_list_node *cmnNode = NULL;
    irtdb_fes_message_bin *msgbin = NULL;
    cmn_list *tmpList;
    
    if(!peer || !peer->send_buffer) return -1;
    if(!cmn_list_get_size(peer->send_buffer)) return 0;
    tmpList = cmn_list_create(NULL,NULL,NULL);
    if(!tmpList) return -1;
    pIter = cmn_list_get_iterator(peer->send_buffer,CMN_LIST_HEAD);
    if(!pIter)
    {
        cmn_list_release(tmpList);
        return -1;
    }
    while(cmnNode = cmn_list_next(pIter))
    {
        msgbin = (irtdb_fes_message_bin *)cmn_list_node_get_value(cmnNode);
        if(!msgbin) continue;
        send_count = send(peer->socket,msgbin->data,msgbin->length,0);
        if (send_count < 0) 
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                CMN_LOGGER_DEBUG(logger,"peer is not ready right now, try again later.\n");
                break;
            }
            else 
            {
                CMN_LOGGER_DEBUG(logger,"send() from peer error");
                return -1;
            }
        }
        else if(send_count == 0)
        {
            break;
        }
        cmn_list_remove(peer->send_buffer,cmnNode); 
    }    
    cmn_list_release_iterator(pIter);
    return 0;
}

irtdb_fes_dbnode* irtdb_fes_create_dbnode(irtdb_fes_dbserver *pServer)
{
    irtdb_fes_dbnode* pNode = NULL;
    if(!pServer) return NULL;
    pNode = calloc(1,sizeof(*pNode));
    if(!pNode) return NULL;
    pNode->pRtdbInfo = pServer;   
    pNode->peer = irtdb_fes_create_peer();
    return pNode;
}

void irtdb_fes_free_dbnode(irtdb_fes_dbnode* pNode)
{
    if(!pNode) return ;
    if(pNode->peer != NULL)
    {
        irtdb_fes_free_dbserver(pNode->pRtdbInfo);
        irtdb_fes_delete_peer(pNode->peer);
        pNode->peer = NULL;
    }
    free(pNode);
    pNode = NULL;
}

irtdb_fes_header* irtdb_fes_create_header()
{
	irtdb_fes_header *header = NULL;
	header = calloc(1, sizeof(*header));
	if (!header)  return NULL;
	return header;
}

void irtdb_fes_show_header(irtdb_fes_header *header)
{
    if(!header) return;
    CMN_LOGGER_DEBUG(logger," ------------- showMsgHeader begin --------------- ");
    CMN_LOGGER_DEBUG(logger,"header->msgLen : %d",header->msgLen);
    CMN_LOGGER_DEBUG(logger,"header->msgType : %d",header->msgType);
    CMN_LOGGER_DEBUG(logger,"header->dataType : %d",header->dataType);
    CMN_LOGGER_DEBUG(logger,"header->version : %d",header->version);
    CMN_LOGGER_DEBUG(logger,"header->crc32 : %d",header->crc32);
    CMN_LOGGER_DEBUG(logger," ------------- showMsgHeader end ---------------");
}

unsigned char* irtdb_fes_dump_header(irtdb_fes_header *header,int *dumLen)
{
	unsigned char *bytes = NULL,*p = NULL;

	if (!header || !dumLen) return NULL;
	bytes = calloc(IRTDB_FES_MSGHEADERLEN, 1);
	if (!bytes) return NULL;
	p = bytes;
	irtdb_fes_encode_int32(header->msgLen, p, *dumLen);
	irtdb_fes_encode_int16(header->msgType, p, *dumLen);
	irtdb_fes_encode_int8(header->dataType, p, *dumLen);
	irtdb_fes_encode_int8(header->version, p, *dumLen);
	irtdb_fes_encode_int32(header->crc32, p, *dumLen);

	return bytes;
}

irtdb_fes_header* irtdb_fes_parse_header(unsigned char *bytes,int len)
{
    irtdb_fes_header *header=NULL;  
	unsigned char *p = bytes;

	if (!bytes || (len < IRTDB_FES_MSGHEADERLEN)) return NULL;
	header = irtdb_fes_create_header();
	if (!header) return NULL;
	irtdb_fes_decode_int32(header->msgLen, p);
	irtdb_fes_decode_int16(header->msgType, p);
	irtdb_fes_decode_int8(header->dataType, p);
	irtdb_fes_decode_int8(header->version, p);
	irtdb_fes_decode_int32(header->crc32, p);
	return header;
}

int irtdb_fes_free_header(irtdb_fes_header* pHeader)
{
    irtdb_fes_free_pointer(pHeader); 
    return 0;
}

void irtdb_fes_show_fesid(irtdb_fes_fesid *pFesId)
{
    if(pFesId == NULL) return ;    
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_fesid begin --------------- ");
    irtdb_fes_show_header(pFesId->header);
    CMN_LOGGER_DEBUG(logger,"pFesId->len : %d",pFesId->len);
    CMN_LOGGER_DEBUG(logger,"pFesId->value : %s",pFesId->value);
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_fesid end --------------- ");       
}

unsigned char* irtdb_fes_dump_fesid(irtdb_fes_fesid *pFesId,int *dumpLen)
{
    int headerLen = 0;
    unsigned char *bytes = NULL,*headerBytes = NULL,*p = NULL;
    int dataOutLen = IRTDB_FES_MSGHEADERLEN; 

    *dumpLen = 0;    
    if(!pFesId || (pFesId->len <= 0) ||!pFesId->value) 
        return NULL;
	dataOutLen += sizeof(pFesId->len) + pFesId->len;

    bytes = calloc(dataOutLen,1);
    if(!bytes) return NULL;
    p = bytes;
	headerBytes = irtdb_fes_dump_header(pFesId->header,&headerLen);
	irtdb_fes_encode_bytes(headerBytes,headerLen,p,*dumpLen);
	free(headerBytes);
    irtdb_fes_encode_int32(pFesId->len,p,*dumpLen);
	irtdb_fes_encode_bytes(pFesId->value, pFesId->len,p, *dumpLen);
    return bytes;
}

irtdb_fes_fesid* irtdb_fes_create_fesid(const char* id)
{
    irtdb_fes_fesid *fesid = NULL;
    int len = 0;

	if (!id) return NULL;
	len = strlen(id);
	fesid = calloc(1, sizeof(*fesid));
	if (!fesid) return NULL;
	fesid->header = irtdb_fes_create_header();

	if(!fesid->header)
	{
		free(fesid);
		return NULL;
	}

	fesid->header->msgType = FESMSG_FESID;
	fesid->header->dataType = FESMSG_UPLOAD;
	fesid->header->msgLen = IRTDB_FES_MSGHEADERLEN - sizeof(fesid->header->msgLen);
	fesid->header->msgLen += sizeof(fesid->len) + len;

	fesid->len = len;
	fesid->value = calloc(len+1,1);
	memcpy(fesid->value, id, len);

	return fesid;
}

irtdb_fes_fesid* irtdb_fes_parse_fesid(const unsigned char *bytes,int len)
{
    irtdb_fes_fesid *fesid;
	unsigned char *p = (unsigned char*)bytes;
	int32_t uilen = 0;

	if (!bytes || (len < IRTDB_FES_MSGHEADERLEN)) return NULL;
	fesid = calloc(1, sizeof(*fesid));
	if (!fesid) return NULL;

	fesid->header = irtdb_fes_parse_header(p, IRTDB_FES_MSGHEADERLEN);
    if(!fesid->header)
    {
        free(fesid);
        return NULL;
    }
	p += IRTDB_FES_MSGHEADERLEN;

	irtdb_fes_decode_int32(fesid->len, p);
	uilen = fesid->len;

	fesid->value = calloc(uilen+1,1);
	if (!fesid->value)
	{
		free(fesid);
		return NULL;
	}

	irtdb_fes_decode_bytes(fesid->value, p, uilen);
	return fesid;
}

void irtdb_fes_free_fesid(irtdb_fes_fesid *fesid)
{
	if (!fesid) return;
	if (fesid->header) irtdb_fes_free_header(fesid->header);
	irtdb_fes_free_pointer(fesid->value);
	irtdb_fes_free_pointer(fesid);
}

irtdb_fes_cmn_path* irtdb_fes_create_cmn_path()
{
    irtdb_fes_cmn_path* path = NULL;
    path = malloc(sizeof(irtdb_fes_cmn_path));
    path->num = 0;
    path->values = NULL;
    return path;
}

void irtdb_fes_free_cmn_path(irtdb_fes_cmn_path* path)
{
    if(path)
    {
        int i = 0;
        for(i = 0 ; i < path->num ; i++)
        {
            irtdb_fes_free_pointer(path->values[i]);
        }
        irtdb_fes_free_pointer(path->values);
        irtdb_fes_free_pointer(path);
    }
}

static int irtdb_fes_cmn_path_add_nodekey(irtdb_fes_cmn_path *path, char *key)
{
    if (path && key)
    {
        char **values = realloc(path->values, sizeof(char*)*(path->num+1));
        if (!values)
        {
            CMN_LOGGER_DEBUG(logger,"irtdb_fes_cmn_path_add_nodekey : values is NULL");
            return -1;
        }
        values[path->num] = key;
        path->values = values;
        path->num++;
        return 0;
    }
    return -1;
}

// domain_label:station_label:pro_system_label:dev_lavel:di_label
irtdb_fes_cmn_path* irtdb_fes_parse_cmn_path(const char* spath)
{    
    irtdb_fes_cmn_path* path = NULL;
    char *newPath = NULL;
    int i,j,slen;
    char ch = ':';
    if(!spath) return NULL;
    CMN_LOGGER_DEBUG(logger,"spath : %s",spath);    
    slen = strlen(spath);
    newPath = malloc(slen + 2);
    memset(newPath,0,slen + 2);
    memcpy(newPath,spath,slen);
    newPath[slen] = ch;
    slen = strlen(newPath);
    path = irtdb_fes_create_cmn_path();
    if(!path) return NULL;
    j = 0;
    for(i = 0 ; i < slen ; i++)
    {
        if(newPath[i] == ch)
        {
            int tlen = i - j;
            char *key = NULL;
            
            key = malloc(tlen + 1);
            memset(key,0,tlen + 1);
            memcpy(key,newPath + j,tlen);
            key[tlen] = 0; 
            //CMN_LOGGER_DEBUG(logger,"%s",key);
            if(0 != irtdb_fes_cmn_path_add_nodekey(path,key))
            {
                free(key);
                irtdb_fes_free_cmn_path(path);
                CMN_LOGGER_DEBUG(logger,"irtdb_fes_cmn_path_add_nodekey fail");
                free(newPath);
                return NULL;
            }
            j = i + 1;
        }
    }
    free(newPath);    
    return path;
}

irtdb_fes_path_label* irtdb_fes_create_path_label()
{
	irtdb_fes_path_label *pathLabel = NULL;
	pathLabel = calloc(1, sizeof(*pathLabel));
	if (!pathLabel) return NULL;
	return pathLabel;
}

void irtdb_fes_free_path_label(irtdb_fes_path_label* pathLabel)
{
	if (!pathLabel) return;
	irtdb_fes_free_pointer(pathLabel->domainLabel);
	irtdb_fes_free_pointer(pathLabel->stationLabel);
	irtdb_fes_free_pointer(pathLabel->systemLabel);
	irtdb_fes_free_pointer(pathLabel->devLabel);
	irtdb_fes_free_pointer(pathLabel->dataLabel);
	free(pathLabel);
}

int irtdb_fes_init_path_label(irtdb_fes_path_label *pathLabel, irtdb_fes_cmn_path *path)
{
	int len = 0;
	if (!pathLabel || !path) return 0;
	irtdb_fes_encode_label(pathLabel->domainLabelLen, pathLabel->domainLabel, path->values[0], len);
	irtdb_fes_encode_label(pathLabel->stationLabelLen, pathLabel->stationLabel, path->values[1], len);
	irtdb_fes_encode_label(pathLabel->systemLabelLen, pathLabel->systemLabel, path->values[2], len);
	irtdb_fes_encode_label(pathLabel->devLabelLen, pathLabel->devLabel, path->values[3], len);
	irtdb_fes_encode_label(pathLabel->dataLabelLen, pathLabel->dataLabel, path->values[4], len);
	return len;
}

int irtdb_fes_show_path_label(irtdb_fes_path_label *pathLabel)
{
	if (!pathLabel) return -1;
	CMN_LOGGER_DEBUG(logger, "pathLabel->domainLabelLen : %d", pathLabel->domainLabelLen);
	CMN_LOGGER_DEBUG(logger, "pathLabel->domainLabel : %s", pathLabel->domainLabel);
	CMN_LOGGER_DEBUG(logger, "pathLabel->stationLabelLen : %d", pathLabel->stationLabelLen);
	CMN_LOGGER_DEBUG(logger, "pathLabel->stationLabel : %s", pathLabel->stationLabel);
	CMN_LOGGER_DEBUG(logger, "pathLabel->systemLabelLen : %d", pathLabel->systemLabelLen);
	CMN_LOGGER_DEBUG(logger, "pathLabel->systemLabel : %s", pathLabel->systemLabel);
	CMN_LOGGER_DEBUG(logger, "pathLabel->devLabelLen : %d", pathLabel->devLabelLen);
	CMN_LOGGER_DEBUG(logger, "pathLabel->devLabel : %s", pathLabel->devLabel);
	CMN_LOGGER_DEBUG(logger, "pathLabel->dataLabelLen : %d", pathLabel->dataLabelLen);
	CMN_LOGGER_DEBUG(logger, "pathLabel->dataLabel : %s", pathLabel->dataLabel);
	return 0;
}

unsigned char* irtdb_fes_dump_path_label(const irtdb_fes_path_label *pathLabel, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int len = 0;

	if (!pathLabel || !dumpLen) return NULL;

	*dumpLen = 0;
	len += sizeof(pathLabel->domainLabelLen) + pathLabel->domainLabelLen;
	len += sizeof(pathLabel->stationLabelLen) + pathLabel->stationLabelLen;
	len += sizeof(pathLabel->systemLabelLen) + pathLabel->systemLabelLen;
	len += sizeof(pathLabel->devLabelLen) + pathLabel->devLabelLen;
	len += sizeof(pathLabel->dataLabelLen) + pathLabel->dataLabelLen;
	bytes = calloc(len, 1);
	if (!bytes) return NULL;

	p = bytes;
	irtdb_fes_encode_int32(pathLabel->domainLabelLen, p, *dumpLen);
	irtdb_fes_encode_bytes(pathLabel->domainLabel, pathLabel->domainLabelLen, p, *dumpLen);
	irtdb_fes_encode_int32(pathLabel->stationLabelLen, p, *dumpLen);
	irtdb_fes_encode_bytes(pathLabel->stationLabel, pathLabel->stationLabelLen, p, *dumpLen);
	irtdb_fes_encode_int32(pathLabel->systemLabelLen, p, *dumpLen);
	irtdb_fes_encode_bytes(pathLabel->systemLabel, pathLabel->systemLabelLen, p, *dumpLen);
	irtdb_fes_encode_int32(pathLabel->devLabelLen, p, *dumpLen);
	irtdb_fes_encode_bytes(pathLabel->devLabel, pathLabel->devLabelLen, p, *dumpLen);
	irtdb_fes_encode_int32(pathLabel->dataLabelLen, p, *dumpLen);
	irtdb_fes_encode_bytes(pathLabel->dataLabel, pathLabel->dataLabelLen, p, *dumpLen);
	return bytes;
}

irtdb_fes_path_label* irtdb_fes_parse_path_label(unsigned char* p, int len)
{
	irtdb_fes_path_label *pathLabel = NULL;

	if (!p || len <= 0) return NULL;
	pathLabel = calloc(1, sizeof(*pathLabel));
	if (!pathLabel) return NULL;

	irtdb_fes_decode_label(pathLabel->domainLabelLen, pathLabel->domainLabel, p);
	irtdb_fes_decode_label(pathLabel->stationLabelLen, pathLabel->stationLabel, p);
	irtdb_fes_decode_label(pathLabel->systemLabelLen, pathLabel->systemLabel, p);
	irtdb_fes_decode_label(pathLabel->devLabelLen, pathLabel->devLabel, p);
	irtdb_fes_decode_label(pathLabel->dataLabelLen, pathLabel->dataLabel, p);

	return pathLabel;
}

irtdb_fes_di_msg* irtdb_fes_create_di_msg(
    irtdb_fes_cmn_path *path,uint8_t dataType,
    char *valueLabel, int32_t value,uint64_t timestamp)
{
    irtdb_fes_di_msg *dimsg = NULL;
    int len = 0,pathLableLen = 0;
    
    if(!path || !valueLabel ) return NULL;    
    dimsg = calloc(1,sizeof(*dimsg));
    dimsg->header = irtdb_fes_create_header();
    if(!dimsg->header)
    {
        free(dimsg);
        return NULL;
    }
    dimsg->header->msgType = FESMSG_UPLOAD_DI;
    dimsg->header->dataType = dataType;

	len += IRTDB_FES_MSGHEADERLEN;

	dimsg->pathLabel = irtdb_fes_create_path_label();
	if (!dimsg->pathLabel)
	{
		irtdb_fes_free_di_msg(dimsg);
		return NULL;
	}
	pathLableLen = irtdb_fes_init_path_label(dimsg->pathLabel, path);
	if (!pathLableLen)
	{
		irtdb_fes_free_di_msg(dimsg);
		return NULL;
	}
	len += pathLableLen;
    irtdb_fes_encode_label(dimsg->valueLabelLen,dimsg->valueLabel,valueLabel,len);   
    len += sizeof(value) + sizeof(timestamp); 
    dimsg->value = value;
    dimsg->timestamp = timestamp;
    dimsg->header->msgLen = len - sizeof(dimsg->header->msgLen);    
    
    return dimsg;
}

int irtdb_fes_show_di_msg(irtdb_fes_di_msg *dimsg)
{
    if(!dimsg) return 0;
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_di_msg begin --------------- ");
    irtdb_fes_show_header(dimsg->header);
	irtdb_fes_show_path_label(dimsg->pathLabel);
    CMN_LOGGER_DEBUG(logger,"dimsg->valueLabelLen : %d",dimsg->valueLabelLen);
    CMN_LOGGER_DEBUG(logger,"dimsg->valueLabel : %s",dimsg->valueLabel);
    CMN_LOGGER_DEBUG(logger,"dimsg->value : %d",dimsg->value);
    CMN_LOGGER_DEBUG(logger,"dimsg->timestamp : %d",dimsg->timestamp);
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_di_msg end --------------- ");    
    return 0;
}

unsigned char* irtdb_fes_dump_di_msg(const irtdb_fes_di_msg *dimsg,int *dumpLen)
{
    unsigned char *bytes = NULL,*p = NULL,*headerBytes = NULL,*pathBytes = NULL;
    int headerLen = 0,pathLabelLen = 0,len = 0;
    
	if (!dimsg || !dimsg->header || !dimsg->pathLabel || !dumpLen) 
		return NULL;
    
    *dumpLen = 0;
    len = sizeof(dimsg->header->msgLen) + dimsg->header->msgLen;
    bytes = calloc(len,1);
    if(!bytes) return NULL;
    
    p = bytes;    
    headerBytes = irtdb_fes_dump_header(dimsg->header,&headerLen);
	irtdb_fes_encode_bytes(headerBytes,headerLen,p,*dumpLen);
	free(headerBytes);

	pathBytes = irtdb_fes_dump_path_label(dimsg->pathLabel, &pathLabelLen);
	irtdb_fes_encode_bytes(pathBytes, pathLabelLen, p, *dumpLen);
	free(pathBytes);
    
    irtdb_fes_encode_int32(dimsg->valueLabelLen,p,*dumpLen);
	irtdb_fes_encode_bytes(dimsg->valueLabel,dimsg->valueLabelLen,p,*dumpLen);
    
    irtdb_fes_encode_int32(dimsg->value,p,*dumpLen);  
    irtdb_fes_encode_int64(dimsg->timestamp,p,*dumpLen);
    
    return bytes;
}

void irtdb_fes_free_di_msg(irtdb_fes_di_msg *dimsg)
{   
    if(!dimsg) return ;
	if(dimsg->header) irtdb_fes_free_header(dimsg->header);
	if(dimsg->pathLabel) irtdb_fes_free_path_label(dimsg->pathLabel);
    irtdb_fes_free_pointer(dimsg->valueLabel);
    free(dimsg);
}

int irtdb_fes_show_ai_msg(irtdb_fes_ai_msg *aimsg)
{
    if(!aimsg) return 0;
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_ai_msg begin --------------- ");
    irtdb_fes_show_header(aimsg->header);
	irtdb_fes_show_path_label(aimsg->pathLabel); 
    CMN_LOGGER_DEBUG(logger,"aimsg->value : %f",aimsg->value);
    CMN_LOGGER_DEBUG(logger,"aimsg->timestamp : %d",aimsg->timestamp);
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_ai_msg end --------------- ");    
    return 0;    
}

irtdb_fes_ai_msg* irtdb_fes_create_ai_msg(
    irtdb_fes_cmn_path *path,int8_t dataType,
    float value,int64_t timestamp)
{
    irtdb_fes_ai_msg *aimsg = NULL;
	int len = 0, pathLableLen = 0;

    if(!path ) return NULL; 
    aimsg = calloc(1,sizeof(*aimsg));
    if(!aimsg) return NULL;

    aimsg->header = irtdb_fes_create_header();
    if(!aimsg->header)
    {
        irtdb_fes_free_ai_msg(aimsg);
        return NULL;
    }
    
    aimsg->header->msgType = FESMSG_UPLOAD_AI;    
    aimsg->header->dataType = dataType; 
	len += IRTDB_FES_MSGHEADERLEN;
	aimsg->pathLabel = irtdb_fes_create_path_label();
	if (!aimsg->pathLabel)
	{
		irtdb_fes_free_ai_msg(aimsg);
		return NULL;
	}

	pathLableLen = irtdb_fes_init_path_label(aimsg->pathLabel, path);
	if (!pathLableLen)
	{
		irtdb_fes_free_ai_msg(aimsg);
		return NULL;
	}
	len += pathLableLen;
    len += sizeof(value) + sizeof(timestamp);
    aimsg->value = value;
    aimsg->timestamp = timestamp;
    aimsg->header->msgLen = len - sizeof(aimsg->header->msgLen);
    
    return aimsg;
}

unsigned char* irtdb_fes_dump_ai_msg(const irtdb_fes_ai_msg *aimsg,int *dumpLen)
{
    unsigned char *bytes = NULL, *p = NULL, *headerBytes = NULL,*pathBytes = NULL;
    int headerLen = 0,pathLabelLen = 0,len = 0;
    
    if(!aimsg || !aimsg->header || !dumpLen)
        return NULL;
    
    *dumpLen = 0;
    len = sizeof(aimsg->header->msgLen) + aimsg->header->msgLen;
    bytes = calloc(len,1);
    if(!bytes) return NULL;
    p = bytes;
    
    headerBytes = irtdb_fes_dump_header(aimsg->header,&headerLen);
	irtdb_fes_encode_bytes(headerBytes,headerLen,p,*dumpLen);
	free(headerBytes);
    
    pathBytes = irtdb_fes_dump_path_label(aimsg->pathLabel, &pathLabelLen);
	irtdb_fes_encode_bytes(pathBytes, pathLabelLen, p, *dumpLen);
	free(pathBytes);
        
    irtdb_fes_encode_float32(aimsg->value,p,*dumpLen);  
    irtdb_fes_encode_int64(aimsg->timestamp,p,*dumpLen);
	return bytes;
}

void irtdb_fes_free_ai_msg(irtdb_fes_ai_msg *aimsg)
{
    if(!aimsg) return ;
    irtdb_fes_free_header(aimsg->header);
	irtdb_fes_free_path_label(aimsg->pathLabel);
    irtdb_fes_free_pointer(aimsg);
}

irtdb_fes_acc_msg* irtdb_fes_create_acc_msg(
    irtdb_fes_cmn_path *path,int8_t dataType,int value, 
    int64_t timestamp)
{
    irtdb_fes_acc_msg *accmsg = NULL;
    int len = 0,pathLableLen = 0;
    
    if(!path) return NULL;

    accmsg = calloc(1,sizeof(*accmsg));
    if(!accmsg) return NULL;
    accmsg->header = irtdb_fes_create_header();
    if(!accmsg->header) 
    {
        irtdb_fes_free_acc_msg(accmsg);
        return NULL;
    }

    accmsg->header->msgType = FESMSG_UPLOAD_ACC;
    accmsg->header->dataType = dataType; 
	len += IRTDB_FES_MSGHEADERLEN;
    accmsg->pathLabel = irtdb_fes_create_path_label();
	if (!accmsg->pathLabel)
	{
		irtdb_fes_free_acc_msg(accmsg);
		return NULL;
	}
	pathLableLen = irtdb_fes_init_path_label(accmsg->pathLabel, path);
	if (!pathLableLen)
	{
		irtdb_fes_free_acc_msg(accmsg);
		return NULL;
	}
	len += pathLableLen;
    len += sizeof(value) + sizeof(timestamp);    
    accmsg->value = value;
    accmsg->timestamp = timestamp;
    accmsg->header->msgLen = len - sizeof(accmsg->header->msgLen);
    
    return accmsg;
}

int irtdb_fes_show_acc_msg(irtdb_fes_acc_msg *accmsg)
{
    if(!accmsg) return 0;
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_acc_msg begin --------------- ");
    irtdb_fes_show_header(accmsg->header);
    irtdb_fes_show_path_label(accmsg->pathLabel);    
    CMN_LOGGER_DEBUG(logger,"accmsg->value : %d",accmsg->value);
    CMN_LOGGER_DEBUG(logger,"accmsg->timestamp : %d",accmsg->timestamp);
    CMN_LOGGER_DEBUG(logger," ------------- irtdb_fes_show_acc_msg end --------------- ");    
    return 0;    
}

unsigned char* irtdb_fes_dump_acc_msg(const irtdb_fes_acc_msg *accmsg,int *dumpLen)
{
    unsigned char *bytes = NULL, *p = NULL, *headerBytes = NULL,*pathBytes = NULL;
    int headerLen = 0,pathLabelLen = 0,len = 0;
    
    if(!accmsg || !accmsg->header || !dumpLen)
        return NULL;
    
    *dumpLen = 0;
    len = sizeof(accmsg->header->msgLen) + accmsg->header->msgLen;
    bytes = calloc(len,1);
    if(!bytes) return NULL;
    p = bytes;
    
    headerBytes = irtdb_fes_dump_header(accmsg->header,&headerLen);
	irtdb_fes_encode_bytes(headerBytes,headerLen,p,*dumpLen);
	free(headerBytes);
    
    pathBytes = irtdb_fes_dump_path_label(accmsg->pathLabel, &pathLabelLen);
	irtdb_fes_encode_bytes(pathBytes, pathLabelLen, p, *dumpLen);
	free(pathBytes);
     
    irtdb_fes_encode_int32(accmsg->value,p,*dumpLen);  
    irtdb_fes_encode_int64(accmsg->timestamp,p,*dumpLen);

    return bytes;
}

void irtdb_fes_free_acc_msg(irtdb_fes_acc_msg *accmsg)
{
    if(!accmsg) return ;
    irtdb_fes_free_header(accmsg->header);
	irtdb_fes_free_path_label(accmsg->pathLabel);
    irtdb_fes_free_pointer(accmsg);
}

irtdb_fes_cmd* irtdb_fes_create_cmd()
{
	irtdb_fes_cmd *fescmd;
	fescmd = calloc(1, sizeof(*fescmd));
	if (!fescmd) return NULL;
	fescmd->header = NULL;
	fescmd->value = NULL;
	return fescmd;
}

void irtdb_fes_free_cmd(irtdb_fes_cmd *fescmd)
{
	if (!fescmd) return;
	if (fescmd->header) irtdb_fes_free_header(fescmd->header);
	if (fescmd->value) free(fescmd->value);
	free(fescmd);
	fescmd = NULL;
}

int irtdb_fes_init_cmd(irtdb_fes_cmd *fescmd,const char *id, int vlen, const char* value)
{
	// 0 : ok ; -1 : fail
	int tlen = 0;

	if (!fescmd || !id || (vlen <= 0) || !vlen) return -1;
	if (strlen(value) < vlen) return -1;
	if (!fescmd->header) fescmd->header = irtdb_fes_create_header();
	if (!fescmd->header) return -1;

	tlen += IRTDB_FES_MSGHEADERLEN;

	snprintf(fescmd->id, sizeof(fescmd->id), "%s", id);
	fescmd->vlen = vlen;
	if (fescmd->value) free(fescmd->value);
	fescmd->value = calloc(vlen, 1);
	if (!fescmd->value) return -1;
	memcpy(fescmd->value, value, vlen);

	tlen += sizeof(fescmd->id);
	tlen += sizeof(fescmd->vlen) + vlen;

	fescmd->header->msgType = IRTDB_FES_CMD;
	fescmd->header->msgLen = tlen;

	return 0;
}

unsigned char* irtdb_fes_dump_cmd(irtdb_fes_cmd *fescmd, int *dumpLen)
{
	unsigned char *bytes = NULL,*p = NULL;
	unsigned char *headerBytes = NULL;
	int headerLen = 0,len = 0;
	
	if (!fescmd || !fescmd->header) return NULL;
	*dumpLen = 0;
	bytes = calloc(fescmd->header->msgLen, 1);
	if (!bytes) return NULL;
	p = bytes;
	headerBytes = irtdb_fes_dump_header(fescmd->header, &headerLen);
	irtdb_fes_encode_bytes(headerBytes,headerLen,p,*dumpLen);
	free(headerBytes);
	irtdb_fes_encode_bytes(fescmd->id, sizeof(fescmd->id), p, *dumpLen);
	irtdb_fes_encode_int16(fescmd->vlen, p, *dumpLen);
	irtdb_fes_encode_bytes(fescmd->value, fescmd->vlen, p, *dumpLen);

	return bytes;
}

irtdb_fes_cmd* irtdb_fes_parse_cmd(unsigned char* bytes, int len)
{
	irtdb_fes_cmd *fescmd = NULL;
	unsigned char *p = bytes;

	if (!bytes || (len < IRTDB_FES_MSGHEADERLEN))
		return NULL;
	fescmd = irtdb_fes_create_cmd();
	if (!fescmd) return NULL;

	fescmd->header = irtdb_fes_parse_header(bytes,IRTDB_FES_MSGHEADERLEN);
	if (!fescmd->header)
	{
		irtdb_fes_free_cmd(fescmd);
		return NULL;
	}
	p += IRTDB_FES_MSGHEADERLEN;
	irtdb_fes_decode_bytes(fescmd->id, p, sizeof(fescmd->id));
	irtdb_fes_decode_int16(fescmd->vlen, p);
    fescmd->value = calloc(fescmd->vlen + 1,1);
    if(!fescmd->value)
    {
        irtdb_fes_free_cmd(fescmd);
        return NULL;
    }
	irtdb_fes_decode_bytes(fescmd->value, p, fescmd->vlen);

	return fescmd;
}

