package com.avp.cdai.web.entity;

import javax.persistence.*;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/8/4.
 */
@Entity
@Table(name = "obj_line",schema = "afccd")
public class ObjLine {
    @Id
    private Integer id;
    @Column(name = "line_id")
    private Integer lineId;
    @Column(name = "line_name")
    private String lineName;
    @Column(name = "sync_time")
    private Date syncTime;
    @Transient
    private List<List<Long>> dataList1;
    @Transient
    private List<List<Long>> dataList2;
    @Transient
    private List<List<Long>> dataList3;
    public List<List<Long>> getDataList1() {
        return dataList1;
    }

    public void setDataList1(List<List<Long>> dataList1) {
        this.dataList1 = dataList1;
    }

    public List<List<Long>> getDataList2() {
        return dataList2;
    }

    public void setDataList2(List<List<Long>> dataList2) {
        this.dataList2 = dataList2;
    }

    public List<List<Long>> getDataList3() {
        return dataList3;
    }

    public void setDataList3(List<List<Long>> dataList3) {
        this.dataList3 = dataList3;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getId() {
        return id;
    }

    public Integer getLineId() {
        return lineId;
    }

    public void setLineId(Integer lineId) {
        this.lineId = lineId;
    }

    public String getLineName() {
        return lineName;
    }

    public void setLineName(String lineName) {
        this.lineName = lineName;
    }

    public Date getSyncTime() {
        return syncTime;
    }

    public void setSyncTime(Date syncTime) {
        this.syncTime = syncTime;
    }

}
