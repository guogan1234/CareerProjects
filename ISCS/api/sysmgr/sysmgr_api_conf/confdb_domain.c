#include "conf_parser.h"
#include "load_conf.h"
#include "local_domain_info.h"
#include "conf_log.h"

#include <stdio.h>
#include <string.h>

#include "sys_conf_global.h"
#include "cmn/cmn_log.h"

/*
 * 功能：获取配置库服务器列表信息
 * 说明：获取配置库服务器列表信息
 * 参数：node[in] - 配置库服务器列表信息的父节点
 *      xml[in] - xml配置文件指针
 * 返回值：配置库服务器列表信息
 */
sys_conf_confdb_server* sys_conf_get_confdb_servers(mxml_node_t* node, mxml_node_t* xml){
    if(!node || !xml) return NULL;

    sys_conf_confdb_server* confdbServer = (sys_conf_confdb_server*)malloc(sizeof(sys_conf_confdb_server));
    memset(confdbServer, 0x00, sizeof(sys_conf_confdb_server));

    mxml_node_t* temp = NULL;

    // http url
    temp = mxmlFindElement(node,xml,"HTTP_URL",NULL,NULL,MXML_DESCEND);
    if(temp) {
        //confdbServer->http_url = strdup(temp->child->value.text.string);
        confdbServer->http_url = strdup(temp->child->value.opaque);
    }

    //rtdb sync ip
    temp = mxmlFindElement(node,xml,"RTDB_SYNC_IP",NULL,NULL,MXML_DESCEND);
    if(temp) {
        //confdbServer->rtdb_sync_ip = strdup(temp->child->value.text.string);
        confdbServer->rtdb_sync_ip = strdup(temp->child->value.opaque);
    }

    // rtdb sync port
    temp = mxmlFindElement(node,xml,"RTDB_SYNC_PORT",NULL,NULL,MXML_DESCEND);
    if(temp) {
        //sscanf(temp->child->value.text.string,"%d",&(confdbServer->rtdb_sync_port));
        sscanf(temp->child->value.opaque,"%d",&(confdbServer->rtdb_sync_port));
    }

    return confdbServer;
}

/*
 * 功能：获取配置库本域配置信息
 * 说明：获取配置库本域配置信息
 * 参数：无
 * 返回值：配置库域配置信息
 */
sys_conf_confdb_domain* sys_conf_get_confdb_domain(){
    cmn_log_init(LOG_FILE,CMN_LOG_LEVEL_DEBUG,LOG_FILE_MAX_MB,LOG_FILE_NUM);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_confdb_domain](start)");
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](start)");
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL) return NULL;
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_id_char](start)");
    char* domain_id = sys_conf_get_domain_id_char(xml);
    if(!domain_id) {
        mxmlDelete(xml);
        return NULL;
    }
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_id_char](end)");

    //get local domain node
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](start)");
    mxml_node_t* p_node = mxmlFindElement(xml, xml, "MODEL","name", "confdb", MXML_DESCEND);
    if(!p_node) {
        mxmlDelete(xml);
        return NULL;
    }

    sys_conf_confdb_domain* domain = (sys_conf_confdb_domain*)malloc(sizeof(sys_conf_confdb_domain));
    memset(domain, 0x00, sizeof(sys_conf_confdb_domain));

    mxml_node_t* node = mxmlFindElement(p_node, xml, "DOMAIN_SERVER", "domain_id", domain_id, MXML_DESCEND);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](end)");

    // load local domain server
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_confdb_servers](start)");
    domain->first = sys_conf_get_confdb_servers(node,xml);
    domain->domain_id = atoi(domain_id);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"domain->domain_id(%d)",domain->domain_id);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_confdb_servers](end)");
    //confdb不需要其他域到信息

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_log](start)");
    domain->log = sys_conf_get_log(p_node,xml);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_log](end)");

    mxmlDelete(xml);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_confdb_domain](end)");
    return domain;
}

sys_conf_confdb_server* sys_conf_confdb_server_get_local(const sys_conf_confdb_domain* confdb) {
    if(confdb) return confdb->first;

    return NULL;
}

sys_conf_log* sys_conf_confdb_log(const sys_conf_confdb_domain* confdb) {
    if(confdb) return confdb->log;

    return NULL;
}

/*
 * 功能：释放配置库域配置信息
 * 说明：释放配置库域配置信息
 * 参数：配置库域配置信息
 * 返回值：结果码
 */
int sys_conf_free_confdb_domain(sys_conf_confdb_domain* data){
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_confdb_domain](start)");

    if(data) {
        if(data->log) {
            if(data->log->file) free(data->log->file);
            free(data->log);
        }
        if(data->first) {
            if(data->first->http_url) free(data->first->http_url);
            if(data->first->rtdb_sync_ip) free(data->first->rtdb_sync_ip);
            free(data->first);
        }
    }
    free(data);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_confdb_domain](end)");
    cmn_log_uninit();
    return 0;
}
