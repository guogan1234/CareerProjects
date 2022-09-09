#include <stdlib.h>
#include "irtdb_fes_common.h"
#include "irtdb_fes_data.h"

irtdb_fep_di* irtdb_fes_create_fep_di(const char *path,const char *label,int32_t value,int64_t timestamp)
{
    irtdb_fep_di *di = NULL;
    
    if(!path || !label) return NULL;
    di = calloc(1,sizeof(*di));
    if(!di) return NULL;
    di->path = strdup(path);
    if(!di->path) 
    {
        free(di);
        return NULL;
    }
    di->label = strdup(label);
    if(!di->label)
    {
        free(di->path);
        free(di);
        return NULL;
    }
    di->value = value;
    di->timestamp = timestamp;
    
    return di;
}

void irtdb_fes_free_fep_di(irtdb_fep_di *di)
{
    if(!di) return;
    if(di->path) free(di->path);
    if(di->label) free(di->label);
    free(di);
}

irtdb_fep_ai* irtdb_fes_create_fep_ai(const char *path,float value,int64_t timestamp)
{
    irtdb_fep_ai *ai = NULL;
    
    if(!path) return NULL;
    ai = calloc(1,sizeof(*ai));
    if(!ai) return NULL;
    ai->path = strdup(path);
    if(!ai->path) 
    {
        free(ai);
        return NULL;
    }
    ai->value = value;
    ai->timestamp = timestamp;
    return ai;
}

void irtdb_fes_free_fep_ai(irtdb_fep_ai *ai)
{
    if(!ai) return ;
    if(ai->path) free(ai->path);
    free(ai);
}

irtdb_fep_acc* irtdb_fes_create_fep_acc(const char *path,int32_t value,int64_t timestamp)
{
    irtdb_fep_acc *acc = NULL;
    
    if(!path) return NULL;
    acc = calloc(1,sizeof(*acc));
    if(!acc) return NULL;
    acc->path = strdup(path);
    if(!acc->path) 
    {
        free(acc);
        return NULL;
    }
    acc->value = value;
    acc->timestamp = timestamp;
    return acc;
}

void irtdb_fes_free_fep_acc(irtdb_fep_acc *acc)
{
    if(!acc) return;
    if(acc->path) free(acc->path);
    free(acc);
}

 