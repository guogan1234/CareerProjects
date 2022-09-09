package com.avp.cdai.web.entity;

import java.util.List;

/**
 * Created by guo on 2017/9/5.
 */
public class Equipment {
    private Integer id;
    private Integer state;
    private String name;
    private List<AlarmContain> tags;

    public List<AlarmContain> getTags() {
        return tags;
    }

    public void setTags(List<AlarmContain> tags) {
        this.tags = tags;
    }

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

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
