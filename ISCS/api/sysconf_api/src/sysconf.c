/**
 * @file sysconf.c
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口实现。接口线程安全。
 **/
#include "sysconf.h"
#include "sysconf_data_private.h"
#include "sysconf_json_loader.h"
#include "sysconf_malloc.h"
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static sysconf_conf *_sysconf = NULL;                      ///< 系统配置对象
static pthread_mutex_t _mtx = PTHREAD_MUTEX_INITIALIZER;   ///< 系统管理对象锁

sysconf_conf* sysconf_get_sysconf()
{
    return _sysconf;
}

SYSCONF_API int32_t sysconf_init()
{
    int32_t ec = SYSCONF_EC_OK;
    char *installPath = NULL;
    char file[SYSCONF_PATH_MAX_LEN];

    // 在多线程、多次初始化时，只有第一次被调用时执行内存分配、文件加载等操作。
    // 后续调用仅做计数操作，不做重新初始化。
    pthread_mutex_lock(&_mtx);
    if (_sysconf)
    {
        _sysconf->initialized++;
        pthread_mutex_unlock(&_mtx);
        return SYSCONF_EC_OK;
    }

    // 为系统配置对象申请内存
    _sysconf = sysconf_malloc(sizeof(*_sysconf));
    if (!_sysconf)
    {
        pthread_mutex_unlock(&_mtx);
        return SYSCONF_EC_OOM;
    }
    memset(_sysconf, 0, sizeof(*_sysconf));
    
    // 初始化系统日志环境，获取日志记录器
    ec = log_init();
    if (ec != LOG_EC_OK)
    {
        return SYSCONF_EC_LOGERR;
    }

    ec = log_create_logger("sysconfApiLog.conf", &_sysconf->log);
    if (ec != LOG_EC_OK)
    {
        log_uninit();
        sysconf_free(_sysconf);
        _sysconf = NULL;
        pthread_mutex_unlock(&_mtx);
        return SYSCONF_EC_LOGERR;
    }

    // 获取系统安装路径
    installPath = getenv("ISCS_HOME");
    installPath = installPath ? installPath : ".";
    log_trace_debug(_sysconf->log, "sysconf_init, installation path(%s)", installPath);
    // 加载系统部署配置
    snprintf(file, sizeof(file), "%s/conf/systemDeployConf.json", installPath);
    ec = sysconf_load_deployconf_json(file);
    if (ec != SYSCONF_EC_OK)
    {
        log_uninit();
        log_free_logger(_sysconf->log);
        sysconf_clean_system_manager(&_sysconf->sm);
        sysconf_free(_sysconf);
        _sysconf = NULL;
        pthread_mutex_unlock(&_mtx);
        return ec;
    }

    // 加载系统运行配置
    snprintf(file, sizeof(file), "%s/conf/systemRunConf.json", installPath);
    ec = sysconf_load_runconf_json(file);
    if (ec != SYSCONF_EC_OK)
    {
        log_uninit();
        log_free_logger(_sysconf->log);
        sysconf_clean_system_manager(&_sysconf->sm);
        sysconf_clean_mq_client(&_sysconf->mqcli);
        sysconf_clean_rtdb_client(&_sysconf->rtdbcli);
        sysconf_clean_tsdb_client(&_sysconf->tsdbcli);
        sysconf_clean_cfdb_client(&_sysconf->cfdbcli);
        sysconf_clean_pm_client(&_sysconf->pmcli);
        sysconf_free(_sysconf);
        _sysconf = NULL;
        pthread_mutex_unlock(&_mtx);
        return ec;
    }

    // 累加初始化次数
    _sysconf->initialized++;
    pthread_mutex_unlock(&_mtx);

    return SYSCONF_EC_OK;
}

SYSCONF_API void sysconf_uninit()
{
    if (!_sysconf)
    {
        return;
    }

    pthread_mutex_lock(&_mtx);
    if (_sysconf)
    {
        _sysconf->initialized--;
        if (_sysconf->initialized > 0)
        {
            log_trace_debug(_sysconf->log, "sysconf_uninit, decrement counter");
            pthread_mutex_unlock(&_mtx);
            return;
        }

        log_trace_debug(_sysconf->log, "sysconf_uninit, destroyed");

        log_uninit();
        log_free_logger(_sysconf->log);
        sysconf_clean_system_manager(&_sysconf->sm);
        sysconf_clean_mq_client(&_sysconf->mqcli);
        sysconf_clean_rtdb_client(&_sysconf->rtdbcli);
        sysconf_clean_tsdb_client(&_sysconf->tsdbcli);
        sysconf_clean_cfdb_client(&_sysconf->cfdbcli);
        sysconf_clean_pm_client(&_sysconf->pmcli);
        sysconf_free(_sysconf);
        _sysconf = NULL;
    }
    pthread_mutex_unlock(&_mtx);
}

SYSCONF_API int32_t sysconf_get_domain(int32_t id, sysconf_domain **domain)
{
    int32_t ec = SYSCONF_EC_NOEXIST;

    // 检查输入指针、检查初始化状态
    if (!domain)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *domain = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    // 查找域列表，设置查询结果
    for (int32_t i = 0; i < _sysconf->sm.domainNum; i++)
    {
        if (_sysconf->sm.domains[i].id == id)
        {
            *domain = &_sysconf->sm.domains[i];
            ec = SYSCONF_EC_OK;
            break;
        }
    }

    return ec;
}

SYSCONF_API int32_t sysconf_get_native_domain(sysconf_domain **domain)
{
    // 检查输入指针、检查初始化状态
    if (!domain)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *domain = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    if ((!_sysconf->sm.native) || (!_sysconf->sm.native->domain))
    {
        return SYSCONF_EC_NOEXIST;
    }

    *domain = _sysconf->sm.native->domain;

    return SYSCONF_EC_OK;
}

SYSCONF_API int32_t sysconf_get_node(int32_t id, sysconf_node **node)
{
    int32_t ec = SYSCONF_EC_NOEXIST;

    // 检查输入指针，设置默认值
    if (!node)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *node = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    // 查找节点列表，设置查询结果
    for (int32_t i = 0; i < _sysconf->sm.nodeNum; i++)
    {
        if (_sysconf->sm.nodes[i].id == id)
        {
            *node = &_sysconf->sm.nodes[i];
            ec = SYSCONF_EC_OK;
        }
    }

    return ec;
}

SYSCONF_API int32_t sysconf_get_native_node(sysconf_node **node)
{
    // 检查输入指针，设置默认值
    if (!node)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *node = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    if (!_sysconf->sm.native)
    {
        return SYSCONF_EC_NOEXIST;
    }

    *node = _sysconf->sm.native;

    return SYSCONF_EC_OK;
}

SYSCONF_API int32_t sysconf_get_process(int32_t type, sysconf_process **process)
{
    int32_t ec = SYSCONF_EC_NOEXIST;

    // 检查输入参数，设置默认值
    if (!process)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *process = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    // 查询进程列表，设置查询结果
    for (int32_t i = 0; i < _sysconf->sm.processNum; i++)
    {
        if (_sysconf->sm.processes[i].type == type)
        {
            *process = &_sysconf->sm.processes[i];
            ec = SYSCONF_EC_OK;
        }
    }

    return ec;
}

SYSCONF_API int32_t sysconf_get_system_manager(sysconf_system_manager **conf)
{
    if (!conf)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *conf = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    *conf = &_sysconf->sm;

    return SYSCONF_EC_OK;
}

SYSCONF_API int32_t sysconf_get_mq_client(sysconf_mq_client **conf)
{
    if (!conf)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *conf = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    *conf = &_sysconf->mqcli;

    return SYSCONF_EC_OK;
}

SYSCONF_API int32_t sysconf_get_rtdb_client(sysconf_rtdb_client **conf)
{
    if (!conf)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *conf = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    *conf = &_sysconf->rtdbcli;

    return SYSCONF_EC_OK;
}

SYSCONF_API int32_t sysconf_get_tsdb_client(sysconf_tsdb_client **conf)
{
    if (!conf)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *conf = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    *conf = &_sysconf->tsdbcli;

    return SYSCONF_EC_OK;
}

SYSCONF_API int32_t sysconf_get_cfdb_client(sysconf_cfdb_client **conf)
{
    if (!conf)
    {
        return SYSCONF_EC_NULLPTR;
    }
    *conf = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    *conf = &_sysconf->cfdbcli;

    return SYSCONF_EC_OK;
}

SYSCONF_API int32_t sysconf_get_pm_client(sysconf_pm_client **conf)
{
    if (!conf)
    {
        return SYSCONF_EC_NULLPTR;
    }

    *conf = NULL;

    if (!_sysconf)
    {
        return SYSCONF_EC_NOINIT;
    }

    *conf = &_sysconf->pmcli;

    return SYSCONF_EC_OK;
}
