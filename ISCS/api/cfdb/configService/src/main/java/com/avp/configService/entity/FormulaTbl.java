package com.avp.configService.entity;

public class FormulaTbl {
    private String formulaLabel;
    private Integer formulaType;
    private  Integer triggerMode;
    private String description;
    private Integer cycleTime;
    private Integer paramNum;
    private String formulaTpLabel;
    private String resultLabel;
    private Integer execOrder;
    private  Integer formulaGrpId;
    private Integer domainId;

    public String getFormulaLabel() {
        return formulaLabel;
    }

    public void setFormulaLabel(String formulaLabel) {
        this.formulaLabel = formulaLabel;
    }

    public Integer getFormulaType() {
        return formulaType;
    }

    public void setFormulaType(Integer formulaType) {
        this.formulaType = formulaType;
    }

    public Integer getTriggerMode() {
        return triggerMode;
    }

    public void setTriggerMode(Integer triggerMode) {
        this.triggerMode = triggerMode;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Integer getCycleTime() {
        return cycleTime;
    }

    public void setCycleTime(Integer cycleTime) {
        this.cycleTime = cycleTime;
    }

    public Integer getParamNum() {
        return paramNum;
    }

    public void setParamNum(Integer paramNum) {
        this.paramNum = paramNum;
    }

    public String getFormulaTpLabel() {
        return formulaTpLabel;
    }

    public void setFormulaTpLabel(String formulaTpLabel) {
        this.formulaTpLabel = formulaTpLabel;
    }

    public String getResultLabel() {
        return resultLabel;
    }

    public void setResultLabel(String resultLabel) {
        this.resultLabel = resultLabel;
    }

    public Integer getExecOrder() {
        return execOrder;
    }

    public void setExecOrder(Integer execOrder) {
        this.execOrder = execOrder;
    }

    public Integer getFormulaGrpId() {
        return formulaGrpId;
    }

    public void setFormulaGrpId(Integer formulaGrpId) {
        this.formulaGrpId = formulaGrpId;
    }

    public Integer getDomainId() {
        return domainId;
    }

    public void setDomainId(Integer domainId) {
        this.domainId = domainId;
    }
}
