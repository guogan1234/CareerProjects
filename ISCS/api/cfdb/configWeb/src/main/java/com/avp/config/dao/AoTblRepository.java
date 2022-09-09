package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.AoTbl;

public interface AoTblRepository extends JpaRepository<AoTbl, String>{
	
	public void deleteByAoLabel(String aoLabel);
}
