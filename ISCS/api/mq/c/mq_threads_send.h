#ifndef MQ_THREADS_SEND_H
#define MQ_THREADS_SEND_H
#include "mq_buffer_list.h"
#include <c_vector.h>
#include <pthread.h>

typedef struct threads_send_param_t     threads_send_param_t;
typedef struct data_send_entry          data_send_entry;

struct threads_send_param_t
{
    int                 index;
    pthread_t*          pthrd;
    MQSafeSession*      session;
    mq_buffer_list_t*   list_send_buffer_message;
};

struct data_send_entry
{
    long    messageid;                  //used for acknowledge
    long    timeout;
    int     priority;
    char    opt[MQ_OPTION_SIZE];
    char    domain[MQ_DOMAIN_SIZE];
    char    queueOrTopic[MQ_TOPIC_SIZE];
    char    message[0];
};



void mq_threads_send_init(int thrdNum);

void mq_threads_send_start();

void* mq_threads_send_run(void* ptr);

void mq_threads_send_stop();

void mq_threads_send_join();

int mq_threads_send_add_entry(const char* domain, long lmsgId, const char* opt, const char* topic, const char* payload, const long timeout, const int priority);

#endif
