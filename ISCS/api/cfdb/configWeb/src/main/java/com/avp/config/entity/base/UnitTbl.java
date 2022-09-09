package com.avp.config.entity.base;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "unit_tbl", schema = "conf")
public class UnitTbl {
    @Id
    @Column(name = "unit_id")
    private Integer unitId;
    @Basic
    @Column(name = "unit_name",length = 32)
    private String unitName;


    public Integer getUnitId() {
        return unitId;
    }

    public void setUnitId(Integer unitId) {
        this.unitId = unitId;
    }


    public String getUnitName() {
        return unitName;
    }

    public void setUnitName(String unitName) {
        this.unitName = unitName;
    }

}
