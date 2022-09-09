#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <stdlib.h>

#include "thread_param.h"
#include "msg.h"
#include "connecthelper.h"
#include "pagecontroller.h"
#include "common.h"

#include "cfg_key_variant.h"
#include "c_list.h"

char* endian_big_to_small(char* array,int len){
#if __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
//    printf("SDK:big endian to small endian...\n");

    for(int i = 0;i<len;i++){
        //printf("SDK:show before - %x\n",array[i]);
    }
    char* ret = (char*)malloc(len);//this malloc now is not free

    for(int i = len;i>0;i--){
        ret[len-i] = array[i-1];
    }
    /*
    for(int i = 0;i<len;i++){
        printf("SDK:show after - %x\n",ret[i]);
    }*/
#endif
#if __BYTE_ORDER == __ORDER_BIG_ENDIAN__
    ret = array;
#endif
    return ret;
}

int recv_piece_msg_head(int fd){
    printf("SDK:recv head...\n");
    int ret = 0;

    char start[2] = {0};
    int recv_total = 0;
    while(1){
        ret = recv(fd,start+recv_total,2-recv_total,0);
        recv_total += ret;
        printf("SDK:start 2 bytes,recv %d bytes.\n",ret);
        if(recv_total == 2){
            break;
        }
    }
    if(start[0] == 0x7e && start[1] == 0x7e){
        printf("SDK:Parse - start all catch!\n");
    }

    //timestamp
    char t[8] = {0};
    recv_total = 0;
    while(1){
        ret = recv(fd,t+recv_total,8-recv_total,0);
        recv_total += ret;
        printf("SDK:timestamp 8 bytes,recv %d bytes.\n",ret);
        if(recv_total == 8){
            break;
        }
    }
    long *long_t = endian_big_to_small(t,8);
    printf("SDK:Parse - t is %ld\n",*long_t);
    free(long_t);

    //domain
    char domain[4] = {0};
    recv_total = 0;
    while(1){
        ret = recv(fd,domain+recv_total,4-recv_total,0);
        recv_total += ret;
        printf("SDK:domain 4 bytes,recv %d bytes.\n",ret);
        if(recv_total == 4){
            break;
        }
    }
    char* domain_ch = endian_big_to_small(domain,4);
    int* domain_id = (int*)domain_ch;
    printf("SDK:Parse - domain is %d\n",*domain_id);
    free(domain_ch);

    //operateId
    char operate[4] = {0};
    recv_total = 0;
    while(1){
        ret = recv(fd,operate+recv_total,4-recv_total,0);
        recv_total += ret;
        printf("SDK:operate 4 bytes,recv %d bytes.\n",ret);
        if(recv_total == 4){
            break;
        }
    }
    char* operate_i = endian_big_to_small(operate,4);
    int* operate_id = (int*)operate_i;
    printf("SDK:Parse - operate is %d\n",*operate_id);
    free(operate_i);

    //table_len
    char table_name_len[4] = {0};
    recv_total = 0;
    while(1){
        ret = recv(fd,table_name_len+recv_total,4-recv_total,0);
        recv_total += ret;
        printf("SDK:table_len 4 bytes,recv %d bytes.\n",ret);
        if(recv_total == 4){
            break;
        }
    }
    char* table_len_ch = endian_big_to_small(table_name_len,4);
    int* table_len = (int*)table_len_ch;
    printf("SDK:Parse - table_len is %d\n",*table_len);
    int t_len = *table_len;
    free(table_len_ch);

    //table_name
    if(t_len <= 0){
        //error
        return -1;
    }else{
        char table_name[t_len];
        recv_total = 0;
        while(1){
            ret = recv(fd,table_name+recv_total,t_len-recv_total,0);
            recv_total += ret;
            printf("SDK:table_name %d bytes,recv %d bytes.\n",t_len,ret);
            if(recv_total == t_len){
                break;
            }
        }
        table_name[t_len] = '\0';
        printf("SDK:Parse - table_name is %s\n",table_name);
    }

    //total
    char total[4];
    recv_total = 0;
    while(1){
        ret = recv(fd,total+recv_total,4-recv_total,0);
        recv_total += ret;
        printf("SDK:total 4 bytes,recv %d bytes.\n",ret);
        if(recv_total == 4){
            break;
        }
    }
    char* total_ch = endian_big_to_small(total,4);
    int* total_i = (int*)total_ch;
    printf("SDK:Parse - total is %d\n",*total_i);
    free(total_ch);
    return 0;
}

int recv_piece_msg_body(int fd,void* param){
    T_fParam* tf = (T_fParam*)param;
    void* data = tf->data;
    c_list* list = (c_list*)data;

    printf("SDK:recv body...\n");
    int return_value = -2;
    int ret = 0;
    //exit flag
    char b_exit[1] = {0};
    int recv_total = 0;
    while(1){
        ret = recv(fd,b_exit+recv_total,1-recv_total,0);
        recv_total += ret;
        printf("SDK:b_exit 1 bytes,recv %d bytes.\n",ret);
        if(recv_total == 1){
            break;
        }
    }
    if(b_exit[0] == 0x01){
        printf("SDK:Parse - b_exit is %x(01)\n",b_exit[0]);
        return_value = 1;
//        return return_value;//do not recv any bytes in body
    }else if(b_exit[0] == 0x00){
        printf("SDK:Parse - b_exit is %x(00)\n",b_exit[0]);
    }else {
        printf("SDK:Parse - b_exit is %x,unkown flag.\n",b_exit[0]);
    }

    //next flag
    char b_next[1] = {0};
    recv_total = 0;
    while(1){  
        ret = recv(fd,b_next+recv_total,1-recv_total,0);
        recv_total += ret;
        printf("SDK:b_next 1 bytes,recv %d bytes.\n",ret);
        if(recv_total == 1){
            printf("SDK:Parse - bNext %d byte,break while.\n",ret);
            break;
        }
    }
    if(b_next[0] == 0x00){
        printf("SDK:Parse - bNext is %x(00)\n",b_next[0]);
        return_value = 0;
        if(b_exit[0] == 0x01){
            return_value = 1;
        }
    }else if(b_next[0] == 0x01){
        printf("SDK:Parse - bNext is %x(01)\n",b_next[0]);
    }else{
        printf("SDK:Parse - bNext is %x,unknown flag.\n",b_next[0]);
    }

    //object_count
    char object_count[4] = {0};
    recv_total = 0;
    while(1){//when send is slower than recv,recv byte is less than want
        ret = recv(fd,object_count+recv_total,4-recv_total,0);
        recv_total += ret;
        printf("SDK:object_count 4 bytes,recv %d bytes.\n",ret);
        if(recv_total == 4){
            printf("SDK:Parse - object_count %d byte,break while.\n",ret);
            break;
        }
    }
    char* dest_1 = endian_big_to_small(object_count,4);
    int* object_num = (int*)dest_1;
    printf("SDK:Parse - object_num is %d\n",*object_num);
    int obj_count = *object_num;
    free(dest_1);

    //object_size
    char object_size[4];
    recv_total = 0;
    while(1){
        ret = recv(fd,object_size+recv_total,4-recv_total,0);
        recv_total += ret;
        printf("SDK:object_size 4 bytes,recv %d bytes.\n",ret);
        if(recv_total == 4){
            break;
        }
    }
    char* object_s = endian_big_to_small(object_size,4);
    int* object_size_i = (int*)object_s;
    int obj_size = *object_size_i;
    printf("SDK:Parse - obj_size is %d\n",obj_size);
    free(object_s);

    //objects
    if(obj_size < 0){
        //error
        return_value = -1;
    }
    printf("SDK:Parse - obj_count is %d\n",obj_count);
    for(int i = 0;i<obj_count;i++){
        c_list_clear(list);

        char single_obj[obj_size];
        recv_total = 0;
        while(1){
            ret = recv(fd,single_obj+recv_total,obj_size-recv_total,0);
            recv_total += ret;
            printf("SDK:single_obj %d bytes,recv %d bytes.\n",obj_size,ret);
            if(recv_total == obj_size){
                break;
            }
        }

        int index = 0;
        int bPropertyNext = 1;
	int fieldNum = 0;
	int propertyNextNum = 0;
        while(bPropertyNext == 1){
	    fieldNum++;
            char type[4];
            type[0] = single_obj[index];
            type[1] = single_obj[++index];
            type[2] = single_obj[++index];
            type[3] = single_obj[++index];
            char* type_ch = endian_big_to_small(type,4);
            int* type_ip = (int*)type_ch;
            int type_i = *type_ip;
            printf("SDK:object property's type is %d.\n",type_i);
            free(type_ch);
            switch(type_i){
            case 0://string
            {
                printf("SDK:object property is string.\n");
                char value_len[4];
                value_len[0] = single_obj[++index];
                value_len[1] = single_obj[++index];
                value_len[2] = single_obj[++index];
                value_len[3] = single_obj[++index];
                char* value_len_ch = endian_big_to_small(value_len,4);
                int* value_len_ip = (int*)value_len_ch;
                int value_len_i = *value_len_ip;
                free(value_len_ch);
                printf("SDK:object property string len is %d.\n",value_len_i);

                char value[value_len_i];
                for(int a = 0;a<value_len_i;a++){
                    value[a] = single_obj[++index];
                }
                value[value_len_i] = '\0';
                printf("SDK:object property string is - %s.\n",value);
                cfg_variant* v = cfg_variant_string(value);
//                c_list_push_back(list,v);

                char field_len[4];
                field_len[0] = single_obj[++index];
                field_len[1] = single_obj[++index];
                field_len[2] = single_obj[++index];
                field_len[3] = single_obj[++index];
                char* field_len_ch = endian_big_to_small(field_len,4);
                int* field_len_ip = (int*)field_len_ch;
                int field_len_i = *field_len_ip;
                printf("SDK:object property's field length is %d.\n",field_len_i);
                char field[field_len_i+1];
                for(int a = 0;a<field_len_i;a++){
                    field[a] = single_obj[++index];
                }
                field[field_len_i] = '\0';
                printf("SDK:object property's field name is %s,%p\n",field,field);
                char* m_field = strdup(field);
                key_variant* obj = (key_variant*)malloc(sizeof(key_variant));
                obj->field = m_field;
                obj->value = v;

                c_list_push_back(list,obj);
            }
                break;
            case 1://bool
            {
                char b[1];
                b[0] = single_obj[++index];
		cfg_variant* v = NULL;
		if(b[0] == 0x00){
			int8_t bo = 0;
			v = cfg_variant_create_bool(bo);		
		}else if(b[0] == 0x01){
			int8_t bo = 1;
			v = cfg_variant_create_bool(bo);
		}

		char field_len[4];
                field_len[0] = single_obj[++index];
                field_len[1] = single_obj[++index];
                field_len[2] = single_obj[++index];
                field_len[3] = single_obj[++index];
                char* field_len_ch = endian_big_to_small(field_len,4);
                int* field_len_ip = (int*)field_len_ch;
                int field_len_i = *field_len_ip;
                printf("SDK:object property's field length is %d.\n",field_len_i);
                char field[field_len_i+1];
                for(int a = 0;a<field_len_i;a++){
                    field[a] = single_obj[++index];
                }
                field[field_len_i] = '\0';
                printf("SDK:object property's field name is %s\n",field);
                char* m_field = strdup(field);
                key_variant* obj = (key_variant*)malloc(sizeof(key_variant));
                obj->field = m_field;
                obj->value = v;

                c_list_push_back(list,obj);
            }
                break;
            case 2://short
            {
                printf("SDK:object property is short.\n");
                char s[2];
                s[0] = single_obj[++index];
                s[1] = single_obj[++index];
                char* s_ch = endian_big_to_small(s,2);
                short* s_sp = (short*)s_ch;
                short s_s = *s_sp;
                printf("SDK:object property value is %d.\n",s_s);
                free(s_ch);
                cfg_variant* v = cfg_variant_int16(s_s);
//                c_list_push_back(list,v);

                char field_len[4];
                field_len[0] = single_obj[++index];
                field_len[1] = single_obj[++index];
                field_len[2] = single_obj[++index];
                field_len[3] = single_obj[++index];
                char* field_len_ch = endian_big_to_small(field_len,4);
                int* field_len_ip = (int*)field_len_ch;
                int field_len_i = *field_len_ip;
                printf("SDK:object property's field length is %d.\n",field_len_i);
                char field[field_len_i+1];
                for(int a = 0;a<field_len_i;a++){
                    field[a] = single_obj[++index];
                }
                field[field_len_i] = '\0';
                printf("SDK:object property's field name is %s\n",field);
                char* m_field = strdup(field);
                key_variant* obj = (key_variant*)malloc(sizeof(key_variant));
                obj->field = m_field;
                obj->value = v;

                c_list_push_back(list,obj);
            }
                break;
            case 3://int
            {
                printf("SDK:object property is int.\n");
                char value[4];
                value[0] = single_obj[++index];
                value[1] = single_obj[++index];
                value[2] = single_obj[++index];
                value[3] = single_obj[++index];
                char* value_ch = endian_big_to_small(value,4);
                int* value_ip = (int*)value_ch;
                int value_i = *value_ip;
                printf("SDK:object property value is %d.\n",value_i);
                free(value_ch);
                cfg_variant* v = cfg_variant_int32(value_i);
//                c_list_push_back(list,v);

                char field_len[4];
                field_len[0] = single_obj[++index];
                field_len[1] = single_obj[++index];
                field_len[2] = single_obj[++index];
                field_len[3] = single_obj[++index];
                char* field_len_ch = endian_big_to_small(field_len,4);
                int* field_len_ip = (int*)field_len_ch;
                int field_len_i = *field_len_ip;
                printf("SDK:object property's field length is %d.\n",field_len_i);
                char field[field_len_i+1];
                for(int a = 0;a<field_len_i;a++){
                    field[a] = single_obj[++index];
                }
                field[field_len_i] = '\0';
                printf("SDK:object property's field name is %s\n",field);
                char* m_field = strdup(field);
                key_variant* obj = (key_variant*)malloc(sizeof(key_variant));
                obj->field = m_field;
                obj->value = v;

                c_list_push_back(list,obj);
            }
                break;
            case 4://long
            {
                printf("SDK:object property is long.\n");
                char value[8];
                value[0] = single_obj[++index];
                value[1] = single_obj[++index];
                value[2] = single_obj[++index];
                value[3] = single_obj[++index];
                value[4] = single_obj[++index];
                value[5] = single_obj[++index];
                value[6] = single_obj[++index];
                value[7] = single_obj[++index];
                char* value_ch = endian_big_to_small(value,8);
                long* value_lp = (long*)value_ch;
                long value_i = *value_lp;
                printf("SDK:object property value is %ld.\n",value_i);
                free(value_ch);
                cfg_variant* v = cfg_variant_int64(value_i);
//                c_list_push_back(list,v);

                char field_len[4];
                field_len[0] = single_obj[++index];
                field_len[1] = single_obj[++index];
                field_len[2] = single_obj[++index];
                field_len[3] = single_obj[++index];
                char* field_len_ch = endian_big_to_small(field_len,4);
                int* field_len_ip = (int*)field_len_ch;
                int field_len_i = *field_len_ip;
                printf("SDK:object property's field length is %d.\n",field_len_i);
                char field[field_len_i+1];
                for(int a = 0;a<field_len_i;a++){
                    field[a] = single_obj[++index];
                }
                field[field_len_i] = '\0';
                printf("SDK:object property's field name is %s\n",field);
                char* m_field = strdup(field);
                key_variant* obj = (key_variant*)malloc(sizeof(key_variant));
                obj->field = m_field;
                obj->value = v;

                c_list_push_back(list,obj);
            }
                break;
            case 5://float
            {
                printf("SDK:object property is float.\n");
                char value[4];
                value[0] = single_obj[++index];
                value[1] = single_obj[++index];
                value[2] = single_obj[++index];
                value[3] = single_obj[++index];
                char* value_ch = endian_big_to_small(value,4);
                float* value_lp = (float*)value_ch;
                float value_i = *value_lp;
                printf("SDK:object property value is %f.\n",value_i);
                free(value_ch);
                cfg_variant* v = cfg_variant_float(value_i);
//                c_list_push_back(list,v);

                char field_len[4];
                field_len[0] = single_obj[++index];
                field_len[1] = single_obj[++index];
                field_len[2] = single_obj[++index];
                field_len[3] = single_obj[++index];
                char* field_len_ch = endian_big_to_small(field_len,4);
                int* field_len_ip = (int*)field_len_ch;
                int field_len_i = *field_len_ip;
                printf("SDK:object property's field length is %d.\n",field_len_i);
                char field[field_len_i+1];
                for(int a = 0;a<field_len_i;a++){
                    field[a] = single_obj[++index];
                }
                field[field_len_i] = '\0';
                printf("SDK:object property's field name is %s\n",field);
                char* m_field = strdup(field);
                key_variant* obj = (key_variant*)malloc(sizeof(key_variant));
                obj->field = m_field;
                obj->value = v;

                c_list_push_back(list,obj);
            }
                break;
            case 6://double
            {
                printf("SDK:object property is double.\n");
                char value[8];
                value[0] = single_obj[++index];
                value[1] = single_obj[++index];
                value[2] = single_obj[++index];
                value[3] = single_obj[++index];
                value[4] = single_obj[++index];
                value[5] = single_obj[++index];
                value[6] = single_obj[++index];
                value[7] = single_obj[++index];
                char* value_ch = endian_big_to_small(value,8);
                double* value_lp = (double*)value_ch;
                double value_i = *value_lp;
                printf("SDK:object property value is %lf.\n",value_i);
                free(value_ch);
                cfg_variant* v = cfg_variant_double(value_i);
//                c_list_push_back(list,v);

                char field_len[4];
                field_len[0] = single_obj[++index];
                field_len[1] = single_obj[++index];
                field_len[2] = single_obj[++index];
                field_len[3] = single_obj[++index];
                char* field_len_ch = endian_big_to_small(field_len,4);
                int* field_len_ip = (int*)field_len_ch;
                int field_len_i = *field_len_ip;
                printf("SDK:object property's field length is %d.\n",field_len_i);
                char field[field_len_i+1];
                for(int a = 0;a<field_len_i;a++){
                    field[a] = single_obj[++index];
                }
                field[field_len_i] = '\0';
                printf("SDK:object property's field name is %s\n",field);
                char* m_field = strdup(field);
                key_variant* obj = (key_variant*)malloc(sizeof(key_variant));
                obj->field = m_field;
                obj->value = v;

                c_list_push_back(list,obj);
            }
                break;
            default:
                printf("SDK:object property's type is unknown.\n");
                break;
            }

            char property_next[1];
            property_next[0] = single_obj[++index];
            if(property_next[0] == 0x00){
                printf("SDK:property next is 0x00.\n");
                bPropertyNext = 0;
            }else if (property_next[0] == 0x01) {
                printf("SDK:property next is 0x01.\n");
                bPropertyNext = 1;
		propertyNextNum++;
            }else{
                printf("SDK:property next is unknown.\n");
                bPropertyNext = -1;
            }
            index = index + 1;
        }
        const char* table_name = tf->table_name;
	printf("SDK:fieldNum - %d,propertyNextNum - %d,table_name - %s\n",fieldNum,propertyNextNum,table_name);
        tf->t_fp(table_name,list);
    }

    return return_value;
}

int recv_piece_msg_tail(int fd){
    printf("SDK:recv tail...\n");
    int ret = 0;

    char crc[2];
    int recv_total = 0;
    while(1){
        ret = recv(fd,crc+recv_total,2-recv_total,0);
        recv_total += ret;
        if(recv_total == 2){
            break;
        }
    }
    if(crc[0] == 0x7e && crc[1] == 0x7e){
        printf("SDK:Parse - crc all catched!\n");
    }

    char end[2];
    recv_total = 0;
    while(1){
        ret = recv(fd,end+recv_total,2-recv_total,0);
        recv_total += ret;
        if(recv_total == 2){
            break;
        }
    }
    if(end[0] == 0x0d && end[1] == 0x0a){
        printf("SDK:Parse - end all catched!\n");
    }

    return 0;
}

int recv_piece_msg(int fd,void* param){
    int ret = -2;
    ret = recv_piece_msg_head(fd);
    ret = recv_piece_msg_body(fd,param);
    recv_piece_msg_tail(fd);
    if(ret == -1){//recv failed handle

    }

    return ret;
}

int send_command_head(int fd,void* param){
    T_fParam* tParam = (T_fParam*)param;
    const int domain_id = tParam->domain_id;
    const char* table_name = tParam->table_name;

    int ret = 0;
    char start[2] = {0x7e,0x7e};
    ret = send(fd,start,2,0);//send byte large than kernel buffer byte,send() will block
    printf("SDK:start - send %d bytes,need %d bytes.\n",ret,2);
    if(ret == -1){
        //send again
        return ret;
    }

    //time
    long t = 0;
    t = get_time_t_msec();
    printf("SDK:time - mseconds is %d\n",t);
    ret = send(fd,&t,sizeof(long),0);
    printf("SDK:send %d bytes,need %d bytes.\n",ret,sizeof(long));
    if(ret == -1){
        //send again
        return ret;
    }

    //domain
    ret = send(fd,&domain_id,sizeof(domain_id),0);
    printf("SDK:domain_id - send %d bytes,need %d bytes.\n",ret,sizeof(long));
    if(ret == -1){
        //send again
        return ret;
    }

    //operate
    int operate_id = 1;
    ret = send(fd,&operate_id,sizeof(int),0);
    printf("SDK:operate_id - send %d bytes,need %d bytes.\n",ret,sizeof(int));
    if(ret == -1){
        //send again
        return ret;
    }

    //table name length
    int table_len = 0;
    table_len = strlen(table_name);
    printf("SDK:table_len - %d\n",table_len);
    ret = send(fd,&table_len,sizeof(table_len),0);
    printf("SDK:table_len - send %d bytes,need %d bytes.\n",ret,sizeof(table_len));
    if(ret == -1){
        //send again
        return ret;
    }

    //table name
    printf("SDK:table_name is %s\n",table_name);
    ret = send(fd,table_name,table_len,0);
    printf("SDK:table_name - send %d bytes,need %d bytes.\n",ret,table_len);
    if(ret == -1){
        //send again
        return ret;
    }

    //total
    //total:page + pageSize = 8;
    int total = sizeof(int)*2;
    //total += (crc + end)
    total += 4;
    printf("SDK:total is %d\n",total);//s%,int - segment fault
    ret = send(fd,&total,sizeof(int),0);
    printf("SDK:total - send %d bytes,need %d bytes.\n",ret,sizeof(int));
    if(ret == -1){
        //send again
        return ret;
    }
}

int send_command_body(int fd,int page){
    int ret = 0;

    printf("SDK:page is %d\n",page);
    ret = send(fd,&page,sizeof(page),0);
    printf("SDK:page - send %d bytes,need %d bytes.\n",ret,sizeof(page));
    if(ret == -1){
        //send again
        return ret;
    }

    int page_size = 0;
    page_size = get_page_size();
    printf("SDK:page_size is %d\n",page_size);
    ret = send(fd,&page_size,sizeof(page_size),0);
    printf("SDK:page_size - send %d bytes,need %d bytes.\n",ret,sizeof(page_size));
    if(ret == -1){
        //send again
        return ret;
    }
    return ret;
}

int send_command_tail(int fd){
    int ret = 0;

    char crc[2] = {0x7e,0x7e};
    ret = send(fd,crc,2,0);
    printf("SDK:crc - send %d bytes,need %d bytes.\n",ret,2);
    if(ret == -1){
        //send again
        return ret;
    }

    char end[2] = {0x0d,0x0a};
    ret = send(fd,end,2,0);
    printf("SDK:end - send %d bytes,need %d bytes.\n",ret,2);
    if(ret == -1){
        //send again
        return ret;
    }

    return ret;
}

int send_piece_command(int fd,void* param,int page){
    int ret = 0;
    ret = send_command_head(fd,param);
    if(ret == -1){
        return ret;
    }
    ret = send_command_body(fd,page);
    if(ret == -1){
        return ret;
    }
    ret = send_command_tail(fd);
    if(ret == -1){
        return ret;
    }
}

void* t_run(void *param){
    printf("SDK:thread run...\n");
    T_fParam* tParam = (T_fParam*)param;

    const char* ip = get_conn_ip();
    int port = get_conn_port();

    int fd = -2;//error socket create is -1
    fd = socket(PF_INET,SOCK_STREAM,0);
    if(fd == -1){
        const char *socket_error = "socket init failed!";
        perror(socket_error);
        return NULL;//compile no warning
    }

    struct sockaddr_in socket_addr;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_family = PF_INET;

    inet_pton(PF_INET,ip,&(socket_addr.sin_addr));

    int conn_res = -2;
    conn_res = connect(fd,(struct sockaddr*)&socket_addr,sizeof(socket_addr));
    if(conn_res == -1){
        const char *conn_error = "connect failed!";
        perror(conn_error);
        return NULL;
    }
    int page = 0;
    while (1) {
        printf("--------send--------\n");
        printf("SDK:send command...%d\n",page);
        int ret = 0;
        ret = send_piece_command(fd,param,page);
        if(ret == -1){
            return NULL;
        }
        page++;

        while(1){
            printf("-----------------\n");
            printf("SDK:recv msg...page %d\n",page);
            int b_next = -2;
            b_next = recv_piece_msg(fd,tParam);
            printf("SDK:recv msg page %d,return is %d\n",page,b_next);
            if(b_next == 0){//page end
                break;
            }else if(b_next == 1){//session end
                close(fd);
		free(tParam->data);
		free(param);
		
		printf("SDK:session end!\n");
                return NULL;
            }else if(b_next == -1){//error
                close(fd);
                return NULL;
            }
        }
    }

	free(tParam->data);
	free(param);
    close(fd);
}
