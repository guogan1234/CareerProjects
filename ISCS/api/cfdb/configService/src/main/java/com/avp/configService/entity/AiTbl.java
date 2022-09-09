package com.avp.configService.entity;

import javax.persistence.*;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
@Table(name = "ai_tbl", schema = "conf")
public class AiTbl {
    private String aiLabel;
    private String aiName;
    private Integer pointType;
    private String pointTpLabel;
    private String devLabel;
    private String devType;
    private Integer aiOrder;
    private Integer domainId;
    private Integer locationId;
    private Integer proSystemId;
    private Integer regionId;
    private Float aiValue;
    private Integer aiStatus;
    private Long updateTime;
    private Long changeTime;
    private Integer unitId;
    private Boolean isLimit;
    private Boolean isControl;
    private String ctrlTransGrpLabel;
    private Boolean isSample;
    private Float sampleDeadband;
    private String cameraTag;
    private String cameraPreset;

    @Id
    @Column(name = "ai_label", nullable = false, length = 128)
    public String getAiLabel() {
        return aiLabel;
    }

    public void setAiLabel(String aiLabel) {
        this.aiLabel = aiLabel;
    }

    @Basic
    @Column(name = "ai_name", nullable = true, length = 128)
    public String getAiName() {
        return aiName;
    }

    public void setAiName(String aiName) {
        this.aiName = aiName;
    }

    @Basic
    @Column(name = "point_type", nullable = true)
    public Integer getPointType() {
        return pointType;
    }

    public void setPointType(Integer pointType) {
        this.pointType = pointType;
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
    @Column(name = "dev_label", nullable = true, length = 96)
    public String getDevLabel() {
        return devLabel;
    }

    public void setDevLabel(String devLabel) {
        this.devLabel = devLabel;
    }

    @Basic
    @Column(name = "dev_type", nullable = true, length = 16)
    public String getDevType() {
        return devType;
    }

    public void setDevType(String devType) {
        this.devType = devType;
    }

    @Basic
    @Column(name = "ai_order", nullable = true)
    public Integer getAiOrder() {
        return aiOrder;
    }

    public void setAiOrder(Integer aiOrder) {
        this.aiOrder = aiOrder;
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
    @Column(name = "ai_value", nullable = true, precision = 0)
    public Float getAiValue() {
        return aiValue;
    }

    public void setAiValue(Float aiValue) {
        this.aiValue = aiValue;
    }

    @Basic
    @Column(name = "ai_status", nullable = true)
    public Integer getAiStatus() {
        return aiStatus;
    }

    public void setAiStatus(Integer aiStatus) {
        this.aiStatus = aiStatus;
    }

    @Basic
    @Column(name = "update_time", nullable = true)
    public Long getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Long updateTime) {
        this.updateTime = updateTime;
    }

    @Basic
    @Column(name = "change_time", nullable = true)
    public Long getChangeTime() {
        return changeTime;
    }

    public void setChangeTime(Long changeTime) {
        this.changeTime = changeTime;
    }

    @Basic
    @Column(name = "unit_id", nullable = true)
    public Integer getUnitId() {
        return unitId;
    }

    public void setUnitId(Integer unitId) {
        this.unitId = unitId;
    }

    @Basic
    @Column(name = "is_limit", nullable = true)
    public Boolean getIsLimit() {
        return isLimit;
    }

    public void setIsLimit(Boolean islimit) {
        this.isLimit = islimit;
    }

    @Basic
    @Column(name = "is_control", nullable = true)
    public Boolean getIsControl() {
        return isControl;
    }

    public void setIsControl(Boolean isControl) {
        this.isControl = isControl;
    }

    @Basic
    @Column(name = "ctrl_trans_grp_label", nullable = true, length = 32)
    public String getCtrlTransGrpLabel() {
        return ctrlTransGrpLabel;
    }

    public void setCtrlTransGrpLabel(String ctrlTransGrpLabel) {
        this.ctrlTransGrpLabel = ctrlTransGrpLabel;
    }

    @Basic
    @Column(name = "is_sample", nullable = true)
    public Boolean getIsSample() {
        return isSample;
    }

    public void setIsSample(Boolean isSample) {
        this.isSample = isSample;
    }

    @Basic
    @Column(name = "sample_deadband", nullable = true, precision = 0)
    public Float getSampleDeadband() {
        return sampleDeadband;
    }

    public void setSampleDeadband(Float sampleDeadband) {
        this.sampleDeadband = sampleDeadband;
    }

    @Basic
    @Column(name = "camera_tag", nullable = true, length = 64)
    public String getCameraTag() {
        return cameraTag;
    }

    public void setCameraTag(String cameraTag) {
        this.cameraTag = cameraTag;
    }

    @Basic
    @Column(name = "camera_preset", nullable = true, length = 64)
    public String getCameraPreset() {
        return cameraPreset;
    }

    public void setCameraPreset(String cameraPreset) {
        this.cameraPreset = cameraPreset;
    }
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AiTbl that = (AiTbl) o;
        return Objects.equals(aiLabel, that.aiLabel) &&
                Objects.equals(aiName, that.aiName) &&
                Objects.equals(pointType, that.pointType) &&
                Objects.equals(pointTpLabel, that.pointTpLabel) &&
                Objects.equals(devLabel, that.devLabel) &&
                Objects.equals(devType, that.devType) &&
                Objects.equals(aiOrder, that.aiOrder) &&
                Objects.equals(domainId, that.domainId) &&
                Objects.equals(locationId, that.locationId) &&
                Objects.equals(proSystemId, that.proSystemId) &&
                Objects.equals(regionId, that.regionId) &&
                Objects.equals(aiValue, that.aiValue) &&
                Objects.equals(aiStatus, that.aiStatus) &&
                Objects.equals(updateTime, that.updateTime) &&
                Objects.equals(changeTime, that.changeTime) &&
                Objects.equals(unitId, that.unitId) &&
                Objects.equals(isLimit, that.isLimit) &&
                Objects.equals(isControl, that.isControl) &&
                Objects.equals(ctrlTransGrpLabel, that.ctrlTransGrpLabel) &&
                Objects.equals(isSample, that.isSample) &&
                Objects.equals(sampleDeadband, that.sampleDeadband) &&
                Objects.equals(cameraTag, that.cameraTag) &&
                Objects.equals(cameraPreset, that.cameraPreset);
    }

    @Override
    public int hashCode() {

        return Objects.hash(aiLabel, aiName, pointType, pointTpLabel, devLabel, devType, aiOrder, domainId, locationId, proSystemId, regionId, aiValue, aiStatus, updateTime, changeTime, unitId, isLimit, isControl, ctrlTransGrpLabel, isSample, sampleDeadband, cameraTag, cameraPreset);
    }
}
