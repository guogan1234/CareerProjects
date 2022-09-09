#ifndef MQ_CMN_DEF_H
#define MQ_CMN_DEF_H

#include "cmn_logger.h"
#include "conf_parser.h"

#define MQ_LOG_DEBUG(...)    cmn_logger_log(mq_logger(), CMN_LOGGER_LEVEL_DEBUG, __VA_ARGS__)
#define MQ_LOG_INFO(...)     cmn_logger_log(mq_logger(), CMN_LOGGER_LEVEL_INFO, __VA_ARGS__)
#define MQ_LOG_WARN(...)     cmn_logger_log(mq_logger(), CMN_LOGGER_LEVEL_WARN, __VA_ARGS__)
#define MQ_LOG_ERR(...)      cmn_logger_log(mq_logger(), CMN_LOGGER_LEVEL_ERR, __VA_ARGS__)
#define MQ_LOG_FATAL(...)    cmn_logger_log(mq_logger(), CMN_LOGGER_LEVEL_FATAL, __VA_ARGS__)


//mq api config
sys_conf_mq_domain* get_mq_conf();
void release_mq_conf();

void mq_config_log_initialization();

cmn_logger* mq_logger();

void mq_logger_destory();

int mq_config_get_int(const char* strkey, int idefault);

const char* mq_config_get_string(const char* strkey, const char* strdef);

int mq_config_priority_supported();

int mq_config_get_max_buffer_size_per_queue();

void init_domain_id_map();

void release_domain_id_map();

const char* get_domain_id_from_name(const char* dm);

int mq_config_domain_name2id_enabled();


#endif //MQ_CMN_DEF_H
