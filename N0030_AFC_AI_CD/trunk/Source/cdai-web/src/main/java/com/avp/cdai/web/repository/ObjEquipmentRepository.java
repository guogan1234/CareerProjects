package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.ObjEquipment;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.util.List;

/**
 * Created by guo on 2017/9/4.
 */
public interface ObjEquipmentRepository extends CrudRepository<ObjEquipment,Integer> {
    List<ObjEquipment> findAll();
    List<ObjEquipment> findByStationId(@Param("stationId")Integer stationId);

    @Query(value = "select distinct equipment_type_id from obj_equipment where station_id=:stationId",nativeQuery = true)
    List<Integer> findDistinctTypeByStationId(@Param("stationId")Integer stationId);

//    @Query(value = "from obj_equipment where station_id=:stationId and equipment_type_id=:typeId",nativeQuery = true)
    List<ObjEquipment> findByStationIdAndTypeId(@Param("stationId")Integer stationId,@Param("typeId")Integer typeId);

    ObjEquipment findByEquipmentId(@Param("equipmentId")Integer equimentId);
}
