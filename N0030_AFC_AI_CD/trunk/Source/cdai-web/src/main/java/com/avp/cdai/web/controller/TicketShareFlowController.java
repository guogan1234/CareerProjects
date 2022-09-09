package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.*;
import com.avp.cdai.web.repository.TicketShareFlowRepository;
import com.avp.cdai.web.repository.TicketTypeRepository;
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
import java.math.BigInteger;
import java.text.SimpleDateFormat;
import java.util.*;

import static org.springframework.data.jpa.domain.Specifications.where;

/**
 * Created by guo on 2017/8/16.
 */
@RestController
public class TicketShareFlowController {
    @Autowired
    TicketShareFlowRepository ticketShareFlowRepository;
    @Autowired
    TicketTypeRepository ticketTypeRepository;

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @RequestMapping(value = "ticketShareFlow",method = RequestMethod.GET)
    ResponseEntity<RestBody<TicketShareFlow>> findAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<TicketShareFlow> list = ticketShareFlowRepository.findAll();
            logger.debug("票卡客流分时记录查询数量为：{}",list.size());
            builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "viewDataTicketShareByConditions",method = RequestMethod.GET)
    ResponseEntity<RestBody<ViewData<TicketType>>> findByConditions(@RequestParam("time")Date time,@RequestParam("section")Integer section){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
//        List<TicketShareFlow> list = ticketShareFlowRepository.getData2(section,id);
        Date startTime = null;
        Date endTime = null;
        if (time != null) {
            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
            String strStart = format1.format(time);
            SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
            String strEnd = format2.format(time);
            logger.debug("viewDataStationShareByConditions--起始时间为：{}，结束时间为：{}", strStart, strEnd);
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            try {
                startTime = format.parse(strStart);
                endTime = format.parse(strEnd);
            } catch (Exception e) {
                logger.error(e.getMessage());
            }
        }
        //定义数据
        ViewData<TicketType> viewData = new ViewData<TicketType>();
        Map<Integer,List<Integer>> flowMap = new HashMap<Integer,List<Integer>>();
        Map<Integer,TicketType> objMap = new HashMap<Integer,TicketType>();
        List<Date> dateList = null;
        //获取票卡列表
        List<TicketType> ticketList = ticketTypeRepository.findAll();
        logger.debug("ticketList--票卡数量为:{}",ticketList.size());
        Integer max = 0;
        for (TicketType t:ticketList) {
            Integer id = t.getTicketId();
            objMap.put(id, t);

            List<Integer> tempFlow = new ArrayList<Integer>();
            //返回Object[]查询结果的解析方式
            //返回List<Object[]>查询结果的解析方式
//            logger.debug("查询参数--id:{},section:{},start:{},end:{}", id, section, startTime, endTime);
            List<Object[]> list = ticketShareFlowRepository.getData4(id, section, startTime, endTime);
//            logger.debug("TicketShare--List数量为:{}", list.size());
            Integer size = list.size();

            List<Date> tempDate = new ArrayList<Date>();
            for (int j = 0; j < size; j++) {
                Object[] objList = list.get(j);
                for (int i = 0; i < objList.length; i++) {
                    if (size > max){
                        if (i == 0) {
                            Date d = (Date) objList[i];
                            tempDate.add(d);
                        }
                    }
                    if (i == 1) {
                        BigInteger count = (BigInteger) objList[i];
                        tempFlow.add(count.intValue());
                    }
                }
            }
            flowMap.put(id, tempFlow);
            if (size > max){
                max = size;
                dateList = tempDate;
            }
        }
        viewData.setFlowCountMap(flowMap);
        viewData.setObjMap(objMap);
        viewData.setDateList(dateList);
        builder.setResultEntity(viewData,ResponseCode.RETRIEVE_SUCCEED);
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "viewDataTicketShareByConditionsFinal",method = RequestMethod.GET)
    ResponseEntity<RestBody<ViewData<TicketType>>> findByConditions2(@RequestParam("time")Date time,@RequestParam("section")Integer section){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        //获取起止时间
        Date startTime = null;
        Date endTime = null;
        if (time != null) {
            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
            String strStart = format1.format(time);
            SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
            String strEnd = format2.format(time);
            logger.debug("viewDataStationShareByConditions--起始时间为：{}，结束时间为：{}", strStart, strEnd);
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            try {
                startTime = format.parse(strStart);
                endTime = format.parse(strEnd);
            } catch (Exception e) {
                logger.error(e.getMessage());
            }
        }
        //定义数据
        ViewData<TicketType> viewData = new ViewData<TicketType>();
        Map<Integer,List<Integer>> flowMap = new HashMap<Integer,List<Integer>>();
        Map<Integer,TicketType> objMap = new HashMap<Integer,TicketType>();
        List<Date> dateList = null;
        //获取票卡列表
        List<TicketType> ticketList = ticketTypeRepository.findAll();
        logger.debug("ticketList--票卡数量为:{}",ticketList.size());
        List<Object[]> list = ticketShareFlowRepository.getAllData(section,startTime,endTime);
        Integer max = 0;
        for(TicketType t:ticketList){
            Integer key = t.getTicketId();
            List<Integer> tempList = new ArrayList<Integer>();
            List<Date> tempDate = new ArrayList<Date>();
            objMap.put(key,t);
            for(int i = 0;i<list.size();i++){
                Object[] obj = list.get(i);
                if(obj.length!=3){
                    //数据长度不对
                }
                Integer dataKey = (Integer) obj[2];
                if(dataKey == key){
                    BigInteger count = (BigInteger) obj[1];
                    tempList.add(count.intValue());
                    Date date = (Date)obj[0];
                    tempDate.add(date);
                }
            }
            if(tempDate.size()>max){
                max = tempDate.size();
                dateList = tempDate;
            }
            flowMap.put(key,tempList);
        }
        //填充模型
        viewData.setFlowCountMap(flowMap);
        viewData.setObjMap(objMap);
        viewData.setDateList(dateList);

        builder.setResultEntity(viewData,ResponseCode.RETRIEVE_SUCCEED);
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "viewDataWithZeroTicketShare",method = RequestMethod.GET)
    ResponseEntity<RestBody<ViewDataWithZero<Integer,TicketType>>> findData(@RequestParam("time")Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        //获取起止时间
        Date startTime = null;
        Date endTime = null;
        if (time != null) {
            SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
            String strStart = format1.format(time);
            SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
            String strEnd = format2.format(time);
            logger.debug("viewDataStationShareByConditions--起始时间为：{}，结束时间为：{}", strStart, strEnd);
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            try {
                startTime = format.parse(strStart);
                endTime = format.parse(strEnd);
            } catch (Exception e) {
                logger.error(e.getMessage());
            }
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "ticketShareByConditions",method = RequestMethod.GET)
    ResponseEntity<RestBody<TicketShareFlow>> findByConditions(@RequestParam("ids") List<Integer> ids,@RequestParam(value = "lineIds",required = false) List<Integer> lineIds,@RequestParam(value = "stationIds",required = false) List<Integer> stationIds, @RequestParam(value = "direct",required = false) Integer direct, Integer section, Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<TicketShareFlow> list = ticketShareFlowRepository.findAll(where(byConditions(ids,lineIds,stationIds, direct,section, time)));
            if (list.size() <= 0) {
                logger.debug("票卡客流分时多条件查询失败，结果为空");
            } else {
                logger.debug("票卡客流分时多条件查询成功，数据量为:({})", list.size());
            }
            builder.setResultEntity(list, ResponseCode.RETRIEVE_SUCCEED);
        } catch (Exception e) {
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    // Dynamic Query Utils
    public Specification<TicketShareFlow> byConditions(List<Integer> ids,List<Integer> lineIds, List<Integer> stationIds,Integer direct, Integer section, Date time) {
        return new Specification<TicketShareFlow>() {
            public Predicate toPredicate(Root<TicketShareFlow> root, CriteriaQuery<?> query, CriteriaBuilder builder) {
                Predicate predicate = builder.conjunction();

                logger.debug("ticketShareByConditions请求的参数ids值为:{}", ids);
//                if (ids != null) {
////                    predicate.getExpressions().add(builder.equal(root.get(LineTimeShareFlow_.id), ids));
////                    predicate.getExpressions().add(builder.in(root.get(LineTimeShareFlow_.lineId)).in(ids));
//                    predicate.getExpressions().add(root.<Integer>get(TicketShareFlow_.ticketId).in(ids));
//                }
//                logger.debug("ticketShareByConditions请求的参数lineIds值为:{}", lineIds);
//                if (lineIds != null) {
////                    predicate.getExpressions().add(builder.equal(root.get(LineTimeShareFlow_.id), ids));
////                    predicate.getExpressions().add(builder.in(root.get(LineTimeShareFlow_.lineId)).in(ids));
//                    predicate.getExpressions().add(root.<Integer>get(TicketShareFlow_.lineId).in(lineIds));
//                }
//                logger.debug("ticketShareByConditions请求的参数stationIds值为:{}", stationIds);
//                if (stationIds != null) {
////                    predicate.getExpressions().add(builder.equal(root.get(LineTimeShareFlow_.id), ids));
////                    predicate.getExpressions().add(builder.in(root.get(LineTimeShareFlow_.lineId)).in(ids));
//                    predicate.getExpressions().add(root.<Integer>get(TicketShareFlow_.stationId).in(stationIds));
//                }
////
//                logger.debug("ticketShareByConditions请求的参数direct值为:{}", direct);
//                if (direct != null) {
//                    predicate.getExpressions().add(builder.equal(root.get(TicketShareFlow_.direction), direct));
//                }
//
//                logger.debug("ticketShareByConditions请求的参数section值为:{}", section);
//                if (section != null) {
//                    predicate.getExpressions().add(builder.equal(root.get(TicketShareFlow_.section), section));
//                }
//
                logger.debug("ticketShareByConditions请求的参数time值为:{}", time);
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
//                    predicate.getExpressions().add(builder.between(root.get(TicketShareFlow_.timestamp),startTime,endTime));
                }

                return predicate;
            }
        };
    }
    // Dynamic End

    @RequestMapping(value = "ObjTicketShare",method = RequestMethod.GET)
    ResponseEntity<RestBody<ObjViewData<TicketType>>> getObject(Date time){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
            //获取起止时间
            Date startTime = null;
            Date endTime = null;
            if (time != null) {
                SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
                String strStart = format1.format(time);
                SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
                String strEnd = format2.format(time);
                logger.debug("ObjTicketShare--起始时间为：{}，结束时间为：{}", strStart, strEnd);
                SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                try {
                    startTime = format.parse(strStart);
                    endTime = format.parse(strEnd);
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            }
            //
            ObjViewData<TicketType> objViewData = new ObjViewData<TicketType>();
            Map<Integer,TicketType> objMap = new HashMap<Integer,TicketType>();
            //获取票卡列表
//            List<TicketType> ticketList = ticketTypeRepository.findAll();
            //获取票卡分时客流前10的ticketId集合
            List<Integer> ticketIds = new ArrayList<Integer>();
            List<Object[]> objectList = ticketShareFlowRepository.getTopData(60,startTime,endTime);
            for(int i = 0;i<10;i++){
                Object[] objects = objectList.get(i);
                Integer id = (Integer) objects[0];
                ticketIds.add(id);
            }
            logger.debug("票卡前10的id集合：{}",ticketIds);
            List<TicketType> ticketList = ticketTypeRepository.findByTicketIdIn(ticketIds);
            logger.debug("ticketList--票卡数量为:{}",ticketList.size());
            for(TicketType obj:ticketList){
                Integer key = obj.getTicketId();

                //所取的票卡数据id。是否等于obj的id
                boolean b = false;
                //只处理前10的票卡数据
                List<List<Long>> dataList1 = new ArrayList<List<Long>>();
                List<List<Long>> dataList2 = new ArrayList<List<Long>>();
                List<List<Long>> dataList3 = new ArrayList<List<Long>>();
                for(int i = 0;i<3;i++){
                    Integer section = 0;
                    if (i == 0) {
                        section = 10;
                    } else if (i == 1) {
                        section = 30;
                    } else if (i == 2) {
                        section = 60;
                    }
                    List<Object[]> objDatas = ticketShareFlowRepository.getObjectData(ticketIds,section,startTime,endTime);
                    logger.debug("获取票卡分时记录，数量为:{}",objDatas.size());
                    for(Object[] objects:objDatas){
                        Integer id = (Integer)objects[2];
                        Date dateLong = (Date) objects[0];
                        BigInteger count = (BigInteger)objects[1];
                        if(id == key){
                            b = true;
                            List<Long> pt = new ArrayList<Long>();
                            pt.add(dateLong.getTime());
                            pt.add(count.longValue());

                            if (i == 0) {
                                dataList1.add(pt);
                            } else if (i == 1) {
                                dataList2.add(pt);
                            } else if (i == 2) {
                                dataList3.add(pt);
                            }
                        }
                    }
                }
                obj.setDataList1(dataList1);
                obj.setDataList2(dataList2);
                obj.setDataList3(dataList3);

                if(b) {
                    objMap.put(key, obj);
                }
            }
            objViewData.setObjMap(objMap);
            builder.setResultEntity(objViewData,ResponseCode.RETRIEVE_SUCCEED);
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

//    @RequestMapping(value = "ObjTicketShare2",method = RequestMethod.GET)
    @RequestMapping(value = "ticketShare",method = RequestMethod.GET)
    ResponseEntity<RestBody<ObjViewData<TicketType>>> getObject2(Date time) {
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            //获取起止时间
            Date startTime = null;
            Date endTime = null;
            if (time != null) {
                SimpleDateFormat format1 = new SimpleDateFormat("yyyy-MM-dd 00:00:00");
                String strStart = format1.format(time);
                SimpleDateFormat format2 = new SimpleDateFormat("yyyy-MM-dd 23:59:59");
                String strEnd = format2.format(time);
                logger.debug("ticketShare--起始时间为：{}，结束时间为：{}", strStart, strEnd);
                SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                try {
                    startTime = format.parse(strStart);
                    endTime = format.parse(strEnd);
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            }
            //
            ObjViewData<TicketType> objViewData = new ObjViewData<TicketType>();
            Map<Integer, TicketType> objMap = new HashMap<Integer, TicketType>();
            //获取票卡列表
//            List<TicketType> ticketList = ticketTypeRepository.findAll();
//            logger.debug("ticketList--票卡数量为:{}", ticketList.size());
                //获取票卡分时客流前10的ticketId集合
                List<Integer> ticketIds = new ArrayList<Integer>();
                List<Object[]> objectList = ticketShareFlowRepository.getTopData(60, startTime, endTime);
                logger.debug("objectList--票卡数量为:{}", objectList.size());
                for (int i = 0; i < 10; i++) {
                    Object[] objects = objectList.get(i);
                    Integer id = (Integer) objects[0];
                    ticketIds.add(id);
                }
                logger.debug("票卡前10的id集合：{}", ticketIds);
                //只处理前10的票卡数据
            List<TicketType> ticketList = ticketTypeRepository.findByTicketIdIn(ticketIds);
            logger.debug("ticketList--票卡数量为:{}", ticketList.size());
            //方法1
            List<Object[]> objDatas2 = ticketShareFlowRepository.getObjectData2(ticketIds,startTime, endTime);
            logger.debug("获取总的票卡分时记录，数量为:{}", objDatas2.size());
            for (TicketType t1:ticketList) {
                Integer dataId = t1.getTicketId();
                List<List<Long>> dataList1 = new ArrayList<List<Long>>();
                List<List<Long>> dataList2 = new ArrayList<List<Long>>();
                List<List<Long>> dataList3 = new ArrayList<List<Long>>();
                for (Object[] objects : objDatas2) {
                    Integer id = (Integer) objects[2];//ticketId
                    if(id == dataId){
                        Date dateLong = (Date) objects[0];
                        BigInteger count = (BigInteger) objects[1];
                        List<Long> pt = new ArrayList<Long>();
                        pt.add(dateLong.getTime());
                        pt.add(count.longValue());
                        Integer sectionFlag = (Integer)objects[3];
                        if(sectionFlag == 10){
                            dataList1.add(pt);
                        }else if(sectionFlag == 30){
                            dataList2.add(pt);
                        }else if(sectionFlag == 60){
                            dataList3.add(pt);
                        }
                    }
                }
                t1.setDataList1(dataList1);
                t1.setDataList2(dataList2);
                t1.setDataList3(dataList3);
                objMap.put(dataId,t1);
            }

            //方法2：
//            for (int i = 0; i < 3; i++) {
//                    Integer section = 0;
//                    if (i == 0) {
//                        section = 10;
//                    } else if (i == 1) {
//                        section = 30;
//                    } else if (i == 2) {
//                        section = 60;
//                    }
//                    List<Object[]> objDatas = ticketShareFlowRepository.getObjectData(ticketIds, section, startTime, endTime);
//                    logger.debug("获取票卡分时记录，数量为:{}", objDatas.size());
//                    for (Object[] objects : objDatas) {
//                        Integer id = (Integer) objects[2];//ticketId
//                        //获取对应基础Obj
//                        TicketType theTicket = new TicketType();
//                        List<List<Long>> dataList1 = new ArrayList<List<Long>>();
//                        List<List<Long>> dataList2 = new ArrayList<List<Long>>();
//                        List<List<Long>> dataList3 = new ArrayList<List<Long>>();
//                        theTicket.setDataList1(dataList1);
//                        theTicket.setDataList2(dataList2);
//                        theTicket.setDataList3(dataList3);
//                        //匹配到ticketId相同的TicketType，并获取TicketType
//                        for(TicketType tt:ticketList){
//                            Integer dataId = tt.getTicketId();
////                            logger.debug("数据id为{},Object的id为{}",id,dataId);
//                            if(dataId == id){
//                                logger.debug("dataId==id");
//                                theTicket = tt;
//                                List<List<Long>> dataList11 = new ArrayList<List<Long>>();
//                                List<List<Long>> dataList22 = new ArrayList<List<Long>>();
//                                List<List<Long>> dataList33 = new ArrayList<List<Long>>();
//                                theTicket.setDataList1(dataList1);
//                                theTicket.setDataList2(dataList2);
//                                theTicket.setDataList3(dataList3);
//                                break;
//                            }
//                        }
//                        Date dateLong = (Date) objects[0];
//                        BigInteger count = (BigInteger) objects[1];
//                            List<Long> pt = new ArrayList<Long>();
//                            pt.add(dateLong.getTime());
//                            pt.add(count.longValue());
//
//                            if (i == 0) {
//                                logger.debug("i==0");
//                                List<List<Long>> temp = null;
//                                temp = theTicket.getDataList1();
//                                temp.add(pt);
////                                dataList1.add(pt);
//                            } else if (i == 1) {
//                                logger.debug("i==1");
//                                List<List<Long>> temp = null;
//                                temp = theTicket.getDataList2();
//                                temp.add(pt);
////                                dataList2.add(pt);
//                            } else if (i == 2) {
//                                logger.debug("i==2");
//                                List<List<Long>> temp = null;
//                                temp = theTicket.getDataList3();
//                                temp.add(pt);
////                                dataList3.add(pt);
//                        }
//                        theTicket.setTicketId(id);
////                        theTicket.setDataList1(dataList1);
////                            theTicket.setDataList2(dataList2);
////                            theTicket.setDataList3(dataList3);
//
//                            objMap.put(id,theTicket);
//                    }
//                }
            objViewData.setObjMap(objMap);
            builder.setResultEntity(objViewData, ResponseCode.RETRIEVE_SUCCEED);
        } catch (Exception e) {
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    //模型可能有问题，需要优化。暂时先实现
    @RequestMapping(value = "TicketShare",method = RequestMethod.GET)
    ResponseEntity<ObjViewData<TicketType>> getObject3(@RequestParam("ids")List<Integer> ids,Date startTime,Date endTime,Integer familyId,@RequestParam("direct")Integer direct){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            logger.debug("TicketShare...");
            ObjViewData<TicketType> obj = new ObjViewData<TicketType>();
            Map<Integer,TicketType> map = new HashMap<Integer,TicketType>();
            List<TicketType> typeList = ticketTypeRepository.findByTicketIdIn(ids);
            for(TicketType ele:typeList){
                Integer ticketId = ele.getTicketId();
                List<List<Long>> dataList1 = new ArrayList<List<Long>>();
                List<List<Long>> dataList2 = new ArrayList<List<Long>>();
                List<List<Long>> dataList3 = new ArrayList<List<Long>>();
                ele.setDataList1(dataList1);
                ele.setDataList2(dataList2);
                ele.setDataList3(dataList3);

                map.put(ticketId,ele);
            }
            //方式一：综合一次查询
            //SQL执行(2.33s)
            List<TicketShareFlow> list = ticketShareFlowRepository.getDataByTicketFamily(ids, startTime, endTime,direct);
            logger.debug("获取的票卡类型{}的票卡分时客流总记录数量为：{}", familyId, list.size());
            for(TicketShareFlow tsf:list){
                Integer ticketId = tsf.getTicketId();
                Integer section = tsf.getSection();

                Date dateLong = tsf.getTimestamp();
                //时间平移4小时
                Calendar cal = Calendar.getInstance();
                cal.setTime(dateLong);
                cal.add(Calendar.HOUR,4);
                Date dest_date = cal.getTime();

                Integer count = tsf.getFlowCount();
                if(map.containsKey(ticketId)){
                    TicketType ele = map.get(ticketId);
                    if(section == 10){
                        List<List<Long>> dataList = ele.getDataList1();

                        List<Long> pt = new ArrayList<Long>();
                        pt.add(dest_date.getTime());
                        pt.add(count.longValue());

                        dataList.add(pt);
//                        ele.setDataList1(dataList);//java的类内对象属性，都为引用，不需要再次set赋值
                    }else if(section == 30){
                        List<List<Long>> dataList = ele.getDataList2();

                        List<Long> pt = new ArrayList<Long>();
                        pt.add(dest_date.getTime());
                        pt.add(count.longValue());

                        dataList.add(pt);
//                        ele.setDataList2(dataList);
                    }else if(section == 60){
                        List<List<Long>> dataList = ele.getDataList3();

                        List<Long> pt = new ArrayList<Long>();
                        pt.add(dest_date.getTime());
                        pt.add(count.longValue());

                        dataList.add(pt);
//                        ele.setDataList3(dataList);
                    }
                }else {
                    logger.error("未在map中找到key={}的元素",ticketId);
                }
            }
            obj.setObjMap(map);
            builder.setResultEntity(obj,ResponseCode.RETRIEVE_SUCCEED);
            //方式二：分片段，多次查询
            //SQL执行(10--1.6s,30--0.4s,60--0.27s),总共为2.27s
//            for(int i = 0;i<3;i++) {
//                if(i == 0){
//                    Integer section = 10;
//                    List<TicketShareFlow> list = ticketShareFlowRepository.getDataByTicketFamily(ids, startTime, endTime,section,direct);
//                    logger.debug("获取的票卡类型{}的票卡{}分钟分时客流总记录数量为：{}", familyId,section, list.size());
//                    //拆分
//                    for(TicketShareFlow tsf:list){
//                        Integer ticketId = tsf.getTicketId();
//                        if(map.containsKey(ticketId)){
//                            TicketType ele = map.get(ticketId);
//
//                        }else {
//                            logger.error("未在map中找到key={}的元素",ticketId);
//                        }
//                    }
//                }
//                else if(i == 1){
//                    Integer section = 30;
//                    List<TicketShareFlow> list = ticketShareFlowRepository.getDataByTicketFamily(ids, startTime, endTime,section,direct);
//                    logger.debug("获取的票卡类型{}的票卡{}分钟分时客流总记录数量为：{}", familyId,section, list.size());
//
//
//                }else if(i == 2){
//                    Integer section = 60;
//                    List<TicketShareFlow> list = ticketShareFlowRepository.getDataByTicketFamily(ids, startTime, endTime,section,direct);
//                    logger.debug("获取的票卡类型{}的票卡{}分钟分时客流总记录数量为：{}", familyId,section, list.size());
//
//
//                }
//            }
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
