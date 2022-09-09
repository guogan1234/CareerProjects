package com.avp.config.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * 模拟量点模板表
 * fxy
 */
@Entity
@Table(name = "aio_tp_tbl",schema = "conf")
public class AioTpTbl {
    //标签名
    @Id
    @Column(name = "aio_tp_label",length = 32)
    private String aioTpLabel;
    //名称
    @Column(name = "aio_tp_name",length = 64)
    private String aioTpName;
    //名称
    @Column(name = "aio_tp_order")
    private Integer aioTpOrder;
    //点类型
    @Column(name = "point_type")
    private Integer pointType;
    //设备模板标签
    @Column(name = "dev_tp_label",length = 32)
    private String devTpLabel;
    //单位
    @Column(name = "unit_id")
    private Integer unitId;
    //报警等级
    @Column(name = "alarm_level")
    private Integer alarmLevel;
    //报警延迟时间
    @Column(name = "alarm_delay_time")
    private Integer alarmDelayTime;
    //是否替换报警
    @Column(name = "is_replace_alarm")
    private  Boolean isReplaceAlarm;
    //报警确认后是否删除
    @Column(name = "is_del_on_ack")
    private Integer isDelOnAck;
    //是否限值
    @Column(name = "is_limit")
    private Boolean isLimit;
    //限值组数
    @Column(name = "limit_num")
    private Integer limitNum;
    //上限值1
    @Column(name = "limit_up1")
    private Float limitUp1;
    @Column(name = "limit_up2")
    private Float limitUp2;
    @Column(name = "limit_up3")
    private Float limitUp3;
    //下限值1
    @Column(name = "limit_low1")
    private Float limitLow1;

    @Column(name = "limit_low2")
    private Float limitLow2;
    @Column(name = "limit_low3")
    private Float limitLow3;
    //是否允许控制
    @Column(name = "is_control")
    private Boolean isControl;
    //控制等待动作
    @Column(name = "ctrl_wait_act")
    private Integer ctrlWaitAct;
    //控制保留时间
    @Column(name = "resv_time")
    private Integer resvTime;
    //控制超时时间
    @Column(name = "ctrl_time_out")
    private Integer ctrlTimeout;
    //控制数据格式
    @Column(name = "ctrl_format")
    private Integer ctrlFormat;
    //控制目标误差
    @Column(name = "ctrl_tolerance")
    private Float ctrlTolerance;
    //控制数据类型
    @Column(name = "ctrl_data_type")
    private Integer ctrlDataType;
    //控制度下限
    @Column(name = "ctrl_bottom_scale")
    private Float ctrlBottomScale;
    //控制度上限
    @Column(name = "ctrl_top_scale")
    private Float ctrlTopScale;
    //制码下限
    @Column(name = "ctrl_bottom_code")
    private Integer ctrlBottomCode;
    //控制码上限
    @Column(name = "ctrl_top_code")
    private Integer ctrlTopCode;
    //控制基值
    @Column(name = "ctrl_base")
    private Float ctrlBase;
    //控制系数
    @Column(name = "ctrl_coeff")
    private Float ctrlCoeff;
    //公式模版标签
    @Column(name = "formula_tp_label",length = 32)
    private String formulaTpLabel;
    //公式模版标签
    @Column(name = "is_sample")
    private Boolean isSample;
    //采样死区
    @Column(name = "sample_deadbande")
    private Float sampleDeadbande;
    //模拟量类型1
    @Column(name = "aio_type1")
    private Integer aioType1;
    @Column(name = "aio_type2")
    private Integer aioType2;
    @Column(name = "aio_type3")
    private Integer aioType3;
    @Column(name = "aio_type4")
    private Integer aioType4;
    //是否过滤遥测突变1
    @Column(name = "is_filter1")
    private Boolean isFilter1;
    @Column(name = "is_filter2")
    private Boolean isFilter2;
    @Column(name = "is_filter3")
    private Boolean isFilter3;
    @Column(name = "is_filter4")
    private Boolean isFilter4;
    //突变百分比1
    @Column(name = "aio_percent1")
    private Integer aioPercent1;
    @Column(name = "aio_percent2")
    private Integer aioPercent2;
    @Column(name = "aio_percent3")
    private Integer aioPercent3;
    @Column(name = "aio_percent4")
    private Integer aioPercent4;
    //是否有效1
    @Column(name = "is_valid1")
    private Boolean isValid1;
    @Column(name = "is_valid2")
    private Boolean isValid2;
    @Column(name = "is_valid3")
    private Boolean isValid3;
    @Column(name = "is_valid4")
    private Boolean isValid4;
    //死区值1
    @Column(name = "aio_deadband1")
    private Float aioDeadband1;
    @Column(name = "aio_deadband2")
    private Float aioDeadband2;
    @Column(name = "aio_deadband3")
    private Float aioDeadband3;
    @Column(name = "aio_deadband4")
    private Float aioDeadband4;
    //归零值1
    @Column(name = "aio_zeroband1")
    private Float aioZeroband1;
    @Column(name = "aio_zeroband2")
    private Float aioZeroband2;
    @Column(name = "aio_zeroband3")
    private Float aioZeroband3;
    @Column(name = "aio_zeroband4")
    private Float aioZeroband4;
    //基值1
    @Column(name = "aio_base1")
    private Float aioBase1;
    @Column(name = "aio_base2")
    private Float aioBase2;
    @Column(name = "aio_base3")
    private Float aioBase3;
    @Column(name = "aio_base4")
    private Float aioBase4;
    //系数1
    @Column(name = "aio_coeff1")
    private Float aioCoeff1;
    @Column(name = "aio_coeff2")
    private Float aioCoeff2;
    @Column(name = "aio_coeff3")
    private Float aioCoeff3;
    @Column(name = "aio_coeff4")
    private Float aioCoeff4;
    //度下限1
    @Column(name = "bottom_scale1")
    private Float bottomScale1;
    @Column(name = "bottom_scale2")
    private Float bottomScale2;
    @Column(name = "bottom_scale3")
    private Float bottomScale3;
    @Column(name = "bottom_scale4")
    private Float bottomScale4;
    //度上限1
    @Column(name = "top_scale1")
    private Float topScale1;
    @Column(name = "top_scale2")
    private Float topScale2;
    @Column(name = "top_scale3")
    private Float topScale3;
    @Column(name = "top_scale4")
    private Float topScale4;
    //码下限1
    @Column(name = "bottom_code1")
    private Integer bottomCode1;
    @Column(name = "bottom_code2")
    private Integer bottomCode2;
    @Column(name = "bottom_code3")
    private Integer bottomCode3;
    @Column(name = "bottom_code4")
    private Integer bottomCode4;
    //码上限1
    @Column(name = "top_code1")
    private Integer topCode1;
    @Column(name = "top_code2")
    private Integer topCode2;
    @Column(name = "top_code3")
    private Integer topCode3;
    @Column(name = "top_code4")
    private Integer topCode4;
    @Column(name = "operate_time")
    private Timestamp operateTime;

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

	public Integer getAioTpOrder() {
		return aioTpOrder;
	}

	public void setAioTpOrder(Integer aioTpOrder) {
		this.aioTpOrder = aioTpOrder;
	}

	public Integer getPointType() {
		return pointType;
	}

	public void setPointType(Integer pointType) {
		this.pointType = pointType;
	}

	public String getDevTpLabel() {
		return devTpLabel;
	}

	public void setDevTpLabel(String devTpLabel) {
		this.devTpLabel = devTpLabel;
	}

	public Integer getUnitId() {
		return unitId;
	}

	public void setUnitId(Integer unitId) {
		this.unitId = unitId;
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

	public Boolean getIsLimit() {
		return isLimit;
	}

	public void setIsLimit(Boolean isLimit) {
		this.isLimit = isLimit;
	}

	public Integer getLimitNum() {
		return limitNum;
	}

	public void setLimitNum(Integer limitNum) {
		this.limitNum = limitNum;
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

	public Boolean getIsControl() {
		return isControl;
	}

	public void setIsControl(Boolean isControl) {
		this.isControl = isControl;
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

	public Float getCtrlBase() {
		return ctrlBase;
	}

	public void setCtrlBase(Float ctrlBase) {
		this.ctrlBase = ctrlBase;
	}

	public Float getCtrlCoeff() {
		return ctrlCoeff;
	}

	public void setCtrlCoeff(Float ctrlCoeff) {
		this.ctrlCoeff = ctrlCoeff;
	}

	public String getFormulaTpLabel() {
		return formulaTpLabel;
	}

	public void setFormulaTpLabel(String formulaTpLabel) {
		this.formulaTpLabel = formulaTpLabel;
	}

	public Boolean getIsSample() {
		return isSample;
	}

	public void setIsSample(Boolean isSample) {
		this.isSample = isSample;
	}

	public Float getSampleDeadbande() {
		return sampleDeadbande;
	}

	public void setSampleDeadbande(Float sampleDeadbande) {
		this.sampleDeadbande = sampleDeadbande;
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
	public Timestamp getOperateTime() {
		return operateTime;
	}

	public void setOperateTime(Timestamp operateTime) {
		this.operateTime = operateTime;
	}

}
