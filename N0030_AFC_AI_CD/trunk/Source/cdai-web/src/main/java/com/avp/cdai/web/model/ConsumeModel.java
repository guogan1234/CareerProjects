package com.avp.cdai.web.model;

import java.util.List;

/**
 * Created by guo on 2017/10/31.
 */
public class ConsumeModel {
    private Integer id;
    private String name;
    private List<List<Long>> data;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

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
