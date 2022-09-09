package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.DiTbl;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;

public interface DiTblRepository extends JpaRepository<DiTbl, String>{
	
	public void deleteByDiLabel(String diLabel);

	@Modifying
	@Query("update DiTbl ac set ac.diName=?1 where ac.pointTpLabel=?2")
	public void updateDiNameByLabel(String diName, String pointTpLabel);
}
