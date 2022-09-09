package com.avp.config.service.alarm;

import com.avp.config.entity.alarm.AlarmInfoTbl;
import com.avp.config.entity.alarm.vw.AlarmInfoDomainStationRegionTypeLevelVw;

import java.util.List;

public interface AlarmInfoTblService {
    public List<AlarmInfoDomainStationRegionTypeLevelVw> getList(AlarmInfoTbl alarmInfoTbl);

    public AlarmInfoTbl saveOrUpdate(AlarmInfoTbl alarmInfoTbl, String operate) throws Exception;

    public AlarmInfoTbl detail(String label,Integer second,Integer msecond);

    public void del(String label,Integer second,Integer msecond);
}
