package com.avp.cdai.web.entity;

import java.util.List;

/**
 * Created by guo on 2017/9/4.
 */
public class AlarmData {
    private Integer id;
    private Integer state;
    private List<AlarmContain> contains;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getState() {
        return state;
    }

    public void setState(Integer state) {
        this.state = state;
    }

    public List<AlarmContain> getContains() {
        return contains;
    }

    public void setContains(List<AlarmContain> contains) {
        this.contains = contains;
    }

    @Override
    public String toString() {
        return "AlarmData{" +
                "id=" + id +
                ", state=" + state +
                ", contains=" + contains +
                '}';
    }
}
