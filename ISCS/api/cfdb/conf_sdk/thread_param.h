#ifndef THREAD_PARAM_H
#define THREAD_PARAM_H

typedef void (*T_fPoint)(const char *table_name,void *record);
typedef struct t_func_param{
    int domain_id;
    char* table_name;
    void* data;
    T_fPoint t_fp;
}T_fParam;

#endif // THREAD_PARAM_H
