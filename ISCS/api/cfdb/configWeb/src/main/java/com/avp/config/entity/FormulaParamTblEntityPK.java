package com.avp.config.entity;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Objects;

public class FormulaParamTblEntityPK implements Serializable {
    private int formulaParamOrder;
    private String formulaLabel;

    @Column(name = "formula_param_order", nullable = false)
    @Id
    public int getFormulaParamOrder() {
        return formulaParamOrder;
    }

    public void setFormulaParamOrder(int formulaParamOrder) {
        this.formulaParamOrder = formulaParamOrder;
    }

    @Column(name = "formula_label", nullable = false, length = 32)
    @Id
    public String getFormulaLabel() {
        return formulaLabel;
    }

    public void setFormulaLabel(String formulaLabel) {
        this.formulaLabel = formulaLabel;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        FormulaParamTblEntityPK that = (FormulaParamTblEntityPK) o;
        return formulaParamOrder == that.formulaParamOrder &&
                Objects.equals(formulaLabel, that.formulaLabel);
    }

    @Override
    public int hashCode() {

        return Objects.hash(formulaParamOrder, formulaLabel);
    }
}
