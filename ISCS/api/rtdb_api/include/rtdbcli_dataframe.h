/**
 * @file rtdbcli_dataframe.h
 * @author
 * @date 
 * @version 
 * @note 客户端接口文件。本文件定义客户用到的数据和数据操作接口。
 **/
#ifndef RTDBCLI_DATAFAME_H
#define RTDBCLI_DATAFAME_H

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

typedef struct rtdb_frameheader {
    unsigned char       head_length;            //48
    unsigned char       version_no;             //0x11
    unsigned short      transfer_type;          //1--Request, 2--Response, 3--Immediate Ack
    unsigned short      domain_id;
    unsigned short      info_type;              
    unsigned short      service_type;
    unsigned short      info_flag;              //default 1
    unsigned int        source_tag;             //reserved
    unsigned int        destination_tag;        //reserved
    unsigned int        body_length;            //less than 64*1024 - 48
    unsigned int        sequence_no;
    unsigned int        expired;
    unsigned long long  timestamp;
    unsigned int        body_check;  
    unsigned int        head_check;
} _rtdb_frameheader, *_prtdb_frameheader;

//RTDB数据帧
typedef struct _rtdb_dataframe {
    _rtdb_frameheader       header;
    unsigned int            body_lengh;
    unsigned int            body_datatype;
    unsigned char           framebody[1];
} rtdb_dataframe, *prtdb_dataframe;

#pragma pack(pop)

int encode_dataframe_header(_prtdb_frameheader inframe, unsigned char* outdatabuff, int* outlen);
int decode_dataframe_header(unsigned char* indatabuff, int inlen, _prtdb_frameheader outframe);

#ifdef __cplusplus
}
#endif
#endif
