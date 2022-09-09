package com.avp.config.entity.system;

import com.fasterxml.jackson.annotation.JsonIgnore;
import org.hibernate.annotations.NotFound;
import org.hibernate.annotations.NotFoundAction;

import javax.persistence.*;

@Entity
@Table(name = "station_tbl",schema = "conf")
public class StationTbl {
	//车站ID
	@Id
	@Column(name = "station_id")
	private Integer stationId;

	//标签
	@Column(name = "station_label",length = 16)
	private String stationLabel;

	//车站名称
	@Column(name = "station_name",length = 32)
	private String stationName;

	//所属车站ID
	@Column(name = "station_pid")
	private Integer stationPid;

	private Integer domainId;

	@ManyToOne(fetch = FetchType.LAZY)
	@JoinColumn(name = "domainId", insertable = false, updatable = false)
	@JsonIgnore
	@NotFound(action = NotFoundAction.IGNORE)
	private DomainTbl domainTbl;

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

	public DomainTbl getDomainTbl() {
		return domainTbl;
	}

	public void setDomainTbl(DomainTbl domainTbl) {
		this.domainTbl = domainTbl;
	}
}