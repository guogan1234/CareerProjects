package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.DevTbl;

import java.util.List;

public interface DevTblRepository extends JpaRepository<DevTbl,String>{
	
    DevTbl save(DevTbl devTbl);

    List<DevTbl> findByDevTpLabel(String devTpLabel);
    
}
