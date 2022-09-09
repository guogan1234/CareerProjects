package com.avp.configService.entity;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

public class DevTbl {
	//标签
	private String devLabel;

	//名称
	private String devName;

	//所属域
	private Integer domainId;

	//所属位置
	private Integer stationId;

	//所属子系统
	private Integer proSystemId;

	//所属责任区
	private Integer regionId;

	//所属RTU
	private Integer rtuId;

	//设备类型
	private String devTypeLabel;

	//所属模板
	private String devTpLabel;

	//控制权转移组
	private String ctrlTransGrpLabel;

	//状态
	private Integer devStatus;

	//是否摘要
	private Integer isSummary;

	//摘要值
	private Integer summaryValue;

	//摘要状态
	private Integer summaryStatus;


	public String getDevLabel() {
		return devLabel;
	}

	public void setDevLabel(String devLabel) {
		this.devLabel = devLabel;
	}

	public String getDevName() {
		return devName;
	}

	public void setDevName(String devName) {
		this.devName = devName;
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

	public Integer getRtuId() {
		return rtuId;
	}

	public void setRtuId(Integer rtuId) {
		this.rtuId = rtuId;
	}

	public String getDevTypeLabel() {
		return devTypeLabel;
	}

	public void setDevTypeLabel(String devTypeLabel) {
		this.devTypeLabel = devTypeLabel;
	}

	public String getDevTpLabel() {
		return devTpLabel;
	}

	public void setDevTpLabel(String devTpLabel) {
		this.devTpLabel = devTpLabel;
	}

	public String getCtrlTransGrpLabel() {
		return ctrlTransGrpLabel;
	}

	public void setCtrlTransGrpLabel(String ctrlTransGrpLabel) {
		this.ctrlTransGrpLabel = ctrlTransGrpLabel;
	}

	public Integer getDevStatus() {
		return devStatus;
	}

	public void setDevStatus(Integer devStatus) {
		this.devStatus = devStatus;
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
}

