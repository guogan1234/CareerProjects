/**************************************************************************
 * Message Queue Interface
 * Version: 1.0
 * Track:
 * 1.0  20180401 Create                                                     Boris.F
 **************************************************************************/


#ifndef MQ_INTERFACE_H
#define MQ_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mq_constant.h"

//点对点消息队列通道
typedef struct MQChannel MQChannel;

//消息回调函数定义
typedef int (*MessageListener)(void* buffer, const int size, void* replybuff, const int replysize);

/*
功能：初始化接口
说明：
    MQ客户端初始化，非线程安全
参数：
    domain      :  mq所在域，默认空为当前域
    username    :  认证账号
    passwd      :  认证密码
    poolSize    :  连接池中连接数量

*/
int mq_init(const char *domain);

/*
功能：回收接口
说明：
    MQ客户端资源回收，关闭回话、连接和连接池
参数：
    无
*/
int mq_release();

/*
功能：创建自定策略的点对点消息发送通道
说明：
    自定发送消息包的策略，包括：优先级、应答模式、应答队列
    提示：发送队列、应答队列均在发送端创建，策略定向至消息包，而非消息队列
参数：
    queue       :  消息队列名称
    priority    :  消息通道的优先级，通道优先级高的消息先被推送
    replyMode   :  消息包的应答模式，分自动、手动
    replyQueue  :  应答消息包的发送队列名
*/
MQChannel* mq_channel_create(const char* domain, const char* queue, int priority, int replyMode, const char* replyQueue);

/*
功能：销毁通道
说明：
    无
参数：
    channel     :  通道对象
*/
int mq_channel_destroy(MQChannel* channel);

/*
功能：自定策略的点对点消息发送
说明：
    自定发送消息包的策略，包括：优先级、应答模式、应答队列
    提示：发送队列、应答队列均在发送端创建，策略定向至消息包，而非消息队列
参数：
    domain      :  目标域名称
    queue       :  消息队列名称
    payload     :  消息载体
    size        :  消息大小
    priority    :  消息包的优先级，即同一队列中优先级高的消息先被推送
    replyMode   :  消息包的应答模式，分自动、手动
    replyQueue  :  应答消息包的发送队列名
    timeout     :  发送超时时长，毫秒
返回值：
    0：ACK， 小于0：NAK，用户自定义错误码计算方式：ret & 0x000000FF
*/
int mq_policy_send(MQChannel* channel, const char* payload, const int size, const long timeout, const int priority);

/*
功能：消息发送:默认策略的点对点和主题广播
说明：
    无
参数：
    queueOrTopic :  消息队列或主题广播名称，以queue://或topic://开头
    payload      :  消息载体
    size         :  消息大小
    timeout      :  发送超时时长，毫秒
*/
int mq_send(const char* domain, const char* queueOrTopic, const char* payload, const int size, const long timeout);

/*
功能：点对点消息手动应答
说明：
    无
参数：
    replybuffer :  acknowledge buffer
    replybuffersize:  消息大小
    ackstatus   :  acknowledge status(x00~0xFE) 0: ACK OK, 0xFF：系统保留，其他: 用户自定NAK
    timeout     :  发送超时时长，毫秒
*/
int mq_acknowledge(const char* replybuffer, const int replybuffersize, const int ackstatus, const long timeout);

///*
//功能：跨域主题广播
//说明：
//    无
//参数：
//    domain      :  目标域名称
//    topic       :  主题名称
//    payload     :  消息载体
//    size        :  消息大小
//    timeout     :  发送超时时长，毫秒
//*/
//int mq_broadcastto(const char* domain, const char* topic, const char* payload, const int size, const long timeout);

/*
功能：消息或者广播接收
说明：
    无
参数：
    domain       :  目标域名称
    queueOrTopic :  队列或主题名称，以queue://或topic://开头
    buffer       :  消息存储缓冲区
    size         :  缓冲区大小
    timeout      :  接收超时时长，毫秒
*/
int mq_receive(const char* domain, const char* queueOrTopic, const long timeout, char** buffer, int* size, char** replybuffer, int* replysize);

/*
功能：释放通过mq_receive接收到的消息包内存
说明：
    无
参数：
    buffer       :  消息存储缓冲区
*/
int mq_received_free(char *buffer);

/*
功能：消息或者广播注册监听
说明：
    无
参数：
    queueOrTopic :  队列或主题名称，以queue://或topic://开头
    listener     :  监听回调函数指针
*/
int mq_subscribe(const char* domain, const char* queueOrTopic, const MessageListener listener);

/*
功能：消息或者广播注销监听
说明：
    无
参数：
    domain       :  目标域名称，以queue://或topic://开头
    queueOrTopic :  队列或主题名称
    listener     :  监听回调函数指针
*/
int mq_unsubscribe(const char* domain, const char* queueOrTopic, const MessageListener listener);

int mq_process_loop();

//点对点消息队列通道
struct MQChannel {
    int priority;
    int replyMode;
	int zipMode;
    char domain[MQ_DOMAIN_SIZE];        // keyA
    char queName[MQ_TOPIC_SIZE];        // keyB
    char replyQueue[MQ_TOPIC_SIZE];

};

#ifdef __cplusplus
}
#endif

#endif //MQ_INTERFACE_H
