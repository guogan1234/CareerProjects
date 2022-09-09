package com.avp.mems.ui.mobile.html.model;

import org.springframework.boot.jackson.JsonComponent;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by guo on 2017/9/21.
 */
@JsonComponent
public class StatusMsg {
    private List<SendMsg> sendMsgList = new ArrayList<>();

    public List<SendMsg> getSendMsgList() {
        return sendMsgList;
    }

    public void setSendMsgList(List<SendMsg> sendMsgList) {
        this.sendMsgList = sendMsgList;
    }
}
