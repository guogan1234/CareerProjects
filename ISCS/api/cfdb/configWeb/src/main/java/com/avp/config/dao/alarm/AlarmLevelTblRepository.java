package com.avp.config.dao.alarm;

import com.avp.config.entity.alarm.AlarmLevelTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface AlarmLevelTblRepository extends JpaRepository<AlarmLevelTbl, Integer>, JpaSpecificationExecutor<AlarmLevelTbl> {

    AlarmLevelTbl save(AlarmLevelTbl alarmLevelTbl);

    public AlarmLevelTbl findByLevelId(Integer levelId);

    public void deleteByLevelId(Integer levelId);

    public List<AlarmLevelTbl> findByLevelName(String levelName);

    public List<AlarmLevelTbl> findByLevelIdOrLevelName(Integer levelId, String levelName);
}
