package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.DeviceDownTime;
import com.avp.cdai.web.model.AxisModel;
import com.avp.cdai.web.repository.DeviceDownTimeRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/23.
 */
@RestController
public class DeviceDownTimeController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    DeviceDownTimeRepository deviceDownTimeRepository;

    @RequestMapping(value = "deviceDown/{flag}",method = RequestMethod.GET)
    private ResponseEntity<AxisModel<Integer,Double>> getData(@PathVariable("flag")String flag, @RequestParam("location")String type, @RequestParam("id")Integer id,Date startTime,Date endTime,Integer count,@RequestParam("page")Integer page,@RequestParam("pageSize")Integer pageSize){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
//            //获取起止时间
//            logger.debug("tagFailed...{}", flag);
//            Calendar cal = Calendar.getInstance();
//            cal.add(Calendar.MONTH, -count);
//            Date date = cal.getTime();
//            logger.debug("Date:{}", date);
//            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
//            String startTimeStr = format1.format(date);
//            Date now = new Date();
//            String endTimeStr = format1.format(now);
//            Date startTime = format1.parse(startTimeStr);
//            Date endTime = format1.parse(endTimeStr);
//            logger.debug("获取的起止时间为{},{}", startTime, endTime);
            //
            List<DeviceDownTime> list = null;
            if(flag.equals("count")){
                if(type.equals("line")) {
                    list = deviceDownTimeRepository.getCountListByLine(id,startTime,endTime);
                    logger.debug("获取的记录数量为：{}",list.size());
                }else if(type.equals("station")){
                    list = deviceDownTimeRepository.getCountListByStation(id,startTime,endTime);
                    logger.debug("获取的记录数量为：{}",list.size());
                }
                if(list != null){
                    AxisModel<Integer,Double> model = new AxisModel<Integer,Double>();
                    List<Integer> deviceIds = new ArrayList<>();
                    List<Double> values = new ArrayList<>();
                    for(int i = 0;i<list.size();i++){
                        //获取前10的数据
                        if(i >= pageSize)
                            break;
                        DeviceDownTime obj = list.get(i);
                        deviceIds.add(obj.getDeviceId());
                        values.add(obj.getFailureCount().doubleValue());
                    }
                    model.setxValue(deviceIds);
                    model.setyValue(values);

                    builder.setResultEntity(model, ResponseCode.RETRIEVE_SUCCEED);
                    return builder.getResponseEntity();
                }
            }else if(flag.equals("percent")){
                if(type.equals("line")){
                    list = deviceDownTimeRepository.getPercentListByLine(id,startTime,endTime);
                    logger.debug("获取的记录数量为：{}",list.size());
                }else if(type.equals("station")){
                    list = deviceDownTimeRepository.getPercentListByStation(id,startTime,endTime);
                    logger.debug("获取的记录数量为：{}",list.size());
                }
                if(list != null){
                    AxisModel<Integer,Double> model = new AxisModel<Integer,Double>();
                    List<Integer> deviceIds = new ArrayList<>();
                    List<Double> values = new ArrayList<>();
                    for(int i = 0;i<list.size();i++){
                        if(i >= pageSize)
                            break;
                        DeviceDownTime obj = list.get(i);
                        deviceIds.add(obj.getDeviceId());
                        Double value = obj.getRate()*100;
                        //double格式化输出
                        DecimalFormat formatter = new DecimalFormat("##.#");
                        String strD = formatter.format(value);
                        double dValue = Double.valueOf(strD).doubleValue();
                        values.add(dValue);
                    }
                    model.setxValue(deviceIds);
                    model.setyValue(values);

                    builder.setResultEntity(model, ResponseCode.RETRIEVE_SUCCEED);
                    return builder.getResponseEntity();
                }
            }

        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
