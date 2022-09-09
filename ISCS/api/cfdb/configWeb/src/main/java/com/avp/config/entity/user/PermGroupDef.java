package com.avp.config.entity.user;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "perm_group_def",schema = "conf")
public class PermGroupDef {
    @Id
    private  Integer groupId;
    private  String groupName;
    private  Integer groupLevel;
    private  String groupDesc;
    private  String groupPic1;
    private  String groupPic2;
    private  String groupPic3;

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

    public Integer getGroupLevel() {
        return groupLevel;
    }

    public void setGroupLevel(Integer groupLevel) {
        this.groupLevel = groupLevel;
    }

    public String getGroupDesc() {
        return groupDesc;
    }

    public void setGroupDesc(String groupDesc) {
        this.groupDesc = groupDesc;
    }

    public String getGroupPic1() {
        return groupPic1;
    }

    public void setGroupPic1(String groupPic1) {
        this.groupPic1 = groupPic1;
    }

    public String getGroupPic2() {
        return groupPic2;
    }

    public void setGroupPic2(String groupPic2) {
        this.groupPic2 = groupPic2;
    }

    public String getGroupPic3() {
        return groupPic3;
    }

    public void setGroupPic3(String groupPic3) {
        this.groupPic3 = groupPic3;
    }
}
