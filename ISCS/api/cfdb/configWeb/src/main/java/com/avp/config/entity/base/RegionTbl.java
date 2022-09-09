package com.avp.config.entity.base;

import com.avp.config.entity.system.ProSystemTbl;
import com.fasterxml.jackson.annotation.JsonIgnore;
import org.hibernate.annotations.NotFound;
import org.hibernate.annotations.NotFoundAction;

import javax.persistence.*;

@Entity
@Table(name = "region_tbl",schema = "conf")
public class RegionTbl {
	//责任区ID
	@Id
	@Column(name = "region_id")
	private Integer regionId;

	//标签
	@Column(name = "region_label",length = 16)
	private String regionLabel;

	//责任区名称
	@Column(name = "region_name",length = 32)
	private String regionName;

	//所属专业子系统
	private Integer proSystemId;

	@ManyToOne(fetch = FetchType.LAZY)
	@JoinColumn(name = "proSystemId", insertable = false, updatable = false)
	@JsonIgnore
	@NotFound(action = NotFoundAction.IGNORE)
	private ProSystemTbl proSystemTbl;


	public Integer getRegionId() {
		return regionId;
	}

	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
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

	public Integer getProSystemId() {
		return proSystemId;
	}

	public void setProSystemId(Integer proSystemId) {
		this.proSystemId = proSystemId;
	}

	public ProSystemTbl getProSystemTbl() {
		return proSystemTbl;
	}

	public void setProSystemTbl(ProSystemTbl proSystemTbl) {
		this.proSystemTbl = proSystemTbl;
	}
}
