#include "pthread.h"
#include "mq_priv.h"
#include <unistd.h>
#include "mq_threads_send.h"
#include "mq_cmn_def.h"
#include "c_set.h"
#include "mq_map_utilities.h"

static mq_buffer_list_t*    __threads_queue_send_buffer_list;
static c_vector             __vector_threads_queue_send;

static c_set                __g_set_all_queues;
static cmn_lock*            __g_lock_set_all_queues;

static bool __g_queue_send_initialized = false;
static bool __g_stop_queue_send_flag = false;

void init_allqueuesets();
bool check_2nd_add_allqueuesets(const char* queuename);
void release_allqueuesets();

void mq_threads_send_init(int thrdNum)
{
    if(__g_queue_send_initialized == false)
    {
        c_vector_create(&__vector_threads_queue_send, NULL);
        
        __threads_queue_send_buffer_list = mq_buffer_list_create(mq_config_get_max_buffer_size_per_queue());
    
        for(int i=0; i < thrdNum; i ++)
        {
            threads_send_param_t* param = malloc(sizeof(threads_send_param_t));
            param->index = i;
            param->pthrd = malloc(sizeof(pthread_t));
            param->session = mq_sessn_open(1);
            param->list_send_buffer_message = __threads_queue_send_buffer_list;
            
            c_vector_push_back(&__vector_threads_queue_send, param);
        }
        
        __g_queue_send_initialized = true;
         __g_stop_queue_send_flag = false;
		 
        init_allqueuesets();
        
        MQ_LOG_INFO("mq_threads_send_init: done.");
    }
}

void mq_threads_send_start()
{
    c_iterator iter;
    c_iterator st_ite = c_vector_begin(&__vector_threads_queue_send);
    c_iterator ed_ite = c_vector_end(&__vector_threads_queue_send);
    for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
    {
        if(ITER_REF(iter))
        {
            threads_send_param_t* prm = (threads_send_param_t*)ITER_REF(iter);
            pthread_create(prm->pthrd, NULL, mq_threads_send_run, (void *) prm);
            
            MQ_LOG_INFO("mq_threads_send_start: start thread(%d).", prm->index);
        }
    }
}

void* mq_threads_send_run(void* ptr)
{
    if(ptr)
    {
        threads_send_param_t* prm = (threads_send_param_t*)ptr;
        MQ_LOG_INFO("mq_threads_send_run: thread (%d) is running.", prm->index);
        
        while( (__g_stop_queue_send_flag == false) || (mq_buffer_list_is_empty(prm->list_send_buffer_message) == 0) )
        {
            data_send_entry* data = (data_send_entry*)mq_buffer_list_get_item(prm->list_send_buffer_message, 1);
            //Send data via session 
            if(data)
            {
                int adjustedpriority = mq_adjust_priority_with_timestamp(data->queueOrTopic, data->priority);
                
                if(check_2nd_add_allqueuesets(data->queueOrTopic) == true){
                    MQ_LOG_DEBUG("mq_threads_send_run: NEW queue detected (%s), register to mq server admin queue (%s).", data->queueOrTopic, NAME_QUEUE_ADMIN);
                    mq_sessn_send(prm->session,  MQ_ADD_QUEUE_ALLQUEUE, NAME_QUEUE_ALLQUEUE, data->queueOrTopic, MQ_MSG_TIMEOUT_DEFAULT, MQ_MSG_PRIORITY_DEFAULT);
                }
                
                mq_sessn_send(prm->session,  data->opt, data->queueOrTopic, data->message, data->timeout, adjustedpriority);

                MQ_LOG_INFO("mq_threads_send_run: command(%s %s %s) in loop thread(%d).", data->opt, data->queueOrTopic, data->message, prm->index);
                
                free(data);
            }
            else
            {
                MQ_LOG_ERR("mq_threads_send_run: empty data entry in loop thread(%d).", prm->index);
            }
        }
        
        MQ_LOG_INFO("mq_threads_send_run: thread (%d) run finished.", prm->index);
    }
    else
    {
        MQ_LOG_ERR("mq_threads_send_run: invalid thread parameter.");
    }
    
    return NULL;
}



void mq_threads_send_stop()
{
    if(__g_queue_send_initialized == true)
    {
        __g_stop_queue_send_flag = true;
        
        c_iterator iter, st_ite, ed_ite;
        st_ite = c_vector_begin(&__vector_threads_queue_send);
        ed_ite = c_vector_end(&__vector_threads_queue_send);
        for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
        {
            if(ITER_REF(iter))
            {
                threads_send_param_t* prm = (threads_send_param_t*)ITER_REF(iter);
				mq_buffer_list_unblock(prm->list_send_buffer_message);
				pthread_join(*prm->pthrd, NULL);
                free(prm->pthrd);
                mq_sessn_close(prm->session);
				mq_buffer_list_release(prm->list_send_buffer_message);
                free(prm);
            }
        }

        release_allqueuesets();
        
        c_vector_destroy(&__vector_threads_queue_send);

        MQ_LOG_INFO("mq_threads_send_stop: done.");    
    }
	__g_queue_send_initialized = false;
	__threads_queue_send_buffer_list = NULL;
}

int mq_threads_send_add_entry(const char* domain, long lmsgId, const char* opt, const char* topic, const char* payload, const long timeout, const int priority)
{
	int iret = MQ_OK;
    if(__threads_queue_send_buffer_list)
    {
        data_send_entry* pData = NULL;
        int total_size = sizeof(data_send_entry) + strlen(payload);
        pData = (data_send_entry*)malloc(total_size);
        memset(pData, 0, total_size);
        
        strncpy(pData->opt, opt, MQ_OPTION_SIZE);    
        strncpy(pData->domain, domain, MQ_DOMAIN_SIZE);
        strncpy(pData->queueOrTopic, topic, MQ_TOPIC_SIZE);
        strncpy(pData->message, payload, strlen(payload) + 1);
        pData->timeout = timeout;
        pData->messageid = lmsgId;
        pData->priority = priority;
        
        MQ_LOG_INFO("mq_threads_queue_send_add_entry: one message added to sending queue. domain(%s), idMsg(%d), opt(%s), queue(%s), message(%s)",
                domain, lmsgId, opt, topic, payload);
        mq_buffer_list_add_item(__threads_queue_send_buffer_list, pData);
    }
    else
    {
		iret = MQ_NOT_INIT;
        MQ_LOG_ERR("mq_threads_queue_send_add_entry: __threads_queue_send_buffer_list is not initialized.");
    }
    return iret;
}

void mq_threads_send_join()
{
    c_iterator iter;
    c_iterator st_ite = c_vector_begin(&__vector_threads_queue_send);
    c_iterator ed_ite = c_vector_end(&__vector_threads_queue_send);
    for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
    {
        if(ITER_REF(iter))
        {
            threads_send_param_t* prm = (threads_send_param_t*)ITER_REF(iter);
            pthread_join(*prm->pthrd, NULL);
        }
    }    
}

static c_set                __g_set_all_queues;
static cmn_lock*            __g_lock_set_all_queues;
void init_allqueuesets(){
    if(__g_lock_set_all_queues == NULL){
        __g_lock_set_all_queues = cmn_lock_create();
        c_set_create(&__g_set_all_queues, _stringkey_comparer);
    }
}

bool check_2nd_add_allqueuesets(const char* queuename){
    bool bret = false;
    if(mq_is_queue(queuename)) {
        if(__g_lock_set_all_queues){
            struct mq_string_key_t searchkey;
            mq_map_string_list_reform_temp_key(&searchkey, queuename);


            cmn_lock_lock(__g_lock_set_all_queues);

            c_iterator target, set_end;	
            set_end = c_set_end(&__g_set_all_queues);
            target = c_set_find(&__g_set_all_queues, &searchkey);
                
            if(ITER_EQUAL(set_end, target)) {
                mq_string_key_t* newkey = mq_map_string_list_create_key(queuename);
                c_set_insert(&__g_set_all_queues, newkey);
                bret = true;
            }
            
            cmn_lock_unlock(__g_lock_set_all_queues);
        }
    }
    return bret;
}

void release_allqueuesets(){
    if(__g_lock_set_all_queues){
        cmn_lock_lock(__g_lock_set_all_queues);

        c_iterator iter = c_set_begin(&__g_set_all_queues);
        c_iterator end = c_set_end(&__g_set_all_queues);

        while(!ITER_EQUAL(iter, end))
        {
            mq_string_key_t* key = (mq_string_key_t*)ITER_REF(iter);
            if(key) free(key);
            
            c_set_erase(&__g_set_all_queues, iter);
            
            iter = c_set_begin(&__g_set_all_queues);
            end = c_set_end(&__g_set_all_queues);
        }
        
        cmn_lock_unlock(__g_lock_set_all_queues);
        cmn_lock_release(__g_lock_set_all_queues);
    }
}
