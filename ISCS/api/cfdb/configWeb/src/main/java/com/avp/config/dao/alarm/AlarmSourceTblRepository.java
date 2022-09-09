package com.avp.config.dao.alarm;

import com.avp.config.entity.alarm.AlarmSourceTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface AlarmSourceTblRepository extends JpaRepository<AlarmSourceTbl, Integer>, JpaSpecificationExecutor<AlarmSourceTbl> {
    AlarmSourceTbl save(AlarmSourceTbl alarmSourceTbl);

    public AlarmSourceTbl findBySourceId(Integer sourceId);

    public void deleteBySourceId(Integer sourceId);

    public List<AlarmSourceTbl> findBySourceName(String sourceName);

    public List<AlarmSourceTbl> findBySourceIdOrSourceName(Integer sourceId, String sourceName);
}
