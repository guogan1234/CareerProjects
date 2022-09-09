#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "conf_parser.h"
#include "mq_cmn_def.h"
#include "mq_constant.h"
#include <c_map.h>
#include "cmn_lock.h"
#include "mq_priv.h"

static sys_conf_mq_domain*       _g_mq_conf_instance = NULL;
static cmn_logger* _g_mq_logger = NULL;


///////////////////////////////////////////
//
// Data for name to id mapping
//
///////////////////////////////////////////
typedef struct s_map_domain_name_ids{
    c_map        map_domain_name_ids;
    cmn_lock*    lock_domain_name_ids;
    char*        string_domain_name_ids;
} s_map_domain_name_ids;

static s_map_domain_name_ids g_map_domain_name_ids;
static int g_flag_initialized_map_domain_name_ids = 0;
///////////////////////////////////////////

sys_conf_mq_domain* get_mq_conf()
{
    if(_g_mq_conf_instance == NULL)
    {
       _g_mq_conf_instance = sys_conf_get_mq_domain();
        MQ_LOG_DEBUG("MQ configuration loaded!");
    }
    return _g_mq_conf_instance;
}

void release_mq_conf()
{
    sys_conf_free_mq_domain(_g_mq_conf_instance);
    release_domain_id_map();
    _g_mq_conf_instance = NULL;
}

void mq_config_log_initialization()
{
    sys_conf_mq_domain* pmqconf = get_mq_conf();
    if(pmqconf)
    {
        sys_conf_log * plogconf = pmqconf->log;
        if(plogconf)
        {
            //cmn_error_code res = cmn_log_init(plogconf->file, plogconf->level, plogconf->max_size, plogconf->roll_num);
            _g_mq_logger = cmn_logger_init(plogconf->file, plogconf->level, plogconf->max_size, plogconf->roll_num);
            if(_g_mq_logger)
            {
                MQ_LOG_INFO("MQ Log initialized OK! FileName: %s, Level: %d, MaxSize: %d, RollNum: %d.", 
                                                plogconf->file, plogconf->level, plogconf->max_size, plogconf->roll_num);
            }
        }
        
        init_domain_id_map();
    }
}

cmn_logger* mq_logger()
{
    return _g_mq_logger;
}

void mq_logger_destory()
{
    if(_g_mq_logger)
    {
        cmn_logger_destory(_g_mq_logger);
        _g_mq_logger=NULL;
    }
}

int mq_config_get_int(const char* strkey, int idefault)
{
    int iret = idefault;
    sys_conf_mq_domain* pmqconf = get_mq_conf();
    if(pmqconf)
    {
        mq_param_key_value * pkeyval = pmqconf->params;
        while(pkeyval)
        {
            if(strcmp(pkeyval->strkey, strkey) == 0)
            {
                const char* strval = pkeyval->strvalue;
                if( (strlen(strval) > 0) && (isdigit(*strval)) )
                {
                    iret = atoi(strval);
                }
            }
            pkeyval = pkeyval->next;
        }
    }
    
    MQ_LOG_DEBUG("MQ Log mq_config_get_int! strkey: %s, default: %d, return value: %d.", strkey, idefault, iret);
    
    return iret;
}

const char* mq_config_get_string(const char* strkey, const char* strdef)
{
    const char* strret = strdef;
    sys_conf_mq_domain* pmqconf = get_mq_conf();
    if(pmqconf)
    {
        mq_param_key_value * pkeyval = pmqconf->params;
        while(pkeyval)
        {
            if(strcmp(pkeyval->strkey, strkey) == 0)
            {
                strret = pkeyval->strvalue;
            }
            pkeyval = pkeyval->next;
        }
    }
    MQ_LOG_DEBUG("MQ Log mq_config_get_string! strkey: %s, default: %s, return value: %s.", strkey, strdef, strret);
    return strret;
}


int mq_config_priority_supported()
{
    static int __g_bzpop_supported = -1;
    
    if(__g_bzpop_supported < 0)
    {
        __g_bzpop_supported = mq_config_get_int(MQ_CONF_BZPOP_SUPPORTED, 1);
    }
    
    return __g_bzpop_supported;
}

int mq_config_get_max_buffer_size_per_queue()
{
    static int __g_max_buffer_size_per_queue = -1;
    
    if(__g_max_buffer_size_per_queue < 0)
    {
        __g_max_buffer_size_per_queue = mq_config_get_int(MQ_CONF_MAX_BUFFER_SIZE_PER_QUEUE, 10);
    }
    
    return __g_max_buffer_size_per_queue;
}

void init_domain_id_map() {
    if(g_flag_initialized_map_domain_name_ids == 0) {
        g_map_domain_name_ids.lock_domain_name_ids = cmn_lock_create();
        //get the domain name id list
        char* pstrcfg = sys_conf_get_domain_name_id_list();
        
        if(pstrcfg != NULL) {
            //lock data
            if(g_map_domain_name_ids.lock_domain_name_ids) {
                cmn_lock_lock(g_map_domain_name_ids.lock_domain_name_ids);

                c_map_create(&g_map_domain_name_ids.map_domain_name_ids, string_comparer);
                const char* delim = "^>";
                char* stoken = strtok(pstrcfg, delim);
                while( stoken != NULL ) {
                    char* sval = strtok(NULL, delim);
                    if(sval != NULL) {
                        c_pair* pkeyval = malloc(sizeof(c_pair));
                        pkeyval->first = stoken;
                        pkeyval->second = sval;
                        c_map_insert(&g_map_domain_name_ids.map_domain_name_ids, pkeyval);
                        MQ_LOG_DEBUG("MQ Log init_domain_id_map! one domain name=>id pair (%s => %s) created.", stoken, sval);
                        //get next token
                        stoken = strtok(NULL, delim);
                    }
                    else{
                        //exit the loop
                        stoken = NULL;
                    }
                }
            
                //unlock data
                cmn_lock_unlock(g_map_domain_name_ids.lock_domain_name_ids);
            }

            g_map_domain_name_ids.string_domain_name_ids = pstrcfg;
            //free(pstrcfg);  it will be freed by release_domain_id_map
            
           g_flag_initialized_map_domain_name_ids = 1;
        }
    }
}

void release_domain_id_map(){
    
    if(g_flag_initialized_map_domain_name_ids == 1)
    {
        cmn_lock_lock(g_map_domain_name_ids.lock_domain_name_ids);
        
        c_iterator iter = c_map_begin(&g_map_domain_name_ids.map_domain_name_ids);
        c_iterator end = c_map_end(&g_map_domain_name_ids.map_domain_name_ids);
        while(!ITER_EQUAL(iter, end))
        {
            c_iterator to_erase = iter;
            c_pair* pkeyval = ITER_REF(to_erase);
            
            c_map_erase(&g_map_domain_name_ids.map_domain_name_ids, to_erase);
            //free(pkeyval->first);
            //free(pkeyval->second);
            free(pkeyval);

            iter = c_map_begin(&g_map_domain_name_ids.map_domain_name_ids);
            end = c_map_end(&g_map_domain_name_ids.map_domain_name_ids);
        }

        cmn_lock_unlock(g_map_domain_name_ids.lock_domain_name_ids);
        cmn_lock_release(g_map_domain_name_ids.lock_domain_name_ids);
        g_map_domain_name_ids.lock_domain_name_ids = NULL;
        c_map_destroy(&g_map_domain_name_ids.map_domain_name_ids);
        
        free(g_map_domain_name_ids.string_domain_name_ids);  //all name value are free by this
        g_map_domain_name_ids.string_domain_name_ids = NULL;
        
        g_flag_initialized_map_domain_name_ids = 0;
        
        MQ_LOG_DEBUG("release_domain_id_map: all resource released.");   
    }    
}

const char* get_domain_id_from_name(const char* dm) {
    const char* retval = NULL;
    if( (dm != NULL) && (g_flag_initialized_map_domain_name_ids != 0) ) {
        cmn_lock_lock(g_map_domain_name_ids.lock_domain_name_ids);
        
        retval = (const char*)c_map_at(&g_map_domain_name_ids.map_domain_name_ids, (void*)dm);
        
        cmn_lock_unlock(g_map_domain_name_ids.lock_domain_name_ids);
    }
    
    if(retval == NULL) {
        retval = dm;
    }

    MQ_LOG_DEBUG("get_domain_id_from_name: returned(name: %s => id: %s).", dm, retval);
    return retval;
}

int mq_config_domain_name2id_enabled() {
    static int __g_domain_name2id_enabled = -1;
    
    if(__g_domain_name2id_enabled < 0)
    {
        __g_domain_name2id_enabled = mq_config_get_int(MQ_CONF_DOMAIN_NAME2ID_ENABLED, 0);
        
        MQ_LOG_DEBUG("mq_config_domain_name2id_enabled: mq_domain_name2id_enabled was set with %d.", __g_domain_name2id_enabled);
    }
    
    return __g_domain_name2id_enabled;
}