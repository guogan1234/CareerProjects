package com.avp.config.entity.vo;

public class AioSignalInstanceVo {
	private String aioTpLabel;
	private String aioTpName;
	private String devLabel;
	
	//ai表,ao表 ,aio-def表,ai_limit
	public String aioLabel;
	
	//--------------------- ai表插入字段 --------------------------
	//aiName
	private String aioName;
	private Integer aiOrder;
	private Integer pointType;
	private Integer unitId;
	private Boolean isLimit;
	private Boolean isControl;
	private Boolean isSample;
	private Float sampleDeadband;
	private String cameraTag;
    private String cameraPreset;
    
    //--------------------- ao表插入字段 --------------------------
    //waitAct
    private Integer ctrlWaitAct;
    private Integer resvTime;
    //execTimeout
    private Integer ctrlTimeout;
    //aoFormat
    private Integer ctrlFormat;
    //aoTolerance
    private Float ctrlTolerance;
    //dataType
    private Integer ctrlDataType;
    //bottomScale
    private Float ctrlBottomScale;
    //topScale
    private Float ctrlTopScale;
    //bottomCode
    private Integer ctrlBottomCode;
    //topCode
    private Integer ctrlTopCode;
    //aoBase
    private Float ctrlAoBase;
    //aoCoeff
    private Float ctrlAoCoeff;
    private String formulaLabel;
    private Integer aoNo;
    
    //------------------- aio_def表插入字段------------------------
    private Integer aioType1;
    private Integer aioType2;
    private Integer aioType3;
    private Integer aioType4;
    private Boolean isFilter1;
    private Boolean isFilter2;
    private Boolean isFilter3;
    private Boolean isFilter4;
    private Integer aioPercent1;
    private Integer aioPercent2;
    private Integer aioPercent3;
    private Integer aioPercent4;
    private Boolean isValid1;
    private Boolean isValid2;
    private Boolean isValid3;
    private Boolean isValid4;
    private Float aioDeadband1;
    private Float aioDeadband2;
    private Float aioDeadband3;
    private Float aioDeadband4;
    private Float aioZeroband1;
    private Float aioZeroband2;
    private Float aioZeroband3;
    private Float aioZeroband4;
    private Float aioBase1;
    private Float aioBase2;
    private Float aioBase3;
    private Float aioBase4;
    private Float aioCoeff1;
    private Float aioCoeff2;
    private Float aioCoeff3;
    private Float aioCoeff4;
    private Float bottomScale1;
    private Float bottomScale2;
    private Float bottomScale3;
    private Float bottomScale4;
    private Float topScale1;
    private Float topScale2;
    private Float topScale3;
    private Float topScale4;
    private Integer bottomCode1;
    private Integer bottomCode2;
    private Integer bottomCode3;
    private Integer bottomCode4;
    private Integer topCode1;
    private Integer topCode2;
    private Integer topCode3;
    private Integer topCode4;
    private String channelTag1;
    private String channelTag2;
    private String channelTag3;
    private String channelTag4;
    private Integer dotNo1;
    private Integer dotNo2;
    private Integer dotNo3;
    private Integer dotNo4;
    
    //------------------- aio_limit表插入字段------------------------
    private Float limitUp1;
    private Float limitUp2;
    private Float limitUp3;
    private Float limitLow1;
    private Float limitLow2;
    private Float limitLow3;
    private Integer limitNum;
    private Integer isDelOnAck;
    private Boolean isReplaceAlarm;
    private Integer alarmDelayTime;
    private String picName;
    private Integer alarmLevel;

	private String pointTpLabel;
    
	public String getAioTpLabel() {
		return aioTpLabel;
	}
	public void setAioTpLabel(String aioTpLabel) {
		this.aioTpLabel = aioTpLabel;
	}
	public String getAioTpName() {
		return aioTpName;
	}
	public void setAioTpName(String aioTpName) {
		this.aioTpName = aioTpName;
	}
	public String getDevLabel() {
		return devLabel;
	}
	public void setDevLabel(String devLabel) {
		this.devLabel = devLabel;
	}
	public String getAioLabel() {
		return aioLabel;
	}
	public void setAioLabel(String aioLabel) {
		this.aioLabel = aioLabel;
	}
	public String getAioName() {
		return aioName;
	}
	public void setAioName(String aioName) {
		this.aioName = aioName;
	}
	public Integer getAiOrder() {
		return aiOrder;
	}
	public void setAiOrder(Integer aiOrder) {
		this.aiOrder = aiOrder;
	}
	public Integer getPointType() {
		return pointType;
	}
	public void setPointType(Integer pointType) {
		this.pointType = pointType;
	}
	public Integer getUnitId() {
		return unitId;
	}
	public void setUnitId(Integer unitId) {
		this.unitId = unitId;
	}
	public Boolean getIsLimit() {
		return isLimit;
	}
	public void setIsLimit(Boolean isLimit) {
		this.isLimit = isLimit;
	}
	public Boolean getIsControl() {
		return isControl;
	}
	public void setIsControl(Boolean isControl) {
		this.isControl = isControl;
	}
	public Boolean getIsSample() {
		return isSample;
	}
	public void setIsSample(Boolean isSample) {
		this.isSample = isSample;
	}
	public Float getSampleDeadband() {
		return sampleDeadband;
	}
	public void setSampleDeadband(Float sampleDeadband) {
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
	public Integer getCtrlWaitAct() {
		return ctrlWaitAct;
	}
	public void setCtrlWaitAct(Integer ctrlWaitAct) {
		this.ctrlWaitAct = ctrlWaitAct;
	}
	public Integer getResvTime() {
		return resvTime;
	}
	public void setResvTime(Integer resvTime) {
		this.resvTime = resvTime;
	}
	public Integer getCtrlTimeout() {
		return ctrlTimeout;
	}
	public void setCtrlTimeout(Integer ctrlTimeout) {
		this.ctrlTimeout = ctrlTimeout;
	}
	public Integer getCtrlFormat() {
		return ctrlFormat;
	}
	public void setCtrlFormat(Integer ctrlFormat) {
		this.ctrlFormat = ctrlFormat;
	}
	public Float getCtrlTolerance() {
		return ctrlTolerance;
	}
	public void setCtrlTolerance(Float ctrlTolerance) {
		this.ctrlTolerance = ctrlTolerance;
	}
	public Integer getCtrlDataType() {
		return ctrlDataType;
	}
	public void setCtrlDataType(Integer ctrlDataType) {
		this.ctrlDataType = ctrlDataType;
	}
	public Float getCtrlBottomScale() {
		return ctrlBottomScale;
	}
	public void setCtrlBottomScale(Float ctrlBottomScale) {
		this.ctrlBottomScale = ctrlBottomScale;
	}
	public Float getCtrlTopScale() {
		return ctrlTopScale;
	}
	public void setCtrlTopScale(Float ctrlTopScale) {
		this.ctrlTopScale = ctrlTopScale;
	}
	public Integer getCtrlBottomCode() {
		return ctrlBottomCode;
	}
	public void setCtrlBottomCode(Integer ctrlBottomCode) {
		this.ctrlBottomCode = ctrlBottomCode;
	}
	public Integer getCtrlTopCode() {
		return ctrlTopCode;
	}
	public void setCtrlTopCode(Integer ctrlTopCode) {
		this.ctrlTopCode = ctrlTopCode;
	}
	public Float getCtrlAoBase() {
		return ctrlAoBase;
	}
	public void setCtrlAoBase(Float ctrlAoBase) {
		this.ctrlAoBase = ctrlAoBase;
	}
	public Float getCtrlAoCoeff() {
		return ctrlAoCoeff;
	}
	public void setCtrlAoCoeff(Float ctrlAoCoeff) {
		this.ctrlAoCoeff = ctrlAoCoeff;
	}
	public String getFormulaLabel() {
		return formulaLabel;
	}
	public void setFormulaLabel(String formulaLabel) {
		this.formulaLabel = formulaLabel;
	}
	public Integer getAoNo() {
		return aoNo;
	}
	public void setAoNo(Integer aoNo) {
		this.aoNo = aoNo;
	}
	public Integer getAioType1() {
		return aioType1;
	}
	public void setAioType1(Integer aioType1) {
		this.aioType1 = aioType1;
	}
	public Integer getAioType2() {
		return aioType2;
	}
	public void setAioType2(Integer aioType2) {
		this.aioType2 = aioType2;
	}
	public Integer getAioType3() {
		return aioType3;
	}
	public void setAioType3(Integer aioType3) {
		this.aioType3 = aioType3;
	}
	public Integer getAioType4() {
		return aioType4;
	}
	public void setAioType4(Integer aioType4) {
		this.aioType4 = aioType4;
	}
	public Boolean getIsFilter1() {
		return isFilter1;
	}
	public void setIsFilter1(Boolean isFilter1) {
		this.isFilter1 = isFilter1;
	}
	public Boolean getIsFilter2() {
		return isFilter2;
	}
	public void setIsFilter2(Boolean isFilter2) {
		this.isFilter2 = isFilter2;
	}
	public Boolean getIsFilter3() {
		return isFilter3;
	}
	public void setIsFilter3(Boolean isFilter3) {
		this.isFilter3 = isFilter3;
	}
	public Boolean getIsFilter4() {
		return isFilter4;
	}
	public void setIsFilter4(Boolean isFilter4) {
		this.isFilter4 = isFilter4;
	}
	public Integer getAioPercent1() {
		return aioPercent1;
	}
	public void setAioPercent1(Integer aioPercent1) {
		this.aioPercent1 = aioPercent1;
	}
	public Integer getAioPercent2() {
		return aioPercent2;
	}
	public void setAioPercent2(Integer aioPercent2) {
		this.aioPercent2 = aioPercent2;
	}
	public Integer getAioPercent3() {
		return aioPercent3;
	}
	public void setAioPercent3(Integer aioPercent3) {
		this.aioPercent3 = aioPercent3;
	}
	public Integer getAioPercent4() {
		return aioPercent4;
	}
	public void setAioPercent4(Integer aioPercent4) {
		this.aioPercent4 = aioPercent4;
	}
	public Boolean getIsValid1() {
		return isValid1;
	}
	public void setIsValid1(Boolean isValid1) {
		this.isValid1 = isValid1;
	}
	public Boolean getIsValid2() {
		return isValid2;
	}
	public void setIsValid2(Boolean isValid2) {
		this.isValid2 = isValid2;
	}
	public Boolean getIsValid3() {
		return isValid3;
	}
	public void setIsValid3(Boolean isValid3) {
		this.isValid3 = isValid3;
	}
	public Boolean getIsValid4() {
		return isValid4;
	}
	public void setIsValid4(Boolean isValid4) {
		this.isValid4 = isValid4;
	}
	public Float getAioDeadband1() {
		return aioDeadband1;
	}
	public void setAioDeadband1(Float aioDeadband1) {
		this.aioDeadband1 = aioDeadband1;
	}
	public Float getAioDeadband2() {
		return aioDeadband2;
	}
	public void setAioDeadband2(Float aioDeadband2) {
		this.aioDeadband2 = aioDeadband2;
	}
	public Float getAioDeadband3() {
		return aioDeadband3;
	}
	public void setAioDeadband3(Float aioDeadband3) {
		this.aioDeadband3 = aioDeadband3;
	}
	public Float getAioDeadband4() {
		return aioDeadband4;
	}
	public void setAioDeadband4(Float aioDeadband4) {
		this.aioDeadband4 = aioDeadband4;
	}
	public Float getAioZeroband1() {
		return aioZeroband1;
	}
	public void setAioZeroband1(Float aioZeroband1) {
		this.aioZeroband1 = aioZeroband1;
	}
	public Float getAioZeroband2() {
		return aioZeroband2;
	}
	public void setAioZeroband2(Float aioZeroband2) {
		this.aioZeroband2 = aioZeroband2;
	}
	public Float getAioZeroband3() {
		return aioZeroband3;
	}
	public void setAioZeroband3(Float aioZeroband3) {
		this.aioZeroband3 = aioZeroband3;
	}
	public Float getAioZeroband4() {
		return aioZeroband4;
	}
	public void setAioZeroband4(Float aioZeroband4) {
		this.aioZeroband4 = aioZeroband4;
	}
	public Float getAioBase1() {
		return aioBase1;
	}
	public void setAioBase1(Float aioBase1) {
		this.aioBase1 = aioBase1;
	}
	public Float getAioBase2() {
		return aioBase2;
	}
	public void setAioBase2(Float aioBase2) {
		this.aioBase2 = aioBase2;
	}
	public Float getAioBase3() {
		return aioBase3;
	}
	public void setAioBase3(Float aioBase3) {
		this.aioBase3 = aioBase3;
	}
	public Float getAioBase4() {
		return aioBase4;
	}
	public void setAioBase4(Float aioBase4) {
		this.aioBase4 = aioBase4;
	}
	public Float getAioCoeff1() {
		return aioCoeff1;
	}
	public void setAioCoeff1(Float aioCoeff1) {
		this.aioCoeff1 = aioCoeff1;
	}
	public Float getAioCoeff2() {
		return aioCoeff2;
	}
	public void setAioCoeff2(Float aioCoeff2) {
		this.aioCoeff2 = aioCoeff2;
	}
	public Float getAioCoeff3() {
		return aioCoeff3;
	}
	public void setAioCoeff3(Float aioCoeff3) {
		this.aioCoeff3 = aioCoeff3;
	}
	public Float getAioCoeff4() {
		return aioCoeff4;
	}
	public void setAioCoeff4(Float aioCoeff4) {
		this.aioCoeff4 = aioCoeff4;
	}
	public Float getBottomScale1() {
		return bottomScale1;
	}
	public void setBottomScale1(Float bottomScale1) {
		this.bottomScale1 = bottomScale1;
	}
	public Float getBottomScale2() {
		return bottomScale2;
	}
	public void setBottomScale2(Float bottomScale2) {
		this.bottomScale2 = bottomScale2;
	}
	public Float getBottomScale3() {
		return bottomScale3;
	}
	public void setBottomScale3(Float bottomScale3) {
		this.bottomScale3 = bottomScale3;
	}
	public Float getBottomScale4() {
		return bottomScale4;
	}
	public void setBottomScale4(Float bottomScale4) {
		this.bottomScale4 = bottomScale4;
	}
	public Float getTopScale1() {
		return topScale1;
	}
	public void setTopScale1(Float topScale1) {
		this.topScale1 = topScale1;
	}
	public Float getTopScale2() {
		return topScale2;
	}
	public void setTopScale2(Float topScale2) {
		this.topScale2 = topScale2;
	}
	public Float getTopScale3() {
		return topScale3;
	}
	public void setTopScale3(Float topScale3) {
		this.topScale3 = topScale3;
	}
	public Float getTopScale4() {
		return topScale4;
	}
	public void setTopScale4(Float topScale4) {
		this.topScale4 = topScale4;
	}
	public Integer getBottomCode1() {
		return bottomCode1;
	}
	public void setBottomCode1(Integer bottomCode1) {
		this.bottomCode1 = bottomCode1;
	}
	public Integer getBottomCode2() {
		return bottomCode2;
	}
	public void setBottomCode2(Integer bottomCode2) {
		this.bottomCode2 = bottomCode2;
	}
	public Integer getBottomCode3() {
		return bottomCode3;
	}
	public void setBottomCode3(Integer bottomCode3) {
		this.bottomCode3 = bottomCode3;
	}
	public Integer getBottomCode4() {
		return bottomCode4;
	}
	public void setBottomCode4(Integer bottomCode4) {
		this.bottomCode4 = bottomCode4;
	}
	public Integer getTopCode1() {
		return topCode1;
	}
	public void setTopCode1(Integer topCode1) {
		this.topCode1 = topCode1;
	}
	public Integer getTopCode2() {
		return topCode2;
	}
	public void setTopCode2(Integer topCode2) {
		this.topCode2 = topCode2;
	}
	public Integer getTopCode3() {
		return topCode3;
	}
	public void setTopCode3(Integer topCode3) {
		this.topCode3 = topCode3;
	}
	public Integer getTopCode4() {
		return topCode4;
	}
	public void setTopCode4(Integer topCode4) {
		this.topCode4 = topCode4;
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
	public Float getLimitUp1() {
		return limitUp1;
	}
	public void setLimitUp1(Float limitUp1) {
		this.limitUp1 = limitUp1;
	}
	public Float getLimitUp2() {
		return limitUp2;
	}
	public void setLimitUp2(Float limitUp2) {
		this.limitUp2 = limitUp2;
	}
	public Float getLimitUp3() {
		return limitUp3;
	}
	public void setLimitUp3(Float limitUp3) {
		this.limitUp3 = limitUp3;
	}
	public Float getLimitLow1() {
		return limitLow1;
	}
	public void setLimitLow1(Float limitLow1) {
		this.limitLow1 = limitLow1;
	}
	public Float getLimitLow2() {
		return limitLow2;
	}
	public void setLimitLow2(Float limitLow2) {
		this.limitLow2 = limitLow2;
	}
	public Float getLimitLow3() {
		return limitLow3;
	}
	public void setLimitLow3(Float limitLow3) {
		this.limitLow3 = limitLow3;
	}
	public Integer getLimitNum() {
		return limitNum;
	}
	public void setLimitNum(Integer limitNum) {
		this.limitNum = limitNum;
	}
	public Integer getIsDelOnAck() {
		return isDelOnAck;
	}
	public void setIsDelOnAck(Integer isDelOnAck) {
		this.isDelOnAck = isDelOnAck;
	}
	public Boolean getIsReplaceAlarm() {
		return isReplaceAlarm;
	}
	public void setIsReplaceAlarm(Boolean isReplaceAlarm) {
		this.isReplaceAlarm = isReplaceAlarm;
	}
	public Integer getAlarmDelayTime() {
		return alarmDelayTime;
	}
	public void setAlarmDelayTime(Integer alarmDelayTime) {
		this.alarmDelayTime = alarmDelayTime;
	}
	public String getPicName() {
		return picName;
	}
	public void setPicName(String picName) {
		this.picName = picName;
	}
	public Integer getAlarmLevel() {
		return alarmLevel;
	}
	public void setAlarmLevel(Integer alarmLevel) {
		this.alarmLevel = alarmLevel;
	}

	public String getPointTpLabel() {
		return pointTpLabel;
	}

	public void setPointTpLabel(String pointTpLabel) {
		this.pointTpLabel = pointTpLabel;
	}
}
