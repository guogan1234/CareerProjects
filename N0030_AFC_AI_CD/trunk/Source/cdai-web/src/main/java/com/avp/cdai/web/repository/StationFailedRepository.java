package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.StationFailed;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.math.BigInteger;
import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/26.
 */
public interface StationFailedRepository extends CrudRepository<StationFailed,Integer>{
    @Query(value = "select sum(failure_num) as failedNum from station_failure_analysis where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime",nativeQuery = true)
    BigInteger getData(@Param("stationId")Integer stationId, @Param("startTime")Date start,@Param("endTime")Date end);

    @Query(value = "select sum(failure_num) as failedNum,station_id from station_failure_analysis where analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by station_id order by failedNum desc",nativeQuery = true)
    List<Object[]> getAllData(@Param("startTime")Date start,@Param("endTime")Date end);
}
