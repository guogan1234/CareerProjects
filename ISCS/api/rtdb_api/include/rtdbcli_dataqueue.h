/**
 * @file rtdbcli_dataqueue.h
 * @author
 * @date 2019.
 * @version v1.0
 * @note 发送接受数据队列。
 **/
#ifndef RTDBCLI_DATAQUEUE_H
#define RTDBCLI_DATAQUEUE_H

#include <stdint.h>
#include <pthread.h>

#define CONDITION_TRIGGERED             1
#define CONDITION_UNTRIGGERED           0

#define MAX_SIZE_DATAFRAME              65536
#define ELEMENT_SIZE_DATA_QUEUE         1024

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct rtdb_dataqueue 
{ 
    int                 front;
    int                 rear;
    int                 size; 
    unsigned int        capacity; 
    
    int                 element_count;
    int                 element_size;
    unsigned char*      data_holder; 

    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
    int                 triggered;
} _rtdb_dataqueue, * _prtdb_dataqueue;


_prtdb_dataqueue rtdb_dataqueue_init(int elsize, int capacity);

int rtdb_dataqueue_add_item(_prtdb_dataqueue dqueue, void* item, int isize);

void* rtdb_dataqueue_get_item(_prtdb_dataqueue dqueue, int timeout, int* pretsize); 

void rtdb_dataqueue_release(_prtdb_dataqueue dqueue);

int rtdb_dataqueue_get_element_count(_prtdb_dataqueue dqueue);

int rtdb_dataqueue_get_size(_prtdb_dataqueue dqueue);

#ifdef __cplusplus
}

#endif
#endif
