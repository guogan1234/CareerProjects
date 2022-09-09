package com.avp.cdai.web.test;

/**
 * Created by guo on 2017/10/19.
 */
public class msgModel {
    private String touser;
    private String toparty;
    private String totag;
    private Integer safe;
    private String msgtype;
    private Integer agentid;
    private msgSub text;

    public String getTouser() {
        return touser;
    }

    public void setTouser(String touser) {
        this.touser = touser;
    }

    public String getToparty() {
        return toparty;
    }

    public void setToparty(String toparty) {
        this.toparty = toparty;
    }

    public String getTotag() {
        return totag;
    }

    public void setTotag(String totag) {
        this.totag = totag;
    }

    public Integer getSafe() {
        return safe;
    }

    public void setSafe(Integer safe) {
        this.safe = safe;
    }

    public String getMsgtype() {
        return msgtype;
    }

    public void setMsgtype(String msgtype) {
        this.msgtype = msgtype;
    }

    public Integer getAgentid() {
        return agentid;
    }

    public void setAgentid(Integer agentid) {
        this.agentid = agentid;
    }

    public msgSub getText() {
        return text;
    }

    public void setText(msgSub text) {
        this.text = text;
    }
}
