package com.avp.cdai.web.controller;

import com.avp.cdai.web.model.AxisModel;
import com.avp.cdai.web.repository.FailureReasonRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.math.BigInteger;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/25.
 */
@RestController
public class FailureReasonController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    FailureReasonRepository failureReasonRepository;

    //对于数组参数的传递（如List<Integer>），一定要加@RequestParam(" ")，否则传入数组size=0
    //private ResponseEntity<AxisModel<Date,Double>> getList(@PathVariable("flag")String flag, @RequestParam("type")Integer deviceType, @RequestParam("deviceTypeList") List<Integer> deviceTypeList,@RequestParam("moduleId")Integer moduleId,@RequestParam("location")String location,@RequestParam(value = "id",required = false)Integer id,Integer count,@RequestParam("startTime")Date start,@RequestParam("endTime")Date end){

    @RequestMapping(value = "failureReason/{flag}",method = RequestMethod.GET)
    private ResponseEntity<AxisModel<Date,Double>> getList(@PathVariable("flag")String flag, Integer deviceType,@RequestParam("moduleId")Integer moduleId,@RequestParam("location")String location,@RequestParam(value = "id",required = false)Integer id,Integer count,@RequestParam("startTime")Date start,@RequestParam("endTime")Date end){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
//            //获取起止时间
//            logger.debug("tagFailed...{}", flag);
//            Calendar cal = Calendar.getInstance();
//            cal.add(Calendar.MONTH, -count);
//            Date date = cal.getTime();
//            logger.debug("Date:{}", date);
//            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
//            String startTime = format1.format(date);
//            Date now = new Date();
//            String endTime = format1.format(now);
//            Date start = format1.parse(startTime);
//            Date end = format1.parse(endTime);
//            logger.debug("获取的起止时间为{},{}", start, end);
            //
            AxisModel<Date,Double> model = new AxisModel<Date,Double>();
            //可封装
            if(flag.equals("section")){
                if(location.equals("default")){
                    List<Object[]> list = failureReasonRepository.getDefaultListBySection(deviceType,moduleId,start,end);
                    logger.debug("section.default -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("line")){
                    logger.debug("id:{}",id);
                    List<Object[]> list = failureReasonRepository.getLineListBySection(deviceType,moduleId,id,start,end);
                    logger.debug("section.line -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }
                else if(location.equals("station")){
                    List<Object[]> list = failureReasonRepository.getStationListBySection(deviceType,moduleId,id,start,end);
                    logger.debug("section.station -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("device")){
                    List<Object[]> list = failureReasonRepository.getDeviceListBySection(deviceType,moduleId,id,start,end);
                    logger.debug("section.device -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("linesWithModule")){
                    List<Object[]> list = failureReasonRepository.getAllLinesBySection(moduleId,start,end);
                    logger.debug("section.linesWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("lineWithModule")){
                    logger.debug("id:{}",id);
                    List<Object[]> list = failureReasonRepository.getLineListBySection(moduleId,id,start,end);
                    logger.debug("section.lineWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("stationWithModule")){
                    List<Object[]> list = failureReasonRepository.getStationListBySection(moduleId,id,start,end);
                    logger.debug("section.stationWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("deviceWithModule")){
                    List<Object[]> list = failureReasonRepository.getDeviceListBySection(moduleId,id,start,end);
                    logger.debug("section.deviceWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }
            }else if(flag.equals("count")){
                if(location.equals("default")){
                    List<Object[]> list = failureReasonRepository.getDefaultListByCount(deviceType,moduleId,start,end);
                    logger.debug("count.default -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("line")){
                    List<Object[]> list = failureReasonRepository.getLineListByCount(deviceType,moduleId,id,start,end);
                    logger.debug("count.line -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("station")){
                    List<Object[]> list = failureReasonRepository.getStationListByCount(deviceType,moduleId,id,start,end);
                    logger.debug("count.station -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("device")){
                    List<Object[]> list = failureReasonRepository.getDeviceListByCount(deviceType,moduleId,id,start,end);
                    logger.debug("count.device -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("lineWithModule")){
                    List<Object[]> list = failureReasonRepository.getLineListByCount(moduleId,id,start,end);
                    logger.debug("count.lineWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("stationWithModule")){
                    List<Object[]> list = failureReasonRepository.getStationListByCount(moduleId,id,start,end);
                    logger.debug("count.stationWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("deviceWithModule")){
                    List<Object[]> list = failureReasonRepository.getDeviceListByCount(moduleId,id,start,end);
                    logger.debug("count.deviceWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("linesWithModule")){
                    List<Object[]> list = failureReasonRepository.getAllLinesByCount(moduleId,start,end);
                    logger.debug("count.linesWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }
            }
            else if(flag.equals("repair")){
                if(location.equals("lineWithModule")){
                    List<Object[]> list = failureReasonRepository.getLineListByRepair(moduleId,id,start,end);
                    logger.debug("repair.lineWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("stationWithModule")){
                    List<Object[]> list = failureReasonRepository.getStationListByRepair(moduleId,id,start,end);
                    logger.debug("repair.stationWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("deviceWithModule")){
                    List<Object[]> list = failureReasonRepository.getDeviceListByRepair(moduleId,id,start,end);
                    logger.debug("repair.deviceWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }else if(location.equals("linesWithModule")){
                    List<Object[]> list = failureReasonRepository.getAllLinesByRepair(moduleId,start,end);
                    logger.debug("repair.linesWithModule -- 获取的记录数量为：{}",list.size());
                    List<Date> xValue = new ArrayList<>();
                    List<Double> yValue = new ArrayList<>();
                    ParseData(list,xValue,yValue,flag);

                    model.setxValue(xValue);
                    model.setyValue(yValue);
                }
            }
            builder.setResultEntity(model,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    //填充xList，yList
    //输入 -- list
    //输出 -- xList，yList
    private void ParseData(List<Object[]> list,List<Date> xList,List<Double> yList,String flag){
        for(Object[] objects:list){
            Date d = (Date) objects[0];
            BigInteger b = (BigInteger)objects[1];
            BigInteger b2 = (BigInteger)objects[2];
            Double b_dou = b.doubleValue();
            Double b2_dou = b2.doubleValue();
//            BigInteger v = null;//除法无法精确到小数点，得到的结果都为整数
            Double v = null;
            if(flag.equals("section")){
                if(b2.equals(0)){
                    //除法分母不能为0
                    logger.error("除法分母不能为0");
                }else {
//                    v = b.divide(b2);
                    v = b_dou/b2_dou;
                }
            }else if(flag.equals("count")){
                if(b2.equals(0)){
                    //除法分母不能为0
                    logger.error("除法分母不能为0");
                }else {
//                    v = b.divide(b2);
                    v = b_dou/b2_dou;
                }
            }else if(flag.equals("repair")){
                if(b2.equals(0)){
                    //除法分母不能为0
                    logger.error("除法分母不能为0");
                }else {
//                    v = b.divide(b2);
                    v = b_dou/b2_dou;
                }
            }
            //double格式化输出
            DecimalFormat formatter = new DecimalFormat("##.##");
            String strD = formatter.format(v);
            double dValue = Double.valueOf(strD).doubleValue();

            xList.add(d);
            yList.add(dValue);
        }
    }
}
