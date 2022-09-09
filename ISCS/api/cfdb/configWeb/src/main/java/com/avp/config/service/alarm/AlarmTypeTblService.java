package com.avp.config.service.alarm;

import com.avp.config.entity.alarm.AlarmSourceTbl;
import com.avp.config.entity.alarm.AlarmTypeTbl;

import java.util.List;

public interface AlarmTypeTblService {

    public List<AlarmTypeTbl> getList(AlarmTypeTbl alarmTypeTbl);

    public AlarmTypeTbl saveOrUpdate(AlarmTypeTbl alarmTypeTbl,String operate) throws Exception;

    public AlarmTypeTbl detail(Integer typeId);

    public void del(Integer typeId);
}
