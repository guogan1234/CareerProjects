/*
 @Copyright Reserved by XXXX.
 RTDB client network operations(TCP socket).
 Create by KanWenDi, 2018.04.03.
 Histories:

 */
#ifndef RTDB_CLI_NET_H
#define RTDB_CLI_NET_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <winsock2.h>
#define network_read(fd,buf,len)     recv((fd),(buf),(len),0)
#define network_write(fd,buf,len)    send((fd),(buf),(len),0)
#else
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#define network_read(fd,buf,len)     read((fd),(buf),(len))
#define network_write(fd,buf,len)    write((fd),(buf),(len))
#endif
#include <stdint.h>

#define NETWORK_ERROR   -1
#define NETWORK_OK      0

#define NETWORK_BLOCK   0
#define NETWORK_NOBLOCK 1

#define NETWORK_CONNECT_NONE    0x00000000
#define NETWORK_CONNECT_NOBLOCK 0x00000001
#define NETWORK_CONNECT_NODELAY 0x00000002

typedef struct rtdb_cli_net rtdb_cli_net;

typedef enum {
    CLIENT_INVALID = -1,
    CLIENT_OK,
    CLIENT_ERROR,
    CLIENT_NULLPTR,
    CLIENT_DISCONN,
    CLIENT_IO_ERROR,
    CLIENT_IO_SETOPT_ERROR,
    CLIENT_IO_GETOPT_ERROR,
    CLIENT_IO_SETCTL_ERROR,
    CLIENT_IO_GETCTL_ERROR,
    CLIENT_IO_INCOMPLETEDATA
} rtdb_cli_net_code;

rtdb_cli_net* rtdb_cli_net_create();
rtdb_cli_net* rtdb_cli_net_dup(rtdb_cli_net *val);
void rtdb_cli_net_free(rtdb_cli_net *val);
void rtdb_cli_net_init();
void rtdb_cli_net_uninit();
void rtdb_cli_net_close(int fd);
rtdb_cli_net_code rtdb_cli_net_setblock(int fd, int noblock);
rtdb_cli_net_code rtdb_cli_net_setnodelay(int fd, int nodelay);
rtdb_cli_net_code rtdb_cli_net_setreuseaddr(int fd);
rtdb_cli_net_code rtdb_cli_net_settcpconn(int*fd, const char* address, int port, const char* sourceAddress);

int16_t rtdb_cli_htoni16(int16_t value);
int32_t rtdb_cli_htoni32(int32_t value);
int64_t rtdb_cli_htoni64(int64_t value);
float    rtdb_cli_htonf32(float value);
double   rtdb_cli_htonf64(double value);
int16_t rtdb_cli_ntohi16(int16_t value);
int32_t rtdb_cli_ntohi32(int32_t value);
int64_t rtdb_cli_ntohi64(int64_t value);
float    rtdb_cli_ntohf32(float value);
double   rtdb_cli_ntohf64(double value);

#ifdef __cplusplus
}
#endif
#endif
