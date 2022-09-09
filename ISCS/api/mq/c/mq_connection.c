
#include <string.h>
#include <stdlib.h>

#include "mq_constant.h"
#include "mq_priv.h"
#include "mq_cmn_def.h"

static MQPooledConnectionFactory g_connFactory;

static int mq_conn_init_hosts();

//hosts: MQ服务器Protocol://IP:Port集合，以“；”隔开.
//如tcp://192.168.1.120:6378;upd://192.168.1.130:6378
int mq_conn_init(){
    return mq_conn_init_hosts();
}

int mq_conn_init_hosts()
{
    sys_conf_mq_domain* pdomain = get_mq_conf();
    sys_conf_mq_server* pserver = pdomain->first;

    c_vector_create(&g_connFactory.mq_hosts, NULL);
    do{
        MQHost* host = malloc(sizeof(MQHost));
        memset(host, 0, sizeof(MQHost));
        strncpy(host->protocol, pserver->schema, MQ_PROTOCOL_SIZE);
        strncpy(host->ip[0], pserver->ip_1, MQ_IP_SIZE);
        strncpy(host->ip[1], pserver->ip_2, MQ_IP_SIZE);
        host->port = pserver->port;
        host->poolsize = pserver->pool_size;
        host->timeout = pserver->timeout;
        host->beat_internal = pserver->beat_internal;

        strncpy(host->user, pserver->username, MQ_USER_SIZE);
        strncpy(host->passwd, pserver->password, MQ_PASSWD_SIZE);
        
        c_vector_push_back(&g_connFactory.mq_hosts, host);
        pserver = pserver->next;
        
        MQ_LOG_INFO("mq_conn_init_hosts, host information retrieved: protocol(%s), ip1(%s), ip2(%s), port(%d), poolsize(%d), timeout(%d), beat_internal(%d)",
                                                                    host->protocol, host->ip[0], host->ip[1], host->port,
                                                                    host->poolsize, host->timeout, host->beat_internal);
    }while(pserver);

    return MQ_OK;
}


static MQConnection* mq_conn_create(const MQHost* host, int isSync)
{
    MQConnection* pConn = malloc(sizeof(MQConnection));
    if (pConn != NULL)
    {
        memcpy(&pConn->host, host, sizeof(MQHost));

        pConn->heartbeat = MQ_HEART_BEAT_DEFAULT;

        if(isSync > 0)
        {
            // connect to redis server
            pConn->conn_type = 1; //synchronized
            for(int i = 0; i < MQ_NUM_HOST_INTERFACES; i ++)
            {
                if( (strcmp(host->protocol, "udp") == 0) || (strcmp(host->protocol, "UDP") == 0) ){
                    pConn->redisClient = redisConnectUdp(host->ip[i], host->port);
                }
                else {
                    pConn->redisClient = redisConnect(host->ip[i], host->port);
                }
                
                if(pConn->redisClient->err ==0)
                {
                    MQ_LOG_INFO("Redis Client is created on ip: %s, port: %d, index: %d.", host->ip[i], host->port, i);
                    break;
                }
                else
                {
                    MQ_LOG_WARN("Redis Client fails on ip: %s, port: %d, index: %d, error: %s.", host->ip[i], host->port, i, pConn->redisClient->errstr);
                }
            }
            pConn->redisAsyncClient = NULL;
            
            if(pConn->redisClient->connection_type != REDIS_CONN_UDP){
                redisEnableKeepAlive(pConn->redisClient);
            }
            
            // auth the user&passwd
            // TODO: reject connectin if failed
            redisReply* reply = (redisReply *)redisCommand(
                        pConn->redisClient, "%s %s:%s",
                        MQ_AUTH, pConn->host.user, pConn->host.passwd);
            if(reply) freeReplyObject(reply);
        }
        else
        {
            // connect to redis server
            pConn->conn_type = 0; //async
            pConn->redisClient = NULL;
            
            //pConn->redisAsyncClient = redisAsyncConnect(host->ip, host->port);
            for(int i = 0; i < MQ_NUM_HOST_INTERFACES; i ++)
            {
                if( (strcmp(host->protocol, "udp") == 0) || (strcmp(host->protocol, "UDP") == 0) ){
                    pConn->redisAsyncClient = redisAsyncConnectUdp(host->ip[i], host->port);
                }
                else{
                    pConn->redisAsyncClient = redisAsyncConnect(host->ip[i], host->port);
                }
                
                if(pConn->redisAsyncClient->err ==0)
                {
                    MQ_LOG_INFO("Redis Async Client is created on ip: %s, port: %d, index: %d.", host->ip[i], host->port, i);
                    break;
                }
                else
                {
                    MQ_LOG_WARN("Redis Async Client fails on ip: %s, port: %d, index: %d, error: %s.", host->ip[i], host->port, i, pConn->redisAsyncClient->errstr);
                }
            }
            //redisEnableKeepAlive(pConn->redisClient);

            // auth the user&passwd
            // TODO: reject connectin if failed
            //redisReply* reply = (redisReply *)redisCommand(
            //            pConn->redisClient, "%s %s:%s",
            //            MQ_AUTH, pConn->host.user, pConn->host.passwd);
            //if(reply) freeReplyObject(reply);
        }
    }

    return pConn;
}

int mq_conn_destroy(MQConnection* conn)
{
    // close the connection
    if(conn){
        MQ_LOG_INFO("mq_conn_destroy: Closing connection on %s | %s, port: %d.", conn->host.ip[0], conn->host.ip[1], conn->host.port);
        
        if(conn->redisClient)  redisFree(conn->redisClient);
        if(conn->redisAsyncClient){
            redisAsyncDisconnect(conn->redisAsyncClient);
        }

        free(conn);
    }

    return MQ_OK;
}

int mq_conn_release(){

    MQ_LOG_INFO("mq_conn_release: all resource will be released.");
    
    c_iterator iter = c_vector_begin(&g_connFactory.mq_hosts);
    c_iterator last = c_vector_end(&g_connFactory.mq_hosts);
    for(;!ITER_EQUAL(iter, last); ITER_INC(iter)){
        if(ITER_REF(iter)){
            MQHost* host = (MQHost*)ITER_REF(iter);
            free(host);
        }
    }
    c_vector_destroy(&g_connFactory.mq_hosts);


    return MQ_OK;
}

int mq_conn_recover(MQConnection* conn) {
    int result = MQ_OK;

    if(!conn) return MQ_ILLEGAL_PARAM;
    return result;
}

int mq_conn_master_index()
{
    if( (g_connFactory.master_host_index < 0) || (g_connFactory.master_host_index >= c_vector_size(&g_connFactory.mq_hosts)) )
    {
        g_connFactory.master_host_index = 0;
    }
    
    return g_connFactory.master_host_index;
    
}

c_vector mq_conn_hosts()
{
    return g_connFactory.mq_hosts;
}


MQConnection* mq_conn_create_connection(int indexHost, int isSync)
{
    MQConnection* conn = NULL;
    int sizeHost = c_vector_size(&g_connFactory.mq_hosts);
    if( (indexHost < 0) || (indexHost >= sizeHost) )
    {
        indexHost = 0;
    }
    MQHost * host = c_vector_at(&g_connFactory.mq_hosts, indexHost);
    
    conn = mq_conn_create(host, isSync);
    
    MQ_LOG_INFO("mq_conn_create_connection: index:%d, isSync:%d.", indexHost, isSync);
    
    return conn;
}
