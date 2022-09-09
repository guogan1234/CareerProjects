package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.ObjStation;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.util.List;

/**
 * Created by guo on 2017/8/8.
 */
public interface ObjStationRepository extends CrudRepository<ObjStation,Integer>{
    List<ObjStation> findAll();
    ObjStation findBystationIdAndLineId(@Param("id") Integer id,@Param("lineId") Integer lineId);
    ObjStation findBystationName(@Param("name") String name);
    List<ObjStation> findBystationIdIn(@Param("ids") List<Integer> stationIds);
    List<ObjStation> findByLineId(@Param("lineId")Integer lineId);

    List<ObjStation> findByStationId(@Param("stationId") Integer stationId);//可能有相同Id的车站
}
