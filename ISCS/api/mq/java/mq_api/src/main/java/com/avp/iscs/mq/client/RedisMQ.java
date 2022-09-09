package com.avp.iscs.mq.client;

import jni.InnerMQChannel;
import jni.InnerMessageListener;
import jni.InnerNativeMQ;

/*    Spring Boot中用法
//    @Autowired
//    private RedisMQ redisMQ;
//
//    @Bean
//    public RedisMQ createRedisMQ(){
//       RedisMQ mq = new RedisMQ();
//       mq.initialize();
//       return mq;
//    }
*/

public class RedisMQ {
	static {
	    try{
	    	System.loadLibrary("mq_api_jni");
	    }catch(UnsatisfiedLinkError ex){
	    }
	    try{
	    	System.loadLibrary("libmq_api_jni");
	    }catch(UnsatisfiedLinkError ex){
	    }
	}
	private RedisMQ() {
	}
	private static InnerNativeMQ innerMQ = new InnerNativeMQ();

    public static int initialize(String domain) {
        return innerMQ.initialize(domain);
    }

    public static int release() {
        return innerMQ.release();
    }

    public static int send(String domain, String queueOrTopic, String message, long timeout) {
        return innerMQ.send(domain, queueOrTopic, message, timeout);
    }

    public static String receive(String domain, String queueOrTopic, long timeout) {
        return innerMQ.receive(domain, queueOrTopic, timeout);
    }

///////////////////////////////////////////////////////////////////////////////////////////
//
//Under this line are not implemented
//
///////////////////////////////////////////////////////////////////////////////////////////
    
    public InnerMQChannel createMQChannel(String domain, String queue, int priority, int replyMode, String replyQueue) {
        return innerMQ.createMQChannel(domain, queue, priority, replyMode, replyQueue);
    }

    public int destroyMQChannel(InnerMQChannel channel) {
        return innerMQ.destroyMQChannel(channel);
    }

    public int policySend(InnerMQChannel channel, String payload, long timeout, int priority){
        return innerMQ.policySend(channel, payload, timeout, priority);
    }

    public int acknowledge(String domain, String replyQueue, String payload, long timeout) {
        return innerMQ.acknowledge(domain, replyQueue, payload, timeout);
    }

    public int subscribe(String domain, String queueOrTopic, MessageListener listener) {
        return innerMQ.subscribe(domain, queueOrTopic, listener);
    }

    public int unsubscribe(String domain, String queueOrTopic, MessageListener listener) {
        return innerMQ.unsubscribe(domain, queueOrTopic, listener);
    }

    public interface MessageListener extends InnerMessageListener {
        @Override
        void OnMessage(String buffer);
    }
}
