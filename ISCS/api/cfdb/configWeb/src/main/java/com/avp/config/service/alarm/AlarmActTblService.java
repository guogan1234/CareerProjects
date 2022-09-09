package com.avp.config.service.alarm;
import com.avp.config.entity.alarm.AlarmActTbl;
import java.util.List;

public interface AlarmActTblService {

    public List<AlarmActTbl> getList(AlarmActTbl alarmActTbl);

    public AlarmActTbl saveOrUpdate(AlarmActTbl alarmActTbl,String operate) throws Exception;

    public AlarmActTbl detail(Integer actId);

    public void del(Integer actId);

}
