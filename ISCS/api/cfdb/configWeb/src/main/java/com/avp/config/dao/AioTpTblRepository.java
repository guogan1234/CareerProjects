package com.avp.config.dao;

import com.avp.config.entity.AioTpTbl;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface AioTpTblRepository extends JpaRepository<AioTpTbl,String> {
    public AioTpTbl findByAioTpLabel(String aioTpLabel);

    public AioTpTbl save(AioTpTbl aioTpTbl);
    
    public List<AioTpTbl> findByDevTpLabel(String devTpLabel);

    public void deleteByAioTpLabel(String aioTpLabel);

    public void deleteByDevTpLabel(String devTpLabel);
}
