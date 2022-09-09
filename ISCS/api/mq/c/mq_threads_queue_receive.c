#include "pthread.h"
#include "mq_priv.h"
#include "mq_constant.h"
#include <unistd.h>
#include "c_list.h"
#include "c_iterator.h"
#include "mq_threads_queue_receive.h"
#include "mq_threads_event_processor.h"
#include "mq_cmn_def.h"
#include "cmn_lock.h"

// subscribe queue list, each item represent a structure of data_queue_receive_subscribe_entry
static mq_buffer_list_t*    __threads_receive_queue_subscribe_list;

//queue recieving thread vector, only one receiving thread is allowed due to complexity of subscribe list, may extend to multiple thread if required.
static c_vector             __vector_threads_receive;

//queue receiving thread initialization flag
static bool __g_receive_initialized = false;

//queue receiving thread stop flag
static bool __g_stop_receive_flag = false;

/* 
 *  his section are data structure and functions for managing subscriptions
 *  BEGIN
 */
 // structure of the subscribing item
typedef struct queue_subscribe_name_entry
{
    int                 priority;
    char              queueOrTopic[MQ_TOPIC_SIZE];
} queue_subscribe_name_entry;

//list of subscribing list, each item point to a structure of queue_subscribe_name_entry
static c_list           __g_list_subscribe_names;

// add a new queue with a priority to the subscribe list 
static int mq_add_topic_to_string_subscribe_list(const char* topic, int priority);

// remove a queue from the subscribe list
static int mq_remove_topic_from_string_subscribe_list(const char* topic);

// the string buffer for subscribe list
static char __g_string_queue_subscribe_list[MQ_QUEUE_SUBSCRIBE_LIST_SIZE];

// lock object for all operation on the subscribe list __g_list_subscribe_names
static cmn_lock*  __g_lock_queue_subscribe_list;

// comparer for the list to sort the subscribe list 
static int _queue_priority_comparer(void * x, void * y);

/* 
 *  END
 *  this section are data structure and functions for managing subscriptions
 */

// function to check if the psubscribeentry (data_queue_receive_subscribe_entry) is with queue name as strQName
static int mq_is_subscribe_entry_with_name(void* psubscribeentry, void* strQName);


/* 
 *  Implementation of the mq thread queue receive
 */

// thread for queue receive initialization 
void mq_threads_queue_receive_init()
{
    if(__g_receive_initialized == false)
    {
        c_vector_create(&__vector_threads_receive, NULL);
        __threads_receive_queue_subscribe_list = mq_buffer_list_create(0);
        __g_lock_queue_subscribe_list = cmn_lock_create();
    
        for(int i=0; i < 1; i ++)  //Only one subscribe thread is supported at this moment
        {
            threads_queue_receive_param_t* param = malloc(sizeof(threads_queue_receive_param_t));
            param->index = i;
            param->pthrd = malloc(sizeof(pthread_t));
            param->session = mq_sessn_open(1);
            param->list_receive_queue_subscribes = __threads_receive_queue_subscribe_list;
            
            c_vector_push_back(&__vector_threads_receive, param);
        }
        
        memset(__g_string_queue_subscribe_list, 0, MQ_QUEUE_SUBSCRIBE_LIST_SIZE);
        c_list_create(&__g_list_subscribe_names, _queue_priority_comparer);
        
        __g_receive_initialized = true;
        __g_stop_receive_flag = false;
        MQ_LOG_INFO("mq_threads_queue_receive_init: done.");
    }
}

// start the processing threads
int mq_threads_queue_receive_start()
{
	int iret = MQ_NOT_INIT;
	if(__g_receive_initialized == true){
		c_iterator iter;
		c_iterator st_ite = c_vector_begin(&__vector_threads_receive);
		c_iterator ed_ite = c_vector_end(&__vector_threads_receive);
		for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
		{
			if(ITER_REF(iter))
			{
				threads_queue_receive_param_t* prm = (threads_queue_receive_param_t*)ITER_REF(iter);
				pthread_create(prm->pthrd, NULL, mq_threads_queue_receive_run, (void *) prm);
				MQ_LOG_INFO("mq_threads_queue_receive_start: started thread (%d).", prm->index);
			}
		}
		iret = MQ_OK;
	}
	return iret;
}

void* mq_threads_queue_receive_run(void* ptr)
{
    if(ptr)
    {
        threads_queue_receive_param_t* prm = (threads_queue_receive_param_t*)ptr;
        MQ_LOG_INFO("mq_threads_queue_receive_run: thread (%d) is running.", prm->index);
        while(__g_stop_receive_flag == false)
        {
			if( __g_receive_initialized == false)
			{
				MQ_LOG_INFO("mq_threads_queue_receive_run: thread was not initialized, will retry again after %d second.", MQ_INTERVAL_NOT_INITIALIZED);
				sleep(MQ_INTERVAL_NOT_INITIALIZED);
				continue;
			}
			else if(mq_sessn_is_connection_valid(prm->session, 1) != MQ_OK){
				MQ_LOG_INFO("mq_threads_queue_receive_run: redis session object is invalid, need fix the invalid session issue...");
				sleep(MQ_INTERVAL_NOT_INITIALIZED);
				continue;
			}
            //Read one message from the session.
            char * popt = (mq_config_priority_supported() > 0) ? MQ_QUEUE_PRECEIVE : MQ_QUEUE_RECEIVE;
            //char* strreply= mq_sessn_execute(prm->session, popt, mq_get_queue_names(), MQ_QUEUE_RECEIVE_TIMEOUT);
			char* strreply= mq_sessn_execute(prm->session, popt, mq_get_queue_names(), MQ_QUEUE_RECEIVE_TIMEOUT);
           
            if( (strreply) && (strlen(strreply)) )
            {
                MQ_LOG_DEBUG("mq_threads_queue_receive_run: mq_sessn_execute returned one message entry(%s).", strreply);
                char * pTopic = strrchr(strreply, '|');
                if(pTopic)
                {
                    *pTopic = 0;
                    pTopic ++;
                    if(strcmp(pTopic, NAME_QUEUE_ADMIN) == 0){
                        MQ_LOG_DEBUG("mq_threads_queue_receive_run: admin message received, empty loop.");
                    }
                    else{
                        data_queue_receive_subscribe_entry* pSubscribeEntry = mq_buffer_list_get_item_by_condition(prm->list_receive_queue_subscribes,
																											mq_is_subscribe_entry_with_name, pTopic);
                        if(pSubscribeEntry)
                        {
                            MQ_LOG_DEBUG("mq_threads_queue_receive_run: added one queue item to event process. domain(%s), queue(%s)", pSubscribeEntry->domain, pTopic);
                            int queuepriority = pSubscribeEntry->priority;
                            mq_threads_event_processor_add_task(pSubscribeEntry->domain, pTopic, strreply, strlen(strreply), pSubscribeEntry->messageHandler, &queuepriority);
                            
                            //Adjust queue priority
                            if(queuepriority != pSubscribeEntry->priority){
                                MQ_LOG_DEBUG("mq_threads_queue_receive_run: priority of queue (%s) is changed from (%d) to (%d).", pTopic, pSubscribeEntry->priority, queuepriority);
                                mq_set_queue_priority(pTopic, queuepriority);
                                pSubscribeEntry->priority = queuepriority;
                            }
                        }
                        else
                        {
							//TBD, IF A MESSAGE IS FOUND WITHOUT SUBSCRIBE ITEM, SHOULD RETURN BACK TO THE QUEUE
                            MQ_LOG_DEBUG("mq_threads_queue_receive_run: not found relevant queue subscribe item.");
                        }
                    }
                    free(strreply);
                }
                else
                {
                    MQ_LOG_DEBUG("mq_threads_queue_receive_run: invalid topic received.");
                }
            }
            else
            {
                MQ_LOG_DEBUG("mq_threads_queue_receive_run: empty message loop.");
                sleep(1);
            }
       }
	   
	   //if the receive thread stoped, but there are still data in the receiving buffer, the data should be put back to server
	   mq_threads_queue_restore_pending_data(prm->list_receive_queue_subscribes, prm->session);
       
       MQ_LOG_INFO("mq_threads_queue_receive_run: thread (%d) run finished.", prm->index);
    }
    else
    {
        MQ_LOG_ERR("mq_threads_queue_receive_run: invalid thread parameter.");
    }
    
    return NULL;
}

void mq_threads_queue_restore_pending_data(mq_buffer_list_t* subscribeList, MQSafeSession* ssnredis){
	MQ_LOG_INFO("mq_threads_queue_restore_pending_data: check and re-insert the retrieved data into mq server.");
	int iretoredmessagecount = 0;
	do{
		data_queue_receive_subscribe_entry* psubscribeentry = mq_buffer_list_get_item(subscribeList, 0); 		//get entry with non-block
		if(psubscribeentry != NULL){
			 MQ_LOG_DEBUG("mq_threads_queue_restore_pending_data: processing data for domain(%s), queue(%s)", psubscribeentry->domain, psubscribeentry->queueOrTopic);
			 mq_buffer_list_t* pReceivedBufferList = mq_threads_event_processor_get_response_buffers(psubscribeentry->domain, psubscribeentry->queueOrTopic);
			 if(pReceivedBufferList != NULL){
				 do{
					 data_event_processor_entry* pReceivedData = mq_buffer_list_get_item(pReceivedBufferList, 0);
					if( (pReceivedData != NULL) && (pReceivedData->replySize <=1) ) { //if pReceivedData->replySize > 1, meaning sending side is waiting for confirmation, it is better to wait for sending side send it again.
						 mq_pubsub_send_queue(psubscribeentry->domain, psubscribeentry->queueOrTopic, pReceivedData->messageData, strlen(pReceivedData->messageData), MQ_MSG_TIMEOUT_DEFAULT, NULL, MQ_MSG_TYPE_QUEUE_NORMAL, psubscribeentry->priority);
						
						MQ_LOG_WARN("mq_threads_queue_restore_pending_data: #%d: queue(%s)m message(%s).", iretoredmessagecount, psubscribeentry->queueOrTopic, pReceivedData->messageData);
						iretoredmessagecount ++;
					 }
				 }while(!mq_buffer_list_is_empty(pReceivedBufferList));
			 }
		}
	}while(!mq_buffer_list_is_empty(subscribeList));
}

void mq_threads_queue_receive_stop()
{
    if(__g_receive_initialized == true)
    {
		__g_receive_initialized = false;
        __g_stop_receive_flag = true;

		mq_threads_queue_receive_join();
        
		c_iterator iter;
        c_iterator st_ite = c_vector_begin(&__vector_threads_receive);
        c_iterator ed_ite = c_vector_end(&__vector_threads_receive);
        for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
        {
            if(ITER_REF(iter))
            {
                threads_queue_receive_param_t* prm = (threads_queue_receive_param_t*)ITER_REF(iter);
				pthread_join(*prm->pthrd, NULL);
				free(prm->pthrd);
				mq_sessn_close(prm->session);
				free(prm);
            }
        }
		c_vector_destroy(&__vector_threads_receive);

        do{
            data_queue_receive_subscribe_entry* psubscribeentry = mq_buffer_list_get_item(__threads_receive_queue_subscribe_list, 0); 		//get entry with non-block
            if(psubscribeentry != NULL) free(psubscribeentry);
        }while(!mq_buffer_list_is_empty(__threads_receive_queue_subscribe_list));
		mq_buffer_list_release(__threads_receive_queue_subscribe_list);
		__threads_receive_queue_subscribe_list = NULL;

        cmn_lock_release(__g_lock_queue_subscribe_list);
        
        
        st_ite = c_list_begin(&__g_list_subscribe_names);
        ed_ite = c_list_end(&__g_list_subscribe_names);
        for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
        {
            if(ITER_REF(iter))
            {
                queue_subscribe_name_entry* prm = (queue_subscribe_name_entry*)ITER_REF(iter);
				free(prm);
            }
        }
		c_list_destroy(&__g_list_subscribe_names);
		
		__g_lock_queue_subscribe_list = NULL;
		
    }
	
	__g_receive_initialized = false;
    MQ_LOG_INFO("mq_threads_queue_receive_stop: done.");
}

void mq_threads_queue_receive_join()
{
    c_iterator iter;
    c_iterator st_ite = c_vector_begin(&__vector_threads_receive);
    c_iterator ed_ite = c_vector_end(&__vector_threads_receive);
    for(iter = st_ite; !ITER_EQUAL(iter, ed_ite); ITER_INC(iter))
    {
        if(ITER_REF(iter))
        {
            threads_queue_receive_param_t* prm = (threads_queue_receive_param_t*)ITER_REF(iter);
            pthread_join(*prm->pthrd, NULL);
        }
    }
}

int mq_threads_queue_subscribe_add_entry(const char* opt, const char * domain, const char* topic, const MessageListener listener)
{
	int iret = MQ_ERROR_PROCESSER_NOT_READY;
    if( (__g_receive_initialized == true)&&(__threads_receive_queue_subscribe_list != NULL) )
    {
        int ires = mq_add_topic_to_string_subscribe_list(topic, MQ_MSG_PRIORITY_DEFAULT);
        
        if(ires > 0)
        {
            data_queue_receive_subscribe_entry* pSubscribeEntry = NULL;
            int total_size = sizeof(data_queue_receive_subscribe_entry);
            pSubscribeEntry = (data_queue_receive_subscribe_entry*)malloc(total_size);
            memset(pSubscribeEntry, 0, total_size);
            strncpy(pSubscribeEntry->opt, opt, MQ_OPTION_SIZE);
            strncpy(pSubscribeEntry->domain, domain, MQ_DOMAIN_SIZE);
            strncpy(pSubscribeEntry->queueOrTopic, topic, MQ_TOPIC_SIZE);
            pSubscribeEntry->messageHandler = listener;
            pSubscribeEntry->priority = MQ_MSG_PRIORITY_DEFAULT;

            mq_buffer_list_add_item(__threads_receive_queue_subscribe_list, pSubscribeEntry);
			iret = MQ_OK;
        }
        else
        {
			iret = MQ_OK_ALREADY_SUBSCRIBED;
        }
    }

    MQ_LOG_INFO("mq_threads_queue_subscribe_add_entry: queue name(%s), result(%d).", topic, iret);
	return iret;
}

int mq_threads_queue_subscribe_remove_entry(const char* opt, const char * domain, const char* topic, const MessageListener listener)
{
	int iret = MQ_ERROR_PROCESSER_NOT_READY;
    if( (__g_receive_initialized == true)&&(__threads_receive_queue_subscribe_list != NULL) )
    {
        int iretunscribe = mq_remove_topic_from_string_subscribe_list(topic);
        void* pData = mq_buffer_list_remove_item_by_condition(__threads_receive_queue_subscribe_list, mq_is_subscribe_entry_with_name, (void*)topic);
        if(pData) 	free(pData);

		if(iretunscribe > 0){
			iret = MQ_OK;
		}
		else{
			iret = MQ_OK_NOT_SUBSCRIBED;
		}
    }
    MQ_LOG_INFO("mq_threads_queue_subscribe_remove_entry: removed queue name is (%s), result: %d.", topic, iret);
	return iret;
}

//check if a subscribe entry is with name of strQName, reutrn 1 if name matches.
int mq_is_subscribe_entry_with_name(void* psubscribeentry, void* strQName)
{
    data_queue_receive_subscribe_entry* pData = (data_queue_receive_subscribe_entry*)psubscribeentry;
    if( (pData) && (strQName) && (strcmp(pData->queueOrTopic, (char*)strQName) == 0) )  return 1;
	return 0;
}

//If new queue is added to the subscribe list, return 1, otherwise, return 0;
int mq_add_topic_to_string_subscribe_list(const char* topic, int priority)
{
    int iret = 1;
    if(__g_lock_queue_subscribe_list)
    {
        cmn_lock_lock(__g_lock_queue_subscribe_list);
        
        c_iterator iter, itstart, itend;
        itstart = c_list_begin(&__g_list_subscribe_names);
        itend = c_list_end(&__g_list_subscribe_names);
        for(iter = itstart; !ITER_EQUAL(iter, itend); ITER_INC(iter)){
            queue_subscribe_name_entry* pentry = ITER_REF(iter);
            if( (pentry) && (strcmp(pentry->queueOrTopic, topic) == 0) ) {
                iret = 0;
                break;
            }
        }
        
        if(iret == 1){
            queue_subscribe_name_entry* newentry = (queue_subscribe_name_entry*)malloc(sizeof(queue_subscribe_name_entry));
            newentry->priority = priority;
            strncpy(newentry->queueOrTopic, topic, MQ_TOPIC_SIZE);
            c_list_push_back(&__g_list_subscribe_names, (value_type)newentry);
        }
        cmn_lock_unlock(__g_lock_queue_subscribe_list);
    }
    
    if(iret ==1){
        MQ_LOG_INFO("mq_add_topic_to_string_subscribe_list: topic (%s), priority (%d).", topic, priority); 
    }
    else{
        MQ_LOG_INFO("mq_add_topic_to_string_subscribe_list: already subscribed topic (%s)", topic); 
    }
        
    return iret;
}

//If new queue is removed from the subscribe list, return 1, otherwise, return 0;
int mq_remove_topic_from_string_subscribe_list(const char* topic)
{
	int iret = 0;
    if(__g_lock_queue_subscribe_list)
    {
        cmn_lock_lock(__g_lock_queue_subscribe_list);
        c_iterator iter, itstart, itend;
        itstart = c_list_begin(&__g_list_subscribe_names);
        itend = c_list_end(&__g_list_subscribe_names);
        for(iter = itstart; !ITER_EQUAL(iter, itend); ITER_INC(iter)){
            queue_subscribe_name_entry* pentry = ITER_REF(iter);
            if( (pentry) && (strcmp(pentry->queueOrTopic, topic) == 0) ) {
                c_list_erase(&__g_list_subscribe_names, iter);
                free(pentry);
				iret = 1;
                break;
            }
        }
        cmn_lock_unlock(__g_lock_queue_subscribe_list);
    }
    if(iret ==1){
        MQ_LOG_INFO("mq_remove_topic_from_string_subscribe_list:  queue  (%s) is unscribed.", topic);        
    }
    else{
        MQ_LOG_INFO("mq_remove_topic_from_string_subscribe_list: queue  (%s) is not scribed.", topic); 
    }

	return iret;
}

void mq_set_queue_priority(const char* topic, int priority)
{
    if(__g_lock_queue_subscribe_list)
    {
        cmn_lock_lock(__g_lock_queue_subscribe_list);
        c_iterator iter, itstart, itend;
        itstart = c_list_begin(&__g_list_subscribe_names);
        itend = c_list_end(&__g_list_subscribe_names);
        for(iter = itstart; !ITER_EQUAL(iter, itend); ITER_INC(iter)){
            queue_subscribe_name_entry* pSubscribeData = ITER_REF(iter);
            if( (pSubscribeData) && (strcmp(pSubscribeData->queueOrTopic, topic) == 0) ) {
                pSubscribeData->priority = priority;
                break;
            }
        }
        //sort the topics by priority
        c_list_sort(&__g_list_subscribe_names);

        cmn_lock_unlock(__g_lock_queue_subscribe_list);
        MQ_LOG_INFO("mq_set_queue_priority: topic (%s), priority(%d).", topic, priority);
    }
}

int mq_threads_queue_reset_queue_priority(const char* topic){
	int iret = MQ_OK_NO_CHANGE;
    data_queue_receive_subscribe_entry* pSubscribeData = 
        mq_buffer_list_get_item_by_condition(__threads_receive_queue_subscribe_list, mq_is_subscribe_entry_with_name, (void*)topic);
    if( (pSubscribeData) && (pSubscribeData->priority < 0) ){
        mq_buffer_list_t* plist = mq_threads_event_processor_get_response_buffers("", topic);
        
        int buffersize = mq_buffer_list_size(plist);
        if(buffersize < mq_config_get_max_buffer_size_per_queue()){
            mq_set_queue_priority(topic, MQ_MSG_PRIORITY_DEFAULT);
            pSubscribeData->priority = MQ_MSG_PRIORITY_DEFAULT;
            MQ_LOG_INFO("mq_threads_queue_reset_queue_priority: queue (%s) priority is reset to (%d).", topic, MQ_MSG_PRIORITY_DEFAULT);
			iret = MQ_OK;
        }
        else{
            MQ_LOG_INFO("mq_threads_queue_reset_queue_priority: queue (%s) priority is not reset due to big buffer size (%d).", topic, buffersize);
        }
    }
    else{
        if(pSubscribeData == NULL){
            MQ_LOG_DEBUG("mq_threads_queue_reset_queue_priority: queue not found (%s).", topic);
        }
        else{
            MQ_LOG_DEBUG("mq_threads_queue_reset_queue_priority: reset queue not required queue name (%s), priority(%d).", topic, pSubscribeData->priority);
        }
    }
	return iret;
}

const char* mq_get_queue_names()
{
    //return __g_string_queue_subscribe_list;
    memset(__g_string_queue_subscribe_list, 0, sizeof(__g_string_queue_subscribe_list));
    if(__g_lock_queue_subscribe_list)
    {
        cmn_lock_lock(__g_lock_queue_subscribe_list);

        c_iterator iter, itstart, itend;
        itstart = c_list_begin(&__g_list_subscribe_names);
        itend = c_list_end(&__g_list_subscribe_names);
        for(iter = itstart; !ITER_EQUAL(iter, itend); ITER_INC(iter)){
            queue_subscribe_name_entry* pentry = ITER_REF(iter);
            if(pentry->priority >= 0){
                strncat(__g_string_queue_subscribe_list, pentry->queueOrTopic, MQ_QUEUE_SUBSCRIBE_LIST_SIZE);
                strncat(__g_string_queue_subscribe_list, " ", MQ_QUEUE_SUBSCRIBE_LIST_SIZE);
            }
        }
        strncat(__g_string_queue_subscribe_list, NAME_QUEUE_ADMIN, MQ_QUEUE_SUBSCRIBE_LIST_SIZE); //Append the admin queue
        cmn_lock_unlock(__g_lock_queue_subscribe_list);
    }
 
    MQ_LOG_INFO("mq_get_queue_names: returns (%s).", __g_string_queue_subscribe_list);        
    return __g_string_queue_subscribe_list;
}


//Comparer to compare the priority of two queue, the input are of structure queue_subscribe_name_entry
int _queue_priority_comparer(void * x, void * y)
{
    return *(int *)(x) - *(int *)(y);
}