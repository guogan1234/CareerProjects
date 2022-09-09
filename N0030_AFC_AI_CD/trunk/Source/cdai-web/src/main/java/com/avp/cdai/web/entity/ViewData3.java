package com.avp.cdai.web.entity;

import java.util.List;

/**
 * Created by guo on 2017/9/11.
 */
public class ViewData3<T,S> {
    private List<DatePoint<S>> dataList;

    public List<DatePoint<S>> getDataList() {
        return dataList;
    }

    public void setDataList(List<DatePoint<S>> dataList) {
        this.dataList = dataList;
    }
}
