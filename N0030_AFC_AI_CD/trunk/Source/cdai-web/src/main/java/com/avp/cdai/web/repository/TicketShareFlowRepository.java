package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.TicketShareFlow;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/8/16.
 */
public interface TicketShareFlowRepository extends JpaRepository<TicketShareFlow,Integer>,JpaSpecificationExecutor<TicketShareFlow>{
    //先排重后分组
    @Query(value = "select t.flow_timestamp,sum(t.passenger_flow),sum(t.ticket_id)/count(*) from (select distinct flow_timestamp,ticket_id,direction,passenger_flow from ticket_share_passenger_flow where section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime order by flow_timestamp asc) t group by t.flow_timestamp",nativeQuery = true)
//    @Query("select new TicketShareFlow(t.timestamp,sum(t.flowCount)) from (select distinct timestamp,ticketId,direction,flowCount from TicketShareFlow where section=:section and ticketId=:ticketId order by timestamp asc) t group by t.timestamp")
//    Object[] getData(@Param("section") Integer section,@Param("ticketId") Integer ticketId);//OK
//    Object getData1(@Param("section") Integer section,@Param("ticketId") Integer ticketId);//Error，返回结果大于一个元素
    List<Object[]> getData1(@Param("section") Integer section, @Param("startTime") Date startTime, @Param("endTime")Date endTime);

    //测试简单查询
    @Query("select distinct timestamp,ticketId,direction,flowCount from TicketShareFlow where section=:section and ticketId=:ticketId order by timestamp asc")
    List<TicketShareFlow> getData2(@Param("section") Integer section, @Param("ticketId") Integer ticketId);//Hibernate会将Object[]转为Class对象

    //先排重再根据(时间和ticketId)分组，查询全部ticketId的
    @Query(value = "select t.flow_timestamp,sum(t.passenger_flow),t.ticket_id from (select distinct flow_timestamp,ticket_id,direction,passenger_flow from ticket_share_passenger_flow where section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime) t group by t.flow_timestamp,t.ticket_id order by t.flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData3(@Param("section") Integer section,@Param("startTime") Date startTime,@Param("endTime") Date endTime);

    //先排重再根据(时间和ticketId)分组，查询指定ticketId的
    //执行一次SQL(965ms)
    @Query(value = "select t.flow_timestamp,sum(t.passenger_flow),t.ticket_id from (select distinct flow_timestamp,ticket_id,direction,passenger_flow from ticket_share_passenger_flow where ticket_id=:ticketId and section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime) t group by t.flow_timestamp,t.ticket_id order by t.flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData4(@Param("ticketId")Integer ticketId,@Param("section")Integer section,@Param("startTime")Date startTime,@Param("endTime")Date endTime);

    //先排重再根据(时间和ticketId)分组，查询全部ticketId的
    @Query(value = "select t.flow_timestamp,sum(t.passenger_flow),t.ticket_id from (select distinct flow_timestamp,ticket_id,direction,passenger_flow from ticket_share_passenger_flow where section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime) t group by t.flow_timestamp,t.ticket_id order by t.flow_timestamp asc",nativeQuery =true)
    List<Object[]> getAllData(@Param("section")Integer section,@Param("startTime")Date startTime,@Param("endTime")Date endTime);

    //先排重再根据(时间和ticketId)分组，查询指定ticketId集合的
    @Query(value = "select t.flow_timestamp,sum(t.passenger_flow),t.ticket_id from (select distinct flow_timestamp,ticket_id,direction,passenger_flow from ticket_share_passenger_flow where ticket_id in (:ids) and section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime) t group by t.flow_timestamp,t.ticket_id order by t.flow_timestamp asc",nativeQuery = true)
    List<Object[]>  getObjectData(@Param("ids")List<Integer> ids,@Param("section")Integer section,@Param("startTime")Date startTime,@Param("endTime")Date endTime);

    //获取当天票卡客流使用量前10的sql(section=60执行最快)
    @Query(value = "select ticket_id,sum(passenger_flow) as flowSum from ticket_share_passenger_flow where section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime group by ticket_id order by flowSum desc",nativeQuery = true)
    List<Object[]> getTopData(@Param("section")Integer section,@Param("startTime")Date startTime,@Param("endTime")Date endTime);

    //查出全部数据，再拆分
    @Query(value = "select t.flow_timestamp,sum(t.passenger_flow),t.ticket_id,t.section from (select distinct flow_timestamp,ticket_id,direction,passenger_flow,section from ticket_share_passenger_flow where ticket_id in (:ids) and flow_timestamp>=:startTime and flow_timestamp<=:endTime) t group by t.flow_timestamp,t.ticket_id,t.section order by t.flow_timestamp asc",nativeQuery = true)
    List<Object[]>  getObjectData2(@Param("ids")List<Integer> ids,@Param("startTime")Date startTime,@Param("endTime")Date endTime);

    @Query(value = "select * from ticket_share_passenger_flow where ticket_id in(:ids) and flow_timestamp>=:startTime and flow_timestamp<=:endTime and section=:section and direction=:direction order by flow_timestamp",nativeQuery = true)
    List<TicketShareFlow> getDataByTicketFamily(@Param("ids")List<Integer> ids,@Param("startTime")Date startTime,@Param("endTime")Date endTime,@Param("section")Integer section,@Param("direction")Integer direct);

    @Query(value = "select * from ticket_share_passenger_flow where ticket_id in(:ids) and flow_timestamp>=:startTime and flow_timestamp<=:endTime and direction=:direction order by flow_timestamp",nativeQuery = true)
    List<TicketShareFlow> getDataByTicketFamily(@Param("ids")List<Integer> ids,@Param("startTime")Date startTime,@Param("endTime")Date endTime,@Param("direction")Integer direct);
}

