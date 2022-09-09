#include "conf_sdk.h"
#include "conf_priv_data.h"
#include "conf_ae_handler.h"
#include "conf_thread_handler.h"
#include "conf_message.h"
#include "conf_net.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "conf_parser.h"

conf_management g_confMgr;

conf_table* conf_create_table(char *tbleName, int fieldNum, int recordNum, int status, int type)
{
    int i;
    conf_table *t;

    if (!tbleName || fieldNum < 0 || recordNum < 0)
    {
        return NULL;
    }
    if (!(t = malloc(sizeof(*t))))
    {
        return NULL;
    }

    memset(t, 0, sizeof(*t));
    t->type = type;
    t->status = status;
    t->fieldNum = fieldNum;
    t->recordNum = recordNum;
    t->tableName = tbleName;

    if (fieldNum > 0)
    {
        t->fieldTypes = malloc(sizeof(int)*fieldNum);
        if (!t->fieldTypes)
        {
            conf_free_table(t);
            return NULL;
        }
        memset(t->fieldTypes, 0, sizeof(int)*fieldNum);

        t->fieldNames = malloc(sizeof(char*)*fieldNum);
        if (!t->fieldNames)
        {
            conf_free_table(t);
            return NULL;
        }
        memset(t->fieldNames, 0, sizeof(char*)*fieldNum);
    }

    if (recordNum > 0)
    {
        t->data = malloc(sizeof(conf_variant*)*recordNum);
        if (!t->data)
        {
            conf_free_table(t);
            return NULL;
        }
        memset(t->data, 0, sizeof(conf_variant*)*recordNum);

        if (fieldNum > 0)
        {
            for (i = 0; i < recordNum; i++)
            {
                t->data[i] = malloc(sizeof(conf_variant)*fieldNum);
                if (!t->data[i])
                {
                    conf_free_table(t);
                    return NULL;
                }

                memset(t->data[i], 0, sizeof(conf_variant)*fieldNum);
            }
        }
    }

    return t;
}

conf_response_message* conf_create_response_message(int fd, char *msg, int msgLen)
{
    conf_response_message *cmsg = malloc(sizeof(*cmsg));
    if (cmsg)
    {
        cmsg->fd = fd;
        cmsg->msg = msg;
        cmsg->msgLen = msgLen;
    }

    return cmsg;
}

conf_server* conf_create_server()
{
    conf_server *s = malloc(sizeof(*s));

    if (s) memset(s, 0, sizeof(*s));

    return s;
}

conf_conn* conf_create_connection(const char *tblName, const conf_net *net)
{
    conf_conn *c;

    if (!tblName) return NULL;

    c = malloc(sizeof(*c));
    if (c)
    {
        if (!(c->tblname = strdup(tblName)))
        {
            free(c);
            return NULL;
        }
    }

    if (net)
    {
        c->net.fd = net->fd;
        c->net.flags = net->flags;
    }
    else
    {
        c->net.fd = -1;
        c->net.flags = CONF_NET_NONE;
    }

    return c;
}

void conf_free_table(void *t)
{
    int i, j;
    conf_table *table = (conf_table*)t;

    if (!table) return;
    if (table->tableName) free(table->tableName);
    if (table->fieldTypes) free(table->fieldTypes);
    if (table->fieldNames)
    {
        for (i = 0; i < table->fieldNum; i++)
        {
            if (table->fieldNames[i])
            {
                free(table->fieldNames[i]);
            }
        }

        free(table->fieldNames);
    }
    if (table->data)
    {
        for (i = 0; i < table->recordNum; i++)
        {
            if (table->data[i])
            {
                for (j = 0; j < table->fieldNum; j++)
                {
                    if (table->fieldTypes[j] == CONF_VT_STRING)
                    {
                        free(table->data[i][j].s);
                    }
                }
                free(table->data[i]);
            }
        }
        free(table->data);
    }

    free(table);
}

void conf_free_response_message(void *msg)
{
    conf_response_message *cmsg = (conf_response_message*)msg;
    if (!cmsg) return;
    if (cmsg->msg) free(cmsg->msg);
    free(cmsg);
}

void conf_free_server(conf_server *s)
{
    if (!s) return;
    if (s->usr) free(s->usr);
    if (s->pwd) free(s->pwd);
    if (s->ip1) free(s->ip1);
    if (s->ip2) free(s->ip2);
    free(s);
}

void conf_free_conn(conf_conn *c)
{
    if (!c) return;
    if (c->tblname) free(c->tblname);
    conf_net_close_conn(&c->net);
    free(c);
}

int conf_management_init()
{
    px_runnable *r;

    memset(&g_confMgr, 0, sizeof(g_confMgr));
    g_confMgr.domainId = -1;

    g_confMgr.connMtx = px_mutex_create();
    if (!g_confMgr.connMtx) return -1;

    g_confMgr.el = aeCreateEventLoop(10240);
    if (!g_confMgr.el)
    {
        conf_management_destroy();
        return -1;
    }

    r = px_runnable_create(conf_threadhandler_recive_message, NULL, NULL);
    if (r)
    {
        g_confMgr.readThread = px_thread_create(r);
        if (!g_confMgr.readThread)
        {
            aeDeleteEventLoop(g_confMgr.el);
            px_runnable_free(r);
            return -1;
        }
    }
    else
    {
        aeDeleteEventLoop(g_confMgr.el);
        return -1;
    }

    r = px_runnable_create(conf_threadhandler_parse_message, NULL, NULL);
    if (r)
    {
        g_confMgr.parseThread = px_thread_create(r);
        if (!g_confMgr.parseThread)
        {
            conf_management_destroy();
            px_runnable_free(r);
            return -1;
        }
    }
    else
    {
        conf_management_destroy();
        return -1;
    }

    r = px_runnable_create(conf_threadhandler_notify_message, NULL, NULL);
    if (r)
    {
        g_confMgr.notifyThread = px_thread_create(r);
        if (!g_confMgr.notifyThread)
        {
            conf_management_destroy();
            px_runnable_free(r);
            return -1;
        }
    }
    else
    {
        conf_management_destroy();
        return -1;
    }

    g_confMgr.messageQ = px_queue_create(conf_free_response_message);
    if (!g_confMgr.messageQ)
    {
        conf_management_destroy();
        return -1;
    }

    g_confMgr.tableQ = px_queue_create(conf_free_table);
    if (!g_confMgr.tableQ)
    {
        conf_management_destroy();
        return -1;
    }

    g_confMgr.messageMtx = px_mutex_create();
    if (!g_confMgr.messageMtx)
    {
        conf_management_destroy();
        return -1;
    }

    g_confMgr.tableMtx = px_mutex_create();
    if (!g_confMgr.tableMtx)
    {
        conf_management_destroy();
        return -1;
    }

    return 0;
}

int conf_management_load_globalconf()
{   
    sys_conf_confdb_domain* confdb = sys_conf_get_confdb_domain();
    sys_conf_confdb_server* confNode = NULL;
    conf_server *s = NULL;
    if(!confdb)
    {
        printf("sys_conf_get_confdb_domain fail!\n");
        return -1;
    }    
    s = conf_create_server();
    if(!s) return -1;
    confNode = confdb->first;       
    if(confNode)
    {
        g_confMgr.domainId = confdb->domain_id;
        s->ip1 = strdup(confNode->rtdb_sync_ip);        
        s->port = confNode->rtdb_sync_port;
        if(conf_management_add_server(s) == -1)
        {
            conf_free_server(s);
        }
       // if(s->ip1) printf("s->ip1  : %s\n",s->ip1);    
        //printf("s->port : %d\n",s->port);
        //printf("confdb->domain_id : %d\n",confdb->domain_id);
    }
    else
    {
        printf("sys_conf_confdb_server get data fail!\n");
    }
    
    sys_conf_free_confdb_domain(confdb);
    return 0;
}

void conf_management_startup()
{
    if (g_confMgr.readThread) px_thread_start(g_confMgr.readThread);
    if (g_confMgr.parseThread) px_thread_start(g_confMgr.parseThread);
    if (g_confMgr.notifyThread) px_thread_start(g_confMgr.notifyThread);
}

void conf_management_shutdown()
{
    if (g_confMgr.el) aeStop(g_confMgr.el);
    if (g_confMgr.readThread) px_thread_stop(g_confMgr.readThread);
    if (g_confMgr.parseThread) px_thread_stop(g_confMgr.parseThread);
    if (g_confMgr.notifyThread) px_thread_stop(g_confMgr.notifyThread);
}

void conf_management_destroy()
{
    int i;

    if (g_confMgr.connMtx) px_mutex_free(g_confMgr.connMtx);

    if (g_confMgr.el)
    {
        aeStop(g_confMgr.el);
        aeDeleteEventLoop(g_confMgr.el);
    }

    if (g_confMgr.servers)
    {
        for (i = 0; i < g_confMgr.serverNum; i++)
        {
            if (g_confMgr.servers[i]) conf_free_server(g_confMgr.servers[i]);
        }
        free(g_confMgr.servers);
    }

    if (g_confMgr.conns)
    {
        for (i = 0; i < g_confMgr.connNum; i++)
        {
            if (g_confMgr.conns[i]) conf_free_conn(g_confMgr.conns[i]);
        }
        free(g_confMgr.conns);
    }

    if (g_confMgr.el) aeDeleteEventLoop(g_confMgr.el);
    if (g_confMgr.readThread) px_thread_free(g_confMgr.readThread);
    if (g_confMgr.parseThread) px_thread_free(g_confMgr.parseThread);
    if (g_confMgr.notifyThread) px_thread_free(g_confMgr.notifyThread);
    if (g_confMgr.messageQ) px_queue_free(g_confMgr.messageQ);
    if (g_confMgr.tableQ) px_queue_free(g_confMgr.tableQ);
    if (g_confMgr.messageMtx) px_mutex_free(g_confMgr.messageMtx);
    if (g_confMgr.tableMtx) px_mutex_free(g_confMgr.tableMtx);

    memset(&g_confMgr, 0, sizeof(g_confMgr));
}

int conf_management_add_server(conf_server *s)
{
    int iRet;
    conf_server **csa;

    if (!s) return -1;

    if (!g_confMgr.servers)
    {
        iRet = -1;
        g_confMgr.servers = malloc(sizeof(conf_server*));
        if (g_confMgr.servers)
        {
            iRet = 0;
            g_confMgr.servers[0] = s;
            g_confMgr.serverNum = 1;
        }

        return iRet;
    }

    iRet = -1;
    csa = realloc(g_confMgr.servers, sizeof(conf_server*)*(g_confMgr.serverNum+1));
    if (csa)
    {
        iRet = 0;
        csa[g_confMgr.serverNum] = s;
        g_confMgr.servers = csa;
        ++g_confMgr.serverNum;
    }

    return iRet;
}

int conf_management_add_conn(conf_conn *c)
{
    int i, iRet;
    conf_conn **cca;

    if (!c) return -1;

    // first time to add conf_conn
    px_mutex_lock(g_confMgr.connMtx);
    if (!g_confMgr.conns)
    {
        iRet = -1;
        g_confMgr.conns = malloc(sizeof(conf_conn*));
        if (g_confMgr.conns)
        {
            iRet = 0;
            g_confMgr.conns[0] = c;
            g_confMgr.connNum = 1;
        }
        px_mutex_unlock(g_confMgr.connMtx);

        return iRet;
    }

    // check null position
    for (i = 0; i < g_confMgr.connNum; ++i)
    {
        if (!g_confMgr.conns[i])
        {
            g_confMgr.conns[i] = c;
            px_mutex_unlock(g_confMgr.connMtx);
            return 0;
        }
    }

    iRet = -1;
    cca = realloc(g_confMgr.conns, sizeof(conf_conn*)*(g_confMgr.connNum+1));
    if (cca)
    {
        iRet = 0;
        cca[g_confMgr.connNum] = c;
        g_confMgr.conns = cca;
        ++g_confMgr.connNum;
    }
    px_mutex_unlock(g_confMgr.connMtx);

    return 0;
}

static int conf_management_get_connindex_byfd(int fd)
{
    int i, index = -1;

    for (i = 0; i < g_confMgr.connNum; i++)
    {
        if (g_confMgr.conns[i] && g_confMgr.conns[i]->net.fd == fd)
        {
            index = i;
            break;
        }
    }

    return index;
}

static int conf_management_get_connindex_byname(const char *tblName)
{
    int i, index = -1;

    if (!tblName) return -1;

    for (i = 0; i < g_confMgr.connNum; i++)
    {
        if (g_confMgr.conns[i] && !strcmp(g_confMgr.conns[i]->tblname, tblName))
        {
            index = i;
            break;
        }
    }

    return index;
}

void conf_management_remove_conn(conf_conn *c, int enableNotify)
{
    int i;

    if (!c) return;

    px_mutex_lock(g_confMgr.connMtx);
    for (i = 0; i < g_confMgr.connNum; i++)
    {
        if (g_confMgr.conns[i] != c) continue;

        // remove event loop, notify collection delete
        g_confMgr.conns[i] = NULL;
        aeDeleteFileEvent(g_confMgr.el, c->net.fd, AE_READABLE);
        if (enableNotify)
        {
            conf_table table;
            memset(&table, 0, sizeof(table));
            table.tableName = c->tblname;
            table.status = TABLE_STATUS_CONNBREAK;
            if (g_confMgr.loadNotifier)
            {
                g_confMgr.loadNotifier(&table);
            }
            if (g_confMgr.syncNotifier)
            {
                g_confMgr.syncNotifier(&table);
            }
        }

        conf_free_conn(c);
        break;
    }
    px_mutex_unlock(g_confMgr.connMtx);
}

void conf_management_remove_conn_byfd(int fd, int enableNotify)
{
    int index ;

    index = conf_management_get_connindex_byfd(fd);
    if (index != -1)
    {
        conf_management_remove_conn(g_confMgr.conns[index], enableNotify);
    }
}

void conf_management_remove_conn_byname(const char *tblName, int enableNotify)
{
    int index;

    if (!tblName) return;

    index = conf_management_get_connindex_byname(tblName);
    if (index != -1)
    {
        conf_management_remove_conn(g_confMgr.conns[index], enableNotify);
    }
}

const conf_conn* conf_management_get_conn_byfd(int fd)
{
    int index ;

    index = conf_management_get_connindex_byfd(fd);
    return (index == -1) ? NULL : g_confMgr.conns[index];
}

const conf_conn* conf_management_get_conn_byname(const char *tblName)
{
    int index ;

    index = conf_management_get_connindex_byname(tblName);
    return (index == -1) ? NULL : g_confMgr.conns[index];
}

int conf_management_push_message(conf_response_message *m)
{
    int iRet;

    if (!m || !g_confMgr.messageQ || !g_confMgr.messageMtx)
    {
        return -1;
    }

    px_mutex_lock(g_confMgr.messageMtx);
    iRet = px_queue_push(g_confMgr.messageQ, m);
    px_mutex_unlock(g_confMgr.messageMtx);

    return iRet;
}

int conf_management_push_table(conf_table *t)
{
    int iRet;

    if (!t || !g_confMgr.tableQ || !g_confMgr.tableMtx)
    {
        return -1;
    }

    px_mutex_lock(g_confMgr.tableMtx);
    iRet = px_queue_push(g_confMgr.tableQ, t);
    px_mutex_unlock(g_confMgr.tableMtx);

    return iRet;
}

conf_response_message *conf_management_pop_message()
{
    conf_response_message *msg;

    if (!g_confMgr.messageQ || !g_confMgr.messageMtx)
    {
        return NULL;
    }

    px_mutex_lock(g_confMgr.messageMtx);
    msg = (conf_response_message*)px_queue_pop(g_confMgr.messageQ);
    px_mutex_unlock(g_confMgr.messageMtx);

    return msg;
}

conf_table* conf_management_pop_table()
{
    conf_table *table;

    if (!g_confMgr.tableQ || !g_confMgr.tableMtx)
    {
        return NULL;
    }

    px_mutex_lock(g_confMgr.tableMtx);
    table = (conf_table*)px_queue_pop(g_confMgr.tableQ);
    px_mutex_unlock(g_confMgr.tableMtx);

    return table;
}

int conf_management_request_table(const char *tblName,int domain_id_flag)
{
    int iRet, reqLen = 0, servIndex, ipIndex;
    char *req, *ip;
    conf_net net;
    conf_conn *cc;
    conf_server *cs;

    req = conf_message_generate_load_request(tblName,domain_id_flag,&reqLen);
    if (!req) return -1;

    // check if already exist the table request.
    cc = (conf_conn*)conf_management_get_conn_byname(tblName);
    if (cc)
    {
        iRet = conf_net_write_bytes(&cc->net, req, reqLen);
        if (iRet == 0)
        {
            free(req);
            return 0;
        }
        else
        {
            conf_management_remove_conn(cc, 1);
        }
    }

    // create new connection for the new request.
    for (servIndex = 0; servIndex < g_confMgr.serverNum; ++servIndex)
    {
        cs = g_confMgr.servers[servIndex];
        if (!cs) continue;

        for (ipIndex = 1; ipIndex <= 2; ++ipIndex)
        {
            ip = ipIndex==1 ? cs->ip1 : cs->ip2;
            if (!ip) continue;

            iRet = conf_net_connect_tcpserver(&net, ip, cs->port, NULL, CONF_NET_NOBLOCK);
            if (iRet == 0)
            {
                cc = conf_create_connection(tblName, &net);
                if (!cc)
                {
                    conf_net_close_conn(&cc->net);
                    free(req);
                    return -1;
                }

                iRet = conf_net_write_bytes(&cc->net, req, reqLen);
                if (iRet == 0)
                {
                    iRet = conf_management_add_conn(cc);
                    if (iRet == -1)
                    {
                        conf_free_conn(cc);
                        free(req);
                        return -1;
                    }

                    iRet = aeCreateFileEvent(g_confMgr.el, cc->net.fd, AE_READABLE, conf_aehandler_read_response,cc);
                    if (iRet == AE_ERR)
                    {
                        conf_management_remove_conn(cc, 1);
                        free(req);
                        return -1;
                    }

                    free(req);
                    return 0;
                }
            }
        }
    }

    free(req);
    return -1;
}
