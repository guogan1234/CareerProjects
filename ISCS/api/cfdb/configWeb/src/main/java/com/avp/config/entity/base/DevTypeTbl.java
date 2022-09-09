package com.avp.config.entity.base;

import com.avp.config.entity.system.ProSystemTbl;
import com.fasterxml.jackson.annotation.JsonIgnore;
import org.hibernate.annotations.NotFound;
import org.hibernate.annotations.NotFoundAction;
import org.springframework.ui.Model;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

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
    private Integer proSystemId;

    @ManyToOne(fetch = FetchType.LAZY)
    @JoinColumn(name = "proSystemId", insertable = false, updatable = false)
    @JsonIgnore
    @NotFound(action = NotFoundAction.IGNORE)
    private ProSystemTbl proSystemTbl;


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

    public ProSystemTbl getProSystemTbl() {
        return proSystemTbl;
    }

    public void setProSystemTbl(ProSystemTbl proSystemTbl) {
        this.proSystemTbl = proSystemTbl;
    }
}
