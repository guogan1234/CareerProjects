package com.avp.cdai.web.entity;

import javax.persistence.*;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/8/7.
 */
@Entity
@Table(name = "obj_station",schema = "afccd")
public class ObjStation {
    @Id
    private Integer id;
    @Column(name = "station_id")
    private Integer stationId;
    @Column(name = "station_name")
    private String stationName;
    @Column(name = "line_id")
    private Integer lineId;
    @Column(name = "sync_time")
    private Date syncTime;
    @Column(name = "longitude")
    private Double longitude;
    @Column(name = "latitude")
    private Double latitude;
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

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getStationId() {
        return stationId;
    }

    public void setStationId(Integer stationId) {
        this.stationId = stationId;
    }

    public String getStationName() {
        return stationName;
    }

    public void setStationName(String stationName) {
        this.stationName = stationName;
    }

    public Integer getLineId() {
        return lineId;
    }

    public void setLineId(Integer lineId) {
        this.lineId = lineId;
    }

    public Date getSyncTime() {
        return syncTime;
    }

    public void setSyncTime(Date syncTime) {
        this.syncTime = syncTime;
    }

    public Double getLongitude() {
        return longitude;
    }

    public void setLongitude(Double longitude) {
        this.longitude = longitude;
    }

    public Double getLatitude() {
        return latitude;
    }

    public void setLatitude(Double latitude) {
        this.latitude = latitude;
    }

    public ObjStation(){}

    public ObjStation(Integer id, Integer stationId, String stationName, Integer lineId, Date syncTime) {
        this.id = id;
        this.stationId = stationId;
        this.stationName = stationName;
        this.lineId = lineId;
        this.syncTime = syncTime;
    }
}
