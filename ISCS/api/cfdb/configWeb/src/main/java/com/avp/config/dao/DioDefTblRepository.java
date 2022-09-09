package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.DioDefTblEntity;

public interface DioDefTblRepository extends JpaRepository<DioDefTblEntity, String>{
	
	public void deleteByDioDefLabel(String dioDefLabel);
}
