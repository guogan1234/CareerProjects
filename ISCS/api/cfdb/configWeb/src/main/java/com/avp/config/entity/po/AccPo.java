package com.avp.config.entity.po;

import com.avp.config.entity.AccDefTblEntity;
import com.avp.config.entity.AccLimitTblEntity;
import com.avp.config.entity.AccTbl;

public class AccPo {
	private AccTbl accTbl;
	private AccLimitTblEntity accLimitTblEntity;
	private AccDefTblEntity accDefTblEntity;
	public AccTbl getAccTbl() {
		return accTbl;
	}
	public void setAccTbl(AccTbl accTbl) {
		this.accTbl = accTbl;
	}
	public AccLimitTblEntity getAccLimitTblEntity() {
		return accLimitTblEntity;
	}
	public void setAccLimitTblEntity(AccLimitTblEntity accLimitTblEntity) {
		this.accLimitTblEntity = accLimitTblEntity;
	}
	public AccDefTblEntity getAccDefTblEntity() {
		return accDefTblEntity;
	}
	public void setAccDefTblEntity(AccDefTblEntity accDefTblEntity) {
		this.accDefTblEntity = accDefTblEntity;
	}
}
