package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.RefModuleSubtag;
import com.avp.cdai.web.model.FailedModel;
import com.avp.cdai.web.repository.DeviceFailedRepository;
import com.avp.cdai.web.repository.RefModuleSubtagRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.math.BigInteger;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/12/12.
 */
@RestController
public class DeviceFailedController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    DeviceFailedRepository deviceFailedRepository;

    @Autowired
    RefModuleSubtagRepository refModuleSubtagRepository;

    private String getTagName(String tag){
        String temp = null;
        List<RefModuleSubtag> list = refModuleSubtagRepository.findAll();
        for(RefModuleSubtag ref:list){
            if(tag.equals(ref.getTagName()))
            {
                temp = ref.getTagDesc();
                break;
            }
        }
        return temp;
    }

    @RequestMapping(value = "DeviceFailed/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> getDeviceData(@PathVariable("flag")String tagFlag, Integer page, Integer pageSize, Integer lineId, Integer stationId, Date startTime,Date endTime,Integer deviceType){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            //获取所有模块信息
//            List<RefModule> moduleList = refModuleRepository.findAll();

            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            if(lineId == null && stationId !=null){
                lists = deviceFailedRepository.getDeviceFailed2(stationId,startTime,endTime,deviceType);
                logger.debug("根据车站查询到设备故障前{}的记录!",lists.size());
            }
            if(lineId != null && stationId == null){
                lists = deviceFailedRepository.getDeviceFailed(lineId,startTime,endTime,deviceType);
                logger.debug("根据线路查询到设备故障前{}的记录!",lists.size());
            }
            //模块故障的显示模式
            if(tagFlag.equals("percent")){
                logger.debug("percent");
                List<String> xList = new ArrayList<String>();
                List<Double> yList = new ArrayList<Double>();
                List<Integer> idList = new ArrayList<Integer>();
                Double total = 0D;
                for(int i = 0;i<lists.size();i++){
                    Object[] objs = lists.get(i);
                    BigInteger num = (BigInteger) objs[0];
                    total = total + num.doubleValue();
                }
                for(int i = 0;i<lists.size();i++){
                    if(i < pageSize){
                        Object[] objects = lists.get(i);
                        BigInteger num = (BigInteger) objects[0];
                        Double d = num.doubleValue();
                        Double value = (d/total)*100;
                        //double格式化输出
                        DecimalFormat formatter = new DecimalFormat("##.#");
                        String strD = formatter.format(value);
                        double dValue = Double.valueOf(strD).doubleValue();
                        yList.add(dValue);

                        //目前根据deviceId找不到设备名称，先传deviceId
                        Integer id = (Integer) objects[1];
                        xList.add(id.toString());

                        Integer deviceId = (Integer)objects[1];
                        idList.add(deviceId);
                    }
                }
                failedModel.setxValue(xList);
                failedModel.setyValue(yList);
                failedModel.setIdList(idList);

                builder.setResultEntity(failedModel, ResponseCode.RETRIEVE_SUCCEED);
                return builder.getResponseEntity();
            }else if(tagFlag.equals("count")){
                logger.debug("count");
                List<String> xList = new ArrayList<String>();
                List<Double> yList = new ArrayList<Double>();
                List<Integer> idList = new ArrayList<Integer>();
                for(int i = 0;i<lists.size();i++){
                    if(i < pageSize){
                        Object[] objects = lists.get(i);
                        BigInteger num = (BigInteger) objects[0];
                        yList.add(num.doubleValue());

                        //目前根据deviceId找不到设备名称，先传deviceId
                        Integer id = (Integer) objects[1];
                        xList.add(id.toString());

                        Integer deviceId = (Integer)objects[1];
                        idList.add(deviceId);
                    }
                }
                failedModel.setxValue(xList);
                failedModel.setyValue(yList);
                failedModel.setIdList(idList);

                builder.setResultEntity(failedModel,ResponseCode.RETRIEVE_SUCCEED);
                return builder.getResponseEntity();
            }else if(tagFlag.equals("rate")){
                logger.debug("rate");
            }
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "DeviceFailedDetail/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> getDeviceDetail(@PathVariable("flag")String flag,Integer deviceId,Date startTime,Date endTime,Integer page,Integer pageSize){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            lists = deviceFailedRepository.getDeviceFailedDetail(deviceId,startTime,endTime);
            logger.debug("根据设备查询到设备详细故障前{}的记录!",lists.size());

            //
            if(flag.equals("percent")){
                logger.debug("percent");
                List<String> xList = new ArrayList<String>();
                List<Double> yList = new ArrayList<Double>();
                List<Integer> idList = new ArrayList<Integer>();
                Double total = 0D;
                for(int i = 0;i<lists.size();i++){
                    Object[] objs = lists.get(i);
                    BigInteger num = (BigInteger) objs[0];
                    total = total + num.doubleValue();
                }
                for(int i = 0;i<lists.size();i++){
                    if(i < pageSize){
                        Object[] objects = lists.get(i);
                        BigInteger num = (BigInteger) objects[0];
                        Double d = num.doubleValue();
                        Double value = (d/total)*100;
                        //double格式化输出
                        DecimalFormat formatter = new DecimalFormat("##.#");
                        String strD = formatter.format(value);
                        double dValue = Double.valueOf(strD).doubleValue();
                        yList.add(dValue);

                        //目前根据deviceId找不到设备名称，先传deviceId
                        String tagName = (String) objects[1];
                        String tagDesc = getTagName(tagName);
                        if(tagDesc == null){
                            logger.error("根据tagName找不到对应的中文描述!");
                        }
                        xList.add(tagDesc);
                    }
                }
                failedModel.setxValue(xList);
                failedModel.setyValue(yList);
                failedModel.setIdList(idList);

                builder.setResultEntity(failedModel, ResponseCode.RETRIEVE_SUCCEED);
                return builder.getResponseEntity();
            }else if(flag.equals("count")) {
                logger.debug("count");
                List<String> xList = new ArrayList<String>();
                List<Double> yList = new ArrayList<Double>();
                List<Integer> idList = new ArrayList<Integer>();
                for (int i = 0; i < lists.size(); i++) {
                    if (i < pageSize) {
                        Object[] objects = lists.get(i);
                        BigInteger num = (BigInteger) objects[0];
                        yList.add(num.doubleValue());

                        //目前根据deviceId找不到设备名称，先传deviceId
                        String tagName = (String) objects[1];
                        String tagDesc = getTagName(tagName);
                        if(tagDesc == null){
                            logger.error("根据tagName找不到对应的中文描述!");
                        }
                        xList.add(tagDesc);
                    }
                }
                failedModel.setxValue(xList);
                failedModel.setyValue(yList);
                failedModel.setIdList(idList);

                builder.setResultEntity(failedModel, ResponseCode.RETRIEVE_SUCCEED);
                return builder.getResponseEntity();
            }
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
