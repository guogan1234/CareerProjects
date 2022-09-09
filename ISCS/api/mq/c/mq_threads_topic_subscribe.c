#include "pthread.h"
#include "mq_priv.h"
#include <unistd.h>
#include "adapters/ae.h"
#include "mq_threads_topic_subscribe.h"
#include "mq_threads_event_processor.h"
#include "mq_cmn_def.h"

// subscribe topic list, each item represent a structure of  data_topic_subscribe_entry
static mq_buffer_list_t*    __threads_subscribe_topic_list;

//topc subscribing thread vector, only one thread is allowed due to complexity of subscribe list, may extend to multiple thread if required.
static c_vector             	__vector_threads_subscribe;

//topic subscribing thread initialization flag
static bool __g_subscribe_initialized = false;

//topic subscribing thread stop flag
static bool __g_stop_subscribe_flag = false;

//aeEventLoop for asynchronized redis commend
static struct aeEventLoop* __g_event_loop = NULL;

void mq_threads_topic_subscribe_init()
{
    if(__g_subscribe_initialized == false)
    {
        c_vector_create(&__vector_threads_subscribe, NULL);
        __threads_subscribe_topic_list = mq_buffer_list_create(0);
    
        for(int i=0; i < 1; i ++)  //Only one subscribe thread is supported at this moment
        {
            threads_topic_subscribe_param_t* param = malloc(sizeof(threads_topic_subscribe_param_t));
            param->index = i;
            param->pthrd = malloc(sizeof(pthread_t));
            param->session = mq_sessn_open(0);
            param->list_buffer_message = __threads_subscribe_topic_list;

            c_vector_push_back(&__vector_threads_subscribe, param);
        }
        __g_subscribe_initialized = true;
		__g_stop_subscribe_flag = false;
        MQ_LOG_INFO("mq_threads_topic_subscribe_init: done.");
    }
}

int mq_threads_topic_subscribe_start()
{
	int iret = MQ_NOT_INIT;
	if(__g_subscribe_initialized == true){
		c_iterator iter;
		c_iterator st_ite = c_vector_begin(&__vector_threads_subscribe);
		c_iterator ed_ite = c_vector_end(&__vector_threads_subscribe);
		for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
		{
			if(ITER_REF(iter))
			{
				threads_topic_subscribe_param_t* prm = (threads_topic_subscribe_param_t*)ITER_REF(iter);
				pthread_create(prm->pthrd, NULL, mq_threads_topic_subscribe_run, (void *) prm);
				MQ_LOG_INFO("mq_threads_topic_subscribe_start: started thread (%d).", prm->index);
			}
		}
		iret = MQ_OK;
	}
	return iret;
}

void* mq_threads_topic_subscribe_run(void* ptr)
{
    if(ptr)
    {
        threads_topic_subscribe_param_t* prm = (threads_topic_subscribe_param_t*)ptr;

#ifdef _WIN32        
        __g_event_loop = aeCreateEventLoop(512);
#else
        __g_event_loop = aeCreateEventLoop(64);
#endif
        MQ_LOG_INFO("mq_threads_topic_subscribe_run: thread (%d) is running.", prm->index);

        mq_sessn_attach_event_loop(prm->session, __g_event_loop);
        MQ_LOG_DEBUG("mq_threads_topic_subscribe_run: thread (%d) is with event loop attached.", prm->index);
        
		mq_sessn_async_subscribe(prm->session, MQ_SUBSCRIBE, "", NAME_TOPIC_ADMIN,  NULL);
		MQ_LOG_DEBUG("mq_threads_topic_subscribe_run: thread (%d) has subscribed to admin topic (%s).", prm->index, NAME_TOPIC_ADMIN);
		
        while(__g_stop_subscribe_flag == false)
        {
			if( __g_subscribe_initialized == false)
			{
					MQ_LOG_INFO("mq_threads_topic_subscribe_run: thread was not initialized, will retry again after %d second.", MQ_INTERVAL_NOT_INITIALIZED);
					sleep(MQ_INTERVAL_NOT_INITIALIZED);
					continue;
			}
			else if(mq_sessn_is_connection_valid(prm->session, 0) != MQ_OK){
				MQ_LOG_INFO("mq_threads_topic_subscribe_run: redis session object is invalid, need fix the invalid session issue...");
				sleep(MQ_INTERVAL_NOT_INITIALIZED);
				continue;
			}
			
            data_topic_subscribe_entry* data = (data_topic_subscribe_entry*)mq_buffer_list_get_item(prm->list_buffer_message, 0);
            if(data)
            {
                MQ_LOG_DEBUG("mq_threads_topic_subscribe_run: subscribe to domain(%s), opt(%s), topic(%s) in thread(%d).",
                                    data->domain, data->opt, data->queueOrTopic, prm->index);
                mq_sessn_async_subscribe(prm->session, data->opt, data->domain, data->queueOrTopic, data);
            }
            else
            {
                MQ_LOG_DEBUG("mq_threads_topic_subscribe_run: not found relevant topic subscribe item.");
            }
            
			if( mq_buffer_list_is_empty(prm->list_buffer_message) == 1 )
            {
                aeMain(__g_event_loop);
                MQ_LOG_DEBUG("mq_threads_topic_subscribe_run: aeMain event loop exited.");            
            }
        }
        
        MQ_LOG_INFO("mq_threads_topic_subscribe_run: thread (%d) run finished.", prm->index);
    }
    else
    {
        MQ_LOG_ERR("mq_threads_topic_subscribe_run: invalid thread parameter.");
    }
    
    return NULL;
}



void mq_threads_topic_subscribe_stop()
{
    if(__g_subscribe_initialized == true)
    {
		__g_subscribe_initialized = false;
        __g_stop_subscribe_flag = true;

        c_iterator iter;
        c_iterator st_ite = c_vector_begin(&__vector_threads_subscribe);
        c_iterator ed_ite = c_vector_end(&__vector_threads_subscribe);
        for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
        {
            if(ITER_REF(iter))
            {
                threads_topic_subscribe_param_t* prm = (threads_topic_subscribe_param_t*)ITER_REF(iter);
				aeStop(__g_event_loop);
				mq_pubsub_trigger_receive_admintopicqueue("", MQ_TQTYPE_TOPIC);
				pthread_join(*prm->pthrd, NULL);
				free(prm->pthrd);
				mq_sessn_close(prm->session);
				free(prm);
            }
        }
		c_vector_destroy(&__vector_threads_subscribe);
        
        do{
            data_topic_subscribe_entry* psubscribeentry = mq_buffer_list_get_item(__threads_subscribe_topic_list, 0); 		//get entry with non-block
            if(psubscribeentry != NULL) free(psubscribeentry);
        }while(!mq_buffer_list_is_empty(__threads_subscribe_topic_list));
        mq_buffer_list_release(__threads_subscribe_topic_list);
		__threads_subscribe_topic_list = NULL;
        
        __g_subscribe_initialized = false;
        MQ_LOG_INFO("mq_threads_topic_subscribe_stop: done.");
    }
}

void mq_threads_topic_subscribe_join()
{
    c_iterator iter;
    c_iterator st_ite = c_vector_begin(&__vector_threads_subscribe);
    c_iterator ed_ite = c_vector_end(&__vector_threads_subscribe);
    for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
    {
        if(ITER_REF(iter))
        {
            threads_topic_subscribe_param_t* prm = (threads_topic_subscribe_param_t*)ITER_REF(iter);
            pthread_join(*prm->pthrd, NULL);
        }
    }
}

int mq_threads_topic_subscribe_add_entry(const char* opt, const char * domain, const char* topic, const MessageListener listener)
{
    if(__threads_subscribe_topic_list)
    {
        data_topic_subscribe_entry* pData = NULL;
        int total_size = sizeof(data_topic_subscribe_entry);
        pData = (data_topic_subscribe_entry*)malloc(total_size);
        memset(pData, 0, total_size);
        
        strncpy(pData->opt, opt, MQ_OPTION_SIZE);
        strncpy(pData->domain, domain, MQ_DOMAIN_SIZE);
        strncpy(pData->queueOrTopic, topic, MQ_TOPIC_SIZE);
        pData->messageHandler = listener;
        
        MQ_LOG_INFO("mq_threads_topic_subscribe_add_entry: added subscribe for domain(%s), opt(%s), topic(%s).",
                    domain, opt, topic);
        
        mq_buffer_list_add_item(__threads_subscribe_topic_list, pData);
        
        aeStop(__g_event_loop);
    }
    else
    {
        MQ_LOG_ERR("mq_threads_topic_subscribe_add_entry: __threads_subscribe_buffer_list is not initialized.");
    }
    return MQ_OK;
}