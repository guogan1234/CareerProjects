package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.*;
import com.avp.cdai.web.repository.LineCumulativeFlowRepository;
import com.avp.cdai.web.repository.ObjLineRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import com.avp.cdai.web.rest.RestBody;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.text.SimpleDateFormat;
import java.util.*;

import static org.springframework.data.jpa.domain.Specifications.where;

/**
 * Created by guo on 2017/8/11.
 */
@RestController
public class LineCumulativeFlowController {
    @Autowired
    LineCumulativeFlowRepository lineCumulativeFlowRepository;
    @Autowired
    ObjLineRepository objLineRepository;

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @RequestMapping(value = "lineCumulativeFlow",method = RequestMethod.GET)
    ResponseEntity<RestBody<LineCumulativeFlow>> findAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
            List<LineCumulativeFlow> list = lineCumulativeFlowRepository.findAll();
            logger.debug("线路客流累计数据条数：{}",list.size());
            builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "viewDataLineSumByConditions",method = RequestMethod.GET)
    ResponseEntity<RestBody<ViewData<ObjLine>>> viewDataFindByConditions(@RequestParam("ids") List<Integer> ids, @RequestParam(value = "direct",required = false) Integer direct, Integer section, Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            Date startTime = null;
            Date endTime = null;
            if (time != null) {
                SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
                String strStart = format1.format(time);
                SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
                String strEnd = format2.format(time);
                logger.debug("viewDataLineSumByConditions--起始时间为：{}，结束时间为：{}", strStart, strEnd);
                SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                try {
                    startTime = format.parse(strStart);
                    endTime = format.parse(strEnd);
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            }
            logger.debug("ids数据量为:({})", ids.size());
            ViewData<ObjLine> viewData = new ViewData<ObjLine>();
            Map<Integer,List<Integer>> flowMap = new HashMap<Integer,List<Integer>>();
            Map<Integer,ObjLine> objMap = new HashMap<Integer,ObjLine>();
            List<Date> dateList = null;
            //取范围较大的时间轴列表
            Integer tempCount = 0;
            for(int i = 0;i<ids.size();i++) {
                List<Integer> temp = new ArrayList<Integer>();
                List<LineCumulativeFlow> list = lineCumulativeFlowRepository.getData(ids.get(i), section,startTime,endTime);
                logger.debug("数据量为:({},{})", list.size(),ids.get(i));
                Integer key = null;
                List<Date> tempDateList = new ArrayList<Date>();
                for (LineCumulativeFlow s:list){
                    temp.add(s.getPassengerFlow());
                    tempDateList.add(s.getFlowTime());
                }
                if(list.size() > tempCount) {
                    tempCount = list.size();
                    dateList = tempDateList;
                }
                flowMap.put(ids.get(i),temp);
            }
            logger.debug("flowMap数据量为:({})", flowMap.size());
//            List<ObjStation> objList = stationShareFlowRepository.getObjData();
            List<ObjLine> objList = objLineRepository.findBylineIdIn(ids);
            logger.debug("ObjStation数据量为:({})", objList.size());
            for (ObjLine obj:objList){
                Integer s_id = obj.getLineId();
                if(s_id == null){
                    logger.debug("s_id为null!");
                }
                objMap.put(s_id,obj);
            }
            viewData.setFlowCountMap(flowMap);
            viewData.setObjMap(objMap);
            viewData.setDateList(dateList);

            builder.setResultEntity(viewData,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        } catch (Exception e) {
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }


    @RequestMapping(value = "lineSumByConditions",method = RequestMethod.GET)
    ResponseEntity<RestBody<LineCumulativeFlow>> findByConditions(@RequestParam("ids") List<Integer> ids, @RequestParam(value = "direct",required = false) Integer direct, Integer section, Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<LineCumulativeFlow> list = lineCumulativeFlowRepository.findAll(where(byConditions(ids, direct,section, time)));
            if (list.size() <= 0) {
                logger.debug("线路客流累计多条件查询失败，结果为空");
            } else {
                logger.debug("线路客流累计多条件查询成功，数据量为:({})", list.size());
            }
            builder.setResultEntity(list, ResponseCode.RETRIEVE_SUCCEED);
        } catch (Exception e) {
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    // Dynamic Query Utils
    public Specification<LineCumulativeFlow> byConditions(List<Integer> ids, Integer direct, Integer section, Date time) {
        return new Specification<LineCumulativeFlow>() {
            public Predicate toPredicate(Root<LineCumulativeFlow> root, CriteriaQuery<?> query, CriteriaBuilder builder) {
                Predicate predicate = builder.conjunction();

                logger.debug("lineSumByConditions请求的参数ids值为:{}", ids);
//                if (ids != null) {
////                    predicate.getExpressions().add(builder.equal(root.get(LineTimeShareFlow_.id), ids));
////                    predicate.getExpressions().add(builder.in(root.get(LineTimeShareFlow_.lineId)).in(ids));
//                    predicate.getExpressions().add(root.<Integer>get(LineCumulativeFlow_.lineId).in(ids));
//                }
////
//                logger.debug("lineSumByConditions请求的参数direct值为:{}", direct);
//                if (direct != null) {
//                    predicate.getExpressions().add(builder.equal(root.get(LineCumulativeFlow_.direction), direct));
//                }
//
//                logger.debug("lineSumByConditions请求的参数section值为:{}", section);
//                if (section != null) {
//                    predicate.getExpressions().add(builder.equal(root.get(LineCumulativeFlow_.section), section));
//                }
//
                logger.debug("lineSumByConditions请求的参数time值为:{}", time);
                if (time != null) {
                    SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
                    String strStart = format1.format(time);
                    SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
                    String strEnd = format2.format(time);
                    logger.debug("起始时间为：{}，结束时间为：{}",strStart,strEnd);
                    SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    Date startTime = null;
                    Date endTime = null;
                    try {
                        startTime = format.parse(strStart);
                        endTime = format.parse(strEnd);
                    }catch (Exception e){
                        logger.error(e.getMessage());
                    }
//                    predicate.getExpressions().add(builder.equal(root.get(LineCumulativeFlow_.flowTime), time));
//                    predicate.getExpressions().add(builder.between(root.get(LineCumulativeFlow_.flowTime),startTime,endTime));
                }

                return predicate;
            }
        };
    }
    // Dynamic End

    @RequestMapping(value = "lineSum",method = RequestMethod.GET)
    ResponseEntity<RestBody<ObjViewData<ObjLine>>> getObject(@RequestParam("ids")List<Integer> ids,Date startTime,Date endTime,@RequestParam("direct")Integer direct){
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
//                logger.debug("lineSum--起始时间为：{}，结束时间为：{}", strStart, strEnd);
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
                    List<LineCumulativeFlow> list = lineCumulativeFlowRepository.getData(id, section, startTime, endTime,direct);
                    for (LineCumulativeFlow ssf : list) {
                        List<Long> valueList = new ArrayList<Long>();
                        Long dateLong = ssf.getFlowTime().getTime();
//                        Date srcDate = ssf.getFlowTime();
//                        //时间平移4小时
//                        Calendar cal = Calendar.getInstance();
//                        cal.setTime(srcDate);
//                        cal.add(Calendar.HOUR,4);
//                        Date dest_date = cal.getTime();
//                        Long dateLong = dest_date.getTime();

                        Long valueLong = ssf.getPassengerFlow().longValue();
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
            builder.setResultEntity(objViewData,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
