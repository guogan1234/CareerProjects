#ifndef IRTDB_FES_PRIVATEDATA_H
#define IRTDB_FES_PRIVATEDATA_H 
#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>

#ifdef WIN32 
#include <winSock2.h>
#define MSG_DONTWAIT 0
#include <Ws2tcpip.h>
#else
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdint.h>
#include <string.h>
#include "cmn_list.h"
    
struct irtdb_fes_dbserver {
    char *ip1;
    char *ip2; 
    char *activeIp;
    int port;
    char *username;
    char *password;
    int isAvailable;
};
typedef struct irtdb_fes_dbserver irtdb_fes_dbserver;

irtdb_fes_dbserver* irtdb_fes_create_dbserver(
    const char *ip1,const char *ip2,
    int port,const char *username,const char *password
);
void irtdb_fes_free_dbserver(irtdb_fes_dbserver *pServer);

typedef struct irtdb_fes_message_bin {
    unsigned char *data;
    int32_t length;
} irtdb_fes_message_bin;

irtdb_fes_message_bin* irtdb_fes_create_message_bin(
    const unsigned char *data,
    int dataLen
);
void irtdb_fes_delete_message_bin(irtdb_fes_message_bin* pMsgBin);
irtdb_fes_message_bin* irtdb_fes_dup_message_bin(
    const irtdb_fes_message_bin* pMsgBin
);

struct irtdb_fes_peer {
    int socket;
    struct sockaddr_in addres;
    cmn_list *send_buffer;
    irtdb_fes_message_bin *recvBuffer;   
};
typedef struct irtdb_fes_peer irtdb_fes_peer;
void irtdb_fes_delete_peer(irtdb_fes_peer *peer);
irtdb_fes_peer* irtdb_fes_create_peer();
int irtdb_fes_peer_add_to_send(irtdb_fes_peer *peer,const irtdb_fes_message_bin *msg);
int irtdb_fes_recv_from_peer(irtdb_fes_peer *peer, int (*message_handler)(irtdb_fes_message_bin *));
int irtdb_fes_send_to_peer(irtdb_fes_peer *peer);

struct irtdb_fes_dbnode {
    irtdb_fes_dbserver *pRtdbInfo;
    irtdb_fes_peer *peer;
    struct irtdb_fes_dbnode *next;
};
typedef struct irtdb_fes_dbnode irtdb_fes_dbnode;
irtdb_fes_dbnode* irtdb_fes_create_dbnode(irtdb_fes_dbserver *pServer);
void irtdb_fes_free_dbnode(irtdb_fes_dbnode* pNode);

struct irtdb_fes_header {
    int32_t msgLen; // 4 Bytes
    int16_t msgType; // 2 Bytes
    int8_t dataType; // 1 Byte
    int8_t version; // 1 Byte
    int32_t crc32; // 4 Bytes 
};
typedef struct irtdb_fes_header irtdb_fes_header;

irtdb_fes_header* irtdb_fes_create_header();
void irtdb_fes_show_header(irtdb_fes_header *header);
unsigned char* irtdb_fes_dump_header(irtdb_fes_header *header,int *dumLen);
irtdb_fes_header* irtdb_fes_parse_header(unsigned char *pDataIn,int dataLen);
int irtdb_fes_free_header(irtdb_fes_header* header);

struct irtdb_fes_fesid {
    irtdb_fes_header *header;
    int32_t len;
    char *value; 
};
typedef struct irtdb_fes_fesid irtdb_fes_fesid;

irtdb_fes_fesid* irtdb_fes_create_fesid(const char* id);
void irtdb_fes_free_fesid(irtdb_fes_fesid *fesid);
void irtdb_fes_show_fesid(irtdb_fes_fesid *pFesId);
unsigned char* irtdb_fes_dump_fesid(irtdb_fes_fesid *pFesId,int *dumpLen);
irtdb_fes_fesid* irtdb_fes_parse_fesid(const unsigned char* pDataIn,int dataLen);

struct irtdb_fes_cmn_path {
    int num;
    char **values;
};
typedef struct irtdb_fes_cmn_path irtdb_fes_cmn_path ;
irtdb_fes_cmn_path* irtdb_fes_create_cmn_path();
void irtdb_fes_free_cmn_path(irtdb_fes_cmn_path* path);
irtdb_fes_cmn_path* irtdb_fes_parse_cmn_path(const char* spath);

struct irtdb_fes_path_label {
    // domain_label:station_label:pro_system_label:dev_label:data_label
    int32_t domainLabelLen;
    char *domainLabel;
    int32_t stationLabelLen;
    char *stationLabel;
    int32_t systemLabelLen;
    char *systemLabel;    
    int32_t devLabelLen;
    char *devLabel;
    
    int32_t dataLabelLen; /* di ai acc */
    char *dataLabel; 
};
typedef struct irtdb_fes_path_label irtdb_fes_path_label;

irtdb_fes_path_label* irtdb_fes_create_path_label();
void irtdb_fes_free_path_label(irtdb_fes_path_label* pathLabel);
int irtdb_fes_init_path_label(irtdb_fes_path_label *pathLabel,irtdb_fes_cmn_path *path);
int irtdb_fes_show_path_label(irtdb_fes_path_label *pathLabel);
unsigned char* irtdb_fes_dump_path_label(const irtdb_fes_path_label *pathLabel,int *dumpLen);
irtdb_fes_path_label* irtdb_fes_parse_path_label(unsigned char* p,int len);

struct irtdb_fes_di_msg {
    irtdb_fes_header *header; 
    // domain_label:station_label:pro_system_label:dev_label:di_label     
    irtdb_fes_path_label *pathLabel;
    
    int32_t valueLabelLen;
    char *valueLabel; 
    int32_t value;
    int64_t timestamp;
};
typedef struct irtdb_fes_di_msg irtdb_fes_di_msg;

irtdb_fes_di_msg* irtdb_fes_create_di_msg(
    irtdb_fes_cmn_path *path,uint8_t dataType,
    char *valueLabel, int32_t value,uint64_t timestamp);
int irtdb_fes_show_di_msg(irtdb_fes_di_msg *dimsg);
unsigned char* irtdb_fes_dump_di_msg(const irtdb_fes_di_msg *dimsg,int *dumpLen);
void irtdb_fes_free_di_msg(irtdb_fes_di_msg *dimsg);

struct irtdb_fes_ai_msg {
    irtdb_fes_header *header;
    // domain_label:station_label:pro_system_label:dev_label:ai_label  
    irtdb_fes_path_label *pathLabel;
    
    float value;
    int64_t timestamp;   
};
typedef struct irtdb_fes_ai_msg irtdb_fes_ai_msg;

irtdb_fes_ai_msg* irtdb_fes_create_ai_msg(
    irtdb_fes_cmn_path *path,int8_t dataType,
    float value,int64_t timestamp);
int irtdb_fes_show_ai_msg(irtdb_fes_ai_msg *aimsg);
unsigned char* irtdb_fes_dump_ai_msg(const irtdb_fes_ai_msg *aimsg,int *dumpLen);
void irtdb_fes_free_ai_msg(irtdb_fes_ai_msg *aimsg);

struct irtdb_fes_acc_msg {
    irtdb_fes_header *header;
    // domain_label:station_label:pro_system_label:dev_label:acc_label
    irtdb_fes_path_label *pathLabel;
    
    int32_t value;
    int64_t timestamp;   
};
typedef struct irtdb_fes_acc_msg irtdb_fes_acc_msg;

irtdb_fes_acc_msg* irtdb_fes_create_acc_msg(
    irtdb_fes_cmn_path *path,int8_t dataType,int value, 
    int64_t timestamp);
int irtdb_fes_show_acc_msg(irtdb_fes_acc_msg *accmsg);
unsigned char* irtdb_fes_dump_acc_msg(const irtdb_fes_acc_msg *accmsg,int *dumpLen);
void irtdb_fes_free_acc_msg(irtdb_fes_acc_msg *accmsg);

/*
fes command and response
*/
struct _irtdb_fes_cmd {
	irtdb_fes_header *header;
	char id[36];
	int16_t vlen; /* cmdLen or resultLen */
	char *value; /* cmd or result */
};
typedef struct _irtdb_fes_cmd irtdb_fes_cmd;
irtdb_fes_cmd* irtdb_fes_create_cmd();
void irtdb_fes_free_cmd(irtdb_fes_cmd *fescmd);
int irtdb_fes_init_cmd(irtdb_fes_cmd *fescmd, const char *id, int vlen, const char* value);
unsigned char* irtdb_fes_dump_cmd(irtdb_fes_cmd *cmd,int *dumpLen);
irtdb_fes_cmd* irtdb_fes_parse_cmd(unsigned char* bytes,int len);

#ifdef __cplusplus
}
#endif
#endif



