package com.avp.config.entity;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "ctrl_trans_grp_tbl", schema = "conf")
public class CtrlTransGrpTblEntity {
    private String ctrlTransGrpLabel;
    private String ctrlTransGrpName;
    private int domainId;
    private int stationId;
    private int proSystemId;
    private int regionId;
    private String ctrlTransGrpPlabel;
    private int transState;
    private Integer transTimeout;
    private String exCmpLabel;
    private int exValue;

    @Id
    @Column(name = "ctrl_trans_grp_label", nullable = false, length = 32)
    public String getCtrlTransGrpLabel() {
        return ctrlTransGrpLabel;
    }

    public void setCtrlTransGrpLabel(String ctrlTransGrpLabel) {
        this.ctrlTransGrpLabel = ctrlTransGrpLabel;
    }

    @Basic
    @Column(name = "ctrl_trans_grp_name", nullable = false, length = 64)
    public String getCtrlTransGrpName() {
        return ctrlTransGrpName;
    }

    public void setCtrlTransGrpName(String ctrlTransGrpName) {
        this.ctrlTransGrpName = ctrlTransGrpName;
    }

    @Basic
    @Column(name = "domain_id", nullable = false)
    public int getDomainId() {
        return domainId;
    }

    public void setDomainId(int domainId) {
        this.domainId = domainId;
    }

    @Basic
    @Column(name = "station_id", nullable = false)
    public int getStationId() {
        return stationId;
    }

    public void setStationId(int stationId) {
        this.stationId = stationId;
    }

    @Basic
    @Column(name = "pro_system_id", nullable = false)
    public int getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(int proSystemId) {
        this.proSystemId = proSystemId;
    }

    @Basic
    @Column(name = "region_id", nullable = false)
    public int getRegionId() {
        return regionId;
    }

    public void setRegionId(int regionId) {
        this.regionId = regionId;
    }

    @Basic
    @Column(name = "ctrl_trans_grp_plabel", nullable = true, length = 32)
    public String getCtrlTransGrpPlabel() {
        return ctrlTransGrpPlabel;
    }

    public void setCtrlTransGrpPlabel(String ctrlTransGrpPlabel) {
        this.ctrlTransGrpPlabel = ctrlTransGrpPlabel;
    }

    @Basic
    @Column(name = "trans_state", nullable = false)
    public int getTransState() {
        return transState;
    }

    public void setTransState(int transState) {
        this.transState = transState;
    }

    @Basic
    @Column(name = "trans_timeout", nullable = true)
    public Integer getTransTimeout() {
        return transTimeout;
    }

    public void setTransTimeout(Integer transTimeout) {
        this.transTimeout = transTimeout;
    }

    @Basic
    @Column(name = "ex_cmp_label", nullable = true, length = 160)
    public String getExCmpLabel() {
        return exCmpLabel;
    }

    public void setExCmpLabel(String exCmpLabel) {
        this.exCmpLabel = exCmpLabel;
    }

    @Basic
    @Column(name = "ex_value", nullable = false)
    public int getExValue() {
        return exValue;
    }

    public void setExValue(int exValue) {
        this.exValue = exValue;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CtrlTransGrpTblEntity that = (CtrlTransGrpTblEntity) o;
        return domainId == that.domainId &&
                stationId == that.stationId &&
                proSystemId == that.proSystemId &&
                regionId == that.regionId &&
                transState == that.transState &&
                exValue == that.exValue &&
                Objects.equals(ctrlTransGrpLabel, that.ctrlTransGrpLabel) &&
                Objects.equals(ctrlTransGrpName, that.ctrlTransGrpName) &&
                Objects.equals(ctrlTransGrpPlabel, that.ctrlTransGrpPlabel) &&
                Objects.equals(transTimeout, that.transTimeout) &&
                Objects.equals(exCmpLabel, that.exCmpLabel);
    }

    @Override
    public int hashCode() {

        return Objects.hash(ctrlTransGrpLabel, ctrlTransGrpName, domainId, stationId, proSystemId, regionId, ctrlTransGrpPlabel, transState, transTimeout, exCmpLabel, exValue);
    }
}
