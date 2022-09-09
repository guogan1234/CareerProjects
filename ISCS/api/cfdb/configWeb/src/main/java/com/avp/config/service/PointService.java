package com.avp.config.service;

import java.util.List;

public interface PointService<T> {
    public T save(T t,String type) throws Exception;

    public T update(T t,String type) throws Exception;

    public List<T> getList(String devId);

    public T getDetail(String id,String type);

    public void del(String id) throws Exception;

    public T copy(T t) throws  Exception;
}
