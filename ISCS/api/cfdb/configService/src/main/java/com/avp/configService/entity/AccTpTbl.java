package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 *acc 点模版
 * 累积量点模板表
 */
@Entity
@Table(name = "acc_tp_tbl",schema = "conf")
public class AccTpTbl {
    //累积量模版标签
    @Id
    @Column(name = "acc_tp_label",length = 32)
    private String accTpLabel;
    //累积量模版名称
    @Column(name = "acc_tp_name",length = 64)
    private String accTpName;
    //设备模版标签
    @Column(name = "dev_tp_label",length = 32)
    private String devTpLabel;
    //顺序号
    @Column(name = "acc_tp_order")
    private  Integer accTpOrder;
    //单位ID
    @Column(name = "unit_id")
    private  Integer unitId;
    //是否限制
    @Column(name = "is_limit")
    private  Boolean isLimit;
    //限制组数
    @Column(name = "limit_num")
    private  Integer limitNum;
    //上限值
    @Column(name = "limit_up1")
    private  Integer limitUp1;
    //上限值
    @Column(name = "limit_up2")
    private  Integer limitUp2;
    //上限值
    @Column(name = "limit_up3")
    private  Integer limitUp3;
    //下限值
    @Column(name = "limit_low1")
    private  Integer limitLow1;
    //下限值
    @Column(name = "limit_low2")
    private  Integer limitLow2;
    //下限值
    @Column(name = "limit_low3")
    private  Integer limitLow3;
    //报警等级
    @Column(name = "alarm_level")
    private Integer alarmLevel;
    //报警延迟时间
    @Column(name = "alarm_delay_time")
    private  Integer alarmDelayTime;
    //是否替换报警
    @Column(name = "is_replace_alarm")
    private Boolean isReplaceAlarm;
    //确认报警后是否删除报警。值及描述：0 -- 确认后删除；1 -- 返回状态确认后删除。
    @Column(name = "is_del_on_ack")
    private  Integer isDelOnAck;
    //是否采样
    @Column(name = "is_sample")
    private  Boolean isSample;
    //采样死区
    @Column(name = "sample_deadbande")
    private  Integer sampleDeadbande;
    //基值1
    @Column(name = "acc_base1")
    private Integer accBase1;
    //基值2
    @Column(name = "acc_base2")
    private Integer accBase2;
    //基值3
    @Column(name = "acc_base3")
    private Integer accBase3;
    //基值4
    @Column(name = "acc_base4")
    private Integer accBase4;
    //系数1
    @Column(name = "acc_coeff1")
    private Integer accCoeff1;
    //系数2
    @Column(name = "acc_coeff2")
    private Integer accCoeff2;
    //系数3
    @Column(name = "acc_coeff3")
    private Integer accCoeff3;
    //系数4
    @Column(name = "acc_coeff4")
    private Integer accCoeff4;

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

    public String getDevTpLabel() {
        return devTpLabel;
    }

    public void setDevTpLabel(String devTpLabel) {
        this.devTpLabel = devTpLabel;
    }

    public Integer getAccTpOrder() {
        return accTpOrder;
    }

    public void setAccTpOrder(Integer accTpOrder) {
        this.accTpOrder = accTpOrder;
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

    public Boolean getIsSample() {
        return isSample;
    }

    public void setIsSample(Boolean isSample) {
        this.isSample = isSample;
    }

    public Integer getSampleDeadbande() {
        return sampleDeadbande;
    }

    public void setSampleDeadbande(Integer sampleDeadbande) {
        this.sampleDeadbande = sampleDeadbande;
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
}
