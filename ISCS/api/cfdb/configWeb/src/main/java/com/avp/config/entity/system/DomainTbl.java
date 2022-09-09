package com.avp.config.entity.system;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "domain_tbl",schema = "conf")
public class DomainTbl {
	//域ID
	@Id
	@Column(name = "domain_id")
	private Integer domainId;

	//标签
	@Column(name = "domain_label",length = 16)
	private String domainLabel;

	//域名称
	@Column(name = "domain_name",length = 32)
	private String domainName;

	//域类型
	@Column(name = "domain_type")
	private Integer domainType;

	public Integer getDomainId() {
		return domainId;
	}

	public void setDomainId(Integer domainId) {
		this.domainId = domainId;
	}

	public String getDomainLabel() {
		return domainLabel;
	}

	public void setDomainLabel(String domainLabel) {
		this.domainLabel = domainLabel;
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

}
