#ifndef MQ_CONSTANT_H
#define MQ_CONSTANT_H

// ERR CODE
#define MQ_NOT_IMPL               0xFFFFFFFF         
#define MQ_OK                     0x00000000

#define MQ_ERROR                  0x80000000
#define MQ_NOT_INIT               0x80000001
#define MQ_SESSION_BUSY           0x80000002
#define MQ_ILLEGAL_PARAM          0x80000004
#define MQ_CONN_BREAK             0x80000008
#define MQ_SESSION_SUBSCRIBED     0x80000010
#define MQ_SYSTEM_ERROR_TRY_AGAIN 0x80000020
#define MQ_SYSTEM_ERROR_MEMORY 	0x80000040
#define MQ_SYSTEM_ERROR_COMPRESS 	0x80000080
#define MQ_SYSTEM_ERROR_OVER_SIZE 	0x80000100

#define MQ_NACK_STATUS_BASE       0x81000000
#define MQ_NACK_STATUS1           0x81000001
#define MQ_NACK_STATUS2           0x81000002
#define MQ_NACK_TIMEOUT           0x81FFFFFF

#define MQ_ERROR_PROCESSER_NOT_READY		0x82000000
#define MQ_OK_ALREADY_SUBSCRIBED            0x00000001
#define MQ_OK_NOT_SUBSCRIBED                0x00000002
/////////////////////////

#define MQ_TRUE    1
#define MQ_FALSE   0


#define MQ_POOL_SIZE_DEFAULT   20 //nerver exceed the default value
#define MQ_OPTION_SIZE                   50
#define MQ_TOPIC_SIZE                    100
#define MQ_DOMAIN_SIZE                   100
#define MQ_ACK_MESSAGE_ID_SIZE           150
#define MQ_EVENT_PROCESSOR_NUM           2
#define MQ_QUEUE_SUBSCRIBE_LIST_SIZE     2048
#define MQ_ACKNOWLEDGE_PAYLOAD_SIZE 4
#define MQ_QUEUE_MAX_SIZE 						20000000

// 订阅类型
#define MQ_QUEUE                    "queue://"
#define MQ_TOPIC                    "topic://"
#define LEN_MQ_TQ_HEAD              8
#define MAXLEN_QUEUE_TOPIC_NAME     100

//host seperator
#define MQ_HOST_SEPERATOR           ";"
#define MQ_PORT_SEPERATOR           ":"
#define MQ_PROTOCOL_SEPERATOR       "://"

//#define MQ_PROTOCOL_TCP                  "tcp"
//#define MQ_PROTOCOL_UDP                  "udp"

///////////////////////////////////
// reply mode
#define MQ_MSG_REPLY_MANUAL             1
#define MQ_MSG_REPLY_AUTO               	2
#define MQ_MSG_REPLY_DEFAUTL            2
#define MQ_MSG_REPLY_NO            			0

// TTL,Default 60 seconds
#define MQ_MSG_TTL_DEFAULT              60000
// Version
#define MQ_MSG_VERSION                  100
// Timeout, Default 60 seconds
#define MQ_MSG_TIMEOUT_DEFAULT          60000
//Priority default
#define MQ_MSG_PRIORITY_DEFAULT         0
////////////////////////////////////

#define MQ_QUEUE_RECEIVE_TIMEOUT        1
#define MQ_TOPIC_SUBSCRIBE_TIMEOUT      1


#define MQ_QUEUE_ACK_TAG                "ACK"

#define MQ_MSG_TYPE_QUEUE_NORMAL        1
#define MQ_MSG_TYPE_QUEUE_ACK           2
#define MQ_MSG_TYPE_TOPIC               3

#define MQ_INTERVAL_NOT_INITIALIZED			1
#define MQ_OK_NO_CHANGE							1

#define MQ_TQTYPE_QUEUE	 							0
#define MQ_TQTYPE_TOPIC								1

////////////////////////////////////
#define MQ_CONF_NUM_QUEUE_SEND_THREADS      "num_queue_send_threads"
#define MQ_CONF_NUM_TOPIC_SEND_THREADS      "num_topic_send_threads"
#define MQ_CONF_NUM_EVENT_PROCESS_POOL_SIZE "num_event_process_pool_size"
#define MQ_CONF_DISABLE_TOPIC_CHANNEL       "mq_disable_topic_channel"
#define MQ_CONF_DISABLE_QUEUE_CHANNEL       "mq_disable_queue_channel"
#define MQ_CONF_BZPOP_SUPPORTED             "mq_bzpop_supported"
#define MQ_CONF_DOMAIN_NAME2ID_ENABLED      "mq_domain_name2id_enabled"
#define MQ_CONF_MAX_BUFFER_SIZE_PER_QUEUE   "mq_max_buffer_size_per_queue"


#endif //MQ_CONSTANT_H

