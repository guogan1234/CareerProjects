package com.avp.config.entity.system;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Date;

@Entity
@Table(name = "sys_node_info",schema = "conf")
public class SysNodeInfo {
    @Id
    @Column(name = "node_id")
    private  Integer nodeId;
    @Column(name = "node_name",length = 40)
    private String nodeName;
    @Column(name = "node_name_cn",length = 64)
    private String nodeNameCn;
    @Column(name = "node_type")
    private  Integer nodeType;
    @Column(name = "node_status")
    private  Integer nodeStatus;
    @Column(name = "nic_num")
    private Integer nicNum;
    @Column(name = "nic1_name",length = 64)
    private  String nic1Name;
    @Column(name = "nic1_address",length = 16)
    private String nic1Address;
    @Column(name = "nic1_mask",length = 16)
    private String nic1Mask;
    @Column(name = "net1_status")
    private  Integer net1Status;
    @Column(name = "nic2_name",length = 64)
    private  String nic2Name;
    @Column(name = "nic2_address",length = 16)
    private String nic2Address;
    @Column(name = "nic2_mask",length = 16)
    private String nic2Mask;
    @Column(name = "net2_status")
    private  Integer net2Status;
    @Column(name = "nic3_name",length = 64)
    private  String nic3Name;
    @Column(name = "nic3_address",length = 16)
    private String nic3Address;
    @Column(name = "nic3_mask",length = 16)
    private String nic3Mask;
    @Column(name = "net3_status")
    private  Integer net3Status;
    @Column(name = "nic4_name",length = 64)
    private  String nic4Name;
    @Column(name = "nic4_address",length = 16)
    private String nic4Address;
    @Column(name = "nic4_mask",length = 16)
    private String nic4Mask;
    @Column(name = "net4_status")
    private  Integer net4Status;
    @Column(name = "net_status")
    private Integer netStatus;
    @Column(name = "node_dev",length = 64)
    private String nodeDev;
    @Column(name = "node_os",length = 40)
    private String nodeOs;
    @Column(name = "cpu_used")
    private Integer cpuUsed;
    @Column(name = "mem_used")
    private Integer memUsed;
    @Column(name = "last_report_time")
    private Date  lastReportTime;
    @Column(name = "location_id")
    private Integer locationId;
    @Column(name = "actual_run_domain_id")
    private Integer actualRunDomainId;
    @Column(name = "start_order")
    private Integer startOrder;

    public Integer getNodeId() {
        return nodeId;
    }

    public void setNodeId(Integer nodeId) {
        this.nodeId = nodeId;
    }

    public String getNodeName() {
        return nodeName;
    }

    public void setNodeName(String nodeName) {
        this.nodeName = nodeName;
    }

    public String getNodeNameCn() {
        return nodeNameCn;
    }

    public void setNodeNameCn(String nodeNameCn) {
        this.nodeNameCn = nodeNameCn;
    }

    public Integer getNodeType() {
        return nodeType;
    }

    public void setNodeType(Integer nodeType) {
        this.nodeType = nodeType;
    }

    public Integer getNodeStatus() {
        return nodeStatus;
    }

    public void setNodeStatus(Integer nodeStatus) {
        this.nodeStatus = nodeStatus;
    }

    public Integer getNicNum() {
        return nicNum;
    }

    public void setNicNum(Integer nicNum) {
        this.nicNum = nicNum;
    }

    public String getNic1Name() {
        return nic1Name;
    }

    public void setNic1Name(String nic1Name) {
        this.nic1Name = nic1Name;
    }

    public String getNic1Address() {
        return nic1Address;
    }

    public void setNic1Address(String nic1Address) {
        this.nic1Address = nic1Address;
    }

    public String getNic1Mask() {
        return nic1Mask;
    }

    public void setNic1Mask(String nic1Mask) {
        this.nic1Mask = nic1Mask;
    }

    public Integer getNet1Status() {
        return net1Status;
    }

    public void setNet1Status(Integer net1Status) {
        this.net1Status = net1Status;
    }

    public String getNic2Name() {
        return nic2Name;
    }

    public void setNic2Name(String nic2Name) {
        this.nic2Name = nic2Name;
    }

    public String getNic2Address() {
        return nic2Address;
    }

    public void setNic2Address(String nic2Address) {
        this.nic2Address = nic2Address;
    }

    public String getNic2Mask() {
        return nic2Mask;
    }

    public void setNic2Mask(String nic2Mask) {
        this.nic2Mask = nic2Mask;
    }

    public Integer getNet2Status() {
        return net2Status;
    }

    public void setNet2Status(Integer net2Status) {
        this.net2Status = net2Status;
    }

    public String getNic3Name() {
        return nic3Name;
    }

    public void setNic3Name(String nic3Name) {
        this.nic3Name = nic3Name;
    }

    public String getNic3Address() {
        return nic3Address;
    }

    public void setNic3Address(String nic3Address) {
        this.nic3Address = nic3Address;
    }

    public String getNic3Mask() {
        return nic3Mask;
    }

    public void setNic3Mask(String nic3Mask) {
        this.nic3Mask = nic3Mask;
    }

    public Integer getNet3Status() {
        return net3Status;
    }

    public void setNet3Status(Integer net3Status) {
        this.net3Status = net3Status;
    }

    public String getNic4Name() {
        return nic4Name;
    }

    public void setNic4Name(String nic4Name) {
        this.nic4Name = nic4Name;
    }

    public String getNic4Address() {
        return nic4Address;
    }

    public void setNic4Address(String nic4Address) {
        this.nic4Address = nic4Address;
    }

    public String getNic4Mask() {
        return nic4Mask;
    }

    public void setNic4Mask(String nic4Mask) {
        this.nic4Mask = nic4Mask;
    }

    public Integer getNet4Status() {
        return net4Status;
    }

    public void setNet4Status(Integer net4Status) {
        this.net4Status = net4Status;
    }

    public Integer getNetStatus() {
        return netStatus;
    }

    public void setNetStatus(Integer netStatus) {
        this.netStatus = netStatus;
    }

    public String getNodeDev() {
        return nodeDev;
    }

    public void setNodeDev(String nodeDev) {
        this.nodeDev = nodeDev;
    }

    public String getNodeOs() {
        return nodeOs;
    }

    public void setNodeOs(String nodeOs) {
        this.nodeOs = nodeOs;
    }

    public Integer getCpuUsed() {
        return cpuUsed;
    }

    public void setCpuUsed(Integer cpuUsed) {
        this.cpuUsed = cpuUsed;
    }

    public Integer getMemUsed() {
        return memUsed;
    }

    public void setMemUsed(Integer memUsed) {
        this.memUsed = memUsed;
    }

    public Date getLastReportTime() {
        return lastReportTime;
    }

    public void setLastReportTime(Date lastReportTime) {
        this.lastReportTime = lastReportTime;
    }

    public Integer getLocationId() {
        return locationId;
    }

    public void setLocationId(Integer locationId) {
        this.locationId = locationId;
    }

    public Integer getActualRunDomainId() {
        return actualRunDomainId;
    }

    public void setActualRunDomainId(Integer actualRunDomainId) {
        this.actualRunDomainId = actualRunDomainId;
    }

    public Integer getStartOrder() {
        return startOrder;
    }

    public void setStartOrder(Integer startOrder) {
        this.startOrder = startOrder;
    }
}
