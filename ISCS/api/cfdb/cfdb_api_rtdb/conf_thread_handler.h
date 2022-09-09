#ifndef CONF_THREADHANDLER_H
#define CONF_THREADHANDLER_H
#ifdef __cplusplus
extern "C"
{
#endif

void* conf_threadhandler_recive_message(void *arg);

void* conf_threadhandler_parse_message(void *arg);

void* conf_threadhandler_notify_message(void *arg);

#ifdef __cplusplus
}
#endif
#endif
