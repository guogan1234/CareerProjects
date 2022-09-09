
#include "jni_InnerNativeMQ.h"

#include <mq.h>


/*
 * Class:     jni_InnerNativeMQ
 * Method:    initialize
 * Signature: ()I
 */
jint JNICALL Java_jni_InnerNativeMQ_initialize(JNIEnv *jniEnv, jobject thiz, jstring domain){
    const char* sDomain = (*jniEnv)->GetStringUTFChars(jniEnv, domain, NULL);
    int ires = mq_init(sDomain);
    (*jniEnv)->ReleaseStringUTFChars(jniEnv, domain, sDomain);
    
    return ires;
}

/*
 * Class:     jni_InnerNativeMQ
 * Method:    release
 * Signature: ()I
 */
jint JNICALL Java_jni_InnerNativeMQ_release(JNIEnv *jniEnv, jobject thiz){
    int ires = mq_release();
    return ires;
}

/*
 * Class:     jni_InnerNativeMQ
 * Method:    send
 * Signature: (Ljava/lang/String;Ljava/lang/String;J)I
 */
jint JNICALL Java_jni_InnerNativeMQ_send
  (JNIEnv *jniEnv, jobject thiz, jstring domain, jstring queueOrTopic, jstring message, jlong timeout){
    const char* sDomain = (*jniEnv)->GetStringUTFChars(jniEnv, domain, NULL);
    const char* sQueueOrTopic = (*jniEnv)->GetStringUTFChars(jniEnv, queueOrTopic, NULL);
    jsize msgSize = (*jniEnv)->GetStringUTFLength(jniEnv, message);
    const char* sMessage = (*jniEnv)->GetStringUTFChars(jniEnv, message, NULL);
    int ires = mq_send(sDomain, sQueueOrTopic, sMessage, (int)msgSize, (long)timeout);
    
    (*jniEnv)->ReleaseStringUTFChars(jniEnv, domain, sDomain);
    (*jniEnv)->ReleaseStringUTFChars(jniEnv, queueOrTopic, sQueueOrTopic);
    (*jniEnv)->ReleaseStringUTFChars(jniEnv, message, sMessage);
    
    return ires;
}

/*
 * Class:     jni_InnerNativeMQ
 * Method:    receive
 * Signature: (Ljava/lang/String;Ljava/lang/String;J)Ljava/lang/String;
 */
jstring JNICALL Java_jni_InnerNativeMQ_receive(JNIEnv *jniEnv, jobject thiz, jstring domain, jstring queueOrTopic, jlong timeout){
    const char* sDomain = (*jniEnv)->GetStringUTFChars(jniEnv, domain, NULL);
    const char* sQueueOrTopic = (*jniEnv)->GetStringUTFChars(jniEnv, queueOrTopic, NULL);
    jstring jsret = NULL;
    char* buffer = NULL;
    int bufSize = 0;
    char* replybuffer = NULL;
    int replySize = 0;
    int result = mq_receive(sDomain, sQueueOrTopic, (long)timeout, &buffer, &bufSize, &replybuffer, &replySize);
    if(result == 0) {
        jsret = (*jniEnv)->NewStringUTF(jniEnv, buffer);
        if(replySize > 1){
            mq_acknowledge(replybuffer, replySize, 0, MQ_MSG_TIMEOUT_DEFAULT/1000); //for Java, no manual acknowledge from caller
        }
    }

    if(buffer!= NULL) mq_received_free(buffer);
    if(replybuffer!= NULL) mq_received_free(replybuffer);

    (*jniEnv)->ReleaseStringUTFChars(jniEnv, domain, sDomain);
    (*jniEnv)->ReleaseStringUTFChars(jniEnv, queueOrTopic, sQueueOrTopic);
    
    return(jsret);
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// Under this line are not implemented
//
///////////////////////////////////////////////////////////////////////////////////////////

/*
 * Class:     jni_InnerNativeMQ
 * Method:    createMQChannel
 * Signature: (Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;)Ljni/InnerMQChannel;
 */
JNIEXPORT jobject JNICALL Java_jni_InnerNativeMQ_createMQChannel
  (JNIEnv *jniEnv, jobject thiz, jstring domain, jstring queue, jint priority, jint replyMode, jstring replyQueue) {
//    //获取Java中的实例类
//    jclass channelClazz = (*jniEnv)->FindClass(jniEnv, "jni/InnerMQChannel");
//    jobject channel = (*jniEnv)->AllocObject(jniEnv, channelClazz);
//    if(!channel) {
//        return NULL;
//    }

//    const char* sDomain = (*jniEnv)->GetStringUTFChars(jniEnv, domain, NULL);
//    const char* sQueue = (*jniEnv)->GetStringUTFChars(jniEnv, queue, NULL);
//    const char* sReplyQueue = (*jniEnv)->GetStringUTFChars(jniEnv, replyQueue, NULL);
//    MQChannel* pChannel = mq_channel_create(sDomain, sQueue, (int)priority, (int)replyMode, sReplyQueue);

//    if(pChannel) {
//        //获取Java中的实例类
//        jclass channelClazz = (*jniEnv)->FindClass(jniEnv, "jni/InnerMQChannel");
//        jobject channel = (*jniEnv)->AllocObject(jniEnv, channelClazz);
//        //获取类中每一个变量的定义
//        jfieldID fid = (*jniEnv)->GetFieldID(jniEnv, channelClazz, "domain","Ljava/lang/String;");
//        (*jniEnv)->SetObjectField(jniEnv, channel, fid, (*jniEnv)->NewStringUTF(jniEnv, pChannel->domain));

//        free(pChannel);
//        return channel;
//    }

    return NULL;
}

/*
 * Class:     jni_InnerNativeMQ
 * Method:    destroyMQChannel
 * Signature: (Ljni/InnerMQChannel;)I
 */
JNIEXPORT jint JNICALL Java_jni_InnerNativeMQ_destroyMQChannel
  (JNIEnv *jniEnv, jobject thiz, jobject channel) {
      return -1;
}

/*
 * Class:     jni_InnerNativeMQ
 * Method:    policySend
 * Signature: (Ljni/InnerMQChannel;Ljava/lang/String;JI)I
 */
JNIEXPORT jint JNICALL Java_jni_InnerNativeMQ_policySend
  (JNIEnv *jniEnv, jobject thiz, jobject channel, jstring payload, jlong timeout, jint priority) {
      return -1;
}	  
  

/*
 * Class:     jni_InnerNativeMQ
 * Method:    acknowledge
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_jni_InnerNativeMQ_acknowledge
  (JNIEnv *jniEnv, jobject thiz, jstring domain, jstring replyQueue, jstring payload, jlong timeout) {
    const char* sDomain = (*jniEnv)->GetStringUTFChars(jniEnv, domain, NULL);
    const char* sReplyQueue = (*jniEnv)->GetStringUTFChars(jniEnv, replyQueue, NULL);
    jsize payloadSize = (*jniEnv)->GetStringUTFLength(jniEnv, payload);
    const char* sPayload = (*jniEnv)->GetStringUTFChars(jniEnv, payload, NULL);
    return mq_acknowledge(sPayload, (int)payloadSize, 1, (long)timeout);
}

/*
 * Class:     jni_InnerNativeMQ
 * Method:    subscribe
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljni/InnerNativeMQ/InnerMessageListener;)I
 */
jint JNICALL Java_jni_InnerNativeMQ_subscribe(JNIEnv *jniEnv, jobject thiz, jstring domain, jstring queueOrTopic, jobject callback){
    return -1;
}

/*
 * Class:     jni_InnerNativeMQ
 * Method:    unsubscribe
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljni/InnerNativeMQ/InnerMessageListener;)I
 */
jint JNICALL Java_jni_InnerNativeMQ_unsubscribe(JNIEnv *jniEnv, jobject thiz, jstring domain, jstring queueOrTopic, jobject callback){
    return -1;
}