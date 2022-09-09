package com.avp.config.entity.user.vw;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "perm_group_realation_vw",schema = "conf")
public class PermGroupRealationVw {
    @Id
    private String relationId;
    private Integer groupId;
    private  String groupName;
    private Integer permId;
    private String permName;
    private  String permDefine;

    public String getRelationId() {
        return relationId;
    }

    public void setRelationId(String relationId) {
        this.relationId = relationId;
    }

    public Integer getGroupId() {
        return groupId;
    }

    public void setGroupId(Integer groupId) {
        this.groupId = groupId;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }

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

    public String getPermDefine() {
        return permDefine;
    }

    public void setPermDefine(String permDefine) {
        this.permDefine = permDefine;
    }
}
