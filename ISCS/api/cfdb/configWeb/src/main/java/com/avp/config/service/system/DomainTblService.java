package com.avp.config.service.system;

import com.avp.config.entity.system.DomainTbl;

import java.util.List;

public interface DomainTblService {

    public List<DomainTbl> getList(DomainTbl domainTbl);

    public DomainTbl saveOrUpdate(DomainTbl domainTbl,String operate) throws Exception;

    public DomainTbl detail(int domainId);

    public void del(int domainId);
}
