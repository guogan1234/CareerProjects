package com.avp.cdai.web.pushWechat.service;

import com.avp.cdai.web.entity.CoreSwitchStatus;
import com.avp.cdai.web.entity.ObjLine;
import com.avp.cdai.web.entity.ObjStation;
import com.avp.cdai.web.model.SendMsg;
import com.avp.cdai.web.repository.CoreSwitchRepository;
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
public class ConnectService {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    private final String CORE_DISCONN = "中央系统内部网络故障";
    private final String LINE_DISCONN = "线路中央至车站网络设备故障";
    private final String STATION_DISCONN = "车站内局域网网络设备故障";

    @Autowired
    CoreSwitchRepository coreSwitchRepository;

    @Autowired
    AlarmTickService alarmTickService;

    @Autowired
    ObjLineRepository objLineRepository;
    @Autowired
    ObjStationRepository objStationRepository;

    public List<String> getConnAlarms(){
        List<String> temp = new ArrayList<>();

        //获取中央系统连接状况
        List<CoreSwitchStatus> coreSwitchStatuses = coreSwitchRepository.getLatestData();
        if(coreSwitchStatuses == null){
            logger.error("获取的中央网络连接状况列表为null");
        }
        logger.debug("获取的中央网络连接状况列表的数量为：{}",coreSwitchStatuses.size());
        Integer status = coreSwitchStatuses.get(0).getStatus();
        if (status == 1){
            temp.add(CORE_DISCONN);
        }
        //获取推送状态信息
        List<SendMsg> datas = alarmTickService.getSendMsgList();
        if(datas == null){
            logger.error("获取的状态信息列表为null！");
        }
        for (SendMsg data:datas){
            Integer state = data.getState();
            if(state == 3){
                Integer type = data.getType();
                if (type == 1){//线路
                    Integer id = data.getId();
                    ObjLine objLine = objLineRepository.findByLineId(id);
                    String name = objLine.getLineName();
                    String msg = "（" + name + "）" + LINE_DISCONN;
                    logger.debug("网络报警生成 -- id:{},name:{}",id,name);
                    temp.add(msg);
                }else if(type == 2){//车站
                    Integer id = data.getType();
                    List<ObjStation> objStations = objStationRepository.findByStationId(id);
                    String name = objStations.get(0).getStationName();
                    String msg = "（" + name + "）" + STATION_DISCONN;
                    logger.debug("网络报警生成 -- id:{},name:{}",id,name);
                    temp.add(msg);
                }
            }
        }
        return temp;
    }
}
