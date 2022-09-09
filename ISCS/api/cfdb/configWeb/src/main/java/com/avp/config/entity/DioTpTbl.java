package com.avp.config.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * 离散点模板表
 * DIG 点模版
 */
@Entity
@Table(name = "dio_tp_tbl",schema = "conf")
public class DioTpTbl {
    //标签
    @Id
    @Column(name = "dio_tp_label",length = 32)
    private  String dioTpLabel;
    //名称
    @Column(name = "dio_tp_name",length = 64)
    private  String dioTpName;
    //序号
    @Column(name = "dio_tp_order")
    private  Integer dioTpOrder;
    //点类型
    @Column(name = "point_type")
    private String pointType;
    //设备模板标签
    @Column(name = "dev_tp_label",length = 32)
    private  String devTpLabel;
    //数字量文本标签
    @Column(name = "di_text_label",length = 32)
    private  String diTextLabel;
    //值分量数
    @Column(name = "bit_num")
    private  Integer bitNum;
    //分量延迟时间
    @Column(name = "bit_delay_time")
    private  Integer bitDelayTime;
    //报警等级
    @Column(name = "alarm_level")
    private  Integer alarmLevel;
    //报警延迟时间
    @Column(name = "alarm_delay_time")
    private  Integer alarmDelayTime;
    //是否替换报警
    @Column(name = "is_replace_alarm")
    private  Boolean isReplaceAlarm;
    //报警确认后是否删除
    @Column(name = "is_del_on_ack")
    private  Integer isDelOnAck;
    //是否控制
    @Column(name = "is_control")
    private  Boolean isControl;
    //控制动作组标签
    @Column(name = "ctrl_grp_label",length = 32)
    private  String ctrlGrpLabel;
    //控制等待动作
    @Column(name = "ctrl_wait_act")
    private  Integer ctrlWaitAct;
    //控制保留时间
    @Column(name = "resv_time")
    private  Integer resvTime;
    //控制超时时间
    @Column(name = "ctrl_timeout")
    private  Integer ctrlTimeout;
    //脉冲保持时间
    @Column(name = "pulse_duration")
    private  Integer pulseDuration;
    //DO点号个数
    @Column(name = "do_num")
    private  Integer doNum;
    //公式模版标签
    @Column(name = "formula_tp_label",length = 32)
    private  String formulaTpLabel;
    //是否摘要
    @Column(name = "is_summary")
    private  Boolean isSummary;
    //是否采样
    @Column(name = "is_sample")
    private  Boolean isSample;
    //极性1
    @Column(name = "polarity1")
    private Integer polarity1;
    @Column(name = "polarity2")
    private Integer polarity2;
    @Column(name = "polarity3")
    private Integer polarity3;
    @Column(name = "polarity4")
    private Integer polarity4;
    //是否过滤误DI1
    @Column(name = "filter_err1")
    private  Boolean filterErr1;
    @Column(name = "filter_err2")
    private  Boolean filterErr2;
    @Column(name = "filter_err3")
    private  Boolean filterErr3;
    @Column(name = "filter_err4")
    private  Boolean filterErr4;
    //是否过滤抖动1
    @Column(name = "filter_disturb1")
    private  Boolean filterDisturb1;
    @Column(name = "filter_disturb2")
    private  Boolean filterDisturb2;
    @Column(name = "filter_disturb3")
    private  Boolean filterDisturb3;
    @Column(name = "filter_disturb4")
    private  Boolean filterDisturb4;
    //抖动时限1
    @Column(name = "disturb_time1")
    private Integer disturbTime1;
    @Column(name = "disturb_time2")
    private Integer disturbTime2;
    @Column(name = "disturb_time3")
    private Integer disturbTime3;
    @Column(name = "disturb_time4")
    private Integer disturbTime4;

    @Column(name = "operate_time")
    private Timestamp operateTime;

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

    public Integer getDioTpOrder() {
        return dioTpOrder;
    }

    public void setDioTpOrder(Integer dioTpOrder) {
        this.dioTpOrder = dioTpOrder;
    }

    public String getPointType() {
		return pointType;
	}

	public void setPointType(String pointType) {
		this.pointType = pointType;
	}

	public String getDevTpLabel() {
        return devTpLabel;
    }

    public void setDevTpLabel(String devTpLabel) {
        this.devTpLabel = devTpLabel;
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

    public String getFormulaTpLabel() {
        return formulaTpLabel;
    }

    public void setFormulaTpLabel(String formulaTpLabel) {
        this.formulaTpLabel = formulaTpLabel;
    }

    public Boolean getIsSummary() {
        return isSummary;
    }

    public void setIsSummary(Boolean isSummary) {
        this.isSummary = isSummary;
    }

    public Boolean getIsSample() {
        return isSample;
    }

    public void setIsSample(Boolean isSample) {
        this.isSample = isSample;
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

    public Boolean getFilterErr1() {
        return filterErr1;
    }

    public void setFilterErr1(Boolean filterErr1) {
        this.filterErr1 = filterErr1;
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

    public Timestamp getOperateTime() {
        return operateTime;
    }

    public void setOperateTime(Timestamp operateTime) {
        this.operateTime = operateTime;
    }
}
