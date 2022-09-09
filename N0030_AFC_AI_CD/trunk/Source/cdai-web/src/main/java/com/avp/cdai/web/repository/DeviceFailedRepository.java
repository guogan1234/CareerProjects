package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.TagFailureDetail;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.Repository;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/12/12.
 */
public interface DeviceFailedRepository extends JpaRepository<TagFailureDetail,Integer> {
    @Query(value = "select sum(failure_num) as sumCount,device_id from device_failure_analysis_detail where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime and device_type=:deviceType group by device_id order by sumCount desc",nativeQuery = true)
    List<Object[]> getDeviceFailed(@Param("lineId")Integer lineId, @Param("startTime")Date start, @Param("endTime")Date end,@Param("deviceType")Integer deviceType);

    @Query(value = "select sum(failure_num) as sumCount,device_id from device_failure_analysis_detail where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime and device_type=:deviceType group by device_id order by sumCount desc",nativeQuery = true)
    List<Object[]> getDeviceFailed2(@Param("stationId")Integer stationId,@Param("startTime")Date start,@Param("endTime")Date end,@Param("deviceType")Integer deviceType);

    @Query(value = "select sum(failure_num) as sumCount,tag_name from device_failure_analysis_detail where device_id=:deviceId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getDeviceFailedDetail(@Param("deviceId")Integer deviceId,@Param("startTime")Date startTime,@Param("endTime")Date endTime);
}
