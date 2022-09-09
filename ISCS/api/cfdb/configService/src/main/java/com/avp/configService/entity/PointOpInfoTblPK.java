package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Objects;

public class PointOpInfoTblPK implements Serializable {
    private String pointLabel;
    private int opType;

    @Column(name = "point_label", nullable = false, length = 128)
    @Id
    public String getPointLabel() {
        return pointLabel;
    }

    public void setPointLabel(String pointLabel) {
        this.pointLabel = pointLabel;
    }

    @Column(name = "op_type", nullable = false)
    @Id
    public int getOpType() {
        return opType;
    }

    public void setOpType(int opType) {
        this.opType = opType;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        PointOpInfoTblPK that = (PointOpInfoTblPK) o;
        return opType == that.opType &&
                Objects.equals(pointLabel, that.pointLabel);
    }

    @Override
    public int hashCode() {

        return Objects.hash(pointLabel, opType);
    }
}
