package com.avp.config.entity;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "formula_grp_tbl", schema = "conf")
public class FormulaGrpTblEntity {
    private int formulaGrpId;
    private String formulaGrpName;

    @Id
    @Column(name = "formula_grp_id", nullable = false)
    public int getFormulaGrpId() {
        return formulaGrpId;
    }

    public void setFormulaGrpId(int formulaGrpId) {
        this.formulaGrpId = formulaGrpId;
    }

    @Basic
    @Column(name = "formula_grp_name", nullable = true, length = 32)
    public String getFormulaGrpName() {
        return formulaGrpName;
    }

    public void setFormulaGrpName(String formulaGrpName) {
        this.formulaGrpName = formulaGrpName;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        FormulaGrpTblEntity that = (FormulaGrpTblEntity) o;
        return formulaGrpId == that.formulaGrpId &&
                Objects.equals(formulaGrpName, that.formulaGrpName);
    }

    @Override
    public int hashCode() {

        return Objects.hash(formulaGrpId, formulaGrpName);
    }
}
