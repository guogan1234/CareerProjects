package com.avp.config.entity.alarm;

import java.io.Serializable;

public class AlarmInfoMultiKeys implements Serializable{

    private String label;
    private Integer second;
    private Integer msecond;

    public String getLabel() {
        return label;
    }

    public void setLabel(String label) {
        this.label = label;
    }

    public Integer getSecond() {
        return second;
    }

    public void setSecond(Integer second) {
        this.second = second;
    }

    public Integer getMsecond() {
        return msecond;
    }

    public void setMsecond(Integer msecond) {
        this.msecond = msecond;
    }
}
