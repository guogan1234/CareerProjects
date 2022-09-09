package com.avp.cdai.web.entity;

import java.util.Date;

/**
 * Created by guo on 2017/9/11.
 */
public class DatePoint<T> {
    private Date date;
    private T value;

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public T getValue() {
        return value;
    }

    public void setValue(T value) {
        this.value = value;
    }

    public DatePoint(Date date, T value) {
        this.date = date;
        this.value = value;
    }
}
