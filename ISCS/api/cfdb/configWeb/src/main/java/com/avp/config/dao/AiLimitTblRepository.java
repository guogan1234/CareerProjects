package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.AiLimitTblEntity;

public interface AiLimitTblRepository extends JpaRepository<AiLimitTblEntity, String>{

	public void deleteByAiLimitLabel(String aiLimitLabel);
}
