/**
 * @file rtdbcli_dataframe.c
 * @author
 * @date 
 * @version 
 * @note RTDB客户端接口文件。本文件定义客户的FES接口实现。
 *          
 **/

#include "stddef.h"
#include "rtdbcli_dataframe.h"
#include "rtdbcli_macros.h"
#include "rtdbcli_utilities.h"

int encode_dataframe_header(_prtdb_frameheader inframe, unsigned char* outdatabuff, int* inoutlen) 
{
    if( (outdatabuff == NULL) || ( (*inoutlen) < sizeof(_rtdb_frameheader)) ) {
        return RTDBCLI_EC_INVALID_PARAMETER;
    }

    _prtdb_frameheader outframe = (_prtdb_frameheader)outdatabuff;
    outframe->head_length = inframe->head_length;
    outframe->version_no = inframe->version_no;
    outframe->transfer_type = HTONS(inframe->transfer_type);
    outframe->domain_id = HTONS(inframe->domain_id);
    outframe->info_type = HTONS(inframe->info_type);
    outframe->service_type = HTONS(inframe->service_type);
    outframe->info_flag = HTONS(inframe->info_flag);

    outframe->source_tag = HTONL(inframe->source_tag);
    outframe->destination_tag = HTONL(inframe->destination_tag);
    outframe->body_length = HTONL(inframe->body_length);
    outframe->sequence_no = HTONL(inframe->sequence_no);
    outframe->expired = HTONL(inframe->expired);
    outframe->timestamp = HTONLL(inframe->timestamp);
    outframe->body_check = HTONL(inframe->body_check);
    outframe->head_check = HTONL(inframe->head_check);

    *inoutlen = sizeof(_rtdb_frameheader);

    return RTDBCLI_EC_OK;
}

int decode_dataframe_header(unsigned char* indatabuff, int inlen, _prtdb_frameheader outframe)
{
    if( ((indatabuff == NULL)) || (outframe == NULL) || ( inlen < sizeof(_rtdb_frameheader)) ) {
        return RTDBCLI_EC_INVALID_PARAMETER;
    }

    _prtdb_frameheader inframe = (_prtdb_frameheader)indatabuff;
    outframe->head_length = inframe->head_length;
    outframe->version_no = inframe->version_no;
    outframe->transfer_type = NTOHS(inframe->transfer_type);
    outframe->domain_id = NTOHS(inframe->domain_id);
    outframe->info_type = NTOHS(inframe->info_type);
    outframe->service_type = HTONS(inframe->service_type);
    outframe->info_flag = HTONS(inframe->info_flag);

    outframe->source_tag = NTOHL(inframe->source_tag);
    outframe->destination_tag = NTOHL(inframe->destination_tag);
    outframe->body_length = NTOHL(inframe->body_length);
    outframe->sequence_no = NTOHL(inframe->sequence_no);
    outframe->expired = NTOHL(inframe->expired);
    outframe->timestamp = NTOHLL(inframe->timestamp);
    outframe->body_check = NTOHL(inframe->body_check);
    outframe->head_check = NTOHL(inframe->head_check);
    
    return RTDBCLI_EC_OK;
}
