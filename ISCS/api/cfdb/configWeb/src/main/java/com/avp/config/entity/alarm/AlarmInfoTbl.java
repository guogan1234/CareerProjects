package com.avp.config.entity.alarm;

import javax.persistence.*;
import java.io.Serializable;

/**
 * 报警信息表
 * fxy
 */
@Entity
@Table(name = "alarm_Info_tbl", schema = "conf")
@IdClass(AlarmInfoMultiKeys.class)
public class AlarmInfoTbl implements Serializable {

    private static final long serialVersionUID = 7479078525641709871L;
    @Id
    @Column(name = "label")
    private String label;
    @Id
    @Column(name = "second")
    private Integer second;
    @Id
    @Column(name = "msecond")
    private Integer msecond;
    @Basic
    @Column(name = "alarm_type")
    private Integer alarmType;
    @Basic
    @Column(name = "domain_id")
    private Integer domainId;
    @Basic
    @Column(name = "station_id")
    private Integer stationId;
    @Basic
    @Column(name = "region_id")
    private Integer regionId;
    @Basic
    @Column(name = "dev_type_label")
    private String devTypeLabel;
    @Basic
    @Column(name = "ctrl_trans_grp_label")
    private String ctrlTransGrpLabel;
    @Basic
    @Column(name = "is_restrain")
    private Boolean restrain;
    @Basic
    @Column(name = "pic_name")
    private String picName;
    @Basic
    @Column(name = "is_recovery")
    private Boolean recovery;
    @Basic
    @Column(name = "is_del_on_ack")
    private Integer delOnAck;
    @Basic
    @Column(name = "is_replace_alarm")
    private Boolean replaceAlarm;
    @Basic
    @Column(name = "alarm_level")
    private Integer alarmLevel;
    @Basic
    @Column(name = "content")
    private String content;
    @Basic
    @Column(name = "is_acked")
    private Boolean acked;

    public String getLabel() {
        return label;
    }

    public void setLabel(String label) {
        this.label = label;
    }

    public Integer getSecond() {
        return second;
    }

    public void setSecond(Integer second) {
        this.second = second;
    }

    public Integer getMsecond() {
        return msecond;
    }

    public void setMsecond(Integer msecond) {
        this.msecond = msecond;
    }

    public Integer getAlarmType() {
        return alarmType;
    }

    public void setAlarmType(Integer alarmType) {
        this.alarmType = alarmType;
    }

    public Integer getDomainId() {
        return domainId;
    }

    public void setDomainId(Integer domainId) {
        this.domainId = domainId;
    }

    public Integer getStationId() {
        return stationId;
    }

    public void setStationId(Integer stationId) {
        this.stationId = stationId;
    }

    public Integer getRegionId() {
        return regionId;
    }

    public void setRegionId(Integer regionId) {
        this.regionId = regionId;
    }

    public String getDevTypeLabel() {
        return devTypeLabel;
    }

    public void setDevTypeLabel(String devTypeLabel) {
        this.devTypeLabel = devTypeLabel;
    }

    public String getCtrlTransGrpLabel() {
        return ctrlTransGrpLabel;
    }

    public void setCtrlTransGrpLabel(String ctrlTransGrpLabel) {
        this.ctrlTransGrpLabel = ctrlTransGrpLabel;
    }

    public String getPicName() {
        return picName;
    }

    public void setPicName(String picName) {
        this.picName = picName;
    }


    public Integer getAlarmLevel() {
        return alarmLevel;
    }

    public void setAlarmLevel(Integer alarmLevel) {
        this.alarmLevel = alarmLevel;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public Boolean getRestrain() {
        return restrain;
    }

    public void setRestrain(Boolean restrain) {
        this.restrain = restrain;
    }

    public Boolean getRecovery() {
        return recovery;
    }

    public void setRecovery(Boolean recovery) {
        this.recovery = recovery;
    }

    public Integer getDelOnAck() {
        return delOnAck;
    }

    public void setDelOnAck(Integer delOnAck) {
        this.delOnAck = delOnAck;
    }

    public Boolean getReplaceAlarm() {
        return replaceAlarm;
    }

    public void setReplaceAlarm(Boolean replaceAlarm) {
        this.replaceAlarm = replaceAlarm;
    }

    public Boolean getAcked() {
        return acked;
    }

    public void setAcked(Boolean acked) {
        this.acked = acked;
    }
}
