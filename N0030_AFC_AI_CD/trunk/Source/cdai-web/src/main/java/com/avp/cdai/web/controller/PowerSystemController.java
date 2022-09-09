package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.PowerSystem;
import com.avp.cdai.web.model.EnergyModel;
import com.avp.cdai.web.repository.PowerSystemRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/10.
 */
@RestController
public class PowerSystemController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    PowerSystemRepository powerSystemRepository;

    @RequestMapping(value = "power",method = RequestMethod.GET)
    ResponseEntity<EnergyModel> getData(@RequestParam(value = "time",required = true)Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            Calendar cal = Calendar.getInstance();
            cal.setTime(time);
            cal.add(Calendar.DATE, -6);
            Date date = cal.getTime();
            logger.debug("Date:{}", date);
            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
            String startTime = format1.format(date);
//            Date now = new Date();
            SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
            String endTime = format2.format(time);
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
            Date start = format1.parse(startTime);
            Date end = format.parse(endTime);
            logger.debug("start:{},end:{},str:{} {}",start,end,startTime,endTime);
            List<PowerSystem> list = powerSystemRepository.findByTimestampBetween(start, end);
            logger.debug("获取的供电系统记录数量为：{}",list.size());
            //
            List<String> modeList = new ArrayList<String>();
            List<Date> dateList = new ArrayList<Date>();
            List<Double> dataList = new ArrayList<Double>();
            for(PowerSystem p:list){
                modeList.add(p.getModeName());
                dateList.add(p.getTimestamp());
                dataList.add(p.getConsume().doubleValue());
            }
            EnergyModel energyModel = new EnergyModel();
            energyModel.setModeList(modeList);
            energyModel.setDateList(dateList);
            energyModel.setEnergyList(dataList);
            builder.setResultEntity(energyModel,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
