/*
 @Copyright Reserved by XXXX.
 RTDB client.
 Create by ZhangGuanju, 2018.04.11.
 Histories:

 */
#ifndef RTDB_CLI_CMN_H
#define RTDB_CLI_CMN_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cmn_logger.h>
#ifdef _WIN32
#define strcasecmp     _stricmp
#else
#include <strings.h>
#endif

#define rtdb_cli_cmn_release(pVar) do {\
	if(pVar){ \
		free(pVar); \
		pVar = NULL; \
	}\
} while (0)
    
#define rtdb_cli_cmn_release2(freeFun,pVar) do {\
	if(pVar != NULL){ \
		freeFun(pVar); \
		pVar = NULL; \
	}\
} while (0)

#ifdef __cplusplus
}
#endif
#endif
