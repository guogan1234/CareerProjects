/**
 * @file sysconf_json_loader.h
 * @author
 * @date 
 * @version 
 * @note 系统全局配置接口。本接口文件定义系统全局配置用到的JSON配置文件加载接口。
 *
 **/
#ifndef SYSCONF_JSON_LOADER_H
#define SYSCONF_JSON_LoADER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  sysconf_load_deployconf_json
 * 
 * @detail 加载deploy配置（JSON格式）。
 * 
 * @param  file  输入，配置文件
 * 
 * @return 返回加载执行结果 @see SYSCONF_EC_XXX
 */
int32_t sysconf_load_deployconf_json(const char *file);

/**
 * @brief  sysconf_load_runconf_json
 * 
 * @detail 加载run配置（JSON格式）。本接口必须在sysconf_load_deployconf_json之后被调用。
 * 
 * @param  file  输入，配置文件
 * 
 * @return 返回加载执行结果 @see SYSCONF_EC_XXX
 */
int32_t sysconf_load_runconf_json(const char *file);

#ifdef __cplusplus
}
#endif
#endif
