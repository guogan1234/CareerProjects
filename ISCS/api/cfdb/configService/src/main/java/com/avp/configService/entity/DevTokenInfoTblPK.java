package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Objects;

public class DevTokenInfoTblPK implements Serializable {
    private String devLabel;
    private int tokenId;

    @Column(name = "dev_label", nullable = false, length = 96)
    @Id
    public String getDevLabel() {
        return devLabel;
    }

    public void setDevLabel(String devLabel) {
        this.devLabel = devLabel;
    }

    @Column(name = "token_id", nullable = false)
    @Id
    public int getTokenId() {
        return tokenId;
    }

    public void setTokenId(int tokenId) {
        this.tokenId = tokenId;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DevTokenInfoTblPK that = (DevTokenInfoTblPK) o;
        return tokenId == that.tokenId &&
                Objects.equals(devLabel, that.devLabel);
    }

    @Override
    public int hashCode() {

        return Objects.hash(devLabel, tokenId);
    }
}
