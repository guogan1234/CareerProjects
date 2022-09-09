#include "load_conf.h"
#include "local_domain_info.h"
#include "conf_log.h"
#include "conf_parser_inner.h"

#include <stdio.h>
#include <string.h>

#include "sys_conf_global.h"
#include "cmn/cmn_log.h"

/*
 * 功能：组合历史库服务器列表信息和域服务器列表IP信息
 * 说明：组合历史库服务器列表信息和域服务器列表IP信息
 * 参数：datas[in] - 域服务器列表IP信息
 *      list[in] - 历史库服务器列表信息
 * 返回值：历史库服务器列表配置信息
 */
sys_conf_tsdb_server* sys_conf_match_combine_tsdb_server(sys_conf_domain_server* datas,sys_conf_tsdb_server_list* list){
    if(datas == NULL || list == NULL){
        return NULL;
    }

    if(list->size != datas->size){
#ifdef SYSDEBUG
		printf("Error:datas size not match local domain server list!\n");
#endif
		return NULL;
	}
    sys_conf_tsdb_server* temp_tsdb = NULL;
	temp_tsdb = list->first;
	
    sys_conf_domain_server* temp = NULL;
	temp = datas;
	while(temp != NULL){
		temp_tsdb->ip_1 = temp->net_1_ip;
		temp_tsdb->ip_2 = temp->net_2_ip;

		temp = temp->next;
		temp_tsdb = temp_tsdb->next;
	}

	return list->first;	
}

/*
 * 功能：获取历史库服务器列表信息
 * 说明：获取历史库服务器列表信息
 * 参数：node[in] - 历史库服务器列表信息的父节点
 *      xml[in] - xml文件指针
 * 返回值：历史库服务器列表信息
 */
sys_conf_tsdb_server_list* sys_conf_get_tsdb_servers(mxml_node_t* node,mxml_node_t* xml){
    if(node == NULL || xml == NULL){
        return NULL;
    }

    sys_conf_tsdb_server_list* ret = (sys_conf_tsdb_server_list*)malloc(sizeof(sys_conf_tsdb_server_list));
	
	int size = 0;
    sys_conf_tsdb_server* prev = NULL;
    sys_conf_tsdb_server* first = NULL;
	mxml_node_t* tnode = mxmlFindElement(node,xml,"SERVER","server_id",NULL,MXML_DESCEND);
	do{
		if(tnode->type == MXML_ELEMENT){
            sys_conf_tsdb_server* temp = malloc(sizeof(sys_conf_tsdb_server));
			temp->next = NULL;
			if(first == NULL){
				first = temp;		
			}else{
				prev->next = temp;
			}

			mxml_node_t* t = NULL;
			t = mxmlFindElement(node,xml,"SCHEMA",NULL,NULL,MXML_DESCEND);
            if(t){
                //temp->schema = strdup(t->child->value.text.string);
                temp->schema = strdup(t->child->value.opaque);
            }

			t = mxmlFindElement(node,xml,"USERNAME",NULL,NULL,MXML_DESCEND);
            if(t){
                //temp->username = strdup(t->child->value.text.string);
                temp->username = strdup(t->child->value.opaque);
            }

			t = mxmlFindElement(node,xml,"PASSWORD",NULL,NULL,MXML_DESCEND);
            if(t){
                //temp->password = strdup(t->child->value.text.string);
                temp->password = strdup(t->child->value.opaque);
            }

			t = mxmlFindElement(node,xml,"database",NULL,NULL,MXML_DESCEND);
            if(t){
                //temp->database = strdup(t->child->value.text.string);
                temp->database = strdup(t->child->value.opaque);
            }

			t = mxmlFindElement(node,xml,"PORT",NULL,NULL,MXML_DESCEND);
            if(t){
                //sscanf(t->child->value.text.string,"%d",&(temp->port));
                sscanf(t->child->value.opaque,"%d",&(temp->port));
            }

			t = mxmlFindElement(node,xml,"TIMEOUT",NULL,NULL,MXML_DESCEND);
            if(t){
                //sscanf(t->child->value.text.string,"%d",&(temp->timeout));
                sscanf(t->child->value.opaque,"%d",&(temp->timeout));
            }

			t = mxmlFindElement(node,xml,"HEART_BEAT_INTERNAL",NULL,NULL,MXML_DESCEND);
            if(t){
                //sscanf(t->child->value.text.string,"%d",&(temp->beat_internal));
                sscanf(t->child->value.opaque,"%d",&(temp->beat_internal));
            }
	
			prev = temp;
			size++;
		}
		tnode = tnode->next;
	}while(tnode);
	ret->size = size;
	ret->first = first;
	return ret;
}

/*
 * 功能：获取历史库域配置信息
 * 说明：获取历史库域配置信息
 * 参数：log_node[in] - 日志配置信息的父节点
 *      node[in] - 历史库服务器列表信息的父节点
 *      xml[in] - xml文件指针
 *      d_servers[in] - 域服务器列表IP信息
 * 返回值：历史库域配置信息
 */
sys_conf_tsdb_domain* sys_conf_get_final_tsdb_domain(mxml_node_t* log_node,mxml_node_t* node,mxml_node_t* xml,sys_conf_domain_server* d_servers){
    sys_conf_tsdb_server_list* servers = NULL;
    servers = sys_conf_get_tsdb_servers(node,xml);

    sys_conf_tsdb_server* final = NULL;
    final = sys_conf_match_combine_tsdb_server(d_servers,servers);

    sys_conf_log* log = NULL;
    log = sys_conf_get_log(log_node,xml);

    sys_conf_tsdb_domain* ret = (sys_conf_tsdb_domain*)malloc(sizeof(sys_conf_tsdb_domain));
    ret->first = final;
    ret->log = log;

    //delete xml
    mxmlDelete(xml);

    return ret;
}

/*
 * 功能：获取MQ本域配置信息
 * 说明：获取MQ本域配置信息
 * 参数：无
 * 返回值：MQ域配置信息
 */
sys_conf_tsdb_domain* sys_conf_get_tsdb_domain(){
    cmn_log_init(LOG_FILE,CMN_LOG_LEVEL_DEBUG,LOG_FILE_MAX_MB,LOG_FILE_NUM);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_tsdb_domain](start)");
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](start)");
    mxml_node_t* xml = sys_conf_load_conf_file();
	if(xml == NULL){
		return NULL;
	}
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_id_char](start)");
    char* domain_id = NULL;
    domain_id = sys_conf_get_domain_id_char(xml);
    if(!domain_id){
        return NULL;
    } 
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_id_char](end)");

	//get common domain server list
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](start)");
    sys_conf_local_domain_worker worker;
    worker.get_id = sys_conf_get_domain_id;
    worker.get_servers = sys_conf_get_domain_servers;

	worker.local_domain_id = worker.get_id(xml);
	worker.datas = worker.get_servers(xml);

	mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","tsdb",MXML_DESCEND);
	mxml_node_t* node = mxmlFindElement(p_node,xml,"DOMAIN_SERVER","domain_id",domain_id,MXML_DESCEND);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_final_tsdb_domain](start)");
    sys_conf_tsdb_domain* ret = NULL;
    ret = sys_conf_get_final_tsdb_domain(p_node,node,xml,worker.datas);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_final_tsdb_domain](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_tsdb_domain](end)");
    return ret;
}

/*
 * 功能：根据域编号，获取历史库指定域配置信息
 * 说明：根据域编号，获取历史库指定域配置信息
 * 参数：domain_id[in] - 域编号
 * 返回值：历史库域配置信息
 */
sys_conf_tsdb_domain* sys_conf_get_tsdb_domain_by_id(int domain_id){
    cmn_log_init(LOG_FILE,CMN_LOG_LEVEL_DEBUG,LOG_FILE_MAX_MB,LOG_FILE_NUM);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_tsdb_domain_by_id](start)");
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](start)");
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return NULL;
    }
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_servers_by_id](start)");
    sys_conf_domain_server* domain_servers = NULL;
    domain_servers = sys_conf_get_domain_servers_by_id(xml,domain_id);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_servers_by_id](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](start)");
    char id[10] = {0};
    int n = sprintf(id,"%d",domain_id);
    if(-1 == n){
        return NULL;
    }

    mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","mq",MXML_DESCEND);
    mxml_node_t* node = mxmlFindElement(p_node,xml,"DOMAIN_SERVER","domain_id",id,MXML_DESCEND);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_final_tsdb_domain](start)");
    sys_conf_tsdb_domain* ret = NULL;
    ret = sys_conf_get_final_tsdb_domain(p_node,node,xml,domain_servers);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_final_tsdb_domain](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_tsdb_domain_by_id](end)");
    return ret;
}

/*
 * 功能：释放历史库域配置信息
 * 说明：释放历史库域配置信息
 * 参数：历史库域配置信息
 * 返回值：结果码
 */
int sys_conf_free_tsdb_domain(sys_conf_tsdb_domain* data) {
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_tsdb_domain](start)");
    if(data) {
        if(data->log) {
            if(data->log->file) free(data->log->file);
            free(data->log);
        }
        sys_conf_tsdb_server* server = data->first;
        while(server) {
            if(server->schema) free(server->schema);
            if(server->username) free(server->username);
            if(server->password) free(server->password);
            if(server->database) free(server->database);
            if(server->ip_1) free(server->ip_1);
            if(server->ip_2) free(server->ip_2);

            sys_conf_tsdb_server* next = server->next;
            free(server);
            server = next;
        }
    }
    free(data);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_tsdb_domain](end)");
    cmn_log_uninit();
    return 0;
}
