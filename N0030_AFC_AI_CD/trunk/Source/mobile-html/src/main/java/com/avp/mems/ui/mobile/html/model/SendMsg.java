package com.avp.mems.ui.mobile.html.model;

import org.springframework.boot.jackson.JsonComponent;

/**
 * Created by guo on 2017/9/7.
 */
@JsonComponent
public class SendMsg{
    private Integer type;
    private Integer id;
    private String tag;
    private Integer state;

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getTag() {
        return tag;
    }

    public void setTag(String tag) {
        this.tag = tag;
    }

    public Integer getState() {
        return state;
    }

    public void setState(Integer state) {
        this.state = state;
    }

    public SendMsg(){

    }

    public SendMsg(Integer type, Integer id, String tag, Integer state) {
        this.type = type;
        this.id = id;
        this.tag = tag;
        this.state = state;
    }
}
