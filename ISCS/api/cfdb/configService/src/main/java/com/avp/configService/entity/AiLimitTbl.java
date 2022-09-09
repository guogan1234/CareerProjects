package com.avp.configService.entity;

import javax.persistence.*;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
@Table(name = "ai_limit_tbl", schema ="conf")
public class AiLimitTbl {
    private String aiLimitLabel;
    private String pointTpLabel;
    private Integer domainId;
    private Integer locationId;
    private Integer proSystemId;
    private Integer limitNum;
    private Float limitUp1;
    private Float limitUp2;
    private Float limitUp3;
    private Float limitLow1;
    private Float limitLow2;
    private Float limitLow3;
    private Integer alarmLevel;
    private String picName;
    private Integer alarmDelayTime;
    private Boolean isReplaceAlarm;
    private Integer isDelOnAck;

    @Id
    @Column(name = "ai_limit_label", nullable = false, length = 128)
    public String getAiLimitLabel() {
        return aiLimitLabel;
    }

    public void setAiLimitLabel(String aiLimitLabel) {
        this.aiLimitLabel = aiLimitLabel;
    }

    @Basic
    @Column(name = "point_tp_label", nullable = true, length = 32)
    public String getPointTpLabel() {
        return pointTpLabel;
    }

    public void setPointTpLabel(String pointTpLabel) {
        this.pointTpLabel = pointTpLabel;
    }

    @Basic
    @Column(name = "domain_id", nullable = true)
    public Integer getDomainId() {
        return domainId;
    }

    public void setDomainId(Integer domainId) {
        this.domainId = domainId;
    }

    @Basic
    @Column(name = "location_id", nullable = true)
    public Integer getLocationId() {
        return locationId;
    }

    public void setLocationId(Integer locationId) {
        this.locationId = locationId;
    }

    @Basic
    @Column(name = "pro_system_id", nullable = true)
    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }

    @Basic
    @Column(name = "limit_num", nullable = true)
    public Integer getLimitNum() {
        return limitNum;
    }

    public void setLimitNum(Integer limitNum) {
        this.limitNum = limitNum;
    }

    @Basic
    @Column(name = "limit_up1", nullable = true, precision = 0)
    public Float getLimitUp1() {
        return limitUp1;
    }

    public void setLimitUp1(Float limitUp1) {
        this.limitUp1 = limitUp1;
    }

    @Basic
    @Column(name = "limit_up2", nullable = true, precision = 0)
    public Float getLimitUp2() {
        return limitUp2;
    }

    public void setLimitUp2(Float limitUp2) {
        this.limitUp2 = limitUp2;
    }

    @Basic
    @Column(name = "limit_up3", nullable = true, precision = 0)
    public Float getLimitUp3() {
        return limitUp3;
    }

    public void setLimitUp3(Float limitUp3) {
        this.limitUp3 = limitUp3;
    }

    @Basic
    @Column(name = "limit_low1", nullable = true, precision = 0)
    public Float getLimitLow1() {
        return limitLow1;
    }

    public void setLimitLow1(Float limitLow1) {
        this.limitLow1 = limitLow1;
    }

    @Basic
    @Column(name = "limit_low2", nullable = true, precision = 0)
    public Float getLimitLow2() {
        return limitLow2;
    }

    public void setLimitLow2(Float limitLow2) {
        this.limitLow2 = limitLow2;
    }

    @Basic
    @Column(name = "limit_low3", nullable = true, precision = 0)
    public Float getLimitLow3() {
        return limitLow3;
    }

    public void setLimitLow3(Float limitLow3) {
        this.limitLow3 = limitLow3;
    }

    @Basic
    @Column(name = "alarm_level", nullable = true)
    public Integer getAlarmLevel() {
        return alarmLevel;
    }

    public void setAlarmLevel(Integer alarmLevel) {
        this.alarmLevel = alarmLevel;
    }

    @Basic
    @Column(name = "pic_name", nullable = true, length = 64)
    public String getPicName() {
        return picName;
    }

    public void setPicName(String picName) {
        this.picName = picName;
    }

    @Basic
    @Column(name = "alarm_delay_time", nullable = true)
    public Integer getAlarmDelayTime() {
        return alarmDelayTime;
    }

    public void setAlarmDelayTime(Integer alarmDelayTime) {
        this.alarmDelayTime = alarmDelayTime;
    }

    @Basic
    @Column(name = "is_replace_alarm", nullable = true)
    public Boolean getIsReplaceAlarm() {
        return isReplaceAlarm;
    }

    public void setIsReplaceAlarm(Boolean isReplaceAlarm) {
        this.isReplaceAlarm = isReplaceAlarm;
    }

    @Basic
    @Column(name = "is_del_on_ack", nullable = true)
    public Integer getIsDelOnAck() {
        return isDelOnAck;
    }

    public void setIsDelOnAck(Integer isDelOnAck) {
        this.isDelOnAck = isDelOnAck;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AiLimitTbl that = (AiLimitTbl) o;
        return Objects.equals(aiLimitLabel, that.aiLimitLabel) &&
                Objects.equals(pointTpLabel, that.pointTpLabel) &&
                Objects.equals(domainId, that.domainId) &&
                Objects.equals(locationId, that.locationId) &&
                Objects.equals(proSystemId, that.proSystemId) &&
                Objects.equals(limitNum, that.limitNum) &&
                Objects.equals(limitUp1, that.limitUp1) &&
                Objects.equals(limitUp2, that.limitUp2) &&
                Objects.equals(limitUp3, that.limitUp3) &&
                Objects.equals(limitLow1, that.limitLow1) &&
                Objects.equals(limitLow2, that.limitLow2) &&
                Objects.equals(limitLow3, that.limitLow3) &&
                Objects.equals(alarmLevel, that.alarmLevel) &&
                Objects.equals(picName, that.picName) &&
                Objects.equals(alarmDelayTime, that.alarmDelayTime) &&
                Objects.equals(isReplaceAlarm, that.isReplaceAlarm) &&
                Objects.equals(isDelOnAck, that.isDelOnAck);
    }

    @Override
    public int hashCode() {

        return Objects.hash(aiLimitLabel, pointTpLabel, domainId, locationId, proSystemId, limitNum, limitUp1, limitUp2, limitUp3, limitLow1, limitLow2, limitLow3, alarmLevel, picName, alarmDelayTime, isReplaceAlarm, isDelOnAck);
    }
}
