package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.DoTbl;

public interface DoTblRepository extends JpaRepository<DoTbl, String>{
	
	public void deleteByDoLabel(String doLabel);
}
