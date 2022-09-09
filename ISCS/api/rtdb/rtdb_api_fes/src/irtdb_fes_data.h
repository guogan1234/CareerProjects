#ifndef IRTDB_FES_DATA_H
#define IRTDB_FES_DATA_H 
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
	char *path; // DI点的路径
	char *label;  // 值标签
	int32_t value;
	int64_t timestamp;
} irtdb_fep_di;
irtdb_fep_di* irtdb_fes_create_fep_di(const char *path,const char *label,int32_t value,int64_t timestamp);
void irtdb_fes_free_fep_di(irtdb_fep_di *di);

typedef struct
{
	char *path;
	float value;
	int64_t timestamp;
} irtdb_fep_ai;
irtdb_fep_ai* irtdb_fes_create_fep_ai(const char *path,float value,int64_t timestamp);
void irtdb_fes_free_fep_ai(irtdb_fep_ai *ai);

typedef struct
{
	char *path;
	int32_t value;
	int64_t timestamp;
} irtdb_fep_acc;
irtdb_fep_acc* irtdb_fes_create_fep_acc(const char *path,int32_t value,int64_t timestamp);
void irtdb_fes_free_fep_acc(irtdb_fep_acc *acc);

typedef struct
{
	char* path;
	char* value;
	char* timeLabel;
	int64_t timestamp;
}irtdb_fep_soe;
irtdb_fep_soe* irtdb_fes_create_fep_soe(const char *path, const char *value,const char *timeLabel, int64_t timestamp);
void irtdb_fes_free_fep_soe(irtdb_fep_soe *soe);

/* irtdb_fep_cmdresult_notifier:
 * 命令结果通知
 * 参数1 -- 输入，命令结果（JSON格式的字符串，内容由命令发送方与执行方约定）
 * 参数2 -- 输入，用户数据
 */
typedef void(irtdb_fep_cmdresult_notifier)(const char*,const void*);

/* irtdb_fep_cmd_processor:
 * 命令处理
 * 参数1 -- 输入，命令数据（JSON格式的字符串，内容由命令发送方与执行方约定）
 * 参数2 -- 输入，命令结果通知函数
 * 参数3 -- 输入，用户数据
 */
typedef void(irtdb_fep_cmd_processor)(const char*,irtdb_fep_cmdresult_notifier*,const void*);

#ifdef __cplusplus
}
#endif
#endif



