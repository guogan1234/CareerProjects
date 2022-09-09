package com.avp.config.dao.alarm.vw;

import com.avp.config.entity.alarm.AlarmInfoMultiKeys;
import com.avp.config.entity.alarm.vw.AlarmInfoDomainStationRegionTypeLevelVw;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/21.
 */
public interface AlarmInfoDomainStationRegionTypeLevelVwRepository extends JpaRepository<AlarmInfoDomainStationRegionTypeLevelVw, AlarmInfoMultiKeys>, JpaSpecificationExecutor<AlarmInfoDomainStationRegionTypeLevelVw> {
}
