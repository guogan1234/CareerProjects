package com.avp.config.entity;

import javax.persistence.*;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
@Table(name = "dio_def_tbl", schema = "conf")
public class DioDefTblEntity {
    private String dioDefLabel;
    private String pointTpLabel;
    private String devTypeLabel;
    private Integer domainId;
    private Integer locationId;
    private Integer proSystemId;
    private Integer bitNum;
    private String channelTag1;
    private Integer dot1No1;
    private Integer dot1No2;
    private Integer dot1No3;
    private Integer dot1No4;
    private Integer dot1No5;
    private Boolean filterErr1;
    private Boolean filterDisturb1;
    private Integer disturbTime1;
    private Integer polarity1;
    private String channelTag2;
    private Integer dot2No1;
    private Integer dot2No2;
    private Integer dot2No3;
    private Integer dot2No4;
    private Integer dot2No5;
    private Boolean filterErr2;
    private Boolean filterDisturb2;
    private Integer disturbTime2;
    private Integer dioPolarity2;
    private String channelTag3;
    private Integer dot3No1;
    private Integer dot3No2;
    private Integer dot3No3;
    private Integer dot3No4;
    private Integer dot3No5;
    private Boolean filterErr3;
    private Boolean filterDisturb3;
    private Integer disturbTime3;
    private Integer dioPolarity3;
    private String channelTag4;
    private Integer dot4No1;
    private Integer dot4No2;
    private Integer dot4No3;
    private Integer dot4No4;
    private Integer dot4No5;
    private Boolean filterErr4;
    private Boolean filterDisturb4;
    private Integer disturbTime4;
    private Integer dioPolarity4;
    @Column(name = "operate_time")
    private Timestamp operateTime;

    @Id
    @Column(name = "dio_def_label", nullable = false, length = 64)
    public String getDioDefLabel() {
        return dioDefLabel;
    }

    public void setDioDefLabel(String dioDefLabel) {
        this.dioDefLabel = dioDefLabel;
    }

    @Basic
    @Column(name = "point_tp_label", nullable = true, length = 32)
    public String getPointTpLabel() {
        return pointTpLabel;
    }

    public void setPointTpLabel(String pointTpLabel) {
        this.pointTpLabel = pointTpLabel;
    }

    @Basic
    @Column(name = "dev_type_label", nullable = true, length = 15)
    public String getDevTypeLabel() {
        return devTypeLabel;
    }

    public void setDevTypeLabel(String devTypeLabel) {
        this.devTypeLabel = devTypeLabel;
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
    @Column(name = "pro_system_id", nullable = true)
    public Integer getProSystemId() {
        return proSystemId;
    }

    public void setProSystemId(Integer proSystemId) {
        this.proSystemId = proSystemId;
    }

    @Basic
    @Column(name = "bit_num", nullable = true)
    public Integer getBitNum() {
        return bitNum;
    }

    public void setBitNum(Integer bitNum) {
        this.bitNum = bitNum;
    }

    @Basic
    @Column(name = "channel_tag1", nullable = true, length = 64)
    public String getChannelTag1() {
        return channelTag1;
    }

    public void setChannelTag1(String channelTag1) {
        this.channelTag1 = channelTag1;
    }

    @Basic
    @Column(name = "dot1_no1", nullable = true)
    public Integer getDot1No1() {
        return dot1No1;
    }

    public void setDot1No1(Integer dot1No1) {
        this.dot1No1 = dot1No1;
    }

    @Basic
    @Column(name = "dot1_no2", nullable = true)
    public Integer getDot1No2() {
        return dot1No2;
    }

    public void setDot1No2(Integer dot1No2) {
        this.dot1No2 = dot1No2;
    }

    @Basic
    @Column(name = "dot1_no3", nullable = true)
    public Integer getDot1No3() {
        return dot1No3;
    }

    public void setDot1No3(Integer dot1No3) {
        this.dot1No3 = dot1No3;
    }

    @Basic
    @Column(name = "dot1_no4", nullable = true)
    public Integer getDot1No4() {
        return dot1No4;
    }

    public void setDot1No4(Integer dot1No4) {
        this.dot1No4 = dot1No4;
    }

    @Basic
    @Column(name = "dot1_no5", nullable = true)
    public Integer getDot1No5() {
        return dot1No5;
    }

    public void setDot1No5(Integer dot1No5) {
        this.dot1No5 = dot1No5;
    }

    @Basic
    @Column(name = "filter_err1", nullable = true)
    public Boolean getFilterErr1() {
        return filterErr1;
    }

    public void setFilterErr1(Boolean filterErr1) {
        this.filterErr1 = filterErr1;
    }

    @Basic
    @Column(name = "filter_disturb1", nullable = true)
    public Boolean getFilterDisturb1() {
        return filterDisturb1;
    }

    public void setFilterDisturb1(Boolean filterDisturb1) {
        this.filterDisturb1 = filterDisturb1;
    }

    @Basic
    @Column(name = "disturb_time1", nullable = true)
    public Integer getDisturbTime1() {
        return disturbTime1;
    }

    public void setDisturbTime1(Integer disturbTime1) {
        this.disturbTime1 = disturbTime1;
    }

    @Basic
    @Column(name = "polarity1", nullable = true)
    public Integer getPolarity1() {
        return polarity1;
    }

    public void setPolarity1(Integer polarity1) {
        this.polarity1 = polarity1;
    }

    @Basic
    @Column(name = "channel_tag2", nullable = true, length = 1)
    public String getChannelTag2() {
        return channelTag2;
    }

    public void setChannelTag2(String channelTag2) {
        this.channelTag2 = channelTag2;
    }

    @Basic
    @Column(name = "dot2_no1", nullable = true)
    public Integer getDot2No1() {
        return dot2No1;
    }

    public void setDot2No1(Integer dot2No1) {
        this.dot2No1 = dot2No1;
    }

    @Basic
    @Column(name = "dot2_no2", nullable = true)
    public Integer getDot2No2() {
        return dot2No2;
    }

    public void setDot2No2(Integer dot2No2) {
        this.dot2No2 = dot2No2;
    }

    @Basic
    @Column(name = "dot2_no3", nullable = true)
    public Integer getDot2No3() {
        return dot2No3;
    }

    public void setDot2No3(Integer dot2No3) {
        this.dot2No3 = dot2No3;
    }

    @Basic
    @Column(name = "dot2_no4", nullable = true)
    public Integer getDot2No4() {
        return dot2No4;
    }

    public void setDot2No4(Integer dot2No4) {
        this.dot2No4 = dot2No4;
    }

    @Basic
    @Column(name = "dot2_no5", nullable = true)
    public Integer getDot2No5() {
        return dot2No5;
    }

    public void setDot2No5(Integer dot2No5) {
        this.dot2No5 = dot2No5;
    }

    @Basic
    @Column(name = "filter_err2", nullable = true)
    public Boolean getFilterErr2() {
        return filterErr2;
    }

    public void setFilterErr2(Boolean filterErr2) {
        this.filterErr2 = filterErr2;
    }

    @Basic
    @Column(name = "filter_disturb2", nullable = true)
    public Boolean getFilterDisturb2() {
        return filterDisturb2;
    }

    public void setFilterDisturb2(Boolean filterDisturb2) {
        this.filterDisturb2 = filterDisturb2;
    }

    @Basic
    @Column(name = "disturb_time2", nullable = true)
    public Integer getDisturbTime2() {
        return disturbTime2;
    }

    public void setDisturbTime2(Integer disturbTime2) {
        this.disturbTime2 = disturbTime2;
    }

    @Basic
    @Column(name = "dio_polarity2", nullable = true)
    public Integer getDioPolarity2() {
        return dioPolarity2;
    }

    public void setDioPolarity2(Integer dioPolarity2) {
        this.dioPolarity2 = dioPolarity2;
    }

    @Basic
    @Column(name = "channel_tag3", nullable = true, length = 64)
    public String getChannelTag3() {
        return channelTag3;
    }

    public void setChannelTag3(String channelTag3) {
        this.channelTag3 = channelTag3;
    }

    @Basic
    @Column(name = "dot3_no1", nullable = true)
    public Integer getDot3No1() {
        return dot3No1;
    }

    public void setDot3No1(Integer dot3No1) {
        this.dot3No1 = dot3No1;
    }

    @Basic
    @Column(name = "dot3_no2", nullable = true)
    public Integer getDot3No2() {
        return dot3No2;
    }

    public void setDot3No2(Integer dot3No2) {
        this.dot3No2 = dot3No2;
    }

    @Basic
    @Column(name = "dot3_no3", nullable = true)
    public Integer getDot3No3() {
        return dot3No3;
    }

    public void setDot3No3(Integer dot3No3) {
        this.dot3No3 = dot3No3;
    }

    @Basic
    @Column(name = "dot3_no4", nullable = true)
    public Integer getDot3No4() {
        return dot3No4;
    }

    public void setDot3No4(Integer dot3No4) {
        this.dot3No4 = dot3No4;
    }

    @Basic
    @Column(name = "dot3_no5", nullable = true)
    public Integer getDot3No5() {
        return dot3No5;
    }

    public void setDot3No5(Integer dot3No5) {
        this.dot3No5 = dot3No5;
    }

    @Basic
    @Column(name = "filter_err3", nullable = true)
    public Boolean getFilterErr3() {
        return filterErr3;
    }

    public void setFilterErr3(Boolean filterErr3) {
        this.filterErr3 = filterErr3;
    }

    @Basic
    @Column(name = "filter_disturb3", nullable = true)
    public Boolean getFilterDisturb3() {
        return filterDisturb3;
    }

    public void setFilterDisturb3(Boolean filterDisturb3) {
        this.filterDisturb3 = filterDisturb3;
    }

    @Basic
    @Column(name = "disturb_time3", nullable = true)
    public Integer getDisturbTime3() {
        return disturbTime3;
    }

    public void setDisturbTime3(Integer disturbTime3) {
        this.disturbTime3 = disturbTime3;
    }

    @Basic
    @Column(name = "dio_polarity3", nullable = true)
    public Integer getDioPolarity3() {
        return dioPolarity3;
    }

    public void setDioPolarity3(Integer dioPolarity3) {
        this.dioPolarity3 = dioPolarity3;
    }

    @Basic
    @Column(name = "channel_tag4", nullable = true, length = 64)
    public String getChannelTag4() {
        return channelTag4;
    }

    public void setChannelTag4(String channelTag4) {
        this.channelTag4 = channelTag4;
    }

    @Basic
    @Column(name = "dot4_no1", nullable = true)
    public Integer getDot4No1() {
        return dot4No1;
    }

    public void setDot4No1(Integer dot4No1) {
        this.dot4No1 = dot4No1;
    }

    @Basic
    @Column(name = "dot4_no2", nullable = true)
    public Integer getDot4No2() {
        return dot4No2;
    }

    public void setDot4No2(Integer dot4No2) {
        this.dot4No2 = dot4No2;
    }

    @Basic
    @Column(name = "dot4_no3", nullable = true)
    public Integer getDot4No3() {
        return dot4No3;
    }

    public void setDot4No3(Integer dot4No3) {
        this.dot4No3 = dot4No3;
    }

    @Basic
    @Column(name = "dot4_no4", nullable = true)
    public Integer getDot4No4() {
        return dot4No4;
    }

    public void setDot4No4(Integer dot4No4) {
        this.dot4No4 = dot4No4;
    }

    @Basic
    @Column(name = "dot4_no5", nullable = true)
    public Integer getDot4No5() {
        return dot4No5;
    }

    public void setDot4No5(Integer dot4No5) {
        this.dot4No5 = dot4No5;
    }

    @Basic
    @Column(name = "filter_err4", nullable = true)
    public Boolean getFilterErr4() {
        return filterErr4;
    }

    public void setFilterErr4(Boolean filterErr4) {
        this.filterErr4 = filterErr4;
    }

    @Basic
    @Column(name = "filter_disturb4", nullable = true)
    public Boolean getFilterDisturb4() {
        return filterDisturb4;
    }

    public void setFilterDisturb4(Boolean filterDisturb4) {
        this.filterDisturb4 = filterDisturb4;
    }

    @Basic
    @Column(name = "disturb_time4", nullable = true)
    public Integer getDisturbTime4() {
        return disturbTime4;
    }

    public void setDisturbTime4(Integer disturbTime4) {
        this.disturbTime4 = disturbTime4;
    }

    @Basic
    @Column(name = "dio_polarity4", nullable = true)
    public Integer getDioPolarity4() {
        return dioPolarity4;
    }

    public void setDioPolarity4(Integer dioPolarity4) {
        this.dioPolarity4 = dioPolarity4;
    }

    public Timestamp getOperateTime() {
        return operateTime;
    }

    public void setOperateTime(Timestamp operateTime) {
        this.operateTime = operateTime;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DioDefTblEntity that = (DioDefTblEntity) o;
        return Objects.equals(dioDefLabel, that.dioDefLabel) &&
                Objects.equals(pointTpLabel, that.pointTpLabel) &&
                Objects.equals(devTypeLabel, that.devTypeLabel) &&
                Objects.equals(domainId, that.domainId) &&
                Objects.equals(locationId, that.locationId) &&
                Objects.equals(proSystemId, that.proSystemId) &&
                Objects.equals(bitNum, that.bitNum) &&
                Objects.equals(channelTag1, that.channelTag1) &&
                Objects.equals(dot1No1, that.dot1No1) &&
                Objects.equals(dot1No2, that.dot1No2) &&
                Objects.equals(dot1No3, that.dot1No3) &&
                Objects.equals(dot1No4, that.dot1No4) &&
                Objects.equals(dot1No5, that.dot1No5) &&
                Objects.equals(filterErr1, that.filterErr1) &&
                Objects.equals(filterDisturb1, that.filterDisturb1) &&
                Objects.equals(disturbTime1, that.disturbTime1) &&
                Objects.equals(polarity1, that.polarity1) &&
                Objects.equals(channelTag2, that.channelTag2) &&
                Objects.equals(dot2No1, that.dot2No1) &&
                Objects.equals(dot2No2, that.dot2No2) &&
                Objects.equals(dot2No3, that.dot2No3) &&
                Objects.equals(dot2No4, that.dot2No4) &&
                Objects.equals(dot2No5, that.dot2No5) &&
                Objects.equals(filterErr2, that.filterErr2) &&
                Objects.equals(filterDisturb2, that.filterDisturb2) &&
                Objects.equals(disturbTime2, that.disturbTime2) &&
                Objects.equals(dioPolarity2, that.dioPolarity2) &&
                Objects.equals(channelTag3, that.channelTag3) &&
                Objects.equals(dot3No1, that.dot3No1) &&
                Objects.equals(dot3No2, that.dot3No2) &&
                Objects.equals(dot3No3, that.dot3No3) &&
                Objects.equals(dot3No4, that.dot3No4) &&
                Objects.equals(dot3No5, that.dot3No5) &&
                Objects.equals(filterErr3, that.filterErr3) &&
                Objects.equals(filterDisturb3, that.filterDisturb3) &&
                Objects.equals(disturbTime3, that.disturbTime3) &&
                Objects.equals(dioPolarity3, that.dioPolarity3) &&
                Objects.equals(channelTag4, that.channelTag4) &&
                Objects.equals(dot4No1, that.dot4No1) &&
                Objects.equals(dot4No2, that.dot4No2) &&
                Objects.equals(dot4No3, that.dot4No3) &&
                Objects.equals(dot4No4, that.dot4No4) &&
                Objects.equals(dot4No5, that.dot4No5) &&
                Objects.equals(filterErr4, that.filterErr4) &&
                Objects.equals(filterDisturb4, that.filterDisturb4) &&
                Objects.equals(disturbTime4, that.disturbTime4) &&
                Objects.equals(dioPolarity4, that.dioPolarity4);
    }

    @Override
    public int hashCode() {

        return Objects.hash(dioDefLabel, pointTpLabel, devTypeLabel, domainId, locationId, proSystemId, bitNum, channelTag1, dot1No1, dot1No2, dot1No3, dot1No4, dot1No5, filterErr1, filterDisturb1, disturbTime1, polarity1, channelTag2, dot2No1, dot2No2, dot2No3, dot2No4, dot2No5, filterErr2, filterDisturb2, disturbTime2, dioPolarity2, channelTag3, dot3No1, dot3No2, dot3No3, dot3No4, dot3No5, filterErr3, filterDisturb3, disturbTime3, dioPolarity3, channelTag4, dot4No1, dot4No2, dot4No3, dot4No4, dot4No5, filterErr4, filterDisturb4, disturbTime4, dioPolarity4);
    }
}
