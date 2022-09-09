package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * 离散输入点表输出点表
 *
 */
@Entity
@Table(name = "di_tbl",schema = "conf")
public class DiTbl {
	//标签
	@Id
	@Column(name = "di_label",length = 128)
	private String diLabel;

	//名称
	@Column(name = "di_name",length = 128)
	private String diName;

	//点类型
	@Column(name = "point_type")
	private Integer pointType;
	
	//是否控制
	@Column(name = "is_control")
	private Boolean isControl;
	
	//点模板标签
	@Column(name = "point_tp_label",length = 32)
	private String pointTpLabel;

	//所属设备标签
	@Column(name = "dev_label",length = 96)
	private String devLabel;

	//所属设备类型
	@Column(name = "dev_type_Label")
	private String devTypeLabel;

	//顺序号
	@Column(name = "di_order")
	private Integer diOrder;

	//所属域
	@Column(name = "domain_id")
	private Integer domainId;

	//所属位置
	@Column(name = "location_id")
	private Integer locationId;

	//所属专业系统
	@Column(name = "pro_system_id")
	private Integer proSystemId;

	//所属责任区
	@Column(name = "region_id")
	private Integer regionId;

	//数字量文本标签
	@Column(name = "di_text_label",length = 32)
	private String diTextLabel;

	//值分量数
	@Column(name = "bit_num")
	private Integer bitNum;

	//分量延迟时间
	@Column(name = "bit_delay_time")
	private Integer bitDelayTime;

	//值
	@Column(name = "di_value")
	private Integer diValue;

	//状态
	@Column(name = "di_status")
	private Integer diStatus;

	//更新时间
	@Column(name = "update_time")
	private Long updateTime;

	//值1
	@Column(name = "di_value1")
	private Integer diValue1;

	//值2
	@Column(name = "di_value2")
	private Integer diValue2;

	//值3
	@Column(name = "di_value3")
	private Integer diValue3;

	//值4
	@Column(name = "di_value4")
	private Integer diValue4;

	//值5
	@Column(name = "di_value5")
	private Integer diValue5;

	//状态1
	@Column(name = "di_status1")
	private Integer diStatus1;

	//状态2
	@Column(name = "di_status2")
	private Integer diStatus2;

	//状态3
	@Column(name = "di_status3")
	private Integer diStatus3;

	//状态4
	@Column(name = "di_status4")
	private Integer diStatus4;

	//状态5
	@Column(name = "di_status5")
	private Integer diStatus5;

	//变化时间1
	@Column(name = "change_time1")
	private Long changeTime1;

	//变化时间2
	@Column(name = "change_time2")
	private Long changeTime2;

	//变化时间3
	@Column(name = "change_time3")
	private Long changeTime3;

	//变化时间4
	@Column(name = "change_time4")
	private Long changeTime4;

	//变化时间5
	@Column(name = "change_time5")
	private Long changeTime5;

	//报警等级
	@Column(name = "alarm_level")
	private Integer alarmLevel;

	//关联画面名
	@Column(name = "pic_name",length = 64)
	private String picName;

	//报警延迟时间
	@Column(name = "alarm_delay_time")
	private Integer alarmDelayTime;

	//是否替换报警
	@Column(name = "is_replace_alarm")
	private Boolean isReplaceAlarm;

	//报警确认后是否删除
	@Column(name = "is_del_on_ack")
	private Integer isDelOnAck;

	//控制权转移组标签
	@Column(name = "ctrl_trans_grp_label",length = 32)
	private String ctrlTransGrpLabel;

	//是否采样
	@Column(name = "is_sample")
	private Boolean isSample;

	//是否摘要
	@Column(name = "is_summary")
	private Integer isSummary;

	//摘要值
	@Column(name = "summary_value")
	private Integer summaryValue;

	//摘要状态
	@Column(name = "summary_status")
	private Integer summaryStatus;

	//关联摄像机
	@Column(name = "camera_tag",length = 64)
	private String cameraTag;

	//摄像机预置位
	@Column(name = "camera_preset",length = 64)
	private String cameraPreset;


	public Boolean getIsControl() {
		return isControl;
	}

	public void setIsControl(Boolean isControl) {
		this.isControl = isControl;
	}

	public Boolean getIsReplaceAlarm() {
		return isReplaceAlarm;
	}

	public void setIsReplaceAlarm(Boolean isReplaceAlarm) {
		this.isReplaceAlarm = isReplaceAlarm;
	}

	public Boolean getIsSample() {
		return isSample;
	}

	public void setIsSample(Boolean isSample) {
		this.isSample = isSample;
	}

	public String getDiLabel() {
		return diLabel;
	}

	public void setDiLabel(String diLabel) {
		this.diLabel = diLabel;
	}

	public String getDiName() {
		return diName;
	}

	public void setDiName(String diName) {
		this.diName = diName;
	}
	
	public String getDevTypeLabel() {
		return devTypeLabel;
	}

	public void setDevTypeLabel(String devTypeLabel) {
		this.devTypeLabel = devTypeLabel;
	}

	public Integer getPointType() {
		return pointType;
	}

	public void setPointType(Integer pointType) {
		this.pointType = pointType;
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

	public Integer getDiOrder() {
		return diOrder;
	}

	public void setDiOrder(Integer diOrder) {
		this.diOrder = diOrder;
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

	public String getDiTextLabel() {
		return diTextLabel;
	}

	public void setDiTextLabel(String diTextLabel) {
		this.diTextLabel = diTextLabel;
	}

	public Integer getBitNum() {
		return bitNum;
	}

	public void setBitNum(Integer bitNum) {
		this.bitNum = bitNum;
	}

	public Integer getBitDelayTime() {
		return bitDelayTime;
	}

	public void setBitDelayTime(Integer bitDelayTime) {
		this.bitDelayTime = bitDelayTime;
	}

	public Integer getDiValue() {
		return diValue;
	}

	public void setDiValue(Integer diValue) {
		this.diValue = diValue;
	}

	public Integer getDiStatus() {
		return diStatus;
	}

	public void setDiStatus(Integer diStatus) {
		this.diStatus = diStatus;
	}

	public Long getUpdateTime() {
		return updateTime;
	}

	public void setUpdateTime(Long updateTime) {
		this.updateTime = updateTime;
	}

	public Integer getDiValue1() {
		return diValue1;
	}

	public void setDiValue1(Integer diValue1) {
		this.diValue1 = diValue1;
	}

	public Integer getDiValue2() {
		return diValue2;
	}

	public void setDiValue2(Integer diValue2) {
		this.diValue2 = diValue2;
	}

	public Integer getDiValue3() {
		return diValue3;
	}

	public void setDiValue3(Integer diValue3) {
		this.diValue3 = diValue3;
	}

	public Integer getDiValue4() {
		return diValue4;
	}

	public void setDiValue4(Integer diValue4) {
		this.diValue4 = diValue4;
	}

	public Integer getDiValue5() {
		return diValue5;
	}

	public void setDiValue5(Integer diValue5) {
		this.diValue5 = diValue5;
	}

	public Integer getDiStatus1() {
		return diStatus1;
	}

	public void setDiStatus1(Integer diStatus1) {
		this.diStatus1 = diStatus1;
	}

	public Integer getDiStatus2() {
		return diStatus2;
	}

	public void setDiStatus2(Integer diStatus2) {
		this.diStatus2 = diStatus2;
	}

	public Integer getDiStatus3() {
		return diStatus3;
	}

	public void setDiStatus3(Integer diStatus3) {
		this.diStatus3 = diStatus3;
	}

	public Integer getDiStatus4() {
		return diStatus4;
	}

	public void setDiStatus4(Integer diStatus4) {
		this.diStatus4 = diStatus4;
	}

	public Integer getDiStatus5() {
		return diStatus5;
	}

	public void setDiStatus5(Integer diStatus5) {
		this.diStatus5 = diStatus5;
	}

	public Long getChangeTime1() {
		return changeTime1;
	}

	public void setChangeTime1(Long changeTime1) {
		this.changeTime1 = changeTime1;
	}

	public Long getChangeTime2() {
		return changeTime2;
	}

	public void setChangeTime2(Long changeTime2) {
		this.changeTime2 = changeTime2;
	}

	public Long getChangeTime3() {
		return changeTime3;
	}

	public void setChangeTime3(Long changeTime3) {
		this.changeTime3 = changeTime3;
	}

	public Long getChangeTime4() {
		return changeTime4;
	}

	public void setChangeTime4(Long changeTime4) {
		this.changeTime4 = changeTime4;
	}

	public Long getChangeTime5() {
		return changeTime5;
	}

	public void setChangeTime5(Long changeTime5) {
		this.changeTime5 = changeTime5;
	}

	public Integer getAlarmLevel() {
		return alarmLevel;
	}

	public void setAlarmLevel(Integer alarmLevel) {
		this.alarmLevel = alarmLevel;
	}

	public String getPicName() {
		return picName;
	}

	public void setPicName(String picName) {
		this.picName = picName;
	}

	public Integer getAlarmDelayTime() {
		return alarmDelayTime;
	}

	public void setAlarmDelayTime(Integer alarmDelayTime) {
		this.alarmDelayTime = alarmDelayTime;
	}

	public Boolean isReplaceAlarm() {
		return isReplaceAlarm;
	}

	public void setReplaceAlarm(Boolean isReplaceAlarm) {
		this.isReplaceAlarm = isReplaceAlarm;
	}

	public Integer getIsDelOnAck() {
		return isDelOnAck;
	}

	public void setIsDelOnAck(Integer isDelOnAck) {
		this.isDelOnAck = isDelOnAck;
	}

	public String getCtrlTransGrpLabel() {
		return ctrlTransGrpLabel;
	}

	public void setCtrlTransGrpLabel(String ctrlTransGrpLabel) {
		this.ctrlTransGrpLabel = ctrlTransGrpLabel;
	}

	public Boolean isSample() {
		return isSample;
	}

	public void setSample(Boolean isSample) {
		this.isSample = isSample;
	}

	public Integer getIsSummary() {
		return isSummary;
	}

	public void setIsSummary(Integer isSummary) {
		this.isSummary = isSummary;
	}

	public Integer getSummaryValue() {
		return summaryValue;
	}

	public void setSummaryValue(Integer summaryValue) {
		this.summaryValue = summaryValue;
	}

	public Integer getSummaryStatus() {
		return summaryStatus;
	}

	public void setSummaryStatus(Integer summaryStatus) {
		this.summaryStatus = summaryStatus;
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
}

