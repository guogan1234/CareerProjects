package com.avp.config.entity;

public class PointEntity {
    private String pointId;

    private String pointName;

    private  String devId;

    private String type;

    private String devTpLabel;

    public String getPointId() {
        return pointId;
    }

    public void setPointId(String pointId) {
        this.pointId = pointId;
    }

    public String getPointName() {
        return pointName;
    }

    public void setPointName(String pointName) {
        this.pointName = pointName;
    }

    public String getDevId() {
        return devId;
    }

    public void setDevId(String devId) {
        this.devId = devId;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getDevTpLabel() {
        return devTpLabel;
    }

    public void setDevTpLabel(String devTpLabel) {
        this.devTpLabel = devTpLabel;
    }
}
