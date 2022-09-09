package com.avp.config.dao.alarm;

import com.avp.config.entity.alarm.AlarmTypeTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface AlarmTypeTblRepository extends JpaRepository<AlarmTypeTbl, Integer>, JpaSpecificationExecutor<AlarmTypeTbl> {
    AlarmTypeTbl save(AlarmTypeTbl alarmTypeTbl);

    public AlarmTypeTbl findByTypeId(Integer typeId);

    public void deleteByTypeId(Integer typeId);

    public List<AlarmTypeTbl> findByTypeName(String typeName);

    public List<AlarmTypeTbl> findByTypeIdOrTypeName(Integer typeId, String typeName);
}
