package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.AccDefTblEntity;

public interface AccDefTblRepository extends JpaRepository<AccDefTblEntity, String>{

	public void deleteByAccDefLabel(String accDefLabel);
}
