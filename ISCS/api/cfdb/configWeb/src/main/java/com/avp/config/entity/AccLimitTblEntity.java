package com.avp.config.entity;

import javax.persistence.*;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
@Table(name = "acc_limit_tbl", schema = "conf")
public class AccLimitTblEntity {
    private String accLimitLabel;
    @Column(name = "point_tp_label",length = 32)
    private String pointTpLabel;
    private Integer domainId;
    private Integer locationId;
    private Integer proSystemId;
    private Integer regionId;
    private Integer limitNum;
    private Integer limitUp1;
    private Integer limitUp2;
    private Integer limitUp3;
    private Integer limitLow1;
    private Integer limitLow2;
    private Integer limitLow3;
    private Integer alarmLevel;
    private String picName;
    private Integer alarmDelayTime;
    private Boolean isReplaceAlarm;
    private Integer isDelOnAck;
    @Column(name = "operate_time")
    private Timestamp operateTime;

    @Id
    @Column(name = "acc_limit_label", nullable = false, length = 128)
    public String getAccLimitLabel() {
        return accLimitLabel;
    }

    public void setAccLimitLabel(String accLimitLabel) {
        this.accLimitLabel = accLimitLabel;
    }

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
    @Column(name = "region_id", nullable = true)
    public Integer getRegionId() {
        return regionId;
    }

    public void setRegionId(Integer regionId) {
        this.regionId = regionId;
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
    @Column(name = "limit_up1", nullable = true)
    public Integer getLimitUp1() {
        return limitUp1;
    }

    public void setLimitUp1(Integer limitUp1) {
        this.limitUp1 = limitUp1;
    }

    @Basic
    @Column(name = "limit_up2", nullable = true)
    public Integer getLimitUp2() {
        return limitUp2;
    }

    public void setLimitUp2(Integer limitUp2) {
        this.limitUp2 = limitUp2;
    }

    @Basic
    @Column(name = "limit_up3", nullable = true)
    public Integer getLimitUp3() {
        return limitUp3;
    }

    public void setLimitUp3(Integer limitUp3) {
        this.limitUp3 = limitUp3;
    }

    @Basic
    @Column(name = "limit_low1", nullable = true)
    public Integer getLimitLow1() {
        return limitLow1;
    }

    public void setLimitLow1(Integer limitLow1) {
        this.limitLow1 = limitLow1;
    }

    @Basic
    @Column(name = "limit_low2", nullable = true)
    public Integer getLimitLow2() {
        return limitLow2;
    }

    public void setLimitLow2(Integer limitLow2) {
        this.limitLow2 = limitLow2;
    }

    @Basic
    @Column(name = "limit_low3", nullable = true)
    public Integer getLimitLow3() {
        return limitLow3;
    }

    public void setLimitLow3(Integer limitLow3) {
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

    public Timestamp getOperateTime() {
        return operateTime;
    }

    public void setOperateTime(Timestamp operateTime) {
        this.operateTime = operateTime;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AccLimitTblEntity that = (AccLimitTblEntity) o;
        return Objects.equals(accLimitLabel, that.accLimitLabel) &&
                Objects.equals(pointTpLabel, that.pointTpLabel) &&
                Objects.equals(domainId, that.domainId) &&
                Objects.equals(locationId, that.locationId) &&
                Objects.equals(proSystemId, that.proSystemId) &&
                Objects.equals(regionId, that.regionId) &&
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

        return Objects.hash(accLimitLabel, pointTpLabel, domainId, locationId, proSystemId, regionId, limitNum, limitUp1, limitUp2, limitUp3, limitLow1, limitLow2, limitLow3, alarmLevel, picName, alarmDelayTime, isReplaceAlarm, isDelOnAck);
    }
}
