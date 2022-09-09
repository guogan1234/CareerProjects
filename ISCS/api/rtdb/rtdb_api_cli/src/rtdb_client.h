/*
 * 实时数据库客户端接口。
 * 作者 wendi kan 2018.04.03
 * 历史履历：
 */
#ifndef RTDB_CLIENT_H
#define RTDB_CLIENT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "rtdb_cli_export.h"
#include "rtdb_cli_data.h"
#include "rtdb_cli_list.h"

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_create.
 * 描述：创建实时数据库客户端。rtdb_cli_free释放返回值。
 * 参数：
 * 		domainId -- 输入，域名ID，预留（默认NULL）。
 *返回值：
 * 		成功返回 rtdb_cli* ，失败返回NULL。
 */
RTDB_CLI_API rtdb_cli* rtdb_cli_create(const int* domainId);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_free。
 * 描述：释放实时数据库客户端。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *返回值：
 */
RTDB_CLI_API void rtdb_cli_free(rtdb_cli *c);

/**
 * 版本：V1.0.0
 * rtdb_cli_get_lasterror.
 * 描述：获取接口最近一次错误。当调用接口的一个方法后，在调用接口另一个方法前，调用该接口
 *       可获取上一个方法执行时的故障状态。
 * 参数：
 * 		c -- 输入，实时数据库客户端。
 *返回值：
 * 		0正常，1网络故障，2服务器错误，3其它。
 */
RTDB_CLI_API int rtdb_cli_get_lasterror(rtdb_cli* c);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_unique_query_device_attribute
 * 描述：查询设备的单个属性。rtdb_cli_variant_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，属性的路径。
 *返回值：
 *      成功，返回属性值；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_variant* rtdb_cli_unique_query_device_attribute(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_unique_query_acc_attribute
 * 描述：查询累积量点的单个属性。rtdb_cli_variant_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，属性的路径。
 *返回值：
 *      成功，返回属性值；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_variant* rtdb_cli_unique_query_acc_attribute(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_unique_query_dig_attribute
 * 描述：查询数字量点的单个属性。rtdb_cli_variant_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，属性的路径。
 *返回值：
 *      成功，返回属性值；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_variant* rtdb_cli_unique_query_dig_attribute(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_unique_query_ana_attribute
 * 描述：查询模拟量点的单个属性。rtdb_cli_variant_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，属性的路径。
 *返回值：
 *      成功，返回属性值；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_variant* rtdb_cli_unique_query_ana_attribute(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_multi_query_device_attributes
 * 描述：查询设备的多个属性。rtdb_cli_record_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，设备路径。
 *      names -- 输入，属性名称列表(rtdb_cli_list<char*>)。
 *返回值：
 *      成功，返回属性记录；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_record* rtdb_cli_multi_query_device_attributes(rtdb_cli *c, const char *path,const rtdb_cli_list *names);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_multi_query_acc_attributes
 * 描述：查询累积量点的多个属性。rtdb_cli_record_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，数字量点路径。
 *      names -- 输入，属性名称列表(rtdb_cli_list<char*>)。
 *返回值：
 *      成功，返回属性记录；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_record* rtdb_cli_multi_query_acc_attributes(rtdb_cli *c, const char *path, const rtdb_cli_list *names);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_multi_query_dig_attributes
 * 描述：查询数字量的多个属性。rtdb_cli_record_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，数字量点路径。
 *      names -- 输入，属性名称列表(rtdb_cli_list<char*>)。
 *返回值：
 *      成功，返回属性记录；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_record* rtdb_cli_multi_query_dig_attributes(rtdb_cli *c, const char *path, const rtdb_cli_list *names);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_multi_query_ana_attributes
 * 描述：查询模拟量点的多个属性。rtdb_cli_record_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，数字量点路径。
 *      names -- 输入，属性名称列表(rtdb_cli_list<char*>)。
 *返回值：
 *      成功，返回属性记录；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_record* rtdb_cli_multi_query_ana_attributes(rtdb_cli *c, const char *path, const rtdb_cli_list *names);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_unique_query_attribute
 * 描述：查询单个属性。rtdb_cli_variant_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，属性的路径。
 *返回值：
 *      成功，返回属性值；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_variant* rtdb_cli_unique_query_attribute(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_multi_query_attributes
 * 描述：查询一个节点的多个属性。rtdb_cli_record_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，点的路径。
 *      names -- 入参， 属性名称列表（rtdb_cli_list<char*>）。
 * 返回值：
 *      成功，返回属性记录；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_record* rtdb_cli_multi_query_attributes(rtdb_cli *c, const char *path, const rtdb_cli_list *names);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_query_node
 * 描述：查询节点，返回节点的属性记录。rtdb_cli_record_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，节点路径。
 * 返回值：
 *      成功，返回属性记录；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_record* rtdb_cli_query_node(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_get_node_type
 * 描述：查询节点，返回节点的类型。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，节点路径。
 * 返回值：
 *      成功，返回点类型(
 *      1  :  TABLE NODE
 *      2  :  RECORD NODE
 *      3  :  COMPLEXRECORD NODE
 *      4  :  DOMAIN NODE
 *      5  :  STATION NODE
 *      6  :  SYSTEM NODE
 *      7  :  DEVICE NODE
 *      8  :  ACC NODE
 *      9  :  DIO NODE
 *      10 :  DI NODE
 *      11 :  DO NODE
 *      12 :  AIO NODE
 *      13 :  AI NODE
 *      14 :  AO NODE
 *  )
 *     失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_get_node_type(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_multi_query_nodes
 * 描述：查询多个点的属性。返回节点属性记录的列表（rtdb_cli_list<rtdb_cli_record*>）。rtdb_cli_list_free释放返回值。
 *      rtdb_cli_record存储点的属性集合。结果列表的元素顺序与参数“paths”的顺序对应。
 *      失败的点对应的返回值为NULL。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      paths -- 输入，点路径列表（rtdb_cli_list<char*>）。
 * 返回值：
 *      成功，返回点属性集合列表；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_list* rtdb_cli_multi_query_nodes(rtdb_cli *c, const rtdb_cli_list *paths);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_mmulti_query_attributes
 * 描述：查询多个点的属性。返回属性值列表（rtdb_cli_list<rtdb_cli_variant*>）。rtdb_cli_list_free释放返回值。
 *      结果列表的元素顺序与参数“paths”的顺序对应。失败的属性对应的返回值为NULL。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      paths -- 输入，属性路径列表（rtdb_cli_list<char*>）。
 *      names -- 输入，属性名称列表（rtdb_cli_list<char*>）。 
 * 返回值：
 *      成功，返回点属性值列表；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_list* rtdb_cli_mmulti_query_attributes(rtdb_cli *c, const rtdb_cli_list *paths, const rtdb_cli_list *names);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_list_children
 * 描述：枚举点的子节点KEY。返回子节点KEY列表（List<char*>）。rtdb_cli_list_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，点路径。
 * 返回值：
 *      成功，返回子节点KEY列表；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_list* rtdb_cli_list_children(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_list_attributes
 * 描述：枚举点的属性KEY。返回属性KEY列表（List<char*>）。rtdb_cli_list_free释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，节点路径。
 * 返回值：
 *      成功，返回属性KEY列表；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_list* rtdb_cli_list_attributes(rtdb_cli *c, const char *path);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_set_attribute
 * 描述：修改属性值。仅仅修改指定属性的值，无属性值的业务处理。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      path -- 输入，属性的路径。
 *      value -- 输入，属性值（字符串形式值，例如整数值123， value = "123"）。
 * 返回值：
 *      成功，返回0，失败返回-1。
 */
RTDB_CLI_API int rtdb_cli_set_attribute(rtdb_cli *c, const char *path, const char *value);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_send_command
 * 描述：发送命令。发送命令给目标系统。
 * 备注：备注内容为开发提供指导用。对客户端使用者无效。
 *      接口在发命令发给RTDB服务的时候，需要生成本命令的唯一标识（例如使用UUID），作为命令的一个参数发送给RTDB服务。
 *      如果命令是FES命令，则RTDB服务把从客户端接收到的参数，全部发送给FES，由返回把部分参数与命令结果，作为命令结果返回。
 *      如果是PLUGIN命令，则RTDB服务把从客户端接收到的所有参数作为命令参数发送给，把命令标识作为插件命令的用户参数使用。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      type -- 输入，命令类型。
 *      destination -- 输入，命令目标系统（例如Plugin、FES等）。
 *      cmdJson -- 输入，JSON格式的命令（内容包括命令类型、目标应用、目标点、值等）。
 *      resultNotifier -- 输入，执行结果通知。
 *      userData -- 输入，用户数据。
 * 返回值：
 */
RTDB_CLI_API void rtdb_cli_send_command
(
	rtdb_cli *c, 
	int domainId, 
	int type, 
	const char* destination, 
	const char *cmdJson, 
	rtdb_cli_cmdResult_notifier *resultNotifier,
	const void *userData
);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_send_message
 * 描述：发送消息。发送消息给目标系统。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      type -- 输入，消息类型。
 *      destination -- 输入，目标系统（例如PluginID、FES等）。
 *      msg -- 输入，消息内容。
 *      msgSize -- 输入，消息长度。
 * 返回值：
 *      成功，返回0；失败，返回-1；
 */
RTDB_CLI_API int rtdb_cli_send_message(rtdb_cli *c, int domainId, int type, const char *destination, const char *msg, int msgSize);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_query_table
 * 描述：查询表的全部记录。返回记录列表（rtdb_cli_list<rtdb_cli_record*>）。rtdb_cli_list_free释放返回值。
 *       
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      tblName -- 输入， 表名称。
 *      filters -- 输入，表字段名称列表(rtdb_cli_list<char*>)。用于指定结果集要包含的字段。
 *          表的字段名称不能使用如下关键字（不能单独使用，可包含。比如不能使用and，但可以使用field_and）：
 *          and break do else elseif end false for function if 
 *          in local nil not or repeat return then true until while
 *          table goto
 * 返回值：
 *      成功，返回表记录列表；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_list* rtdb_cli_query_table(rtdb_cli *c, int domainId, const char *tblName, const rtdb_cli_list *filters);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_unique_query_record
 * 描述：查询表的单条记录。rtdb_cli_record_free释放返回值。
 *      查询条件，支持的条件运算符： "=", "!=", ">", ">=", "<", "<=", "&", "|"。
 *      查询条件类型支持：单字段，多字段与，多字段或。
 *      查询条件不要包含空格。
 *      例如：
 *      "string='Joe'",
 *      "string!='Joe'"
 *      "num=1"
 *      "num!=1"
 *      "num>1"
 *      "num>=1"
 *      "num<1"
 *      "num<=1"
 *      "num=1|name='Joe'"
 *      "num=1&name='Joe'"
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      tblName -- 输入， 表名称。
 *      cond -- 输入，查询条件。参照rtdb_cli_query_table说明。
 *      filters -- 输入，表字段名称列表(rtdb_cli_list<char*>)。用于指定结果集要包含的字段。
 * 返回值：
 *       成功， 返回记录；失败，返回NULL。
 * 
 */
RTDB_CLI_API rtdb_cli_record* rtdb_cli_unique_query_record
(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_list *filters
);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_multi_query_records
 * 描述：查询多条记录。返回记录列表（rtdb_cli_list<rtdb_cli_record*>）。rtdb_client_free_list释放返回值。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      tblName -- 输入， 表名称。
 *      cond -- 输入，查询条件。参照rtdb_cli_unique_query_record说明。
 *      filters -- 输入，参照rtdb_cli_unique_query_record说明。
 * 返回值：
 *      成功，返回记录列表；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_list* rtdb_cli_multi_query_records
(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_list *filters
);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_delete_records
 * 描述：删除表记录。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      tblName -- 输入，表名称。
 *      cond -- 输入，删除条件。参照rtdb_cli_unique_query_record说明。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_delete_records(rtdb_cli *c, int domainId, const char *tblName, const char *cond);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_update_records
 * 描述：更新表记录。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      tblName -- 输入，表名称。
 *      cond -- 输入，更新条件。参照rtdb_cli_unique_query_record说明。
 *      record -- 输入，字段的目标值记录。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_update_records
(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_record *record
);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_insert_record
 * 描述：插入表记录。要插入的记录必须包含表的全部字段，空的字段需要给出默认值，数字类型默认值为0，字符串类型默认值""。
 * 参数：
 *      c -- 输入，实时数据库客户端。
 *      domainId -- 输入，域ID。
 *      tblName -- 输入，表名称。
 *      cond -- 输入，条件为真时，禁止插入。参照rtdb_cli_unique_query_record说明。
 *      record -- 输入，要插入的记录。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_insert_record
(
	rtdb_cli *c,
	int domainId,
	const char *tblName,
	const char *cond,
	const rtdb_cli_record *record
);

/**
 * 版本：V1.0.0
 * 函数：rtdb_cli_create_subscriber
 * 描述：创建订阅。rtdb_cli_free_subscriber释放返回值。
 *      订阅后，出现断网等网络异常后，需要释放订阅对象，重新创建订阅对象，重新订阅。
 * 参数：
 *      subscriberName -- 输入，订阅名称。
 *      rtdb_cli_node_notifier -- 输入，点数据通知函数。
 *      rtdb_cli_table_notifier -- 输入，表数据通知函数。
 *      rtdb_cli_exception_notifier -- 输入，异常通知函数。
 * 返回值：
 *      成功，返回订阅对象；失败，返回NULL。
 */
RTDB_CLI_API rtdb_cli_subscriber* rtdb_cli_create_subscriber
(
	const char *subscriberName,
	rtdb_cli_node_notifier *nodeNotifier,
	rtdb_cli_table_notifier *tableNotifier,
	rtdb_cli_exception_notifier *expectionNotifier
);

/**
 * 版本：V1.0.0
 * rtdb_cli_free_subscriber
 * 描述：销毁订阅对象。如果订阅对象被销毁之前没有取消订阅，RTDB服务会在特定时机下，取消该订阅对象的所有订阅。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 * 返回值：
 */
RTDB_CLI_API void rtdb_cli_free_subscriber(rtdb_cli_subscriber *subscriber);

/**
 * 版本：V1.0.0
 * rtdb_cli_unique_subscribe_table
 * 描述：单表订阅。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 *      domainId -- 输入，域ID。
 *      tableName -- 输入，表名。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_unique_subscribe_table(rtdb_cli_subscriber *subscriber, int domainId, const char *tableName);

/**
 * 版本：V1.0.0
 * rtdb_cli_multi_subscribe_tables
 * 描述：多表订阅。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 *      domainId -- 输入，域ID。
 *      tableNames -- 输入，表名列表（rtdb_cli_list<char*>）。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_multi_subscribe_tables(rtdb_cli_subscriber *subscriber, int domainId, const rtdb_cli_list *tableNames);

/**
 * 版本：V1.0.0
 * rtdb_cli_unique_subscribe_node
 * 描述：单点订阅。订阅一个点的多个属性。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 *      nodePath -- 输入，点路径。
 *      names -- 输入，属性名称列表（rtdb_cli_list<char*>）。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_unique_subscribe_node(rtdb_cli_subscriber *subscriber, const char *nodePath, const rtdb_cli_list *names);

/**
 * 版本：V1.0.0
 * rtdb_cli_multi_subscribe_nodes
 * 描述：多点订阅。订阅多个点的多个属性。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 *      nodePaths -- 输入，点路径列表（rtdb_cli_list<char*>）。
 *      names -- 输入，属性名称列表（rtdb_cli_list<char*>）。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_multi_subscribe_nodes
(
	rtdb_cli_subscriber *subscriber,
	const rtdb_cli_list *nodePaths,
	const rtdb_cli_list *names
);

/**
 * 版本：V1.0.0
 * rtdb_cli_multi_unsubscribe_tables
 * 描述：多表订阅取消。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 *      domainId -- 输入，域ID。
 *      tableNames -- 输入，表名称列表（rtdb_cli_list<char*>）。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_multi_unsubscribe_tables(rtdb_cli_subscriber *subscriber, int domainId, const rtdb_cli_list *tableNames);

/**
 * 版本：V1.0.0
 * rtdb_cli_multi_unsubscribe_nodes
 * 描述：多点订阅取消。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 *      nodePaths -- 输入，点路径列表（rtdb_cli_list<char*>）。
 * 返回值：
 *      成功，返回0；失败，返回-1。
 */
RTDB_CLI_API int rtdb_cli_multi_unsubscribe_nodes(rtdb_cli_subscriber *subscriber, const rtdb_cli_list *nodePaths);

/**
 * 版本：V1.0.0
 * rtdb_cli_publish
 * 描述：数据发布。启动接RTDB服务发布的订阅数据。
 * 参数：
 *      subscriber -- 输入，订阅对象。
 * 返回值：
 */
RTDB_CLI_API void rtdb_cli_publish(rtdb_cli_subscriber *subscriber);

#ifdef __cplusplus
}
#endif
#endif
