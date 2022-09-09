package com.avp.cdai.web.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Date;

/**
 * Created by guo on 2017/9/4.
 */
@Entity
@Table(name = "equipment_subtype",schema = "afccd")
public class EquipmentSubType {
    @Id
    private Integer id;
    @Column(name = "equipment_subtype_id")
    private Integer equipmentSubType;
    @Column(name = "equipment_type_id")
    private Integer equipmentType;
    @Column(name = "name")
    private String equipmentName;
    @Column(name = "short_name")
    private String shortName;
    @Column(name = "sync_time")
    private Date syncTime;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getEquipmentSubType() {
        return equipmentSubType;
    }

    public void setEquipmentSubType(Integer equipmentSubType) {
        this.equipmentSubType = equipmentSubType;
    }

    public Integer getEquipmentType() {
        return equipmentType;
    }

    public void setEquipmentType(Integer equipmentType) {
        this.equipmentType = equipmentType;
    }

    public String getEquipmentName() {
        return equipmentName;
    }

    public void setEquipmentName(String equipmentName) {
        this.equipmentName = equipmentName;
    }

    public String getShortName() {
        return shortName;
    }

    public void setShortName(String shortName) {
        this.shortName = shortName;
    }

    public Date getSyncTime() {
        return syncTime;
    }

    public void setSyncTime(Date syncTime) {
        this.syncTime = syncTime;
    }
}
