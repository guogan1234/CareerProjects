#include "load_conf.h"
#include <stdio.h>
#include <stdlib.h>

#include "sys_conf_global.h"

#define    GLOBAL_CFGFILE    "conf/systemMangerConfig.xml"

/*
 * 功能：加载xml文档，获取xml文件指针
 * 说明：根据xml文件路径，获取xml文件指针
 * 参数：file_path[in] - xml文件路径
 * 返回值：xml文件指针
 */
mxml_node_t* sys_conf_load_conf_file()
{
    char *iscsBase, file[1024];
    FILE* fp;
    mxml_node_t* xml;

    // 从环境变量ISCS_HOME获取ISCS安装目录，不存在使用当前目录作为ISCS安装目录
    if (!(iscsBase = getenv("ISCS_HOME")))
    {
        snprintf(file, sizeof(file), "../%s", GLOBAL_CFGFILE);
    }
    else
    {
        snprintf(file, sizeof(file), "%s/%s", iscsBase, GLOBAL_CFGFILE);
    }

    if (!(fp = fopen(file,"r")))
    {
#ifdef SYSDEBUG
        PRINT("%s:fopen xml config file failed.\n",__FILE__);
#endif
        return NULL;
    }
    //xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
    xml = mxmlLoadFile(NULL,fp,MXML_OPAQUE_CALLBACK);
    fclose(fp);

    return xml;
}
