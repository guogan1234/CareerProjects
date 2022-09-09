package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.ComponentConsume;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/31.
 */
public interface ComponentConsumeRepository extends CrudRepository<ComponentConsume,Integer>{
    @Query(value = "select component_type,sum(consumption) as sumCount from component_consume_analysis where analysis_timestamp>=:start and analysis_timestamp<=:end group by component_type order by sumCount",nativeQuery = true)
    List<Object[]> getTopData(@Param("start")Date start,@Param("end")Date end);

    @Query(value = "select component_type,component_name,consumption,analysis_timestamp from component_consume_analysis where analysis_timestamp>=:start and analysis_timestamp<=:end and component_type=:id",nativeQuery = true)
    List<Object[]> getData(@Param("id")Integer id,@Param("start")Date start,@Param("end")Date end);
}
