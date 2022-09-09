package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * 模拟输出点表
 *
 */
@Entity
@Table(name = "ao_tbl",schema = "conf")
public class AoTbl {
	//模拟输出点标签
	@Id
	@Column(name = "ao_label",length = 128)
	private String aoLabel;

	//控制等待动作
	@Column(name = "wait_act")
	private Integer waitAct;

	//控制保留时间
	@Column(name = "resv_time")
	private Integer resvTime;

	//控制超时时间
	@Column(name = "exec_timeout")
	private Integer execTimeout;

	//点号
	@Column(name = "ao_no")
	private Integer aoNo;

	//控制数据格式
	@Column(name = "ao_format")
	private Integer aoFormat;

	//控制目标误差
	@Column(name = "ao_tolerance")
	private Float aoTolerance;

	//控制数据类型
	@Column(name = "data_type")
	private Integer dataType;

	//控制标度下限
	@Column(name = "bottom_scale")
	private Float bottomScale;

	//控制标度上限
	@Column(name = "top_scale")
	private Float topScale;

	//控制码下限
	@Column(name = "bottom_code")
	private Integer bottomCode;

	//控制码上限
	@Column(name = "top_code")
	private Integer topCode;

	//控制基值
	@Column(name = "ao_base")
	private Float aoBase;

	//控制系数
	@Column(name = "ao_coeff")
	private Float aoCoeff;

	//公式标签
	@Column(name = "formula_label",length = 32)
	private String formulaLabel;


	public String getAoLabel() {
		return aoLabel;
	}

	public void setAoLabel(String aoLabel) {
		this.aoLabel = aoLabel;
	}

	public Integer getWaitAct() {
		return waitAct;
	}

	public void setWaitAct(Integer waitAct) {
		this.waitAct = waitAct;
	}

	public Integer getResvTime() {
		return resvTime;
	}

	public void setResvTime(Integer resvTime) {
		this.resvTime = resvTime;
	}

	public Integer getExecTimeout() {
		return execTimeout;
	}

	public void setExecTimeout(Integer execTimeout) {
		this.execTimeout = execTimeout;
	}

	public Integer getAoNo() {
		return aoNo;
	}

	public void setAoNo(Integer aoNo) {
		this.aoNo = aoNo;
	}

	public Integer getAoFormat() {
		return aoFormat;
	}

	public void setAoFormat(Integer aoFormat) {
		this.aoFormat = aoFormat;
	}

	public Float getAoTolerance() {
		return aoTolerance;
	}

	public void setAoTolerance(Float aoTolerance) {
		this.aoTolerance = aoTolerance;
	}

	public Integer getDataType() {
		return dataType;
	}

	public void setDataType(Integer dataType) {
		this.dataType = dataType;
	}

	public Float getBottomScale() {
		return bottomScale;
	}

	public void setBottomScale(Float bottomScale) {
		this.bottomScale = bottomScale;
	}

	public Float getTopScale() {
		return topScale;
	}

	public void setTopScale(Float topScale) {
		this.topScale = topScale;
	}

	public Integer getBottomCode() {
		return bottomCode;
	}

	public void setBottomCode(Integer bottomCode) {
		this.bottomCode = bottomCode;
	}

	public Integer getTopCode() {
		return topCode;
	}

	public void setTopCode(Integer topCode) {
		this.topCode = topCode;
	}

	public Float getAoBase() {
		return aoBase;
	}

	public void setAoBase(Float aoBase) {
		this.aoBase = aoBase;
	}

	public Float getAoCoeff() {
		return aoCoeff;
	}

	public void setAoCoeff(Float aoCoeff) {
		this.aoCoeff = aoCoeff;
	}

	public String getFormulaLabel() {
		return formulaLabel;
	}

	public void setFormulaLabel(String formulaLabel) {
		this.formulaLabel = formulaLabel;
	}

}

