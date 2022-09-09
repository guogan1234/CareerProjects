#ifndef IRTDB_FES_PRIVATECOMMON_H
#define IRTDB_FES_PRIVATECOMMON_H 
#ifdef __cplusplus
extern "C" 
{
#endif

#include <stdio.h>
#include <errno.h>

#ifdef WIN32 
#include <winSock2.h>
#define MSG_DONTWAIT 0
#include <Ws2tcpip.h>
#else
#include <sys/select.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define DEBUG 0

#define bool    _Bool  
#define true    1  
#define false   0  

#if DEBUG
    #define irtdb_fes_trace(...) do { \
        printf("[%d %s:%d(%s)]:",time(NULL),__FILE__,__LINE__,__func__);\
        printf(__VA_ARGS__);\
    } while(0)
#else
    #define irtdb_fes_trace(...) ((void)0)
#endif

#define MAX_SEND_SIZE 1024
#define MAX_BUFFER_SIZE 1024

#define SENDER_MAXSIZE 128
#define DATA_MAXSIZE 1024

#define IRTDB_FES_MSGHEADERLEN			12

#define FESMSG_UPLOAD                   0
#define FESMSG_TIMED_UPLOAD             1
#define FESMSG_CHANGED_UPLOAD           2

/* 消息上报类型（内部使用） */
#define FESMSG_FESID                  1  
#define FESMSG_UPLOAD_DI              2
#define FESMSG_UPLOAD_AI              3
#define FESMSG_UPLOAD_ACC             4
#define FESMSG_UPLOAD_SOE             5
#define FESMSG_UPLOAD_ATSPOS          6
#define FESMSG_UPLOAD_ATSTT           7
#define FESMSG_UPLOAD_TIMETABLE       8

#define IRTDB_FES_CMD                 0x201

#define irtdb_fes_free_pointer(pVar) do {\
    if(pVar != NULL){ \
        free(pVar); \
        pVar=NULL; \
    }\
} while(0)

int16_t irtdb_fes_htoni16(int16_t value);
int32_t irtdb_fes_htoni32(int32_t value);
int64_t irtdb_fes_htoni64(int64_t value);
float    irtdb_fes_htonf32(float value);
double   irtdb_fes_htonf64(double value);
int16_t irtdb_fes_ntohi16(int16_t value);
int32_t irtdb_fes_ntohi32(int32_t value);
int64_t irtdb_fes_ntohi64(int64_t value);
float    irtdb_fes_ntohf32(float value);
double   irtdb_fes_ntohf64(double value);
int irtdb_fes_check_tcp_port(const char* ip,int port,int timeout);

#ifdef __cplusplus
}
#endif
#endif
