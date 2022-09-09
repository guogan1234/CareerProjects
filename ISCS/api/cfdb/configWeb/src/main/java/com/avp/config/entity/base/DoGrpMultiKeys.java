package com.avp.config.entity.base;

import java.io.Serializable;
import java.util.Objects;

public class DoGrpMultiKeys implements Serializable{

    private String doGrpLabel;

    private  int doOrder;

    public DoGrpMultiKeys(){

    }

    public DoGrpMultiKeys(String doGrpLabel,int doOrder){
        this.doGrpLabel=doGrpLabel;
        this.doOrder=doOrder;
    }

    public String getDoGrpLabel() {
        return doGrpLabel;
    }

    public void setDoGrpLabel(String doGrpLabel) {
        this.doGrpLabel = doGrpLabel;
    }

    public int getDoOrder() {
        return doOrder;
    }

    public void setDoOrder(int doOrder) {
        this.doOrder = doOrder;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DoGrpMultiKeys that = (DoGrpMultiKeys) o;
        return doOrder == that.doOrder &&
                Objects.equals(doGrpLabel, that.doGrpLabel);
    }

    @Override
    public int hashCode() {

        return Objects.hash(doGrpLabel, doOrder);
    }
}
