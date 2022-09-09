package com.avp.cdai.web.communicate;

import com.avp.cdai.web.entity.AlarmContain;
import com.avp.cdai.web.entity.AlarmData;
import com.avp.cdai.web.entity.AlarmData2;
import com.avp.cdai.web.repository.ObjEquipmentRepository;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by guo on 2017/9/4.
 */
public class AlarmDataGetter {
    private List<AlarmData> alarmData = new ArrayList<AlarmData>();

    private AlarmData2<Integer> alarmData2 = new AlarmData2<Integer>();

    @Autowired
    ObjEquipmentRepository objEquipmentRepository;

    public void GetAlarmData(){
        CreateAlarmData();//测试使用
    }
    public void GetAlarmData2(){CreateAlarmData2();}

    private void CreateAlarmData(){
        AlarmContain contain1 = new AlarmContain("SER",1);
        AlarmContain contain2 = new AlarmContain("CS2",2);
        List<AlarmContain> list = new ArrayList<AlarmContain>();
        list.add(contain1);
        list.add(contain2);

        //线路级别报警信息
        AlarmData alarm1 = new AlarmData();
        alarm1.setId(4);
        alarm1.setState(0);
        alarm1.setContains(null);

        //车站级别报警信息
        AlarmData alarm2 = new AlarmData();
        alarm2.setId(5);
        alarm2.setState(1);
        alarm2.setContains(list);

        AlarmData alarm3 = new AlarmData();
        alarm3.setId(6);
        alarm3.setState(2);
        alarm3.setContains(list);

        alarmData.add(alarm1);
        alarmData.add(alarm2);
        alarmData.add(alarm3);
    }
    private void CreateAlarmData2(){

    }
    public List<AlarmData> getAlarmData(){
        return alarmData;
    }

}
