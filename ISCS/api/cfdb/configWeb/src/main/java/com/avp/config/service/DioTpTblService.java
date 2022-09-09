package com.avp.config.service;

import com.avp.config.entity.DioTpTbl;

import java.util.List;

public interface DioTpTblService {
    public DioTpTbl save(DioTpTbl dioTpTbl);

    public void update(DioTpTbl dioTpTbl);

    public void del(String dioTpLabel);

    public DioTpTbl getDetail(String dioTpLabel);

    public List<DioTpTbl> getList(String devTpLabel);

    public void copy(DioTpTbl dioTpTbl);
}
