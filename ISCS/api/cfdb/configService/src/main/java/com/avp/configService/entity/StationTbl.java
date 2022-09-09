package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

public class StationTbl {
	//车站ID

	private Integer stationId;

	//标签
	private String stationLabel;

	//车站名称
	private String stationName;

	//所属车站ID
	private Integer stationPid;

	private Integer domainId;

	public Integer getStationId() {
		return stationId;
	}

	public void setStationId(Integer stationId) {
		this.stationId = stationId;
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

	public Integer getStationPid() {
		return stationPid;
	}

	public void setStationPid(Integer stationPid) {
		this.stationPid = stationPid;
	}

	public Integer getDomainId() {
		return domainId;
	}

	public void setDomainId(Integer domainId) {
		this.domainId = domainId;
	}
}