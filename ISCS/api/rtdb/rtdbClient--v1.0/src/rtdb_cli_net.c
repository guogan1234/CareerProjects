/*
 @Copyright Reserved by XXXX.
 RTDB client network operations(TCP socket).
 Create by KanWenDi, 2018.04.03.
 Histories:

 */ 
#include "rtdb_cli_fmacros.h"
#include "rtdb_cli_cmn.h"
#include "rtdb_cli_net.h"
#include <errno.h>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include <stddef.h>
#include <ws2tcpip.h>
#include <winerror.h>
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <netdb.h>
#endif

#ifdef _WIN32
#pragma comment (lib, "Ws2_32.lib")
static int networkInintialized = 0;
#endif


struct rtdb_cli_net
{
    int fd;
};

rtdb_cli_net* rtdb_cli_net_create()
{
    rtdb_cli_net *net = NULL;
    net = malloc(sizeof(rtdb_cli_net));
    if(net)
    {
        net->fd = 0;
    }
    return net;
}

rtdb_cli_net* rtdb_cli_net_dup(rtdb_cli_net *val)
{
    rtdb_cli_net *net = NULL;
    net = malloc(sizeof(rtdb_cli_net));
    if(net)
    {
        net->fd = val->fd;
    }
    return net;
}

void rtdb_cli_net_free(rtdb_cli_net *val)
{
    if(val)
    {
        free(val);
        val = NULL;
    }
}


void rtdb_cli_net_init()
{
#ifdef _WIN32
    if (0 == networkInintialized) {
        WSADATA wsdata;
        if (0 == WSAStartup(MAKEWORD(2,2), &wsdata)) {
            networkInintialized = 1;
        }
    }
#endif
}

void rtdb_cli_net_uninit()
{
#ifdef _WIN32
    if (1 == networkInintialized) {
        WSACleanup();
        networkInintialized = 0;
    }
#endif
}

void rtdb_cli_net_close(int fd)
{
    if (NETWORK_ERROR == fd) return;
#ifdef _WIN32
    closesocket(fd);
#else
    close(fd);
#endif
}

rtdb_cli_net_code rtdb_cli_net_setblock(int fd, int noblock) {
    int status;

#ifdef _WIN32
    status = ioctlsocket(fd, FIONBIO, (unsigned long*)&noblock);
    if (0 != status) return CLIENT_IO_SETCTL_ERROR;
#else
    status = fcntl(fd, F_GETFL);
    if (-1 == status) return CLIENT_IO_GETCTL_ERROR;
    if (noblock) status |= O_NONBLOCK;
    else status &= ~O_NONBLOCK;
    status = fcntl(fd, F_SETFL, status);
    if (-1 == status) return CLIENT_IO_SETCTL_ERROR;
#endif

    return CLIENT_OK;
}

rtdb_cli_net_code rtdb_cli_net_setnodelay(int fd, int nodelay) 
{
    int status;

    status = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const void*)&nodelay, sizeof(nodelay));
    if (0 != status) return CLIENT_IO_SETOPT_ERROR;
    return CLIENT_OK;
}

rtdb_cli_net_code rtdb_cli_net_setreuseaddr(int fd) 
{
    int status;
    int yes = 1;

    status = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));
    if (0 != status) return CLIENT_IO_SETOPT_ERROR;
    return CLIENT_OK;
}

rtdb_cli_net_code rtdb_cli_net_settcpconn(int*fd, const char* ip, int port, const char* sourceAddress)
{
    int sockfd;
    int status;
    struct sockaddr_in severAddr;
    rtdb_cli_net_code code = CLIENT_OK;

    *fd = NETWORK_ERROR;
    memset(&severAddr, 0, sizeof(severAddr));
    severAddr.sin_family = AF_INET;
    severAddr.sin_port = htons(port);
    severAddr.sin_addr.s_addr = inet_addr(ip);
    
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
        return CLIENT_INVALID;

    status = connect(sockfd,&severAddr, sizeof(severAddr));    
    if (0 != status) 
    {
        rtdb_cli_net_close(sockfd);
        return CLIENT_INVALID;
    }        
    *fd = sockfd;
    return code;
}

int16_t rtdb_cli_htoni16(int16_t value) 
{
    char bytes[2];
    
    bytes[0] = (value >> 8) & 0xff;
    bytes[1] = (value >> 0) & 0xff;
    memcpy(&value, bytes, 2);
    return value;
}

int32_t rtdb_cli_htoni32(int32_t value) 
{
    char bytes[4];
    
    bytes[0] = (value >> 24) & 0xff;
    bytes[1] = (value >> 16) & 0xff;
    bytes[2] = (value >> 8) & 0xff;
    bytes[3] = (value >> 0) & 0xff;
    memcpy(&value, bytes, 4);
    return value;
}

int64_t rtdb_cli_htoni64(int64_t value) 
{
    char bytes[8];
    
    bytes[0] = (value >> 56) & 0xff;
    bytes[1] = (value >> 48) & 0xff;
    bytes[2] = (value >> 40) & 0xff;
    bytes[3] = (value >> 32) & 0xff;
    bytes[4] = (value >> 24) & 0xff;
    bytes[5] = (value >> 16) & 0xff;
    bytes[6] = (value >> 8) & 0xff;
    bytes[7] = (value >> 0) & 0xff;
    memcpy(&value, bytes, 8);
    return value;
}

float rtdb_cli_htonf32(float value) 
{
    int32_t ivalue;
    
    memcpy(&ivalue, &value, 4);
    ivalue = rtdb_cli_htoni32(ivalue);
    memcpy(&value, &ivalue, 4);
    return value;
}

double rtdb_cli_htonf64(double value) 
{
    int64_t lvalue;
    
    memcpy(&lvalue, &value, 8);
    lvalue = rtdb_cli_htoni64(lvalue);
    memcpy(&value, &lvalue, 8);
    return value;
}

int16_t rtdb_cli_ntohi16(int16_t value) 
{
    value = (((value >> 0) & 0xff) << 8) +
        (((value >> 8) & 0xff) << 0);
    return value;
}

int32_t rtdb_cli_ntohi32(int32_t value) 
{
    value = (((value >> 0) & 0xff) << 24) + 
        (((value >> 8) & 0xff) << 16) +
        (((value >> 16) & 0xff) << 8) +
        (((value >> 24) & 0xff) << 0);
    return value;
}

int64_t rtdb_cli_ntohi64(int64_t value) 
{
    value = (((value >> 0) & 0xff) << 56) +
        (((value >> 8) & 0xff) << 48) +
        (((value >> 16) & 0xff) << 40) +
        (((value >> 24) & 0xff) << 32) +
        (((value >> 32) & 0xff) << 24) +
        (((value >> 40) & 0xff) << 16) +
        (((value >> 48) & 0xff) << 8) +
        (((value >> 56) & 0xff) << 0);
    
    return value;
}

float rtdb_cli_ntohf32(float value) 
{
    int32_t ivalue;
    
    memcpy(&ivalue, &value, 4);
    ivalue = rtdb_cli_ntohi32(ivalue);
    memcpy(&value, &ivalue, 4);
    return value;
}

double rtdb_cli_ntohf64(double value) 
{
    int32_t lvalue;
    
    memcpy(&lvalue, &value, 8);
    lvalue = rtdb_cli_ntohi64(lvalue);
    memcpy(&value, &lvalue, 8);
    return value;
}

