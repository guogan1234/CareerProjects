#ifndef CONF_PRIVDATA_H
#define CONF_PRIVDATA_H
#ifdef __cplusplus
extern "C"
{
#endif

#include <time.h>
#include "ae.h"
#include "conf_net.h"
#include "conf_sdk.h"
#include <pxthread/px_thread.h>
#include <pxthread/px_mutex.h>
#include <pxthread/px_queue.h>

typedef struct _conf_response_message
{
    int fd;
    int msgLen;
    char *msg;
} conf_response_message;

typedef struct _conf_server
{
    char *usr;
    char *pwd;
    char *ip1;
    char *ip2;
    int port;
} conf_server;

typedef struct _conf_conn
{
    conf_net net;
    char *tblname;
} conf_conn;

typedef struct _conf_management
{
    int domainId;

    /* cfg-server */
    int serverNum;
    conf_server **servers;

    /* connections */
    int connNum;
    conf_conn **conns;
    px_mutex *connMtx;

    /* threads */
    px_thread *readThread;
    px_thread *parseThread;
    px_thread *notifyThread;
    px_queue *messageQ; // queue of string type
    px_queue *tableQ; // queue of conf_table type
    px_mutex *messageMtx;
    px_mutex *tableMtx;

    /* notifiers */
    conf_load_notifier *loadNotifier;
    conf_sync_notifier *syncNotifier;

    aeEventLoop *el;
} conf_management;

extern conf_management g_confMgr;

conf_table* conf_create_table(char *tbleName, int fieldNum, int recordNum, int status, int type);

conf_response_message* conf_create_response_message(int fd, char *msg, int msgLen);

conf_server* conf_create_server();

conf_conn* conf_create_connection(const char *tblName, const conf_net *net);

void conf_free_table(void *t);

void conf_free_response_message(void *msg);

void conf_free_server(conf_server *s);

void conf_free_conn(conf_conn *c);

int conf_management_init();

int conf_management_load_globalconf();

void conf_management_startup();

void conf_management_shutdown();

void conf_management_destroy();

int conf_management_add_server(conf_server *s);

int conf_management_add_conn(conf_conn *c);

void conf_management_remove_conn(conf_conn *c, int enableNotify);

void conf_management_remove_conn_byfd(int fd, int enableNotify);

void conf_management_remove_conn_byname(const char *tblName, int enableNotify);

const conf_conn* conf_management_get_conn_byfd(int fd);

const conf_conn* conf_management_get_conn_byname(const char *tblName);

int conf_management_push_message(conf_response_message *m);

int conf_management_push_table(conf_table *t);

conf_response_message* conf_management_pop_message();

conf_table* conf_management_pop_table();

int conf_management_request_table(const char *tblName,int domain_id_flag);

#ifdef __cplusplus
}
#endif
#endif
