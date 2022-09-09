package com.avp.config.dao;

import com.avp.config.entity.DioTpTbl;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface DioTpTblRepository extends JpaRepository<DioTpTbl,String> {
    public DioTpTbl findByDioTpLabel(String dioTpLabel);

    public DioTpTbl save(DioTpTbl dioTpTbl);

    public List<DioTpTbl> findByDevTpLabel(String devTpLabel);

    public void deleteByDioTpLabel(String dioTpLabel);

    public void deleteByDevTpLabel(String devTpLabel);
}
