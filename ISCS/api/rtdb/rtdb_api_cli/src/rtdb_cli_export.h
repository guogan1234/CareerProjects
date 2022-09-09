/*
 @Copyright Reserved by XXXX.
 RTDB client.
 Create by WendiKan, 2018.06.19.
 Histories:

 */
#ifndef RTDB_CLI_EXPORT_H
#define RTDB_CLI_EXPORT_H
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER) && defined(RTDB_CLI_USE_DLL)
#ifdef RTDB_CLI_EXPORT
#define RTDB_CLI_API    __declspec(dllexport)
#else
#define RTDB_CLI_API    __declspec(dllimport)
#endif
#else
#define RTDB_CLI_API
#endif
    
#ifdef __cplusplus
}
#endif
#endif
