package com.avp.config.dao.alarm;

import com.avp.config.entity.alarm.AlarmInfoMultiKeys;
import com.avp.config.entity.alarm.AlarmInfoTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/14.
 */
public interface AlarmInfoTblRepository extends JpaRepository<AlarmInfoTbl, AlarmInfoMultiKeys>, JpaSpecificationExecutor<AlarmInfoTbl> {

    public AlarmInfoTbl save(AlarmInfoTbl alarmInfoTbl);

    public AlarmInfoTbl findByLabelAndSecondAndMsecond(String label, Integer second, Integer msecond);

    public void deleteByLabelAndSecondAndMsecond(String label, Integer second, Integer msecond);
}
