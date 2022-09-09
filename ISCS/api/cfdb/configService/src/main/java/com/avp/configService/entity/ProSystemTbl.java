package com.avp.configService.entity;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * 设备模版查询专业model
 * fxy
 */
@Entity
@Table(name = "pro_system_tbl",schema = "conf")
public class ProSystemTbl {
    //专业id
    @Id
    @Column(name = "pro_system_id")
    private Integer proSystemId;
   //专业标签
    @Column(name = "pro_system_label",length = 16)
    private String proSystemLabel;
    //专业名称
    @Column(name = "pro_system_name",length = 32)
    private  String proSystemName;

    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }

    public String getProSystemLabel() {
        return proSystemLabel;
    }

    public void setProSystemLabel(String proSystemLabel) {
        this.proSystemLabel = proSystemLabel;
    }

    public String getProSystemName() {
        return proSystemName;
    }

    public void setProSystemName(String proSystemName) {
        this.proSystemName = proSystemName;
    }
}




