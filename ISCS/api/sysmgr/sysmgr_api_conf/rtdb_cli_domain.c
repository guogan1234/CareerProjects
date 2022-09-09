#include "load_conf.h"
#include "local_domain_info.h"
#include "conf_log.h"
#include "conf_parser_inner.h"

#include "sys_conf_global.h"
#include "cmn/cmn_log.h"

/*
 * 功能：组合实时库服务器列表信息和域服务器列表IP信息
 * 说明：组合实时库服务器列表信息和域服务器列表IP信息
 * 参数：datas[in] - 域服务器列表IP信息
 *      list[in] - 实时库服务器列表信息
 * 返回值：实时库服务器列表配置信息
 */
sys_conf_rtdb_server* sys_conf_match_combine_rtdb_server(sys_conf_domain_server* datas,sys_conf_rtdb_server_list* list){
    if(datas == NULL || list == NULL){
        return NULL;
    }
    sys_conf_domain_server* temp = NULL;
	temp = datas;
    sys_conf_rtdb_server* temp_servers = NULL;
    temp_servers = list->first;
	
	//size not match
    if(list->size != temp->size){
#ifdef SYSDEBUG
        PRINT("Error:datas size not match local domain server list!\n");
#endif
		return NULL;	
	}

	while(temp != NULL){
		temp_servers->ip1 = temp->net_1_ip;
		temp_servers->ip2 = temp->net_2_ip;

		temp = temp->next;
		temp_servers = temp_servers->next;
	}
	
    return list->first;
}

/*
 * 功能：获取实时库服务器列表信息
 * 说明：获取实时库服务器列表信息
 * 参数：node[in] - 实时库服务器列表信息的父节点
 *      xml[in] - xml文件指针
 * 返回值：实时库服务器列表信息
 */
sys_conf_rtdb_server_list* sys_conf_get_rtdb_server(mxml_node_t* node,mxml_node_t* xml){
    if(node == NULL || xml == NULL){
        return NULL;
    }
    sys_conf_rtdb_server_list* ret = (sys_conf_rtdb_server_list*)malloc(sizeof(sys_conf_rtdb_server_list));

    sys_conf_rtdb_server* prev = NULL;
    sys_conf_rtdb_server* first = NULL;
	int size = 0;
	mxml_node_t* tnode = mxmlFindElement(node,xml,"SERVER","server_id",NULL,MXML_DESCEND);
	do{
		if(tnode->type == MXML_ELEMENT){
        sys_conf_rtdb_server* servers = (sys_conf_rtdb_server*)malloc(sizeof(sys_conf_rtdb_server));//not free
		servers->next = NULL;
		if(first ==NULL){
			first = servers;
		}
		if(prev != NULL){
			prev->next = servers;
		}

		mxml_node_t* t = NULL;
		t = mxmlFindElement(tnode,xml,"BASE_PORT",NULL,NULL,MXML_DESCEND);
        if(t){
            //sscanf(t->child->value.text.string,"%d",&(servers->port));
            sscanf(t->child->value.opaque,"%d",&(servers->basePort));
        }
        
        t = mxmlFindElement(tnode,xml,"MODE_PORT",NULL,NULL,MXML_DESCEND);
        if(t){
            //sscanf(t->child->value.text.string,"%d",&(servers->port));
            sscanf(t->child->value.opaque,"%d",&(servers->modePort));
        }

		t = mxmlFindElement(tnode,xml,"USERNAME",NULL,NULL,MXML_DESCEND);
        if(t){
            //servers->username = strdup(t->child->value.text.string);
            servers->username = strdup(t->child->value.opaque);
        }

		t = mxmlFindElement(tnode,xml,"PASSWORD",NULL,NULL,MXML_DESCEND);
        if(t){
            //servers->password = strdup(t->child->value.text.string);
            servers->password = strdup(t->child->value.opaque);
        }

		prev = servers;
		size++;
		}
		tnode = tnode->next;
	}while(tnode);
    ret->first = first;
    ret->size = size;

    return ret;
}

/*
 * 功能：获取实时库本域配置信息
 * 说明：获取实时库本域配置信息
 * 参数：无
 * 返回值：实时库域配置信息
 */
sys_conf_rtdb_domain* sys_conf_get_rtdb_domain(){
    cmn_log_init(LOG_FILE,CMN_LOG_LEVEL_DEBUG,LOG_FILE_MAX_MB,LOG_FILE_NUM);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_rtdb_domain](start)");
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](start)");
    mxml_node_t* xml = sys_conf_load_conf_file();
	if(xml == NULL){
		return NULL;
	}
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_id_char](start)");
    char* domain_id = sys_conf_get_domain_id_char(xml);
    if(!domain_id){
        return NULL;
    }
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_id_char](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](start)");
	//get common domain server list
    sys_conf_local_domain_worker worker;
    worker.get_id = sys_conf_get_domain_id;
    worker.get_servers = sys_conf_get_domain_servers;

	worker.local_domain_id = worker.get_id(xml);
	worker.datas = worker.get_servers(xml);
		
	//
	mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","rtdb-cli",MXML_DESCEND);
	mxml_node_t* node = mxmlFindElement(p_node,xml,"DOMAIN_SERVER","domain_id",domain_id,MXML_DESCEND);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](end)");

	//combine return datas
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_rtdb_server](start)");
    sys_conf_rtdb_server_list* list = NULL;
    list = sys_conf_get_rtdb_server(node,xml);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_rtdb_server](end)");
	
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_match_combine_rtdb_server](start)");
    sys_conf_rtdb_server* final = NULL;
    final = sys_conf_match_combine_rtdb_server(worker.datas,list);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_match_combine_rtdb_server](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_log](start)");
    sys_conf_log* log = NULL;
    log = sys_conf_get_log(p_node,xml);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_log](end)");

    sys_conf_rtdb_domain* ret = (sys_conf_rtdb_domain*)malloc(sizeof(sys_conf_rtdb_domain));
    ret->servers = final;
    ret->log = log;

    //delete xml
    mxmlDelete(xml);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_rtdb_domain](end)");
    return ret;
}

/*
 * 功能：释放实时库域配置信息
 * 说明：释放实时库域配置信息
 * 参数：实时库域配置信息
 * 返回值：结果码
 */
int sys_conf_free_rtdb_domain(sys_conf_rtdb_domain* data){
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_rtdb_domain](end)");
    sys_conf_log* log = data->log;
    if(log){
        if(log->file){
            free(log->file);
        }
        free(log);
    }

    sys_conf_rtdb_server* temp = data->servers;
    while(temp){
        if(temp->username){
            free(temp->username);
        }
        if(temp->password){
            free(temp->password);
        }
        if(temp->ip1){
            free(temp->ip1);
        }
        if(temp->ip2){
            free(temp->ip2);
        }
        sys_conf_rtdb_server* next = temp->next;
        free(temp);
        temp = next;
    }
    free(data);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_confdb_domain](end)");
    cmn_log_uninit();
    return 0;
}
