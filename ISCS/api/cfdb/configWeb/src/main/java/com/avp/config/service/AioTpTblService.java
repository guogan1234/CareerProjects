package com.avp.config.service;
import com.avp.config.entity.AioTpTbl;
import java.util.List;

public interface AioTpTblService {
    public void save(AioTpTbl aioTpTbl);

    public void update(AioTpTbl aioTpTbl);

    public void del(String aioTpLabel);

    public AioTpTbl getDetail(String aioTpLabel);

    public List<AioTpTbl> getList(String devTpLabel);

    public void copy(AioTpTbl aioTpTbl);
}
