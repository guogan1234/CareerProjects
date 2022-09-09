package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

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
	@Column(name = "pro_system_id")
	private Integer proSystemId;

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

}
