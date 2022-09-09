#include "local_domain_info.h"
#include <stdlib.h>
#include "sys_conf_global.h"
#include "conf_parser.h"

/*
 * 功能：获取本域编号信息
 * 说明：根据xml文件，获取本域编号信息
 * 参数：xml[in] - xml文件指针
 * 返回值：本域编号
 */
int sys_conf_get_domain_id(mxml_node_t* xml){
    if(xml == NULL){
        return -1;
    }

	int temp = 0;
    mxml_node_t* node = NULL;

	node = mxmlFindElement(xml,xml,"LOCAL_DOMAIN_ID",NULL,NULL,MXML_DESCEND);
    //char* str_id = node->child->value.text.string;
    char* str_id = node->child->value.opaque;
	sscanf(str_id,"%d",&temp);

#ifdef SYSDEBUG
    PRINT("get_domain_id:id -- %d\n",temp);
#endif
	return temp;
}

sys_domain_group* sys_conf_create_domain_group(int num)
{
	sys_domain_group *domainGroup = NULL;
	
	domainGroup = calloc(1,sizeof(*domainGroup));
	if(!domainGroup) return NULL;
	domainGroup->num = num;
	if(!(domainGroup->lables = calloc(num,sizeof(char*))))
	{
		free(domainGroup);
		return NULL;
	}
	if(!(domainGroup->ids = calloc(num,sizeof(int))))
	{
		free(domainGroup->lables);
		free(domainGroup);
		return NULL;
	}
	return domainGroup;
}

void sys_conf_free_domain_group(sys_domain_group *domainGroup)
{
	int i = 0;
	if(!domainGroup) return ;
	
	for(i = 0 ; i < domainGroup->num ; i++)
	{
		if(domainGroup->lables[i]) free(domainGroup->lables[i]);
	}
	free(domainGroup->lables);
	free(domainGroup->ids);
	free(domainGroup);
}

/*
 * 功能：获取dominGroup信息
 * 说明：根据xml文件，获取dominGroup信息
 * 参数：xml[in] - xml文件指针
 * 返回值：sys_domain_group指针
 */
sys_domain_group* sys_conf_get_domain_group(mxml_node_t* xml){
	sys_domain_group *domainGroup = NULL;
    mxml_node_t *node = NULL,*p = NULL;
	int num = 0,i = 0;
	char *label=NULL,*id = NULL;
	
    if(!xml) return NULL;
	p = node = mxmlFindElement(xml,xml,"DOMAIN_GROUP",NULL,NULL,MXML_DESCEND);
	if(!node) return NULL;

    
	while(p=mxmlFindElement(p,node,"DOMAIN",NULL,NULL,MXML_DESCEND))
		num++;
	//printf("num : %d\n",num);
	domainGroup = sys_conf_create_domain_group(num);
	if(!domainGroup) return NULL;
	
	i = 0;
	p = node;
	while(p = mxmlFindElement(p,node,"DOMAIN",NULL,NULL,MXML_DESCEND))
	{
		label = mxmlElementGetAttr(p,"label");
		id = mxmlElementGetAttr(p,"id");
		if(label && id)
		{
			//printf("type : %d ,label:%s , id : %s \n",p->type,label,id);
			domainGroup->lables[i] = strdup(label);
			domainGroup->ids[i] = atoi(id);			
		}
		i++;
	}
	return domainGroup;
}

/*
 * 功能：获取本域编号字符串
 * 说明：根据xml文件，获取本域编号信息
 * 参数：xml[in] - xml文件指针
 * 返回值：本域编号字符串
 */
char* sys_conf_get_domain_id_char(mxml_node_t* xml){
    if(xml == NULL){
        return NULL;
    }
    mxml_node_t* node = NULL;

	node = mxmlFindElement(xml,xml,"LOCAL_DOMAIN_ID",NULL,NULL,MXML_DESCEND);
    //char* str_id = node->child->value.text.string;
    char* str_id = node->child->value.opaque;

	return str_id;
}

/*
 * 功能：获取域服务器列表IP信息的父节点
 * 说明：根据域编号，获取指定域服务器列表IP信息的父节点
 * 参数：xml[in] - xml文件指针
 *      id[in] - 域编号
 * 返回值：域服务器列表IP信息的父节点
 */
mxml_node_t* sys_conf_get_domain_servers_node_by_id(mxml_node_t* xml, int id){
    char* c_id = NULL;
    c_id = (char*)malloc(10);
    if(NULL == c_id){
        return NULL;
    }
    memset(c_id,'\0',10);

    int ret = sprintf(c_id,"%d",id);
    if(-1 == ret){
        return NULL;
    }

    mxml_node_t* node = NULL;
    node = mxmlFindElement(xml,xml,"DOMAIN_SERVER","domain_id",c_id,MXML_DESCEND);
    free(c_id);

    return node;
}

/*
 * 功能：获取域服务器列表IP信息
 * 说明：获取域服务器列表IP信息
 * 参数：p_node[in] - 域服务器列表IP信息的父节点
 *      xml[in] - xml文件指针
 * 返回值：域服务器列表IP信息
 */
sys_conf_domain_server* sys_conf_get_final_domain_servers(mxml_node_t* p_node,mxml_node_t* xml){
    mxml_node_t* node = NULL;
    node = mxmlFindElement(p_node,xml,"SERVER","server_id",NULL,MXML_DESCEND);//SERVER
    if(NULL == node){
        return NULL;
    }

    int size = 0;
    sys_conf_domain_server* prev = NULL;
    sys_conf_domain_server* first = NULL;
    do{
#ifdef SYSDEBUG
        printf("-----local domain info------\n");
        //printf("i -- %d\n",i);
        printf("node -- %p\n",node);
        printf("node type -- %d\n",node->type);
#endif
        if(node->type == MXML_ELEMENT){
        sys_conf_domain_server* datas = NULL;
        datas = (sys_conf_domain_server*)malloc(sizeof(sys_conf_domain_server));
        memset(datas,'\0',sizeof(sys_conf_domain_server));
        datas->size = 0;
        datas->next = NULL;
        if(prev != NULL){
            prev->next = datas;
        }else{
            first = datas;
        }

        mxml_node_t* net_node_1 = mxmlFindElement(node,xml,"NET1_IP",NULL,NULL,MXML_DESCEND);
        if(net_node_1){
            //datas->net_1_ip = strdup(net_node_1->child->value.text.string);
            datas->net_1_ip = strdup(net_node_1->child->value.opaque);
        }

        mxml_node_t* net_node_2 = mxmlFindElement(node,xml,"NET2_IP",NULL,NULL,MXML_DESCEND);
        //char* net_ip_2 = net_node_2->child->value.text.string;
        //datas->net_2_ip = strdup(net_node_2->child->value.text.string);
        datas->net_2_ip = strdup(net_node_2->child->value.opaque);

        prev = datas;
        size++;
        }

        node = node->next;
    }while(node);
    first->size = size;

    return first;
}

/*
 * 功能：获取域服务器列表IP信息
 * 说明：根据域编号，获取指定域服务器列表IP信息
 * 参数：xml[in] - xml文件指针
 *      id[in] - 域编号
 * 返回值：域服务器列表IP信息
 */
sys_conf_domain_server* sys_conf_get_domain_servers_by_id(mxml_node_t* xml,int domain_id){
    if(NULL == xml){
        return NULL;
    }
    mxml_node_t* p_node = NULL;
    p_node = sys_conf_get_domain_servers_node_by_id(xml,domain_id);
    if(!p_node){
        return NULL;
    }

    sys_conf_domain_server* ret = NULL;
    ret = sys_conf_get_final_domain_servers(p_node,xml);

    return ret;
}

/*
 * 功能：获取本域服务器列表IP信息
 * 说明：获取本域服务器列表IP信息
 * 参数：xml[in] - xml文件指针
 * 返回值：域服务器列表IP信息
 */
sys_conf_domain_server* sys_conf_get_domain_servers(mxml_node_t* xml){
    if(xml == NULL){
        return NULL;
    }
    int domain_id = sys_conf_get_domain_id(xml);
	
	mxml_node_t* p_node = NULL;
    p_node = sys_conf_get_domain_servers_node_by_id(xml,domain_id);
	if(!p_node){
        return NULL;
	}

    sys_conf_domain_server* ret = NULL;
    ret = sys_conf_get_final_domain_servers(p_node,xml);
	
    return ret;
}
