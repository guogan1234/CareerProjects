package com.avp.config.entity;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "formula_param_tbl", schema = "conf")
@IdClass(FormulaParamTblEntityPK.class)
public class FormulaParamTblEntity {
    private int formulaParamOrder;
    private String formulaLabel;
    private String formulaParamLabel;
    private Integer domainId;

    @Id
    @Column(name = "formula_param_order", nullable = false)
    public int getFormulaParamOrder() {
        return formulaParamOrder;
    }

    public void setFormulaParamOrder(int formulaParamOrder) {
        this.formulaParamOrder = formulaParamOrder;
    }

    @Id
    @Column(name = "formula_label", nullable = false, length = 32)
    public String getFormulaLabel() {
        return formulaLabel;
    }

    public void setFormulaLabel(String formulaLabel) {
        this.formulaLabel = formulaLabel;
    }

    @Basic
    @Column(name = "formula_param_label", nullable = true, length = 160)
    public String getFormulaParamLabel() {
        return formulaParamLabel;
    }

    public void setFormulaParamLabel(String formulaParamLabel) {
        this.formulaParamLabel = formulaParamLabel;
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
        FormulaParamTblEntity that = (FormulaParamTblEntity) o;
        return formulaParamOrder == that.formulaParamOrder &&
                Objects.equals(formulaLabel, that.formulaLabel) &&
                Objects.equals(formulaParamLabel, that.formulaParamLabel) &&
                Objects.equals(domainId, that.domainId);
    }

    @Override
    public int hashCode() {

        return Objects.hash(formulaParamOrder, formulaLabel, formulaParamLabel, domainId);
    }
}
