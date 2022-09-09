package com.avp.config.entity;

import javax.persistence.*;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
@Table(name = "aio_def_tbl", schema = "conf")
public class AioDefTblEntity {
    private String aioDefLabel;
    private Integer domainId;
    private Integer locationId;
    private Integer proSystemId;
    private String pointTpLabel;
    private String devTypeLabel;
    private String channelTag1;
    private String channelTag2;
    private String channelTag3;
    private String channelTag4;
    private Integer dotNo1;
    private Integer dotNo2;
    private Integer dotNo3;
    private Integer dotNo4;
    private Integer aioType1;
    private Integer aioType2;
    private Integer aioType3;
    private Integer aioType4;
    private Boolean isFilter1;
    private Boolean isFilter2;
    private Boolean isFilter3;
    private Boolean isFilter4;
    private Integer aioPercent1;
    private Integer aioPercent2;
    private Integer aioPercent3;
    private Integer aioPercent4;
    private Boolean isValid1;
    private Boolean isValid2;
    private Boolean isValid3;
    private Boolean isValid4;
    private Float aioDeadband1;
    private Float aioDeadband2;
    private Float aioDeadband3;
    private Float aioDeadband4;
    private Float aioZeroband1;
    private Float aioZeroband2;
    private Float aioZeroband3;
    private Float aioZeroband4;
    private Float aioBase1;
    private Float aioBase2;
    private Float aioBase3;
    private Float aioBase4;
    private Float aioCoeff1;
    private Float aioCoeff2;
    private Float aioCoeff3;
    private Float aioCoeff4;
    private Float bottomScale1;
    private Float bottomScale2;
    private Float bottomScale3;
    private Float bottomScale4;
    private Float topScale1;
    private Float topScale2;
    private Float topScale3;
    private Float topScale4;
    private Integer bottomCode1;
    private Integer bottomCode2;
    private Integer bottomCode3;
    private Integer bottomCode4;
    private Integer topCode1;
    private Integer topCode2;
    private Integer topCode3;
    private Integer topCode4;

    private Timestamp operateTime;

    @Id
    @Column(name = "aio_def_label", nullable = false, length = 64)
    public String getAioDefLabel() {
        return aioDefLabel;
    }

    public void setAioDefLabel(String aioDefLabel) {
        this.aioDefLabel = aioDefLabel;
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
    @Column(name = "point_tp_label", nullable = true, length = 32)
    public String getPointTpLabel() {
        return pointTpLabel;
    }

    public void setPointTpLabel(String pointTpLabel) {
        this.pointTpLabel = pointTpLabel;
    }

    @Basic
    @Column(name = "dev_type_label", nullable = true, length = 16)
    public String getDevTypeLabel() {
        return devTypeLabel;
    }

    public void setDevTypeLabel(String devTypeLabel) {
        this.devTypeLabel = devTypeLabel;
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
    @Column(name = "aio_type1", nullable = true)
    public Integer getAioType1() {
        return aioType1;
    }

    public void setAioType1(Integer aioType1) {
        this.aioType1 = aioType1;
    }

    @Basic
    @Column(name = "aio_type2", nullable = true)
    public Integer getAioType2() {
        return aioType2;
    }

    public void setAioType2(Integer aioType2) {
        this.aioType2 = aioType2;
    }

    @Basic
    @Column(name = "aio_type3", nullable = true)
    public Integer getAioType3() {
        return aioType3;
    }

    public void setAioType3(Integer aioType3) {
        this.aioType3 = aioType3;
    }

    @Basic
    @Column(name = "aio_type4", nullable = true)
    public Integer getAioType4() {
        return aioType4;
    }

    public void setAioType4(Integer aioType4) {
        this.aioType4 = aioType4;
    }

    @Basic
    @Column(name = "is_filter1", nullable = true)
    public Boolean getIsFilter1() {
        return isFilter1;
    }

    public void setIsFilter1(Boolean isFilter1) {
        this.isFilter1 = isFilter1;
    }

    @Basic
    @Column(name = "is_filter2", nullable = true)
    public Boolean getIsFilter2() {
        return isFilter2;
    }

    public void setIsFilter2(Boolean isFilter2) {
        this.isFilter2 = isFilter2;
    }

    @Basic
    @Column(name = "is_filter3", nullable = true)
    public Boolean getIsFilter3() {
        return isFilter3;
    }

    public void setIsFilter3(Boolean isFilter3) {
        this.isFilter3 = isFilter3;
    }

    @Basic
    @Column(name = "is_filter4", nullable = true)
    public Boolean getIsFilter4() {
        return isFilter4;
    }

    public void setIsFilter4(Boolean isFilter4) {
        this.isFilter4 = isFilter4;
    }

    @Basic
    @Column(name = "aio_percent1", nullable = true)
    public Integer getAioPercent1() {
        return aioPercent1;
    }

    public void setAioPercent1(Integer aioPercent1) {
        this.aioPercent1 = aioPercent1;
    }

    @Basic
    @Column(name = "aio_percent2", nullable = true)
    public Integer getAioPercent2() {
        return aioPercent2;
    }

    public void setAioPercent2(Integer aioPercent2) {
        this.aioPercent2 = aioPercent2;
    }

    @Basic
    @Column(name = "aio_percent3", nullable = true)
    public Integer getAioPercent3() {
        return aioPercent3;
    }

    public void setAioPercent3(Integer aioPercent3) {
        this.aioPercent3 = aioPercent3;
    }

    @Basic
    @Column(name = "aio_percent4", nullable = true)
    public Integer getAioPercent4() {
        return aioPercent4;
    }

    public void setAioPercent4(Integer aioPercent4) {
        this.aioPercent4 = aioPercent4;
    }

    @Basic
    @Column(name = "is_valid1", nullable = true)
    public Boolean getIsValid1() {
        return isValid1;
    }

    public void setIsValid1(Boolean isValid1) {
        this.isValid1 = isValid1;
    }

    @Basic
    @Column(name = "is_valid2", nullable = true)
    public Boolean getIsValid2() {
        return isValid2;
    }

    public void setIsValid2(Boolean isValid2) {
        this.isValid2 = isValid2;
    }

    @Basic
    @Column(name = "is_valid3", nullable = true)
    public Boolean getIsValid3() {
        return isValid3;
    }

    public void setIsValid3(Boolean isValid3) {
        this.isValid3 = isValid3;
    }

    @Basic
    @Column(name = "is_valid4", nullable = true)
    public Boolean getIsValid4() {
        return isValid4;
    }

    public void setIsValid4(Boolean isValid4) {
        this.isValid4 = isValid4;
    }

    @Basic
    @Column(name = "aio_deadband1", nullable = true, precision = 0)
    public Float getAioDeadband1() {
        return aioDeadband1;
    }

    public void setAioDeadband1(Float aioDeadband1) {
        this.aioDeadband1 = aioDeadband1;
    }

    @Basic
    @Column(name = "aio_deadband2", nullable = true, precision = 0)
    public Float getAioDeadband2() {
        return aioDeadband2;
    }

    public void setAioDeadband2(Float aioDeadband2) {
        this.aioDeadband2 = aioDeadband2;
    }

    @Basic
    @Column(name = "aio_deadband3", nullable = true, precision = 0)
    public Float getAioDeadband3() {
        return aioDeadband3;
    }

    public void setAioDeadband3(Float aioDeadband3) {
        this.aioDeadband3 = aioDeadband3;
    }

    @Basic
    @Column(name = "aio_deadband4", nullable = true, precision = 0)
    public Float getAioDeadband4() {
        return aioDeadband4;
    }

    public void setAioDeadband4(Float aioDeadband4) {
        this.aioDeadband4 = aioDeadband4;
    }

    @Basic
    @Column(name = "aio_zeroband1", nullable = true, precision = 0)
    public Float getAioZeroband1() {
        return aioZeroband1;
    }

    public void setAioZeroband1(Float aioZeroband1) {
        this.aioZeroband1 = aioZeroband1;
    }

    @Basic
    @Column(name = "aio_zeroband2", nullable = true, precision = 0)
    public Float getAioZeroband2() {
        return aioZeroband2;
    }

    public void setAioZeroband2(Float aioZeroband2) {
        this.aioZeroband2 = aioZeroband2;
    }

    @Basic
    @Column(name = "aio_zeroband3", nullable = true, precision = 0)
    public Float getAioZeroband3() {
        return aioZeroband3;
    }

    public void setAioZeroband3(Float aioZeroband3) {
        this.aioZeroband3 = aioZeroband3;
    }

    @Basic
    @Column(name = "aio_zeroband4", nullable = true, precision = 0)
    public Float getAioZeroband4() {
        return aioZeroband4;
    }

    public void setAioZeroband4(Float aioZeroband4) {
        this.aioZeroband4 = aioZeroband4;
    }

    @Basic
    @Column(name = "aio_base1", nullable = true, precision = 0)
    public Float getAioBase1() {
        return aioBase1;
    }

    public void setAioBase1(Float aioBase1) {
        this.aioBase1 = aioBase1;
    }

    @Basic
    @Column(name = "aio_base2", nullable = true, precision = 0)
    public Float getAioBase2() {
        return aioBase2;
    }

    public void setAioBase2(Float aioBase2) {
        this.aioBase2 = aioBase2;
    }

    @Basic
    @Column(name = "aio_base3", nullable = true, precision = 0)
    public Float getAioBase3() {
        return aioBase3;
    }

    public void setAioBase3(Float aioBase3) {
        this.aioBase3 = aioBase3;
    }

    @Basic
    @Column(name = "aio_base4", nullable = true, precision = 0)
    public Float getAioBase4() {
        return aioBase4;
    }

    public void setAioBase4(Float aioBase4) {
        this.aioBase4 = aioBase4;
    }

    @Basic
    @Column(name = "aio_coeff1", nullable = true, precision = 0)
    public Float getAioCoeff1() {
        return aioCoeff1;
    }

    public void setAioCoeff1(Float aioCoeff1) {
        this.aioCoeff1 = aioCoeff1;
    }

    @Basic
    @Column(name = "aio_coeff2", nullable = true, precision = 0)
    public Float getAioCoeff2() {
        return aioCoeff2;
    }

    public void setAioCoeff2(Float aioCoeff2) {
        this.aioCoeff2 = aioCoeff2;
    }

    @Basic
    @Column(name = "aio_coeff3", nullable = true, precision = 0)
    public Float getAioCoeff3() {
        return aioCoeff3;
    }

    public void setAioCoeff3(Float aioCoeff3) {
        this.aioCoeff3 = aioCoeff3;
    }

    @Basic
    @Column(name = "aio_coeff4", nullable = true, precision = 0)
    public Float getAioCoeff4() {
        return aioCoeff4;
    }

    public void setAioCoeff4(Float aioCoeff4) {
        this.aioCoeff4 = aioCoeff4;
    }

    @Basic
    @Column(name = "bottom_scale1", nullable = true, precision = 0)
    public Float getBottomScale1() {
        return bottomScale1;
    }

    public void setBottomScale1(Float bottomScale1) {
        this.bottomScale1 = bottomScale1;
    }

    @Basic
    @Column(name = "bottom_scale2", nullable = true, precision = 0)
    public Float getBottomScale2() {
        return bottomScale2;
    }

    public void setBottomScale2(Float bottomScale2) {
        this.bottomScale2 = bottomScale2;
    }

    @Basic
    @Column(name = "bottom_scale3", nullable = true, precision = 0)
    public Float getBottomScale3() {
        return bottomScale3;
    }

    public void setBottomScale3(Float bottomScale3) {
        this.bottomScale3 = bottomScale3;
    }

    @Basic
    @Column(name = "bottom_scale4", nullable = true, precision = 0)
    public Float getBottomScale4() {
        return bottomScale4;
    }

    public void setBottomScale4(Float bottomScale4) {
        this.bottomScale4 = bottomScale4;
    }

    @Basic
    @Column(name = "top_scale1", nullable = true, precision = 0)
    public Float getTopScale1() {
        return topScale1;
    }

    public void setTopScale1(Float topScale1) {
        this.topScale1 = topScale1;
    }

    @Basic
    @Column(name = "top_scale2", nullable = true, precision = 0)
    public Float getTopScale2() {
        return topScale2;
    }

    public void setTopScale2(Float topScale2) {
        this.topScale2 = topScale2;
    }

    @Basic
    @Column(name = "top_scale3", nullable = true, precision = 0)
    public Float getTopScale3() {
        return topScale3;
    }

    public void setTopScale3(Float topScale3) {
        this.topScale3 = topScale3;
    }

    @Basic
    @Column(name = "top_scale4", nullable = true, precision = 0)
    public Float getTopScale4() {
        return topScale4;
    }

    public void setTopScale4(Float topScale4) {
        this.topScale4 = topScale4;
    }

    @Basic
    @Column(name = "bottom_code1", nullable = true)
    public Integer getBottomCode1() {
        return bottomCode1;
    }

    public void setBottomCode1(Integer bottomCode1) {
        this.bottomCode1 = bottomCode1;
    }

    @Basic
    @Column(name = "bottom_code2", nullable = true)
    public Integer getBottomCode2() {
        return bottomCode2;
    }

    public void setBottomCode2(Integer bottomCode2) {
        this.bottomCode2 = bottomCode2;
    }

    @Basic
    @Column(name = "bottom_code3", nullable = true)
    public Integer getBottomCode3() {
        return bottomCode3;
    }

    public void setBottomCode3(Integer bottomCode3) {
        this.bottomCode3 = bottomCode3;
    }

    @Basic
    @Column(name = "bottom_code4", nullable = true)
    public Integer getBottomCode4() {
        return bottomCode4;
    }

    public void setBottomCode4(Integer bottomCode4) {
        this.bottomCode4 = bottomCode4;
    }

    @Basic
    @Column(name = "top_code1", nullable = true)
    public Integer getTopCode1() {
        return topCode1;
    }

    public void setTopCode1(Integer topCode1) {
        this.topCode1 = topCode1;
    }

    @Basic
    @Column(name = "top_code2", nullable = true)
    public Integer getTopCode2() {
        return topCode2;
    }

    public void setTopCode2(Integer topCode2) {
        this.topCode2 = topCode2;
    }

    @Basic
    @Column(name = "top_code3", nullable = true)
    public Integer getTopCode3() {
        return topCode3;
    }

    public void setTopCode3(Integer topCode3) {
        this.topCode3 = topCode3;
    }

    @Basic
    @Column(name = "top_code4", nullable = true)
    public Integer getTopCode4() {
        return topCode4;
    }

    public void setTopCode4(Integer topCode4) {
        this.topCode4 = topCode4;
    }
    @Basic
    @Column(name = "operate_time")
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
        AioDefTblEntity that = (AioDefTblEntity) o;
        return Objects.equals(aioDefLabel, that.aioDefLabel) &&
                Objects.equals(domainId, that.domainId) &&
                Objects.equals(locationId, that.locationId) &&
                Objects.equals(proSystemId, that.proSystemId) &&
                Objects.equals(pointTpLabel, that.pointTpLabel) &&
                Objects.equals(devTypeLabel, that.devTypeLabel) &&
                Objects.equals(channelTag1, that.channelTag1) &&
                Objects.equals(channelTag2, that.channelTag2) &&
                Objects.equals(channelTag3, that.channelTag3) &&
                Objects.equals(channelTag4, that.channelTag4) &&
                Objects.equals(dotNo1, that.dotNo1) &&
                Objects.equals(dotNo2, that.dotNo2) &&
                Objects.equals(dotNo3, that.dotNo3) &&
                Objects.equals(dotNo4, that.dotNo4) &&
                Objects.equals(aioType1, that.aioType1) &&
                Objects.equals(aioType2, that.aioType2) &&
                Objects.equals(aioType3, that.aioType3) &&
                Objects.equals(aioType4, that.aioType4) &&
                Objects.equals(isFilter1, that.isFilter1) &&
                Objects.equals(isFilter2, that.isFilter2) &&
                Objects.equals(isFilter3, that.isFilter3) &&
                Objects.equals(isFilter4, that.isFilter4) &&
                Objects.equals(aioPercent1, that.aioPercent1) &&
                Objects.equals(aioPercent2, that.aioPercent2) &&
                Objects.equals(aioPercent3, that.aioPercent3) &&
                Objects.equals(aioPercent4, that.aioPercent4) &&
                Objects.equals(isValid1, that.isValid1) &&
                Objects.equals(isValid2, that.isValid2) &&
                Objects.equals(isValid3, that.isValid3) &&
                Objects.equals(isValid4, that.isValid4) &&
                Objects.equals(aioDeadband1, that.aioDeadband1) &&
                Objects.equals(aioDeadband2, that.aioDeadband2) &&
                Objects.equals(aioDeadband3, that.aioDeadband3) &&
                Objects.equals(aioDeadband4, that.aioDeadband4) &&
                Objects.equals(aioZeroband1, that.aioZeroband1) &&
                Objects.equals(aioZeroband2, that.aioZeroband2) &&
                Objects.equals(aioZeroband3, that.aioZeroband3) &&
                Objects.equals(aioZeroband4, that.aioZeroband4) &&
                Objects.equals(aioBase1, that.aioBase1) &&
                Objects.equals(aioBase2, that.aioBase2) &&
                Objects.equals(aioBase3, that.aioBase3) &&
                Objects.equals(aioBase4, that.aioBase4) &&
                Objects.equals(aioCoeff1, that.aioCoeff1) &&
                Objects.equals(aioCoeff2, that.aioCoeff2) &&
                Objects.equals(aioCoeff3, that.aioCoeff3) &&
                Objects.equals(aioCoeff4, that.aioCoeff4) &&
                Objects.equals(bottomScale1, that.bottomScale1) &&
                Objects.equals(bottomScale2, that.bottomScale2) &&
                Objects.equals(bottomScale3, that.bottomScale3) &&
                Objects.equals(bottomScale4, that.bottomScale4) &&
                Objects.equals(topScale1, that.topScale1) &&
                Objects.equals(topScale2, that.topScale2) &&
                Objects.equals(topScale3, that.topScale3) &&
                Objects.equals(topScale4, that.topScale4) &&
                Objects.equals(bottomCode1, that.bottomCode1) &&
                Objects.equals(bottomCode2, that.bottomCode2) &&
                Objects.equals(bottomCode3, that.bottomCode3) &&
                Objects.equals(bottomCode4, that.bottomCode4) &&
                Objects.equals(topCode1, that.topCode1) &&
                Objects.equals(topCode2, that.topCode2) &&
                Objects.equals(topCode3, that.topCode3) &&
                Objects.equals(topCode4, that.topCode4) &&
                Objects.equals(operateTime, that.operateTime);
    }

    @Override
    public int hashCode() {

        return Objects.hash(aioDefLabel, domainId, locationId, proSystemId, pointTpLabel, devTypeLabel, channelTag1, channelTag2, channelTag3, channelTag4, dotNo1, dotNo2, dotNo3, dotNo4, aioType1, aioType2, aioType3, aioType4, isFilter1, isFilter2, isFilter3, isFilter4, aioPercent1, aioPercent2, aioPercent3, aioPercent4, isValid1, isValid2, isValid3, isValid4, aioDeadband1, aioDeadband2, aioDeadband3, aioDeadband4, aioZeroband1, aioZeroband2, aioZeroband3, aioZeroband4, aioBase1, aioBase2, aioBase3, aioBase4, aioCoeff1, aioCoeff2, aioCoeff3, aioCoeff4, bottomScale1, bottomScale2, bottomScale3, bottomScale4, topScale1, topScale2, topScale3, topScale4, bottomCode1, bottomCode2, bottomCode3, bottomCode4, topCode1, topCode2, topCode3, topCode4,operateTime);
    }
}
