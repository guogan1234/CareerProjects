#include "conf_parser.h"
#include "load_conf.h"
#include "sys_conf_global.h"
#include "cmn/cmn_log.h"
#include "local_domain_info.h"

#include <stdlib.h>
#include <string.h>

/*
 * 功能：获取系统配置域进程信息
 * 说明：获取系统配置域进程信息
 * 参数：p_node[in] - 系统配置域进程信息的父节点
 *      xml[in] - xml配置文件指针
 * 返回值：系统配置域进程信息
 */
sys_conf_system_domain_process* sys_conf_get_domain_process(mxml_node_t* p_node,mxml_node_t* xml){
    if(p_node == NULL || xml== NULL){
        return NULL;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,"DOMAIN_PROCESS",NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return NULL;
    }

    mxml_node_t* node = mxmlFindElement(t_node,xml,"DOMAIN","id",NULL,MXML_DESCEND);
    if(node == NULL){
        return NULL;
    }

    sys_conf_system_domain_process* first = NULL;
    sys_conf_system_domain_process* prev = NULL;
    do{
        if(node->type == MXML_ELEMENT){
            sys_conf_system_domain_process* temp = (sys_conf_system_domain_process*)malloc(sizeof(sys_conf_system_domain_process));
            memset(temp,'\0',sizeof(sys_conf_system_domain_process));
            temp->next = NULL;

            mxml_node_t* temp_node = NULL;
            temp_node = mxmlFindElement(node,xml,"PROCESS_SUB",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->process_sub = strdup(temp_node->child->value.text.string);
                temp->process_sub = strdup(temp_node->child->value.opaque);
            }
            const char* domain_id = mxmlElementGetAttr(node,"id");
            sscanf(domain_id,"%d",&temp->domain_id);

            if(first == NULL){
                first = temp;
            }
            if(prev != NULL){
                prev->next = temp;
            }
            prev = temp;
        }
        node = node->next;
    }while(node);

    return first;
}

/*
 * 功能：获取系统配置进程列表信息
 * 说明：获取系统配置进程列表信息
 * 参数：p_node[in] - 系统配置进程列表信息的父节点
 *      xml[in] - xml配置文件指针
 * 返回值：系统配置进程列表信息
 */
sys_conf_system_process* sys_conf_get_system_process(mxml_node_t* p_node,mxml_node_t* xml){
    if(p_node == NULL || xml== NULL){
        return NULL;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,"PROCESSES",NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return NULL;
    }

    mxml_node_t* node = mxmlFindElement(t_node,xml,"PROCESS","id",NULL,MXML_DESCEND);
    if(node == NULL){
        return NULL;
    }

    sys_conf_system_process* first = NULL;
    sys_conf_system_process* prev = NULL;
    do{
        if(node->type == MXML_ELEMENT){
            sys_conf_system_process* temp = (sys_conf_system_process*)malloc(sizeof(sys_conf_system_process));
            memset(temp,'\0',sizeof(sys_conf_system_process));
            temp->next = NULL;

            const char* process_id = mxmlElementGetAttr(node,"id");
            sscanf(process_id,"%d",&temp->id);

            mxml_node_t* temp_node = NULL;
            temp_node = mxmlFindElement(node,xml,"ALIAS",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->alias = strdup(temp_node->child->value.text.string);
                temp->alias = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"COMMAND",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->command = strdup(temp_node->child->value.text.string);
                temp->command = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"INSTANCE_NUM",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->instance_num);
                sscanf(temp_node->child->value.opaque,"%d",&temp->instance_num);
            }

            temp_node = mxmlFindElement(node,xml,"START_TYPE",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->start_type);
                sscanf(temp_node->child->value.opaque,"%d",&temp->start_type);
            }

            temp_node = mxmlFindElement(node,xml,"AUTORUN",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->autorun);
                sscanf(temp_node->child->value.opaque,"%d",&temp->autorun);
            }

            temp_node = mxmlFindElement(node,xml,"CYCLE_START",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->cycle_start);
                sscanf(temp_node->child->value.opaque,"%d",&temp->cycle_start);
            }

            temp_node = mxmlFindElement(node,xml,"CYCLE_PERIOD",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->cycle_period);
                sscanf(temp_node->child->value.opaque,"%d",&temp->cycle_period);
            }

            temp_node = mxmlFindElement(node,xml,"OFFLINE_RUN",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->offline_run);
                sscanf(temp_node->child->value.opaque,"%d",&temp->offline_run);
            }

            temp_node = mxmlFindElement(node,xml,"DUTY_RUN",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->duty_run);
                sscanf(temp_node->child->value.opaque,"%d",&temp->duty_run);
            }

            temp_node = mxmlFindElement(node,xml,"STANDBY_RUN",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->standby_run);
                sscanf(temp_node->child->value.opaque,"%d",&temp->standby_run);
            }

            temp_node = mxmlFindElement(node,xml,"RUN_ORDER",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->run_order);
                sscanf(temp_node->child->value.opaque,"%d",&temp->run_order);
            }

            temp_node = mxmlFindElement(node,xml,"REPORT_TYPE",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->report_type);
                sscanf(temp_node->child->value.opaque,"%d",&temp->report_type);
            }

            temp_node = mxmlFindElement(node,xml,"FILE_PATH",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->file_path = strdup(temp_node->child->value.text.string);
                temp->file_path = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"PARA",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->para = strdup(temp_node->child->value.text.string);
                temp->para = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"DESCRIPTION",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->description = strdup(temp_node->child->value.text.string);
                temp->description = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"RUN_STYLE",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->run_style);
                sscanf(temp_node->child->value.opaque,"%d",&temp->run_style);
            }

            temp_node = mxmlFindElement(node,xml,"PROC_TYPE",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->proc_type);
                sscanf(temp_node->child->value.opaque,"%d",&temp->proc_type);
            }

            if(first == NULL){
                first = temp;
            }
            if(prev != NULL){
                prev->next = temp;
            }
            prev = temp;
        }
        node = node->next;
    }while(node);

    return first;
}

/*
 * 功能：获取系统配置节点列表信息
 * 说明：获取系统配置节点列表信息
 * 参数：p_node[in] - 系统配置节点列表信息的父节点
 *      xml[in] - xml配置文件指针
 * 返回值：系统配置节点列表信息
 */
sys_conf_system_node* sys_conf_get_system_node(mxml_node_t* p_node,mxml_node_t* xml){
    if(p_node == NULL || xml== NULL){
        return NULL;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,"NODES",NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return NULL;
    }

    mxml_node_t* node = mxmlFindElement(t_node,xml,"NODE","id",NULL,MXML_DESCEND);
    if(node == NULL){
        return NULL;
    }

    sys_conf_system_node* first = NULL;
    sys_conf_system_node* prev = NULL;
    do{
        if(node->type == MXML_ELEMENT){
            sys_conf_system_node* temp = (sys_conf_system_node*)malloc(sizeof(sys_conf_system_node));
            memset(temp,'\0',sizeof(sys_conf_system_node));
            temp->next = NULL;

            const char* id = mxmlElementGetAttr(node,"id");
            sscanf(id,"%d",&temp->id);
            const char* domain_id = mxmlElementGetAttr(node,"domain_id");
            sscanf(domain_id,"%d",&temp->domain_id);
            mxml_node_t* temp_node = NULL;
            temp_node = mxmlFindElement(node,xml,"TYPE",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->type);
                sscanf(temp_node->child->value.opaque,"%d",&temp->type);
            }

            temp_node = mxmlFindElement(node,xml,"NAME",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->name = strdup(temp_node->child->value.text.string);
                temp->name = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NAME_CH",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->name_ch = strdup(temp_node->child->value.text.string);
                temp->name_ch = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"STATUS",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->status);
                sscanf(temp_node->child->value.opaque,"%d",&temp->status);
            }

            temp_node = mxmlFindElement(node,xml,"NIC_NUM",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->nic_num);
                sscanf(temp_node->child->value.opaque,"%d",&temp->nic_num);
            }

            temp_node = mxmlFindElement(node,xml,"NIC1_NAME",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->nic_1_name = strdup(temp_node->child->value.text.string);
                temp->nic_1_name = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NIC1_NETMASK",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->nic_1_netmask = strdup(temp_node->child->value.text.string);
                temp->nic_1_netmask = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NIC1_ADDRESS",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->nic_1_address = strdup(temp_node->child->value.text.string);
                temp->nic_1_address = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NIC2_NAME",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->nic_2_name = strdup(temp_node->child->value.text.string);
                temp->nic_2_name = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NIC2_NETMASK",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->nic_2_netmask = strdup(temp_node->child->value.text.string);
                temp->nic_2_netmask = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NIC2_ADDRESS",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->nic_2_address = strdup(temp_node->child->value.text.string);
                temp->nic_2_address = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NET_STATUS",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->net_status);
                sscanf(temp_node->child->value.opaque,"%d",&temp->net_status);
            }

            temp_node = mxmlFindElement(node,xml,"DEV",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->dev = strdup(temp_node->child->value.text.string);
                temp->dev = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"OS",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->os = strdup(temp_node->child->value.text.string);
                temp->os = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"LOCATION",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->location = strdup(temp_node->child->value.text.string);
                temp->location = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"DOMAIN_INFO",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->net_status);
                sscanf(temp_node->child->value.opaque,"%d",&temp->domain_info);
            }

            if(first == NULL){
                first = temp;
            }
            if(prev != NULL){
                prev->next = temp;
            }
            prev = temp;
        }
        node = node->next;
    }while(node);

    return first;
}

/*
 * 功能：获取系统配置域列表信息
 * 说明：获取系统配置域列表信息
 * 参数：p_node[in] - 系统配置域列表信息的父节点
 *      xml[in] - xml配置文件指针
 * 返回值：系统配置域列表信息
 */
sys_conf_system_domain* sys_conf_get_system_domain(mxml_node_t* p_node,mxml_node_t* xml){
    if(p_node==NULL || xml==NULL){
        return NULL;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,"DOMAINS",NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return NULL;
    }

    mxml_node_t* node = mxmlFindElement(t_node,xml,"DOMAIN","id",NULL,MXML_DESCEND);
    if(node == NULL){
        return NULL;
    }

    sys_conf_system_domain* first = NULL;
    sys_conf_system_domain* prev = NULL;
    do{
        if(node->type == MXML_ELEMENT){
            sys_conf_system_domain* temp = (sys_conf_system_domain*)malloc(sizeof(sys_conf_system_domain));
            memset(temp,'\0',sizeof(sys_conf_system_domain));
            temp->next = NULL;

            const char* id = mxmlElementGetAttr(node,"id");
            sscanf(id,"%d",&temp->id);
            mxml_node_t* temp_node = NULL;
            temp_node = mxmlFindElement(node,xml,"NAME",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->name = strdup(temp_node->child->value.text.string);
                temp->name = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"NAME_CH",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //temp->name_ch = strdup(temp_node->child->value.text.string);
                temp->name_ch = strdup(temp_node->child->value.opaque);
            }

            temp_node = mxmlFindElement(node,xml,"DOMAIN_TYPE",NULL,NULL,MXML_DESCEND);
            if(temp_node){
                //sscanf(temp_node->child->value.text.string,"%d",&temp->domain_type);
                sscanf(temp_node->child->value.opaque,"%d",&temp->domain_type);
            }

            if(first == NULL){
                first = temp;
            }
            if(prev != NULL){
                prev->next = temp;
            }
            prev = temp;
        }
        node = node->next;
    }while(node);

    return first;
}

/*
 * 功能：获取系统配置配置信息
 * 说明：获取系统配置配置信息
 * 参数：无
 * 返回值：系统配置配置信息
 */
sys_conf_system_manager_domain* sys_conf_get_system_manager_domain(){
    cmn_log_init(LOG_FILE,CMN_LOG_LEVEL_DEBUG,LOG_FILE_MAX_MB,LOG_FILE_NUM);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_manager_domain](start)");
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](start)");
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return NULL;
    }
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_load_conf_file](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](start)");
    mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","SystemMgr",MXML_DESCEND);
    if(p_node == NULL){
        return NULL;
    }
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[mxmlFindElement](end)");

    sys_conf_system_manager_domain* ret = (sys_conf_system_manager_domain*)malloc(sizeof(sys_conf_system_manager_domain));
    memset(ret,'\0',sizeof(sys_conf_system_manager_domain));

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_domain](end)");
    ret->domain_list_first = sys_conf_get_system_domain(p_node,xml);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_domain](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_node](end)");
    ret->node_list_first = sys_conf_get_system_node(p_node,xml);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_node](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_process](end)");
    ret->process_list_first = sys_conf_get_system_process(p_node,xml);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_process](end)");

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_process](end)");
    ret->domain_proc_list_first = sys_conf_get_domain_process(p_node,xml);
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_domain_process](end)");

    mxmlDelete(xml);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_get_system_manager_domain](end)");
    return ret;
}

int sys_conf_get_local_domain_id(int *domain_id){
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return -1;
    }
    *domain_id = sys_conf_get_domain_id(xml);
    return *domain_id;
}

sys_domain_group* sys_conf_local_get_domain_group(){
	mxml_node_t* xml = NULL;	
	if(!(xml = sys_conf_load_conf_file())) return NULL;
    return sys_conf_get_domain_group(xml);
}


/*
 * 功能：释放系统配置配置信息
 * 说明：释放系统配置配置信息
 * 参数：系统配置配置信息
 * 返回值：结果码
 */
int sys_conf_free_system_manager_domain(sys_conf_system_manager_domain* data){
    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_system_manager_domain](start)");
    sys_conf_system_domain* temp = NULL;
    temp = data->domain_list_first;
    while(temp){
        if(temp->name){
            free(temp->name);
        }
        if(temp->name_ch){
            free(temp->name_ch);
        }
        sys_conf_system_domain* next = temp->next;
        free(temp);
        temp = next;
    }

    sys_conf_system_node* node = NULL;
    node = data->node_list_first;
    while(node){
        if(node->name) free(node->name);
        if(node->name_ch) free(node->name_ch);
        if(node->nic_1_name) free(node->nic_1_name);
        if(node->nic_1_netmask) free(node->nic_1_netmask);
        if(node->nic_1_address) free(node->nic_1_address);
        if(node->nic_2_name) free(node->nic_2_name);
        if(node->nic_2_netmask) free(node->nic_2_netmask);
        if(node->nic_2_address) free(node->nic_2_address);
        if(node->dev) free(node->dev);
        if(node->os) free(node->os);
        if(node->location) free(node->location);

        sys_conf_system_node* next = node->next;
        free(node);
        node = next;
    }

    sys_conf_system_process* proc = NULL;
    while(proc){
        if(proc->alias) free(proc->alias);
        if(proc->command) free(proc->command);
        if(proc->file_path) free(proc->file_path);
        if(proc->para) free(proc->para);
        if(proc->description) free(proc->description);

        sys_conf_system_process* next = proc->next;
        free(proc);
        proc = next;
    }

    sys_conf_system_domain_process* domain_proc = NULL;
    while(domain_proc){
        if(domain_proc->process_sub){
            free(domain_proc->process_sub);
        }

        sys_conf_system_domain_process* next = domain_proc->next;
        free(domain_proc);
        domain_proc = next;
    }
    free(data);

    cmn_log_log(CMN_LOG_LEVEL_DEBUG,"%s","[sys_conf_free_system_manager_domain](end)");
    cmn_log_uninit();
    return 0;
    //loop_free(temp);
}

char* sys_conf_get_node_name_by_id(int node_id){
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return NULL;
    }

    mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","SystemMgr",MXML_DESCEND);
    if(p_node == NULL){
        return NULL;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,"NODES",NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return NULL;
    }

    char id[8];
    int ret = -1;
    ret = sprintf(id,"%d",node_id);
    if(-1 == ret){
        return NULL;
    }
    mxml_node_t* node = mxmlFindElement(t_node,xml,"NODE","id",id,MXML_DESCEND);
    if(node == NULL){
        return NULL;
    }
    mxml_node_t* name_node = mxmlFindElement(node,xml,"NAME",NULL,NULL,MXML_DESCEND);
    if(name_node == NULL){
        return NULL;
    }
    char* temp = strdup(name_node->child->value.opaque);
    return temp;
}


char* sys_conf_get_domain_name_id_list() {
    char* retval = NULL;
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return NULL;
    }

    mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","SystemMgr",MXML_DESCEND);
    if(p_node == NULL){
        return NULL;
    }

    mxml_node_t* t_domain = mxmlFindElement(p_node,xml,"DOMAINS",NULL,NULL,MXML_DESCEND);
    if(t_domain == NULL){
        return NULL;
    }

    mxml_node_t* odomain = mxmlFindElement(t_domain,xml,"DOMAIN",NULL,NULL,MXML_DESCEND_FIRST);
    if(odomain == NULL){
        return NULL;
    }
    
    //continue looping into all nodes
    const int iallocationsize=1024;
    int ifactor = 1;
    int ipos = 0;
    do{
        char* sname = NULL;
        char* sid = NULL;
        sid = mxmlElementGetAttr(odomain, "id");
        
        mxml_node_t* name_node = mxmlFindElement(odomain,xml,"NAME",NULL,NULL,MXML_DESCEND);
        if(name_node != NULL) {
            sname = name_node->child->value.opaque;
        }
        
        if( (sid != NULL) && (sname != NULL) ){
            int ireqiredsize = strlen(sid) + strlen(sname) +2;
            if(retval == NULL) {
                retval = calloc(iallocationsize, sizeof(char));
            }
            else if( (ireqiredsize +ipos) >= (iallocationsize*ifactor) ){
                ifactor ++;
                retval = realloc(retval, ifactor*iallocationsize);
            }
            
            char* ppos = retval + ipos;
            strcat(ppos, sname);
            strcat(ppos, "^");
            strcat(ppos, sid);
            strcat(ppos, ">");
            ipos += ireqiredsize;
        }
        
        odomain = mxmlFindElement(odomain,xml,"DOMAIN",NULL,NULL,MXML_NO_DESCEND);
    }while(odomain != NULL);
    
    return retval;    
}

int sys_conf_get_node_name(int node_id,char* result){
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return -1;
    }

    mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","SystemMgr",MXML_DESCEND);
    if(p_node == NULL){
        return -1;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,"NODES",NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return -1;
    }

    char id[8];
    int ret = -1;
    ret = sprintf(id,"%d",node_id);
    if(-1 == ret){
        return -1;
    }
    mxml_node_t* node = mxmlFindElement(t_node,xml,"NODE","id",id,MXML_DESCEND);
    if(node == NULL){
        return -1;
    }
    mxml_node_t* name_node = mxmlFindElement(node,xml,"NAME",NULL,NULL,MXML_DESCEND);
    if(name_node == NULL){
        return -1;
    }
    char* temp = strdup(name_node->child->value.opaque);
//    result = strdup(name_node->child->value.opaque);
    result = temp;
    return 0;
}

int sys_conf_get_domain_processes(int domain_id,int result[],int len){
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return -1;
    }

    mxml_node_t* p_node = mxmlFindElement(xml,xml,"MODEL","name","SystemMgr",MXML_DESCEND);
    if(p_node == NULL){
        return -1;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,"DOMAIN_PROCESS",NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return -1;
    }

    char id[8];
    int ret = -1;
    ret = sprintf(id,"%d",domain_id);
    if(-1 == ret){
        return -1;
    }

    mxml_node_t* node = mxmlFindElement(t_node,xml,"DOMAIN","id",id,MXML_DESCEND);
    if(node == NULL){
        return -1;
    }

    mxml_node_t* name_node = mxmlFindElement(node,xml,"PROCESS_SUB",NULL,NULL,MXML_DESCEND);
    if(name_node == NULL){
        return -1;
    }
    const char* value;
    value = name_node->child->value.opaque;
    int strLen = strlen(value);
    int i;
    for(i = 0;i < strLen; ++i){
        if(i >= len) break;
#ifdef SYSDEBUG
        printf("proc_sub - %c\n",value[i]);
#endif
        char strChar[2];
        strChar[0] = value[i];
        strChar[1] = '\0';
        sscanf(strChar,"%d",&result[i]);

//        sscanf(&value[i],"%d",&result[i]);
//        result[i] = value[i];
    }
    return 0;
}

char* sys_conf_get_value_by_key(char* p_element,char* p_attr,char* p_attr_value,char* element){
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return NULL;
    }

    mxml_node_t* p_node = mxmlFindElement(xml,xml,p_element,p_attr,p_attr_value,MXML_DESCEND);
    if(p_node == NULL){
        return NULL;
    }

    mxml_node_t* t_node = mxmlFindElement(p_node,xml,element,NULL,NULL,MXML_DESCEND);
    if(t_node == NULL){
        return NULL;
    }
    char* temp = strdup(t_node->child->value.opaque);
    return temp;
}

char* sys_conf_get_value_by_key_strings(char* elements,char* attributes,char* attr_values){
    if(!elements || !attributes || !attr_values) return NULL;
    mxml_node_t* xml = sys_conf_load_conf_file();
    if(xml == NULL){
        return NULL;
    }

    char *eles[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    char *attrs[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    char *a_values[8] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    char s[2] = ".";

    char elements_buf[256] = {0};
    char attributes_buf[256] = {0};
    char attr_values_buf[256] = {0};
    memcpy(elements_buf,elements,strlen(elements));
    memcpy(attributes_buf,attributes,strlen(attributes));
    memcpy(attr_values_buf,attr_values,strlen(attr_values));

    int a = -1,b = -1,c = -1;
    eles[0] = strtok(elements_buf,s);
    int i = -1;
    for(i = 1;i < 8; ++i){
        eles[i] = strtok(NULL,s);
//        printf("%d %s\n",i,eles[i]);
        if(eles[i] == NULL){
            a = i;
            break;
        }
    }
    attrs[0] = strtok(attributes_buf,s);
    for(i = 1;i < 8; ++i){
        attrs[i] = strtok(NULL,s);
        if(attrs[i] == NULL){
            b = i;
            break;
        }
    }
    a_values[0] = strtok(attr_values_buf,s);
    for(i = 1;i < 8; ++i){
        a_values[i] = strtok(NULL,s);
        if(a_values[i] == NULL){
            c = i;
            break;
        }
    }

#ifdef SYSDEBUG
    printf("elements len - %d\n",a);
    printf("attributes len - %d\n",b);
    printf("attr_values len - %d\n",c);
    for(i = 0;i < 8; ++i){
        printf("%d %s\n",i,eles[i]);
        printf("%d %s\n",i,attrs[i]);
        printf("%d %s\n",i,a_values[i]);
    }
#endif
    if((1 == a - b) && (1 == a - c)){
        mxml_node_t* p_node = xml;
        for(i = 0;i < b; ++i){
            char* element = NULL;char* attribute = NULL;char* attr_value = NULL;
            if(strcmp(eles[i],"NULL")) element = eles[i];
            if(strcmp(attrs[i],"NULL")) attribute = attrs[i];
            if(strcmp(a_values[i],"NULL")) attr_value = a_values[i];
            p_node = mxmlFindElement(p_node,xml,element,attribute,attr_value,MXML_DESCEND);
            if(!p_node) return NULL;
        }
        mxml_node_t* node = NULL;
        node = mxmlFindElement(p_node,xml,eles[a - 1],NULL,NULL,MXML_DESCEND);
        if(node){
            char* temp = strdup(node->child->value.opaque);
            return temp;
        }
    }
    return NULL;
}
