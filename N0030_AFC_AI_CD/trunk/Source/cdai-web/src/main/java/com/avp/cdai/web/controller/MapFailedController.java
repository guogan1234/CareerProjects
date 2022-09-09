package com.avp.cdai.web.controller;

import com.avp.cdai.web.config.ProjectConfig;
import com.avp.cdai.web.entity.ObjStation;
import com.avp.cdai.web.model.FailedModel;
import com.avp.cdai.web.model.MapStationFailed;
import com.avp.cdai.web.model.MapStationFailedSub;
import com.avp.cdai.web.repository.ObjStationRepository;
import com.avp.cdai.web.repository.StationFailedRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.math.BigInteger;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/26.
 */
@RestController
public class MapFailedController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    ProjectConfig projectConfig;

    @Autowired
    StationFailedRepository stationFailedRepository;

    @Autowired
    ObjStationRepository objStationRepository;

    @RequestMapping(value = "stationFailureTop",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Integer>> getStationFailureTop(Date startTime,Date endTime,Integer count){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
            //暂生成起止时间
            Calendar cal = Calendar.getInstance();
            cal.add(Calendar.MONTH,-count);
            Date date = cal.getTime();
            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
            String startTimeStr = format1.format(date);
            Date now = new Date();
            String endTimeStr = format1.format(now);
            Date start = format1.parse(startTimeStr);
            Date end = format1.parse(endTimeStr);

            FailedModel<String,Integer> model = new FailedModel<String,Integer>();
            List<String> xList = new ArrayList<String>();
            List<Integer> yList = new ArrayList<Integer>();
            //获取所有车站报警总数
            List<Object[]> list = stationFailedRepository.getAllData(start, end);
            for(int i = 0;i<list.size();i++){
                if(i < count){
                    Object[] objects = list.get(i);
                    BigInteger num = (BigInteger) objects[0];
                    Integer stationId = (Integer) objects[1];
                    List<ObjStation> objStationList = objStationRepository.findByStationId(stationId);
                    if(objStationList.size() == 0){
                        continue;
                    }
                    String stationName = objStationList.get(0).getStationName();

                    xList.add(stationName);
                    yList.add(num.intValue());
                }
            }
            model.setxValue(xList);
            model.setyValue(yList);

            builder.setResultEntity(model,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "mapDetail",method = RequestMethod.GET)
    private ResponseEntity<MapStationFailed> getMapInfo(Date startTime,Date endTime,Integer level){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
//            //获取前一个月的起止时间
//            Date startTime = null;
//            Date endTime = null;
//            Calendar cal = Calendar.getInstance();
//            endTime = cal.getTime();
//            cal.add(Calendar.MONTH, -7);
//            startTime = cal.getTime();
//            logger.debug("mapDetail--获取的起止时间为：{},{}", startTime, endTime);
            //
            MapStationFailed obj = new MapStationFailed();
            List<MapStationFailedSub> statusList = new ArrayList<MapStationFailedSub>();
            //获取状态列表
            List<Object[]> list = stationFailedRepository.getAllData(startTime, endTime);
            List<ObjStation> objStations = objStationRepository.findAll();//减少数据库查询次数
            for (Object[] objects : list) {
                MapStationFailedSub sub = new MapStationFailedSub();
                BigInteger total = (BigInteger) objects[0];
                Integer status = weightStationFailed(total.longValue());
                Integer stationId = (Integer) objects[1];
                logger.debug("车站{},状态级别{}", stationId, status);
                sub.setStatus(status);
                //根据选择级别填充数据
                if(level == -1) {
                    //附加车站信息
                    for (ObjStation station : objStations) {
                        if (station.getStationId() == stationId) {
                            logger.debug("车站id和名称：{},{}", stationId, station.getStationName());
                            sub.setStationName(station.getStationName());
                            sub.setLongitude(station.getLongitude());
                            sub.setLatitude(station.getLatitude());
                            sub.setTotalNum(total.intValue());
                            break;
                        }
                    }
                    statusList.add(sub);
                }else {
                    if(status == level){
                        for (ObjStation station : objStations) {
                            if (station.getStationId() == stationId) {
                                logger.debug("车站id和名称：{},{}", stationId, station.getStationName());
                                sub.setStationName(station.getStationName());
                                sub.setLongitude(station.getLongitude());
                                sub.setLatitude(station.getLatitude());
                                sub.setTotalNum(total.intValue());
                                break;
                            }
                        }
                        statusList.add(sub);
                    }
                }
            }
            obj.setStatusList(statusList);
            builder.setResultEntity(obj, ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    private Integer weightStationFailed(Long total){
        Integer level1 = Integer.parseInt(projectConfig.getMapLevel1());
        Integer level2 = Integer.parseInt(projectConfig.getMapLevel2());
        Integer level3 = Integer.parseInt(projectConfig.getMapLevel3());
        logger.debug("获取的level级别数量分别为：{},{},{}",level1,level2,level3);
        if(total < level1) {
            return 0;
        }else{
            if(total < level2){
                return 0;//1
            }else {
                if(total > level3){
                    return 2;
                }else {
                    return 1;
                }
            }
        }
    }
}
