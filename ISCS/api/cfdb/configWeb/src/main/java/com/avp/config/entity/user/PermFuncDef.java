package com.avp.config.entity.user;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "perm_func_def",schema = "conf")
public class PermFuncDef {
    @Id
    private  Integer permId;
    private  String permName;
    private  Integer permTypeId;
    private  String permTypeName;
    private  String permDefine;

    public Integer getPermId() {
        return permId;
    }

    public void setPermId(Integer permId) {
        this.permId = permId;
    }

    public String getPermName() {
        return permName;
    }

    public void setPermName(String permName) {
        this.permName = permName;
    }

    public String getPermTypeName() {
        return permTypeName;
    }

    public void setPermTypeName(String permTypeName) {
        this.permTypeName = permTypeName;
    }

    public String getPermDefine() {
        return permDefine;
    }

    public void setPermDefine(String permDefine) {
        this.permDefine = permDefine;
    }

    public Integer getPermTypeId() {
        return permTypeId;
    }

    public void setPermTypeId(Integer permTypeId) {
        this.permTypeId = permTypeId;
    }
}
