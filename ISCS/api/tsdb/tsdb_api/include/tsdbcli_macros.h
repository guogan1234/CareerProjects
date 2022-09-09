/**
 * @file tsdbcli_macros.h
 * @author
 * @date 
 * @version 
 * @note TSDB客户端接口。本接口文件定义TSDB客户端用到的宏定义。
 *
 **/
#ifndef TSDBCLI_MACROS_H
#define TSDBCLI_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _MSC_VER
#ifdef USE_DLL
#define TSDBCLI_API __declspec(dllexport)
#else
#define TSDBCLI_API __declspec(dllimport)
#endif
#else
#define TSDBCLI_API
#endif

/// 错误状态码定义
enum
{
    TSDBCLI_EC_OK = 0,          ///< 成功
    TSDBCLI_EC_ERR = -1,        ///< 错误
    TSDBCLI_EC_NULLPTR = 1,     ///< 空指针
    TSDBCLI_EC_AUTH = 2,        ///< 认证失败
    TSDBCLI_EC_WR_SLAVE = 3     ///< 写备机失败
};

#ifdef __cplusplus
}
#endif
#endif
