package com.avp.cdai.web.config;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.PropertySource;
import org.springframework.stereotype.Component;

/**
 * Created by guo on 2017/9/26.
 */
@Component
@PropertySource("classpath:project.properties")
@ConfigurationProperties(prefix = "cfg")
public class ProjectConfig {
    private String pushDelay;
    private String pushUrl;
    private Integer test;

    private String mapLevel1;
    private String mapLevel2;
    private String mapLevel3;

    private String alarmWebUrl;
    private String alarmPushUrl;
    private String alarmAgentId;

    public String getAlarmWebUrl() {
        return alarmWebUrl;
    }

    public void setAlarmWebUrl(String alarmWebUrl) {
        this.alarmWebUrl = alarmWebUrl;
    }

    public String getAlarmPushUrl() {
        return alarmPushUrl;
    }

    public void setAlarmPushUrl(String alarmPushUrl) {
        this.alarmPushUrl = alarmPushUrl;
    }

    public String getAlarmAgentId() {
        return alarmAgentId;
    }

    public void setAlarmAgentId(String alarmAgentId) {
        this.alarmAgentId = alarmAgentId;
    }

    public String getMapLevel1() {
        return mapLevel1;
    }

    public void setMapLevel1(String mapLevel1) {
        this.mapLevel1 = mapLevel1;
    }

    public String getMapLevel2() {
        return mapLevel2;
    }

    public void setMapLevel2(String mapLevel2) {
        this.mapLevel2 = mapLevel2;
    }

    public String getMapLevel3() {
        return mapLevel3;
    }

    public void setMapLevel3(String mapLevel3) {
        this.mapLevel3 = mapLevel3;
    }

    public String getPushDelay() {
        return pushDelay;
    }

    public void setPushDelay(String pushDelay) {
        this.pushDelay = pushDelay;
    }

    public String getPushUrl() {
        return pushUrl;
    }

    public void setPushUrl(String pushUrl) {
        this.pushUrl = pushUrl;
    }

    public Integer getTest() {
        return test;
    }

    public void setTest(Integer test) {
        this.test = test;
    }
}
