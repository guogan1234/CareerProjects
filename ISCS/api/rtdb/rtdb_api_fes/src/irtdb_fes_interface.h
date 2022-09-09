#ifndef IRTDB_FES_INTERFACE_H
#define IRTDB_FES_INTERFACE_H 
#ifdef __cplusplus
extern "C" {
#endif

#include "irtdb_fes_data.h"

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_init.
 * 描述：初始化FES接口运行环境。本接口为同步接口，在同一个进程内，即使重复调用本方法，
 *      只有第一次成功调用时真正执行初始化操作。
 *      参数"fesId"是一个由FES本地的RTUID构成的字符串。它们之间用空格分开。例如：
 *      "RTUID1 RTUID2 ... RTUIDn"
 * 参数：
 * 		fesId -- 输入，FES标识。
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_init(const char* fesId);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_uninit.
 * 描述：清理FES接口运行环境。本接口为同步接口，在同一个进程内，即使重复调用本方法，
 *      只有第一次被调用时真正执行清理操作。建议在进程退出时，调用本接口。
 * 参数：
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_uninit();

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_start.
 * 描述：启动FES接口服务。
 *      调用本接口前，首先需要调用接口irtdb_fes_init接口。
 *      再次调用本接口前，首先需要调用一次irtdb_fes_stop接口。
 * 参数：
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_start();

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_stop.
 * 描述：停止FES接口服务。
 * 参数：
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_stop();

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_timed_upload_di.
 * 描述：上报数字量。该接口用于上报那些需要定时上报的数字量。该接口仅负责数据上报，不负责定时。
 * 参数：
 *      path -- 输入，点路径
 *      label -- 输入，值标签
 *      value -- 输入，值
 *      timestamp -- 输入，时间戳（微妙）
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_timed_upload_di(char *path, char *label, int32_t value, int64_t timestamp);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_timed_multiupload_di.
 * 描述：批量上报数字量。该接口用于上报那些需要定时上报的数字量。该接口仅负责数据上报，不负责定时。
 * 参数：
 *      dis -- 输入，数字量数组
 *      disSize -- 输入，数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_timed_multiupload_di(irtdb_fep_di **dis, int disSize); 
 
/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_changed_upload_di.
 * 描述：上报数字量。该接口用于上报发生变化了的数字量。
 * 参数：
 *      path -- 输入，点路径
 *      label -- 输入，值标签
 *      value -- 输入，值。
 *      timestamp -- 输入，时间戳（微妙）
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_changed_upload_di(char *path, char *label, int32_t value, int64_t timestamp);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_changed_multiupload_di.
 * 描述：批量上报数字量。该接口用于上报发生变化了的数字量。
 * 参数：
 *      dis -- 输入，数字量数组
 *      disSize -- 输入，数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_changed_multiupload_di(irtdb_fep_di **dis, int disSize);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_timed_upload_ai.
 * 描述：上报模拟量。该接口用于上报那些需要定时上报的模拟量。该接口仅负责数据上报，不负责定时。
 * 参数：
 *      path -- 输入，点路径
 *      value -- 输入，值。
 *      timestamp -- 输入，时间戳（微妙）
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_timed_upload_ai(char *path, float value, int64_t timestamp);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_timed_multiupload_ai.
 * 描述：批量上报模拟量。该接口用于上报那些需要定时上报的模拟量。该接口仅负责数据上报，不负责定时。
 * 参数：
 *      ais -- 输入，模拟量点数组
 *      aisSize -- 输入，数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_timed_multiupload_ai(irtdb_fep_ai **ais, int aisSize);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_changed_upload_ai.
 * 描述：上报模拟量。该接口用于上报发生变化了的模拟量。
 * 参数：
 *      path -- 输入，点路径
 *      value -- 输入，值。
 *      timestamp -- 输入，时间戳（微妙）
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_changed_upload_ai(char *path, float value, int64_t timestamp);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_changed_multiupload_ai.
 * 描述：批量上报模拟量。该接口用于上报发生变化了的模拟量。
 * 参数：
 *      ais -- 输入，模拟量点数组
 *      aisSize -- 输入，数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_changed_multiupload_ai(irtdb_fep_ai **ais, int aisSize);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_timed_upload_acc.
 * 描述：上报累积量。该接口用于上报那些需要定时上报的累积量。该接口仅负责数据上报，不负责定时。
 * 参数：
 *      path -- 输入，点路径
 *      value -- 输入，值
 *      timestamp -- 输入，时间戳（微妙）
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_timed_upload_acc(char *path, int value, int64_t timestamp);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_timed_multiupload_di.
 * 描述：批量上报累积量。该接口用于上报那些需要定时上报的累积量。该接口仅负责数据上报，不负责定时。
 * 参数：
 *      accs -- 输入，数字量数组
 *      accsSize -- 输入，数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_timed_multiupload_acc(irtdb_fep_acc **accs, int accsSize); 

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_changed_upload_ai.
 * 描述：上报累积量。该接口用于上报发生变化了的累积量。
 * 参数：
 *      path -- 输入，点路径
 *      value -- 输入，值。
 *      timestamp -- 输入，时间戳（微妙）
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_changed_upload_acc(char *path, int value, int64_t timestamp);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_changed_multiupload_acc.
 * 描述：批量上报累积量。该接口用于上报发生变化了的累积量。
 * 参数：
 *      accs -- 输入，数字量数组
 *      accsSize -- 输入，数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_changed_multiupload_acc(irtdb_fep_acc **accs, int accsSize);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_changed_upload_ai.
 * 描述：上报SOE
 * 参数：
 *      path -- 输入，点路径
 *      value -- 输入，SOE值
 *      timeLabel -- 输入，时标
 *      timestamp -- 输入，时间戳（微妙）
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_upload_soe(char *path, char *value, char *timeLabel, int64_t timestamp);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_multiupload_soe.
 * 描述：批量上报SOE
 * 参数：
 *      soes -- 输入，SOE数组
 *      soesSize -- 输入，SOE数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_multiupload_soe(irtdb_fep_soe **soes, int soesSize);

/**
 * 版本：V1.0.1
 * irtdb_fes_upload_message.
 * 描述：上报消息
 * 参数：
 *      domain -- 输入，域ID
 *      destType -- 输入，接收消息的目标系统类型
 *      dest -- 输入，接收消息的目标系统
 *      msg -- 输入，消息
 *      msgLen -- 输入，消息长度
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_upload_message(int domain, int destType, const char *dest, const char *msg, int msgLen);

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_register_commandProcessor.
 * 描述：注册命令处理函数。如果接口没有注册的命令处理函数，接口收到实时数据库发送的命令后，
 *      直接返回错误信息给实时数据库。默认错误信息为{"code":-1,"msg":"No command Processor"}
 * 参数：
 *      soes -- 输入，SOE数组
 *      soesSize -- 输入，SOE数组元素个数
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_register_commandProcessor(irtdb_fep_cmd_processor *cmdProcessor); 

/**
 * 版本：V1.0.0
 * 函数：irtdb_fes_unregister_commandProcessor.
 * 描述: 注销命令处理函数。如果命令处理函数被注销，接口收到实时数据库发送的命令后，
 *      直接返回错误信息给实时数据库。默认错误信息为{"code":-1,"msg":"No command Processor"}
 * 参数：
 *返回值：
 * 		0成功，-1失败。
 */
int irtdb_fes_unregister_commandProcessor(); 

#ifdef __cplusplus
}
#endif
#endif
