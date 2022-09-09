package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.AccTbl;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

public interface AccTblRepository extends JpaRepository<AccTbl, String>{
	
    public void deleteByDevLabel(String devLabel);
    
	public void deleteByAccLabel(String accLabel);

    @Modifying
    @Query("update AccTbl ac set ac.accName=?1 where ac.pointTpLabel=?2")
	public void updateAccNameByLabel(String accName, String pointTpLabel);
}
