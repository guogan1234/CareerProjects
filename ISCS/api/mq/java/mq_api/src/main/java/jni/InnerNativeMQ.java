package jni;

public class InnerNativeMQ {
    public native int initialize(String domain);

    public native int release();

    public native int send(String domain, String queueOrTopic, String payload, long timeout);

    public native String receive(String domain, String queueOrTopic, long timeout);

    public native InnerMQChannel createMQChannel(String domain, String queue, int priority, int replyMode, String replyQueue);

    public native int destroyMQChannel(InnerMQChannel channel);

    public native int acknowledge(String domain, String replyQueue, String payload, long timeout);

    public native int policySend(InnerMQChannel channel, String payload, long timeout, int priority);

    public native int subscribe(String domain, String queueOrTopic, InnerMessageListener listener);

    public native int unsubscribe(String domain, String queueOrTopic, InnerMessageListener listener);
}
