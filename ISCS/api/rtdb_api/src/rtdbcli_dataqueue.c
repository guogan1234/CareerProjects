/**
 * @file rtdbcli_dataqueue.c
 * @author
 * @date 
 * @version 
 * @note 
 *          
 **/

#include "rtdbcli_dataqueue.h"
#include "rtdbcli_malloc.h"
#include <stdio.h>
#include <string.h>

_prtdb_dataqueue rtdb_dataqueue_init(int elmsize, int capacity)
{
	_rtdb_dataqueue * dqueue = (_rtdb_dataqueue *)rtdbcli_malloc(sizeof(_rtdb_dataqueue));
	if (dqueue == NULL) {
		return NULL;
	}
	else{
		memset(dqueue, 0 , sizeof(_rtdb_dataqueue));
	}

	//[elm 1][elm 2][elm 3]......[elm n][MAX_SIZE_DATAFRAME] //MAX_SIZE_DATAFRAME data buffer is reserved for last element
	int imallocsize = capacity * elmsize + MAX_SIZE_DATAFRAME;   
	dqueue->data_holder = rtdbcli_malloc(imallocsize);
	if(dqueue->data_holder == NULL) {
		rtdbcli_free(dqueue);
		return NULL;
	}
	else {
		memset(dqueue->data_holder, 0, imallocsize);
	}

    dqueue->front = dqueue->rear = 0;  
	dqueue->size = dqueue->element_count = 0;
    dqueue->capacity = capacity;
	dqueue->element_size = elmsize;
	
	pthread_mutex_init(&dqueue->mutex, NULL);
	pthread_cond_init(&dqueue->cond, NULL);
	dqueue->triggered = CONDITION_UNTRIGGERED;
	
	return dqueue;
}

int32_t rtdb_dataqueue_add_item(_prtdb_dataqueue dqueue, void *item, int isize)
{
	if( (dqueue == NULL) || (dqueue->element_size == 0) ) {
		return RTDBCLI_EC_ERROR;
	}

	pthread_mutex_lock(&dqueue->mutex);

	int inumofblocks = (isize + 4) / dqueue->element_size + ((isize + 4) % dqueue->element_size > 0 ? 1 : 0);

	//queue is full
	if(dqueue->size + inumofblocks > dqueue->capacity) {
		pthread_mutex_unlock(&dqueue->mutex);
		return RTDBCLI_EC_ERROR;		
	}

	//copy the items to the list
	unsigned char* cur_pos = dqueue->data_holder + dqueue->rear * dqueue->element_size;
	memcpy(cur_pos, &isize, 4);
	cur_pos += 4;
	memcpy(cur_pos, (unsigned char*)item, isize);

	if(dqueue->rear + inumofblocks > dqueue->capacity){
		dqueue->rear = 0;
	}
	else{
		dqueue->rear = dqueue->rear + inumofblocks;
	}
    
	dqueue->size = dqueue->size + inumofblocks;
	dqueue->element_count += 1;

	dqueue->triggered = CONDITION_TRIGGERED;
	pthread_cond_signal(&dqueue->cond);
	
	pthread_mutex_unlock(&dqueue->mutex);
	return RTDBCLI_EC_OK;
}

void* rtdb_dataqueue_get_item(_prtdb_dataqueue dqueue, int timeout, int* pretsize)
{
	void* retitem = NULL;
	if( (dqueue == NULL) || (pretsize == 0) ) {
		return retitem;
	}
	*pretsize = 0;

	pthread_mutex_lock(&dqueue->mutex);   
	int is_data_available = CONDITION_UNTRIGGERED;

	if (dqueue->element_count < 1) {
		struct timespec time_to_wait = {0, 0};
        time_to_wait.tv_sec = time(NULL) + timeout;

		if(dqueue->triggered == CONDITION_UNTRIGGERED) {
            pthread_cond_timedwait(&dqueue->cond, &dqueue->mutex, &time_to_wait);
        }
        is_data_available = dqueue->triggered;
	}
	else {
		is_data_available = CONDITION_TRIGGERED;
	}

	if( (is_data_available == CONDITION_TRIGGERED) && (dqueue->element_count > 0) ) {
		unsigned char* cur_pos = dqueue->data_holder + dqueue->front * dqueue->element_size;
		int elmsize = *(int*)cur_pos;
		*pretsize = elmsize;
		retitem = cur_pos + 4;

		int inumofblocks = (elmsize + 4) / dqueue->element_size + ((elmsize + 4) % dqueue->element_size > 0 ? 1 : 0);

		if(dqueue->front + inumofblocks > dqueue->capacity){
			dqueue->front = 0;
		}
		else{
			dqueue->front = dqueue->front + inumofblocks;
		}

		dqueue->size = dqueue->size - inumofblocks;
		dqueue->element_count -= 1;  

		if(dqueue->element_count > 0) {
			dqueue->triggered = CONDITION_TRIGGERED;
			pthread_cond_signal(&dqueue->cond);
		}
		else {
			dqueue->triggered = CONDITION_UNTRIGGERED;
		}
	}	
	
	pthread_mutex_unlock(&dqueue->mutex);
	return retitem;
}

void rtdb_dataqueue_release(_prtdb_dataqueue queue)
{
	pthread_mutex_destroy(&queue->mutex);
	pthread_cond_destroy(&queue->cond);

	if( (queue != NULL) && (queue->data_holder != NULL) ) {
		rtdbcli_free(queue->data_holder);
	}

	if(queue != NULL) {
		rtdbcli_free(queue);
	}
}


int rtdb_dataqueue_get_size(_prtdb_dataqueue dqueue)
{
	if(dqueue != NULL) {
		return dqueue->size;
	}
	else {
		return RTDBCLI_EC_ERROR;
	}
}

int rtdb_dataqueue_get_element_count(_prtdb_dataqueue dqueue)
{
	if(dqueue != NULL) {
		return dqueue->element_count;
	}
	else {
		return RTDBCLI_EC_ERROR;
	}
}
