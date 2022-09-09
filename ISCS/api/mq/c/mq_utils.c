#include <string.h>

#include "hiredis.h"
#include "c_vector.h"

#include "mq_constant.h"
#include "mq_priv.h"
#include "cmn_time.h"
#include "miniz.c"
#include "base64.h"
#include "mq_cmn_def.h"

bool mq_is_topic(const char* dest) {
    return (strstr(dest, MQ_TOPIC) == dest ? MQ_TRUE : MQ_FALSE);
}

bool mq_is_queue(const char* dest) {
    return (strstr(dest, MQ_QUEUE) == dest ? MQ_TRUE : MQ_FALSE);
}

//depends on uuid-dev
long mq_msg_id_generate() {
    static volatile long id = 1;
    return id++;
//    uuid_t uuid = {0};
//    uuid_generate(uuid);

//    //uuid to string
//    char buffer[sizeof(uuid)*2] = {0};
//    for(int i = 0; i < sizeof(uuid); i++){
//        sprintf(buffer+i*2, "%02X", uuid[i]);
//    }

//    // string to hash code
//    long hash = 0L;
//    for(int j = 0; j < sizeof(buffer); j++){
//        hash = hash * 31 + buffer[j];
//    }

//    return hash;
}

const char * mq_extract_queue_name(const char* fullQN)
{
    char* pChar = strrchr(fullQN, ':');
    if(pChar == NULL)
    {
        pChar = (char*)fullQN;
    }
    else
    {
        pChar ++;
    }
    
    return pChar;
        
}

const char * mq_get_local_domain()
{
    return "localhost"; //read it from configuration
}

int mq_adjust_priority_with_timestamp(const char* queuename, int ipriority){
    int iret = ipriority > 9 ? 9 : ipriority;
    
    iret = 9 - iret;  //reverted the priority, so that in line with time priority, larger value has less priority

    if(mq_is_queue(queuename)){
        int64_t curtime = cmn_time_get_utc_seconds();
        //iret = (1 + iret) * 100000000 - curtime / 60;
		iret = iret * 100000000 + curtime / 60;
    }
    
    return iret;
}

int mq_compress_buffer(const unsigned char* rawData, const int inrawlen, bool based64Enc, unsigned char** outDataBuff, int* outlen)
{
	int iret = MQ_OK;
	int istatus;
	uLong src_len = (uLong)inrawlen;
	uLong compressedLen = compressBound(src_len);
	unsigned char *pCompressedData;

    // Allocate buffers to hold compressed and uncompressed data.
    pCompressedData = (unsigned char *)malloc((size_t)compressedLen);
    if (pCompressedData)
    {
		istatus = compress(pCompressedData, &compressedLen, (const unsigned char *)rawData, src_len);
		 if (istatus == Z_OK){
			 if(based64Enc == true){
				 unsigned char * base64data;
				 size_t base64len = 0;
				 base64data = base64_encode(pCompressedData, compressedLen, &base64len);
				*outDataBuff = base64data;
				*outlen = base64len;
				free(pCompressedData);
				
				 if(base64data == NULL){
					 iret = MQ_SYSTEM_ERROR_COMPRESS;
				 }
			 }
			 else{
				 *outDataBuff = pCompressedData;  //pCompressedData will be released by caller of mq_compress_buffer 
				 *outlen = compressedLen;
			 }
		 }
		 else{
			//free the allocated memory for output
	 		 free(pCompressedData);
			 iret = MQ_SYSTEM_ERROR_COMPRESS;
		 }
	}
	else{
		iret = MQ_SYSTEM_ERROR_MEMORY;
	}
	
	return iret;
}

//uncompressedlen should be passed into the method, and outuncompressedDataBuffer is allocated by the caller
int mq_uncompress_buffer(const unsigned char* incompressedData, int incompressedlen, bool based64Enc, unsigned char* outuncompressedDataBuffer, int* uncompressedlen)
{
	int iret = MQ_OK;
	
	unsigned char* compressedNotBase64 = (unsigned char*)incompressedData;
	unsigned int ilendata = incompressedlen;
	//decode from base64 if required
	if(based64Enc == true){
		compressedNotBase64 = base64_decode(incompressedData, incompressedlen, &ilendata);
	}
	
	int istatus;
	uLong ullen=*uncompressedlen;
	istatus = uncompress(outuncompressedDataBuffer, &ullen, compressedNotBase64, ilendata);
	 if (istatus == Z_OK){
		*uncompressedlen = (int)ullen;
	 }
	 else{
		 iret = MQ_SYSTEM_ERROR_COMPRESS;
	 }

	if(compressedNotBase64 != incompressedData){
		free(compressedNotBase64);
	}

	return iret;
}

char* format_queue_topic_name(const char* domain, const char* tqname, char* databuffer, int lenbuffer){
    char* retVal = tqname;
    
    if( (mq_config_domain_name2id_enabled() > 0) && (domain != NULL) && (domain[0] != 0) ){
        const char* domainid = get_domain_id_from_name(domain);
        int newlen = strlen(domainid) + strlen(tqname) + 1; //example "queue://domainid:test_queue_send_receive_mass"
        if( (databuffer != NULL) && (newlen < (lenbuffer - 1) ) ){
            if(mq_is_topic(tqname) == MQ_TRUE){
                memset(databuffer, 0, lenbuffer);
                strcat(databuffer, MQ_TOPIC);
                strcat(databuffer, domainid);
                strcat(databuffer, ":");
                strcat(databuffer, tqname + LEN_MQ_TQ_HEAD);
                retVal = databuffer;
            }
            else if(mq_is_queue(tqname) == MQ_TRUE){
                memset(databuffer, 0, lenbuffer);
                strcat(databuffer, MQ_QUEUE);
                strcat(databuffer, domainid);
                strcat(databuffer, ":");
                strcat(databuffer, tqname + LEN_MQ_TQ_HEAD);
                retVal = databuffer;
            }
        }
    }
    
    return retVal;
}

int string_comparer(void * x, void * y)
{
    int iret = strcmp((char*)x, (char*)y);
    return iret;
}