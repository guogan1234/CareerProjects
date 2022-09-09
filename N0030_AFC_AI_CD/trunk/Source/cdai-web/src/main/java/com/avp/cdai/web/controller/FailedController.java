package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.RefModuleSubtag;
import com.avp.cdai.web.model.FailedModel;
import com.avp.cdai.web.repository.RefModuleSubtagRepository;
import com.avp.cdai.web.repository.TagFailureDetailRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.PageRequest;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.math.BigInteger;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/14.
 */
@RestController
public class FailedController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    TagFailureDetailRepository tagFailureDetailRepository;

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

    private Integer getTagId(String tag){
        Integer temp = null;
        List<RefModuleSubtag> list = refModuleSubtagRepository.findAll();
        for(RefModuleSubtag ref:list){
            if(tag.equals(ref.getTagName()))
            {
                temp = ref.getId();
                break;
            }
        }
        return temp;
    }

    private String getTagName(Integer tagId){
        String temp = null;
        List<RefModuleSubtag> list = refModuleSubtagRepository.findAll();
        for(RefModuleSubtag ref:list){
            if(tagId.equals(ref.getId()))
            {
                temp = ref.getTagName();
                break;
            }
        }
        return temp;
    }

    @RequestMapping(value = "tagFailed/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> tagFailedPercent(@PathVariable("flag")String tagFlag, @RequestParam("page")Integer page, @RequestParam("pageSize") Integer pageSize, Integer lineId, Integer stationId, Date startTime,Date endTime){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
//            //获取起止时间
//            logger.debug("tagFailed...{}",tagFlag);
//            Calendar cal = Calendar.getInstance();
//            cal.add(Calendar.MONTH,-timeCount);
//            Date date = cal.getTime();
//            logger.debug("Date:{}",date);
//            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
//            String startTimeStr = format1.format(date);
//            Date now = new Date();
//            String endTimeStr = format1.format(now);
//            Date startTime = format1.parse(startTimeStr);
//            Date endTime = format1.parse(endTimeStr);
//            logger.debug("获取的起止时间为{},{}",startTime,endTime);
            //分页
            PageRequest pageRequest = new PageRequest(page,pageSize);

            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            if(lineId == null && stationId !=null){
                lists = tagFailureDetailRepository.getStationData2(stationId,startTime,endTime);
                logger.debug("根据车站查询到Tag故障前{}的记录!",lists.size());
            }
            if(lineId != null && stationId == null){
                lists = tagFailureDetailRepository.getLineData2(lineId,startTime,endTime);
                logger.debug("根据线路查询到Tag故障前{}的记录!",lists.size());
            }
            //tag故障的显示模式
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

                        String tagName = (String) objects[1];
                        String tagDesc = getTagName(tagName);
                        xList.add(tagDesc);

                        Integer tagId = getTagId(tagName);
                        idList.add(tagId);
                    }
                }
                failedModel.setxValue(xList);
                failedModel.setyValue(yList);
                failedModel.setIdList(idList);

                builder.setResultEntity(failedModel,ResponseCode.RETRIEVE_SUCCEED);
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

                        String tagName = (String) objects[1];
                        String tagDesc = getTagName(tagName);
                        xList.add(tagDesc);

                        Integer tagId = getTagId(tagName);
                        idList.add(tagId);
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

    @RequestMapping(value = "TagFailed/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> tagFailedPercent2(@PathVariable("flag")String tagFlag, @RequestParam("page")Integer page, @RequestParam("pageSize") Integer pageSize, Integer lineId, Integer stationId, Date startTime,Date endTime){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            if(lineId == null && stationId !=null){
                lists = tagFailureDetailRepository.getStationData2_cascade(stationId,startTime,endTime);
                logger.debug("根据车站查询到Tag故障前{}的记录!",lists.size());
            }
            if(lineId != null && stationId == null){
                lists = tagFailureDetailRepository.getLineData2_cascade(lineId,startTime,endTime);
                logger.debug("根据线路查询到Tag故障前{}的记录!",lists.size());
            }
            //tag故障的显示模式
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

                        String tagName = (String) objects[1];
                        String tagDesc = (String) objects[3];
                        xList.add(tagDesc);

                        BigInteger tagId = (BigInteger)objects[2];
                        idList.add(tagId.intValue());
                    }
                }
                failedModel.setxValue(xList);
                failedModel.setyValue(yList);
                failedModel.setIdList(idList);

                builder.setResultEntity(failedModel,ResponseCode.RETRIEVE_SUCCEED);
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

                        String tagName = (String) objects[1];
                        String tagDesc = (String) objects[3];
                        xList.add(tagDesc);

                        BigInteger tagId = (BigInteger)objects[2];
                        idList.add(tagId.intValue());
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


    @RequestMapping(value = "tagFailedDetail/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> getTagFailedDetail(@PathVariable("flag")String tagFlag,String tagName,Integer page,Integer pageSize,Integer lineId,Integer stationId,Date startTime,Date endTime,Integer tagId){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
//            //获取起止时间
//            logger.debug("tagFailedDetail...{}",tagFlag);
//            Calendar cal = Calendar.getInstance();
//            cal.add(Calendar.MONTH,-timeCount);
//            Date date = cal.getTime();
//            logger.debug("Date:{}",date);
//            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
//            String startTimeStr = format1.format(date);
//            Date now = new Date();
//            String endTimeStr = format1.format(now);
//            Date startTime = format1.parse(startTimeStr);
//            Date endTime = format1.parse(endTimeStr);
            //获取所有模块信息
//            List<RefModule> moduleList = refModuleRepository.findAll();

            //获取tagName
            tagName = getTagName(tagId);
            if(tagName == null){
                logger.error("获取的tagName为null");
            }

            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            if(lineId == null && stationId !=null){
                lists = tagFailureDetailRepository.getTagFailedDetail2(stationId,startTime,endTime,tagName);
                logger.debug("根据车站查询到标签详细故障前{}的记录!",lists.size());
            }
            if(lineId != null && stationId == null){
                lists = tagFailureDetailRepository.getTagFailedDetail(lineId,startTime,endTime,tagName);
                logger.debug("根据线路查询到标签详细故障前{}的记录!",lists.size());
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

    @RequestMapping(value = "deviceFailed/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> getDeviceData(@PathVariable("flag")String tagFlag,Integer page,Integer pageSize,Integer lineId,Integer stationId,Integer timeCount){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            //获取起止时间
            logger.debug("deviceFailed...{}",tagFlag);
            Calendar cal = Calendar.getInstance();
            cal.add(Calendar.MONTH,-timeCount);
            Date date = cal.getTime();
            logger.debug("Date:{}",date);
            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
            String startTime = format1.format(date);
            Date now = new Date();
            String endTime = format1.format(now);
            Date start = format1.parse(startTime);
            Date end = format1.parse(endTime);
            //获取所有模块信息
//            List<RefModule> moduleList = refModuleRepository.findAll();

            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            if(lineId == null && stationId !=null){
                lists = tagFailureDetailRepository.getDeviceFailed2(stationId,start,end);
                logger.debug("根据车站查询到设备故障前{}的记录!",lists.size());
            }
            if(lineId != null && stationId == null){
                lists = tagFailureDetailRepository.getDeviceFailed(lineId,start,end);
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
}
