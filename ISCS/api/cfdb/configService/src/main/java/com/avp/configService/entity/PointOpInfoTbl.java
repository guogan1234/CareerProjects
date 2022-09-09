package com.avp.configService.entity;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "point_op_info_tbl", schema = "conf")
@IdClass(PointOpInfoTblPK.class)
public class PointOpInfoTbl {
    private String pointLabel;
    private Integer opType;
    private Integer opTime;
    private Float forceValue;
    private String displayText;
    private String hostName;
    private Integer userId;
    private Integer userGrpId;
    private String description;
    private Integer stationId;
    private Integer domainId;

    @Id
    @Column(name = "point_label", nullable = false, length = 128)
    public String getPointLabel() {
        return pointLabel;
    }

    public void setPointLabel(String pointLabel) {
        this.pointLabel = pointLabel;
    }

    @Id
    @Column(name = "op_type", nullable = false)
    public int getOpType() {
        return opType;
    }

    public void setOpType(int opType) {
        this.opType = opType;
    }

    @Basic
    @Column(name = "op_time", nullable = true)
    public Integer getOpTime() {
        return opTime;
    }

    public void setOpTime(Integer opTime) {
        this.opTime = opTime;
    }

    @Basic
    @Column(name = "force_value", nullable = true, precision = 0)
    public Float getForceValue() {
        return forceValue;
    }

    public void setForceValue(Float forceValue) {
        this.forceValue = forceValue;
    }

    @Basic
    @Column(name = "display_text", nullable = true, length = 32)
    public String getDisplayText() {
        return displayText;
    }

    public void setDisplayText(String displayText) {
        this.displayText = displayText;
    }

    @Basic
    @Column(name = "host_name", nullable = true, length = 64)
    public String getHostName() {
        return hostName;
    }

    public void setHostName(String hostName) {
        this.hostName = hostName;
    }

    @Basic
    @Column(name = "user_id", nullable = true)
    public Integer getUserId() {
        return userId;
    }

    public void setUserId(Integer userId) {
        this.userId = userId;
    }

    @Basic
    @Column(name = "user_grp_id", nullable = true)
    public Integer getUserGrpId() {
        return userGrpId;
    }

    public void setUserGrpId(Integer userGrpId) {
        this.userGrpId = userGrpId;
    }

    @Basic
    @Column(name = "description", nullable = true, length = 128)
    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    @Basic
    @Column(name = "station_id", nullable = true)
    public Integer getStationId() {
        return stationId;
    }

    public void setStationId(Integer stationId) {
        this.stationId = stationId;
    }

    @Basic
    @Column(name = "domain_id", nullable = true)
    public Integer getDomainId() {
        return domainId;
    }

    public void setDomainId(Integer domainId) {
        this.domainId = domainId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        PointOpInfoTbl that = (PointOpInfoTbl) o;
        return opType == that.opType &&
                Objects.equals(pointLabel, that.pointLabel) &&
                Objects.equals(opTime, that.opTime) &&
                Objects.equals(forceValue, that.forceValue) &&
                Objects.equals(displayText, that.displayText) &&
                Objects.equals(hostName, that.hostName) &&
                Objects.equals(userId, that.userId) &&
                Objects.equals(userGrpId, that.userGrpId) &&
                Objects.equals(description, that.description) &&
                Objects.equals(stationId, that.stationId) &&
                Objects.equals(domainId, that.domainId);
    }

    @Override
    public int hashCode() {

        return Objects.hash(pointLabel, opType, opTime, forceValue, displayText, hostName, userId, userGrpId, description, stationId, domainId);
    }
}
