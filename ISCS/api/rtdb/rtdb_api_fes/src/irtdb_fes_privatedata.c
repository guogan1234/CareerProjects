#include <stdlib.h>
#include <cmn_logger.h>
#include "irtdb_fes_common.h"
#include "irtdb_fes_privatecommon.h"
#include "irtdb_fes_privatedata.h"
#include "cmn_list.h"

extern cmn_logger *logger;

#define irtdb_fes_encode_bytes(src, srcLen, p, dumpLen) \
	do                                                  \
	{                                                   \
		memcpy(p, src, srcLen);                         \
		p += srcLen;                                    \
		dumpLen += srcLen;                              \
	} while (0)

#define irtdb_fes_decode_bytes(dst, p, dstLen) \
	do                                         \
	{                                          \
		memcpy(dst, p, dstLen);                \
		p += dstLen;                           \
	} while (0)

#define irtdb_fes_encode_base(type, label, base, src, p, dumpLen) \
	do                                                            \
	{                                                             \
		type tmp = irtdb_fes_hton##label##base(src);              \
		irtdb_fes_encode_bytes(&tmp, base / 8, p, dumpLen);       \
	} while (0)

#define irtdb_fes_decode_base(type, label, base, dst, p) \
	do                                                   \
	{                                                    \
		type tmp;                                        \
		irtdb_fes_decode_bytes(&tmp, p, base / 8);       \
		dst = irtdb_fes_ntoh##label##base(tmp);          \
	} while (0)

#define irtdb_fes_encode_int8(src, p, dumpLen) irtdb_fes_encode_bytes(&src, 1, p, dumpLen)
#define irtdb_fes_encode_int16(src, p, dumpLen) irtdb_fes_encode_base(int16_t, i, 16, src, p, dumpLen)
#define irtdb_fes_encode_int32(src, p, dumpLen) irtdb_fes_encode_base(int32_t, i, 32, src, p, dumpLen)
#define irtdb_fes_encode_int64(src, p, dumpLen) irtdb_fes_encode_base(int64_t, i, 64, src, p, dumpLen)
#define irtdb_fes_encode_float32(src, p, dumpLen) irtdb_fes_encode_base(float, f, 32, src, p, dumpLen)
#define irtdb_fes_encode_float64(src, p, dumpLen) irtdb_fes_encode_base(double, f, 64, src, p, dumpLen)

#define irtdb_fes_decode_int8(dst, p) irtdb_fes_decode_bytes(&dst, p, 1)
#define irtdb_fes_decode_int16(dst, p) irtdb_fes_decode_base(int16_t, i, 16, dst, p)
#define irtdb_fes_decode_int32(dst, p) irtdb_fes_decode_base(int32_t, i, 32, dst, p)
#define irtdb_fes_decode_int64(dst, p) irtdb_fes_decode_base(int64_t, i, 64, dst, p)
#define irtdb_fes_decode_float32(dst, p) irtdb_fes_decode_base(float, f, 32, dst, p)
#define irtdb_fes_decode_float64(dst, p) irtdb_fes_decode_base(double, f, 64, dst, p)

static int irtdb_fes_encode_header(irtdb_fes_header *header, int *dumpLen, unsigned char **p);
static int irtdb_fes_encode_pathlabel(irtdb_fes_path_label *pathLabel, int *dumpLen, unsigned char **p);
static int irtdb_fes_encode_label(int32_t *dstLen, char **dstLabel, char *srcLabel, int *dataOutLen);
static int irtdb_fes_sock_read(int socket, void *bufOut, int bufLen);
static int irtdb_fes_sock_write(int socket, irtdb_fes_message_bin *msgbin);
static int irtdb_fes_cmn_path_add_nodekey(irtdb_fes_cmn_path *path, char *key);

irtdb_fes_dbserver *irtdb_fes_create_dbserver(const char *ip1, const char *ip2, int port,const char *username,const char *password)
{
	irtdb_fes_dbserver *pObj = NULL;

	pObj = calloc(1, sizeof(*pObj));
	if (!pObj)
	{
		return NULL;
	}

	if (ip1)
	{
		pObj->ip1 = strdup(ip1);
		if (!pObj->ip1)
		{
			free(pObj);
			return NULL;
		}
	}
	if (ip2)
	{
		pObj->ip2 = strdup(ip2);
		if (!pObj->ip2)
		{
			irtdb_fes_free_dbserver(pObj);
			return NULL;
		}
	}
	if (username)
	{
		pObj->username = strdup(username);
		if (!pObj->username)
		{
			irtdb_fes_free_dbserver(pObj);
			return NULL;
		}
	}
	if (password)
	{
		pObj->password = strdup(password);
		if (!pObj->password)
		{
			irtdb_fes_free_dbserver(pObj);
			return NULL;
		}
	}
	pObj->isAvailable = 0;
	pObj->port = port;

	CMN_LOGGER_DEBUG(logger, "irtdb_fes_create_dbserver ok");
	return pObj;
}

void irtdb_fes_free_dbserver(irtdb_fes_dbserver *pServer)
{
	if (!pServer)
		return;
	irtdb_fes_free_pointer(pServer->ip1);
	irtdb_fes_free_pointer(pServer->ip2);
	irtdb_fes_free_pointer(pServer->username);
	irtdb_fes_free_pointer(pServer->password);
	irtdb_fes_free_pointer(pServer);
}

irtdb_fes_message_bin *irtdb_fes_create_message_bin(const unsigned char *data, int dataLen)
{
	irtdb_fes_message_bin *msgbin = NULL;

	if (!data || (dataLen < 0))
		return NULL;

	msgbin = calloc(1, sizeof(*msgbin));
	if (!msgbin)
		return NULL;

	msgbin->data = calloc(dataLen + 1, 1);
	if (!msgbin->data)
	{
		free(msgbin);
		return NULL;
	}
	memcpy(msgbin->data, data, dataLen);
	msgbin->length = dataLen;

	return msgbin;
}

void irtdb_fes_delete_message_bin(void *msgbin)
{
	irtdb_fes_message_bin *mb = (irtdb_fes_message_bin *)msgbin;
	if (!mb)
		return;
	irtdb_fes_free_pointer(mb->data);
	irtdb_fes_free_pointer(mb);
}

irtdb_fes_message_bin *irtdb_fes_dup_message_bin(const irtdb_fes_message_bin *msgbinIn)
{
	irtdb_fes_message_bin *msgbinOut = NULL;
	if (!msgbinIn)
		return NULL;
	msgbinOut = calloc(1, sizeof(*msgbinOut));
	if (!msgbinOut)
		return NULL;

	msgbinOut->data = calloc(msgbinIn->length, 1);
	if (!msgbinOut->data)
	{
		free(msgbinOut);
		return NULL;
	}
	memcpy(msgbinOut->data, msgbinIn->data, msgbinIn->length);
	msgbinOut->length = msgbinIn->length;

	return msgbinOut;
}

irtdb_fes_peer *irtdb_fes_create_peer()
{
	irtdb_fes_peer *peer = NULL;
	peer = calloc(1, sizeof(*peer));
	if (!peer)
		return NULL;
	peer->send_buffer = cmn_list_create(NULL, irtdb_fes_delete_message_bin, NULL);
	if (!peer->send_buffer)
	{
		free(peer);
		return NULL;
	}

	return peer;
}

void irtdb_fes_delete_peer(irtdb_fes_peer *peer)
{
	if (!peer)
		return;
	close(peer->socket);
	if (peer->send_buffer)
	{
		cmn_list_release(peer->send_buffer);
		peer->send_buffer = NULL;
	}
	irtdb_fes_delete_message_bin(peer->recvBuffer);
	irtdb_fes_free_pointer(peer);
}

int irtdb_fes_peer_add_to_send(irtdb_fes_peer *peer, const irtdb_fes_message_bin *msg)
{
	if (peer && peer->send_buffer && (peer->socket > 0) && msg)
	{
		irtdb_fes_message_bin *dupMsg = irtdb_fes_dup_message_bin(msg);
		if (dupMsg)
		{
			if (cmn_list_add_tail(peer->send_buffer, dupMsg))
			{
				CMN_LOGGER_DEBUG(logger, "irtdb_fes_peer_add_to_send success");
				return 0;
			}
			else
			{
				irtdb_fes_delete_message_bin(dupMsg);
			}
		}
	}

	CMN_LOGGER_DEBUG(logger, "irtdb_fes_peer_add_to_send fail");

	return -1;
}

int irtdb_fes_recv_from_peer(irtdb_fes_peer *peer, int (*message_handler)(irtdb_fes_message_bin *))
{
	unsigned char *binBuf = NULL;
	irtdb_fes_message_bin *msgbin = NULL;
	int msgLen = 0, code = 0;

	if (!peer)
		return -1;
	msgLen = 0;
	code = irtdb_fes_sock_read(peer->socket, &msgLen, 4);

	if (0 == code)
	{
		msgLen = irtdb_fes_ntohi32(msgLen);
		if (msgLen < IRTDB_FES_MSGHEADERLEN || msgLen > IFES_MAX_MSG_SIZE)
			return -1;
		printf("msgLen = %d\n", msgLen);
		binBuf = calloc(msgLen, 1);
		if (!binBuf)
			return -1;
		code = irtdb_fes_sock_read(peer->socket, binBuf, msgLen);
		if (0 == code)
		{
			msgbin = irtdb_fes_create_message_bin(binBuf, msgLen);
			message_handler(msgbin); // 这里的msgbin由谁进行释放，KWD
		}
		irtdb_fes_free_pointer(binBuf);
	}

	return (-1 == code) ? -1 : 0;
}

int irtdb_fes_send_to_peer(irtdb_fes_peer *peer)
{
	int code = 0;
	cmn_list_iterator *pIter;
	cmn_list_node *cmnNode = NULL;
	irtdb_fes_message_bin *msgbin = NULL;

	if (!peer || !peer->send_buffer)
		return -1;
	if (!cmn_list_get_size(peer->send_buffer))
		return 0;
	pIter = cmn_list_get_iterator(peer->send_buffer, CMN_LIST_HEAD);
	if (!pIter)
		return -1;
	while (cmnNode = cmn_list_next(pIter))
	{
		msgbin = (irtdb_fes_message_bin *)cmn_list_node_get_value(cmnNode);
		if (!msgbin)
			continue;
		code = irtdb_fes_sock_write(peer->socket, msgbin);
		cmn_list_remove(peer->send_buffer, cmnNode);
		if (0 != code)
			break;
	}
	cmn_list_release_iterator(pIter);
	return (-1 == code) ? -1 : 0;
}

irtdb_fes_dbnode *irtdb_fes_create_dbnode(irtdb_fes_dbserver *pServer)
{
	irtdb_fes_dbnode *pNode = NULL;

	if (!pServer)
	{
		return NULL;
	}
	pNode = calloc(1, sizeof(*pNode));
	if (!pNode)
	{
		return NULL;
	}
	pNode->peer = irtdb_fes_create_peer();
	if (!pNode->peer)
	{
		free(pNode);
		return NULL;
	}

	pNode->pRtdbInfo = pServer;
	
	return pNode;
}

void irtdb_fes_free_dbnode(irtdb_fes_dbnode *pNode)
{
	irtdb_fes_dbnode *pNext = pNode;

	while(pNext)
	{
		pNode = pNext->next;
		irtdb_fes_send_to_peer(pNext->peer);
		irtdb_fes_free_dbserver(pNext->pRtdbInfo);
		irtdb_fes_delete_peer(pNext->peer);
		free(pNext);
		pNext = pNode;
	};
}

irtdb_fes_header *irtdb_fes_create_header()
{
	irtdb_fes_header *header = calloc(1, sizeof(*header));
	return header;
}

unsigned char *irtdb_fes_dump_header(irtdb_fes_header *header, int *dumLen)
{
	unsigned char *bytes = NULL, *p = NULL;

	if (!header || !dumLen)
		return NULL;
	bytes = calloc(IRTDB_FES_MSGHEADERLEN, 1);
	if (!bytes)
		return NULL;
	p = bytes;
	irtdb_fes_encode_int32(header->msgLen, p, *dumLen);
	irtdb_fes_encode_int16(header->msgType, p, *dumLen);
	irtdb_fes_encode_int8(header->dataType, p, *dumLen);
	irtdb_fes_encode_int8(header->version, p, *dumLen);
	irtdb_fes_encode_int32(header->crc32, p, *dumLen);

	return bytes;
}

irtdb_fes_header *irtdb_fes_parse_header(unsigned char *bytes, int len)
{
	irtdb_fes_header *header = NULL;
	unsigned char *p = bytes;

	if (!bytes || (len < IRTDB_FES_MSGHEADERLEN - 4))
		return NULL;
	header = irtdb_fes_create_header();
	if (!header)
		return NULL;
	header->msgLen = len;
	//irtdb_fes_decode_int32(header->msgLen, p);
	irtdb_fes_decode_int16(header->msgType, p);
	irtdb_fes_decode_int8(header->dataType, p);
	irtdb_fes_decode_int8(header->version, p);
	irtdb_fes_decode_int32(header->crc32, p);
	return header;
}

int irtdb_fes_free_header(irtdb_fes_header *pHeader)
{
	if (pHeader)
	{
		free(pHeader);
	}
	return 0;
}

unsigned char *irtdb_fes_dump_fesid(irtdb_fes_fesid *pFesId, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int dataOutLen = 0;

	*dumpLen = 0;
	if (!pFesId || (pFesId->len <= 0) || !pFesId->value)
		return NULL;
	dataOutLen = IRTDB_FES_MSGHEADERLEN + sizeof(pFesId->len) + pFesId->len;

	bytes = calloc(dataOutLen, 1);
	if (!bytes)
		return NULL;
	p = bytes;
	if (-1 == irtdb_fes_encode_header(pFesId->header, dumpLen, &p))
		return NULL;
	irtdb_fes_encode_int32(pFesId->len, p, *dumpLen);
	irtdb_fes_encode_bytes(pFesId->value, pFesId->len, p, *dumpLen);
	return bytes;
}

irtdb_fes_fesid *irtdb_fes_create_fesid(const char *id)
{
	irtdb_fes_fesid *fesid = NULL;
	int len = 0;

	if (!id || id[0] == '\0')
	{
		return NULL;
	}

	len = strlen(id) ;
	fesid = calloc(1, sizeof(*fesid));
	if (!fesid)
	{
		return NULL;
	}

	fesid->header = irtdb_fes_create_header();
	if (!fesid->header)
	{
		free(fesid);
		return NULL;
	}

	fesid->header->msgType = FESMSG_FESID;
	fesid->header->dataType = FESMSG_UPLOAD;
	fesid->header->msgLen = IRTDB_FES_MSGHEADERLEN - sizeof(fesid->header->msgLen);
	fesid->header->msgLen += sizeof(fesid->len) + len;

	fesid->len = len;
	fesid->value = calloc(len + 1, 1);
	if (!fesid->value)
	{
		irtdb_fes_free_fesid(fesid);
		return NULL;
	}
	memcpy(fesid->value, id, len);

	return fesid;
}

irtdb_fes_fesid *irtdb_fes_parse_fesid(const unsigned char *bytes, int len)
{
	irtdb_fes_fesid *fesid;
	unsigned char *p = (unsigned char *)bytes;
	int32_t uilen = 0;

	if (!bytes || (len < IRTDB_FES_MSGHEADERLEN))
		return NULL;
	fesid = calloc(1, sizeof(*fesid));
	if (!fesid)
		return NULL;

	fesid->header = irtdb_fes_parse_header(p, IRTDB_FES_MSGHEADERLEN);
	if (!fesid->header)
	{
		free(fesid);
		return NULL;
	}
	p += IRTDB_FES_MSGHEADERLEN;

	irtdb_fes_decode_int32(fesid->len, p);
	uilen = fesid->len;

	fesid->value = calloc(uilen + 1, 1);
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
	if (!fesid)
		return;
	if (fesid->header)
		irtdb_fes_free_header(fesid->header);
	irtdb_fes_free_pointer(fesid->value);
	irtdb_fes_free_pointer(fesid);
}

irtdb_fes_cmn_path *irtdb_fes_create_cmn_path()
{
	irtdb_fes_cmn_path *path = malloc(sizeof(irtdb_fes_cmn_path));

	if (path)
	{
		path->num = 0;
		path->values = NULL;
	}

	return path;
}

void irtdb_fes_free_cmn_path(irtdb_fes_cmn_path *path)
{
	int i = 0;

	if (!path) return;
	
	for (i = 0; i < path->num; i++)
	{
		irtdb_fes_free_pointer(path->values[i]);
	}
	irtdb_fes_free_pointer(path->values);
	irtdb_fes_free_pointer(path);
}

// domain_label:station_label:pro_system_label:dev_lavel:di_label
irtdb_fes_cmn_path *irtdb_fes_parse_cmn_path(const char *spath)
{
	irtdb_fes_cmn_path *path = NULL;
	char *newPath = NULL, *key = NULL;
	;
	int i, j, slen, tlen;
	char ch = ':';

	CMN_LOGGER_DEBUG(logger, "spath : %s", spath ? spath : "");
	slen = spath ? strlen(spath) : 0;
	if (!slen)
		return NULL;

	newPath = malloc(slen + 2);
	if (!newPath)
		return NULL;
	memset(newPath, 0, slen + 2);
	memcpy(newPath, spath, slen);
	newPath[slen] = ch;
	slen = strlen(newPath);
	path = irtdb_fes_create_cmn_path();
	if (!path)
	{
		free(newPath);
		return NULL;
	}

	for (i = 0, j = 0; i < slen; i++)
	{
		if (newPath[i] != ch)
			continue;
		tlen = i - j;
		key = calloc(tlen + 1, 1);
		if (!key)
			goto ERR_END;
		memcpy(key, newPath + j, tlen);
		key[tlen] = 0;
		//CMN_LOGGER_DEBUG(logger,"%s",key);
		if (0 != irtdb_fes_cmn_path_add_nodekey(path, key))
		{
			free(key);
			CMN_LOGGER_DEBUG(logger, "irtdb_fes_cmn_path_add_nodekey fail");
			goto ERR_END;
		}
		j = i + 1;
	}
	free(newPath);

	return path;

ERR_END:
	CMN_LOGGER_DEBUG(logger, "irtdb_fes_cmn_path_add_nodekey fail");
	irtdb_fes_free_cmn_path(path);
	free(newPath);
	return NULL;
}

irtdb_fes_path_label *irtdb_fes_create_path_label()
{
	irtdb_fes_path_label *pathLabel = NULL;
	pathLabel = calloc(1, sizeof(*pathLabel));
	return pathLabel;
}

void irtdb_fes_free_path_label(irtdb_fes_path_label *pathLabel)
{
	if (pathLabel)
	{
		// 释放dataLabel数组　KWD
		for (int i = 0; i < 5; i++)
		{
			if (pathLabel->dataLabel[i])
			{
				free(pathLabel->dataLabel[i]);
			}
		}
		free(pathLabel);
	}
}

int irtdb_fes_init_path_label(irtdb_fes_path_label *pathLabel, irtdb_fes_cmn_path *path)
{
	int len = 0, i, status;

	if (!pathLabel || !path || (path->num != 5) )
	{
		return 0;
	}

	for (int i = 0; i < 5; i++)
	{
		status = irtdb_fes_encode_label(&pathLabel->arrLabelLen[i], &pathLabel->dataLabel[i], path->values[i], &len);
		if (-1 == status)
		{
			return 0;
		}
	}

	return len;
}

unsigned char *irtdb_fes_dump_path_label(const irtdb_fes_path_label *pathLabel, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int len = 0, i = 0;

	if (!pathLabel || !dumpLen)
	{
		return NULL;
	}

	*dumpLen = 0;
	for (i = 0; i < 5; i++)
	{
		len += sizeof(int32_t) + pathLabel->arrLabelLen[i];
	}
	bytes = calloc(len+1, 1);
	if (!bytes)
	{
		return NULL;
	}

	p = bytes;
	for (i = 0; i < 5; i++)
	{
		irtdb_fes_encode_int32(pathLabel->arrLabelLen[i], p, *dumpLen);
		irtdb_fes_encode_bytes(pathLabel->dataLabel[i], pathLabel->arrLabelLen[i], p, *dumpLen);
	}

	return bytes;
}

irtdb_fes_di_msg *irtdb_fes_create_di_msg(
	irtdb_fes_cmn_path *path, 
	uint8_t dataType, 
	char *valueLabel, 
	int32_t value, 
	uint64_t timestamp
)
{
	irtdb_fes_di_msg *dimsg = NULL;
	int len = 0, pathLableLen = 0;

	if (!path || !valueLabel)
	{
		return NULL;
	}
	dimsg = calloc(1, sizeof(*dimsg));
	if (!dimsg)
	{
		return NULL;
	}
	dimsg->header = irtdb_fes_create_header();
	if (!dimsg->header)
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
	if (-1 == irtdb_fes_encode_label(&dimsg->valueLabelLen, &dimsg->valueLabel, valueLabel, &len))
	{
		irtdb_fes_free_di_msg(dimsg);
		return NULL;
	}

	len += sizeof(value) + sizeof(timestamp);
	dimsg->value = value;
	dimsg->timestamp = timestamp;
	dimsg->header->msgLen = len - sizeof(dimsg->header->msgLen);

	return dimsg;
}

unsigned char *irtdb_fes_dump_di_msg(const irtdb_fes_di_msg *dimsg, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int len = 0;

	if (!dimsg || !dimsg->header || !dimsg->pathLabel || !dumpLen)
	{
		return NULL;
	}

	*dumpLen = 0;
	len = sizeof(dimsg->header->msgLen) + dimsg->header->msgLen;
	bytes = calloc(len, 1);
	if (!bytes)
	{
		return NULL;
	}

	p = bytes;
	if (-1 == irtdb_fes_encode_header(dimsg->header, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}
	if (-1 == irtdb_fes_encode_pathlabel(dimsg->pathLabel, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}

	irtdb_fes_encode_int32(dimsg->valueLabelLen, p, *dumpLen);
	irtdb_fes_encode_bytes(dimsg->valueLabel, dimsg->valueLabelLen, p, *dumpLen);
	irtdb_fes_encode_int32(dimsg->value, p, *dumpLen);
	irtdb_fes_encode_int64(dimsg->timestamp, p, *dumpLen);

	return bytes;
}

void irtdb_fes_free_di_msg(irtdb_fes_di_msg *dimsg)
{
	if (dimsg)
	{
		if (dimsg->header)
		{
			irtdb_fes_free_header(dimsg->header);
		}
		if (dimsg->pathLabel)
		{
			irtdb_fes_free_path_label(dimsg->pathLabel);
		}
		if (dimsg->valueLabel)
		{
			free(dimsg->valueLabel);
		}
		free(dimsg);
	}
}

irtdb_fes_ai_msg *irtdb_fes_create_ai_msg(irtdb_fes_cmn_path *path, int8_t dataType, float value, int64_t timestamp)
{
	irtdb_fes_ai_msg *aimsg = NULL;
	int len = 0, pathLableLen = 0;

	if (!path)
	{
		return NULL;
	}
	aimsg = calloc(1, sizeof(*aimsg));
	if (!aimsg)
	{
		return NULL;
	}
	aimsg->header = irtdb_fes_create_header();
	if (!aimsg->header)
	{
		free(aimsg);
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

unsigned char *irtdb_fes_dump_ai_msg(const irtdb_fes_ai_msg *aimsg, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int len = 0;

	if (!aimsg || !aimsg->header || !dumpLen)
	{
		return NULL;
	}

	*dumpLen = 0;
	len = sizeof(aimsg->header->msgLen) + aimsg->header->msgLen;
	bytes = calloc(len, 1);
	if (!bytes)
	{
		return NULL;
	}

	p = bytes;
	if (-1 == irtdb_fes_encode_header(aimsg->header, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}
	if (-1 == irtdb_fes_encode_pathlabel(aimsg->pathLabel, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}

	irtdb_fes_encode_float32(aimsg->value, p, *dumpLen);
	irtdb_fes_encode_int64(aimsg->timestamp, p, *dumpLen);

	return bytes;
}

void irtdb_fes_free_ai_msg(irtdb_fes_ai_msg *aimsg)
{
	if (aimsg)
	{
		irtdb_fes_free_header(aimsg->header);
		irtdb_fes_free_path_label(aimsg->pathLabel);
		free(aimsg);
	}
}

irtdb_fes_acc_msg *irtdb_fes_create_acc_msg(irtdb_fes_cmn_path *path, int8_t dataType, int value, int64_t timestamp)
{
	irtdb_fes_acc_msg *accmsg = NULL;
	int len = 0, pathLableLen = 0;

	if (!path)
	{
		return NULL;
	}
	accmsg = calloc(1, sizeof(*accmsg));
	if (!accmsg)
	{
		return NULL;
	}
	accmsg->header = irtdb_fes_create_header();
	if (!accmsg->header)
	{
		free(accmsg);
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

unsigned char *irtdb_fes_dump_acc_msg(const irtdb_fes_acc_msg *accmsg, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int len = 0;

	if (!accmsg || !accmsg->header || !dumpLen)
	{
		return NULL;
	}

	*dumpLen = 0;
	len = sizeof(accmsg->header->msgLen) + accmsg->header->msgLen;
	bytes = calloc(len, 1);
	if (!bytes)
	{
		return NULL;
	}

	p = bytes;
	if (-1 == irtdb_fes_encode_header(accmsg->header, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}
	if (-1 == irtdb_fes_encode_pathlabel(accmsg->pathLabel, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}

	irtdb_fes_encode_int32(accmsg->value, p, *dumpLen);
	irtdb_fes_encode_int64(accmsg->timestamp, p, *dumpLen);

	return bytes;
}

void irtdb_fes_free_acc_msg(irtdb_fes_acc_msg *accmsg)
{
	if (accmsg)
	{
		irtdb_fes_free_header(accmsg->header);
		irtdb_fes_free_path_label(accmsg->pathLabel);
		free(accmsg);
	}
}

irtdb_fes_soe_msg *irtdb_fes_create_soe_msg(
	irtdb_fes_cmn_path *path,
	int8_t dataType,
	const char *value,
	const char *timeScale,
	int64_t timestamp
)
{
	irtdb_fes_soe_msg *soemsg = NULL;
	int len = 0, pathLableLen = 0;
	int32_t valueLen = 0, scaleLen = 0;

	if (!path || !value || !timeScale)
	{
		return NULL;
	}
	soemsg = calloc(1, sizeof(*soemsg));
	if (!soemsg)
	{
		return NULL;
	}
	soemsg->header = irtdb_fes_create_header();
	if (!soemsg->header)
	{
		free(soemsg);
		return NULL;
	}

	soemsg->header->msgType = FESMSG_UPLOAD_SOE;
	soemsg->header->dataType = dataType;
	len += IRTDB_FES_MSGHEADERLEN;
	soemsg->pathLabel = irtdb_fes_create_path_label();
	if (!soemsg->pathLabel)
	{
		irtdb_fes_free_soe_msg(soemsg);
		return NULL;
	}
	pathLableLen = irtdb_fes_init_path_label(soemsg->pathLabel, path);
	if (!pathLableLen)
	{
		irtdb_fes_free_soe_msg(soemsg);
		return NULL;
	}

	len += pathLableLen;
	valueLen = strlen(value);
	scaleLen = strlen(timeScale);
	soemsg->valueLen = valueLen;
	soemsg->value = strdup(value);
	if (!soemsg->value)
	{
		irtdb_fes_free_soe_msg(soemsg);
		return NULL;
	}

	soemsg->scaleLen = scaleLen;
	soemsg->timeScale = strdup(timeScale);
	if (!soemsg->timeScale)
	{
		irtdb_fes_free_soe_msg(soemsg);
		return NULL;
	}

	len += sizeof(soemsg->valueLen) + valueLen;
	len += sizeof(soemsg->scaleLen) + scaleLen;
	len += sizeof(timestamp);
	soemsg->timestamp = timestamp;
	soemsg->header->msgLen = len - sizeof(soemsg->header->msgLen);

	return soemsg;
}

unsigned char *irtdb_fes_dump_soe_msg(const irtdb_fes_soe_msg *msg, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;
	int len = 0;

	if (!msg || !msg->header || !dumpLen)
	{
		return NULL;
	}

	*dumpLen = 0;
	len = sizeof(msg->header->msgLen) + msg->header->msgLen;
	bytes = calloc(len, 1);
	if (!bytes)
	{
		return NULL;
	}

	p = bytes;
	if (-1 == irtdb_fes_encode_header(msg->header, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}
	if (-1 == irtdb_fes_encode_pathlabel(msg->pathLabel, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}

	irtdb_fes_encode_int32(msg->valueLen, p, *dumpLen);
	irtdb_fes_encode_bytes(msg->value, msg->valueLen, p, *dumpLen);
	irtdb_fes_encode_int32(msg->scaleLen, p, *dumpLen);
	irtdb_fes_encode_bytes(msg->timeScale, msg->scaleLen, p, *dumpLen);
	irtdb_fes_encode_int64(msg->timestamp, p, *dumpLen);

	return bytes;
}

void irtdb_fes_free_soe_msg(irtdb_fes_soe_msg *msg)
{
	if (msg)
	{
		irtdb_fes_free_header(msg->header);
		irtdb_fes_free_path_label(msg->pathLabel);
		if (msg->value)
		{
			free(msg->value);
		}
		if (msg->timeScale)
		{
			free(msg->timeScale);
		}
		free(msg);
	}
}

irtdb_fes_cmd *irtdb_fes_create_cmd()
{
	irtdb_fes_cmd *fescmd;
	fescmd = calloc(1, sizeof(*fescmd));
	return fescmd;
}

void irtdb_fes_free_cmd(irtdb_fes_cmd *fescmd)
{
	if (fescmd)
	{
		if (fescmd->header)
		{
			irtdb_fes_free_header(fescmd->header);
		}
		if (fescmd->value)
		{
			free(fescmd->value);
		}
		free(fescmd);
	}
}

int irtdb_fes_init_cmd(irtdb_fes_cmd *fescmd, const char *id, int vlen, const char *value)
{
	// 0 : ok ; -1 : fail
	int tlen = 0;

	if (!fescmd || !id || (vlen <= 0) || !value)
	{
		return -1;
	}
	if (strlen(value) < vlen)
	{
		return -1;
	}
	if (!fescmd->header)
	{
		fescmd->header = irtdb_fes_create_header();
		if (!fescmd->header)
		{
			return -1;
		}
	}
	if (strlen(id) < sizeof(fescmd->id))
	{
		return -1;
	}

	tlen += IRTDB_FES_MSGHEADERLEN;
	memcpy(fescmd->id, id, sizeof(fescmd->id));
	fescmd->vlen = vlen;
	if (fescmd->value)
	{
		free(fescmd->value);
	}

	fescmd->value = calloc(vlen, 1);
	if (!fescmd->value)
	{
		return -1;
	}

	memcpy(fescmd->value, value, vlen);
	tlen += sizeof(fescmd->id);
	tlen += sizeof(fescmd->vlen) + vlen;
	fescmd->header->msgType = IRTDB_FES_CMD;
	fescmd->header->msgLen = tlen - sizeof(fescmd->header->msgLen);

	return 0;
}

unsigned char *irtdb_fes_dump_cmd(irtdb_fes_cmd *fescmd, int *dumpLen)
{
	unsigned char *bytes = NULL, *p = NULL;

	if (!fescmd || !fescmd->header)
	{
		return NULL;
	}

	*dumpLen = 0;
	bytes = calloc(fescmd->header->msgLen, 1);
	if (!bytes)
	{
		return NULL;
	}

	p = bytes;
	if (-1 == irtdb_fes_encode_header(fescmd->header, dumpLen, &p))
	{
		free(bytes);
		return NULL;
	}

	irtdb_fes_encode_bytes(fescmd->id, sizeof(fescmd->id), p, *dumpLen);
	irtdb_fes_encode_int16(fescmd->vlen, p, *dumpLen);
	irtdb_fes_encode_bytes(fescmd->value, fescmd->vlen, p, *dumpLen);

	return bytes;
}

irtdb_fes_cmd *irtdb_fes_parse_cmd(unsigned char *bytes, int len)
{
	irtdb_fes_cmd *fescmd = NULL;
	unsigned char *p = bytes;
	int headerLen = IRTDB_FES_MSGHEADERLEN - 4;

	if (!bytes || (len < headerLen))
	{
		return NULL;
	}
	fescmd = irtdb_fes_create_cmd();
	if (!fescmd)
	{
		return NULL;
	}
	fescmd->header = irtdb_fes_parse_header(bytes, headerLen);
	if (!fescmd->header)
	{
		irtdb_fes_free_cmd(fescmd);
		return NULL;
	}

	p += headerLen;
	irtdb_fes_decode_bytes(fescmd->id, p, sizeof(fescmd->id));
	irtdb_fes_decode_int16(fescmd->vlen, p);
	fescmd->value = calloc(fescmd->vlen + 1, 1);
	if (!fescmd->value)
	{
		irtdb_fes_free_cmd(fescmd);
		return NULL;
	}
	irtdb_fes_decode_bytes(fescmd->value, p, fescmd->vlen);

	return fescmd;
}

/////////////////////////////////////////////////////
static int irtdb_fes_encode_header(irtdb_fes_header *header, int *dumpLen, unsigned char **p)
{
	unsigned char *headerBytes = NULL;
	int headerLen = 0;

	headerBytes = irtdb_fes_dump_header(header, &headerLen);
	if (!headerBytes)
	{
		return -1;
	}

	irtdb_fes_encode_bytes(headerBytes, headerLen, *p, *dumpLen);
	free(headerBytes);

	return 0;
}

static int irtdb_fes_encode_pathlabel(irtdb_fes_path_label *pathLabel, int *dumpLen, unsigned char **p)
{
	unsigned char *pathBytes = NULL;
	int pathLabelLen = 0;

	pathBytes = irtdb_fes_dump_path_label(pathLabel, &pathLabelLen);
	if (!pathBytes)
	{
		return -1;
	}

	irtdb_fes_encode_bytes(pathBytes, pathLabelLen, *p, *dumpLen);
	free(pathBytes);

	return 0;
}

static int irtdb_fes_encode_label(int32_t *dstLen, char **dstLabel, char *srcLabel, int *dataOutLen)
{
	int32_t srcLen = strlen(srcLabel);

	*dstLen = srcLen;
	*dstLabel = calloc(srcLen + 1, 1);
	if (!(*dstLabel))
	{
		return -1;
	}

	memcpy(*dstLabel, srcLabel, srcLen);
	*dataOutLen += sizeof(*dstLen) + *dstLen;

	return 0;
}

static int irtdb_fes_sock_read(int socket, void *bufOut, int bufLen)
{
	int code = 0, recvdCount = 0;
	int totalLen = 0;

	if (!bufOut)
	{
		return -1;
	}

	while (totalLen != bufLen)
	{
		recvdCount = recv(socket, bufOut + totalLen, bufLen - totalLen, MSG_DONTWAIT);
		CMN_LOGGER_DEBUG(logger, "recvdCount : %d", recvdCount);
		if (recvdCount == 0)
		{
			return -1;
		}
		else if (recvdCount < 0)
		{
			code = -1;
			CMN_LOGGER_DEBUG(logger, "recv() from peer error");
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				CMN_LOGGER_DEBUG(logger, "peer is not ready right now, try again later.");
				code = 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			totalLen += recvdCount;
		}
	}

	return code;
}

static int irtdb_fes_sock_write(int socket, irtdb_fes_message_bin *msgbin)
{
	int code = 0, send_count = 0;
	int totalLen = 0;

	if (!msgbin || !msgbin->data)
	{
		return -1;
	}

	while (totalLen != msgbin->length)
	{
		send_count = send(socket, msgbin->data + totalLen, msgbin->length - totalLen, 0);
		if (send_count == 0)
		{
			return -1;
		}
		else if (send_count < 0)
		{
			code = -1;
			CMN_LOGGER_DEBUG(logger, "send() from peer error");
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				CMN_LOGGER_DEBUG(logger, "peer is not ready right now, try again later.\n");
				code = 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			totalLen += send_count;
		}
	}

	return code;
}

static int irtdb_fes_cmn_path_add_nodekey(irtdb_fes_cmn_path *path, char *key)
{
	char **values = NULL;

	if (!path || !key)
		return -1;
	values = realloc(path->values, sizeof(char *) * (path->num + 1));
	if (!values)
	{
		CMN_LOGGER_DEBUG(logger, "irtdb_fes_cmn_path_add_nodekey : values is NULL");
		return -1;
	}
	values[path->num] = key;
	path->values = values;
	path->num++;
	return 0;
}
