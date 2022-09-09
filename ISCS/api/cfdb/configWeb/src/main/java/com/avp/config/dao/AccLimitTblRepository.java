package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.AccLimitTblEntity;

public interface AccLimitTblRepository extends JpaRepository<AccLimitTblEntity, String>{

	public void deleteByAccLimitLabel(String accLimitLabel);
}
