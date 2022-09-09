package com.avp.config.entity.dto;

import com.avp.config.entity.DevTbl;

public class AddDevDto extends DevTbl{
	private String stationLabel;

	public String getStationLabel() {
		return stationLabel;
	}

	public void setStationLabel(String stationLabel) {
		this.stationLabel = stationLabel;
	}
}
