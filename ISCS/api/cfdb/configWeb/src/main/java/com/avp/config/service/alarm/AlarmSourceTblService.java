package com.avp.config.service.alarm;

import com.avp.config.entity.alarm.AlarmSourceTbl;
import com.avp.config.entity.alarm.vw.AlarmSourceActTypeVw;

import java.util.List;

public interface AlarmSourceTblService {
    public List<AlarmSourceActTypeVw> getList(AlarmSourceTbl alarmSourceTbl);

    public AlarmSourceTbl saveOrUpdate(AlarmSourceTbl alarmSourceTbl,String operate) throws Exception;

    public AlarmSourceTbl detail(Integer sourceId);

    public void del(Integer sourceId);
}
