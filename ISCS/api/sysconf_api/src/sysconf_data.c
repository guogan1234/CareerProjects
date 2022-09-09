/**
 * @file sysconf_data.c
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口。本接口文件定义系统全局配置用到的数据结构接口实现。
 *
 **/
#include "sysconf_data_private.h"
#include "sysconf_malloc.h"

void sysconf_clean_system_manager(sysconf_system_manager *conf)
{
    if (!conf)
    {
        return;
    }

    // 清理进程列表
    if (conf->processes)
    {
        sysconf_free(conf->processes);
    }

    // 清理域列表
    if (conf->domains)
    {
        for (int32_t i = 0; i < conf->domainNum; i++)
        {
            if (conf->domains[i].nodes)
            {
                sysconf_free(conf->domains[i].nodes);
            }
        }
        sysconf_free(conf->domains);
    }

    // 清理节点列表
    if (conf->nodes)
    {
        for (int32_t i = 0; i < conf->nodeNum; i++)
        {
            if (conf->nodes[i].nics)
            {
                sysconf_free(conf->nodes[i].nics);
            }
            if (conf->nodes[i].instances)
            {
                sysconf_free(conf->nodes[i].instances);
            }
        }
        sysconf_free(conf->nodes);
    }
}

void sysconf_clean_mq_client(sysconf_mq_client *conf)
{
    if (!conf)
    {
        return;
    }

    if (conf->nics)
    {
        sysconf_free(conf->nics);
    }

    if (conf->pairs)
    {
        sysconf_free(conf->pairs);
    }
}

void sysconf_clean_rtdb_client(sysconf_rtdb_client *conf)
{
    if (!conf)
    {
        return;
    }

    if (conf->nics)
    {
        sysconf_free(conf->nics);
    }

    if (conf->pairs)
    {
        sysconf_free(conf->pairs);
    }
}

void sysconf_clean_tsdb_client(sysconf_tsdb_client *conf)
{
    if ((conf) && (conf->nics))
    {
        sysconf_free(conf->nics);
    }
}

void sysconf_clean_cfdb_client(sysconf_cfdb_client *conf)
{
    if ((conf) && (conf->nics))
    {
        sysconf_free(conf->nics);
    }
}

void sysconf_clean_pm_client(sysconf_pm_client *conf)
{
    if ((conf) && (conf->nics))
    {
        sysconf_free(conf->nics);
    }
}
