package com.avp.configService.entity;

import javax.persistence.*;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
@Table(name = "acc_def_tbl", schema = "conf")
public class AccDefTbl {
    private String accDefLabel;
    @Column(name = "point_tp_label",length = 32)
    private String pointTpLabel;
    private Integer domainId;
    private Integer locationId;
    private Integer proSystemId;
    private String devTypeLabel;
    private Integer channelNum;
    private String channelTag1;
    private String channelTag2;
    private String channelTag3;
    private String channelTag4;
    private Integer dotNo1;
    private Integer dotNo2;
    private Integer dotNo3;
    private Integer dotNo4;
    private Integer accBase1;
    private Integer accBase2;
    private Integer accBase3;
    private Integer accBase4;
    private Integer accCoeff1;
    private Integer accCoeff2;
    private Integer accCoeff3;
    private Integer accCoeff4;


    @Id
    @Column(name = "acc_def_label", nullable = false, length = 64)
    public String getAccDefLabel() {
        return accDefLabel;
    }

    public void setAccDefLabel(String accDefLabel) {
        this.accDefLabel = accDefLabel;
    }

    public String getPointTpLabel() {
        return pointTpLabel;
    }

    public void setPointTpLabel(String pointTpLabel) {
        this.pointTpLabel = pointTpLabel;
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
    @Column(name = "dev_type_label", nullable = true,length = 16)
    public String getDevTypeLabel() {
        return devTypeLabel;
    }

    public void setDevTypeLabel(String devTypeLabel) {
        this.devTypeLabel = devTypeLabel;
    }

    @Basic
    @Column(name = "channel_num", nullable = true)
    public Integer getChannelNum() {
        return channelNum;
    }

    public void setChannelNum(Integer channelNum) {
        this.channelNum = channelNum;
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
    @Column(name = "channel_tag2", nullable = true, length = 64)
    public String getChannelTag2() {
        return channelTag2;
    }

    public void setChannelTag2(String channelTag2) {
        this.channelTag2 = channelTag2;
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
    @Column(name = "channel_tag4", nullable = true, length = 64)
    public String getChannelTag4() {
        return channelTag4;
    }

    public void setChannelTag4(String channelTag4) {
        this.channelTag4 = channelTag4;
    }

    @Basic
    @Column(name = "dot_no1", nullable = true)
    public Integer getDotNo1() {
        return dotNo1;
    }

    public void setDotNo1(Integer dotNo1) {
        this.dotNo1 = dotNo1;
    }

    @Basic
    @Column(name = "dot_no2", nullable = true)
    public Integer getDotNo2() {
        return dotNo2;
    }

    public void setDotNo2(Integer dotNo2) {
        this.dotNo2 = dotNo2;
    }

    @Basic
    @Column(name = "dot_no3", nullable = true)
    public Integer getDotNo3() {
        return dotNo3;
    }

    public void setDotNo3(Integer dotNo3) {
        this.dotNo3 = dotNo3;
    }

    @Basic
    @Column(name = "dot_no4", nullable = true)
    public Integer getDotNo4() {
        return dotNo4;
    }

    public void setDotNo4(Integer dotNo4) {
        this.dotNo4 = dotNo4;
    }

    @Basic
    @Column(name = "acc_base1", nullable = true)
    public Integer getAccBase1() {
        return accBase1;
    }

    public void setAccBase1(Integer accBase1) {
        this.accBase1 = accBase1;
    }

    @Basic
    @Column(name = "acc_base2", nullable = true)
    public Integer getAccBase2() {
        return accBase2;
    }

    public void setAccBase2(Integer accBase2) {
        this.accBase2 = accBase2;
    }

    @Basic
    @Column(name = "acc_base3", nullable = true)
    public Integer getAccBase3() {
        return accBase3;
    }

    public void setAccBase3(Integer accBase3) {
        this.accBase3 = accBase3;
    }

    @Basic
    @Column(name = "acc_base4", nullable = true)
    public Integer getAccBase4() {
        return accBase4;
    }

    public void setAccBase4(Integer accBase4) {
        this.accBase4 = accBase4;
    }

    @Basic
    @Column(name = "acc_coeff1", nullable = true)
    public Integer getAccCoeff1() {
        return accCoeff1;
    }

    public void setAccCoeff1(Integer accCoeff1) {
        this.accCoeff1 = accCoeff1;
    }

    @Basic
    @Column(name = "acc_coeff2", nullable = true)
    public Integer getAccCoeff2() {
        return accCoeff2;
    }

    public void setAccCoeff2(Integer accCoeff2) {
        this.accCoeff2 = accCoeff2;
    }

    @Basic
    @Column(name = "acc_coeff3", nullable = true)
    public Integer getAccCoeff3() {
        return accCoeff3;
    }

    public void setAccCoeff3(Integer accCoeff3) {
        this.accCoeff3 = accCoeff3;
    }

    @Basic
    @Column(name = "acc_coeff4", nullable = true)
    public Integer getAccCoeff4() {
        return accCoeff4;
    }

    public void setAccCoeff4(Integer accCoeff4) {
        this.accCoeff4 = accCoeff4;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        AccDefTbl that = (AccDefTbl) o;
        return Objects.equals(accDefLabel, that.accDefLabel) &&
                Objects.equals(pointTpLabel, that.pointTpLabel) &&
                Objects.equals(domainId, that.domainId) &&
                Objects.equals(locationId, that.locationId) &&
                Objects.equals(proSystemId, that.proSystemId) &&
                Objects.equals(devTypeLabel, that.devTypeLabel) &&
                Objects.equals(channelNum, that.channelNum) &&
                Objects.equals(channelTag1, that.channelTag1) &&
                Objects.equals(channelTag2, that.channelTag2) &&
                Objects.equals(channelTag3, that.channelTag3) &&
                Objects.equals(channelTag4, that.channelTag4) &&
                Objects.equals(dotNo1, that.dotNo1) &&
                Objects.equals(dotNo2, that.dotNo2) &&
                Objects.equals(dotNo3, that.dotNo3) &&
                Objects.equals(dotNo4, that.dotNo4) &&
                Objects.equals(accBase1, that.accBase1) &&
                Objects.equals(accBase2, that.accBase2) &&
                Objects.equals(accBase3, that.accBase3) &&
                Objects.equals(accBase4, that.accBase4) &&
                Objects.equals(accCoeff1, that.accCoeff1) &&
                Objects.equals(accCoeff2, that.accCoeff2) &&
                Objects.equals(accCoeff3, that.accCoeff3) &&
                Objects.equals(accCoeff4, that.accCoeff4);
    }

    @Override
    public int hashCode() {

        return Objects.hash(accDefLabel, pointTpLabel, domainId, locationId, proSystemId, devTypeLabel, channelNum, channelTag1, channelTag2, channelTag3, channelTag4, dotNo1, dotNo2, dotNo3, dotNo4, accBase1, accBase2, accBase3, accBase4, accCoeff1, accCoeff2, accCoeff3, accCoeff4);
    }
}
