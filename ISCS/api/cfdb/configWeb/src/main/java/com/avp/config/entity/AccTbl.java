package com.avp.config.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * 累积量点表
 *
 */
@Entity
@Table(name = "acc_tbl",schema = "conf")
public class AccTbl {
	//标签
	@Id
	@Column(name = "acc_label",length = 128)
	private String accLabel;

	//名称
	@Column(name = "acc_name",length = 128)
	private String accName;

	//顺序号
	@Column(name = "acc_order")
	private Integer accOrder;

	//点模板标签
	@Column(name = "point_tp_label",length = 32)
	private String pointTpLabel;

	//所属设备标签
	@Column(name = "dev_label",length = 96)
	private String devLabel;

	//设备类型标签
	@Column(name = "dev_type_label",length = 16)
	private String devTypeLabel;

	//所属域
	@Column(name = "domain_id")
	private Integer domainId;

	//所属位置
	@Column(name = "location_id")
	private Integer locationId;

	//所属子系统
	@Column(name = "pro_system_id")
	private Integer proSystemId;

	//所属责任区
	@Column(name = "region_id")
	private Integer regionId;

	//状态
	@Column(name = "acc_status")
	private Integer accStatus;

	//值
	@Column(name = "acc_value")
	private Integer accValue;

	//更新时间
	@Column(name = "update_time")
	private Long updateTime;

	//变化时间
	@Column(name = "change_time")
	private Long changeTime;

	//单位
	@Column(name = "unit_id")
	private Integer unitId;

	//是否限值
	@Column(name = "is_limit")
	private Boolean isLimit;

	//是否采样
	@Column(name = "is_sample")
	private Boolean isSample;

	//采样死区
	@Column(name = "sample_deadband")
	private Integer sampleDeadband;

	//关联摄像机
	@Column(name = "camera_tag",length = 64)
	private String cameraTag;

	//摄像机预置位
	@Column(name = "camera_preset",length = 64)
	private String cameraPreset;

	@Column(name = "operate_time")
	private Timestamp operateTime;

	public String getAccLabel() {
		return accLabel;
	}

	public void setAccLabel(String accLabel) {
		this.accLabel = accLabel;
	}

	public String getAccName() {
		return accName;
	}

	public void setAccName(String accName) {
		this.accName = accName;
	}

	public Integer getAccOrder() {
		return accOrder;
	}

	public void setAccOrder(Integer accOrder) {
		this.accOrder = accOrder;
	}
	
	public String getPointTpLabel() {
		return pointTpLabel;
	}

	public void setPointTpLabel(String pointTpLabel) {
		this.pointTpLabel = pointTpLabel;
	}

	public String getDevLabel() {
		return devLabel;
	}

	public void setDevLabel(String devLabel) {
		this.devLabel = devLabel;
	}

	public String getDevTypeLabel() {
		return devTypeLabel;
	}

	public void setDevTypeLabel(String devTypeLabel) {
		this.devTypeLabel = devTypeLabel;
	}

	public Integer getDomainId() {
		return domainId;
	}

	public void setDomainId(Integer domainId) {
		this.domainId = domainId;
	}

	public Integer getLocationId() {
		return locationId;
	}

	public void setLocationId(Integer locationId) {
		this.locationId = locationId;
	}

	public Integer getProSystemId() {
		return proSystemId;
	}

	public void setProSystemId(Integer proSystemId) {
		this.proSystemId = proSystemId;
	}

	public Integer getRegionId() {
		return regionId;
	}

	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
	}

	public Integer getAccStatus() {
		return accStatus;
	}

	public void setAccStatus(Integer accStatus) {
		this.accStatus = accStatus;
	}

	public Integer getAccValue() {
		return accValue;
	}

	public void setAccValue(Integer accValue) {
		this.accValue = accValue;
	}

	public Long getUpdateTime() {
		return updateTime;
	}

	public void setUpdateTime(Long updateTime) {
		this.updateTime = updateTime;
	}

	public Long getChangeTime() {
		return changeTime;
	}

	public void setChangeTime(Long changeTime) {
		this.changeTime = changeTime;
	}

	public Integer getUnitId() {
		return unitId;
	}

	public void setUnitId(Integer unitId) {
		this.unitId = unitId;
	}

	public Boolean getIsLimit() {
		return isLimit;
	}

	public void setIsLimit(Boolean isLimit) {
		this.isLimit = isLimit;
	}

	public Boolean getIsSample() {
		return isSample;
	}

	public void setIsSample(Boolean isSample) {
		this.isSample = isSample;
	}

	public Integer getSampleDeadband() {
		return sampleDeadband;
	}

	public void setSampleDeadband(Integer sampleDeadband) {
		this.sampleDeadband = sampleDeadband;
	}

	public String getCameraTag() {
		return cameraTag;
	}

	public void setCameraTag(String cameraTag) {
		this.cameraTag = cameraTag;
	}

	public String getCameraPreset() {
		return cameraPreset;
	}

	public void setCameraPreset(String cameraPreset) {
		this.cameraPreset = cameraPreset;
	}

	public Timestamp getOperateTime() {
		return operateTime;
	}

	public void setOperateTime(Timestamp operateTime) {
		this.operateTime = operateTime;
	}
}

