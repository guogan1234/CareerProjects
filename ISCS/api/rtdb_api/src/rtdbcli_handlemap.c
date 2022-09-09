/**
 * @file rtdbcli_handlemap.c
 * @author
 * @date 
 * @version 
 * @note ClientHandle map implementation class
 *          
 **/

#include <string.h>
#include "rtdbcli_malloc.h"
#include "rtdbcli_handlemap.h"

_handle_map* handle_map_init(int initSize) {
	_handle_map * hm = (_handle_map *)rtdbcli_malloc(sizeof(_handle_map));
	if (hm == NULL) {
		return NULL;
	}
	else{
		memset(hm, 0, sizeof(_handle_map));
	}
        
	hm->lasthandleid = 0;
	pthread_mutex_init(&hm->mutex, NULL);
	hm->capacity = initSize;
	hm->count = 0;
	int imallocsize = hm->capacity * sizeof(_handle_entry);
	hm->handles = (_handle_entry *)rtdbcli_malloc(imallocsize);

	if (hm->handles == NULL) {
		rtdbcli_free(hm);
		return NULL;
	}
	else{
		memset(hm->handles, 0, imallocsize);
	}
	return hm;
}

static _handle_map *
expand_handle_map(_handle_map *hm) {
	int i, capacity = hm->capacity;
	_phandle_entry pentry = NULL;
	int imallocsize = capacity * 2 * sizeof(_handle_entry);
	pentry = (_phandle_entry)rtdbcli_malloc(imallocsize);
	if (pentry == NULL) {
		return NULL;
	}
	else {
		memset(pentry, 0, imallocsize);
	}

	for (i=0; i<capacity; i++) {
		_phandle_entry os = &hm->handles[i];
		_phandle_entry ns = &pentry[os->handleid & (capacity * 2 -1)];
		*ns = *os;
	}
	
	rtdbcli_free(hm->handles);
	hm->handles = pentry;
	hm->capacity = capacity * 2;
	return hm;
}

void handle_map_release(_handle_map * hm){
	if (hm) {
		rtdbcli_free(hm->handles);
		rtdbcli_free(hm);
	}
}

int32_t handle_map_add_entry(_handle_map* hm, void * ud){
	int i;
	if ( (ud == NULL) || (hm == NULL) ) {
		return 0;
	}

	pthread_mutex_lock(&hm->mutex);
	if (hm->count >= hm->capacity * 3 / 4) {
		if (expand_handle_map(hm) == NULL) {
			pthread_mutex_unlock(&hm->mutex);
			return 0;
		}
	}
	
	for (i=0; ; i++) {
		_handle_entry *cur_entry;
		int32_t hid = ++hm->lasthandleid;
		if (hid == 0) {
			hid = ++hm->lasthandleid;
		}

		cur_entry = &hm->handles[hid & (hm->capacity - 1)];
		if (cur_entry->handleid) {
			continue;
		}
	
		cur_entry->handleid = hid;
		cur_entry->ref_count = 1;
		cur_entry->ud = ud;
		++hm->count;

		pthread_mutex_unlock(&hm->mutex);
		return hid;
	}

}

void*   handle_map_get_entry(_handle_map* hm, int32_t hid){
	_handle_entry * cur_entry;
	void * ud;
	if (hid == 0)
		return NULL;
	pthread_mutex_lock(&hm->mutex);
	cur_entry = &hm->handles[hid & (hm->capacity - 1)];
	if (cur_entry->handleid != hid) {
		pthread_mutex_unlock(&hm->mutex);
		return NULL;
	}
	cur_entry->ref_count ++;
	ud = cur_entry->ud;
	
	pthread_mutex_unlock(&hm->mutex);
	return ud;
}

void*   handle_map_release_entry(_handle_map* hm, int32_t hid){
	_handle_entry * cur_entry;
	void * ud = NULL;
	if (hid == 0){
		return NULL;
	}

	pthread_mutex_lock(&hm->mutex);
	cur_entry = &hm->handles[hid & (hm->capacity - 1)];
	if (cur_entry->handleid != hid) {
		pthread_mutex_unlock(&hm->mutex);
		return NULL;
	}

	cur_entry->ref_count--;
	if (cur_entry->ref_count <= 0) {
		ud = cur_entry->ud;
		cur_entry->handleid = 0;
		hm->count--;
	}

	pthread_mutex_unlock(&hm->mutex);
	return ud;
}

int     handle_map_list(_handle_map* hm, int count, int32_t* res){
	int i,t=0;
	pthread_mutex_lock(&hm->mutex);
	for (i=0; t < count && i < hm->capacity; i++) {
		_handle_entry * cur_entry = &hm->handles[i];
		if (cur_entry->handleid == 0) {
			continue;
		}

		res[t] = cur_entry->handleid;
		++t;
	}

	t=hm->count;
	pthread_mutex_unlock(&hm->mutex);
	
	return t;
}
