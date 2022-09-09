package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.LineShareFlowPredict;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/18.
 */
public interface LineSharePredictRepository extends JpaRepository<LineShareFlowPredict,Integer>{
    //全部查询
    @Query(value = "select sum(passenger_flow),line_id,flow_timestamp from line_time_sharing_predict_flow where line_id in(:lineIds) and flow_timestamp>=:startTime and flow_timestamp<=:endTime and section=:section group by line_id,flow_timestamp order by flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData(@Param("lineIds")List<Integer> lineIds, @Param("startTime")Date start,@Param("endTime")Date end,@Param("section")Integer section);

    //单条线路查询
    @Query(value = "select sum(passenger_flow),flow_timestamp from line_time_sharing_predict_flow where line_id=:lineId and flow_timestamp>=:startTime and flow_timestamp<=:endTime and section=:section group by line_id,flow_timestamp order by flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData2(@Param("lineId")Integer lineId,@Param("startTime")Date start,@Param("endTime")Date end,@Param("section")Integer section);

    //单条线路查询
    @Query(value = "select passenger_flow,flow_timestamp from line_time_sharing_predict_flow where line_id=:lineId and flow_timestamp>=:startTime and flow_timestamp<=:endTime and section=:section and direction=:direction order by flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData2(@Param("lineId")Integer lineId,@Param("startTime")Date start,@Param("endTime")Date end,@Param("section")Integer section,@Param("direction")Integer direct);
}
