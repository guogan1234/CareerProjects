package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.*;
import com.avp.cdai.web.repository.ObjStationRepository;
import com.avp.cdai.web.repository.StationCumulativeFlowRepository;
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
 * Created by guo on 2017/8/15.
 */
@RestController
public class StationCumulativeFlowController {
    @Autowired
    StationCumulativeFlowRepository stationCumulativeFlowRepository;
    @Autowired
    ObjStationRepository objStationRepository;

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @RequestMapping(value = "stationSumFlow",method = RequestMethod.GET)
    ResponseEntity<RestBody<StationCumulativeFlow>> findAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<StationCumulativeFlow> list = stationCumulativeFlowRepository.findAll();
            logger.debug("车站客流累计查询记录数量为：{}",list.size());
            builder.setResultEntity(list, ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "viewDataSumFindByConditions",method = RequestMethod.GET)
    ResponseEntity<RestBody<ViewData<ObjStation>>> viewDataFindByConditions(@RequestParam("ids") List<Integer> ids, @RequestParam(value = "direct",required = false) Integer direct, Integer section, Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            Date startTime = null;
            Date endTime = null;
            if (time != null) {
                SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
                String strStart = format1.format(time);
                SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
                String strEnd = format2.format(time);
                logger.debug("viewDataSumFindByConditions--起始时间为：{}，结束时间为：{}", strStart, strEnd);
                SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                try {
                    startTime = format.parse(strStart);
                    endTime = format.parse(strEnd);
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            }
            logger.debug("ids数据量为:({})", ids.size());
            ViewData<ObjStation> viewData = new ViewData<ObjStation>();
            Map<Integer,List<Integer>> flowMap = new HashMap<Integer,List<Integer>>();
            Map<Integer,ObjStation> objMap = new HashMap<Integer,ObjStation>();
            List<Date> dateList = null;
            //取范围较大的时间轴列表
            Integer tempCount = 0;
            for(int i = 0;i<ids.size();i++) {
                List<Integer> temp = new ArrayList<Integer>();
                List<StationCumulativeFlow> list = stationCumulativeFlowRepository.getData(ids.get(i), section,startTime,endTime);
                logger.debug("数据量为:{},id为:{}", list.size(),ids.get(i));
                Integer key = null;
                List<Date> tempDateList = new ArrayList<Date>();
                for (StationCumulativeFlow s:list){
                    temp.add(s.getFlowCount());
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
            List<ObjStation> objList = objStationRepository.findBystationIdIn(ids);
            logger.debug("ObjStation数据量为:({})", objList.size());
            for (ObjStation obj:objList){
                Integer s_id = obj.getStationId();
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


    @RequestMapping(value = "stationSumByConditions",method = RequestMethod.GET)
    ResponseEntity<RestBody<StationCumulativeFlow>> findByConditions(@RequestParam("ids") List<Integer> ids, @RequestParam(value = "direct",required = false) Integer direct, Integer section, Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<StationCumulativeFlow> list = stationCumulativeFlowRepository.findAll(where(byConditions(ids, direct,section, time)));
            if (list.size() <= 0) {
                logger.debug("车站客流累计多条件查询失败，结果为空");
            } else {
                logger.debug("车站客流累计多条件查询成功，数据量为:({})", list.size());
            }
            builder.setResultEntity(list, ResponseCode.RETRIEVE_SUCCEED);
        } catch (Exception e) {
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    // Dynamic Query Utils
    public Specification<StationCumulativeFlow> byConditions(List<Integer> ids, Integer direct, Integer section, Date time) {
        return new Specification<StationCumulativeFlow>() {
            public Predicate toPredicate(Root<StationCumulativeFlow> root, CriteriaQuery<?> query, CriteriaBuilder builder) {
                Predicate predicate = builder.conjunction();

                logger.debug("stationSumByConditions请求的参数ids值为:{}", ids);
//                if (ids != null) {
////                    predicate.getExpressions().add(builder.equal(root.get(LineTimeShareFlow_.id), ids));
////                    predicate.getExpressions().add(builder.in(root.get(LineTimeShareFlow_.lineId)).in(ids));
//                    predicate.getExpressions().add(root.<Integer>get(StationCumulativeFlow_.stationId).in(ids));
//                }
////
//                logger.debug("stationSumByConditions请求的参数direct值为:{}", direct);
//                if (direct != null) {
//                    predicate.getExpressions().add(builder.equal(root.get(StationCumulativeFlow_.direction), direct));
//                }
//
//                logger.debug("stationSumByConditions请求的参数section值为:{}", section);
//                if (section != null) {
//                    predicate.getExpressions().add(builder.equal(root.get(StationCumulativeFlow_.section), section));
//                }
//
                logger.debug("stationSumByConditions请求的参数time值为:{}", time);
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
//                    predicate.getExpressions().add(builder.equal(root.get(StationCumulativeFlow_.flowTime), time));
//                    predicate.getExpressions().add(builder.between(root.get(StationCumulativeFlow_.flowTime),startTime,endTime));
                }

                return predicate;
            }
        };
    }
    // Dynamic End

    @RequestMapping(value = "ViewData3_StationSum",method = RequestMethod.GET)
    ResponseEntity<RestBody<ViewDataModel3<ObjStation,Integer>>> getData(@RequestParam("ids") List<Integer> ids, @RequestParam(value = "direct",required = false) Integer direct, Integer section, Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            //生成起止时间
            Date startTime = null;
            Date endTime = null;
            if (time != null) {
                SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
                String strStart = format1.format(time);
                SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
                String strEnd = format2.format(time);
                logger.debug("ViewData3_StationSum--起始时间为：{}，结束时间为：{}", strStart, strEnd);
                SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                try {
                    startTime = format.parse(strStart);
                    endTime = format.parse(strEnd);
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            }
            logger.debug("ids数据量为:({})", ids.size());
            //
            ViewDataModel3<ObjStation, Integer> viewDataModel3 = new ViewDataModel3<ObjStation, Integer>();
            Map<Integer, List<DatePoint<Integer>>> data3Map = new HashMap<Integer, List<DatePoint<Integer>>>();
            for (Integer id : ids) {
                List<StationCumulativeFlow> list = stationCumulativeFlowRepository.getData(id, section, startTime, endTime);
                List<DatePoint<Integer>> listDataPoint = new ArrayList<DatePoint<Integer>>();
                for (StationCumulativeFlow ssf : list) {
                    DatePoint<Integer> point = new DatePoint<Integer>(ssf.getFlowTime(), ssf.getFlowCount());
                    listDataPoint.add(point);
                }
                data3Map.put(id, listDataPoint);
                logger.debug("数据量为:({},{})", list.size(), id);
            }
            Map<Integer, ObjStation> objMap = new HashMap<Integer, ObjStation>();
            List<ObjStation> objList = objStationRepository.findBystationIdIn(ids);
            logger.debug("ObjStation数据量为:({})", objList.size());
            for (ObjStation obj : objList) {
                Integer s_id = obj.getStationId();
                if (s_id == null) {
                    logger.debug("s_id为null!");
                }
                objMap.put(s_id, obj);
            }
            viewDataModel3.setData3Map(data3Map);
            viewDataModel3.setObjMap(objMap);

            builder.setResultEntity(viewDataModel3, ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "stationSum",method = RequestMethod.GET)
    ResponseEntity<RestBody<ObjViewData<ObjStation>>> getObject(@RequestParam("ids")List<Integer> ids,Date startTime,Date endTime,@RequestParam("direct")Integer direct){
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
//                logger.debug("stationSum--起始时间为：{}，结束时间为：{}", strStart, strEnd);
//                SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
//                try {
//                    startTime = format.parse(strStart);
//                    endTime = format.parse(strEnd);
//                } catch (Exception e) {
//                    logger.error(e.getMessage());
//                }
//            }
            logger.debug("ids数据量为:({})", ids.size());
            ObjViewData<ObjStation> objViewData = new ObjViewData<ObjStation>();
            Map<Integer,ObjStation> objMap = new HashMap<Integer,ObjStation>();
            List<ObjStation> objList = objStationRepository.findBystationIdIn(ids);
            logger.debug("ObjStation数据量为:({})", objList.size());
            for (ObjStation obj : objList) {
                Integer id = obj.getStationId();
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
                    List<StationCumulativeFlow> list = stationCumulativeFlowRepository.getData(id, section, startTime, endTime,direct);
                    for (StationCumulativeFlow ssf : list) {
                        List<Long> valueList = new ArrayList<Long>();
                        Long dateLong = ssf.getFlowTime().getTime();
//                        Date srcDate = ssf.getFlowTime();
//                        //时间平移4小时
//                        Calendar cal = Calendar.getInstance();
//                        cal.setTime(srcDate);
//                        cal.add(Calendar.HOUR,4);
//                        Date dest_date = cal.getTime();
//                        Long dateLong = dest_date.getTime();

                        Long valueLong = ssf.getFlowCount().longValue();
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
