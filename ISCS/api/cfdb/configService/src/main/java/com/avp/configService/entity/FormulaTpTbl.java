package com.avp.configService.entity;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * 公式模版
 */
@Entity
@Table(name = "formula_tp_tbl",schema = "conf")
public class FormulaTpTbl {
    //模版标签
    @Id
    @Column(name = "formula_tp_label",length = 32)
    public String formulaTpLabel;
    //模版名称
    @Column(name = "formula_tp_name",length = 64)
    public String formulaTpName;
    //公式内容
    @Column(name = "formula_text",length = 4000)
    public String formulaText;

    public String getFormulaTpLabel() {
        return formulaTpLabel;
    }

    public void setFormulaTpLabel(String formulaTpLabel) {
        this.formulaTpLabel = formulaTpLabel;
    }

    public String getFormulaTpName() {
        return formulaTpName;
    }

    public void setFormulaTpName(String formulaTpName) {
        this.formulaTpName = formulaTpName;
    }

    public String getFormulaText() {
        return formulaText;
    }

    public void setFormulaText(String formulaText) {
        this.formulaText = formulaText;
    }
}




