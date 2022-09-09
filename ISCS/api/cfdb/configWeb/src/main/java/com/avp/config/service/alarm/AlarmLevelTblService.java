package com.avp.config.service.alarm;

import com.avp.config.entity.alarm.AlarmLevelTbl;

import java.util.List;

public interface AlarmLevelTblService {

    public List<AlarmLevelTbl> getList(AlarmLevelTbl alarmLevelTbl);

    public AlarmLevelTbl saveOrUpdate(AlarmLevelTbl alarmLevelTbl,String operate) throws Exception;

    public AlarmLevelTbl detail(Integer levelId);

    public void del(Integer levelId);
}
