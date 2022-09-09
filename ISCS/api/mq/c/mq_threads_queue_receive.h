#ifndef MQ_THREADS_QUEUE_RECEIVE_H
#define MQ_THREADS_QUEUE_RECEIVE_H
#include "mq_buffer_list.h"
#include <c_vector.h>
#include <pthread.h>

typedef struct threads_queue_receive_param_t     threads_queue_receive_param_t;
typedef struct data_queue_receive_subscribe_entry  data_queue_receive_subscribe_entry;

struct threads_queue_receive_param_t
{
    int                 index;
    pthread_t*          pthrd;
    MQSafeSession*      session;
    mq_buffer_list_t*   list_receive_queue_subscribes;
};

struct data_queue_receive_subscribe_entry
{
    long    timeout;
    int     priority;
    char    opt[MQ_OPTION_SIZE];
    char    domain[MQ_DOMAIN_SIZE];
    char    queueOrTopic[MQ_TOPIC_SIZE];
    MessageListener    messageHandler;
};

void mq_threads_queue_receive_init();

int mq_threads_queue_receive_start();

void* mq_threads_queue_receive_run(void* ptr);

void mq_threads_queue_receive_stop();

void mq_threads_queue_receive_join();

void mq_threads_queue_restore_pending_data(mq_buffer_list_t* subscribeList, MQSafeSession* ssnredis);

int mq_threads_queue_subscribe_add_entry(const char* opt, const char * domain, const char* topic, const MessageListener listener);

int mq_threads_queue_subscribe_remove_entry(const char* opt, const char * domain, const char* topic, const MessageListener listener);

int mq_threads_queue_reset_queue_priority(const char* topic);

void mq_set_queue_priority(const char* topic, int priority);

const char* mq_get_queue_names();

#endif
