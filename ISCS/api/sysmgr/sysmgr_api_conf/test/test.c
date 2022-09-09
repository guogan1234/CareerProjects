#include "conf_parser.h"

#include "sys_conf_global.h"

#include <stdio.h>

void domainGoup_test()
{
	int i = 0;
	sys_domain_group *dGroup = NULL;

	dGroup = sys_conf_local_get_domain_group();
	if(!dGroup) return ;
	
	for(i = 0 ; i < dGroup->num ; i++)
	{
		printf("label:%s , id : %d \n", dGroup->lables[i],dGroup->ids[i]);
	}
	sys_conf_free_domain_group(dGroup);
}

void confdb_test() {
    sys_conf_confdb_domain* confdb = sys_conf_get_confdb_domain();
    printf("+++++++++++++show confdb data++++++++++\n");
    printf("[test]domain_id - %d\n",confdb->domain_id);

    sys_conf_confdb_server* temp = confdb->first;
    printf("[test]basePort - %s\n",temp->http_url);
    printf("[test]modePort - %s\n",temp->rtdb_sync_ip);
    printf("[test]username - %d\n",temp->rtdb_sync_port);

    sys_conf_free_confdb_domain(confdb);
}

void rtdb_cli_test(){
    sys_conf_rtdb_domain* data = sys_conf_get_rtdb_domain();

    printf("+++++++++++++show rtdb data++++++++++\n");
    sys_conf_rtdb_server* temp_final = data->servers;
    do{
        printf("[test]---loop---\t\n");
        printf("[test]basePort - %d\n",temp_final->basePort);
        printf("[test]modePort - %d\n",temp_final->modePort);
        printf("[test]username - %s\n",temp_final->username);
        printf("[test]password - %s\n",temp_final->password);
        printf("[test]ip1 - %s\n",temp_final->ip1);
        printf("[test]ip2 - %s\n",temp_final->ip2);

        temp_final = temp_final->next;
    }while(temp_final);

    printf("------log-----\n");
    sys_conf_log* log = data->log;
    printf("[test]file - %s\n",log->file);
    printf("[test]level - %d\n",log->level);
    printf("[test]max_size - %d\n",log->max_size);
    printf("[test]roll_num - %d\n",log->roll_num);

    sys_conf_free_rtdb_domain(data);
}

void rtdb_fes_test(){
    sys_conf_rtdb_fes_domain* data = sys_conf_get_rtdb_fes_domain();

    printf("+++++++++++++show rtdb-fes data++++++++++\n");
    sys_conf_rtdb_fes_server* temp_final = data->first;
    do{
        printf("[test]---loop---\t\n");
        printf("[test]port - %d\n",temp_final->port);
        printf("[test]username - %s\n",temp_final->username);
        printf("[test]password - %s\n",temp_final->password);
        printf("[test]ip1 - %s\n",temp_final->ip1);
        printf("[test]ip2 - %s\n",temp_final->ip2);

        temp_final = temp_final->next;
    }while(temp_final);

    printf("------log-----\n");
    sys_conf_log* log = data->log;
    printf("[test]file - %s\n",log->file);
    printf("[test]level - %d\n",log->level);
    printf("[test]max_size - %d\n",log->max_size);
    printf("[test]roll_num - %d\n",log->roll_num);

    sys_conf_free_rtdb_fes_domain(data);
}

void mq_test(){
    sys_conf_mq_domain* data = sys_conf_get_mq_domain();

    printf("+++++++++++++show mq data++++++++++\n");
    sys_conf_mq_server* temp = data->first;
    do{
        printf("[test]---loop---\t\n");
        printf("[test]schema - %s\n",temp->schema);
        printf("[test]username - %s\n",temp->username);
        printf("[test]password - %s\n",temp->password);
        printf("[test]pool_size - %d\n",temp->pool_size);
        printf("[test]port - %d\n",temp->port);
        printf("[test]timeout - %d\n",temp->timeout);
        printf("[test]beat_internal - %d\n",temp->beat_internal);
        printf("[test]ip_1 - %s\n",temp->ip_1);
        printf("[test]ip_2 - %s\n",temp->ip_2);

        temp = temp->next;
    }while(temp);

    printf("------log-----\n");
    sys_conf_log* log = data->log;
    printf("[test]file - %s\n",log->file);
    printf("[test]level - %d\n",log->level);
    printf("[test]max_size - %d\n",log->max_size);
    printf("[test]roll_num - %d\n",log->roll_num);
    
    printf("------params-----\n");
    mq_param_key_value* key_values = data->params;
    while(key_values){
        printf("[test]---loop---\t\n");
        printf("[test]parameter: key=%s, value=%s\n", key_values->strkey, key_values->strvalue);
        key_values = key_values->next;
    }

    sys_conf_free_mq_domain(data);
}

void tsdb_test(){
    sys_conf_tsdb_domain* data = sys_conf_get_tsdb_domain();

    printf("+++++++++++++show tsdb data++++++++++\n");
    sys_conf_tsdb_server* temp = data->first;
    do{
        printf("[test]---loop---\t\n");
        printf("[test]schema - %s\n",temp->schema);
        printf("[test]username - %s\n",temp->username);
        printf("[test]password - %s\n",temp->password);
        printf("[test]database - %s\n",temp->database);
        printf("[test]port - %d\n",temp->port);
        printf("[test]timeout - %d\n",temp->timeout);
        printf("[test]beat_internal - %d\n",temp->beat_internal);
        printf("[test]ip_1 - %s\n",temp->ip_1);
        printf("[test]ip_2 - %s\n",temp->ip_2);

        temp = temp->next;
    }while(temp);

    printf("------log-----\n");
    sys_conf_log* log = data->log;
    printf("[test]file - %s\n",log->file);
    printf("[test]level - %d\n",log->level);
    printf("[test]max_size - %d\n",log->max_size);
    printf("[test]roll_num - %d\n",log->roll_num);

    sys_conf_free_tsdb_domain(data);
}

void tsdb_test_by_id(int domain_id){
    sys_conf_tsdb_domain* data = sys_conf_get_tsdb_domain_by_id(domain_id);

    printf("+++++++++++++show tsdb data by id++++++++++\n");
    sys_conf_tsdb_server* temp = data->first;
    do{
        printf("[test]---loop---\t\n");
        printf("[test]schema - %s\n",temp->schema);
        printf("[test]username - %s\n",temp->username);
        printf("[test]password - %s\n",temp->password);
        printf("[test]database - %s\n",temp->database);
        printf("[test]port - %d\n",temp->port);
        printf("[test]timeout - %d\n",temp->timeout);
        printf("[test]beat_internal - %d\n",temp->beat_internal);
        printf("[test]ip_1 - %s\n",temp->ip_1);
        printf("[test]ip_2 - %s\n",temp->ip_2);

        temp = temp->next;
    }while(temp);

    printf("------log-----\n");
    sys_conf_log* log = data->log;
    printf("[test]file - %s\n",log->file);
    printf("[test]level - %d\n",log->level);
    printf("[test]max_size - %d\n",log->max_size);
    printf("[test]roll_num - %d\n",log->roll_num);

    sys_conf_free_tsdb_domain(data);
}

void system_test(){
    sys_conf_system_manager_domain* data = sys_conf_get_system_manager_domain();

    printf("+++++++++++++show system data++++++++++\n");
    //domains
    printf("+++++++++++++domains++++++++++\n");
    sys_conf_system_domain* domain = data->domain_list_first;
    while(domain){
        printf("[test]---loop---\t\n");
        printf("[test]id - %d\n",domain->id);
        printf("[test]name - %s\n",domain->name);
        printf("[test]name_ch - %s\n",domain->name_ch);
        printf("[test]name_ch - %d\n",domain->domain_type);

        domain = domain->next;
    }
    //nodes
    printf("+++++++++++++nodes++++++++++\n");
    sys_conf_system_node* node = data->node_list_first;
    while(node){
        printf("[test]---loop---\t\n");
        printf("[test]id - %d\n",node->id);
        printf("[test]domain_id - %d\n",node->domain_id);
        printf("[test]type - %d\n",node->type);
        printf("[test]name - %s\n",node->name);
        printf("[test]name_ch - %s\n",node->name_ch);
        printf("[test]status - %d\n",node->status);
        printf("[test]nic_num - %d\n",node->nic_num);
        printf("[test]nic_1_name - %s\n",node->nic_1_name);
        printf("[test]nic_1_netmask - %s\n",node->nic_1_netmask);
        printf("[test]nic_1_address - %s\n",node->nic_1_address);
        printf("[test]nic_2_name - %s\n",node->nic_2_name);
        printf("[test]nic_2_netmask - %s\n",node->nic_2_netmask);
        printf("[test]nic_2_address - %s\n",node->nic_2_address);
        printf("[test]net_status - %d\n",node->net_status);
        printf("[test]dev - %s\n",node->dev);
        printf("[test]os - %s\n",node->os);
        printf("[test]location - %s\n",node->location);

        node = node->next;
    }
    //processes
    printf("+++++++++++++processes++++++++++\n");
    sys_conf_system_process* proc = data->process_list_first;
    while(proc){
        printf("[test]---loop---\t\n");
        printf("[test]id - %d\n",proc->id);
        printf("[test]alias - %s\n",proc->alias);
        printf("[test]command - %s\n",proc->command);
        printf("[test]instance_num - %d\n",proc->instance_num);
        printf("[test]start_type - %d\n",proc->start_type);
        printf("[test]autorun - %d\n",proc->autorun);
        printf("[test]cycle_start - %d\n",proc->cycle_start);
        printf("[test]cycle_period - %d\n",proc->cycle_period);
        printf("[test]offline_run - %d\n",proc->offline_run);
        printf("[test]duty_run - %d\n",proc->duty_run);
        printf("[test]standby_run - %d\n",proc->standby_run);
        printf("[test]run_order - %d\n",proc->run_order);
        printf("[test]report_type - %d\n",proc->report_type);
        printf("[test]file_path - %s\n",proc->file_path);
        printf("[test]para - %s\n",proc->para);
        printf("[test]description - %s\n",proc->description);
        printf("[test]run_style - %d\n",proc->run_style);
        printf("[test]proc_type - %d\n",proc->proc_type);

        proc = proc->next;
    }
    //domain processes
    printf("+++++++++++++domain processes++++++++++\n");
    sys_conf_system_domain_process* d_proc = data->domain_proc_list_first;
    while(d_proc){
        printf("[test]---loop---\t\n");
        printf("[test]process_sub - %s\n",d_proc->process_sub);
        printf("[test]domain_id - %d\n",d_proc->domain_id);

        d_proc = d_proc->next;
    }
    sys_conf_free_system_manager_domain(data);
}

void mq_test_by_id(int id){
    sys_conf_mq_domain* data = sys_conf_get_mq_domain_by_id(id);

    printf("+++++++++++++show mq data by id++++++++++\n");
    sys_conf_mq_server* temp = data->first;
    do{
        printf("[test]---loop---\t\n");
        printf("[test]schema - %s\n",temp->schema);
        printf("[test]username - %s\n",temp->username);
        printf("[test]password - %s\n",temp->password);
        printf("[test]pool_size - %d\n",temp->pool_size);
        printf("[test]port - %d\n",temp->port);
        printf("[test]timeout - %d\n",temp->timeout);
        printf("[test]beat_internal - %d\n",temp->beat_internal);
        printf("[test]ip_1 - %s\n",temp->ip_1);
        printf("[test]ip_2 - %s\n",temp->ip_2);

        temp = temp->next;
    }while(temp);

    printf("------log-----\n");
    sys_conf_log* log = data->log;
    printf("[test]file - %s\n",log->file);
    printf("[test]level - %d\n",log->level);
    printf("[test]max_size - %d\n",log->max_size);
    printf("[test]roll_num - %d\n",log->roll_num);

    sys_conf_free_mq_domain(data);
}

void system_test_particular(){
    printf("+++++++++++++system_test_particular++++++++++\n");
    int domain_id = 0;
    sys_conf_get_local_domain_id(&domain_id);
    printf("[test]domain_id - %d\n",domain_id);

    char* node_name = NULL;
//    sys_conf_get_node_name(2,node_name);
    node_name = sys_conf_get_node_name_by_id(2);
    printf("[test]node_name - %s\n",node_name);

    int domain_proc[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
    sys_conf_get_domain_processes(2,domain_proc,8);
    printf("[test]domain_proc:\n");
    int i;
    for(i = 0;i < 8; ++i){
        printf("[test]int - %d ,%d\n",domain_proc[i],i);
    }

    printf("[test]sys_conf_get_value_by_key:\n");
    char* value;
    value = sys_conf_get_value_by_key("NODE","id","1","NAME");
    printf("[test]value:%s\n",value);

    printf("[test]sys_conf_get_value_by_key_strings:\n");
    char* elements = "SYSTEMCFG.MODEL_LIST.MODEL.DOMAINS.DOMAIN.NAME";
    char* attributes = "NULL.NULL.name.NULL.id";//若没有属性，必须传入大写NULL，小写null错误
    char* attr_values = "NULL.NULL.SystemMgr.NULL.1";//必须传入大写NULL，小写null错误
    char* key_value;
    key_value = sys_conf_get_value_by_key_strings(elements,attributes,attr_values);
    printf("[test]key_value:%s\n",key_value);
}

int main(){
	domainGoup_test();
    confdb_test();
    rtdb_cli_test();
    rtdb_fes_test();
    mq_test();
    tsdb_test();
    system_test();

    mq_test_by_id(2);
    tsdb_test_by_id(2);
	
    system_test_particular();
	return 0;
}
