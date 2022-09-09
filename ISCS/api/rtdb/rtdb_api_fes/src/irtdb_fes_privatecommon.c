#include "irtdb_fes_privatecommon.h"
#include "irtdb_fes_privatedata.h"
#include <string.h>
#include <stdlib.h>

int16_t irtdb_fes_htoni16(int16_t value) 
{
    char bytes[2];
    
    bytes[0] = (value >> 8) & 0xff;
    bytes[1] = (value >> 0) & 0xff;
    memcpy(&value, bytes, 2);
    return value;
}

int32_t irtdb_fes_htoni32(int32_t value) 
{
    char bytes[4];
    
    bytes[0] = (value >> 24) & 0xff;
    bytes[1] = (value >> 16) & 0xff;
    bytes[2] = (value >> 8) & 0xff;
    bytes[3] = (value >> 0) & 0xff;
    memcpy(&value, bytes, 4);
    return value;
}

int64_t irtdb_fes_htoni64(int64_t value) 
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

float irtdb_fes_htonf32(float value) 
{
    int32_t ivalue;
    
    memcpy(&ivalue, &value, 4);
    ivalue = irtdb_fes_htoni32(ivalue);
    memcpy(&value, &ivalue, 4);
    return value;
}

double irtdb_fes_htonf64(double value) 
{
    int64_t lvalue;
    
    memcpy(&lvalue, &value, 8);
    lvalue = irtdb_fes_htoni64(lvalue);
    memcpy(&value, &lvalue, 8);
    return value;
}

int16_t irtdb_fes_ntohi16(int16_t value) 
{
    value = (((value >> 0) & 0xff) << 8) +
        (((value >> 8) & 0xff) << 0);
    return value;
}

int32_t irtdb_fes_ntohi32(int32_t value) 
{
    value = (((value >> 0) & 0xff) << 24) + 
        (((value >> 8) & 0xff) << 16) +
        (((value >> 16) & 0xff) << 8) +
        (((value >> 24) & 0xff) << 0);
    return value;
}

int64_t irtdb_fes_ntohi64(int64_t value) 
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

float irtdb_fes_ntohf32(float value) 
{
    int32_t ivalue;
    
    memcpy(&ivalue, &value, 4);
    ivalue = irtdb_fes_ntohi32(ivalue);
    memcpy(&value, &ivalue, 4);
    return value;
}

double irtdb_fes_ntohf64(double value) 
{
    int64_t lvalue;
    
    memcpy(&lvalue, &value, 8);
    lvalue = irtdb_fes_ntohi64(lvalue);
    memcpy(&value, &lvalue, 8);
    return value;
}

int irtdb_fes_check_tcp_port(const char* ip,int port,int timeout)
{
    int iRet = 0; // 0 : ok , -1 : fail
	int sockfd = 0, status = 0, activity = 0;
    struct sockaddr_in saddr;
    struct timeval tv;
    fd_set read_fds;
    long noblock = 1; // 1 : yes
    
    if(!ip) return -1;

    sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(port);

    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);

#ifdef _WIN32
    status = ioctlsocket(sockfd,FIONBIO,(unsigned long*)&noblock); 
    if (status != NO_ERROR)
    {
        closesocket(sockfd);
        return -1;   
    }
#else
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
#endif   
    
    status = connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(status < 0)
    {
        #ifndef _WIN32
        if (errno != EINPROGRESS) 
        {
            close(sockfd);
            return -1;
        }
        #endif
    }
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    activity = select(sockfd + 1,&read_fds,&read_fds,NULL,&tv);
    if (activity)
    {
        if(send(sockfd,"\x00",1,0) == 1) 
            iRet = 0;
        else
            iRet = -1;
    }
    else
        iRet = -1;

#ifdef _WIN32
    closesocket(sockfd);
#else
    close(sockfd);
#endif

    return iRet;
}
