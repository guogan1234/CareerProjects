package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.FailureReason;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/25.
 */
public interface FailureReasonRepository extends CrudRepository<FailureReason,Integer>{
    //根据条件获取改模块的平均运行时长(本月正常运行总时长/本月总故障次数)，且按月统计
    //Section
    //默认查询
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getDefaultListBySection(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId, @Param("start")Date start,@Param("end")Date end);

    //根据线路查询模块(线路+设备类型+模块类型)
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getLineListBySection(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId,@Param("lineId")Integer lineId,@Param("start")Date start,@Param("end")Date end);
    //根据线路查询模块(线路+设备类型+模块类型)
    //可以共用一个接口，根据设备类型list查询，但调试不方便(暂弃用)
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type in :deviceTypeList and module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getLineListBySection(@Param("deviceTypeList")List<Integer> deviceTypeList, @Param("moduleId")Integer moduleId,@Param("lineId")Integer lineId,@Param("start")Date start,@Param("end")Date end);
    //根据线路查询模块(线路+模块类型)
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end group by analysis_timestamp,module_id order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getLineListBySection(@Param("moduleId")Integer moduleId,@Param("lineId")Integer lineId,@Param("start")Date start,@Param("end")Date end);

    //根据车站查询模块(线路+车站+设备类型+模块类型)
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getStationListBySection(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId,@Param("stationId")Integer stationId,@Param("start")Date start,@Param("end")Date end);
    //根据车站查询模块(线路+车站+模块类型)
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:start and analysis_timestamp<=:end group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getStationListBySection(@Param("moduleId")Integer moduleId,@Param("stationId")Integer stationId,@Param("start")Date start,@Param("end")Date end);

    //根据设备查询模块(线路+车站+设备+设备类型+模块类型)
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and device_id=:deviceId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getDeviceListBySection(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId,@Param("deviceId")Integer deviceId,@Param("start")Date start,@Param("end")Date end);
    //根据设备查询模块(线路+车站+设备+模块类型)
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and device_id=:deviceId and analysis_timestamp>=:start and analysis_timestamp<=:end group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getDeviceListBySection(@Param("moduleId")Integer moduleId,@Param("deviceId")Integer deviceId,@Param("start")Date start,@Param("end")Date end);

    //根据模块类型分组，查询全线路的改模块的平均运行时长(本月正常运行总时长/本月总故障次数)
    //全线路+模块类型
    @Query(value = "select analysis_timestamp,sum(normal_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getAllLinesBySection(@Param("moduleId")Integer moduleId,@Param("startTime")Date startTime,@Param("endTime")Date endTime);

    //根据条件获取该模块的平均故障次数(本月总的交易次数/本月故障总数)
    //Count
    //默认查询
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getDefaultListByCount(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId, @Param("start")Date start,@Param("end")Date end);

    //根据线路查询模块
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getLineListByCount(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId,@Param("lineId")Integer lineId,@Param("start")Date start,@Param("end")Date end);

    //根据车站查询模块
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getStationListByCount(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId,@Param("stationId")Integer stationId,@Param("start")Date start,@Param("end")Date end);

    //根据设备查询模块
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,device_type,module_id from failure_reason_analysis where device_type=:deviceType and module_id=:moduleId and device_id=:deviceId and analysis_timestamp>=:start and analysis_timestamp<=:end group by device_type,module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getDeviceListByCount(@Param("deviceType")Integer deviceType, @Param("moduleId")Integer moduleId,@Param("deviceId")Integer deviceId,@Param("start")Date start,@Param("end")Date end);

    //分割线-------------
    //根据线路查询模块(线路+模块类型)
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end group by analysis_timestamp,module_id order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getLineListByCount(@Param("moduleId")Integer moduleId,@Param("lineId")Integer lineId,@Param("start")Date start,@Param("end")Date end);

    //根据车站查询模块(线路+车站+模块类型)
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:start and analysis_timestamp<=:end group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getStationListByCount(@Param("moduleId")Integer moduleId,@Param("stationId")Integer stationId,@Param("start")Date start,@Param("end")Date end);

    //根据设备查询模块(线路+车站+设备+模块类型)
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and device_id=:deviceId and analysis_timestamp>=:start and analysis_timestamp<=:end group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getDeviceListByCount(@Param("moduleId")Integer moduleId,@Param("deviceId")Integer deviceId,@Param("start")Date start,@Param("end")Date end);

    //全线路+模块类型
    @Query(value = "select analysis_timestamp,sum(trans_count) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getAllLinesByCount(@Param("moduleId")Integer moduleId,@Param("startTime")Date startTime,@Param("endTime")Date endTime);

    //repair
    //平均修复时长(本月总的故障时间/本月总的故障次数)
    //根据线路查询模块(线路+模块类型)
    @Query(value = "select analysis_timestamp,sum(failure_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:start and analysis_timestamp<=:end group by analysis_timestamp,module_id order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getLineListByRepair(@Param("moduleId")Integer moduleId,@Param("lineId")Integer lineId,@Param("start")Date start,@Param("end")Date end);

    //根据车站查询模块(线路+车站+模块类型)
    @Query(value = "select analysis_timestamp,sum(failure_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:start and analysis_timestamp<=:end group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getStationListByRepair(@Param("moduleId")Integer moduleId,@Param("stationId")Integer stationId,@Param("start")Date start,@Param("end")Date end);

    //根据设备查询模块(线路+车站+设备+模块类型)
    @Query(value = "select analysis_timestamp,sum(failure_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and device_id=:deviceId and analysis_timestamp>=:start and analysis_timestamp<=:end group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getDeviceListByRepair(@Param("moduleId")Integer moduleId,@Param("deviceId")Integer deviceId,@Param("start")Date start,@Param("end")Date end);

    //全线路+模块类型
    @Query(value = "select analysis_timestamp,sum(failure_time_in_minutes) as sumCount,sum(failure_num) as sumTransCount,module_id from failure_reason_analysis where module_id=:moduleId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by module_id,analysis_timestamp order by analysis_timestamp",nativeQuery = true)
    List<Object[]> getAllLinesByRepair(@Param("moduleId")Integer moduleId,@Param("startTime")Date startTime,@Param("endTime")Date endTime);
}
