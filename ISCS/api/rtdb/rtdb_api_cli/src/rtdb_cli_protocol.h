/*
 * @Copyright Reserved by XXXX.
 * rtdb cli protocol .
 * Create by ZhangGuanju, 2018.08.06.
 * Histories:
 */
#ifndef RTDB_CLI_PROTOCOL
#define RTDB_CLI_PROTOCOL
#ifdef __cplusplus
extern "C" {
#endif

#include "rtdb_cli_data.h"
#include <stdint.h>

#define RTDB_CLI_SUBTYPE_CMD	0
#define RTDB_CLI_SUBTYPE_TBL	1
#define RTDB_CLI_SUBTYPE_NODE	2

#define RTDB_CLI_REQTYPE_REQUEST  0  // 请求
#define RTDB_CLI_REQTYPE_RESPONSE 1  // 响应

#define RTDB_CLI_SENDER_CLI       0
#define RTDB_CLI_SENDER_RDB       1
#define RTDB_CLI_SENDER_FES       2
#define RTDB_CLI_SENDER_PLUGIN    3

#define RTDB_CLI_RECEIVER_CLI       0
#define RTDB_CLI_RECEIVER_RDB       1
#define RTDB_CLI_RECEIVER_FES       2
#define RTDB_CLI_RECEIVER_PLUGIN    3

#define RTDB_CLI_TYPE_HEARTBEAT		0x0000
#define RTDB_CLI_TYPE_QUERYTABLE	0x0100
#define RTDB_CLI_TYPE_UPDATETABLE	0x0101
#define RTDB_CLI_TYPE_DELTABLE		0x0102
#define RTDB_CLI_TYPE_QUERYNODE		0x0103
#define RTDB_CLI_TYPE_UPDATENODE	0x0104 
#define RTDB_CLI_TYPE_CHECKRTDB		0x0105
#define RTDB_CLI_TYPE_RDBCMD		0x0200
#define RTDB_CLI_TYPE_FESCMD		0x0201
#define RTDB_CLI_TYPE_RDBMSG		0x0202

#define RTDB_CLI_SUB_RDBCMD			0x0300
#define RTDB_CLI_SUB_RDBTBL			0x0301
#define RTDB_CLI_SUB_RDBNODE		0x0302

#define RTDB_CLI_ACT_SUB            1
#define RTDB_CLI_ACT_CANCELSUB      0

#define RTDB_CLI_PUB_RDBCMD		0x0400
#define RTDB_CLI_PUB_RDBTBL		0x0401
#define RTDB_CLI_PUB_RDBNODE	0x0402


#define RTDB_CLI_MSGHEADERLEN		20
#define RTDB_CLI_MD5LEN				16 
#define RTDB_CLI_RDBCMDBASELEN		106
#define RTDB_CLI_TBLSUBBASELEN		42 
#define RTDB_CLI_NODESUBBASELEN		42 

#define RTDB_CLI_UUIDLEN       36
#define RTDB_CLI_TBNAMELEN     32
#define RTDB_CLI_FIELDNAMELEN  64
#define RTDB_CLI_PATHLEN       128
#define RTDB_CLI_ATTRNMELEN    64

typedef unsigned char rtdb_cli_byte;
typedef char rtdb_cli_uuid[RTDB_CLI_UUIDLEN];
typedef char rtdb_cli_tbname[RTDB_CLI_TBNAMELEN];
typedef char rtdb_cli_sub_path[RTDB_CLI_PATHLEN];
typedef char rtdb_cli_attrname[RTDB_CLI_ATTRNMELEN];

#define RTDB_REPLY_TYPE_ERROR			-1
#define RTDB_CLI_REPLY_TYPE_VARIANT		1
#define RTDB_CLI_REPLY_TYPE_TABLE		2
#define RTDB_CLI_REPLY_TYPE_NODE		3


#define rtdb_cli_encode_bytes(src,p,srcLen,dumpLen) do {\
	memcpy(p, src, srcLen); \
	p += srcLen; \
	dumpLen += srcLen; \
}while(0)
    
#define rtdb_cli_decode_bytes(dst,p,dstLen) do {\
	memcpy(dst, p, dstLen); \
	p += dstLen; \
}while(0)

#define rtdb_cli_encode_base(type,label,base,src,p,dumpLen) do { \
    type tmp = rtdb_cli_hton##label##base(src); \
    rtdb_cli_encode_bytes(&tmp,p,base/8,dumpLen); \
} while(0)

#define rtdb_cli_decode_base(type,label,base,dst,p) do { \
    type tmp;\
    rtdb_cli_decode_bytes(&tmp,p,base/8);\
    dst = rtdb_cli_ntoh##label##base(tmp); \
} while(0)

#define rtdb_cli_encode_int8(src,p,dumpLen) rtdb_cli_encode_bytes(&src,p,1,dumpLen)
#define rtdb_cli_encode_int16(src,p,dumpLen) rtdb_cli_encode_base(uint16_t,i,16,src,p,dumpLen)
#define rtdb_cli_encode_int32(src,p,dumpLen) rtdb_cli_encode_base(uint32_t,i,32,src,p,dumpLen)
#define rtdb_cli_encode_int64(src,p,dumpLen) rtdb_cli_encode_base(uint64_t,i,64,src,p,dumpLen)
#define rtdb_cli_encode_float32(src,p,dumpLen) rtdb_cli_encode_base(float,f,32,src,p,dumpLen)
#define rtdb_cli_encode_float64(src,p,dumpLen) rtdb_cli_encode_base(double,f,64,src,p,dumpLen)

#define rtdb_cli_decode_int8(dst,p) rtdb_cli_decode_bytes(&dst,p,1)
#define rtdb_cli_decode_int16(dst,p) rtdb_cli_decode_base(uint16_t,i,16,dst,p) 
#define rtdb_cli_decode_int32(dst,p) rtdb_cli_decode_base(uint32_t,i,32,dst,p) 
#define rtdb_cli_decode_int64(dst,p) rtdb_cli_decode_base(uint64_t,i,64,dst,p)
#define rtdb_cli_decode_float32(dst,p) rtdb_cli_decode_base(float,f,32,dst,p)
#define rtdb_cli_decode_float64(dst,p) rtdb_cli_decode_base(double,f,64,dst,p)

#define rtdb_cli_assign_string(src,dst,dstSize,curLen) do {\
    snprintf(dst,dstSize,"%s",src);\
    curLen += dstSize;\
    dst += dstSize;\
}while(0)     


typedef struct {
	int32_t msgLen; // 报文的长度（不包括长度本身）
	int32_t domain; // 域id
	int16_t msgType; // 报文的类型
	int8_t reqType; // 请求类型：0 -- 请求，1 --响应。
	int16_t sender; // 发送者:0 -- CLI, 1 -- RDB, 2 -- FES, 3 -- PLUGIN
	int16_t receiver; // 接收者:0 -- CLI, 1 -- RDB, 2 -- FES, 3 -- PLUGIN
	int8_t version; // 协议版本号，默认为0，预留
	uint32_t crc32; // CRC32校验码，预留   
} rtdb_cli_msg_header;

rtdb_cli_msg_header* rtdb_cli_create_msg_header();
void rtdb_cli_free_msg_header(rtdb_cli_msg_header *header);
unsigned char* rtdb_cli_dump_msg_header(rtdb_cli_msg_header *header);
rtdb_cli_msg_header* rtdb_cli_decode_msg_header(const unsigned char *bytes,int len);
unsigned char* rtdb_cli_gen_header_bytes(int bytesLen,int domainId,int msgType);

/*
* RDB命令（单发）
*/
typedef struct {
    rtdb_cli_msg_header *header;
	int type;
    rtdb_cli_uuid id;
	char destination[64];
    short cmdLen;
	char *cmdJson;
    rtdb_cli_byte md5[RTDB_CLI_MD5LEN];
} rtdb_cli_msg_rdbcmd;

typedef struct {
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int32_t seqnum; // table and node
	int8_t num;
	int8_t *noArr;
	int8_t *resultArr;
} rtdb_cli_subrep;
rtdb_cli_subrep* rtdb_cli_create_subrep();
void rtdb_cli_free_subrep(rtdb_cli_subrep *subrep);
rtdb_cli_subrep* rtdb_cli_parse_subrep(const rtdb_cli_byte *bytes, int len);


/*
* RDB订阅命令结果
*/
typedef struct {
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int8_t action;
	int8_t num;
	int8_t *noArr;
	rtdb_cli_uuid *cmdidArr; // uuid
    
    rtdb_cli_byte md5[RTDB_CLI_MD5LEN];
}rtdb_cli_cmdsub;

rtdb_cli_cmdsub* rtdb_cli_create_cmdsub(int domain);
void rtdb_cli_free_cmdsub(rtdb_cli_cmdsub *cmdsub);
rtdb_cli_byte* rtdb_cli_dump_cmdsub(rtdb_cli_cmdsub *cmdsub, int *dumpLen);

/*
* RDB命令通知
*/
struct _rtdb_cli_cmd_notifier
{
    rtdb_cli_msg_rdbcmd *rdbcmd;
	rtdb_cli_cmdResult_notifier *resultNotifier;
	const void *userData;
};

/*
* RDB订阅表模型
*/
typedef struct {
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int32_t seqnum;
	int8_t action;
	int8_t num;

	int8_t *noArr;
	rtdb_cli_tbname *nameArr;
	rtdb_cli_byte md5[RTDB_CLI_MD5LEN];
	int status;
} rtdb_cli_tblsub;
rtdb_cli_tblsub* rtdb_cli_create_tblsub(int domain,const char *uuid, int seqnum, int8_t action, int subNum);
void rtdb_cli_free_tblsub(rtdb_cli_tblsub *sub);
rtdb_cli_byte* rtdb_cli_dump_tblsub(rtdb_cli_tblsub *sub, int *dumpLen);

/*
* RDB订阅点模型
*/
typedef struct {
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int32_t seqnum;
	int8_t action;
	int8_t nNum;

	int8_t *noArr;
	rtdb_cli_sub_path *path;

	int8_t aNum;
	rtdb_cli_attrname *nameArr;
    
    rtdb_cli_byte md5[RTDB_CLI_MD5LEN];
} rtdb_cli_nodesub;
rtdb_cli_nodesub* rtdb_cli_create_nodesub(int domain,rtdb_cli_uuid uuid, const rtdb_cli_list *names, int8_t action, int subNum);
void rtdb_cli_free_nodesub(rtdb_cli_nodesub *sub);
rtdb_cli_byte* rtdb_cli_dump_nodesub(const rtdb_cli_nodesub *sub,int *dumpLen);

typedef struct {
	rtdb_cli_uuid cmdid;
	int8_t code;
	int16_t resultLen;
	char *result;
} rtdb_cli_cmdpub_item;
rtdb_cli_cmdpub_item* rtdb_cli_create_cmdpub_item();
void rtdb_cli_free_cmdpub_item(rtdb_cli_cmdpub_item *item);

/*
* 命令结果发布
*/
typedef struct {
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int8_t num;
	rtdb_cli_cmdpub_item *items;
} rtdb_cli_cmdpub;
rtdb_cli_cmdpub* rtdb_cli_create_cmdpub();
void rtdb_cli_free_cmdpub(rtdb_cli_cmdpub *cmdpub);
rtdb_cli_cmdpub* rtdb_cli_parse_cmdpub(const rtdb_cli_byte *bytes, int len);

typedef struct {
	char aName[64];
	int8_t aType;
	int16_t avLen;
	rtdb_cli_variant *vt;
} rtdb_cli_attr_item;
rtdb_cli_attr_item* rtdb_cli_create_attr_item();
void rtdb_cli_free_attr_item(rtdb_cli_attr_item *item);
rtdb_cli_attr_item* rtdb_cli_attr_item_dup(rtdb_cli_attr_item* attrItem);

typedef struct {
	uint8_t aNum;
	rtdb_cli_attr_item **attrItems;
} rtdb_cli_node_item;

rtdb_cli_node_item* rtdb_cli_create_node_item(int aNum);
void rtdb_cli_free_node_item(rtdb_cli_node_item *item);
rtdb_cli_node_item* rtdb_cli_node_item_dup(rtdb_cli_node_item *nodeItem);
rtdb_cli_node_item* rtdb_cli_parse_node_item(unsigned char **p);

/*
* 表模型发布
*/
typedef struct {
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	rtdb_cli_tbname tName;
	int8_t type;
	int8_t rNum;
	rtdb_cli_node_item **nodeItem;
} rtdb_cli_tblpub ;
rtdb_cli_tblpub* rtdb_cli_create_tblpub(int8_t rNum);
void rtdb_cli_free_tblpub(rtdb_cli_tblpub *tblpub);
rtdb_cli_tblpub* rtdb_cli_parse_tblpub(const rtdb_cli_byte *bytes, int len);


/*
* 点模型发布
*/
typedef struct {
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	rtdb_cli_sub_path path;
	rtdb_cli_node_item *nodeItem;
} rtdb_cli_nodepub;
rtdb_cli_nodepub* rtdb_cli_create_nodepub();
void rtdb_cli_free_nodepub(rtdb_cli_nodepub *nodepub);
rtdb_cli_nodepub* rtdb_cli_parse_nodepub(const rtdb_cli_byte *bytes, int len);

rtdb_cli_msg_rdbcmd* rtdb_cli_create_msg_rdbcmd(
    int domainId, 
	int type,
    const char* uuid,
	const char* destination, 
	const char *cmdJson
);
void rtdb_cli_free_msg_rdbcmd(rtdb_cli_msg_rdbcmd *rdbcmd);
rtdb_cli_byte* rtdb_cli_dump_msg_rdbcmd(rtdb_cli_msg_rdbcmd *rdbcmd,int *dumpLen);


typedef struct {
	int type;  // -1 : error ; 1: variant ; 2 : table ; 3 : node 
	int elenum;
	int32_t *arrRowid;
	rtdb_cli_variant **vtItems;
	rtdb_cli_node_item **nodeItems; // node or table 
} rtdb_cli_reply;

rtdb_cli_reply* rtdb_cli_create_reply();
void rtdb_cli_free_reply(rtdb_cli_reply *reply);
rtdb_cli_reply* rtdb_cli_prase_reply(const unsigned char *bytes);



#ifdef __cplusplus
}
#endif
#endif
