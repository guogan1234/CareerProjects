/**
 * return values of all the conf_net_xxx, return CONF_NET_OK(0) on ok,
 * CONF_NET_ERROR(-1) on error.
 */
#ifndef CONF_NET_H
#define CONF_NET_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define CONF_NET_NONE       0x00000000
#define CONF_NET_NOBLOCK    0x00000001
#define CONF_NET_NODELAY    0x00000002

typedef struct _conf_net
{
    int fd;
    int flags;
} conf_net;

void conf_net_init();

void conf_net_uninit();

void conf_net_close_conn(conf_net *net);

int conf_net_check_error(conf_net *net, int *error);

/*
 * conf_net_set_timeout used to set read and write timeout.
 * flag:[1 -- read; 2 -- write; 3 -- read and write]
 */
int conf_net_set_timeout(conf_net *net, int ms, int flag);

int conf_net_keep_alive(conf_net *net, int second);

/* conf_net_set_block used to set connection block feature.
 * noblock=1, set connection with noblock.
 * noblock=0, set connection with block.
 */
int conf_net_set_block(conf_net *net, int noblock);

/* conf_net_set_delay used to set connection delay feature.
 * nodelay=1, set connection with nodelay.
 * nodelay=0, set connection with delay.
 */
int conf_net_set_delay(conf_net *net, int nodelay);

int conf_net_enable_reuseaddress(conf_net *net);

/*
 * conf_net_connect used to conect tcp server.
 * ip the target tcp server ip.
 * port the target tcp server port.
 * bindIp is the local ip to be bound to connection.
 * flags is the mask value of CONF_NET_BLOCK, CONF_NET_NOBLOCK, CONF_NET_DELAY, CONF_NET_NODELAY
 * return 0 on ok, -1 on error.
 **/
int conf_net_connect_tcpserver(conf_net *net, const char *ip, int port, const char *bindIp, int flags);

/*
 * conf_net_read_bytes read bytes data from connection.
 * buf use to store data, allocated by caller.
 * size input the max bytes to read, and output bytes actually read.
 **/
int conf_net_read_bytes(conf_net *net, char *buf, int *size);

int conf_net_read_int16(conf_net *net, int16_t *v);

int conf_net_read_int32(conf_net *net, int32_t *v);

int conf_net_read_int64(conf_net *net, int64_t *v);

int conf_net_read_float(conf_net *net, float *v);

int conf_net_read_double(conf_net *net, double *v);

int conf_net_write_bytes(conf_net *net, const char *buf, int size);

int conf_net_write_int16(conf_net *net, int16_t v);

int conf_net_write_int32(conf_net *net, int32_t v);

int conf_net_write_int64(conf_net *net, int64_t v);

int conf_net_write_float(conf_net *net, float v);

int conf_net_write_double(conf_net *net, double v);

#ifdef __cplusplus
}
#endif
#endif
