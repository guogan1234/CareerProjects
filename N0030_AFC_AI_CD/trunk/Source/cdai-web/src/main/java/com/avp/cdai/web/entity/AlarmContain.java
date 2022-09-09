package com.avp.cdai.web.entity;

/**
 * Created by guo on 2017/9/4.
 */
public class AlarmContain {
    private String tag;
    private Integer tagState;

    public String getTagId() {
        return tag;
    }

    public void setTagId(String tag) {
        this.tag = tag;
    }

    public Integer getTagState() {
        return tagState;
    }

    public void setTagState(Integer tagState) {
        this.tagState = tagState;
    }

    public AlarmContain(String tag, Integer tagState) {
        this.tag = tag;
        this.tagState = tagState;
    }

    @Override
    public String toString() {
        return "AlarmContain{" +
                "tag='" + tag + '\'' +
                ", tagState=" + tagState +
                '}';
    }
}
