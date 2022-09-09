package com.avp.cdai.web.entity;

import java.util.List;
import java.util.Map;

/**
 * Created by guo on 2017/9/11.
 */
public class ViewDataModel3<T,S> {
    Map<Integer,List<DatePoint<S>>> data3Map;
    Map<Integer,T> objMap;

    public Map<Integer, List<DatePoint<S>>> getData3Map() {
        return data3Map;
    }

    public void setData3Map(Map<Integer, List<DatePoint<S>>> data3Map) {
        this.data3Map = data3Map;
    }

    public Map<Integer, T> getObjMap() {
        return objMap;
    }

    public void setObjMap(Map<Integer, T> objMap) {
        this.objMap = objMap;
    }
}
