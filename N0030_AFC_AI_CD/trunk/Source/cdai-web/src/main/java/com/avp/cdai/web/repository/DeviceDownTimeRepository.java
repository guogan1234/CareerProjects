package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.DeviceDownTime;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/23.
 */
public interface DeviceDownTimeRepository extends CrudRepository<DeviceDownTime,Integer>{
    @Query(value = "select * from device_downtime_percent_analysis where line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end order by failure_time_in_minutes desc",nativeQuery = true)
    List<DeviceDownTime> getCountListByLine(@Param("lineId")Integer lineId, @Param("start")Date start,@Param("end")Date end);

    @Query(value = "select * from device_downtime_percent_analysis where station_id=:stationId and analysis_timestamp>=:start and analysis_timestamp<=:end order by failure_time_in_minutes desc",nativeQuery = true)
    List<DeviceDownTime> getCountListByStation(@Param("stationId")Integer stationId, @Param("start")Date start,@Param("end")Date end);

    @Query(value = "select * from device_downtime_percent_analysis where line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end order by failure_rate desc",nativeQuery = true)
    List<DeviceDownTime> getPercentListByLine(@Param("lineId")Integer lineId, @Param("start")Date start,@Param("end")Date end);

    @Query(value = "select * from device_downtime_percent_analysis where station_id=:stationId and analysis_timestamp>=:start and analysis_timestamp<=:end order by failure_rate desc",nativeQuery = true)
    List<DeviceDownTime> getPercentListByStation(@Param("stationId")Integer stationId, @Param("start")Date start,@Param("end")Date end);
}
