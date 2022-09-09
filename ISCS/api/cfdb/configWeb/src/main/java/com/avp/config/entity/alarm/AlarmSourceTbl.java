package com.avp.config.entity.alarm;

import javax.persistence.*;

@Entity
@Table(name = "alarm_source_tbl",schema = "conf")
public class AlarmSourceTbl {
    @Id
    private  Integer sourceId;
    private  String sourceName;
    private  Integer alarmType;
    private  Integer alarmLevel;
    @Basic
    @Column(name="is_replace_alarm")
    private  Boolean ReplaceAlarm;
    private  Integer isDelOnAck;

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
        return ReplaceAlarm;
    }

    public void setReplaceAlarm(Boolean replaceAlarm) {
        ReplaceAlarm = replaceAlarm;
    }

    public Integer getIsDelOnAck() {
        return isDelOnAck;
    }

    public void setIsDelOnAck(Integer isDelOnAck) {
        this.isDelOnAck = isDelOnAck;
    }
}
