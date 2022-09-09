#include <string.h>
#include <cmn_logger.h>
#include "irtdb_fes_common.h"
#include "irtdb_fes_privatecommon.h"
#include "irtdb_fes_privatedata.h"
#include "irtdb_fes_interface.h"
#include "conf_parser.h"

cmn_logger* logger = NULL;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
char *g_fesId = NULL;
irtdb_fes_dbnode *g_dbNode;
int g_quit = 0;

irtdb_fep_cmd_processor *g_cmd_processor = NULL;
cmn_list *gCmdResultList = NULL;

static int irtdb_fes_load_globalconf();
static int irtdb_fes_add_server(
    const char *ip1, const char *ip2,
    int port,const char *username,const char *password 
);
static int irtdb_fes_send_fesid(irtdb_fes_peer *pPeer);
static int irtdb_fes_load_globalconf();
static int irtdb_fes_add_to_send(const irtdb_fes_message_bin *pMsgBin);    
static int irtdb_fes_direct_send(const irtdb_fes_message_bin *pMsgBin);    
static int irtdb_fes_upload_di_base(
    int8_t dataType,char *label,char *valueLabel,
    int32_t value,int64_t timestamp   
);
static int irtdb_fes_upload_ai_base(
    int8_t dataType,char *label,float value,int64_t timestamp
);
static int irtdb_fes_upload_acc_base(
    int8_t dataType,char *label,int value,int64_t timestamp
);
static void ifes_commandResult_process(const char *cmdResult, const void *userData);
static int irtdb_fes_connect_rtdb(irtdb_fes_dbnode *pNode);
static void irtdb_fes_shutdown_properly(irtdb_fes_peer *peer);
static int irtdb_fes_handle_received(irtdb_fes_message_bin *pMsgBin);
static int irtdb_fes_build_fdsets(irtdb_fes_peer *peer,fd_set *rfds,fd_set *wfds,fd_set *efds);
static void* irtdb_fes_rtdb_clientthread(void* arg);
static void ifes_commandResult_process(const char *cmdResult, const void *userData);

static int irtdb_fes_send_msgbin(const irtdb_fes_message_bin *pMsgBin);
static irtdb_fes_message_bin* irtdb_fes_dump_di_base(
    int8_t dataType,char *label,char *valueLabel,int32_t value, int64_t timestamp
);
static int irtdb_fes_multiupload_di_base(int8_t dataType,irtdb_fep_di **dis, int disSize);
static irtdb_fes_message_bin* irtdb_fes_dump_ai_base(
	int8_t dataType,char *label,float value,int64_t timestamp
);
static int irtdb_fes_multiupload_ai_base(int8_t dataType,irtdb_fep_ai **ais, int aisSize);
static irtdb_fes_message_bin* irtdb_fes_dump_acc_base(
	int8_t dataType,char *label,int value,int64_t timestamp
);
static int irtdb_fes_multiupload_acc_base(int8_t dataType,irtdb_fep_acc **accs, int accSize);


int irtdb_fes_init(const char* fesId)
{
	int len = 0;

	if (!fesId) return RET_ERROR;
	len = strlen(fesId);

    pthread_mutex_lock(&g_mutex);
	g_fesId = calloc(len + 1, 1);
	memcpy(g_fesId, fesId, len);
    g_dbNode = NULL;
    pthread_mutex_unlock(&g_mutex);

    irtdb_fes_load_globalconf();
	gCmdResultList = cmn_list_create(NULL,irtdb_fes_free_cmd,NULL);
	if (!gCmdResultList) return RET_ERROR;
	return RET_SUCCESS;
}

int irtdb_fes_uninit()
{   
    irtdb_fes_dbnode *pCur = g_dbNode;

	if (!pCur) return RET_ERROR;
	pthread_mutex_lock(&g_mutex);
    do 
    {
        if(irtdb_fes_send_to_peer(pCur->peer) != 0)
            CMN_LOGGER_ERR(logger,"irtdb_fes_uninit : send fail!");
        irtdb_fes_free_dbnode(pCur);
		pCur = pCur->next;
    } while(pCur != NULL);
    pthread_mutex_unlock(&g_mutex);

    if(logger)
    {
        cmn_logger_destory(logger);
        logger = NULL;
    }
	if (gCmdResultList)
	{
		cmn_list_release(gCmdResultList);
		gCmdResultList = NULL;
	}	
	return RET_SUCCESS;
}

int irtdb_fes_start()
{
    int iRet = RET_SUCCESS;
    int isAllFail = 1;
    irtdb_fes_dbnode *pCur = NULL;
	irtdb_fes_dbserver *pRtdb = NULL;
        
	if (!g_dbNode) 
	{
		CMN_LOGGER_DEBUG(logger, "g_dbNode is NULL!\n");
		iRet = RET_ERROR;
	}
    pCur = g_dbNode;
	do 
	{   
		pthread_attr_t attr;
		pthread_t thread;

		pRtdb = pCur->pRtdbInfo;
		if(!pRtdb || !pRtdb->ip1 || !pRtdb->ip2)
			continue;
		pRtdb->activeIp = NULL;
		if(0 == irtdb_fes_check_tcp_port(pRtdb->ip1,pRtdb->port,1))
			pRtdb->activeIp = pRtdb->ip1;
		else
		{
			// check ip2
			if(0 == irtdb_fes_check_tcp_port(pRtdb->ip2,pRtdb->port,1))
				pRtdb->activeIp = pRtdb->ip2;
		}
		if(!pRtdb->activeIp)
		{
			CMN_LOGGER_ERR(logger,"connect(%s,%s):%d fail",pRtdb->ip1,pRtdb->ip2,pRtdb->port);
			continue;
		}
            
		pRtdb->isAvailable = 1;
            
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		pthread_create(&thread,&attr,&irtdb_fes_rtdb_clientthread,pCur);
		pthread_attr_destroy(&attr);
		usleep(10000);
		if(pCur->peer->socket < 0)
		{
			iRet = RET_ERROR;
			break;
		}
		isAllFail = 0; // not all fail , at least one start
	} while(pCur = pCur->next);
   
    if(isAllFail) iRet = RET_ERROR;
    if(RET_SUCCESS == iRet)
        CMN_LOGGER_DEBUG(logger,"success\n");
    else
        CMN_LOGGER_DEBUG(logger,"fail\n");
    return iRet;
}

int irtdb_fes_stop()
{
    pthread_mutex_lock(&g_mutex);
    g_quit = 1;
    pthread_mutex_unlock(&g_mutex);
    usleep(10000);
    irtdb_fes_uninit();
    CMN_LOGGER_DEBUG(logger,"ok!\n");
	return RET_SUCCESS;
}

int irtdb_fes_timed_upload_di(
    char *label, 
    char *valueLabel, 
    int32_t value, 
    int64_t timestamp
)
{
    int iRet = RET_SUCCESS;
    iRet = irtdb_fes_upload_di_base(
        FESMSG_TIMED_UPLOAD,
        label,valueLabel,value,timestamp
    );
    if(RET_SUCCESS == iRet)
        CMN_LOGGER_DEBUG(logger,"success\n");
    else
        CMN_LOGGER_DEBUG(logger,"fail\n");
    return iRet;
}



int irtdb_fes_timed_multiupload_di(irtdb_fep_di **dis, int disSize)
{   
    if(!dis) return -1;    
    return irtdb_fes_multiupload_di_base(FESMSG_TIMED_UPLOAD,dis,disSize);
}

int irtdb_fes_changed_upload_di(
    char *label, 
    char *valueLabel, 
    int32_t value,
    int64_t timestamp  
)
{
    int iRet = RET_SUCCESS;
    iRet = irtdb_fes_upload_di_base(
        FESMSG_CHANGED_UPLOAD,
        label,valueLabel,value,timestamp
    ); 
    if(RET_SUCCESS == iRet)
        CMN_LOGGER_DEBUG(logger,"success\n");
    else
        CMN_LOGGER_DEBUG(logger,"fail\n");
    return iRet;
}

int irtdb_fes_changed_multiupload_di(irtdb_fep_di **dis, int disSize)
{
    if(!dis) return -1;  
    return irtdb_fes_multiupload_di_base(FESMSG_CHANGED_UPLOAD,dis,disSize);
}

int irtdb_fes_timed_upload_ai(
    char *aiLabel, 
    float value, 
    int64_t timestamp   
)
{
    int iRet = RET_SUCCESS;
    iRet = irtdb_fes_upload_ai_base(
        FESMSG_TIMED_UPLOAD,
        aiLabel,value,timestamp
    ); 
    if(RET_SUCCESS == iRet)
        CMN_LOGGER_DEBUG(logger,"success\n");
    else
        CMN_LOGGER_DEBUG(logger,"fail\n");
    return iRet;
}

int irtdb_fes_timed_multiupload_ai(irtdb_fep_ai **ais, int aisSize)
{
    if(!ais) return -1;    
    return irtdb_fes_multiupload_ai_base(FESMSG_TIMED_UPLOAD,ais,aisSize);
}

int irtdb_fes_changed_upload_ai(
    char *aiLabel, 
    float value, 
    int64_t timestamp     
)
{
    int iRet = RET_SUCCESS;
    iRet = irtdb_fes_upload_ai_base(
        FESMSG_CHANGED_UPLOAD,
        aiLabel,value,timestamp
    ); 
    if(RET_SUCCESS == iRet)
        CMN_LOGGER_DEBUG(logger,"success\n");
    else
        CMN_LOGGER_DEBUG(logger,"fail\n");
    return iRet;
}

int irtdb_fes_changed_multiupload_ai(irtdb_fep_ai **ais, int aisSize)
{
    if(!ais) return -1;    
    return irtdb_fes_multiupload_ai_base(FESMSG_CHANGED_UPLOAD,ais,aisSize);
}

int irtdb_fes_timed_upload_acc(
    char *accLabel,
    int value, 
    int64_t timestamp  
)
{
    int iRet = RET_SUCCESS;
    iRet = irtdb_fes_upload_acc_base(
        FESMSG_TIMED_UPLOAD,
        accLabel,value,timestamp
    ); 
    if(RET_SUCCESS == iRet)
        CMN_LOGGER_DEBUG(logger,"irtdb_fes_timed_upload_acc success\n");
    else
        CMN_LOGGER_DEBUG(logger,"irtdb_fes_timed_upload_acc fail\n");
    return iRet;
}

int irtdb_fes_timed_multiupload_acc(irtdb_fep_acc **accs, int accsSize)
{
    if(!accs) return -1;    
    return irtdb_fes_multiupload_acc_base(FESMSG_TIMED_UPLOAD,accs,accsSize);
}

int irtdb_fes_changed_upload_acc(char *accLabel, int value, int64_t timestamp)
{
    int iRet = RET_SUCCESS;
    iRet = irtdb_fes_upload_acc_base(
        FESMSG_CHANGED_UPLOAD,
        accLabel,value,timestamp
    ); 
    if(RET_SUCCESS == iRet)
        CMN_LOGGER_DEBUG(logger,"irtdb_fes_changed_upload_acc success\n");
    else
        CMN_LOGGER_DEBUG(logger,"irtdb_fes_changed_upload_acc fail\n");
    return iRet;
}

int irtdb_fes_changed_multiupload_acc(irtdb_fep_acc **accs, int accsSize)
{
    if(!accs) return -1;    
    return irtdb_fes_multiupload_acc_base(FESMSG_CHANGED_UPLOAD,accs,accsSize);
}

int irtdb_fes_upload_soe(char *path, char *value, char *timeLabel, int64_t timestamp)
{
	return 0;
}

int irtdb_fes_multiupload_soe(irtdb_fep_soe **soes, int soesSize)
{
	return 0;
}

int irtdb_fes_register_commandProcessor(irtdb_fep_cmd_processor *cmdProcessor)
{
	if (!cmdProcessor) return -1;
	g_cmd_processor = cmdProcessor;
    CMN_LOGGER_INFO(logger,"irtdb_fes_register_commandProcessor ok");
	return 0;
}

int irtdb_fes_unregister_commandProcessor()
{
	g_cmd_processor = NULL;
    CMN_LOGGER_INFO(logger,"irtdb_fes_unregister_commandProcessor ok");
	return 0;
}


//////////////////////////////////////////////////////
static int irtdb_fes_add_server(
    const char *ip1, const char *ip2,
    int port,const char *username,const char *password 
)
{
    int iRet = RET_SUCCESS;
    irtdb_fes_dbserver *pServer = NULL;
    irtdb_fes_dbnode *pNode = NULL,*pCur = NULL;;
    
    pthread_mutex_lock(&g_mutex);
    
    pServer = irtdb_fes_create_dbserver(ip1,ip2,port,username,password);
    if(pServer != NULL)
    {
        pNode = irtdb_fes_create_dbnode(pServer);
        if(pNode)
        {
            if(NULL == g_dbNode)
            {
                g_dbNode = pNode;
                g_dbNode->next = NULL;
            }
            pCur = g_dbNode;
            while(pCur->next != NULL) 
            {
                pCur = pCur->next;
            }
            pCur->next = pNode;
            pCur = pCur->next;
            pCur->next = NULL;
        }
        else
        {
            iRet = RET_ERROR;
        }
    }
    else
    {
        iRet = RET_ERROR;
    }
    pthread_mutex_unlock(&g_mutex);
    CMN_LOGGER_DEBUG(logger,"irtdb_fes_add_server ok");
    return iRet;
}

static int irtdb_fes_send_fesid(irtdb_fes_peer *pPeer)
{
    int iRet = RET_SUCCESS;
    irtdb_fes_fesid* pFesId = NULL;
    unsigned char *bytes = NULL;
    int dumpLen = 0,tmpRet = 0;
    
    if(!pPeer) return RET_ERROR;
    pFesId = irtdb_fes_create_fesid(g_fesId);
    if(!pFesId) return RET_ERROR;
    
    bytes = irtdb_fes_dump_fesid(pFesId,&dumpLen);
    if(!bytes)
    {
        irtdb_fes_free_fesid(pFesId);
        return RET_ERROR;
    }    
    tmpRet = send(pPeer->socket,bytes,dumpLen,0);
    if(tmpRet < 0)
    {
        CMN_LOGGER_DEBUG(logger,"rtdb_fes_connect_rtdb send fesId error!");
        iRet = RET_ERROR;
    }
    else
    {
        CMN_LOGGER_DEBUG(logger,"rtdb_fes_connect_rtdb send fesId ok!");
    }
    irtdb_fes_free_pointer(bytes);
    irtdb_fes_free_fesid(pFesId);
    return iRet;
}

static int irtdb_fes_load_globalconf()
{
    sys_conf_rtdb_fes_domain* confdb = NULL;
    sys_conf_rtdb_fes_server* confNode = NULL;
    sys_conf_log* log = NULL;
    char *ip1,*ip2,*user,*password;
    int port;
    
    confdb = sys_conf_get_rtdb_fes_domain();
    if(!confdb)
    {
        CMN_LOGGER_DEBUG(logger,"sys_conf_get_confdb_domain fail!");
        return -1;
    }    
    confNode = confdb->first;
    while(confNode)
    {
        ip1 = confNode->ip1;
        ip2 = confNode->ip2;
        port = confNode->port;
        user = confNode->username;
        password = confNode->password;

        irtdb_fes_add_server(ip1,ip2,port,user,password);
        confNode = confNode->next;
    }
    if(!g_dbNode)
    {
        CMN_LOGGER_DEBUG(logger,"load global conf fail!");
    }    
    log = confdb->log;
    if(!logger && log && log->file)
    {
        logger = cmn_logger_init(log->file,log->level,log->max_size,log->roll_num);
        CMN_LOGGER_DEBUG(logger,"logger init ok");
    }    
    sys_conf_free_rtdb_fes_domain(confdb);
}

static int irtdb_fes_add_to_send(const irtdb_fes_message_bin *pMsgBin)
{
    int iRet = RET_SUCCESS;    
    irtdb_fes_dbnode *pCur = NULL;
    
    CMN_LOGGER_DEBUG(logger,"irtdb_fes_add_to_send begin!");
    if(!pMsgBin) return RET_ERROR;
    
    pCur = g_dbNode;
    if(pCur)
    {
        do 
        {   
            irtdb_fes_dbserver *pRtdb = pCur->pRtdbInfo;
            if(!pRtdb->isAvailable) continue;
            if(pCur->peer != NULL)
            {
                //pthread_mutex_lock(&g_mutex);
                if(irtdb_fes_peer_add_to_send(pCur->peer,pMsgBin) != 0)
                {
                    iRet = RET_ERROR;
                }
                //pthread_mutex_unlock(&g_mutex);
            }
            else
            {
                CMN_LOGGER_DEBUG(logger,"pCur->peer is NULL !");
                iRet = RET_ERROR;
            }
        } while(pCur = pCur->next);
    }
    else
    {
        CMN_LOGGER_DEBUG(logger,"pCur is NULL");
        iRet = RET_ERROR;
    }
    CMN_LOGGER_DEBUG(logger,"irtdb_fes_add_to_send end!");
    return iRet;
}

static int irtdb_fes_direct_send(const irtdb_fes_message_bin *pMsgBin)
{
	int iRet = RET_SUCCESS;
	irtdb_fes_dbnode *pCur = NULL;
	int send_count = 0;

	CMN_LOGGER_DEBUG(logger, "irtdb_fes_direct_send begin!");
	if (!pMsgBin || !pMsgBin->data) return RET_ERROR;

	pCur = g_dbNode;
	if (pCur)
	{
		do
		{
			irtdb_fes_dbserver *pRtdb = pCur->pRtdbInfo;
			if (!pRtdb->isAvailable) continue;
			if ((pCur->peer != NULL) && (pCur->peer->socket > 0))
			{
				send_count = send(pCur->peer->socket,pMsgBin->data, pMsgBin->length, 0);
				if (send_count <= 0)
				{
					iRet = RET_ERROR;
				}
				
			}
			else
			{
				CMN_LOGGER_DEBUG(logger, "pCur->peer is NULL !");
				iRet = RET_ERROR;
			}
		} while (pCur = pCur->next);
	}
	else
	{
		CMN_LOGGER_DEBUG(logger, "pCur is NULL");
		iRet = RET_ERROR;
	}
	CMN_LOGGER_DEBUG(logger, "irtdb_fes_direct_send end!");
	return iRet;
}
    
static int irtdb_fes_upload_di_base(
    int8_t dataType,
    char *label,char *valueLabel, 
    int32_t value,int64_t timestamp   
)
{
    int iRet = RET_ERROR;
	irtdb_fes_message_bin *msgbin = NULL;

	if(!label || !valueLabel) return RET_ERROR;
	msgbin = irtdb_fes_dump_di_base(dataType,label,valueLabel,value,timestamp);
	if(!msgbin)  return RET_ERROR;
	iRet = irtdb_fes_send_msgbin(msgbin);
	irtdb_fes_delete_message_bin(msgbin);
	return iRet;	
}

static int irtdb_fes_upload_ai_base(
    int8_t dataType,
    char *label, 
    float value, 
    int64_t timestamp
)
{
    int iRet = RET_ERROR;
	irtdb_fes_message_bin *msgbin = NULL;

	if(!label) return RET_ERROR;
	msgbin = irtdb_fes_dump_ai_base(dataType,label,value,timestamp);
	if(!msgbin)  return RET_ERROR;
	iRet = irtdb_fes_send_msgbin(msgbin);
	irtdb_fes_delete_message_bin(msgbin);
	return iRet;
}

static int irtdb_fes_upload_acc_base(
    int8_t dataType,
    char *label, 
    int value, 
    int64_t timestamp
)
{
    int iRet = RET_ERROR;
	irtdb_fes_message_bin *msgbin = NULL;

	if(!label) return RET_ERROR;
	msgbin = irtdb_fes_dump_acc_base(dataType,label,value,timestamp);
	if(!msgbin)  return RET_ERROR;
	iRet = irtdb_fes_send_msgbin(msgbin);
	irtdb_fes_delete_message_bin(msgbin);
	return iRet;
}

static int irtdb_fes_connect_rtdb(irtdb_fes_dbnode *pNode)
{
    int iRet = RET_SUCCESS;

    if((pNode != NULL) && (pNode->peer != NULL))
    {
        irtdb_fes_peer *pPeer=pNode->peer;
        
        #ifdef WIN32
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != NO_ERROR) {
          CMN_LOGGER_ERR(logger,"WSAStartup failed: %d", iResult);
          iRet = RET_ERROR;
        }
        #endif
        
        pPeer->socket = socket(AF_INET, SOCK_STREAM, 0);
        if (pPeer->socket < 0)
        {
            CMN_LOGGER_ERR(logger,"irtdb_fes_connect_rtdb : socket error!");
            iRet = RET_ERROR;
        }
        else
        {
            int tmpRet = 0;
            irtdb_fes_dbserver *pRtdb = pNode->pRtdbInfo;
            struct sockaddr_in server_addr;
            memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = inet_addr(pRtdb->activeIp);
            server_addr.sin_port = htons(pRtdb->port);
                        
            pPeer->addres = server_addr;
            tmpRet = connect(pPeer->socket,(struct sockaddr *)&pPeer->addres,sizeof(struct sockaddr));
            if(tmpRet != 0)
            {
                CMN_LOGGER_ERR(logger,"connect(%s:%d) error!",pRtdb->activeIp,pRtdb->port);
                pPeer->socket = -1;
                iRet = RET_ERROR;
            }
            else
            {
                CMN_LOGGER_DEBUG(logger,"connect to (%s:%d) ok",pRtdb->activeIp,pRtdb->port);
                // send fesId
                iRet = irtdb_fes_send_fesid(pPeer);
            }            
        }
    }
    else
    {
        CMN_LOGGER_ERR(logger,"irtdb_fes_connect_rtdb : connect error!");
        iRet = RET_ERROR;
    }
    if(RET_ERROR == iRet)
    {
        #ifdef WIN32
        WSACleanup();
        #endif
    }
    return iRet;
}

static void irtdb_fes_shutdown_properly(irtdb_fes_peer *peer)
{
    irtdb_fes_delete_peer(peer);
    CMN_LOGGER_DEBUG(logger,"shutdown client properly");
}

static int irtdb_fes_handle_received(irtdb_fes_message_bin *pMsgBin)
{
	irtdb_fes_header* pMsgHeader = NULL;
	irtdb_fes_cmd *fescmd = NULL;

    CMN_LOGGER_DEBUG(logger,"received message from server ");
    if(!pMsgBin)
    {
        CMN_LOGGER_ERR(logger,"irtdb_fes_handle_received : pMsgBin is NULL");
        return -1;
    }
    CMN_LOGGER_DEBUG(logger,"len : %d ",pMsgBin->length);
	
	pMsgHeader = irtdb_fes_parse_header(pMsgBin->data, IRTDB_FES_MSGHEADERLEN);
	if (!pMsgHeader || (pMsgHeader->msgLen <= 0)) -1;

	switch (pMsgHeader->msgType)
	{
	case IRTDB_FES_CMD:
		fescmd = irtdb_fes_parse_cmd(pMsgBin->data, pMsgBin->length);
		if (fescmd)
		{
            CMN_LOGGER_DEBUG(logger,"fescmd->vlen : %d",fescmd->vlen);
            CMN_LOGGER_DEBUG(logger,"fescmd->value : %s",fescmd->value);
            CMN_LOGGER_DEBUG(logger,"fescmd->id : %s",fescmd->id);
			if (g_cmd_processor) 
				g_cmd_processor(fescmd->value, ifes_commandResult_process, fescmd->id);
			irtdb_fes_free_cmd(fescmd);
		}
		break;
	default:
		break;
	}

    irtdb_fes_delete_message_bin(pMsgBin);
    return 0;
}

static int irtdb_fes_build_fdsets(irtdb_fes_peer *peer,fd_set *rfds,fd_set *wfds,fd_set *efds)
{
    if(!peer || !peer->socket || !rfds || !wfds ) return -1;
    FD_ZERO(rfds);
    FD_SET(peer->socket, rfds);

    FD_ZERO(wfds);
    if (peer->send_buffer && cmn_list_get_size(peer->send_buffer) > 0)
        FD_SET(peer->socket, wfds);

    FD_ZERO(efds);
    FD_SET(peer->socket, efds);

    return 0;
}

static void* irtdb_fes_rtdb_clientthread(void* arg)
{
    int iRet = RET_SUCCESS;
    irtdb_fes_dbnode *pNode = (irtdb_fes_dbnode*)arg;
    irtdb_fes_peer *peer = pNode->peer;
    fd_set read_fds,write_fds,except_fds;
    int activity = 0,maxfd = 0,status = 0;
    struct timeval tv;    
    
    pthread_mutex_lock(&g_mutex);
    iRet = irtdb_fes_connect_rtdb(pNode);
    pthread_mutex_unlock(&g_mutex);
    
	if (RET_ERROR == iRet)
	{
		CMN_LOGGER_DEBUG(logger, "error occur\n");
		return ((void*)0);
	}

    activity = maxfd = 0;
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    while(1) 
    {
        if(!peer) 
        {
            CMN_LOGGER_ERR(logger,"irtdb_fes_rtdb_clientthread quit,peer is NULL!");
            break;
        }
        maxfd = peer->socket;
        if(maxfd <= 0 )
        {
            CMN_LOGGER_ERR(logger,"irtdb_fes_rtdb_clientthread quit,maxfd = %d!\n",maxfd);
            break;
        }
        irtdb_fes_build_fdsets(peer,&read_fds,&write_fds,&except_fds);
        activity = select(maxfd + 1,&read_fds,&write_fds,NULL,&tv);
        if(g_quit) 
        {
            CMN_LOGGER_INFO(logger,"irtdb_fes_rtdb_clientthread quit!\n");
            break;
        }
        if (activity)
        {
            if (FD_ISSET(peer->socket, &read_fds)) 
            {
                    pthread_mutex_lock(&g_mutex);
                    status = irtdb_fes_recv_from_peer(peer, &irtdb_fes_handle_received);
                    if(status != 0)
                    {
                        irtdb_fes_shutdown_properly(peer);
                        peer = NULL;
                        CMN_LOGGER_ERR(logger,"irtdb_fes_rtdb_clientthread : recv fail!\n");
                    }
                    pthread_mutex_unlock(&g_mutex);
            }
            else if (FD_ISSET(peer->socket, &write_fds)) 
            {
                pthread_mutex_lock(&g_mutex);
                status = irtdb_fes_send_to_peer(peer) ;
                pthread_mutex_unlock(&g_mutex);
                if(status != 0)
                {
                    irtdb_fes_shutdown_properly(peer);
                    peer = NULL;
                    CMN_LOGGER_ERR(logger,"irtdb_fes_rtdb_clientthread : send fail!\n");
                }
                    
            }
        }
        else
        {
            //CMN_LOGGER_DEBUG(logger,"do sleep!\n");
            usleep(10);
        }
    }
    return ((void*)0);
}

static void ifes_commandResult_process(const char *cmdResult, const void *userData)
{
	irtdb_fes_cmd* fescmd = NULL;
	const char *value = (const char*)cmdResult;
    const char *id = (const char*)userData;
	int vlen = 0;
    int iRet = RET_SUCCESS;
    unsigned char *pDataOut = NULL;
    int dataOutLen = 0;
    irtdb_fes_message_bin *pMsgBin = NULL;

	if (!cmdResult || !userData) return;
	vlen = strlen(value);
    
    CMN_LOGGER_DEBUG(logger, "ifes_commandResult_process , cmdResult : %s\n",value);
    CMN_LOGGER_DEBUG(logger, "ifes_commandResult_process , cmdid : %s\n",id);

	fescmd = irtdb_fes_create_cmd();
	if (!fescmd)
	{
		CMN_LOGGER_ERR(logger, "irtdb_fes_create_cmd fail , fescmd is NULL");
		return;
	}
	if (-1 == irtdb_fes_init_cmd(fescmd, id, vlen, value))
	{
		CMN_LOGGER_ERR(logger, "irtdb_fes_init_cmd fail ");
		irtdb_fes_free_cmd(fescmd);
		return;
	}    
    pDataOut = irtdb_fes_dump_cmd(fescmd,&dataOutLen);
    irtdb_fes_free_cmd(fescmd);
    if(!pDataOut) 
    {
        CMN_LOGGER_ERR(logger, "irtdb_fes_init_cmd fail,pDataOut is NULL ");
        return;
    }
    pMsgBin = irtdb_fes_create_message_bin(pDataOut,dataOutLen);
    irtdb_fes_free_pointer(pDataOut);
    if(!pMsgBin) 
    {
        CMN_LOGGER_ERR(logger, "irtdb_fes_init_cmd fail,pMsgBin is NULL ");
        return;
    }
    iRet = irtdb_fes_add_to_send(pMsgBin);
    irtdb_fes_delete_message_bin(pMsgBin);
    if(RET_ERROR == iRet)
    {
        CMN_LOGGER_ERR(logger, "irtdb_fes_add_to_send fail ");
        return ;
    }
    CMN_LOGGER_DEBUG(logger, "irtdb_fes_add_to_send success ");
}

static int irtdb_fes_send_msgbin(
    const irtdb_fes_message_bin *pMsgBin
)
{
    int iRet = RET_SUCCESS;
   
    if(!pMsgBin)
        return RET_ERROR; 
    pthread_mutex_lock(&g_mutex);
    iRet = irtdb_fes_direct_send(pMsgBin);    
    pthread_mutex_unlock(&g_mutex);    
    return iRet;   
}

static irtdb_fes_message_bin* irtdb_fes_dump_di_base(
    int8_t dataType,char *label, char *valueLabel, 
    int32_t value, int64_t timestamp
)
{
    int iRet = RET_SUCCESS;
    unsigned char *pDataOut = NULL;
    irtdb_fes_di_msg *pFesMsg = NULL;
    irtdb_fes_message_bin *pMsgBin = NULL;
    irtdb_fes_cmn_path *path = NULL;
    int dataOutLen = 0;
    
    if(!label || !valueLabel)  return NULL;
    path = irtdb_fes_parse_cmn_path(label);
    if(!path || (path->num != 5))  return NULL;
    pFesMsg = irtdb_fes_create_di_msg(path,dataType,valueLabel,value,timestamp);
    irtdb_fes_free_cmn_path(path);  
    if(!pFesMsg)    return NULL;
    //irtdb_fes_show_di_msg(pFesMsg);
    pDataOut = irtdb_fes_dump_di_msg(pFesMsg,&dataOutLen);
    irtdb_fes_free_di_msg(pFesMsg);
    if(!pDataOut || !dataOutLen)  return NULL;
    pMsgBin = irtdb_fes_create_message_bin(pDataOut,dataOutLen);
	irtdb_fes_free_pointer(pDataOut);  
	
    return pMsgBin;
}

static int irtdb_fes_multiupload_di_base(int8_t dataType,irtdb_fep_di **dis, int disSize)
{
    int isErr = 0,iRet = 0;
    int i;
    irtdb_fep_di *di;
    irtdb_fes_message_bin **msgbinArr = NULL;
    irtdb_fes_message_bin *msgbin = NULL;
    
    if(!dis || (disSize <= 0)) return -1;
    msgbinArr = calloc(disSize,sizeof(irtdb_fes_message_bin*));
    if(!msgbinArr) return -1;
    
    for(i = 0; i < disSize ; i++)
    {
        di = dis[i];
		if(!di) 
        {
            isErr = 1;
            break;
        }
        msgbin = irtdb_fes_dump_di_base(
            dataType,di->path, di->label,di->value,di->timestamp);
        if(!msgbin)
        {
            isErr = 1;
            break;
        }
        msgbinArr[i] = msgbin;
    }
    
    if(isErr) 
        iRet = -1;
    else
    {
        // do send
        for(i = 0; i < disSize ; i++)
        {
            msgbin = msgbinArr[i];
            iRet = irtdb_fes_send_msgbin(msgbin);
            if(iRet != 0 ) break;
        }
    }
    // do clean
    for(i = 0; i < disSize ; i++)
    {
        msgbin = msgbinArr[i];
        if(!msgbin) irtdb_fes_delete_message_bin(msgbin);
    }
    free(msgbinArr);
    return iRet;
}

static irtdb_fes_message_bin* irtdb_fes_dump_ai_base(
	int8_t dataType,char *label,float value,int64_t timestamp
)
{
    unsigned char *pDataOut = NULL;
    int dataOutLen = 0;
    irtdb_fes_ai_msg *pFesMsg = NULL;
    irtdb_fes_message_bin *pMsgBin = NULL;
    irtdb_fes_cmn_path *path = NULL;
    
    if(!label) return NULL;
    path = irtdb_fes_parse_cmn_path(label);
    if(!path || (path->num != 5)) return NULL;
    pFesMsg = irtdb_fes_create_ai_msg(path,dataType,value,timestamp);
    irtdb_fes_free_cmn_path(path);
    if(!pFesMsg) return NULL;
    //irtdb_fes_show_ai_msg(pFesMsg);
    pDataOut = irtdb_fes_dump_ai_msg(pFesMsg,&dataOutLen);
    irtdb_fes_free_ai_msg(pFesMsg);
    if(!pDataOut || !dataOutLen) return NULL;
    CMN_LOGGER_DEBUG(logger,"dataOutLen : %d",dataOutLen);
    pMsgBin = irtdb_fes_create_message_bin(pDataOut,dataOutLen);
    irtdb_fes_free_pointer(pDataOut);  

	return pMsgBin;
}

static int irtdb_fes_multiupload_ai_base(int8_t dataType,irtdb_fep_ai **ais, int aisSize)
{
    int isErr = 0,iRet = 0;
    int i;
    irtdb_fep_ai *ai;
    irtdb_fes_message_bin **msgbinArr = NULL;
    irtdb_fes_message_bin *msgbin = NULL;
    
    if(!ais || (aisSize <= 0)) return -1;
    msgbinArr = calloc(aisSize,sizeof(irtdb_fes_message_bin*));
    if(!msgbinArr) return -1;
    
    for(i = 0; i < aisSize ; i++)
    {
        ai = ais[i];
		if(!ai) 
        {
            isErr = 1;
            break;
        }
        msgbin = irtdb_fes_dump_ai_base(
            dataType,ai->path,ai->value,ai->timestamp);
        if(!msgbin)
        {
            isErr = 1;
            break;
        }
        msgbinArr[i] = msgbin;
    }
    
    if(isErr) 
        iRet = -1;
    else
    {
        // do send
        for(i = 0; i < aisSize ; i++)
        {
            msgbin = msgbinArr[i];
            iRet = irtdb_fes_send_msgbin(msgbin);
            if(iRet != 0 ) break;
        }
    }
    // do clean
    for(i = 0; i < aisSize ; i++)
    {
        msgbin = msgbinArr[i];
        if(!msgbin) irtdb_fes_delete_message_bin(msgbin);
    }
    free(msgbinArr);
    return iRet;
}

static irtdb_fes_message_bin* irtdb_fes_dump_acc_base(
	int8_t dataType,char *label,int value,int64_t timestamp
)
{
    int iRet = RET_SUCCESS;
    unsigned char *pDataOut = NULL;
    int32_t labelLen = 0;
    int dataOutLen = 0;
    irtdb_fes_acc_msg *pFesMsg = NULL;
    irtdb_fes_message_bin  *pMsgBin = NULL;
    irtdb_fes_dbnode *pCur = NULL;
    irtdb_fes_cmn_path *path = NULL;
    
    if(!label) return NULL;  
    path = irtdb_fes_parse_cmn_path(label);
    if(!path || (path->num != 5)) return NULL;

    pFesMsg = irtdb_fes_create_acc_msg(path,dataType,value,timestamp);  
    irtdb_fes_free_cmn_path(path);    
    if(!pFesMsg)  return NULL;    
    //irtdb_fes_show_acc_msg(pFesMsg);
    pDataOut = irtdb_fes_dump_acc_msg(pFesMsg,&dataOutLen);
    irtdb_fes_free_acc_msg(pFesMsg);
    if(!pDataOut || !dataOutLen) return NULL;
    CMN_LOGGER_DEBUG(logger,"dataOutLen : %d",dataOutLen);
    pMsgBin = irtdb_fes_create_message_bin(pDataOut,dataOutLen);
    irtdb_fes_free_pointer(pDataOut); 

	return pMsgBin;
}

static int irtdb_fes_multiupload_acc_base(int8_t dataType,irtdb_fep_acc **accs, int accSize)
{
    int isErr = 0,iRet = 0;
    int i;
    irtdb_fep_acc *acc;
    irtdb_fes_message_bin **msgbinArr = NULL;
    irtdb_fes_message_bin *msgbin = NULL;
    
    if(!accs || (accSize <= 0)) return -1;
    msgbinArr = calloc(accSize,sizeof(irtdb_fes_message_bin*));
    if(!msgbinArr) return -1;
    
    for(i = 0; i < accSize ; i++)
    {
        acc = accs[i];
		if(!acc) 
        {
            isErr = 1;
            break;
        }
        msgbin = irtdb_fes_dump_acc_base(dataType,acc->path,acc->value,acc->timestamp);
        if(!msgbin)
        {
            isErr = 1;
            break;
        }
        msgbinArr[i] = msgbin;
    }
    
    if(isErr) 
        iRet = -1;
    else
    {
        // do send
        for(i = 0; i < accSize ; i++)
        {
            msgbin = msgbinArr[i];
            iRet = irtdb_fes_send_msgbin(msgbin);
            if(iRet != 0 ) break;
        }
    }
    // do clean
    for(i = 0; i < accSize ; i++)
    {
        msgbin = msgbinArr[i];
        if(!msgbin) irtdb_fes_delete_message_bin(msgbin);
    }
    free(msgbinArr);
    return iRet;
}

