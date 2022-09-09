#include "conf_net.h"
#include "conf_byte_order.h"
#include <errno.h>
#include <time.h>
#include <stdio.h>
#ifdef _WIN32
#include <stddef.h>
#include <ws2tcpip.h>
#include <winerror.h>
#include <winsock2.h>

#pragma comment (lib, "Ws2_32.lib")
#define conf_net_local_read(fd,buf,len)     recv((fd),(buf),(len),0)
#define conf_net_local_write(fd,buf,len)    send((fd),(buf),(len),0)
#define conf_net_local_close(fd)            closesocket(fd)
#define conf_net_local_init()               do { WSADATA wsd; WSAStartup(MAKEWORD(2,2), &wsd); } while(0)
#define conf_net_local_uninit()             do { WSACleanup(); } while(0)
#define snprintf                            sprintf_s

#else
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

#define conf_net_local_read(fd,buf,len)     read((fd),(buf),(len))
#define conf_net_local_write(fd,buf,len)    write((fd),(buf),(len))
#define conf_net_local_close(fd)            close(fd)
#define conf_net_local_init()               do {} while(0)
#define conf_net_local_uninit()             do {} while(0)

#endif

void conf_net_init()
{
    conf_net_local_init();
}

void conf_net_uninit()
{
    conf_net_local_uninit();
}

void conf_net_close_conn(conf_net *net)
{
    if (!net || net->fd == -1) return;
    conf_net_local_close(net->fd);
}

int conf_net_check_error(conf_net *net, int *error)
{
    int iRet;
    socklen_t el = sizeof(int);

    *error = 0;
    iRet = getsockopt(net->fd, SOL_SOCKET, SO_ERROR, (void*)error, &el);

    return iRet ? -1 : 0;
}

int conf_net_set_timeout(conf_net *net, int ms, int flag)
{
    int iRet = -1;
#ifdef _WIN32
    DWORD to = ms;
#else
    struct timeval to = {ms/1000, (ms%1000)*1000};
#endif

    switch(flag)
    {
    case 1: // read timeout
        iRet = setsockopt(net->fd, SOL_SOCKET, SO_RCVTIMEO, (void*)&to, sizeof(to));
        break;
    case 2: // write timeout
        iRet = setsockopt(net->fd, SOL_SOCKET, SO_SNDTIMEO, (void*)&to, sizeof(to));
        break;
    default: // read and write timeout
        iRet = setsockopt(net->fd, SOL_SOCKET, SO_RCVTIMEO, (void*)&to, sizeof(to));
        if (iRet) break;
        iRet = setsockopt(net->fd, SOL_SOCKET, SO_SNDTIMEO, (void*)&to, sizeof(to));
        break;
    }

    return iRet ? -1 : 0;
}

int conf_net_keep_alive(conf_net *net, int second)
{
    int iRet, val = 1;

    iRet = setsockopt(net->fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&val, sizeof(val));
    if (iRet) return -1;

    val = second;
#ifdef _OSX
    iRet = setsockopt(net->fd, IPPROTO_ICP, TCP_KEEPALIVE, (void*)&val, sizeof(val));
    if (iRet) return -1;
#elif defined(__linux__)
    iRet = setsockopt(net->fd, IPPROTO_TCP, TCP_KEEPIDLE, (void*)&val, sizeof(val));
    if (iRet) return -1;

    val = second/3;
    if (!val) val = 1;
    iRet = setsockopt(net->fd, IPPROTO_TCP, TCP_KEEPINTVL, (void*)&val, sizeof(val));
    if (iRet) return -1;

    val = 3;
    iRet = setsockopt(net->fd, IPPROTO_TCP, TCP_KEEPCNT, (void*)&val, sizeof(val));
    if (iRet) return -1;
#endif

    return 0;
}

int conf_net_set_block(conf_net *net, int noblock)
{
    int iRet;

#ifdef _WIN32
    iRet = ioctlsocket(net->fd, FIONBIO, (unsigned long*)&noblock);
    if (iRet)
    {
        return -1;
    }
#else
    iRet = fcntl(net->fd, F_GETFL);
    if (iRet == -1)
    {
        return -1;
    }

    if (noblock)
    {
        iRet |= O_NONBLOCK;
    }
    else
    {
        iRet &= ~O_NONBLOCK;
    }

    iRet = fcntl(net->fd, F_SETFL, iRet);
    if (iRet == -1)
    {
        return -1;
    }
#endif

    return 0;
}

int conf_net_set_delay(conf_net *net, int nodelay)
{
    int iRet;

    iRet = setsockopt(net->fd, IPPROTO_TCP, TCP_NODELAY, (const void*)&nodelay, sizeof(nodelay));

    return iRet ? -1 : 0;
}

int conf_net_enable_reuseaddress(conf_net *net)
{
    int iRet;
    int yes = 1;

    iRet = setsockopt(net->fd, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));

    return iRet ? -1 : 0;
}

int conf_net_connect_tcpserver(conf_net *net, const char *ip, int port, const char *bindIp, int flags)
{
    int fd, iRet;
    char sport[6]; /* strlen("65535") + 1; */
    struct addrinfo hints, *pInfo, *pSInfoN;
    conf_net net1;

    net->fd = -1;
    net->flags = flags;
    snprintf(sport, sizeof(sport), "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    iRet = getaddrinfo(ip, sport, &hints, &pInfo);
    if (iRet)
    {
        return -1;
    }

    for (pSInfoN = pInfo; pSInfoN; pSInfoN = pSInfoN->ai_next)
    {
        fd = socket(pSInfoN->ai_family, pSInfoN->ai_socktype, pSInfoN->ai_protocol);
        if (fd == -1) continue;
        net1.fd = fd;

        /*
        iRet = conf_net_enable_reuseaddress(fd);
        if (iRet != CONF_NET_OK)
        {
            conf_net_close_conn(fd);
            break;
        }
        */
        iRet = conf_net_set_block(&net1, 1);
        if (iRet == -1)
        {
            conf_net_close_conn(&net1);
            break;
        }

        if (bindIp)
        {
            int bound = 0;
            struct addrinfo *pBSInfo, *pBSInfoN;

            iRet = getaddrinfo(bindIp, NULL, &hints, &pBSInfo);
            if (iRet)
            {
                conf_net_close_conn(&net1);
                break;
            }

            for (pBSInfoN = pBSInfo; pBSInfoN; pBSInfoN = pBSInfoN->ai_next)
            {
                iRet = bind(net1.fd, pBSInfoN->ai_addr, pBSInfoN->ai_addrlen);
                if (!iRet)
                {
                    bound = 1;
                    break;
                }
            }

            freeaddrinfo(pBSInfo);
            if (!bound)
            {
                conf_net_close_conn(&net1);
                break;
            }
        }

        iRet = connect(net1.fd, pSInfoN->ai_addr, pSInfoN->ai_addrlen);
        if (iRet)
        {
            if (errno == EHOSTUNREACH)
            {
                printf("connect error, errno=%d, errdesc=%s\n", errno, strerror(errno));
                conf_net_close_conn(&net1);
                continue;
            }
            else if (errno == EINPROGRESS && (flags & CONF_NET_NOBLOCK))
            {
                // this is ok
                // printf("connect ok\n");
            }
            else
            {
                printf("connect error, errno=%d, errdesc=%s\n", errno, strerror(errno));
                break;
            }
        }

        if (!(flags & CONF_NET_NOBLOCK))
        {
            if (conf_net_set_block(&net1, 0) == -1)
            {
                conf_net_close_conn(&net1);
                break;
            }
        }
        if (conf_net_set_delay(&net1, 1) == -1)
        {
            conf_net_close_conn(&net1);
            break;
        }

        net->fd = net1.fd;
        break;
    }

    freeaddrinfo(pInfo);

    return net->fd == -1 ? -1 : 0;
}

int conf_net_read_bytes(conf_net *net, char *buf, int *size)
{
    int nread, maxlen, totallen = 0, iRet = 0;

    if (!net) return -1;

    for (maxlen = *size; totallen != maxlen;)
    {
        nread = conf_net_local_read(net->fd, buf, maxlen - totallen);
        if (nread == 0)
        {// server close the connection
            break;
        }
        else if (nread == -1)
        {
            if ((errno == EAGAIN && (net->flags & CONF_NET_NOBLOCK)) || (errno == EINTR))
            {// try again later
                continue;
            }
            else
            {// error
                iRet = -1;
                break;
            }
        }

        totallen += nread;
        buf += nread;
    }

    *size = totallen;

    return iRet;
}

int conf_net_read_int16(conf_net *net, int16_t *v)
{
    int size = 2;

    if (!net || net->fd == -1 || !v) return -1;

    if (conf_net_read_bytes(net, (char*)v, &size) == 0)
    {
        *v = conf_byteorder_n2h_int16(*v);
        return 0;
    }

    return -1;
}

int conf_net_read_int32(conf_net *net, int32_t *v)
{
    int size = 4;

    if (!net || net->fd == -1 || !v) return -1;

    if (conf_net_read_bytes(net, (char*)v, &size) == 0)
    {
        *v = conf_byteorder_n2h_int32(*v);
        return 0;
    }

    return -1;
}

int conf_net_read_int64(conf_net *net, int64_t *v)
{
    int size = 8;

    if (!net || net->fd == -1 || !v) return -1;

    if (conf_net_read_bytes(net, (char*)v, &size) == 0)
    {
        *v = conf_byteorder_n2h_int64(*v);
        return 0;
    }

    return -1;
}

int conf_net_read_float(conf_net *net, float *v)
{
    int size = 4;

    if (!net || net->fd == -1 || !v) return -1;

    if (conf_net_read_bytes(net, (char*)v, &size) == 0)
    {
        *v = conf_byteorder_n2h_float(*v);
        return 0;
    }

    return -1;
}

int conf_net_read_double(conf_net *net, double *v)
{
    int size = 8;

    if (!net || net->fd == -1 || !v) return -1;

    if (conf_net_read_bytes(net, (char*)v, &size) == 0)
    {
        *v = conf_byteorder_n2h_double(*v);
        return 0;
    }

    return -1;
}

int conf_net_write_bytes(conf_net *net, const char *buf, int size)
{
    int nwrite, totallen = 0, iRet = 0;

    while (totallen != size)
    {
        nwrite = conf_net_local_write(net->fd, buf, size - totallen);
        if (!nwrite)
        {
            break;
        }
        else if (nwrite == -1)
        {
            if ((errno == EAGAIN && (net->flags & CONF_NET_NOBLOCK)) || (errno == EINTR))
            {// try again later
                continue;
            }
            else
            {
                iRet = -1;
                break;
            }
        }

        totallen += nwrite;
        buf += nwrite;
        if (totallen == size) break;
    }

    return iRet;
}

int conf_net_write_int16(conf_net *net, int16_t v)
{
    int16_t nv = conf_byteorder_h2n_int16(v);
    return conf_net_write_bytes(net, (const char*)&nv, 2);
}

int conf_net_write_int32(conf_net *net, int32_t v)
{
    int32_t nv = conf_byteorder_h2n_int32(v);
    return conf_net_write_bytes(net, (const char*)&nv, 4);
}

int conf_net_write_int64(conf_net *net, int64_t v)
{
    int64_t nv = conf_byteorder_h2n_int64(v);
    return conf_net_write_bytes(net, (const char*)&nv, 8);
}

int conf_net_write_float(conf_net *net, float v)
{
    float nv = conf_byteorder_h2n_float(v);
    return conf_net_write_bytes(net, (const char*)&nv, 4);
}

int conf_net_write_double(conf_net *net, double v)
{
    double nv = conf_byteorder_h2n_double(v);
    return conf_net_write_bytes(net, (const char*)&nv, 8);
}
