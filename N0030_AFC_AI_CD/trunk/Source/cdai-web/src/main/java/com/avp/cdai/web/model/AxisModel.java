package com.avp.cdai.web.model;

import java.util.List;

/**
 * Created by guo on 2017/10/25.
 */
public class AxisModel<T,S> {
    private List<T> xValue;
    private List<S> yValue;

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
