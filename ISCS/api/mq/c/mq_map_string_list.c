#include <c_map.h>
#include "cmn_lock.h"
#include "mq_map_string_list.h"
#include "mq_cmn_def.h"

struct mq_map_string_list_t
{
    cmn_lock* 	lock;
	c_map		map_data;
};

mq_map_string_list_t* mq_map_string_list_create()
{
    mq_map_string_list_t* pData = (mq_map_string_list_t*) malloc(sizeof(mq_map_string_list_t));
    if(pData)
    {
        pData->lock = cmn_lock_create();
        c_map_create(&pData->map_data, _stringkey_comparer);
        MQ_LOG_DEBUG("mq_map_string_list_create: Done.");
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_list_create: failed in allocating memory for the mapping.");
    }
    return pData;
}

void* mq_map_string_list_add_item_entry(mq_map_string_list_t* maptm, const char* key, void* value, int maxbuffsize)
{
    struct mq_string_key_t searchkey;
    mq_map_string_list_reform_temp_key(&searchkey, key);

    
    mq_buffer_list_t* plist = NULL;
    if( (maptm) && (searchkey.hashCode != 0))
    {
        cmn_lock_lock(maptm->lock);
        plist = (mq_buffer_list_t*)c_map_at(&maptm->map_data, &searchkey);
        if(plist == NULL)
        {
            mq_string_key_t* pkey = mq_map_string_list_create_key(key);
            plist = mq_buffer_list_create(maxbuffsize);
            
            //Create new (key, val) pair
            c_pair* pkeyval = malloc(sizeof(c_pair));
            pkeyval->first = pkey;
            pkeyval->second = plist;
            
            c_map_insert(&maptm->map_data, pkeyval);
        }
        
        cmn_lock_unlock(maptm->lock);
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_list_add_item_entry: invalid parameters.");
    }
    
    if( (plist) && (value) )
    {
        mq_buffer_list_add_item(plist, value);
        MQ_LOG_DEBUG("mq_map_string_list_add_item_entry: new item added.");
    }
    
    return plist;
}

void* mq_map_string_list_get_item(mq_map_string_list_t* maptm, const char* key)
{
    void* plist = NULL;
    struct mq_string_key_t searchkey;
    mq_map_string_list_reform_temp_key(&searchkey, key);
    
    if( (maptm) && (searchkey.hashCode != 0))
    {
        cmn_lock_lock(maptm->lock);
        plist = (mq_buffer_list_t*)c_map_at(&maptm->map_data, &searchkey);
        cmn_lock_unlock(maptm->lock);
        
        MQ_LOG_DEBUG("mq_map_string_list_get_item: item returned(isnull=%d).", (plist==NULL));
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_list_get_item: invalid parameters.");
    }
    
    return plist;
}

void mq_map_string_list_remove_item(mq_map_string_list_t* maptm, const char* key)
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
            mq_buffer_list_release(pkeyval->second);
            free(pkeyval);
            
            MQ_LOG_DEBUG("mq_map_string_list_remove_item: item removed.");
        }
        else
        {
            MQ_LOG_DEBUG("mq_map_string_list_remove_item: item not found in map");
        }
        
        cmn_lock_unlock(maptm->lock);
        
    }
    else
    {
        MQ_LOG_ERR("mq_map_string_list_remove_item: invalid parameters.");
    }
}

void mq_map_string_list_release(mq_map_string_list_t* maptm)
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
            do{
                void* pentry = mq_buffer_list_get_item(pkeyval->second, 0); 		//get entry with non-block
                if(pentry != NULL) free(pentry);
            }while(!mq_buffer_list_is_empty(pkeyval->second));
            mq_buffer_list_release(pkeyval->second);
            
            free(pkeyval);

            iter = c_map_begin(&maptm->map_data);
            end = c_map_end(&maptm->map_data);
        }

        cmn_lock_unlock(maptm->lock);
        cmn_lock_release(maptm->lock);
        c_map_destroy(&maptm->map_data);
        
        free(maptm);
        MQ_LOG_DEBUG("mq_map_string_list_release: all resource released.");   
    }    
}
