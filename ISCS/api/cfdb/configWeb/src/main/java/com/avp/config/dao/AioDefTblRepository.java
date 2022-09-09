package com.avp.config.dao;

import org.springframework.data.jpa.repository.JpaRepository;

import com.avp.config.entity.AioDefTblEntity;

public interface AioDefTblRepository extends JpaRepository<AioDefTblEntity, String>{

	public void deleteByAioDefLabel(String aioDefLabel);
}
