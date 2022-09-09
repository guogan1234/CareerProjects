package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.ModuleFailure;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;
import org.springframework.web.bind.annotation.PathVariable;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/15.
 */
public interface ModuleFailureRepository extends CrudRepository<ModuleFailure,Integer> {
//    @Query(value = "select sum(failure_num) as sumCount,module_id from module_failure_analysis where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by module_id order by sumCount desc",nativeQuery = true)
//    List<Object[]> getData(@Param("lineId")Integer lineId, @Param("startTime")Date start,@Param("endTime")Date end);
//
//    @Query(value = "select sum(failure_num) as sumCount,module_id from module_failure_analysis where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by module_id order by sumCount desc",nativeQuery = true)
//    List<Object[]> getStationData(@Param("stationId")Integer stationId, @Param("startTime")Date start,@Param("endTime")Date end);
//
//    @Query(value = "select sum(failure_num) as sumCount,device_id from module_failure_analysis where line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime and module_id=:moduleId group by device_id order by sumCount desc",nativeQuery = true)
//    List<Object[]> getModuleDetailData(@Param("lineId")Integer lineId,@Param("moduleId")Integer moduleId,@Param("startTime")Date start,@Param("endTime")Date end);
//
//    @Query(value = "select sum(failure_num) as sumCount,device_id from module_failure_analysis where station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime and module_id=:moduleId group by device_id order by sumCount desc",nativeQuery = true)
//    List<Object[]> getModuleDetailData2(@Param("stationId")Integer stationId,@Param("moduleId")Integer moduleId,@Param("startTime")Date start,@Param("endTime")Date end);

    @Query(value = "select sum(failure_num) as sumCount,tag_name from module_failure_analysis_bystation where module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getModuleFailed(@Param("moduleId")Integer moduleId, @Param("lineId")Integer lineId, @Param("startTime")Date start,@Param("endTime")Date end);

    @Query(value = "select sum(failure_num) as sumCount,tag_name from module_failure_analysis_bystation where module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getModuleFailed2(@Param("moduleId")Integer moduleId, @Param("stationId")Integer stationId, @Param("startTime")Date start,@Param("endTime")Date end);

    //根据tagName遍历查询tagId较慢，修改为级联查询
    @Query(value = "select sumCount,a.tag_name as tagName,b.id as tagId,tag_desc from (select sum(failure_num) as sumCount,tag_name from module_failure_analysis_bystation where module_id=:moduleId and line_id=:lineId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc) a left join ref_module_subtag_define b on a.tag_name=b.tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getModuleFailed_cascade(@Param("moduleId")Integer moduleId, @Param("lineId")Integer lineId,@Param("startTime")Date start, @Param("endTime")Date end);

    @Query(value = "select sumCount,a.tag_name as tagName,b.id as tagId,tag_desc from (select sum(failure_num) as sumCount,tag_name from module_failure_analysis_bystation where module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc) a left join ref_module_subtag_define b on a.tag_name=b.tag_name order by sumCount desc",nativeQuery = true)
    List<Object[]> getModuleFailed2_cascade(@Param("moduleId")Integer moduleId, @Param("stationId")Integer stationId,@Param("startTime")Date start, @Param("endTime")Date end);

    //数据表中没有stationId，无法获取车站数据
//    @Query(value = "select sum(failure_num) as sumCount,tag_name from module_failure_analysis where module_id=:moduleId and station_id=:stationId and analysis_timestamp>=:startTime and analysis_timestamp<=:endTime group by tag_name order by sumCount desc",nativeQuery = true)
//    List<Object[]> getModuleFailed2(@Param("moduleId")Integer moduleId, @Param("stationId")Integer stationId, @Param("startTime")Date start,@Param("endTime")Date end);
}
