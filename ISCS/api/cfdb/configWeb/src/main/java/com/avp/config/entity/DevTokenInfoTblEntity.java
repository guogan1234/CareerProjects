package com.avp.config.entity;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "dev_token_info_tbl", schema = "conf")
@IdClass(DevTokenInfoTblEntityPK.class)
public class DevTokenInfoTblEntity {
    private String devLabel;
    private int tokenId;
    private Integer opTime;
    private String hostName;
    private Integer userId;
    private Integer userGrpId;
    private String description;
    private Integer stationId;
    private Integer domainId;

    @Id
    @Column(name = "dev_label", nullable = false, length = 96)
    public String getDevLabel() {
        return devLabel;
    }

    public void setDevLabel(String devLabel) {
        this.devLabel = devLabel;
    }

    @Id
    @Column(name = "token_id", nullable = false)
    public int getTokenId() {
        return tokenId;
    }

    public void setTokenId(int tokenId) {
        this.tokenId = tokenId;
    }

    @Basic
    @Column(name = "op_time", nullable = true)
    public Integer getOpTime() {
        return opTime;
    }

    public void setOpTime(Integer opTime) {
        this.opTime = opTime;
    }

    @Basic
    @Column(name = "host_name", nullable = true, length = 64)
    public String getHostName() {
        return hostName;
    }

    public void setHostName(String hostName) {
        this.hostName = hostName;
    }

    @Basic
    @Column(name = "user_id", nullable = true)
    public Integer getUserId() {
        return userId;
    }

    public void setUserId(Integer userId) {
        this.userId = userId;
    }

    @Basic
    @Column(name = "user_grp_id", nullable = true)
    public Integer getUserGrpId() {
        return userGrpId;
    }

    public void setUserGrpId(Integer userGrpId) {
        this.userGrpId = userGrpId;
    }

    @Basic
    @Column(name = "description", nullable = true, length = 128)
    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    @Basic
    @Column(name = "station_id", nullable = true)
    public Integer getStationId() {
        return stationId;
    }

    public void setStationId(Integer stationId) {
        this.stationId = stationId;
    }

    @Basic
    @Column(name = "domain_id", nullable = true)
    public Integer getDomainId() {
        return domainId;
    }

    public void setDomainId(Integer domainId) {
        this.domainId = domainId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DevTokenInfoTblEntity that = (DevTokenInfoTblEntity) o;
        return tokenId == that.tokenId &&
                Objects.equals(devLabel, that.devLabel) &&
                Objects.equals(opTime, that.opTime) &&
                Objects.equals(hostName, that.hostName) &&
                Objects.equals(userId, that.userId) &&
                Objects.equals(userGrpId, that.userGrpId) &&
                Objects.equals(description, that.description) &&
                Objects.equals(stationId, that.stationId) &&
                Objects.equals(domainId, that.domainId);
    }

    @Override
    public int hashCode() {

        return Objects.hash(devLabel, tokenId, opTime, hostName, userId, userGrpId, description, stationId, domainId);
    }
}
