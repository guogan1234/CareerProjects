package com.avp.cdai.web.service;

import com.avp.cdai.web.entity.ObjEquipment;
import com.avp.cdai.web.entity.ObjLine;
import com.avp.cdai.web.entity.ObjStation;
import com.avp.cdai.web.model.ReceiveMsg;
import com.avp.cdai.web.model.SendMsg;
import com.avp.cdai.web.repository.ObjEquipmentRepository;
import com.avp.cdai.web.repository.ObjLineRepository;
import com.avp.cdai.web.repository.ObjStationRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by guo on 2017/9/12.
 */
@Service
public class AlarmTickService {
    @Autowired
    ObjLineRepository objLineRepository;
    @Autowired
    ObjStationRepository objStationRepository;
    @Autowired
    ObjEquipmentRepository objEquipmentRepository;

    private List<ReceiveMsg> receiveMsgList = new ArrayList<ReceiveMsg>();

    public List<ReceiveMsg> getReceiveMsgList() {
        return receiveMsgList;
    }

    public void setReceiveMsgList(List<ReceiveMsg> receiveMsgList) {
        this.receiveMsgList = receiveMsgList;
    }

    //推送信息列表
    private List<SendMsg> sendMsgList = new ArrayList<SendMsg>();

    public List<SendMsg> getSendMsgList() {
        return sendMsgList;
    }

    public void setSendMsgList(List<SendMsg> sendMsgList) {
        this.sendMsgList = sendMsgList;
    }

    public AlarmTickService(){
        //测试spring容器代码
        //获取线路id
        if(objLineRepository == null){
            System.out.println("objLineRepository is null!");
            return;
        }
//        List<ObjLine> lineList = objLineRepository.findAll();
    }

    public void InitMsgConfig() {
        //填充要推送的id集合
        //获取线路id
        List<ObjLine> lineList = objLineRepository.findAll();
        ReceiveMsg lineMsg = new ReceiveMsg();
        List<Integer> lineIds = new ArrayList<Integer>();
        for(ObjLine obj:lineList){
            Integer id = obj.getLineId();
            lineIds.add(id);
        }
        lineMsg.setType(1);
        lineMsg.setIds(lineIds);
        //获取车站id
        List<ObjStation> stationList = objStationRepository.findAll();
        ReceiveMsg stationMsg = new ReceiveMsg();
        List<Integer> stationIds = new ArrayList<Integer>();
        for(ObjStation obj:stationList){
            Integer id = obj.getStationId();
            stationIds.add(id);
        }
        stationMsg.setType(2);
        stationMsg.setIds(stationIds);
        //获取设备id
        List<ObjEquipment> equipmentList = objEquipmentRepository.findAll();
        ReceiveMsg equipmentMsg = new ReceiveMsg();
        List<Integer> equipmentIds = new ArrayList<Integer>();
        for(ObjEquipment obj:equipmentList){
            Integer id = obj.getEquipmentId();
            equipmentIds.add(id);
        }
        equipmentMsg.setType(3);
        equipmentMsg.setIds(equipmentIds);

        receiveMsgList.add(lineMsg);
        receiveMsgList.add(stationMsg);
        receiveMsgList.add(equipmentMsg);
        //生成推送信息列表
        MakeSendList();
    }

    private String[] tagArray = {"SER","CS2","ECU","PSU","EOD","CPU","PLC","MEM","CTN","ETH","CS1","DSK","CHS","BCG","TIM","BNA"};

    private void MakeSendList(){
        for(ReceiveMsg msg:receiveMsgList){
            Integer type = msg.getType();
            List<Integer> ids = msg.getIds();
            for(Integer id:ids){
                int n = (int) (0 + Math.random() * (3 - 0 + 1));
                if(type == 1) {
                    SendMsg s = new SendMsg(type, id, "line", n);
                    sendMsgList.add(s);
                }else if(type == 2){
                    SendMsg s = new SendMsg(type, id, "station", n);
                    sendMsgList.add(s);
                }else if(type == 3){
                    n = (int) (0 + Math.random() * (1 - 0 + 1));
                    SendMsg s = new SendMsg(type, id, "COMSTA", n);
                    n = (int) (0 + Math.random() * (7 - 0 + 1));
                    SendMsg s2 = new SendMsg(type, id, "MODE", n);
                    sendMsgList.add(s);
                    sendMsgList.add(s2);
                }
            }
            if(type == 3){//生成标签报警信息
                for(Integer id:ids) {
                    for (String str : tagArray) {
//                        System.out.println("tag:"+str);//调试使用
                        int n = (int) (0 + Math.random() * (3 - 0 + 1));
                        SendMsg s = new SendMsg(4, id, str, n);
                        sendMsgList.add(s);
                    }
                }
            }
        }
    }

    public List<SendMsg> getMsg(Integer start,Integer end){
        List<SendMsg> temp = new ArrayList<SendMsg>();
        Integer len = sendMsgList.size();
        if(start > len){
            return temp;
        }else if(start <= len) {
            if(end >= len ){
                end = len;
            }
            temp = sendMsgList.subList(start, end);
        }
        return temp;
    }

    public Integer getSendMsgLength(){
        return sendMsgList.size();
    }

    public void clearSendMsgList(){
        sendMsgList.clear();
    }
}
