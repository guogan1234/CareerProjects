#include <c_map.h>
#include "cmn_lock.h"
#include "mq_map_utilities.h"
#include "mq_map_string_evtitem.h"
#include "mq_cmn_def.h"
#include "mq_priv.h"

struct mq_map_string_itemevent_t
{
    cmn_lock* 	        lock;
	c_map		        map_data;
    mq_events_pool*     pool_event;
};

mq_map_string_itemevent_t* mq_map_string_itemevent_create()
{
    mq_map_string_itemevent_t* pData = (mq_map_string_itemevent_t*) malloc(sizeof(mq_map_string_itemevent_t));
    
    if(pData)
    {
        pData->lock = cmn_lock_create();
        c_map_create(&pData->map_data, _stringkey_comparer);
        pData->pool_event = mq_events_pool_create();
        MQ_LOG_DEBUG("mq_map_string_itemevent_create: Done.");
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_itemevent_create: failed in allocating memory for the mapping.");
    }

    return pData;   
}

void* mq_map_string_itemevent_add_item(mq_map_string_itemevent_t* maptm, const char* key, void* value)
{
    struct mq_string_key_t searchkey;
    mq_map_string_list_reform_temp_key(&searchkey, key);
    bool isNew = false;
    
    event_item_entry* pentry = NULL;
    if( (maptm) && (searchkey.hashCode != 0))
    {
        cmn_lock_lock(maptm->lock);
        pentry = (event_item_entry*)c_map_at(&maptm->map_data, &searchkey);
        if(pentry == NULL)
        {
            //get one event item from the pool
            pentry = (event_item_entry*)mq_events_pool_get_event_item(maptm->pool_event);
            if(pentry)
            {
                mq_string_key_t* pkey = mq_map_string_list_create_key(key);
                
                //Create new (key, val) pair
                c_pair* pkeyval = malloc(sizeof(c_pair));
                pkeyval->first = pkey;
                pkeyval->second = pentry;
                
                c_map_insert(&maptm->map_data, pkeyval);
            }
            else{
                MQ_LOG_ERR("mq_events_pool_get_event_item: return null.");
            }
            
            isNew = true;
        }
        
        if( pentry )
        {
            cmn_lock_lock(pentry->plock);
            if(pentry->pData)
            {
                free(pentry->pData);
            }
            pentry->iStatus = 0;
            pentry->pData = value;
            cmn_event_reset(pentry->pevt);
            
            cmn_lock_unlock(pentry->plock);
        }
        cmn_lock_unlock(maptm->lock);
        
        MQ_LOG_DEBUG("mq_map_string_itemevent_add_item: is new item created?(%d).", isNew);
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_itemevent_add_item: invalid parameters.");
    }
    return pentry;    
}

void* mq_map_string_itemevent_get_item(mq_map_string_itemevent_t* maptm, const char* key)
{
    void* pentry = NULL;
    struct mq_string_key_t searchkey;
    mq_map_string_list_reform_temp_key(&searchkey, key);

    if( (maptm) && (searchkey.hashCode != 0))
    {
        cmn_lock_lock(maptm->lock);
        pentry = (event_item_entry*)c_map_at(&maptm->map_data, &searchkey);
        cmn_lock_unlock(maptm->lock);
        MQ_LOG_DEBUG("mq_map_string_itemevent_get_item: returned (%s)", (pentry==NULL ? "null": "one entry"));
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_itemevent_get_item: invalid parameters.");
    }
    
    return pentry;    
}

void mq_map_string_itemevent_remove_item(mq_map_string_itemevent_t* maptm, const char* key)
{
    struct mq_string_key_t searchkey;
    mq_map_string_list_reform_temp_key(&searchkey, key);
    if( (maptm) && (searchkey.hashCode != 0))
    {
        cmn_lock_lock(maptm->lock);
        
        c_iterator to_remove, map_end;	
        to_remove = c_map_find(&maptm->map_data, &searchkey);
        map_end = c_map_end(&maptm->map_data);

        if(!ITER_EQUAL(map_end, to_remove))
        {
            c_pair* pkeyval = ITER_REF(to_remove);
            c_map_erase(&maptm->map_data, to_remove);
            free(pkeyval->first);
            mq_events_pool_release_event_item(maptm->pool_event, pkeyval->second, true);
            free(pkeyval);
            MQ_LOG_DEBUG("mq_map_string_itemevent_remove_item: one item removed");
        }
        else
        {
            MQ_LOG_DEBUG("mq_map_string_itemevent_remove_item: item not found in map");
        }
        
        cmn_lock_unlock(maptm->lock);
        
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_itemevent_remove_item: invalid parameters.");
    }    
}

void mq_map_string_itemevent_release(mq_map_string_itemevent_t* maptm)
{
    if(maptm)
    {
        cmn_lock_lock(maptm->lock);
        
        c_iterator iter = c_map_begin(&maptm->map_data);
        c_iterator end = c_map_end(&maptm->map_data);
        while(!ITER_EQUAL(iter, end))
        {
            c_iterator to_erase = iter;
            c_pair* pkeyval = ITER_REF(to_erase);
            
            c_map_erase(&maptm->map_data, to_erase);
            free(pkeyval->first);
            mq_events_pool_release_event_item(maptm->pool_event, pkeyval->second, true);
            free(pkeyval);

            iter = c_map_begin(&maptm->map_data);
            end = c_map_end(&maptm->map_data);
        }

        cmn_lock_unlock(maptm->lock);
        cmn_lock_release(maptm->lock);
        c_map_destroy(&maptm->map_data);

        mq_events_pool_release(maptm->pool_event);
        
        free(maptm);
    }

    MQ_LOG_DEBUG("mq_map_string_itemevent_release: all resource released.");    
}