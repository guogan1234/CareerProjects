package com.avp.configService.test.TableToModel;

/**
 * Created by guo on 2018/4/27.
 */
public class ObjectModel<T> {
    int type;
    T value;

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public T getValue() {
        return value;
    }

    public void setValue(T value) {
        this.value = value;
    }
}
