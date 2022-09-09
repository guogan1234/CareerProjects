#include <stdio.h>

#include "sdk.h"
#include "thread_param.h"
#include "c_list.h"
#include "cfg_key_variant.h"

void show_table_data(const char* table_name,void* record){
    printf("Client:show_table_data...%s\n",table_name);
    c_list* p = (c_list*)record;

    c_iterator first, last;
    first = c_list_begin(p);
    last = c_list_end(p);

    c_iterator iter;
//    int i = 0;
    for(iter = first;!ITER_EQUAL(iter, last);ITER_INC(iter))
    {
        key_variant* obj = (key_variant*)(ITER_REF(iter));
//        void* var_v = (void*)ITER_REF(iter);
//        cfg_variant* var = (cfg_variant*)var_v;
        char* field = obj->field;
	//field[len] = '\0';
        //int len = strlen(field);
        
        //printf("Client:list - field len %d\n",field);
	printf("------------------\n");
        printf("Client:list - field %s\n",field);
        cfg_variant* var = obj->value;
        int8_t type = cfg_variant_get_type(var);
        printf("Client:list - type %d\n",type);
        if(type == CFG_VARIANT_INT64){
            int64_t v = cfg_variant_get_int64(var);
            printf("Client:list - long %ld\n",v);
        }else if(type == CFG_VARIANT_INT32){
            int32_t v = cfg_variant_get_int32(var);
            printf("Client:list - int %d\n",v);
        }else if(type == CFG_VARIANT_STRING){
            const char* str = cfg_variant_get_string(var);
            printf("Client:list - string %s\n",str);
        }
    }
}

void load_table(const char* table_name,void *record){
    printf("Client:load_table...\n");
    show_table_data(table_name,record);
}

char* point_array[31];

void init_point_array(){
	const char* c_0 = "conf.unit_tbl";
	const char* c_1 = "conf.domain_tbl";
	const char* c_2 = "conf.station_tbl";
	const char* c_3 = "conf.pro_system_tbl";
	const char* c_4 = "conf.region_tbl";
	const char* c_5 = "conf.ctrl_trans_grp_tbl";
	const char* c_6 = "conf.dev_type_tbl";
	const char* c_7 = "conf.dev_tp_tbl";
	const char* c_8 = "conf.dev_tbl";
	const char* c_9 = "conf.dev_token_info_tbl";
	const char* c_10 = "conf.point_op_info_tbl";
	const char* c_11 = "conf.dev_node_tbl";
	const char* c_12 = "conf.node_conn_tbl";
	const char* c_13 = "conf.di_text_tbl";
	const char* c_14 = "conf.formula_grp_tbl";
	const char* c_15 = "conf.formula_tp_tbl";
	const char* c_16 = "conf.formula_param_tbl";
	const char* c_17 = "conf.do_grp_tbl";
	const char* c_18 = "conf.do_tbl";
	const char* c_19 = "conf.ao_tbl";
	const char* c_20 = "conf.acc_tp_tbl";
	const char* c_21 = "conf.acc_tbl";
	const char* c_22 = "conf.acc_limit_tbl";
	const char* c_23 = "conf.acc_def_tbl";
	const char* c_24 = "conf.dio_tp_tbl";
	const char* c_25 = "conf.di_tbl";
	const char* c_26 = "conf.dio_def_tbl";
	const char* c_27 = "conf.aio_tp_tbl";
	const char* c_28 = "conf.ai_tbl";
	const char* c_29 = "conf.ai_limit_tbl";
	const char* c_30 = "conf.aio_def_tbl";

	printf("char* size - %d\n",sizeof(c_6));

	point_array[0] = c_0;
	point_array[1] = c_1;
	point_array[2] = c_2;
	point_array[3] = c_3;
	point_array[4] = c_4;
	point_array[5] = c_5;
	point_array[6] = c_6;
	point_array[7] = c_7;
	point_array[8] = c_8;
	point_array[9] = c_9;
	point_array[10] = c_10;
	point_array[11] = c_11;
	point_array[12] = c_12;
	point_array[13] = c_13;
	point_array[14] = c_14;
	point_array[15] = c_15;
	point_array[16] = c_16;
	point_array[17] = c_17;
	point_array[18] = c_18;
	point_array[19] = c_19;
	point_array[20] = c_20;
	point_array[21] = c_21;
	point_array[22] = c_22;
	point_array[23] = c_23;
	point_array[24] = c_24;
	point_array[25] = c_25;
	point_array[26] = c_26;
	point_array[27] = c_27;
	point_array[28] = c_28;
	point_array[29] = c_29;
	point_array[30] = c_30;

	printf("----------------\n");
	for(int i = 0;i<31;i++){
		printf("table - %s\n",point_array[i]);
	}
} 

int main()
{
    printf("Hello World!\n");

    printf("simulate a client use sdk...\n");
    T_fPoint fp;
    fp = load_table;

    //const char* table_name = NULL;
    //table_name = "conf.acc_tbl";
    int domain_id = -1;
    //conf_module_load_table(table_name,domain_id,fp);

	init_point_array();
    for(int i = 0;i<31;i++){
		conf_module_load_table(point_array[i],domain_id,fp);
	}	

	//const char* table_name_2 = "conf.domain_tbl";
	//domain_id = -1;
	//conf_module_load_table(table_name,domain_id,fp);

    //
    int i = 0;
    while(1){
        i++;
    }
    return 0;
}
