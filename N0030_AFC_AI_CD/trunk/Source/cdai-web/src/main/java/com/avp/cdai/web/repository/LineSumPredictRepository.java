package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.LineCumulativePredict;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/18.
 */
public interface LineSumPredictRepository extends JpaRepository<LineCumulativePredict,Integer>{
    @Query(value = "select sum(passenger_flow),flow_timestamp from line_cumulative_predict_flow where line_id=:lineId and section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime group by flow_timestamp,line_id order by flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData(@Param("lineId")Integer lineId, @Param("section")Integer section, @Param("startTime")Date start,@Param("endTime")Date end);

    @Query(value = "select passenger_flow,flow_timestamp from line_cumulative_predict_flow where line_id=:lineId and section=:section and flow_timestamp>=:startTime and flow_timestamp<:endTime and direction=:direction order by flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData(@Param("lineId")Integer lineId, @Param("section")Integer section, @Param("startTime")Date start,@Param("endTime")Date end,@Param("direction")Integer direct);
}
