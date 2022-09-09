/**
 * @file rtdbcli_utilities.h
 * @author
 * @date 
 * @version 
 * @note 
 * 
 **/
#ifndef RTDBCLI_UTILITIES_H
#define RTDBCLI_UTILITIES_H

#ifndef INTEL_LITTLE_ENDIAN
#define HTONLL(_ll)     (_ll)
#define HTONL(_l)       (_l)
#define HTONS(_s)       (_s)
#define NTOHLL(_ll)     (_ll)
#define NTOHL(_l)       (_l)
#define NTOHS(_s)       (_s)
#else 
//#include <arpa/inet.h>
//#define HTONLL(_ll)     (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
//#define HTONL(_l)       htonl(_l)
//#define HTONS(_s)       htons(_s)
//#define NTOHLL(_ll)     (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))
//#define NTOHL(_l)       ntohl(_l)
//#define NTOHS(_s)       ntohs(_s)
# define CT_HTONS(x)    (((x >> 8) & 0x00FF) | ((x << 8) & 0xFF00))
# define CT_HTONL(x)    (((x >> 24) & 0x000000FF) | ((x >> 8) & 0x0000FF00) | ((x << 8) & 0x00FF0000) | ((x << 24) & 0xFF000000))
#define HTONLL(_ll)     (((uint64_t)CT_HTONL((x) & 0xFFFFFFFFUL)) << 32) | CT_HTONL((uint32_t)((x) >> 32)))
#define HTONL(_l)       CT_HTONL(_l)
#define HTONS(_s)       CT_HTONS(_s)
#define NTOHLL          HTONLL     
#define NTOHL           HTONL
#define NTOHS           HTONS
#endif


/**
 * @brief  dbgHexDump
 * 
 * @detail debug utilities to dump memory in hex format。
 *         
 * @param  desc        输入，description on the data to be dumped
 * @param  addr        输入，start address
 * @param  len         输入，length of data to be dumped
 * 
 * @return void
 */ 
void dbgHexDump(char *desc, void *addr, int len);


#ifdef __cplusplus
extern "C"
{
#endif



#ifdef __cplusplus
}
#endif
#endif