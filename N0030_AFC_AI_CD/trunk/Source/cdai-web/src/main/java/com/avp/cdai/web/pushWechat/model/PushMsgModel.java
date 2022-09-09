package com.avp.cdai.web.pushWechat.model;

/**
 * Created by guo on 2017/10/23.
 */
public class PushMsgModel {
    private String tag;//报警类型
    private String location;//哪里报警
    private String msg;
    private String url;

    public String getTag() {
        return tag;
    }

    public void setTag(String tag) {
        this.tag = tag;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }
}
