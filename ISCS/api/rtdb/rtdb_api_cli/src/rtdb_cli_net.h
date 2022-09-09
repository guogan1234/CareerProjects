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


uint16_t rtdb_cli_htoni16(uint16_t value);
uint32_t rtdb_cli_htoni32(uint32_t value);
uint64_t rtdb_cli_htoni64(uint64_t value);
float    rtdb_cli_htonf32(float value);
double   rtdb_cli_htonf64(double value);
uint16_t rtdb_cli_ntohi16(uint16_t value);
uint32_t rtdb_cli_ntohi32(uint32_t value);
uint64_t rtdb_cli_ntohi64(uint64_t value);
float    rtdb_cli_ntohf32(float value);
double   rtdb_cli_ntohf64(double value);

int rtdb_cli_net_read(int fd, char *buf, int count);
int rtdb_cli_net_write(int fd, char *buf, int count);
int rtdb_cli_net_check_tcp(const char* ip, int port,char *bytes,int bytesLen, int timeout);
int rtdb_cli_net_cnn_rtdb(const char *ip, int port);
int rtdb_cli_net_getfd(const char *active_ip, int modePort, int *fdOut);

#ifdef __cplusplus
}
#endif
#endif
