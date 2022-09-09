package com.avp.config.entity;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "dev_node_tbl", schema = "conf")
public class DevNodeTblEntity {
    private String tagName;
    private Integer domainId;
    private Integer locationId;
    private Integer systemId;
    private Integer regionId;
    private Integer topologyColor;
    private Integer voltageId;
    private Integer nodeNo1;
    private Integer nodeNo1Status;
    private Integer nodeNo2;
    private Integer nodeNo2Status;
    private Integer nodeNo3;
    private Integer nodeNo3Status;
    private Integer nodeNo4;
    private Integer nodeNo4Status;
    private Integer nodeNo5;
    private Integer nodeNo5Status;
    private Integer nodeNo6;
    private Integer nodeNo6Status;
    private Integer nodeNo7;
    private Integer nodeNo7Status;
    private Integer nodeNo8;
    private Integer nodeNo8Status;
    private Integer nodeNo9;
    private Integer nodeNo9Status;
    private Integer nodeNo10;
    private Integer nodeNo10Status;

    @Id
    @Column(name = "tag_name", nullable = false, length = 32)
    public String getTagName() {
        return tagName;
    }

    public void setTagName(String tagName) {
        this.tagName = tagName;
    }

    @Basic
    @Column(name = "domain_id", nullable = true)
    public Integer getDomainId() {
        return domainId;
    }

    public void setDomainId(Integer domainId) {
        this.domainId = domainId;
    }

    @Basic
    @Column(name = "location_id", nullable = true)
    public Integer getLocationId() {
        return locationId;
    }

    public void setLocationId(Integer locationId) {
        this.locationId = locationId;
    }

    @Basic
    @Column(name = "system_id", nullable = true)
    public Integer getSystemId() {
        return systemId;
    }

    public void setSystemId(Integer systemId) {
        this.systemId = systemId;
    }

    @Basic
    @Column(name = "region_id", nullable = true)
    public Integer getRegionId() {
        return regionId;
    }

    public void setRegionId(Integer regionId) {
        this.regionId = regionId;
    }

    @Basic
    @Column(name = "topology_color", nullable = true)
    public Integer getTopologyColor() {
        return topologyColor;
    }

    public void setTopologyColor(Integer topologyColor) {
        this.topologyColor = topologyColor;
    }

    @Basic
    @Column(name = "voltage_id", nullable = true)
    public Integer getVoltageId() {
        return voltageId;
    }

    public void setVoltageId(Integer voltageId) {
        this.voltageId = voltageId;
    }

    @Basic
    @Column(name = "node_no1", nullable = true)
    public Integer getNodeNo1() {
        return nodeNo1;
    }

    public void setNodeNo1(Integer nodeNo1) {
        this.nodeNo1 = nodeNo1;
    }

    @Basic
    @Column(name = "node_no1status", nullable = true)
    public Integer getNodeNo1Status() {
        return nodeNo1Status;
    }

    public void setNodeNo1Status(Integer nodeNo1Status) {
        this.nodeNo1Status = nodeNo1Status;
    }

    @Basic
    @Column(name = "node_no2", nullable = true)
    public Integer getNodeNo2() {
        return nodeNo2;
    }

    public void setNodeNo2(Integer nodeNo2) {
        this.nodeNo2 = nodeNo2;
    }

    @Basic
    @Column(name = "node_no2status", nullable = true)
    public Integer getNodeNo2Status() {
        return nodeNo2Status;
    }

    public void setNodeNo2Status(Integer nodeNo2Status) {
        this.nodeNo2Status = nodeNo2Status;
    }

    @Basic
    @Column(name = "node_no3", nullable = true)
    public Integer getNodeNo3() {
        return nodeNo3;
    }

    public void setNodeNo3(Integer nodeNo3) {
        this.nodeNo3 = nodeNo3;
    }

    @Basic
    @Column(name = "node_no3status", nullable = true)
    public Integer getNodeNo3Status() {
        return nodeNo3Status;
    }

    public void setNodeNo3Status(Integer nodeNo3Status) {
        this.nodeNo3Status = nodeNo3Status;
    }

    @Basic
    @Column(name = "node_no4", nullable = true)
    public Integer getNodeNo4() {
        return nodeNo4;
    }

    public void setNodeNo4(Integer nodeNo4) {
        this.nodeNo4 = nodeNo4;
    }

    @Basic
    @Column(name = "node_no4status", nullable = true)
    public Integer getNodeNo4Status() {
        return nodeNo4Status;
    }

    public void setNodeNo4Status(Integer nodeNo4Status) {
        this.nodeNo4Status = nodeNo4Status;
    }

    @Basic
    @Column(name = "node_no5", nullable = true)
    public Integer getNodeNo5() {
        return nodeNo5;
    }

    public void setNodeNo5(Integer nodeNo5) {
        this.nodeNo5 = nodeNo5;
    }

    @Basic
    @Column(name = "node_no5status", nullable = true)
    public Integer getNodeNo5Status() {
        return nodeNo5Status;
    }

    public void setNodeNo5Status(Integer nodeNo5Status) {
        this.nodeNo5Status = nodeNo5Status;
    }

    @Basic
    @Column(name = "node_no6", nullable = true)
    public Integer getNodeNo6() {
        return nodeNo6;
    }

    public void setNodeNo6(Integer nodeNo6) {
        this.nodeNo6 = nodeNo6;
    }

    @Basic
    @Column(name = "node_no6status", nullable = true)
    public Integer getNodeNo6Status() {
        return nodeNo6Status;
    }

    public void setNodeNo6Status(Integer nodeNo6Status) {
        this.nodeNo6Status = nodeNo6Status;
    }

    @Basic
    @Column(name = "node_no7", nullable = true)
    public Integer getNodeNo7() {
        return nodeNo7;
    }

    public void setNodeNo7(Integer nodeNo7) {
        this.nodeNo7 = nodeNo7;
    }

    @Basic
    @Column(name = "node_no7status", nullable = true)
    public Integer getNodeNo7Status() {
        return nodeNo7Status;
    }

    public void setNodeNo7Status(Integer nodeNo7Status) {
        this.nodeNo7Status = nodeNo7Status;
    }

    @Basic
    @Column(name = "node_no8", nullable = true)
    public Integer getNodeNo8() {
        return nodeNo8;
    }

    public void setNodeNo8(Integer nodeNo8) {
        this.nodeNo8 = nodeNo8;
    }

    @Basic
    @Column(name = "node_no8status", nullable = true)
    public Integer getNodeNo8Status() {
        return nodeNo8Status;
    }

    public void setNodeNo8Status(Integer nodeNo8Status) {
        this.nodeNo8Status = nodeNo8Status;
    }

    @Basic
    @Column(name = "node_no9", nullable = true)
    public Integer getNodeNo9() {
        return nodeNo9;
    }

    public void setNodeNo9(Integer nodeNo9) {
        this.nodeNo9 = nodeNo9;
    }

    @Basic
    @Column(name = "node_no9status", nullable = true)
    public Integer getNodeNo9Status() {
        return nodeNo9Status;
    }

    public void setNodeNo9Status(Integer nodeNo9Status) {
        this.nodeNo9Status = nodeNo9Status;
    }

    @Basic
    @Column(name = "node_no10", nullable = true)
    public Integer getNodeNo10() {
        return nodeNo10;
    }

    public void setNodeNo10(Integer nodeNo10) {
        this.nodeNo10 = nodeNo10;
    }

    @Basic
    @Column(name = "node_no10status", nullable = true)
    public Integer getNodeNo10Status() {
        return nodeNo10Status;
    }

    public void setNodeNo10Status(Integer nodeNo10Status) {
        this.nodeNo10Status = nodeNo10Status;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DevNodeTblEntity that = (DevNodeTblEntity) o;
        return Objects.equals(tagName, that.tagName) &&
                Objects.equals(domainId, that.domainId) &&
                Objects.equals(locationId, that.locationId) &&
                Objects.equals(systemId, that.systemId) &&
                Objects.equals(regionId, that.regionId) &&
                Objects.equals(topologyColor, that.topologyColor) &&
                Objects.equals(voltageId, that.voltageId) &&
                Objects.equals(nodeNo1, that.nodeNo1) &&
                Objects.equals(nodeNo1Status, that.nodeNo1Status) &&
                Objects.equals(nodeNo2, that.nodeNo2) &&
                Objects.equals(nodeNo2Status, that.nodeNo2Status) &&
                Objects.equals(nodeNo3, that.nodeNo3) &&
                Objects.equals(nodeNo3Status, that.nodeNo3Status) &&
                Objects.equals(nodeNo4, that.nodeNo4) &&
                Objects.equals(nodeNo4Status, that.nodeNo4Status) &&
                Objects.equals(nodeNo5, that.nodeNo5) &&
                Objects.equals(nodeNo5Status, that.nodeNo5Status) &&
                Objects.equals(nodeNo6, that.nodeNo6) &&
                Objects.equals(nodeNo6Status, that.nodeNo6Status) &&
                Objects.equals(nodeNo7, that.nodeNo7) &&
                Objects.equals(nodeNo7Status, that.nodeNo7Status) &&
                Objects.equals(nodeNo8, that.nodeNo8) &&
                Objects.equals(nodeNo8Status, that.nodeNo8Status) &&
                Objects.equals(nodeNo9, that.nodeNo9) &&
                Objects.equals(nodeNo9Status, that.nodeNo9Status) &&
                Objects.equals(nodeNo10, that.nodeNo10) &&
                Objects.equals(nodeNo10Status, that.nodeNo10Status);
    }

    @Override
    public int hashCode() {

        return Objects.hash(tagName, domainId, locationId, systemId, regionId, topologyColor, voltageId, nodeNo1, nodeNo1Status, nodeNo2, nodeNo2Status, nodeNo3, nodeNo3Status, nodeNo4, nodeNo4Status, nodeNo5, nodeNo5Status, nodeNo6, nodeNo6Status, nodeNo7, nodeNo7Status, nodeNo8, nodeNo8Status, nodeNo9, nodeNo9Status, nodeNo10, nodeNo10Status);
    }
}
