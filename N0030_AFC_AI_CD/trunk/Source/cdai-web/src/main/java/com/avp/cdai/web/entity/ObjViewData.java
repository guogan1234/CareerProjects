package com.avp.cdai.web.entity;

import java.util.Map;

/**
 * Created by guo on 2017/9/11.
 */
public class ObjViewData<T> {
    private Map<Integer,T> objMap;

    public Map<Integer, T> getObjMap() {
        return objMap;
    }

    public void setObjMap(Map<Integer, T> objMap) {
        this.objMap = objMap;
    }
}
