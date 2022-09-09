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
#include <winsock.h>
#include <winsock2.h>
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
#endif

extern cmn_logger* logger;

uint16_t rtdb_cli_htoni16(uint16_t value) 
{
    char bytes[2];
    
    bytes[0] = (value >> 8) & 0xff;
    bytes[1] = (value >> 0) & 0xff;
    memcpy(&value, bytes, 2);
    return value;
}

uint32_t rtdb_cli_htoni32(uint32_t value) 
{
    char bytes[4];
    
    bytes[0] = (value >> 24) & 0xff;
    bytes[1] = (value >> 16) & 0xff;
    bytes[2] = (value >> 8) & 0xff;
    bytes[3] = (value >> 0) & 0xff;
    memcpy(&value, bytes, 4);
    return value;
}

uint64_t rtdb_cli_htoni64(uint64_t value) 
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

uint16_t rtdb_cli_ntohi16(uint16_t value) 
{
    value = (((value >> 0) & 0xff) << 8) +
        (((value >> 8) & 0xff) << 0);
    return value;
}

uint32_t rtdb_cli_ntohi32(uint32_t value) 
{
    value = (((value >> 0) & 0xff) << 24) + 
        (((value >> 8) & 0xff) << 16) +
        (((value >> 16) & 0xff) << 8) +
        (((value >> 24) & 0xff) << 0);
    return value;
}

uint64_t rtdb_cli_ntohi64(uint64_t value) 
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
    int64_t lvalue;
    
    memcpy(&lvalue, &value, 8);
    lvalue = rtdb_cli_ntohi64(lvalue);
    memcpy(&value, &lvalue, 8);
    return value;
}

int rtdb_cli_net_read(int fd, char *buf, int count)
{
	int nread, totlen = 0;
	while (totlen != count)
	{
		nread = network_read(fd, buf, count - totlen);
		if (nread == 0) return totlen;
		if (nread == -1) return -1;
		totlen += nread;
		buf += nread;
	}
	return totlen;
}

int rtdb_cli_net_write(int fd, char *buf, int count)
{
	int nwritten, totlen = 0;

	while (totlen != count)
	{
		nwritten = network_write(fd, buf, count - totlen);
		if (nwritten == 0) return totlen;
		if (nwritten == -1) return -1;
		totlen += nwritten;
		buf += nwritten;
	}
	return totlen;
}

int rtdb_cli_net_check_tcp(const char* ip, int port,char *bytes,int bytesLen, int timeout)
{
	int iRet = -1; // 0 : ok , -1 : fail
	int sockfd = 0, activity = 0, status = 0;
	struct sockaddr_in saddr;
	struct timeval tv;
	fd_set read_fds;

	if (!ip || !port || !bytes || !bytesLen) return -1;
	CMN_LOGGER_DEBUG(logger,"rtdb_cli_net_check_tcp (%s:%d) ",ip,port);

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(ip);
	saddr.sin_port = htons(port);

	FD_ZERO(&read_fds);
	FD_SET(sockfd, &read_fds);

#ifdef _WIN32
	long noblock = 1; // 1 : yes
	status = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&noblock);
	if (status != NO_ERROR)
		return -1;
#else
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif   

	status = connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	if (status < 0)
	{
#ifndef _WIN32
    if (errno != EINPROGRESS) return -1;
#endif
	}
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	activity = select(sockfd + 1, &read_fds, &read_fds, NULL, &tv);
	if (activity)
	{
		//if (send(sockfd, "\x00", 1, 0) == 1)
		if(rtdb_cli_net_write(sockfd,bytes,bytesLen) == bytesLen)
			iRet = 0;
	}
#ifdef _WIN32
	closesocket(sockfd);
#else
	close(sockfd);
#endif
	return iRet;
}

int rtdb_cli_net_cnn_rtdb(const char *ip, int port)
{
	int tmpRet = 0;
	int sockfd = 0;
	struct sockaddr_in server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		CMN_LOGGER_ERR(logger, "rtdb_cli_net_cnn_rtdb : socket error!");
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	tmpRet = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if (tmpRet != 0)
	{
		CMN_LOGGER_ERR(logger, "connect(%s:%d) error!",ip,port);
		return -1;
	}
	CMN_LOGGER_DEBUG(logger, "connect to (%s:%d) ok.sockfd = %d", ip,port,sockfd);
	return sockfd;
}

int rtdb_cli_net_getfd(const char *active_ip,int modePort,int *fdOut)
{
	int clientFd = -1;
	
	if(!active_ip || !fdOut) return -1;
	CMN_LOGGER_DEBUG(logger, "rtdb_cli_net_getfd : %d", *fdOut);
	if(*fdOut <= 0)
	{
		clientFd = rtdb_cli_net_cnn_rtdb(active_ip,modePort);
		*fdOut = clientFd;
	}
	clientFd = *fdOut ;
	return clientFd;
}


