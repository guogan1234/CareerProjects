package com.avp.mems.ui.mobile.html.model;

import java.util.List;

/**
 * Created by guo on 2017/9/6.
 */
public class ReceiveMsg {
    private Integer type;
    private List<Integer> ids;

    public Integer getType() {
        return type;
    }

    public void setType(Integer type) {
        this.type = type;
    }

    public List<Integer> getIds() {
        return ids;
    }

    public void setIds(List<Integer> ids) {
        this.ids = ids;
    }
}
