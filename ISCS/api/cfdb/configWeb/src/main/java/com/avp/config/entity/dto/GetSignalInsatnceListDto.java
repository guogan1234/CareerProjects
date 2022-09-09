package com.avp.config.entity.dto;

public class GetSignalInsatnceListDto {
	/*
	 * 信号实例主键
	 */
	private String signaId;
	/*
	 * 信号实例名称
	 */
	private String signaName;
	/*
	 * 信号实例类别(1:DIO,2:AIO:3.ACC:4:MIX)
	 */
	private Integer signaType;

	private String devInstanceId;

	/*
	 * 是否是特殊实例（0：否，1：是）
	 */
	private Integer ifSpecail;
	public String getSignaId() {
		return signaId;
	}
	public void setSignaId(String signaId) {
		this.signaId = signaId;
	}
	public String getSignaName() {
		return signaName;
	}
	public void setSignaName(String signaName) {
		this.signaName = signaName;
	}
	public Integer getSignaType() {
		return signaType;
	}
	public void setSignaType(Integer signaType) {
		this.signaType = signaType;
	}
	public Integer getIfSpecail() {
		return ifSpecail;
	}
	public void setIfSpecail(Integer ifSpecail) {
		this.ifSpecail = ifSpecail;
	}

	public String getDevInstanceId() {
		return devInstanceId;
	}

	public void setDevInstanceId(String devInstanceId) {
		this.devInstanceId = devInstanceId;
	}
}
