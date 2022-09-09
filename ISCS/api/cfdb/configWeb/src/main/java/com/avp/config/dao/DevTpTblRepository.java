package com.avp.config.dao;

import com.avp.config.entity.DevTpTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;
import org.springframework.data.jpa.repository.Query;

import java.sql.Timestamp;
import java.util.List;

public interface DevTpTblRepository extends JpaRepository<DevTpTbl,String>,JpaSpecificationExecutor<DevTpTbl> {

    public DevTpTbl findByDevTpLabel(String label);

    public DevTpTbl save(DevTpTbl devTpTbl);

    public List<DevTpTbl> findByProSystemId(int proSystemId);

    public void  deleteByDevTpLabel(String label);
}
