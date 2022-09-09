package com.avp.config.entity.user.vw;

import javax.persistence.*;

/**
 * Created by zhoujs on 2018/6/26.
 */
@Entity
@Table(name = "user_group_vw",schema = "conf")
public class UserGroupVw {

    @Id
    private  String relationId;
    private  Integer userId;
    private  Integer groupId;
    private  Integer ifLead;
    private String userName;
    private String groupName;

    public String getRelationId() {
        return relationId;
    }

    public void setRelationId(String relationId) {
        this.relationId = relationId;
    }

    public Integer getUserId() {
        return userId;
    }

    public void setUserId(Integer userId) {
        this.userId = userId;
    }

    public Integer getGroupId() {
        return groupId;
    }

    public void setGroupId(Integer groupId) {
        this.groupId = groupId;
    }

    public Integer getIfLead() {
        return ifLead;
    }

    public void setIfLead(Integer ifLead) {
        this.ifLead = ifLead;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getGroupName() {
        return groupName;
    }

    public void setGroupName(String groupName) {
        this.groupName = groupName;
    }
}
