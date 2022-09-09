package com.avp.configService.entity;

import javax.persistence.*;
import java.util.Objects;

public class CtrlTransGrpTbl {
    private String ctrlTransGrpLabel;
    private String ctrlTransGrpName;
    private Integer domainId;
    private Integer stationId;
    private Integer proSystemId;
    private Integer regionId;
    private String ctrlTransGrpPlabel;
    private Integer transState;
    private Integer transTimeout;
    private String exCmpLabel;
    private Integer exValue;

    public String getCtrlTransGrpLabel() {
        return ctrlTransGrpLabel;
    }

    public void setCtrlTransGrpLabel(String ctrlTransGrpLabel) {
        this.ctrlTransGrpLabel = ctrlTransGrpLabel;
    }

    public String getCtrlTransGrpName() {
        return ctrlTransGrpName;
    }

    public void setCtrlTransGrpName(String ctrlTransGrpName) {
        this.ctrlTransGrpName = ctrlTransGrpName;
    }

    public Integer getDomainId() {
        return domainId;
    }

    public void setDomainId(Integer domainId) {
        this.domainId = domainId;
    }

    public Integer getStationId() {
        return stationId;
    }

    public void setStationId(Integer stationId) {
        this.stationId = stationId;
    }

    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }

    public Integer getRegionId() {
        return regionId;
    }

    public void setRegionId(Integer regionId) {
        this.regionId = regionId;
    }

    public String getCtrlTransGrpPlabel() {
        return ctrlTransGrpPlabel;
    }

    public void setCtrlTransGrpPlabel(String ctrlTransGrpPlabel) {
        this.ctrlTransGrpPlabel = ctrlTransGrpPlabel;
    }

    public Integer getTransState() {
        return transState;
    }

    public void setTransState(Integer transState) {
        this.transState = transState;
    }

    public Integer getTransTimeout() {
        return transTimeout;
    }

    public void setTransTimeout(Integer transTimeout) {
        this.transTimeout = transTimeout;
    }

    public String getExCmpLabel() {
        return exCmpLabel;
    }

    public void setExCmpLabel(String exCmpLabel) {
        this.exCmpLabel = exCmpLabel;
    }

    public Integer getExValue() {
        return exValue;
    }

    public void setExValue(Integer exValue) {
        this.exValue = exValue;
    }
}
