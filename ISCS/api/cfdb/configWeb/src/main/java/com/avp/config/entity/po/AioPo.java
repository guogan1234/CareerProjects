package com.avp.config.entity.po;

import com.avp.config.entity.AiLimitTblEntity;
import com.avp.config.entity.AiTblEntity;
import com.avp.config.entity.AioDefTblEntity;
import com.avp.config.entity.AoTbl;

public class AioPo {
	private AoTbl aoTbl;
	private AiTblEntity aiTblEntity;
	private AioDefTblEntity aioDefTblEntity;
	private AiLimitTblEntity aiLimitTblEntity;
	
	public AoTbl getAoTbl() {
		return aoTbl;
	}
	public void setAoTbl(AoTbl aoTbl) {
		this.aoTbl = aoTbl;
	}
	public AiTblEntity getAiTblEntity() {
		return aiTblEntity;
	}
	public void setAiTblEntity(AiTblEntity aiTblEntity) {
		this.aiTblEntity = aiTblEntity;
	}
	public AioDefTblEntity getAioDefTblEntity() {
		return aioDefTblEntity;
	}
	public void setAioDefTblEntity(AioDefTblEntity aioDefTblEntity) {
		this.aioDefTblEntity = aioDefTblEntity;
	}
	public AiLimitTblEntity getAiLimitTblEntity() {
		return aiLimitTblEntity;
	}
	public void setAiLimitTblEntity(AiLimitTblEntity aiLimitTblEntity) {
		this.aiLimitTblEntity = aiLimitTblEntity;
	}
}
