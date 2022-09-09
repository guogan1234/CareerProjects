package com.avp.cdai.web.entity;

import java.util.List;
import java.util.Map;

/**
 * Created by guo on 2017/9/4.
 */
public class EquipmentData<T> {
    private Map<T,List<ObjEquipment>> equipmentMap;

    public Map<T, List<ObjEquipment>> getEquipmentMap() {
        return equipmentMap;
    }

    public void setEquipmentMap(Map<T, List<ObjEquipment>> equipmentMap) {
        this.equipmentMap = equipmentMap;
    }
}
