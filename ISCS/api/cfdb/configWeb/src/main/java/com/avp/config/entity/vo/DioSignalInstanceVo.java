package com.avp.config.entity.vo;

public class DioSignalInstanceVo {
	private String dioTpLabel;
	private String dioTpName;
	private String devLabel;
	//di - do - dio-def 表需要
	private String dioLabel;
	//---------------------- 往下与DI表名称相符--------------------
	//diName
	private String dioName;
	private Integer diOrder;
	private Integer pointType;
	private String diTextLabel;
	//dio_def表也需要用到
	private Integer bitNum;
	private Integer bitDelayTime;
	private Integer alarmLevel;
	private Integer alarmDelayTime;
	private Boolean isReplaceAlarm;
	private Integer isDelOnAck;
	private Boolean isControl;
	private Integer isSummary;
	private Boolean isSample;
	private String picName;
	private String cameraTag;
	private String cameraPreset;
	
	//---------------------- 往下与do表名称相符-------------------
	//doGrpLabel
	private String ctrlGrpLabel;
	//waitAct
	private Integer ctrlWaitAct;
	//execTimeout
	private Integer ctrlTimeout;
	private Integer resvTime;
	private Integer pulseDuration;
	private Integer doNum;
	private String formulaLabel;
	private Integer doNo1;
	private Integer doNo2;
	private Integer doNo3;
	private Integer doNo4;
	private Integer doNo5;
	
	//---------------------- 往下与def表名称相符-------------------
	private Integer polarity1;
	//dioPolarity2
	private Integer polarity2;
	//dioPolarity2
	private Integer polarity3;
	//dioPolarity2
	private Integer polarity4;
	private Boolean filterErr1;
	private Boolean filterErr2;
	private Boolean filterErr3;
	private Boolean filterErr4;
	private Boolean filterDisturb1;
	private Boolean filterDisturb2;
	private Boolean filterDisturb3;
	private Boolean filterDisturb4;
	private Integer disturbTime1;
	private Integer disturbTime2;
	private Integer disturbTime3;
	private Integer disturbTime4;
	private String channelTag1;
	private String channelTag2;
	private String channelTag3;
	private String channelTag4;
    private Integer dot1No1;
    private Integer dot1No2;
    private Integer dot1No3;
    private Integer dot1No4;
    private Integer dot1No5;
    private Integer dot2No1;
    private Integer dot2No2;
    private Integer dot2No3;
    private Integer dot2No4;
    private Integer dot2No5;
    private Integer dot3No1;
    private Integer dot3No2;
    private Integer dot3No3;
    private Integer dot3No4;
    private Integer dot3No5;
    private Integer dot4No1;
    private Integer dot4No2;
    private Integer dot4No3;
    private Integer dot4No4;
    private Integer dot4No5;

	private String pointTpLabel;
    
	public String getDioTpLabel() {
		return dioTpLabel;
	}
	public void setDioTpLabel(String dioTpLabel) {
		this.dioTpLabel = dioTpLabel;
	}
	public String getDioTpName() {
		return dioTpName;
	}
	public void setDioTpName(String dioTpName) {
		this.dioTpName = dioTpName;
	}
	public String getDevLabel() {
		return devLabel;
	}
	public void setDevLabel(String devLabel) {
		this.devLabel = devLabel;
	}
	public String getDioLabel() {
		return dioLabel;
	}
	public void setDioLabel(String dioLabel) {
		this.dioLabel = dioLabel;
	}
	public String getDioName() {
		return dioName;
	}
	public void setDioName(String dioName) {
		this.dioName = dioName;
	}
	public Integer getDiOrder() {
		return diOrder;
	}
	public void setDiOrder(Integer diOrder) {
		this.diOrder = diOrder;
	}
	public Integer getPointType() {
		return pointType;
	}
	public void setPointType(Integer pointType) {
		this.pointType = pointType;
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
	public Integer getAlarmLevel() {
		return alarmLevel;
	}
	public void setAlarmLevel(Integer alarmLevel) {
		this.alarmLevel = alarmLevel;
	}
	public Integer getAlarmDelayTime() {
		return alarmDelayTime;
	}
	public void setAlarmDelayTime(Integer alarmDelayTime) {
		this.alarmDelayTime = alarmDelayTime;
	}
	public Boolean getIsReplaceAlarm() {
		return isReplaceAlarm;
	}
	public void setIsReplaceAlarm(Boolean isReplaceAlarm) {
		this.isReplaceAlarm = isReplaceAlarm;
	}
	public Integer getIsDelOnAck() {
		return isDelOnAck;
	}
	public void setIsDelOnAck(Integer isDelOnAck) {
		this.isDelOnAck = isDelOnAck;
	}
	public Boolean getIsControl() {
		return isControl;
	}
	public void setIsControl(Boolean isControl) {
		this.isControl = isControl;
	}
	public Integer getIsSummary() {
		return isSummary;
	}
	public void setIsSummary(Integer isSummary) {
		this.isSummary = isSummary;
	}
	public Boolean getIsSample() {
		return isSample;
	}
	public void setIsSample(Boolean isSample) {
		this.isSample = isSample;
	}
	public String getPicName() {
		return picName;
	}
	public void setPicName(String picName) {
		this.picName = picName;
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
	public String getCtrlGrpLabel() {
		return ctrlGrpLabel;
	}
	public void setCtrlGrpLabel(String ctrlGrpLabel) {
		this.ctrlGrpLabel = ctrlGrpLabel;
	}
	public Integer getCtrlWaitAct() {
		return ctrlWaitAct;
	}
	public void setCtrlWaitAct(Integer ctrlWaitAct) {
		this.ctrlWaitAct = ctrlWaitAct;
	}
	public Integer getCtrlTimeout() {
		return ctrlTimeout;
	}
	public void setCtrlTimeout(Integer ctrlTimeout) {
		this.ctrlTimeout = ctrlTimeout;
	}
	public Integer getResvTime() {
		return resvTime;
	}
	public void setResvTime(Integer resvTime) {
		this.resvTime = resvTime;
	}
	public Integer getPulseDuration() {
		return pulseDuration;
	}
	public void setPulseDuration(Integer pulseDuration) {
		this.pulseDuration = pulseDuration;
	}
	public Integer getDoNum() {
		return doNum;
	}
	public void setDoNum(Integer doNum) {
		this.doNum = doNum;
	}
	public String getFormulaLabel() {
		return formulaLabel;
	}
	public void setFormulaLabel(String formulaLabel) {
		this.formulaLabel = formulaLabel;
	}
	public Integer getDoNo1() {
		return doNo1;
	}
	public void setDoNo1(Integer doNo1) {
		this.doNo1 = doNo1;
	}
	public Integer getDoNo2() {
		return doNo2;
	}
	public void setDoNo2(Integer doNo2) {
		this.doNo2 = doNo2;
	}
	public Integer getDoNo3() {
		return doNo3;
	}
	public void setDoNo3(Integer doNo3) {
		this.doNo3 = doNo3;
	}
	public Integer getDoNo4() {
		return doNo4;
	}
	public void setDoNo4(Integer doNo4) {
		this.doNo4 = doNo4;
	}
	public Integer getDoNo5() {
		return doNo5;
	}
	public void setDoNo5(Integer doNo5) {
		this.doNo5 = doNo5;
	}
	public Integer getPolarity1() {
		return polarity1;
	}
	public void setPolarity1(Integer polarity1) {
		this.polarity1 = polarity1;
	}
	public Integer getPolarity2() {
		return polarity2;
	}
	public void setPolarity2(Integer polarity2) {
		this.polarity2 = polarity2;
	}
	public Integer getPolarity3() {
		return polarity3;
	}
	public void setPolarity3(Integer polarity3) {
		this.polarity3 = polarity3;
	}
	public Integer getPolarity4() {
		return polarity4;
	}
	public void setPolarity4(Integer polarity4) {
		this.polarity4 = polarity4;
	}
	public Boolean getFilterErr2() {
		return filterErr2;
	}
	public void setFilterErr2(Boolean filterErr2) {
		this.filterErr2 = filterErr2;
	}
	public Boolean getFilterErr3() {
		return filterErr3;
	}
	public void setFilterErr3(Boolean filterErr3) {
		this.filterErr3 = filterErr3;
	}
	public Boolean getFilterErr4() {
		return filterErr4;
	}
	public void setFilterErr4(Boolean filterErr4) {
		this.filterErr4 = filterErr4;
	}
	public Boolean getFilterDisturb1() {
		return filterDisturb1;
	}
	public void setFilterDisturb1(Boolean filterDisturb1) {
		this.filterDisturb1 = filterDisturb1;
	}
	public Boolean getFilterDisturb2() {
		return filterDisturb2;
	}
	public void setFilterDisturb2(Boolean filterDisturb2) {
		this.filterDisturb2 = filterDisturb2;
	}
	public Boolean getFilterDisturb3() {
		return filterDisturb3;
	}
	public void setFilterDisturb3(Boolean filterDisturb3) {
		this.filterDisturb3 = filterDisturb3;
	}
	public Boolean getFilterDisturb4() {
		return filterDisturb4;
	}
	public void setFilterDisturb4(Boolean filterDisturb4) {
		this.filterDisturb4 = filterDisturb4;
	}
	public Integer getDisturbTime1() {
		return disturbTime1;
	}
	public void setDisturbTime1(Integer disturbTime1) {
		this.disturbTime1 = disturbTime1;
	}
	public Integer getDisturbTime2() {
		return disturbTime2;
	}
	public void setDisturbTime2(Integer disturbTime2) {
		this.disturbTime2 = disturbTime2;
	}
	public Integer getDisturbTime3() {
		return disturbTime3;
	}
	public void setDisturbTime3(Integer disturbTime3) {
		this.disturbTime3 = disturbTime3;
	}
	public Integer getDisturbTime4() {
		return disturbTime4;
	}
	public void setDisturbTime4(Integer disturbTime4) {
		this.disturbTime4 = disturbTime4;
	}
	public String getChannelTag1() {
		return channelTag1;
	}
	public void setChannelTag1(String channelTag1) {
		this.channelTag1 = channelTag1;
	}
	public String getChannelTag2() {
		return channelTag2;
	}
	public void setChannelTag2(String channelTag2) {
		this.channelTag2 = channelTag2;
	}
	public String getChannelTag3() {
		return channelTag3;
	}
	public void setChannelTag3(String channelTag3) {
		this.channelTag3 = channelTag3;
	}
	public String getChannelTag4() {
		return channelTag4;
	}
	public void setChannelTag4(String channelTag4) {
		this.channelTag4 = channelTag4;
	}
	public Integer getDot1No1() {
		return dot1No1;
	}
	public void setDot1No1(Integer dot1No1) {
		this.dot1No1 = dot1No1;
	}
	public Integer getDot1No2() {
		return dot1No2;
	}
	public void setDot1No2(Integer dot1No2) {
		this.dot1No2 = dot1No2;
	}
	public Integer getDot1No3() {
		return dot1No3;
	}
	public void setDot1No3(Integer dot1No3) {
		this.dot1No3 = dot1No3;
	}
	public Integer getDot1No4() {
		return dot1No4;
	}
	public void setDot1No4(Integer dot1No4) {
		this.dot1No4 = dot1No4;
	}
	public Integer getDot1No5() {
		return dot1No5;
	}
	public void setDot1No5(Integer dot1No5) {
		this.dot1No5 = dot1No5;
	}
	public Integer getDot2No1() {
		return dot2No1;
	}
	public void setDot2No1(Integer dot2No1) {
		this.dot2No1 = dot2No1;
	}
	public Integer getDot2No2() {
		return dot2No2;
	}
	public void setDot2No2(Integer dot2No2) {
		this.dot2No2 = dot2No2;
	}
	public Integer getDot2No3() {
		return dot2No3;
	}
	public void setDot2No3(Integer dot2No3) {
		this.dot2No3 = dot2No3;
	}
	public Integer getDot2No4() {
		return dot2No4;
	}
	public void setDot2No4(Integer dot2No4) {
		this.dot2No4 = dot2No4;
	}
	public Integer getDot2No5() {
		return dot2No5;
	}
	public void setDot2No5(Integer dot2No5) {
		this.dot2No5 = dot2No5;
	}
	public Integer getDot3No1() {
		return dot3No1;
	}
	public void setDot3No1(Integer dot3No1) {
		this.dot3No1 = dot3No1;
	}
	public Integer getDot3No2() {
		return dot3No2;
	}
	public void setDot3No2(Integer dot3No2) {
		this.dot3No2 = dot3No2;
	}
	public Integer getDot3No3() {
		return dot3No3;
	}
	public void setDot3No3(Integer dot3No3) {
		this.dot3No3 = dot3No3;
	}
	public Integer getDot3No4() {
		return dot3No4;
	}
	public void setDot3No4(Integer dot3No4) {
		this.dot3No4 = dot3No4;
	}
	public Integer getDot3No5() {
		return dot3No5;
	}
	public void setDot3No5(Integer dot3No5) {
		this.dot3No5 = dot3No5;
	}
	public Integer getDot4No1() {
		return dot4No1;
	}
	public void setDot4No1(Integer dot4No1) {
		this.dot4No1 = dot4No1;
	}
	public Integer getDot4No2() {
		return dot4No2;
	}
	public void setDot4No2(Integer dot4No2) {
		this.dot4No2 = dot4No2;
	}
	public Integer getDot4No3() {
		return dot4No3;
	}
	public void setDot4No3(Integer dot4No3) {
		this.dot4No3 = dot4No3;
	}
	public Integer getDot4No4() {
		return dot4No4;
	}
	public void setDot4No4(Integer dot4No4) {
		this.dot4No4 = dot4No4;
	}
	public Integer getDot4No5() {
		return dot4No5;
	}
	public void setDot4No5(Integer dot4No5) {
		this.dot4No5 = dot4No5;
	}

	public Boolean getFilterErr1() {
		return filterErr1;
	}

	public void setFilterErr1(Boolean filterErr1) {
		this.filterErr1 = filterErr1;
	}

	public String getPointTpLabel() {
		return pointTpLabel;
	}

	public void setPointTpLabel(String pointTpLabel) {
		this.pointTpLabel = pointTpLabel;
	}
}
