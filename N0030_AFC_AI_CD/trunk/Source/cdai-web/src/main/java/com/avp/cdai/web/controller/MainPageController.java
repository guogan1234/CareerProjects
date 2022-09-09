package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.LineTimeShareFlow;
import com.avp.cdai.web.model.IncontinueTimeModel;
import com.avp.cdai.web.repository.LineSharePredictRepository;
import com.avp.cdai.web.repository.LineTimeShareFlowRepository;
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
 * Created by guo on 2018/1/2.
 */
@RestController
public class MainPageController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    LineTimeShareFlowRepository lineTimeShareFlowRepository;

    @Autowired
    LineSharePredictRepository lineSharePredictRepository;

    @RequestMapping(value = "compareByLine",method = RequestMethod.GET)
    ResponseEntity<IncontinueTimeModel> compareByLine(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<IncontinueTimeModel> listModel = new ArrayList<IncontinueTimeModel>();
            //指定查询的车站和进出站
            Integer lineId = 8;
            Integer direct = 1;
            Integer section = 10;
            //获取昨天的客流数据
            Calendar cal = Calendar.getInstance();
            cal.add(Calendar.DAY_OF_MONTH, -1);
            Date date1 = cal.getTime();
            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
            String strStart = format1.format(date1);
            SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
            String strEnd = format2.format(date1);
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            Date startTime = format.parse(strStart);
            Date endTime = format.parse(strEnd);
            List<LineTimeShareFlow> list1 = lineTimeShareFlowRepository.getData(lineId,section,startTime,endTime,direct);
            logger.debug("主页面分时客流记录数量为：{}",list1.size());

            //获取今天的客流预测数据，并把时间平移为昨天
            Calendar cal2 = Calendar.getInstance();
            Date date2 = cal2.getTime();
            String strStart2 = format1.format(date2);
            String strEnd2 = format2.format(date2);
            Date start = format.parse(strStart2);
            Date end = format.parse(strEnd2);
            logger.debug("start:{},end:{}",start,end);
            List<Object[]> list2 = lineSharePredictRepository.getData2(lineId,start,end,section,direct);
            logger.debug("主页面分时客流预测记录数量为：{}",list2.size());

            //
            IncontinueTimeModel model1 = new IncontinueTimeModel();
            List<List<Long>> data1 = new ArrayList<List<Long>>();
            for(LineTimeShareFlow temp1:list1){
                Integer flowCount = temp1.getPassengerFlow();
                Date timeStamp = temp1.getTimestamp();

                List<Long> subData1 = new ArrayList<Long>();
                subData1.add(timeStamp.getTime());
                subData1.add(flowCount.longValue());

                data1.add(subData1);
            }
            model1.setName("分时客流");
            model1.setData(data1);

            IncontinueTimeModel model2 = new IncontinueTimeModel();
            List<List<Long>> data2 = new ArrayList<List<Long>>();
            for(Object[] objects:list2){
                Integer flowCount = (Integer) objects[0];
                Date timeStamp = (Date) objects[1];
                //时间平移为昨天
                Calendar c = Calendar.getInstance();
                c.setTime(timeStamp);
                c.add(Calendar.DAY_OF_MONTH, -1);
                Date destTime = c.getTime();

                List<Long> subData1 = new ArrayList<Long>();
                subData1.add(destTime.getTime());
                subData1.add(flowCount.longValue());

                data2.add(subData1);
            }
            model2.setName("分时客流预测");
            model2.setData(data2);

            listModel.add(model1);
            listModel.add(model2);

            builder.setResultEntity(listModel,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
