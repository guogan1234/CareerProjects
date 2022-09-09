package com.avp.config.entity.vo;

public class AccSignalInstanceVo {
	private String accTpLabel;
	private String accTpName;
	private String devLabel;
	//Acc表,Acc_limit表,Acc_def表
	private String accLabel;
	
	//Acc表传入字段
	private Integer unitId;
	private String accName;
	private Boolean isLimit;
	private Boolean isSample;
	private Integer sampleDeadband;
	private String cameraTag;
	private String cameraPreset;
	
	//AccLimit表传入字段
	private Integer limitNum;
    private Integer limitUp1;
    private Integer limitUp2;
    private Integer limitUp3;
    private Integer limitLow1;
    private Integer limitLow2;
    private Integer limitLow3;
    private Integer alarmLevel;
    private Integer alarmDelayTime;
    private Boolean isReplaceAlarm;
    private Integer isDelOnAck;
    private String picName;
    
    //AccDef表传入地段
    private Integer accBase1;
    private Integer accBase2;
    private Integer accBase3;
    private Integer accBase4;
    private Integer accCoeff1;
    private Integer accCoeff2;
    private Integer accCoeff3;
    private Integer accCoeff4;
    private Integer dotNo1;
    private Integer dotNo2;
    private Integer dotNo3;
    private Integer dotNo4;
    private String channelTag1;
    private String channelTag2;
    private String channelTag3;
    private String channelTag4;
	private String pointTpLabel;
	
	
	public String getAccTpLabel() {
		return accTpLabel;
	}
	public void setAccTpLabel(String accTpLabel) {
		this.accTpLabel = accTpLabel;
	}
	public String getAccTpName() {
		return accTpName;
	}
	public void setAccTpName(String accTpName) {
		this.accTpName = accTpName;
	}
	public String getDevLabel() {
		return devLabel;
	}
	public void setDevLabel(String devLabel) {
		this.devLabel = devLabel;
	}
	public String getAccLabel() {
		return accLabel;
	}
	public void setAccLabel(String accLabel) {
		this.accLabel = accLabel;
	}
	public Integer getUnitId() {
		return unitId;
	}
	public void setUnitId(Integer unitId) {
		this.unitId = unitId;
	}
	public String getAccName() {
		return accName;
	}
	public void setAccName(String accName) {
		this.accName = accName;
	}
	public Boolean getIsLimit() {
		return isLimit;
	}
	public void setIsLimit(Boolean isLimit) {
		this.isLimit = isLimit;
	}
	public Boolean getIsSample() {
		return isSample;
	}
	public void setIsSample(Boolean isSample) {
		this.isSample = isSample;
	}
	public Integer getSampleDeadband() {
		return sampleDeadband;
	}
	public void setSampleDeadband(Integer sampleDeadband) {
		this.sampleDeadband = sampleDeadband;
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
	public Integer getLimitNum() {
		return limitNum;
	}
	public void setLimitNum(Integer limitNum) {
		this.limitNum = limitNum;
	}
	public Integer getLimitUp1() {
		return limitUp1;
	}
	public void setLimitUp1(Integer limitUp1) {
		this.limitUp1 = limitUp1;
	}
	public Integer getLimitUp2() {
		return limitUp2;
	}
	public void setLimitUp2(Integer limitUp2) {
		this.limitUp2 = limitUp2;
	}
	public Integer getLimitUp3() {
		return limitUp3;
	}
	public void setLimitUp3(Integer limitUp3) {
		this.limitUp3 = limitUp3;
	}
	public Integer getLimitLow1() {
		return limitLow1;
	}
	public void setLimitLow1(Integer limitLow1) {
		this.limitLow1 = limitLow1;
	}
	public Integer getLimitLow2() {
		return limitLow2;
	}
	public void setLimitLow2(Integer limitLow2) {
		this.limitLow2 = limitLow2;
	}
	public Integer getLimitLow3() {
		return limitLow3;
	}
	public void setLimitLow3(Integer limitLow3) {
		this.limitLow3 = limitLow3;
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
	public String getPicName() {
		return picName;
	}
	public void setPicName(String picName) {
		this.picName = picName;
	}
	public Integer getAccBase1() {
		return accBase1;
	}
	public void setAccBase1(Integer accBase1) {
		this.accBase1 = accBase1;
	}
	public Integer getAccBase2() {
		return accBase2;
	}
	public void setAccBase2(Integer accBase2) {
		this.accBase2 = accBase2;
	}
	public Integer getAccBase3() {
		return accBase3;
	}
	public void setAccBase3(Integer accBase3) {
		this.accBase3 = accBase3;
	}
	public Integer getAccBase4() {
		return accBase4;
	}
	public void setAccBase4(Integer accBase4) {
		this.accBase4 = accBase4;
	}
	public Integer getAccCoeff1() {
		return accCoeff1;
	}
	public void setAccCoeff1(Integer accCoeff1) {
		this.accCoeff1 = accCoeff1;
	}
	public Integer getAccCoeff2() {
		return accCoeff2;
	}
	public void setAccCoeff2(Integer accCoeff2) {
		this.accCoeff2 = accCoeff2;
	}
	public Integer getAccCoeff3() {
		return accCoeff3;
	}
	public void setAccCoeff3(Integer accCoeff3) {
		this.accCoeff3 = accCoeff3;
	}
	public Integer getAccCoeff4() {
		return accCoeff4;
	}
	public void setAccCoeff4(Integer accCoeff4) {
		this.accCoeff4 = accCoeff4;
	}
	public Integer getDotNo1() {
		return dotNo1;
	}
	public void setDotNo1(Integer dotNo1) {
		this.dotNo1 = dotNo1;
	}
	public Integer getDotNo2() {
		return dotNo2;
	}
	public void setDotNo2(Integer dotNo2) {
		this.dotNo2 = dotNo2;
	}
	public Integer getDotNo3() {
		return dotNo3;
	}
	public void setDotNo3(Integer dotNo3) {
		this.dotNo3 = dotNo3;
	}
	public Integer getDotNo4() {
		return dotNo4;
	}
	public void setDotNo4(Integer dotNo4) {
		this.dotNo4 = dotNo4;
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
	public String getPointTpLabel() {
		return pointTpLabel;
	}
	public void setPointTpLabel(String pointTpLabel) {
		this.pointTpLabel = pointTpLabel;
	}
	

}
