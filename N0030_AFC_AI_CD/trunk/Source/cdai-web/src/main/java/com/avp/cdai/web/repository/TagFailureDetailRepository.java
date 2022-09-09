package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.TagFailureDetail;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/14.
 */
public interface TagFailureDetailRepository extends JpaRepository<TagFailureDetail,Integer>{
    //故障查询
//    @Query(value = "select sum(failure_num) as sumCount,tag_name from device_failure_analysis_detail where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc",nativeQuery = true)
//    Page<TagFailureDetail> getLineData(@Param("lineId")Integer lineId, @Param("startTime")String start, @Param("endTime")String end, Pageable pageable);

    @Query(value = "select sum(failure_num) as sumCount,tag_name from device_failure_analysis_detail where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getLineData2(@Param("lineId")Integer lineId, @Param("startTime")Date start, @Param("endTime")Date end);
    //    List<TagFailureDetail> getLineData2(@Param("lineId")Integer lineId, @Param("startTime")String start, @Param("endTime")String end);//Error，时间不接受String

    @Query(value = "select sum(failure_num) as sumCount,tag_name from device_failure_analysis_detail where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getStationData2(@Param("stationId")Integer stationId, @Param("startTime")Date start, @Param("endTime")Date end);

    @Query(value = "select sum(failure_num) as sumCount,device_id from device_failure_analysis_detail where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by device_id order by sumCount desc",nativeQuery = true)
    List<Object[]> getDeviceFailed(@Param("lineId")Integer lineId,@Param("startTime")Date start,@Param("endTime")Date end);

    @Query(value = "select sum(failure_num) as sumCount,device_id from device_failure_analysis_detail where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by device_id order by sumCount desc",nativeQuery = true)
    List<Object[]> getDeviceFailed2(@Param("stationId")Integer stationId,@Param("startTime")Date start,@Param("endTime")Date end);

    @Query(value = "select sum(failure_num) as sumCount,device_id from device_failure_analysis_detail where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime and tag_name=:tagName group by device_id order by sumCount desc",nativeQuery = true)
    List<Object[]> getTagFailedDetail(@Param("lineId")Integer lineId,@Param("startTime")Date start,@Param("endTime")Date end,@Param("tagName")String tagName);

    @Query(value = "select sum(failure_num) as sumCount,device_id from device_failure_analysis_detail where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime and tag_name=:tagName group by device_id order by sumCount desc",nativeQuery = true)
    List<Object[]> getTagFailedDetail2(@Param("stationId")Integer stationId,@Param("startTime")Date start,@Param("endTime")Date end,@Param("tagName")String tagName);

    //上面接口根据结果再遍历查询id较慢，故改为级联查询
    @Query(value = "select sumCount,a.tag_name as tagName,b.id as tagId,tag_desc from (select sum(failure_num) as sumCount,tag_name from device_failure_analysis_detail where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name) a left join ref_module_subtag_define b on a.tag_name=b.tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getStationData2_cascade(@Param("stationId")Integer stationId,@Param("startTime")Date start, @Param("endTime")Date end);

    @Query(value = "select sumCount,a.tag_name as tagName,b.id as tagId,tag_desc from (select sum(failure_num) as sumCount,tag_name from device_failure_analysis_detail where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name) a left join ref_module_subtag_define b on a.tag_name=b.tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getLineData2_cascade(@Param("lineId")Integer lineId,@Param("startTime")Date start, @Param("endTime")Date end);
}
