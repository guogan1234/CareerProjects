package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.EquipmentSubType;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

/**
 * Created by guo on 2017/9/4.
 */
public interface EquipmentSubTypeRepository extends CrudRepository<EquipmentSubType,Integer>{
    List<EquipmentSubType> findAll();
}
