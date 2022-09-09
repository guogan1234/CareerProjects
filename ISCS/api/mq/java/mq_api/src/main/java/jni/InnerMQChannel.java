package jni;

/**
 * Created by boris feng on 2018/7/26.
 */
public class InnerMQChannel {
    public Integer priority;
    public Integer replyMode;
    public String domain;
    public String queName;
    public String replyQueue;

    public InnerMQChannel(Integer priority, Integer replyMode, String domain, String queName, String replyQueue) {
        this.priority = priority;
        this.replyMode = replyMode;
        this.domain = domain;
        this.queName = queName;
        this.replyQueue = replyQueue;
    }
}
