package com.avp.config.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.sql.Timestamp;

/**
 * 离散输出点表
 *
 */
@Entity
@Table(name = "do_tbl",schema = "conf")
public class DoTbl {
	//离散输出点标签
	@Id
	@Column(name = "do_label",length = 128)
	private String doLabel;

	//离散输出组标签
	@Column(name = "do_grp_label",length = 32)
	private String doGrpLabel;

	//控制等待动作
	@Column(name = "wait_act")
	private Integer waitAct;

	//控制保留时间
	@Column(name = "resv_time")
	private Integer resvTime;

	//执行超时时间
	@Column(name = "exec_timeout")
	private Integer execTimeout;

	//脉冲保持时间
	@Column(name = "pulse_duration")
	private Integer pulseDuration;

	//输出点个数
	@Column(name = "do_num")
	private Integer doNum;

	//点号1
	@Column(name = "do_no1")
	private Integer doNo1;

	//点号2
	@Column(name = "do_no2")
	private Integer doNo2;

	//点号3
	@Column(name = "do_no3")
	private Integer doNo3;

	//点号4
	@Column(name = "do_no4")
	private Integer doNo4;

	//点号5
	@Column(name = "do_no5")
	private Integer doNo5;

	//公式标签
	@Column(name = "formula_label",length = 32)
	private String formulaLabel;

	@Column(name = "operate_time")
	private Timestamp operateTime;

	public String getDoLabel() {
		return doLabel;
	}

	public void setDoLabel(String doLabel) {
		this.doLabel = doLabel;
	}

	public String getDoGrpLabel() {
		return doGrpLabel;
	}

	public void setDoGrpLabel(String doGrpLabel) {
		this.doGrpLabel = doGrpLabel;
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

	public String getFormulaLabel() {
		return formulaLabel;
	}

	public void setFormulaLabel(String formulaLabel) {
		this.formulaLabel = formulaLabel;
	}

	public Timestamp getOperateTime() {
		return operateTime;
	}

	public void setOperateTime(Timestamp operateTime) {
		this.operateTime = operateTime;
	}
}
