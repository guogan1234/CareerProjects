package com.avp.cdai.web.pushWechat.service;

import com.avp.cdai.web.config.ProjectConfig;
import com.avp.cdai.web.entity.ObjEquipment;
import com.avp.cdai.web.entity.ObjLine;
import com.avp.cdai.web.entity.ObjStation;
import com.avp.cdai.web.model.SendMsg;
import com.avp.cdai.web.pushWechat.model.PushMsgModel;
import com.avp.cdai.web.repository.ObjEquipmentRepository;
import com.avp.cdai.web.repository.ObjLineRepository;
import com.avp.cdai.web.repository.ObjStationRepository;
import com.avp.cdai.web.service.AlarmTickService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by guo on 2017/10/23.
 */
@Service
public class AlarmService {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

//    private final String WEB_URL = "http://192.168.1.110:4065";
    private String WEB_URL = null;
    private final String STATION_PATH = "/skip?path=device/alldevices&stationid=";
    private final String GAT_PATH = "/skip?path=device/gat&stationid=";
    private final String TVM_PATH = "/skip?path=device/tvm&stationid=";
    private final String BOM_PATH = "/skip?path=device/bom&stationid=";

    @Autowired
    ProjectConfig projectConfig;

    @Autowired
    AlarmTickService alarmTickService;

    @Autowired
    ObjStationRepository objStationRepository;
    @Autowired
    ObjLineRepository objLineRepository;
    @Autowired
    ObjEquipmentRepository objEquipmentRepository;

    public List<PushMsgModel> getPushList(){
        List<PushMsgModel> temp = new ArrayList<>();
        //初始化常量
        WEB_URL = projectConfig.getAlarmWebUrl();
        //获取推送状态信息
        List<SendMsg> datas = alarmTickService.getSendMsgList();
        if(datas == null){
            logger.error("获取的状态信息列表为null！");
        }
        //分析出报警信息
        for(SendMsg msg:datas){
            Integer state = msg.getState();
            if(state == 3){
                Integer type = msg.getType();
                PushMsgModel model = new PushMsgModel();
                if(type == 2){//车站
                    //获取车站详细信息
                    Integer id = msg.getId();
//                    logger.debug("车站id：{}",id);
                    List<ObjStation> stationList = objStationRepository.findByStationId(id);
                    String stationName = stationList.get(0).getStationName();
                    Integer lineId = stationList.get(0).getLineId();
                    ObjLine objLine = objLineRepository.findByLineId(lineId);
                    String lineName = objLine.getLineName();

                    String message = lineName + " " + stationName + "发生报警，请及时处理！";

                    model.setTag("车站报警");
                    model.setUrl(WEB_URL + STATION_PATH + id);
                    model.setLocation(stationName);
                    model.setMsg(message);
                    temp.add(model);
                }else if(type == 3){
                    //获取设备详细信息
                    Integer id = msg.getId();
//                    logger.debug("设备id：{}",id);
                    ObjEquipment objEquipment = objEquipmentRepository.findByEquipmentId(id);
                    String deviceName = objEquipment.getEquipmentName();
                    Integer lineId = objEquipment.getLineId();
                    Integer deviceType = objEquipment.getTypeId();
                    Integer deviceId = objEquipment.getEquipmentId();
                    ObjLine objLine = objLineRepository.findByLineId(lineId);
                    String lineName = objLine.getLineName();
                    Integer stationId = objEquipment.getStationId();
                    List<ObjStation> stationList = objStationRepository.findByStationId(stationId);
                    String stationName = stationList.get(0).getStationName();
                    String message = null;
                    String modelUrl = null;
                    if(deviceType == 2) {//GATE
                        modelUrl = WEB_URL + GAT_PATH + stationId + "&deviceid=" +deviceId;
                        message = lineName + " " + stationName + " 设备名称为：" + deviceName + "发生报警，请及时处理！";
                    }else if(deviceType == 3){//TVM
                        modelUrl = WEB_URL + TVM_PATH + stationId + "&deviceid=" +deviceId;
                        message = lineName + " " + stationName + " 设备名称为：" + deviceName + "发生报警，请及时处理！";
                    }else{//BOM
                        modelUrl = WEB_URL + BOM_PATH + stationId + "&deviceid=" +deviceId;
                        message = lineName + " " + stationName + " 设备名称为：" + deviceName + "发生报警，请及时处理！";
                    }
                    model.setTag("设备报警");
                    model.setUrl(modelUrl);
                    model.setLocation(deviceName);
                    model.setMsg(message);
                    temp.add(model);
                }
            }
        }
        logger.debug("获取的报警信息数量为：{}",temp.size());
        return temp;
    }
}
