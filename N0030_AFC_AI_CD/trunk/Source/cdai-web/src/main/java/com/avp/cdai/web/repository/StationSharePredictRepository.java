package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.StationShareFlowPredict;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/18.
 */
public interface StationSharePredictRepository extends JpaRepository<StationShareFlowPredict,Integer>{
    @Query(value = "select sum(passenger_flow),flow_timestamp from station_time_sharing_predict_flow where station_id=:stationId and section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime group by station_id,flow_timestamp order by flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData(@Param("stationId")Integer stationId, @Param("section")Integer section, @Param("startTime")Date start,@Param("endTime")Date end);

    @Query(value = "select passenger_flow,flow_timestamp from station_time_sharing_predict_flow where station_id=:stationId and section=:section and flow_timestamp>=:startTime and flow_timestamp<=:endTime and direction=:direction order by flow_timestamp asc",nativeQuery = true)
    List<Object[]> getData(@Param("stationId")Integer stationId, @Param("section")Integer section, @Param("startTime")Date start,@Param("endTime")Date end,@Param("direction")Integer direct);
}
