#ifndef SDK_H
#define SDK_H

int conf_module_load_table(const char* table_name,const int domain_id,void (*fp)(const char* table_name,void *record));

#endif // SDK_H
