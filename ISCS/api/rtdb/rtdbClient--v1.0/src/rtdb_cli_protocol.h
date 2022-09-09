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

#define RTDB_CLI_TYPE_RDBCMD     0x0200
#define RTDB_CLI_TYPE_FESCMD     0x0201
#define RTDB_CLI_TYPE_RDBMSG     0x0202

#define RTDB_CLI_SUB_RDBCMD      0x0300
#define RTDB_CLI_SUB_RDBTBL      0x0301
#define RTDB_CLI_SUB_RDBNODE     0x0302

#define RTDB_CLI_ACT_SUB            1
#define RTDB_CLI_ACT_CANCELSUB      0

#define RTDB_CLI_PUB_RDBCMD		0x0400
#define RTDB_CLI_PUB_RDBTBL		0x0401
#define RTDB_CLI_PUB_RDBNODE	0x0402


#define RTDB_CLI_MSGHEADERLEN  16 
#define RTDB_CLI_RDBCMDBASELEN 110
#define RTDB_CLI_TBLSUBBASELEN  42 
#define RTDB_CLI_NODESUBBASELEN  38 

#define RTDB_CLI_UUIDLEN       36
#define RTDB_CLI_TBNAMELEN     32
#define RTDB_CLI_FIELDNAMELEN  64
#define RTDB_CLI_PATHLEN       128
#define RTDB_CLI_ATTRNMELEN    64

typedef unsigned char rtdb_cli_byte;
typedef char rtdb_cli_uuid[RTDB_CLI_UUIDLEN];
typedef char rtdb_cli_tbname[RTDB_CLI_TBNAMELEN];
typedef char rtdb_cli_fieldname[RTDB_CLI_FIELDNAMELEN];
typedef char rtdb_cli_sub_path[RTDB_CLI_PATHLEN];
typedef char rtdb_cli_attrname[RTDB_CLI_ATTRNMELEN];

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
#define rtdb_cli_encode_int16(src,p,dumpLen) rtdb_cli_encode_base(int16_t,i,16,src,p,dumpLen)
#define rtdb_cli_encode_int32(src,p,dumpLen) rtdb_cli_encode_base(int32_t,i,32,src,p,dumpLen)
#define rtdb_cli_encode_int64(src,p,dumpLen) rtdb_cli_encode_base(int64_t,i,64,src,p,dumpLen)
#define rtdb_cli_encode_float32(src,p,dumpLen) rtdb_cli_encode_base(float,f,32,src,p,dumpLen)
#define rtdb_cli_encode_float64(src,p,dumpLen) rtdb_cli_encode_base(double,f,64,src,p,dumpLen)

#define rtdb_cli_decode_int8(dst,p) rtdb_cli_decode_bytes(&dst,p,1)
#define rtdb_cli_decode_int16(dst,p) rtdb_cli_decode_base(int16_t,i,16,dst,p) 
#define rtdb_cli_decode_int32(dst,p) rtdb_cli_decode_base(int32_t,i,32,dst,p) 
#define rtdb_cli_decode_int64(dst,p) rtdb_cli_decode_base(int64_t,i,64,dst,p)
#define rtdb_cli_decode_float32(dst,p) rtdb_cli_decode_base(float,f,32,dst,p)
#define rtdb_cli_decode_float64(dst,p) rtdb_cli_decode_base(double,f,64,dst,p)

#define rtdb_cli_assign_string(src,dst,dstSize,curLen) do {\
    snprintf(dst,dstSize,"%s",src);\
    curLen += dstSize;\
    dst += dstSize;\
}while(0)     


struct _rtdb_cli_msg_header
{
	int32_t msgLen; // 报文的长度（不包括长度本身）
	int16_t msgType; // 报文的类型
	int8_t reqType; // 请求类型：0 -- 请求，1 --响应。
	int16_t sender; // 发送者:0 -- CLI, 1 -- RDB, 2 -- FES, 3 -- PLUGIN
	int16_t receiver; // 接收者:0 -- CLI, 1 -- RDB, 2 -- FES, 3 -- PLUGIN
	int8_t version; // 协议版本号，默认为0，预留
	uint32_t crc32; // CRC32校验码，预留   
};
typedef struct _rtdb_cli_msg_header rtdb_cli_msg_header;

rtdb_cli_msg_header* rtdb_cli_create_msg_header();
void rtdb_cli_free_msg_header(rtdb_cli_msg_header *header);
rtdb_cli_byte* rtdb_cli_dump_msg_header(rtdb_cli_msg_header *header);
rtdb_cli_msg_header* rtdb_cli_decode_msg_header(const rtdb_cli_byte *bytes,int len);


/*
* RDB命令（单发）
*/
struct _rtdb_cli_msg_rdbcmd
{
    rtdb_cli_msg_header *header;
    int domainId; 
	int type;
    rtdb_cli_uuid id;
	char destination[64];
    short cmdLen;
	char *cmdJson;
}; 
typedef struct _rtdb_cli_msg_rdbcmd rtdb_cli_msg_rdbcmd;

struct _rtdb_cli_subrep
{
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int8_t num;
	int8_t *noArr;
	int8_t *resultArr;
};
typedef struct _rtdb_cli_subrep rtdb_cli_subrep;
rtdb_cli_subrep* rtdb_cli_create_subrep();
void rtdb_cli_free_subrep(rtdb_cli_subrep *subrep);
rtdb_cli_subrep* rtdb_cli_parse_subrep(const rtdb_cli_byte *bytes, int len);


/*
* RDB订阅命令结果
*/
typedef struct
{
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int8_t action;
	int8_t num;
	int8_t *noArr;
	rtdb_cli_uuid *cmdidArr; // uuid
}rtdb_cli_cmdsub;

rtdb_cli_cmdsub* rtdb_cli_create_cmdsub();
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
struct _rtdb_cli_tblsub
{
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int8_t action;
	int32_t domain;
	int8_t num;
	int8_t *noArr;
	rtdb_cli_tbname *nameArr;
};
typedef struct _rtdb_cli_tblsub rtdb_cli_tblsub;
rtdb_cli_tblsub* rtdb_cli_create_tblsub();
void rtdb_cli_free_tblsub(rtdb_cli_tblsub *sub);
rtdb_cli_byte* rtdb_cli_dump_tblsub(rtdb_cli_tblsub *sub, int *dumpLen);

/*
* RDB订阅点模型
*/
struct _rtdb_cli_nodesub
{
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int8_t action;
	int8_t nNum;

	int8_t *noArr;
	rtdb_cli_sub_path *path;

	int8_t aNum;
	rtdb_cli_attrname *nameArr;
};
typedef struct _rtdb_cli_nodesub rtdb_cli_nodesub;
rtdb_cli_nodesub* rtdb_cli_create_nodesub();
void rtdb_cli_free_nodesub(rtdb_cli_nodesub *sub);
rtdb_cli_byte* rtdb_cli_dump_nodesub(const rtdb_cli_nodesub *sub,int *dumpLen);

typedef struct
{
	rtdb_cli_uuid cmdid;
	int8_t code;
	int16_t resultLen;
	char *result;
}rtdb_cli_cmdpub_item;
rtdb_cli_cmdpub_item* rtdb_cli_create_cmdpub_item();
void rtdb_cli_free_cmdpub_item(rtdb_cli_cmdpub_item *item);

/*
* 命令结果发布
*/
struct _rtdb_cli_cmdpub
{
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	int8_t num;
	rtdb_cli_cmdpub_item *items;
} ;
typedef struct _rtdb_cli_cmdpub rtdb_cli_cmdpub;
rtdb_cli_cmdpub* rtdb_cli_create_cmdpub();
void rtdb_cli_free_cmdpub(rtdb_cli_cmdpub *cmdpub);
rtdb_cli_cmdpub* rtdb_cli_parse_cmdpub(const rtdb_cli_byte *bytes, int len);

struct _rtdb_cli_tblpub_item
{
	rtdb_cli_fieldname fName;
	int8_t fType;
	int16_t fvLen;
	rtdb_cli_variant *vt;
};
typedef struct _rtdb_cli_tblpub_item rtdb_cli_tblpub_item;
rtdb_cli_tblpub_item* rtdb_cli_create_tblpub_item();
void rtdb_cli_free_tblpub_item(rtdb_cli_tblpub_item *item);
/*
* 表模型发布
*/
struct _rtdb_cli_tblpub
{
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	rtdb_cli_tbname tName;
	int8_t type;
	int8_t rNum;
	int8_t fNum;
	rtdb_cli_tblpub_item *items;
};
typedef struct _rtdb_cli_tblpub rtdb_cli_tblpub;
rtdb_cli_tblpub* rtdb_cli_create_tblpub();
void rtdb_cli_free_tblpub(rtdb_cli_tblpub *tblpub);
rtdb_cli_tblpub* rtdb_cli_parse_tblpub(const rtdb_cli_byte *bytes, int len);

struct _rtdb_cli_nodepub_item
{
	char aName[64];
	int8_t aType;
	int16_t avLen;
	rtdb_cli_variant *vt;
};
typedef struct _rtdb_cli_nodepub_item rtdb_cli_nodepub_item;
rtdb_cli_nodepub_item* rtdb_cli_create_nodepub_item();
void rtdb_cli_free_nodepub_item(rtdb_cli_nodepub_item *item);

/*
* 点模型发布
*/
struct _rtdb_cli_nodepub
{
	rtdb_cli_msg_header *header;
	rtdb_cli_uuid id;
	rtdb_cli_sub_path path;
	int8_t aNum;
	rtdb_cli_nodepub_item *items;
};
typedef struct _rtdb_cli_nodepub rtdb_cli_nodepub;
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

#ifdef __cplusplus
}
#endif
#endif
