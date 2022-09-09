package com.avp.config.service.base;

import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.system.DomainTbl;

import java.util.List;

public interface DiTextTblService {
    public List<DiTextTbl> getList(DiTextTbl diTextTbl);

    public DiTextTbl saveOrUpdate(DiTextTbl diTextTbl,String operate) throws Exception;

    public DiTextTbl detail(int diValue,String diTextLabel);

    public void del(int diValue,String diTextLabel);
}
