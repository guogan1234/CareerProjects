package com.avp.config.service;

import com.avp.config.entity.AccTpTbl;

import java.util.List;

public interface AccTpTblService {
    public void save(AccTpTbl accTpTbl);

    public void update(AccTpTbl accTpTbl);

    public void del(String accTpLabel);

    public AccTpTbl getDetail(String accTpLabel);

    public List<AccTpTbl> getList(String devTpLabel);

    public void copy(AccTpTbl accTpTbl);
}
