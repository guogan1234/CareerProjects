package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Objects;

public class DiTextTblPK implements Serializable {
    private String diTextLabel;
    private int diValue;

    @Column(name = "di_text_label", nullable = false, length = 32)
    @Id
    public String getDiTextLabel() {
        return diTextLabel;
    }

    public void setDiTextLabel(String diTextLabel) {
        this.diTextLabel = diTextLabel;
    }

    @Column(name = "di_value", nullable = false)
    @Id
    public int getDiValue() {
        return diValue;
    }

    public void setDiValue(int diValue) {
        this.diValue = diValue;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DiTextTblPK that = (DiTextTblPK) o;
        return diValue == that.diValue &&
                Objects.equals(diTextLabel, that.diTextLabel);
    }

    @Override
    public int hashCode() {

        return Objects.hash(diTextLabel, diValue);
    }
}
