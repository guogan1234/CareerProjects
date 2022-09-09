// 维护心跳，选择Master服务,断开重连
#include <unistd.h>
#include <pthread.h>

#include "cmn_sched.h"
#include "cmn_threadpool.h"
#include "c_list.h"

#include "mq_constant.h"
#include "mq_priv.h"
#include "mq_cmn_def.h"

static pthread_mutex_t g_sessionLock = PTHREAD_MUTEX_INITIALIZER;

static c_list __g_open_sessions;

static void mq_sessn_pingpong();
static void mq_sessn_handler(cmn_sched_task_t tid, void* param);
static int mq_sessn_subscribe_recover(MQSafeSession* session);
static MQSafeSession* mq_session_create(int isSync);

int mq_sessn_init(){
    c_list_create(&__g_open_sessions, NULL);
    // Ping/Pong First, Get the master mq connection first
    mq_sessn_pingpong();
    
    MQ_LOG_INFO("mq_sessn_init: sessions initiaized.");

    return MQ_OK;
}

int mq_sessn_release(){

    pthread_mutex_lock(&g_sessionLock);
    {
        c_iterator last = c_list_end(&__g_open_sessions);
        for(c_iterator iter = c_list_begin(&__g_open_sessions); !ITER_EQUAL(iter, last); ITER_INC(iter)) {
            MQSafeSession* p = (MQSafeSession*)(ITER_REF(iter));
            if(p) 
            {
                cmn_lock_release(p->session_lock);
                free(p);
            }
        }
        c_list_destroy(&__g_open_sessions);
    }
    pthread_mutex_unlock(&g_sessionLock);
    
    MQ_LOG_INFO("mq_sessn_release: all session resources releases.");

    return MQ_OK;
}

//get idle session
MQSafeSession* mq_sessn_open(int isSync){
    pthread_mutex_lock(&g_sessionLock);
    
    MQSafeSession* session = mq_session_create(isSync);
    if(session != NULL)
    {
        c_list_push_back(&__g_open_sessions, session);
    }
    
    pthread_mutex_unlock(&g_sessionLock);
    
    MQ_LOG_INFO("mq_sessn_open: new session opened for use (isSync=%d).", isSync);
    
    return session;
}

int mq_sessn_is_connection_valid(MQSafeSession* session, int isSync){
	int iret = MQ_ERROR;
	if(isSync  > 0){
		redisContext* rediscli = (redisContext*)mq_session_get_master_connection(session, 1);
        if( (rediscli) && (rediscli->err==REDIS_OK) ) iret = MQ_OK;
	}
	else{
		redisAsyncContext* async_conn = mq_session_get_master_connection(session, 0);
		if( (async_conn) && (async_conn->err==REDIS_OK) ) iret = MQ_OK;
	}
	
	return iret;
}

//release the session
int mq_sessn_close(MQSafeSession* session){
    if(session) {
        pthread_mutex_lock(&g_sessionLock);
        c_list_remove(&__g_open_sessions, session);
        
        
        c_iterator iter;
		c_iterator st_ite = c_vector_begin(&session->session_connections);
		c_iterator ed_ite = c_vector_end(&session->session_connections);
		for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter)) {
            MQConnection* oconn = (MQConnection*)ITER_REF(iter);
            if(oconn) mq_conn_destroy(oconn);
        }
        cmn_lock_release(session->session_lock);
        c_vector_destroy(&session->session_connections);
        
        pthread_mutex_unlock(&g_sessionLock);
        
        free(session);

        MQ_LOG_INFO("mq_sessn_close: implementation to be updated...");
        return MQ_OK;
    }

    return MQ_ILLEGAL_PARAM;
}

int mq_sessn_send(MQSafeSession* session, const char* opt, const char* queueOrTopic, const char* message, const int timeout, const int parm1){
    int result = MQ_OK;
    //pthread_mutex_lock(&session->monoLock);
    {
        redisContext* rediscli = (redisContext*)mq_session_get_master_connection(session, 1);
        if( (rediscli) && (rediscli->err==REDIS_OK) )
        {
            redisReply *reply = NULL;
            if(strcmp(opt, MQ_QUEUE_PSEND) ==0)
            {
                MQ_LOG_DEBUG("mq_sessn_send: command sent: %s %s %d %s", opt, queueOrTopic, parm1, message);
                reply = redisCommand(rediscli, "%s %s %d %s", opt, queueOrTopic, parm1, message);
                //redisReply *reply = redisCommand(rediscli, "%s %s %d %s", opt, queueOrTopic, parm1, "olleh");
            }
            else{
                MQ_LOG_DEBUG("mq_sessn_send: command sent: %s %s %s", opt, queueOrTopic, message);
                reply = redisCommand(rediscli, "%s %s %s", opt, queueOrTopic, message);
            }
            
            MQ_LOG_DEBUG("mq_sessn_send: command reponse: %s %s, reply: %s", opt, queueOrTopic, (reply==NULL ? "NULL" : reply->str));
            if(reply)
            {
                freeReplyObject(reply);
            } 
            else 
            {
                result |= MQ_CONN_BREAK;
            }
        }
        else
        {
            MQ_LOG_ERR("mq_sessn_send: invalid redisContext for command sending. error: %s", (rediscli==NULL ? "rediscli==NULL" : rediscli->errstr));
        }
    }
    //pthread_mutex_unlock(&session->monoLock);
    return result;
}

void* mq_sessn_execute(MQSafeSession* session, const char* opt, const char* queueOrTopic, const int timeout)
{
    char* result = "";
    cmn_lock_lock(session->session_lock);
    redisContext* rediscli = (redisContext*)mq_session_get_master_connection(session, 1);
    if( (rediscli) && (rediscli->err==REDIS_OK) )
    {
        char cmd_buff[1024];
        memset(cmd_buff, 0, sizeof(1024));
        snprintf(cmd_buff, 1023, "%s %s %d", opt, queueOrTopic, timeout);
        
        MQ_LOG_DEBUG("mq_sessn_execute: command sent: %s",cmd_buff);
        redisReply *reply = redisCommand(rediscli, cmd_buff);
        
        if ( reply && reply->type == REDIS_REPLY_ARRAY && (reply->elements == 2 || reply->elements == 3) )
        {
            int ilen = strlen(reply->element[1]->str) + strlen(reply->element[0]->str) + 2;
            char* res_buff = malloc(ilen);
            memset(res_buff, 0, ilen);
            snprintf(res_buff, ilen, "%s|%s", reply->element[1]->str, reply->element[0]->str);
            result = res_buff;
            
            MQ_LOG_DEBUG("mq_sessn_execute: command response: %s, reply: %s", cmd_buff, result);
        }
        else if(reply != NULL)
        {
            MQ_LOG_DEBUG("mq_sessn_execute: command reponse: %s, reply: %s", cmd_buff, (reply==NULL ? "NULL" : reply->str));
        }
        
        if(reply)
        {
            freeReplyObject(reply);
        } 
    }
    else
    {
        MQ_LOG_ERR("mq_sessn_execute: invalid redisContext for command execution. error: %s", (rediscli==NULL ? "rediscli==NULL" : rediscli->errstr));
    }
    
    cmn_lock_unlock(session->session_lock);
    return result;
}

#ifdef MQ_SESSN_ENABLED
//support in 2.0
static int mq_sessn_subscribe_recover(MQSafeSession* session) {
    int result = MQ_OK;
    //pthread_mutex_lock(&session->monoLock);
    //前提：subscribe状态下是没有心跳
    //1. 如果简单断连，eg拔client端网线，则重连+再次sub
    //2. 如果主从切换，eg关闭master-sever或者收到切换信号，则切m/s+再次sub
    //pthread_mutex_unlock(&session->monoLock);

    return result;
}
#endif

//static bool mq_sessn_subscribed(MQSafeSession* session) {
//    return (session && session->subCallback);
//}

static void mq_sessn_pingpong(){
    // simulator, all are master as it running in single server, remove these code in 2.0
    //pthread_mutex_lock(&g_sessionLock);
    //c_vector connPool = mq_conn_pool();
    //for(int i = 0; i < c_vector_size(&connPool); i++) {
    //    MQSafeSession* sess = malloc(sizeof(MQSafeSession));
    //    if(sess) {
    //        sess->masterConn = c_vector_at(&connPool, i);
    //        sess->slaveConn = NULL;
    //        pthread_mutex_init(&sess->monoLock, NULL);
    //        c_list_push_back(&g_idleSessions, sess);
    //    }
    //}
    //pthread_mutex_unlock(&g_sessionLock);

    //todo: add heatbeat, too complex
    //slave connection, if断开then重连
    //master connections：
    // 1.if断开then重连，2.if主从切换then交换session内的连接指针，
    // 3. 在subscribe状态下,发生了上述1,2两条，通知pubsub重新注册
}

#ifdef MQ_SESSN_ENABLED
static void mq_sessn_handler(cmn_sched_task_t tid, void* param)
{
    // do ping/pong
    mq_sessn_pingpong();
}
#endif

static MQSafeSession* mq_session_create(int isSync)
{
    MQSafeSession* sess = malloc(sizeof(MQSafeSession));
    if(sess) {
        sess->session_type = isSync;
        c_vector hosts = mq_conn_hosts();
        c_vector_create(&sess->session_connections, NULL);
        for(int i = 0; i < c_vector_size(&hosts); i++) 
        {
            MQConnection* oconn = mq_conn_create_connection(i, isSync);
            c_vector_push_back(&sess->session_connections, oconn);
        }
        sess->session_lock = cmn_lock_create();
        
        MQ_LOG_DEBUG("mq_session_create: new session created.");
    }
    return sess;
}

void * mq_session_get_master_connection(MQSafeSession* sess, int isSync)
{
    void * retVal = NULL;
    int ind = mq_conn_master_index();
    if( (ind >= 0) && (ind < c_vector_size(&sess->session_connections)) )
    {
        MQConnection * oconn = c_vector_at(&sess->session_connections, ind);
        
        if( (isSync) && (oconn->conn_type>0) )
        {
            retVal = oconn->redisClient;
        }
        else if((isSync == 0) && (oconn->conn_type == 0))
        {
            retVal = oconn->redisAsyncClient;
        }
    }
    
    MQ_LOG_DEBUG("mq_session_get_master_connection: isSync(%d), RedisClient(%s).", isSync, (retVal == NULL ? "NULL" : "Valid"));
    return retVal;
}
