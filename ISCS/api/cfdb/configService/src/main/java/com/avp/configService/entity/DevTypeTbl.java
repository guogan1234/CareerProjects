package com.avp.configService.entity;

import org.springframework.ui.Model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * 设备类型
 * fxy
 */
@Entity
@Table(name = "dev_type_tbl",schema = "conf")
public class DevTypeTbl {
    //'设备类型标签
    @Id
    @Column(name = "dev_type_label",length = 16)
    private  String devTypeLabel;

    //设备类型名称
    @Column(name = "dev_type_name",length = 32)
    private String devTypeName;
    //所属专业系统唯一标识
    @Column(name = "pro_system_id")
    private Integer proSystemId;

    public String getDevTypeLabel() {
        return devTypeLabel;
    }

    public void setDevTypeLabel(String devTypeLabel) {
        this.devTypeLabel = devTypeLabel;
    }

    public String getDevTypeName() {
        return devTypeName;
    }

    public void setDevTypeName(String devTypeName) {
        this.devTypeName = devTypeName;
    }

    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }
}
