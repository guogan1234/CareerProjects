package com.avp.config.entity.user;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "perm_type_def",schema = "conf")
public class PermTypeDef {
    @Id
    private Integer permTypeId;
    private  String permTypeName;

    public Integer getPermTypeId() {
        return permTypeId;
    }

    public void setPermTypeId(Integer permTypeId) {
        this.permTypeId = permTypeId;
    }

    public String getPermTypeName() {
        return permTypeName;
    }

    public void setPermTypeName(String permTypeName) {
        this.permTypeName = permTypeName;
    }
}
