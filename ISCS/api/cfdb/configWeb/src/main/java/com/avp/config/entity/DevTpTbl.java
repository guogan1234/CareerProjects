package com.avp.config.entity;

import java.sql.Timestamp;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * 设备模版model
 *
 */
@Entity
@Table(name = "dev_tp_tbl",schema = "conf")
public class DevTpTbl {
    //设备模版标签
    @Id
    @Column(name = "dev_tp_label",length = 32)
    private String devTpLabel;
    //设备模版名称
    @Column(name = "dev_tp_name",length = 64)
    private String devTpName;
    //设备类型标签
    @Column(name = "dev_type_label",length = 16)
    private String devTypeLabel;
    //所属专业系统唯一标识
    @Column(name = "pro_system_id")
    private Integer proSystemId;

    @Column(name = "operate_time")
    private Timestamp operateTime;

    public String getDevTpLabel() {
        return devTpLabel;
    }

    public void setDevTpLabel(String devTpLabel) {
        this.devTpLabel = devTpLabel;
    }

    public String getDevTpName() {
        return devTpName;
    }

    public void setDevTpName(String devTpName) {
        this.devTpName = devTpName;
    }

    public String getDevTypeLabel() {
        return devTypeLabel;
    }

    public void setDevTypeLabel(String devTypeLabel) {
        this.devTypeLabel = devTypeLabel;
    }

    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }

    public Timestamp getOperateTime() {
        return operateTime;
    }

    public void setOperateTime(Timestamp operateTime) {
        this.operateTime = operateTime;
    }
}
