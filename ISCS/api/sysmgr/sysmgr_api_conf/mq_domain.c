#include "load_conf.h"
#include "local_domain_info.h"
#include "conf_log.h"
#include "conf_parser_inner.h"

#include <stdio.h>
#include <string.h>

#include "sys_conf_global.h"
#include "cmn/cmn_log.h"

static mq_param_key_value* retrieve_keyvalue_params(mxml_node_t* mq_node);


/*
 * 功能：组合MQ服务器列表信息和域服务器列表IP信息
 * 说明：组合MQ服务器列表信息和域服务器列表IP信息
 * 参数：datas[in] - 域服务器列表IP信息
 *      list[in] - MQ服务器列表信息
 * 返回值：MQ服务器列表配置信息
 */
sys_conf_mq_server* sys_conf_match_combine_mq_server(sys_conf_domain_server* datas,sys_conf_mq_server_list* list){
    if(datas == NULL || list == NULL){
        return NULL;
    }
    if(list->size != datas->size){
#ifdef SYSDEBUG
		printf("Error:datas size not match local domain server list!\n");
#endif
		return NULL;
	}
    sys_conf_mq_server* temp_mq = NULL;
	temp_mq = list->first;
	
    sys_conf_domain_server* temp = NULL;
	temp = datas;
	while(temp != NULL){
		temp_mq->ip_1 = temp->net_1_ip;
		temp_mq->ip_2 = temp->net_2_ip;

		temp = temp->next;
		temp_mq = temp_mq->next;
	}

    sys_conf_mq_server* ret = NULL;
    ret = list->first;
    free(list);

    return ret;
}

/*
 * 功能：获取MQ服务器列表信息
 * 说明：获取MQ服务器列表信息
 * 参数：node[in] - MQ服务器列表信息的父节点
 *      xml[in] - xml文件指针
 * 返回值：MQ服务器列表信息
 */
sys_conf_mq_server_list* sys_conf_get_mq_servers(mxml_node_t* node,mxml_node_t* xml){
    if(node == NULL || xml == NULL){
        return NULL;
    }
    sys_conf_mq_server_list* ret = (sys_conf_mq_server_list*)malloc(sizeof(sys_conf_mq_server_list));
    //sys_conf_mq_server* first_server = ret->first;

	int size = 0;
    sys_conf_mq_server* prev = NULL;
    sys_conf_mq_server* first = NULL;
	mxml_node_t* tnode = mxmlFindElement(node,xml,"SERVER","server_id",NULL,MXML_DESCEND);
	do{
		if(tnode->type == MXML_ELEMENT){
            sys_conf_mq_server* temp = (sys_conf_mq_server*)malloc(sizeof(sys_conf_mq_server));
			temp->next = NULL;
			if(first == NULL){
				first = temp;		
			}else{
				prev->next = temp;
			}

			mxml_node_t* t = NULL;
			t = mxmlFindElement(node,xml,"SCHEMA",NULL,NULL,MXML_DESCEND);
            if(t){
                //char* schema = t->child->value.text.string;
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

			t = mxmlFindElement(node,xml,"POOL_SIZE",NULL,NULL,MXML_DESCEND);
            if(t){
                //sscanf(t->child->value.text.string,"%d",&(temp->pool_size));
                sscanf(t->child->value.opaque,"%d",&(temp->pool_size));
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
 * 功能：获取MQ域配置信息
 * 说明：获取MQ域配置信息
 * 参数：log_node[in] - 日志配置信息的父节点
 *      node[in] - MQ服务器列表信息的父节点
 *      xml[in] - xml文件指针
 *      d_servers[in] - 域服务器列表IP信息
 * 返回值：MQ域配置信息
 */
sys_conf_mq_domain* sys_conf_get_final_domain(mxml_node_t* log_node,mxml_node_t* node,mxml_node_t* xml,sys_conf_domain_server* d_servers){
    sys_conf_mq_server_list* servers = NULL;
    servers = sys_conf_get_mq_servers(node,xml);

    sys_conf_mq_server* final = NULL;
    final = sys_conf_match_combine_mq_server(d_servers,servers);

    sys_conf_log* log = NULL;
    log = sys_conf_get_log(log_node,xml);

    sys_conf_mq_domain* ret = (sys_conf_mq_domain*)malloc(sizeof(sys_conf_mq_domain));
    ret->first = final;
    ret->log = log;

    //delete xml
    mxmlDelete(xml);

    return ret;
}

/*
 * 功能：根据域编号，获取MQ指定域配置信息
 * 说明：根据域编号，获取MQ指定域配置信息
 * 参数：domain_id[in] - 域编号
 * 返回值：MQ域配置信息
 */
sys_conf_mq_domain* sys_conf_get_mq_domain_by_id(int domain_id){
    cmn_log_init(LOG_FILE,CMN_LOG_LEVEL_DEBUG,LOG_FILE_MAX_MB,LOG_FILE_NUM);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_mq_domain_by_id](start)");
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

    mq_param_key_value* params = retrieve_keyvalue_params(p_node);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_final_domain](start)");
    sys_conf_mq_domain* ret = NULL;
    ret = sys_conf_get_final_domain(p_node,node,xml,domain_servers);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_final_domain](end)");

    if( (params) && (ret) )
    {
        ret->params = params;
    }

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_mq_domain_by_id](end)");
    return ret;
}

/*
 * 功能：获取MQ本域配置信息
 * 说明：获取MQ本域配置信息
 * 参数：无
 * 返回值：MQ域配置信息
 */
sys_conf_mq_domain* sys_conf_get_mq_domain(){
    cmn_log_init(LOG_FILE,CMN_LOG_LEVEL_DEBUG,LOG_FILE_MAX_MB,LOG_FILE_NUM);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_mq_domain](start)");
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](start)");
    mxml_node_t* xml = sys_conf_load_conf_file();
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](end)");
	if(xml == NULL){
		return NULL;
	}    

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_id_char](start)");
    char* domain_id = NULL;
    domain_id = sys_conf_get_domain_id_char(xml);
    if(domain_id == NULL){
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

	//
	mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","mq",MXML_DESCEND);
	mxml_node_t* node = mxmlFindElement(p_node,xml,"DOMAIN_SERVER","domain_id",domain_id,MXML_DESCEND);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](end)");

    mq_param_key_value* params = retrieve_keyvalue_params(p_node);
    
    sys_conf_mq_domain* ret = NULL;
    ret = sys_conf_get_final_domain(p_node,node,xml,worker.datas);
//    ret->params = params;
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_final_domain](end)");

    if( (params) && (ret) )
    {
        ret->params = params;
    }
    
	return ret;
}

mq_param_key_value* retrieve_keyvalue_params(mxml_node_t* mq_node)
{
    mq_param_key_value *retval = NULL, *lstval = NULL;
    
    if(mq_node)
    {
        mxml_node_t* param_node = mxmlFindElement(mq_node,mq_node,"STRINGVALUE_PAIRS",NULL,NULL,MXML_DESCEND);
        if(param_node)
        {
            mxml_node_t* firstNode = mxmlGetFirstChild(param_node);
            mxml_node_t* nextNode = firstNode;
            while(nextNode)
            {
                mxml_type_t mtype = mxmlGetType(nextNode);
                if(mtype == MXML_ELEMENT)
                {
                    const char* strkey = mxmlElementGetAttr(nextNode, "key");
                    const char* strval = mxmlElementGetAttr(nextNode, "value");
                    
                    if( (strkey) && (strval) )
                    {
                        mq_param_key_value* newval = (mq_param_key_value*)malloc(sizeof(mq_param_key_value));
                        newval->strkey = strdup(strkey);
                        newval->strvalue = strdup(strval);
                        newval->next = NULL;
                        
                        if(lstval)
                        {
                            lstval->next = newval;
                        }
                        
                        if(retval == NULL)
                        {
                            retval = newval;
                            lstval = newval;
                        }
                        
                        lstval = newval;
                    }
                }
                nextNode = mxmlGetNextSibling(nextNode);
            }
        }
    }
    
    return retval;
}

/*
 * 功能：释放MQ域配置信息
 * 说明：释放MQ域配置信息
 * 参数：MQ域配置信息
 * 返回值：结果码
 */
int sys_conf_free_mq_domain(sys_conf_mq_domain* data){
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_mq_domain](start)");
    sys_conf_log* log = data->log;
    if(log){
        if(log->file){
            free(log->file);
        }
        free(log);
    }

    sys_conf_mq_server* temp = data->first;
    while(temp){
        if(temp->schema){
            free(temp->schema);
        }
        if(temp->username){
            free(temp->username);
        }
        if(temp->password){
            free(temp->password);
        }
        if(temp->ip_1){
            free(temp->ip_1);
        }
        if(temp->ip_2){
            free(temp->ip_2);
        }
        sys_conf_mq_server* next = temp->next;
        free(temp);
        temp = next;
    }  
    
    mq_param_key_value* string_pairs = NULL;
    string_pairs = data->params;
#ifdef SYSDEBUG
    printf("free:address - %p\n",string_pairs);
#endif
    while(string_pairs) {
        if(string_pairs->strkey){
#ifdef SYSDEBUG
            printf("free:strkey - %s\n",string_pairs->strkey);
#endif
            free(string_pairs->strkey);
        }
        
        if(string_pairs->strvalue){
#ifdef SYSDEBUG
            printf("free:strvalue - %s\n",string_pairs->strvalue);
#endif
            free(string_pairs->strvalue);
        }
        
        mq_param_key_value* next = string_pairs->next;
        free(string_pairs);
        string_pairs = next;
    }
    free(data);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_mq_domain](end)");
    cmn_log_uninit();
    return 0;
}
