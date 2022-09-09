package com.avp.cdai.web.model;

import java.util.List;

/**
 * Created by guo on 2017/12/29.
 */
public class IncontinueTimeModel {
    private String name;
    private List<List<Long>> data;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<List<Long>> getData() {
        return data;
    }

    public void setData(List<List<Long>> data) {
        this.data = data;
    }
}
