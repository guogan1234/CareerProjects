#include "mq_map_utilities.h"
#include "c_list.h"
#include "cmn_lock.h"
#include "mq_cmn_def.h"

// Data Definition
struct mq_events_pool
{
    cmn_lock*   lock;
    c_list      lst_events;
    int         num_events_used;
};


//Comparer
int _stringkey_comparer(void * x, void * y)
{
    int iret = *(int *)(x) - *(int *)(y);
    if(iret ==0){
        mq_string_key_t* tmp = (mq_string_key_t*)x;
        char* str_x = tmp->keytype== 0 ? tmp->strKey : tmp->ptrKey;
        tmp = (mq_string_key_t*)y;
        char* str_y = tmp->keytype== 0 ? tmp->strKey : tmp->ptrKey;
        iret = strcmp(str_x, str_y);
    }
    return iret;
}

void mq_map_string_list_reform_temp_key(mq_string_key_t* pkey, const char* inp)
{
    if(pkey != NULL){
        pkey->hashCode = mq_get_hash_from_string(inp);
        pkey->keytype = 1;
        pkey->ptrKey = ( char*)inp;
    }
}

mq_string_key_t* mq_map_string_list_create_key(const char* inp)
{
    int isize = sizeof(mq_string_key_t) + strlen(inp);
    mq_string_key_t* pData = (mq_string_key_t*)malloc(isize);
    if(pData)
    {
        memset(pData, 0, isize);
        strncpy(pData->strKey, inp, strlen(inp));
        pData->hashCode = mq_get_hash_from_string(inp);
        MQ_LOG_DEBUG("mq_map_string_list_create_key: Done.");
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_list_create_key: failed in allocating memory for the mapping.");
    }    
    return pData;
}

int mq_get_hash_from_string(const char* inp)
{
    if(inp)
    {
        int hash = 7;
        int ilen = strlen(inp);
        for (int i = 0; i < ilen; i++) 
        {
            hash = hash*31 + inp[i];
        }
        return hash;
    }
    else
    {
        return 0;
    }
}

//event pool
mq_events_pool* mq_events_pool_create()
{
    mq_events_pool* pevp = (mq_events_pool*)malloc(sizeof(mq_events_pool));
    if(pevp)
    {
        pevp->lock = cmn_lock_create();
        c_list_create(&pevp->lst_events, NULL);
        pevp->num_events_used = 0;
        MQ_LOG_DEBUG("mq_events_pool_create: Done.");
    }
    else
    {
        MQ_LOG_ERR("mq_events_pool_create: failed in allocating memory for pool.");
    }
    return pevp;
}

event_item_entry* mq_events_pool_get_event_item(mq_events_pool* pool)
{
    value_type retVal = NULL;
    int ievtsize = 0;
    if (pool)
    {
        cmn_lock_lock(pool->lock);

        ievtsize = c_list_size(&pool->lst_events);
        if(ievtsize > 0)
        {
            retVal = c_list_front(&pool->lst_events);
            c_list_pop_front(&pool->lst_events);
            MQ_LOG_DEBUG("mq_events_pool_get_event_item: Item from pool.");
        }
        else if( (ievtsize + pool->num_events_used) < MQ_MAX_SIZE_EVENT_POOL)
        {
            event_item_entry* pentry = (event_item_entry*) malloc(sizeof(event_item_entry));
            pentry->pevt = cmn_event_create();
            pentry->pData = NULL;
            pentry->iStatus = 0;
            pentry->plock = cmn_lock_create();
            retVal = pentry;
            pool->num_events_used += 1;
            MQ_LOG_DEBUG("mq_events_pool_get_event_item: Item create new.");
        }
        else{
            MQ_LOG_DEBUG("mq_events_pool_get_event_item: too many events.");
        }

        cmn_lock_unlock(pool->lock);
    }
    else
    {
        MQ_LOG_ERR("mq_events_pool_get_event_item: invalid parameter.");
    }
	return retVal;
}

void mq_events_pool_release_event_item(mq_events_pool* pool, event_item_entry* pentry, bool freeData)
{
    if (pool)
    {
        if(pentry)
        {
            cmn_lock_lock(pentry->plock);
            
            if( (pentry->pData) && (freeData) )
            {
                free(pentry->pData);
                pentry->pData = NULL;
            }
            pentry->iStatus = 0;
            cmn_event_reset(pentry->pevt);
            cmn_lock_unlock(pentry->plock);

            cmn_lock_lock(pool->lock);
            c_list_push_back(&pool->lst_events, pentry);
            cmn_lock_unlock(pool->lock);

            MQ_LOG_DEBUG("mq_events_pool_release_event_item: Item release back to pool.");
        }
        
    }
    else
    {
        MQ_LOG_ERR("mq_events_pool_release_event_item: invalid parameter.");
    }
}

void mq_events_pool_release(mq_events_pool* pool)
{
    if (pool)
    {
        cmn_lock_lock(pool->lock);
		
		//To remove and release all list item
		while(c_list_empty(&pool->lst_events) == false)
		{
            event_item_entry* pentry= c_list_front(&pool->lst_events);
            if(pentry)
            {
                cmn_lock_lock(pentry->plock);
                
                if(pentry->pData)
                {
                    free(pentry->pData);
                }
                
                if(pentry->pevt)
                {
                    cmn_event_release(pentry->pevt);
                }
                cmn_lock_unlock(pentry->plock);

                free(pentry);
            }
			c_list_pop_front(&pool->lst_events) ;
		}
		
        c_list_destroy(&pool->lst_events);
        cmn_lock_unlock(pool->lock);

        cmn_lock_release(pool->lock);
		free(pool);
        
        MQ_LOG_DEBUG("mq_events_pool_release: All resource released.");
    }
}
