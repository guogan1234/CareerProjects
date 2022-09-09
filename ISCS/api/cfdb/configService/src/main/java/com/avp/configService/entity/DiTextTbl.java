package com.avp.configService.entity;

import javax.persistence.*;
import java.io.Serializable;

/**
 * 数字量文本定义表
 * fxy
 */
@Entity
@Table(name = "di_text_tbl",schema = "conf")
@IdClass(DiTextTblPK.class)
public class DiTextTbl implements  Serializable {
    //数字量文本标签
    @Id
    @Column(name = "di_text_label",length = 32)
    private String diTextLabel;
    //实际值
    @Id
    @Column(name = "di_value")
    private Integer diValue;
    //显示文本
    @Column(name = "diText",length = 32)
    private String diText;
    //报警模式。后台根据报警模式来设置点的报警状态为报警或报警确认。需要发送给报警
    //服务。值及描述：0 -- 报警+事件；1 -- 报警返回（即报警恢复）+事件；2 -- 仅事
    //件； 3 -- 无
    @Column(name = "alarm_mode")
    private  Integer alarmMode;
    //所属专业系统ID
    @Column(name = "pro_system_id")
    private  Integer proSystemId;

    public String getDiTextLabel() {
        return diTextLabel;
    }

    public void setDiTextLabel(String diTextLabel) {
        this.diTextLabel = diTextLabel;
    }

    public Integer getDiValue() {
        return diValue;
    }

    public void setDiValue(Integer diValue) {
        this.diValue = diValue;
    }

    public String getDiText() {
        return diText;
    }

    public void setDiText(String diText) {
        this.diText = diText;
    }

    public Integer getAlarmMode() {
        return alarmMode;
    }

    public void setAlarmMode(Integer alarmMode) {
        this.alarmMode = alarmMode;
    }

    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }
}
