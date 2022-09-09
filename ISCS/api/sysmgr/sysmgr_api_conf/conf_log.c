#include "conf_log.h"

#include "sys_conf_global.h"

/*
 * 功能：获取日志配置信息
 * 说明：根据日志节点的父节点，获取日志配置信息
 * 参数：parent[in] - 日志节点的父节点
 *      xml[in] - xml文件指针
 * 返回值：日志配置信息
 */
sys_conf_log* sys_conf_get_log(mxml_node_t* parent,mxml_node_t* xml){
    if(!parent || !xml) return NULL;

    mxml_node_t* node = NULL;
    sys_conf_log* log = (sys_conf_log*)malloc(sizeof(sys_conf_log));
    memset(log, 0x00, sizeof(sys_conf_log));

    node = mxmlFindElement(parent, xml, "LOG_FILE",NULL,NULL,MXML_DESCEND);//LOG_FILE
    if(node) {
        //log->file = strdup(node->child->value.text.string);
        log->file = strdup(node->child->value.opaque);
    }

    node = mxmlFindElement(parent, xml, "LOG_LEVEL", NULL, NULL, MXML_DESCEND);//LOG_LEVEL
    if(node) {
        //sscanf(node->child->value.text.string, "%d", &(log->level));
        sscanf(node->child->value.opaque, "%d", &(log->level));
    }

	node = mxmlFindElement(parent,xml,"FILE_MAX_SIZE_MB",NULL,NULL,MXML_DESCEND);//FILE_MAX_SIZE_MB
    if(node) {
        //sscanf(node->child->value.text.string,"%d",&(log->max_size));
        sscanf(node->child->value.opaque,"%d",&(log->max_size));
    }

	node = mxmlFindElement(parent,xml,"ROLLING_FILE_NUM",NULL,NULL,MXML_DESCEND);//ROLLING_FILE_NUM
    if(node) {
        //sscanf(node->child->value.text.string,"%d",&(log->roll_num));
        sscanf(node->child->value.opaque,"%d",&(log->roll_num));
    }

    return log;
}
