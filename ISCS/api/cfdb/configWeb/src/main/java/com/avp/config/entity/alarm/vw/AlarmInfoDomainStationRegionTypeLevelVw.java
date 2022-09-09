package com.avp.config.entity.alarm.vw;

import com.avp.config.entity.alarm.AlarmInfoMultiKeys;

import javax.persistence.*;
import java.io.Serializable;

/**
 * Created by zhoujs on 2018/6/21.
 */
@Entity
@Table(name = "alarm_info_domain_station_region_type_level_vw", schema = "conf")
@IdClass(AlarmInfoMultiKeys.class)
public class AlarmInfoDomainStationRegionTypeLevelVw implements Serializable {

    private static final long serialVersionUID = -3550293856369130792L;

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
    @Basic
    @Column(name = "type_name")
    private String typeName;
    @Basic
    @Column(name = "domain_name")
    private String domainName;
    @Basic
    @Column(name = "domain_type")
    private Integer domainType;
    @Basic
    @Column(name = "station_label")
    private String stationLabel;
    @Basic
    @Column(name = "station_name")
    private String stationName;
    @Basic
    @Column(name = "pro_system_id")
    private Integer proSystemId;
    @Basic
    @Column(name = "region_label")
    private String regionLabel;
    @Basic
    @Column(name = "region_name")
    private String regionName;
    @Basic
    @Column(name = "level_name")
    private String levelName;
    @Basic
    @Column(name = "priority")
    private Integer priority;
    @Basic
    @Column(name = "sound_file")
    private String soundFile;
    @Basic
    @Column(name = "alarm_act")
    private String alarmAct;

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

    public String getTypeName() {
        return typeName;
    }

    public void setTypeName(String typeName) {
        this.typeName = typeName;
    }

    public String getDomainName() {
        return domainName;
    }

    public void setDomainName(String domainName) {
        this.domainName = domainName;
    }

    public Integer getDomainType() {
        return domainType;
    }

    public void setDomainType(Integer domainType) {
        this.domainType = domainType;
    }

    public String getStationLabel() {
        return stationLabel;
    }

    public void setStationLabel(String stationLabel) {
        this.stationLabel = stationLabel;
    }

    public String getStationName() {
        return stationName;
    }

    public void setStationName(String stationName) {
        this.stationName = stationName;
    }

    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }

    public String getRegionLabel() {
        return regionLabel;
    }

    public void setRegionLabel(String regionLabel) {
        this.regionLabel = regionLabel;
    }

    public String getRegionName() {
        return regionName;
    }

    public void setRegionName(String regionName) {
        this.regionName = regionName;
    }

    public String getLevelName() {
        return levelName;
    }

    public void setLevelName(String levelName) {
        this.levelName = levelName;
    }

    public Integer getPriority() {
        return priority;
    }

    public void setPriority(Integer priority) {
        this.priority = priority;
    }

    public String getSoundFile() {
        return soundFile;
    }

    public void setSoundFile(String soundFile) {
        this.soundFile = soundFile;
    }

    public String getAlarmAct() {
        return alarmAct;
    }

    public void setAlarmAct(String alarmAct) {
        this.alarmAct = alarmAct;
    }
}
