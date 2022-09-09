package com.avp.cdai.web.entity;

import java.util.List;
import java.util.Map;

/**
 * Created by guo on 2017/9/5.
 */
public class AlarmData2<T> {
    private Integer id;
    private Integer state;
    private Map<T,List<Equipment>> equipmentMap;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getState() {
        return state;
    }

    public void setState(Integer state) {
        this.state = state;
    }

    public Map<T, List<Equipment>> getEquipmentMap() {
        return equipmentMap;
    }

    public void setEquipmentMap(Map<T, List<Equipment>> equipmentMap) {
        this.equipmentMap = equipmentMap;
    }
}
