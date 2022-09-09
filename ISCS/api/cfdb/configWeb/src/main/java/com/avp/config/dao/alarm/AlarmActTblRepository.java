package com.avp.config.dao.alarm;

import com.avp.config.entity.alarm.AlarmActTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface AlarmActTblRepository extends JpaRepository<AlarmActTbl, Integer>, JpaSpecificationExecutor<AlarmActTbl> {
    AlarmActTbl save(AlarmActTbl alarmActTbl);

    public  AlarmActTbl findByActId(Integer actId);

    public void deleteByActId(Integer actId);

    public List<AlarmActTbl> findByActName(String actName);

    public List<AlarmActTbl>  findByActIdOrActName(Integer actId,String actName);
}
