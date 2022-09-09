package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.RefModule;
import com.avp.cdai.web.entity.RefModuleSubtag;
import com.avp.cdai.web.model.FailedModel;
import com.avp.cdai.web.repository.ModuleFailureRepository;
import com.avp.cdai.web.repository.RefModuleRepository;
import com.avp.cdai.web.repository.RefModuleSubtagRepository;
import com.avp.cdai.web.repository.TagFailureDetailRepository;
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
 * Created by guo on 2017/9/15.
 */
@RestController
public class ModuleFailureController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    ModuleFailureRepository moduleFailureRepository;

    @Autowired
    RefModuleRepository refModuleRepository;

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

    private String findModuleNameById(Integer id, List<RefModule> list){
        String s = null;
        for(RefModule module:list){
            if(id == module.getModuleId()){
                s = module.getModuleName();
                return s;
            }
        }
        logger.debug("findModuleNameById -- 未找到对应模块id={}的模块名称！",id);
        return s;
    }

    @RequestMapping(value = "moduleFailed/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> getModuleFailData2(@PathVariable("flag")String tagFlag,Integer moduleId,Integer page,Integer pageSize,Integer lineId,Integer stationId,Integer timeCount,Date startTime,Date endTime){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
//            //获取起止时间
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
            //获取返回数据
            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            if(lineId == null && stationId !=null){
                lists = moduleFailureRepository.getModuleFailed2(moduleId,stationId,startTime,endTime);
                logger.debug("根据车站查询到模块故障的tag故障前{}的记录!",lists.size());
            }
            if(lineId != null && stationId == null){
                lists = moduleFailureRepository.getModuleFailed(moduleId,lineId,startTime,endTime);
                logger.debug("根据线路查询到模块故障的tag故障前{}的记录!",lists.size());
            }
            //模块故障的显示模式
            if(tagFlag.equals("percent")){
                logger.debug("percent");
                List<String> xList = new ArrayList<String>();
                List<Double> yList = new ArrayList<Double>();
                List<Integer> idList = new ArrayList<Integer>();
                //计算总量
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
                        DecimalFormat formatter = new DecimalFormat("##.##");
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

    @RequestMapping(value = "ModuleFailed/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> getModuleFailData2_2(@PathVariable("flag")String tagFlag,Integer moduleId,Integer page,Integer pageSize,Integer lineId,Integer stationId,Integer timeCount,Date startTime,Date endTime){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            //获取返回数据
            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            if(lineId == null && stationId !=null){
                lists = moduleFailureRepository.getModuleFailed2_cascade(moduleId,stationId,startTime,endTime);
                logger.debug("根据车站查询到模块故障的tag故障前{}的记录!",lists.size());
            }
            if(lineId != null && stationId == null){
                lists = moduleFailureRepository.getModuleFailed_cascade(moduleId,lineId,startTime,endTime);
                logger.debug("根据线路查询到模块故障的tag故障前{}的记录!",lists.size());
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


    @RequestMapping(value = "moduleFailedDetail/{flag}",method = RequestMethod.GET)
    ResponseEntity<FailedModel<String,Double>> getModuleDetail2(@PathVariable("flag")String tagFlag,Integer tagId,String tagName,Integer page,Integer pageSize,Integer lineId,Integer stationId,Integer timeCount,Date startTime,Date endTime){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
//            //获取起止时间
//            logger.debug("tagFailedPercent...{}",tagFlag);
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
            //获取数据
            FailedModel<String,Double> failedModel = new FailedModel<String,Double>();
            List<Object[]> lists = null;
            tagName = getTagName(tagId);
            if(tagName == null){
                logger.error("根据tagId未找到对应的tagName!");
            }

            if(lineId == null && stationId !=null){
                lists = tagFailureDetailRepository.getTagFailedDetail2(stationId,startTime,endTime,tagName);
                logger.debug("根据车站查询到模块故障id为{}的tag详细设备信息前{}的记录!",tagId,lists.size());
            }
            if(lineId != null && stationId == null){
                lists = tagFailureDetailRepository.getTagFailedDetail(lineId,startTime,endTime,tagName);
                logger.debug("根据线路查询到模块故障id为{}的tag详细设备信息前{}的记录!",tagId,lists.size());
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

                        Integer id = (Integer) objects[1];
                        xList.add(id.toString());
                        idList.add(id);
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

                        Integer id = (Integer) objects[1];
                        xList.add(id.toString());
                        idList.add(id);
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
