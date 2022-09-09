package com.avp.config.entity.vo;

public class SignalInstanceVo {
	/*
	 * 操作的点类型(1:Dio,2:AIO,3:ACC,4:MIX)
	 */
	private Integer signalType;
	/*
	 * 信号标签
	 */
	private String signalLabel;

	public Integer getSignalType() {
		return signalType;
	}

	public void setSignalType(Integer signalType) {
		this.signalType = signalType;
	}

	public String getSignalLabel() {
		return signalLabel;
	}
	public void setSignalLabel(String signalLabel) {
		this.signalLabel = signalLabel;
	}
}
