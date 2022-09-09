package com.avp.cdai.web.model;

import java.util.List;

/**
 * Created by guo on 2017/9/14.
 */
public class FailedModel<T,S> {
    private List<T> xValue;
    private List<S> yValue;
    private List<Integer> idList;

    public List<Integer> getIdList() {
        return idList;
    }

    public void setIdList(List<Integer> idList) {
        this.idList = idList;
    }

    public List<T> getxValue() {
        return xValue;
    }

    public void setxValue(List<T> xValue) {
        this.xValue = xValue;
    }

    public List<S> getyValue() {
        return yValue;
    }

    public void setyValue(List<S> yValue) {
        this.yValue = yValue;
    }
}
