package com.avp.config.dao;

import com.avp.config.entity.AccTpTbl;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface AccTpTblRepository extends JpaRepository<AccTpTbl,String>{
    public AccTpTbl findByAccTpLabel(String accTpLabel);

    public AccTpTbl save(AccTpTbl accTpTbl);

    public List<AccTpTbl> findByDevTpLabel(String devTpLabel);

    public void deleteByAccTpLabel(String accTpLabel);

    public void deleteByDevTpLabel(String devTpLabel);
}
