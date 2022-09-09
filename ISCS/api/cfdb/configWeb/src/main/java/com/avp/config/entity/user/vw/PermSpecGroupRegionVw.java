package com.avp.config.entity.user.vw;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Created by zhoujs on 2018/6/28.
 */
@Entity
@Table(name = "perm_spec_group_region_vw", schema = "conf")
public class PermSpecGroupRegionVw {
    @Id
    private String relationId;
    private Integer groupId;
    private Integer permId;
    private Integer regionId;
    private String permDefine;

    private String permName;
    private String permTypeName;
    private Integer permTypeId;
    private String groupName;
    private Integer groupLevel;
    private String groupDesc;
    private String regionName;
    private String regionLabel;

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

    public Integer getPermId() {
        return permId;
    }

    public void setPermId(Integer permId) {
        this.permId = permId;
    }

    public Integer getRegionId() {
        return regionId;
    }

    public void setRegionId(Integer regionId) {
        this.regionId = regionId;
    }

    public String getPermDefine() {
        return permDefine;
    }

    public void setPermDefine(String permDefine) {
        this.permDefine = permDefine;
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

    public Integer getPermTypeId() {
        return permTypeId;
    }

    public void setPermTypeId(Integer permTypeId) {
        this.permTypeId = permTypeId;
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

    public String getRegionName() {
        return regionName;
    }

    public void setRegionName(String regionName) {
        this.regionName = regionName;
    }

    public String getRegionLabel() {
        return regionLabel;
    }

    public void setRegionLabel(String regionLabel) {
        this.regionLabel = regionLabel;
    }
}
