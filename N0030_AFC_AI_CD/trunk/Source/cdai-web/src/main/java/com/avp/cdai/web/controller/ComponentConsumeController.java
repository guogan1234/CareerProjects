package com.avp.cdai.web.controller;

import com.avp.cdai.web.model.AxisModel;
import com.avp.cdai.web.model.ConsumeModel;
import com.avp.cdai.web.repository.ComponentConsumeRepository;
import com.avp.cdai.web.repository.ComponentDestRepository;
import com.avp.cdai.web.repository.ComponentPredictRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/31.
 */
@RestController
public class ComponentConsumeController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    ComponentConsumeRepository componentConsumeRepository;
    @Autowired
    ComponentPredictRepository componentPredictRepository;

    @Autowired
    ComponentDestRepository componentDestRepository;

    @RequestMapping(value = "component/{flag}",method = RequestMethod.GET)
    private ResponseEntity<ConsumeModel> getData(@PathVariable("flag")String flag, Integer count, @RequestParam("page")Integer page, @RequestParam("pageSize")Integer pageSize,Date startTime,Date endTime){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
//            //获取起止时间
//            logger.debug("tagFailed...{}", flag);
//            Calendar cal = Calendar.getInstance();
//            if(flag.equals("consume")) {
//                cal.add(Calendar.MONTH, -count);
//            }else if(flag.equals("predict")){
//                cal.add(Calendar.MONTH, count);
//            }
//            Date date = cal.getTime();
//            logger.debug("Date:{}", date);
//            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
//            String startTime = format1.format(date);
//            Date now = new Date();
//            String endTime = format1.format(now);
//            Date start = format1.parse(startTime);
//            Date end = format1.parse(endTime);
//            logger.debug("获取的起止时间为{},{}", start, end);
            //耗材消耗分析
            List<ConsumeModel> list = new ArrayList<>();
            if(flag.equals("consume")){
                //获取耗材消耗前十的id列表
//                List<Object[]> topList = componentConsumeRepository.getTopData(start,end);
                List<Object[]> topList = componentConsumeRepository.getTopData(startTime,endTime);
                List<Integer> idList = new ArrayList<>();
                for(int i = 0;i<topList.size();i++){
                    if(i < pageSize) {
                        Object[] objects = topList.get(i);
                        Integer id = (Integer) objects[0];
                        idList.add(id);
                    }else {
                        break;
                    }
                }
                //根据id集合获取数据
//                list = getModelList(idList,start,end,flag);
                list = getModelList(idList,startTime,endTime,flag);
                logger.debug("获取的记录数量为：{}",list.size());
                builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
            }else if(flag.equals("predict")){//耗材消耗预测
//                List<Object[]> topList = componentPredictRepository.getTopData(end,start);
                List<Object[]> topList = componentPredictRepository.getTopData(startTime,endTime);
                List<Integer> idList = new ArrayList<>();
                for(int i = 0;i<topList.size();i++){
                    if(i < pageSize) {
                        Object[] objects = topList.get(i);
                        Integer id = (Integer) objects[0];
                        idList.add(id);
                    }else {
                        break;
                    }
                }
                //根据id集合获取数据
//                list = getModelList(idList,end,start,flag);//起止时间颠倒
                list = getModelList(idList,startTime,endTime,flag);
                logger.debug("获取的记录数量为：{}",list.size());
                builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
            }
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    //根据查询的前十id列表，获取数据列表
    private List<ConsumeModel> getModelList(List<Integer> idList,Date start,Date end,String flag){
        logger.debug("idList：{}",idList);
        List<ConsumeModel> models = new ArrayList<>();
        List<Object[]> tempList = new ArrayList<>();
        for(Integer id:idList){
            if(flag.equals("consume")) {
                tempList = componentConsumeRepository.getData(id, start, end);
            }else if(flag.equals("predict")){
                tempList = componentPredictRepository.getData(id,start,end);
            }

            ConsumeModel model = new ConsumeModel();
            List<List<Long>> data = new ArrayList<>();
            String name = null;
            for(Object[] objects:tempList){
                name = (String)objects[1];

                List<Long> pt = new ArrayList<>();
                Date d = (Date)objects[3];
                Integer num = (Integer)objects[2];
                pt.add(d.getTime());
                pt.add(num.longValue());

                data.add(pt);
            }
            model.setId(id);
            model.setName(name);
            model.setData(data);

            models.add(model);
        }
        return models;
    }

    @RequestMapping(value = "component/destination",method = RequestMethod.GET)
    private ResponseEntity<AxisModel<Date,Integer>> getDestData(@RequestParam("dest")String dest,@RequestParam("count")Integer count){
        ResponseBuilder builder = ResponseBuilder.createBuilder();

        return builder.getResponseEntity();
    }
}
