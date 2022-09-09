package com.avp.cdai.web.service;

import com.avp.cdai.web.model.ReceiveMsg;
import com.avp.cdai.web.model.SendMsg;
import com.fasterxml.jackson.databind.JavaType;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * Created by guo on 2017/9/6.
 */
@Service
public class AlarmSetService {
    private List<ReceiveMsg> receiveMsgs = null;
    private List<SendMsg> sendMsgs = new ArrayList<SendMsg>();

    public List<SendMsg> getSendMsgs() {
        return sendMsgs;
    }

    public void setSendMsgs(List<SendMsg> sendMsgs) {
        this.sendMsgs = sendMsgs;
    }

    private ObjectMapper objectMapper = new ObjectMapper();

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    public List<ReceiveMsg> getReceiveMsgs() {
        return receiveMsgs;
    }

    public void setReceiveMsgs(List<ReceiveMsg> receiveMsgs) {
        this.receiveMsgs = receiveMsgs;
    }

    public void parseMsg(String msg){
        JavaType javaType1 = objectMapper.getTypeFactory().constructParametricType(ArrayList.class, ReceiveMsg.class);
        try {
            receiveMsgs = (List<ReceiveMsg>) objectMapper.readValue(msg, javaType1);
            if(receiveMsgs == null){
                logger.debug("receiveMsgs is null!!!");
                return;
            }
            for (ReceiveMsg r : receiveMsgs) {
                if(r.getIds() == null){
                    logger.debug("1:msg.type = {},msg.ids=null", r.getType());
                }else {
                    logger.debug("1:msg.type = {},msg.ids={}", r.getType(), r.getIds());
                }
            }
            //拿取报警数据，并填充发送数据
            getSendMsg();
        }
        catch (Exception e){
            logger.error(e.getMessage());
        }
    }

    private String[] tagArray = {"SER","CS2","ECU","PSU","EOD","CPU","PLC","MEM","CTN","ETH","CS1","DSK","CHS","BCG","TIM","BNA"};

    public void clearSendMsgs(){
        this.sendMsgs.clear();
    }

    public void getSendMsg(){
        for (ReceiveMsg r : receiveMsgs) {
            if(r.getIds() == null){
                logger.debug("2:msg.type = {},msg.ids=null", r.getType());
            }else {
                logger.debug("2:msg.type = {},msg.ids={}", r.getType(), r.getIds());
            }
            Integer type = r.getType();
            List<Integer> ids = r.getIds();
            if(ids == null){
                continue;
            }
            for(Integer id:ids) {
                //生成随机数
                if(type == 1) {
                    int n = (int) (0 + Math.random() * (3 - 0 + 1));
                    SendMsg s = new SendMsg(type, id, "line", n);
                    sendMsgs.add(s);
                }else if(type == 2){
                    int n = (int) (0 + Math.random() * (3 - 0 + 1));
                    SendMsg s = new SendMsg(type, id, "station", n);
                    sendMsgs.add(s);
                }else if(type == 3){
                    int n = (int) (0 + Math.random() * (1 - 0 + 1));
                    SendMsg s = new SendMsg(type, id, "COMSTA", n);
                    n = (int) (0 + Math.random() * (7 - 0 + 1));
                    SendMsg s2 = new SendMsg(type, id, "MODE", n);
                    sendMsgs.add(s);
                    sendMsgs.add(s2);
                }else if(type == 4){
                    for(String tag:tagArray){
                        int n = (int) (0 + Math.random() * (3 - 0 + 1));
                        SendMsg s = new SendMsg(type, id, tag, n);
                        sendMsgs.add(s);
                    }
                }
            }
        }
    }
}
