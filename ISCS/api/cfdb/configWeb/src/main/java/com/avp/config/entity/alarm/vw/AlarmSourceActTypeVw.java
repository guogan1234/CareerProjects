package com.avp.config.entity.alarm.vw;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Created by zhoujs on 2018/6/14.
 */
@Entity
@Table(name = "alarm_source_act_type_vw", schema = "conf")
public class AlarmSourceActTypeVw {
    @Id
    private Integer sourceId;
    private String sourceName;
    private Integer alarmType;
    private Integer alarmLevel;
    private Boolean isReplaceAlarm;
    private Integer isDelOnAck;
    private String levelName;
    private String alarmAct;
    private String typeName;

    public Integer getSourceId() {

        return sourceId;
    }

    public void setSourceId(Integer sourceId) {
        this.sourceId = sourceId;
    }

    public String getSourceName() {
        return sourceName;
    }

    public void setSourceName(String sourceName) {
        this.sourceName = sourceName;
    }

    public Integer getAlarmType() {
        return alarmType;
    }

    public void setAlarmType(Integer alarmType) {
        this.alarmType = alarmType;
    }

    public Integer getAlarmLevel() {
        return alarmLevel;
    }

    public void setAlarmLevel(Integer alarmLevel) {
        this.alarmLevel = alarmLevel;
    }

    public Boolean getReplaceAlarm() {
        return isReplaceAlarm;
    }

    public void setReplaceAlarm(Boolean replaceAlarm) {
        isReplaceAlarm = replaceAlarm;
    }

    public Integer getIsDelOnAck() {
        return isDelOnAck;
    }

    public void setIsDelOnAck(Integer isDelOnAck) {
        this.isDelOnAck = isDelOnAck;
    }

    public String getLevelName() {
        return levelName;
    }

    public void setLevelName(String levelName) {
        this.levelName = levelName;
    }

    public String getAlarmAct() {
        return alarmAct;
    }

    public void setAlarmAct(String alarmAct) {
        this.alarmAct = alarmAct;
    }

    public String getTypeName() {
        return typeName;
    }

    public void setTypeName(String typeName) {
        this.typeName = typeName;
    }
}
