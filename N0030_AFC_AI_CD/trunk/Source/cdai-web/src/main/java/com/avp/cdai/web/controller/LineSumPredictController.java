package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.ObjLine;
import com.avp.cdai.web.entity.ObjViewData;
import com.avp.cdai.web.repository.LineSumPredictRepository;
import com.avp.cdai.web.repository.ObjLineRepository;
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

import java.util.*;

/**
 * Created by guo on 2017/9/18.
 */
@RestController
public class LineSumPredictController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    ObjLineRepository objLineRepository;

    @Autowired
    LineSumPredictRepository lineSumPredictRepository;

    @RequestMapping(value = "lineSumPredict",method = RequestMethod.GET)
    ResponseEntity<ObjViewData<ObjLine>> getData(@RequestParam("ids")List<Integer> ids,Date startTime,Date endTime,@RequestParam("direct")Integer direct){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
//            //生成起止时间
//            Date startTime = null;
//            Date endTime = null;
//            if (time != null) {
//                SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
//                String strStart = format1.format(time);
//                SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
//                String strEnd = format2.format(time);
//                logger.debug("lineSumPredict--起始时间为：{}，结束时间为：{}", strStart, strEnd);
//                SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
//                try {
//                    startTime = format.parse(strStart);
//                    endTime = format.parse(strEnd);
//                } catch (Exception e) {
//                    logger.error(e.getMessage());
//                }
//            }
            logger.debug("ids数据量为:({})", ids.size());
            ObjViewData<ObjLine> objViewData = new ObjViewData<ObjLine>();
            Map<Integer,ObjLine> objMap = new HashMap<Integer,ObjLine>();
            List<ObjLine> objList = objLineRepository.findBylineIdIn(ids);
            logger.debug("ObjLine数据量为:({})", objList.size());
            for (ObjLine obj : objList) {
                Integer id = obj.getLineId();
                if (id == null) {
                    logger.debug("s_id为null!");
                }
                List<List<Long>> dataList1 = new ArrayList<List<Long>>();
                List<List<Long>> dataList2 = new ArrayList<List<Long>>();
                List<List<Long>> dataList3 = new ArrayList<List<Long>>();
                for (int i = 0; i < 3; i++) {
                    Integer section = 0;
                    if (i == 0) {
                        section = 10;
                    } else if (i == 1) {
                        section = 30;
                    } else if (i == 2) {
                        section = 60;
                    }
                    List<Object[]> list = lineSumPredictRepository.getData(id,section,startTime,endTime,direct);
                    for (Object[] objects : list) {
                        List<Long> valueList = new ArrayList<Long>();
                        Integer value = (Integer)objects[0];
                        Long valueLong = value.longValue();

//                        Date srcDate = (Date)objects[1];
//                        //时间平移4小时
//                        Calendar cal = Calendar.getInstance();
//                        cal.setTime(srcDate);
//                        cal.add(Calendar.HOUR,4);
//                        Date dest_date = cal.getTime();
//                        Long dateLong = dest_date.getTime();
                        Date d = (Date)objects[1];
                        Long dateLong = d.getTime();
                        valueList.add(dateLong);
                        valueList.add(valueLong);

                        if (i == 0) {
                            dataList1.add(valueList);
                        } else if (i == 1) {
                            dataList2.add(valueList);
                        } else if (i == 2) {
                            dataList3.add(valueList);
                        }
                    }
                    logger.debug("{}:数据量为:({},{})", i, list.size(), id);
                }
                obj.setDataList1(dataList1);
                obj.setDataList2(dataList2);
                obj.setDataList3(dataList3);

                objMap.put(id,obj);
            }
            objViewData.setObjMap(objMap);
            builder.setResultEntity(objViewData, ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
